#include "schedule/task.h"
#include "mem/mem_values.h"
#include "mem/mem.h"
#include "irq/interrupts.h"
#include "cpu_values/system_registers.h"
#include "schedule/scheduler.h"
#include "utils/printf.h"

void preempt_enable(struct task *t)
{
	(t -> preempt_block) ++;
}
void preempt_disable(struct task *t)
{
	(t -> preempt_block) --;
}

struct task* new_task(void *start_addr, uint32_t argc, uint64_t *argv) // creates the idle task
{
	struct task *_task;
	_task = (struct task *) allocate_physical_page();
	if(!_task)
		return NULL;
	//_idle = (struct task *) ((uint64_t) _idle + mid);
	_task -> state = READY;
	_task -> priority = 1;
	_task -> preempt_block = 0;

	// immitate a interrupt having happened
	_task -> cpu_context.pgd = (uint64_t) allocate_pages();
	if(!(_task -> cpu_context.pgd))
		return NULL;

	uint64_t task_va = allocate_section((uint64_t *) _task -> cpu_context.pgd);
	if(!task_va)
		return NULL;
	uint64_t task_pa = read_pa((uint64_t *) _task -> cpu_context.pgd, task_va);
	if(!task_pa)
		return NULL;

	// copy all user code below the stack - stack + user_code needs to be less than 2MB
	mcopy((uint64_t *) &PROC_DEF_BEGIN, ((&PROC_DEF_END) - (&PROC_DEF_BEGIN)) >> 3, (uint64_t *) task_pa + PROC_STACK_SIZE);

	// stack imitates an interrupt having happened - registers are saved on top
	_task -> cpu_context.sp = PROC_STACK_SIZE - STACK_ALLOC; // stack is going down to 0 VA
	_task -> cpu_context.pc = start_addr - (void *) (&PROC_DEF_BEGIN) + PROC_STACK_SIZE;
	_task -> cpu_context.el = SPSR_M_EL1t;

	for(int i = 0; i < argc && i < 8; i ++)
		*((uint64_t *) (_task -> cpu_context.sp + (8 * i))) = argv[i];

	*((uint64_t *) (_task -> cpu_context.sp + IRQ_LR)) = (uint64_t) on_return;

	//_idle -> stack_end = (uint64_t) _idle - mid; // this marks the beginning of the task struct and the end of the stack

	return _task;
}

void free_task(struct task *t)
{
	free_pages((uint64_t *) (t -> cpu_context.pgd));
	free_physical_page((uint64_t) t);
}

void drop_to_user(struct task *t)
{
	t -> cpu_context.el = SPSR_M_EL0t;
}

void set_priority(struct task *t, uint8_t to)
{
	t -> priority = to;
}

struct task* copy(struct task *t, void *to, uint32_t argc, uint64_t *argv)
{
	preempt_disable(t);

	struct task *child;
	child = NULL;//(struct task*) malloc();
	// if(!child || !t || !to)
	// 	return NULL;
	// child = (struct task *) ((uint64_t) child + mid); // leave space for the stack

	// child -> state = READY;
	// child -> priority = t -> priority;
	// child -> preempt_block = 0; // enable by default

	// // immitate a interrupt having happened
	// uint64_t sp = (uint64_t) child; // stack is from here FD
	// sp -= STACK_ALLOC;

	// for(int i = 0; i < argc && i < 8; i ++)
	// 	*((uint64_t *) (sp + (8 * i))) = argv[i];

	// *((uint64_t *) (sp + (8*30))) = (uint64_t) on_return;

	// child -> cpu_context.sp = (uint64_t) sp;
	// child -> cpu_context.pc = (uint64_t) to;
	// child -> cpu_context.el = t -> cpu_context.el;

	// child -> stack_end = (uint64_t) child - mid; // this marks the beginning of the task struct and the end of the stack

	preempt_enable(t);

	return child;
}