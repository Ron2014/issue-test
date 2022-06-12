#ifndef C_COROUTINE_H
#define C_COROUTINE_H

// 协程实例在运行时的状态
#define COROUTINE_DEAD 0                // 已执行结束
#define COROUTINE_READY 1               // 刚创建，等待执行
#define COROUTINE_RUNNING 2             // 执行中
#define COROUTINE_SUSPEND 3             // 挂起

struct schedule;

typedef void (*coroutine_func)(struct schedule *, void *ud);        // 函数原型，协程其实时对它的一层 wrapper

struct schedule * coroutine_open(void);
void coroutine_close(struct schedule *);

int coroutine_new(struct schedule *, coroutine_func, void *ud);
void coroutine_resume(struct schedule *, int id);
int coroutine_status(struct schedule *, int id);
int coroutine_running(struct schedule *);
void coroutine_yield(struct schedule *);

#endif
