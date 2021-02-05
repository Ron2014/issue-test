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

/**********************************************************

��ջЭ�̵�ԭ��
һ������Ҫ����������������Ҫ�������أ���ִ�д���Ρ����ݡ������� CPU �У���Ҫ�������¼������棺

EIP �Ĵ����������洢 CPU Ҫ��ȡָ��ĵ�ַ
ESP �Ĵ�����ָ��ǰ�߳�ջ��ջ��λ��
����ͨ�üĴ��������ݣ����������������� rdi��rsi �ȵȡ�
�߳�ջ�е��ڴ����ݡ�

��Щ�������ݣ�����һ�㽫���Ϊ �������ġ� ���� ���ֳ�����

����γ����У�����Ϊ ucontext_t ����

**********************************************************/

struct coroutine;

// Э�̵�����
struct schedule {
	char stack[STACK_SIZE];
	ucontext_t main;

	int running;					// ��ǰ�����е�Э��ID����-1��ʾ��/���߳�

	int nco;						// Э��ʵ��������
	int cap;						// Э��ָ�����������
	struct coroutine **co;			// Э��ָ�����飺ָ����ڴ�Ҫ malloc����̬�ӣ���Э��ʵ�����ڴ�ҲҪ malloc
	// һ��Э��map�ļ�ʵ��
};

/***********************************************************
 * Э�̵������Ĵ���������
 */
struct schedule * 
coroutine_open(void) {
	struct schedule *S = malloc(sizeof(*S));

	// û��ִ���е�Э��
	S->running = -1;

	// ��û��Э��ʵ��
	S->nco = 0;

	// ��ʼ��ָ������
	// ָ�루�ڴ��ַ������ʼ��Ϊ0
	// ע��ֻ�Ǹ�ָ������ռ�
	S->cap = DEFAULT_COROUTINE;
	S->co = malloc(sizeof(struct coroutine *) * S->cap);
	memset(S->co, 0, sizeof(struct coroutine *) * S->cap);

	return S;
}

void 
coroutine_close(struct schedule *S) {
	int i;

	// ��������Э��ʵ��
	for (i = 0; i < S->cap; i++) {
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
	co->status = COROUTINE_READY;		// ��ʼ����������׼������

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
		// Э��map���ˡ������ڿ��в�λ��ָ�룩��

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
			int id = (i+S->nco) % S->cap;			// +S->nco ��Ϊ�����������
			// int id = i;

			if (S->co[id] == NULL) {
				S->co[id] = co;
				++S->nco;
				return id;
			}
		}
	}
*/


	// �������Լ�����дϰ����һ�εȼ۵ĸĶ�

	if (S->nco < S->cap) {
		// Э��mapδ���ꡣ�ҵ�һ�����еĲ�λ
		// ע�� S->co �е�Ԫ������ɢ��

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
		// Э��map���ˡ������ڿ��в�λ��ָ�룩��

		// ˫������ָ�����飬�Ҹ������Ĳ��ֳ�ʼ��Ϊ0
		S->co = realloc(S->co, S->cap * 2 * sizeof(struct coroutine *));
		memset(S->co + S->cap , 0 , sizeof(struct coroutine *) * S->cap);
		S->cap *= 2;

		// ��������Э�̷����λ��
		// ���� S->co �ж�����Ч��Э�̣����Ỻ���Ѿ�������Э��
		// �ϸ������ⲻ����Э�̳�
		// ֻ��һ���ֵ�

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
 * ��ʵ�ǰ�������
 * �ڶ����ڴ�ռ�ִ�еĺ����� mainfunc
 * ��Ҫ�߼�ʵ���� C->func
 * 
 * ��
 * mainfunc ������Э�̵ĵ��ȣ�Э�̽����Ĵ���
 * C->func ����ҵ���߼� 
 *******************************************************************/
static void
mainfunc(uint32_t low32, uint32_t hi32) {
	uintptr_t ptr = (uintptr_t)low32 | ((uintptr_t)hi32 << 32);
	struct schedule *S = (struct schedule *)ptr;

	int id = S->running;
	struct coroutine *C = S->co[id];

	C->func(S, C->ud);

	// �˴�������չ��
	// ������Э�̲���������
	// ���ǻ�����������Ϊ����ش���
	_co_delete(C);
	S->co[id] = NULL;
	--S->nco;

	S->running = -1;
}

/*********************************************************************
 * ucontext ���API
 * getcontext
 * makecontext
 * swapcontext
 * 
 * Э�̵�����ʱջ���ڴ�ռ����Լ������
 * ��������Ҫ��������Э�̵����пռ���ǵ�����ջ S->stack
 * 
 * ���пռ� == ������ == ����ִ��ջ
 */
void 
coroutine_resume(struct schedule * S, int id) {
	assert(S->running == -1);				// û������ִ�е�Э�̡�ǰ���Э�̵����˳���
	
	// ��Ч��Э��ID
	assert(id >= 0 && id < S->cap);
	struct coroutine *C = S->co[id];
	if (C == NULL)
		return;

	int status = C->status;

	switch(status) {
	case COROUTINE_READY:

		// C->ctx ���������Э�̵�������
		getcontext(&C->ctx);

		// ׼�������µ�������
		// ������һЩ���ã�
		// �ѵ�����ջ S->stack �ڴ�ռ������µ�������
		C->ctx.uc_stack.ss_sp = S->stack;
		C->ctx.uc_stack.ss_size = STACK_SIZE;

		// ���Э��ִ���꣬���л��� S->main ��Э���н���ִ�С����������, ��Ĭ��Ϊ NULL����ôЭ��ִ���꣬��������ͽ����ˡ�
		C->ctx.uc_link = &S->main;

		uintptr_t ptr = (uintptr_t)S;

		// ָ�������������µ�������
		makecontext(&C->ctx, (void (*)(void)) mainfunc, 2, (uint32_t)ptr, (uint32_t)(ptr>>32));

		S->running = id;
		C->status = COROUTINE_RUNNING;
		
		// ��ǰ�����ķ��� S->main ��
		// C->ctx ���������滻����ǰ������
		swapcontext(&S->main, &C->ctx);
		break;

	case COROUTINE_SUSPEND:
		// ��Э��ִ��֮ǰ��������״̬���浽������ջ��β��
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
 * ��Э���˳�ǰ���������ε�����ջĩβ�ľ���
 * ����ξ���������ڴ�浵����Э��ջ
 * 
 * ���ﱣ�����ջ�׵�ջ�������ݣ��Ѿ�ִ�еĲ���
 *************************************************************/
static void
_save_stack(struct coroutine *C, char *top) {
	// ����һ��char�õ������ִ�е�������
	char dummy = 0;
	ptrdiff_t csize = top - &dummy;		// ջ�����������ǴӸߵ�ַ���͵�ַ
	assert(csize <= STACK_SIZE);

	// ֻ�е�һ�� yield �浵��ʱ��
	// �Ż���� C->cap����ʼֵΪ0��С�� csize �����
	if (C->cap < csize) {
		free(C->stack);
		C->cap = csize;
		C->stack = malloc(C->cap);
	}

	// C->stack ������ǵ�ǰЭ�̵�������
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

// Э�̽���״̬��û�б���ģ�ֱ��ͨ�� NULL �ж�ȷ��
// ֻ���������������壺�п��ܾ���Э�̲����ڶ��� 
int 
coroutine_status(struct schedule * S, int id) {
	assert(id >= 0 && id < S->cap);
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

