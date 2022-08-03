#include "coroutine.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

#if __APPLE__ && __MACH__
    #include <sys/ucontext.h>
#else 
    #include <ucontext.h>
#endif 

#define STACK_SIZE (1024*1024)		// 1048576
#define DEFAULT_COROUTINE 16		// 初始的协程容量

/**********************************************************

有栈协程的原理
一个程序要真正运行起来，需要两个因素：可执行代码段、数据。体现在 CPU 中，主要包含以下几个方面：

EIP 寄存器：用来存储 CPU 要读取指令的地址
ESP 寄存器：指向当前线程栈的栈顶位置
其他通用寄存器的内容：包括代表函数参数的 rdi、rsi 等等。
线程栈中的内存内容。

这些数据内容，我们一般将其称为 “上下文” 或者 “现场”。

在这段程序中，表现为 ucontext_t 对象。

**********************************************************/

struct coroutine;

// 协程调度器
struct schedule {
    char stack[STACK_SIZE];
    ucontext_t main;

    int running;					// 当前在运行的协程ID，若-1表示无/主线程

    int nco;						// 协程实例的数量
    int cap;						// 协程指针数组的容量
    struct coroutine **co;			// 协程指针数组：指针的内存要 malloc（动态加），协程实例的内存也要 malloc
    // 一个协程map的简单实现
};

/***********************************************************
 * 协程调度器的创建和销毁
 */
struct schedule * 
coroutine_open(void) {
    struct schedule *S = malloc(sizeof(*S));

    // 没有执行中的协程
    S->running = -1;

    // 还没有协程实例
    S->nco = 0;

    // 初始化指针数量
    // 指针（内存地址）都初始化为0
    // 注意只是给指针申请空间
    S->cap = DEFAULT_COROUTINE;
    S->co = malloc(sizeof(struct coroutine *) * S->cap);
    memset(S->co, 0, sizeof(struct coroutine *) * S->cap);

    return S;
}

void 
coroutine_close(struct schedule *S) {
    int i;

    // 销毁所有协程实例
    for (i = 0; i < S->cap; i++) {
        struct coroutine * co = S->co[i];
        if (co) {
            _co_delete(co);
        }
    }

    // 释放指针数组
    free(S->co);
    S->co = NULL;

    // 释放协程调度器
    free(S);
}
/***********************************************************/

// 协程
struct coroutine {
    coroutine_func func;		// 包装的函数
    void *ud;					// 函数参数
    struct schedule * sch;		// 调度器，协程实例被它管理
    int status;					// 运行时状态

    ucontext_t ctx;
    ptrdiff_t cap;
    ptrdiff_t size;
    char *stack;
};


struct coroutine * 
_co_new(struct schedule *S , coroutine_func func, void *ud) {
    struct coroutine * co = malloc(sizeof(*co));

    co->func = func;
    co->ud = ud;
    co->sch = S;
    co->status = COROUTINE_READY;		// 初始化结束，即准备好了

    co->cap = 0;
    co->size = 0;
    co->stack = NULL;

    return co;
}

void
_co_delete(struct coroutine *co) {
    free(co->stack);
    free(co);
}

int 
coroutine_new(struct schedule *S, coroutine_func func, void *ud) {
    struct coroutine *co = _co_new(S, func , ud);

/**
    if (S->nco >= S->cap) {
        // 协程map满了。不存在空闲槽位（指针）了

        int id = S->cap;

        // 双倍扩容指针数组，且给新增的部分初始化为0
        S->co = realloc(S->co, S->cap * 2 * sizeof(struct coroutine *));
        memset(S->co + S->cap , 0 , sizeof(struct coroutine *) * S->cap);

        S->co[S->cap] = co;
        S->cap *= 2;
        ++S->nco;

        return id;

    } else {

        int i;
        for (i=0;i<S->cap;i++) {
            int id = (i+S->nco) % S->cap;			// +S->nco 是为了提高命中率
            // int id = i;

            if (S->co[id] == NULL) {
                S->co[id] = co;
                ++S->nco;
                return id;
            }
        }
    }
*/


    // 这里用自己的书写习惯做一次等价的改动

    if (S->nco < S->cap) {
        // 协程map未用完。找到一个空闲的槽位
        // 注意 S->co 中的元素是离散的

        int i, id = -1;
        for (i = 0; i < S->cap; i++) {
            if (S->co[i] == NULL) {
                id = i;
                break;
            }
        }

        assert(id >= 0);

        S->co[id] = co;
        ++S->nco;

        return id;

    } else {
        // 协程map满了。不存在空闲槽位（指针）了

        // 双倍扩容指针数组，且给新增的部分初始化为0
        S->co = realloc(S->co, S->cap * 2 * sizeof(struct coroutine *));
        memset(S->co + S->cap , 0 , sizeof(struct coroutine *) * S->cap);
        S->cap *= 2;

        // 将创建的协程放入槽位中
        // 表明 S->co 中都是有效的协程，不会缓存已经结束的协程
        // 严格来讲这不算是协程池
        // 只是一个字典

        int id = S->nco;
        S->co[S->nco++] = co;

        return id;
    }

    assert(0);
    return -1;
}

