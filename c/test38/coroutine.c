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
#define DEFAULT_COROUTINE 16		// ��ʼ��Э������

struct coroutine;

// Э�̵�����
struct schedule {
	char stack[STACK_SIZE];
	ucontext_t main;

	int running;					// ��ǰ�����е�Э��ID����-1��ʾ��/���߳�

	int nco;						// Э��ʵ��������
	int cap;						// Э��ָ�����������
	struct coroutine **co;			// Э��ָ�����飺ָ����ڴ�Ҫ malloc����̬�ӣ���Э��ʵ�����ڴ�ҲҪ malloc
	// һ��Э�̳صļ�ʵ��
};

/***********************************************************
 * Э�̵������Ĵ���������
 */
struct schedule * 
coroutine_open(void) {
	struct schedule *S = malloc(sizeof(*S));

	// û��ִ���е�Э��
	S->running = -1;

	// ��û�д���Э��
	S->nco = 0;

	// ��ʼ��ָ������
	// ָ�루�ڴ��ַ������ʼ��Ϊ0
	S->cap = DEFAULT_COROUTINE;
	S->co = malloc(sizeof(struct coroutine *) * S->cap);
	memset(S->co, 0, sizeof(struct coroutine *) * S->cap);

	return S;
}

void 
coroutine_close(struct schedule *S) {
	int i;

	// ��������Э��ʵ��
	for (i=0;i<S->cap;i++) {
		struct coroutine * co = S->co[i];
		if (co) {
			_co_delete(co);
		}
	}

	// �ͷ�ָ������
	free(S->co);
	S->co = NULL;

	// �ͷ�Э�̵�����
	free(S);
}
/***********************************************************/

// Э��
struct coroutine {
	coroutine_func func;		// ��װ�ĺ���
	void *ud;					// ��������
	struct schedule * sch;		// ��������Э��ʵ����������
	int status;					// ����ʱ״̬

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
		// Э�̳����ˡ������ڿ��в�λ��ָ�룩��

		int id = S->cap;

		// ˫������ָ�����飬�Ҹ������Ĳ��ֳ�ʼ��Ϊ0
		S->co = realloc(S->co, S->cap * 2 * sizeof(struct coroutine *));
		memset(S->co + S->cap , 0 , sizeof(struct coroutine *) * S->cap);

		S->co[S->cap] = co;
		S->cap *= 2;
		++S->nco;

		return id;

	} else {

		int i;
		for (i=0;i<S->cap;i++) {
			int id = (i+S->nco) % S->cap;			// +S->nco ��Ϊ�˸�����������еĲ�λ
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
		// Э�̳����ˡ������ڿ��в�λ��ָ�룩��
		int id = S->nco;

		// ˫������ָ�����飬�Ҹ������Ĳ��ֳ�ʼ��Ϊ0
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

// ���ص�ǰ��ִ�е�Э��ID
int 
coroutine_running(struct schedule * S) {
	return S->running;
}

