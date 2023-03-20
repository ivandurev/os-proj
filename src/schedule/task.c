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

// creates a new task with a callback function and parameters
struct task* new_task(void *start_addr, void *return_to, uint32_t argc, uint64_t *argv)
{
	struct task *_task;
	_task = (struct task *) allocate_physical_page(); // will hold the task information
	if(!_task)
		return NULL;

	_task -> state = READY; // immediately ready for scheduling
	_task -> priority = 1;
	_task -> global_priority = 1; // default priority can be changed later
	_task -> preempt_block = 0;

	// next we need space to hold the virtual memory pages
	_task -> cpu_context.pgd = (uint64_t) allocate_pages();
	if(!(_task -> cpu_context.pgd))
		return NULL;

	// final memory allocation is the actual memory dedicated to the process that it will be able to see
	uint64_t task_va = allocate_section((uint64_t *) _task -> cpu_context.pgd, false);
	if(!task_va)
		return NULL;

	uint64_t task_pa = read_pa((uint64_t *) _task -> cpu_context.pgd, task_va);
	if(!task_pa)
		return NULL;


	// the structure of the process memory is (0 - PROC_STACK_SIZE) -> stack, (PROC_STACK_SIZE - SECTION_SIZE) -> code, bss, ro
	// copy all user code above the stack - stack + user_code needs to be less than 2MB
	// this calculation uses the process offset from the beginning of user definitions in the RAM 
	// that will change once filesystems are implemented and will instead load from the storage
	mcopy((uint64_t *) &PROC_DEF_BEGIN,
		  ((&PROC_DEF_END) - (&PROC_DEF_BEGIN)) >> 3,
		  (uint64_t *) (task_pa + PROC_STACK_SIZE));

	// stack imitates an interrupt having happened - registers are saved on it that is why stack already has STACK_ALLOC bytes taken
	_task -> cpu_context.sp = task_va + PROC_STACK_SIZE - STACK_ALLOC; // stack is going down to beginning of task VA

	// PC is offset the same amount from the new VA as it is in the physical memory where it was copied from
	// stack is accounted for
	_task -> cpu_context.pc = task_va + ((uint64_t) start_addr - ((uint64_t) (&PROC_DEF_BEGIN))) + PROC_STACK_SIZE;
	_task -> cpu_context.el = SPSR_M_EL1t; // default privilege level - can be dropped by the kernel

	// apply parameters - putting them on the stack as the interrupt exit will "restore" them
	uint64_t sp_pa = task_pa + PROC_STACK_SIZE - STACK_ALLOC;
	for(int i = 0; i < argc && i < 8; i ++)
		*((uint64_t *) (sp_pa + (8 * i))) = *(argv + i);

	// return to the specified callback function - the same as the PC calculation
	*((uint64_t *) (sp_pa + IRQ_LR)) = task_va + ((uint64_t) return_to - (uint64_t) (&PROC_DEF_BEGIN)) + PROC_STACK_SIZE;

	return _task;
}

// the below will take care of all allocated memory for a process once it is finished
void free_task(struct task *t)
{
	free_pages((uint64_t *) (t -> cpu_context.pgd));
	free_physical_page((uint64_t) t);
}

// when switching privilege level we need to also account for memory access
void drop_to_user(struct task *t)
{
	t -> cpu_context.el = SPSR_M_EL0t;
	allow_user_access((uint64_t *) t -> cpu_context.pgd);
}

// priority resets counter
void set_priority(struct task *t, uint8_t to)
{
	t -> priority = to;
	t -> global_priority = to;
}
