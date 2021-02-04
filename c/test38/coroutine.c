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

struct coroutine;

// 协程调度器
struct schedule {
	char stack[STACK_SIZE];
	ucontext_t main;

	int running;					// 当前在运行的协程ID，若-1表示无/主线程

	int nco;						// 协程实例的数量
	int cap;						// 协程指针数组的容量
	struct coroutine **co;			// 协程指针数组：指针的内存要 malloc（动态加），协程实例的内存也要 malloc
	// 一个协程池的简单实现
};

/***********************************************************
 * 协程调度器的创建和销毁
 */
struct schedule * 
coroutine_open(void) {
	struct schedule *S = malloc(sizeof(*S));

	// 没有执行中的协程
	S->running = -1;

	// 还没有创建协程
	S->nco = 0;

	// 初始化指针数量
	// 指针（内存地址）都初始化为0
	S->cap = DEFAULT_COROUTINE;
	S->co = malloc(sizeof(struct coroutine *) * S->cap);
	memset(S->co, 0, sizeof(struct coroutine *) * S->cap);

	return S;
}

void 
coroutine_close(struct schedule *S) {
	int i;

	// 销毁所有协程实例
	for (i=0;i<S->cap;i++) {
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
	co->status = COROUTINE_READY;

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
		// 协程池满了。不存在空闲槽位（指针）了

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
			int id = (i+S->nco) % S->cap;			// +S->nco 是为了更快遍历到空闲的槽位
			// int id = i;

			if (S->co[id] == NULL) {
				S->co[id] = co;
				++S->nco;
				return id;
			}
		}
	}
*/

	if (S->nco < S->cap) {
		int i;
		for (i=0;i<S->cap;i++) {
			int id = i;
			if (S->co[i] == NULL) {
				S->co[id] = co;
				++S->nco;
				return id;
			}
		}

	} else {
		// 协程池满了。不存在空闲槽位（指针）了
		int id = S->nco;

		// 双倍扩容指针数组，且给新增的部分初始化为0
		S->co = realloc(S->co, S->cap * 2 * sizeof(struct coroutine *));
		memset(S->co + S->cap , 0 , sizeof(struct coroutine *) * S->cap);
		S->cap *= 2;

		S->co[S->nco++l = co;

		return id;

	}

	assert(0);
	return -1;
}

static void
mainfunc(uint32_t low32, uint32_t hi32) {
	uintptr_t ptr = (uintptr_t)low32 | ((uintptr_t)hi32 << 32);
	struct schedule *S = (struct schedule *)ptr;
	int id = S->running;
	struct coroutine *C = S->co[id];
	C->func(S,C->ud);
	_co_delete(C);
	S->co[id] = NULL;
	--S->nco;
	S->running = -1;
}

void 
coroutine_resume(struct schedule * S, int id) {
	assert(S->running == -1);
	assert(id >=0 && id < S->cap);
	struct coroutine *C = S->co[id];
	if (C == NULL)
		return;
	int status = C->status;
	switch(status) {
	case COROUTINE_READY:
		getcontext(&C->ctx);
		C->ctx.uc_stack.ss_sp = S->stack;
		C->ctx.uc_stack.ss_size = STACK_SIZE;
		C->ctx.uc_link = &S->main;
		S->running = id;
		C->status = COROUTINE_RUNNING;
		uintptr_t ptr = (uintptr_t)S;
		makecontext(&C->ctx, (void (*)(void)) mainfunc, 2, (uint32_t)ptr, (uint32_t)(ptr>>32));
		swapcontext(&S->main, &C->ctx);
		break;
	case COROUTINE_SUSPEND:
		memcpy(S->stack + STACK_SIZE - C->size, C->stack, C->size);
		S->running = id;
		C->status = COROUTINE_RUNNING;
		swapcontext(&S->main, &C->ctx);
		break;
	default:
		assert(0);
	}
}

static void
_save_stack(struct coroutine *C, char *top) {
	char dummy = 0;
	assert(top - &dummy <= STACK_SIZE);
	if (C->cap < top - &dummy) {
		free(C->stack);
		C->cap = top-&dummy;
		C->stack = malloc(C->cap);
	}
	C->size = top - &dummy;
	memcpy(C->stack, &dummy, C->size);
}

void
coroutine_yield(struct schedule * S) {
	int id = S->running;
	assert(id >= 0);
	struct coroutine * C = S->co[id];
	assert((char *)&C > S->stack);
	_save_stack(C,S->stack + STACK_SIZE);
	C->status = COROUTINE_SUSPEND;
	S->running = -1;
	swapcontext(&C->ctx , &S->main);
}

int 
coroutine_status(struct schedule * S, int id) {
	assert(id>=0 && id < S->cap);
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

