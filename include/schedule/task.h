#ifndef _SCHED_TASK_H
#define _SCHED_TASK_H

#include "cpu_values/system_registers.h"

// beginning and end of process code in memory - to know to copy that code when executing them
// having FS would eliminate the need for this
extern char PROC_DEF_BEGIN;
extern char PROC_DEF_END;

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
	uint64_t pgd; // address of root VA table - includes 0xffff
};

struct task
{
	struct cpu_context cpu_context;
	enum states state;
	uint8_t priority;
	uint8_t preempt_block; // how many methods are disabling preemption
	uint64_t stack_end; // the address at which the stack is full
};

struct task* new_task(void *start_addr, void *return_to, uint32_t argc, uint64_t *argv); // creates the idle task
void free_task(struct task *t);

void preempt_enable(struct task *t);
void preempt_disable(struct task *t);

void drop_to_user(struct task *t);
void set_priority(struct task *t, uint8_t to);
struct task* copy(struct task *t, void *to, uint32_t argc, uint64_t *argv);

#endif // _SCHED_TASK_H