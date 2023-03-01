#ifndef _SCHED_TASK_H
#define _SCHED_TASK_H

#include "cpu_values/system_registers.h"

#define THREAD_SIZE (1 << 12) // 4 KiB = PAGE_SIZE

// TASK LAYOUT
// addr   field
// x+0    stack (addr is in stack_end)
// x+8    stack
// x+16   stack
// .
// on the top of the stack are the saved registers of the task
// .
// x+n    task struct
// .
// .
// .
// PAGE END
// x+n is aligned to 16 bytes to keep the stack valid
// n is the maximum value so that the struct doesn't go over the page size

enum states
{
	READY = 1,
	DONE = 2,
};

struct cpu_context
{
	uint64_t pc; // which is essentially elr
	uint64_t sp; // the SP of the process - in EL0
	uint8_t el; // EL to return to
	uint64_t pgd; // address of root VA table
};

struct task
{
	struct cpu_context cpu_context;
	enum states state;
	uint8_t priority;
	uint8_t preempt_block; // how many methods are disabling preemption
	uint64_t stack_end; // the address at which the stack is full
};

struct task* idle_task();

void preempt_enable(struct task *t);
void preempt_disable(struct task *t);

void drop_to_user(struct task *t);
void set_priority(struct task *t, uint8_t to);
struct task* copy(struct task *t, void *to, uint32_t argc, uint64_t *argv);

#endif // _SCHED_TASK_H