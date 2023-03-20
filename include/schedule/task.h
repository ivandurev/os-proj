#ifndef _SCHED_TASK_H
#define _SCHED_TASK_H

// Task structure and manipulation

#include "cpu_values/system_registers.h"

// beginning and end of process code in memory - to know to copy that code when executing them
// having FS would eliminate the need for this
extern char PROC_DEF_BEGIN;
extern char PROC_DEF_END;

// states of a task
enum states
{
	READY = 1,
	DONE = 2,
};

// the CPU context that needs to be switched - the registers are expected to be on the stack
struct cpu_context
{
	uint64_t pc; // which is essentially elr
	uint64_t sp; // the SP of the process - in EL0
	uint8_t el; // EL to return to
	uint64_t pgd; // address of root VA table - includes 0xffff
};

// information about the task
struct task
{
	struct cpu_context cpu_context;
	enum states state;
	uint8_t priority; // priority for current schedule tick - goes from global_priority to 0
	uint8_t global_priority; // constant priority level (proportional) (0 is reserved for the idle task)
	uint8_t preempt_block; // how many methods are disabling preemption
	uint64_t stack_end; // the address at which the stack is full
};

// creates a task
// - start_addr is the kernel address of the function (gets converted to virtual address)
// - return_to is the kernel address of the callback function (default is on_return)
// - argc and argv are optional specifying parameters to pass to the function - max 8
// - sets priority to 1 and state to READY and exception level to EL1
struct task* new_task(void *start_addr, void *return_to, uint32_t argc, uint64_t *argv);
void free_task(struct task *t);

// manages preemption while a task is running
void preempt_enable(struct task *t);
void preempt_disable(struct task *t);

// sets the task to execute at EL0 from now on
void drop_to_user(struct task *t);

// changes the priority of a task
void set_priority(struct task *t, uint8_t to);

#endif // _SCHED_TASK_H