#include "schedule/task.h"
#include "mem/mem.h"
#include "irq/interrupts.h"
#include "cpu_values/system_registers.h"
#include "schedule/scheduler.h"
#include "utils/printf.h"

uint64_t mid = (((PAGE_SIZE - sizeof(struct task)) >> 4) << 4); // ALIGN TO 16 BYTES for SP

void preempt_enable(struct task *t)
{
	(t -> preempt_block) ++;
}
void preempt_disable(struct task *t)
{
	(t -> preempt_block) --;
}

void idle()
{
	while(1) {printf("idle\n");}
}

struct task* idle_task() // creates the idle task
{
	struct task *_idle;
	_idle = (struct task*) page_alloc();
	if(!_idle)
		return NULL;
	_idle = (struct task *) ((uint64_t) _idle + mid);
	_idle -> state = READY;
	_idle -> priority = 0;
	_idle -> preempt_block = 0;

	// immitate a interrupt having happened

	_idle -> cpu_context.sp = (uint64_t) _idle - STACK_ALLOC;
	_idle -> cpu_context.pc = (uint64_t) idle;
	_idle -> cpu_context.el = SPSR_M_EL1t;

	_idle -> stack_end = (uint64_t) _idle - mid; // this marks the beginning of the task struct and the end of the stack

	return _idle;
}

struct task* copy(struct task *t, void *to, uint32_t argc, uint64_t *argv)
{
	preempt_disable(t);

	struct task *child;
	child = (struct task*) page_alloc();
	if(!child || !t || !to)
		return NULL;
	child = (struct task *) ((uint64_t) child + mid); // leave space for the stack

	child -> state = READY;
	child -> priority = t -> priority;
	child -> preempt_block = 0; // enable by default

	// immitate a interrupt having happened
	uint64_t sp = (uint64_t) child; // stack is from here FD
	sp -= STACK_ALLOC;

	for(int i = 0; i < argc && i < 8; i ++)
		*((uint64_t *) (sp + (8 * i))) = argv[i];

	*((uint64_t *) (sp + (8*30))) = (uint64_t) on_return;

	child -> cpu_context.sp = (uint64_t) sp;
	child -> cpu_context.pc = (uint64_t) to;
	child -> cpu_context.el = t -> cpu_context.el;

	child -> stack_end = (uint64_t) child - mid; // this marks the beginning of the task struct and the end of the stack

	preempt_enable(t);

	return child;
}