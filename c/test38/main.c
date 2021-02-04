#include "coroutine.h"
#include <stdio.h>

struct args {
	int n;
};

static void
foo(struct schedule * S, void *ud) {
	struct args * arg = ud;				// 强制类型转换
	int start = arg->n;

	int i;
	for (i=0;i<5;i++) {
		printf("coroutine %d : %d\n", coroutine_running(S), start + i);
		coroutine_yield(S);													// 协程退出/释放控制权/协程调度/切换协程
	}
}

static void
test(struct schedule *S) {
	struct args arg1 = { 0 };
	struct args arg2 = { 100 };

	// 创建协程实例
	int co1 = coroutine_new(S, foo, &arg1);
	int co2 = coroutine_new(S, foo, &arg2);

	printf("main start\n");

	while (coroutine_status(S,co1) && coroutine_status(S,co2)) {		// 协程都结束时停止
		// 执行协程逻辑，即 foo 函数
		coroutine_resume(S,co1);
		coroutine_resume(S,co2);
	} 

	printf("main end\n");
}

int 
main() {
	struct schedule * S = coroutine_open();				// 创建协程调度器
	test(S);											// 测试协程
	coroutine_close(S);									// 销毁协程调度器
	return 0;
}