/********************************************************************
 * mainfunc ==> C->func
 * 
 * 其实是包了两层
 * 在独立内存空间执行的函数是 mainfunc
 * 主要逻辑实现是 C->func
 * 
 * 即
 * mainfunc 用来做协程的调度：协程结束的处理
 * C->func 处理业务逻辑 
 *******************************************************************/
static void
mainfunc(uint32_t low32, uint32_t hi32) {
    uintptr_t ptr = (uintptr_t)low32 | ((uintptr_t)hi32 << 32);
    struct schedule *S = (struct schedule *)ptr;

    int id = S->running;
    struct coroutine *C = S->co[id];

    C->func(S, C->ud);

    // 此处可以扩展：
    // 结束的协程不急着销毁
    // 而是缓存下来，作为对象池存在
    _co_delete(C);
    S->co[id] = NULL;
    --S->nco;

    S->running = -1;
}

/*********************************************************************
 * ucontext 相关API
 * getcontext
 * makecontext
 * swapcontext
 * 
 * 协程的运行时栈的内存空间是自己分配的
 * 即，我们要做到，子协程的运行空间就是调度器栈 S->stack
 * 
 * 运行空间 == 上下文 == 函数执行栈
 */
void 
coroutine_resume(struct schedule * S, int id) {
    assert(S->running == -1);				// 没有正在执行的协程。前面的协程得先退出来
    
    // 有效的协程ID
    assert(id >= 0 && id < S->cap);
    struct coroutine *C = S->co[id];
    if (C == NULL)
        return;

    int status = C->status;

    switch(status) {
    case COROUTINE_READY:

        // C->ctx 保存的是主协程的上下文
        getcontext(&C->ctx);

        // 准备创建新的上下文
        // 这里做一些配置：
        // 把调度器栈 S->stack 内存空间分配给新的上下文
        C->ctx.uc_stack.ss_sp = S->stack;
        C->ctx.uc_stack.ss_size = STACK_SIZE;

        // 如果协程执行完，则切换到 S->main 主协程中进行执行。如果不设置, 则默认为 NULL，那么协程执行完，整个程序就结束了。
        C->ctx.uc_link = &S->main;

        uintptr_t ptr = (uintptr_t)S;

        // 指定函数，创建新的上下文
        makecontext(&C->ctx, (void (*)(void)) mainfunc, 2, (uint32_t)ptr, (uint32_t)(ptr>>32));

        S->running = id;
        C->status = COROUTINE_RUNNING;
        
        // 当前上下文放入 S->main 中
        // C->ctx 的上下文替换到当前上下文
        swapcontext(&S->main, &C->ctx);
        break;

    case COROUTINE_SUSPEND:
        // 子协程执行之前，将他的状态保存到调度器栈的尾部
        memcpy(S->stack + STACK_SIZE - C->size, C->stack, C->size);

        S->running = id;
        C->status = COROUTINE_RUNNING;

        swapcontext(&S->main, &C->ctx);
        break;

    default:
        assert(0);
    }
}

/*************************************************************
 * 子协程退出前，计算代码段到调度栈末尾的距离
 * 将这段距离的所有内存存档至子协程栈
 * 
 * 这里保存的是栈底到栈顶的数据：已经执行的部分
 *************************************************************/
static void
_save_stack(struct coroutine *C, char *top) {
    // 利用一个char得到代码段执行到哪里了
    char dummy = 0;
    ptrdiff_t csize = top - &dummy;		// 栈的生长方向是从高地址往低地址
    assert(csize <= STACK_SIZE);

    // 只有第一次 yield 存档的时候
    // 才会出现 C->cap（初始值为0）小于 csize 的情况
    if (C->cap < csize) {
        free(C->stack);
        C->cap = csize;
        C->stack = malloc(C->cap);
    }

    // C->stack 保存的是当前协程的上下文
    C->size = csize;
    memcpy(C->stack, &dummy, C->size);
}

void
coroutine_yield(struct schedule * S) {
    int id = S->running;
    assert(id >= 0);

    struct coroutine * C = S->co[id];
    assert((char *)&C > S->stack);

    _save_stack(C, S->stack + STACK_SIZE);

    C->status = COROUTINE_SUSPEND;
    S->running = -1;

    swapcontext(&C->ctx , &S->main);
}

// 协程结束状态是没有保存的，直接通过 NULL 判定确定
// 只是这种做法有歧义：有可能就是协程不存在而已 
int 
coroutine_status(struct schedule * S, int id) {
    assert(id >= 0 && id < S->cap);
    if (S->co[id] == NULL) {
        return COROUTINE_DEAD;
    }
    return S->co[id]->status;
}

// 返回当前在执行的协程ID
int 
coroutine_running(struct schedule * S) {
    return S->running;
}

