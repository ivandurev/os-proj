#include "schedule/task.h"
#include "mem/mem_values.h"
#include "mem/mem.h"
#include "irq/interrupts.h"
#include "cpu_values/system_registers.h"
#include "schedule/scheduler.h"
#include "utils/printf.h"
#include "user/base.h"

void preempt_enable(struct task *t)
{
	(t -> preempt_block) --;
}
void preempt_disable(struct task *t)
{
	(t -> preempt_block) ++;
}

struct task* new_task(void *start_addr, void *return_to, uint32_t argc, uint64_t *argv) // creates the idle task
{
	struct task *_task;
	_task = (struct task *) allocate_physical_page();
	if(!_task)
		return NULL;

	_task -> state = READY;
	_task -> priority = 1;
	_task -> global_priority = 1;
	_task -> preempt_block = 0;

	// immitate a interrupt having happened
	_task -> cpu_context.pgd = (uint64_t) allocate_pages();
	if(!(_task -> cpu_context.pgd))
		return NULL;

	uint64_t task_va = allocate_section((uint64_t *) _task -> cpu_context.pgd, false);
	if(!task_va)
		return NULL;

	uint64_t task_pa = read_pa((uint64_t *) _task -> cpu_context.pgd, task_va);
	if(!task_pa)
		return NULL;

	// copy all user code below the stack - stack + user_code needs to be less than 2MB
	mcopy((uint64_t *) &PROC_DEF_BEGIN,
		  ((&PROC_DEF_END) - (&PROC_DEF_BEGIN)) >> 3,
		  (uint64_t *) (task_pa + PROC_STACK_SIZE));

	// stack imitates an interrupt having happened - registers are saved on top
	_task -> cpu_context.sp = task_va + PROC_STACK_SIZE - STACK_ALLOC; // stack is going down to beginning of task VA
	_task -> cpu_context.pc = task_va + ((uint64_t) start_addr - ((uint64_t) (&PROC_DEF_BEGIN))) + PROC_STACK_SIZE;
	_task -> cpu_context.el = SPSR_M_EL1t;

	uint64_t sp_pa = task_pa + PROC_STACK_SIZE - STACK_ALLOC;
	for(int i = 0; i < argc && i < 8; i ++)
		*((uint64_t *) (sp_pa + (8 * i))) = *(argv + i);

	// return to the specified parent function
	*((uint64_t *) (sp_pa + IRQ_LR)) = task_va + ((uint64_t) return_to - (uint64_t) (&PROC_DEF_BEGIN)) + PROC_STACK_SIZE;

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
	allow_user_access((uint64_t *) t -> cpu_context.pgd);
}

void set_priority(struct task *t, uint8_t to)
{
	t -> priority = to;
	t -> global_priority = to;
}
