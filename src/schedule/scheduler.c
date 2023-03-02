#include "schedule/scheduler.h"
#include "schedule/task.h"
#include "mem/mem.h"
#include "utils/printf.h"

struct task *tasks[100]; // all tasks
uint8_t tasks_len = 0;
struct task *curr = NULL; // current running task

struct task* get_current_task()
{
	return curr;
}

void init_task(struct task *t) // must be done before any scheduling happens
{
	// does not even consider that for scheduling ever again - just placeholder
	curr = t;
	curr -> preempt_block = 0;
	curr -> priority = 0;
}

void schedule()
{
	preempt_disable(curr);
	int16_t maxp = -1;
	int16_t next = -1;
	maxp = curr -> priority;

	if(curr -> state == DONE)
	{
		bool found = false;
		for(int i = 0; i < tasks_len-1; i ++)
		{
			if(tasks[i] == curr)
			{
				found = true;
			}
			if(found)
				tasks[i] = tasks[i+1];
		}
		tasks_len --;
		maxp = -1;
	}

	for (int i = 0; i < tasks_len; i ++)
	{
		//printf("Task %d - state %d, priority %d, PC %d, SP %d, EL %d\n", i, tasks[i] -> state, tasks[i] -> priority, tasks[i] -> cpu_context.pc, tasks[i] -> cpu_context.sp, tasks[i] -> cpu_context.el);
		if(tasks[i] -> state == READY && tasks[i] -> priority >= maxp && curr != tasks[i])
		{
			maxp = tasks[i] -> priority;
			next = i;
		}
	}

	if (next >= 0)
	{
		preempt_disable(tasks[next]);
		switch_to(tasks[next]);
		struct task *old = curr;
		curr = tasks[next];
		if(old -> state == DONE)
			;//mfree(old -> stack_end);
		else
			preempt_enable(old);
	}
	preempt_enable(curr);
}

void queue_task(struct task *t)
{
	tasks[tasks_len ++] = t;
}

void tick()
{
	if(!(curr -> preempt_block))
		schedule();
}

void switch_to(struct task *to)
{
	__asm__("mrs %0, sp_el0" : "=r" (curr -> cpu_context.sp));
	__asm__("mrs %0, elr_el1" : "=r" (curr -> cpu_context.pc));
	__asm__("mrs %0, spsr_el1" : "=r" (curr -> cpu_context.el));

	uint64_t volatile register tosp = to -> cpu_context.sp;
	uint64_t volatile register topc = to -> cpu_context.pc;
	uint64_t volatile register toel = to -> cpu_context.el;
	uint64_t volatile register topgd = to -> cpu_context.pgd;
	
	__asm__("msr sp_el0, %0" : : "r" (tosp));
	__asm__("msr elr_el1, %0" : : "r" (topc));
	__asm__("msr spsr_el1, %0" : : "r" (toel));

	__asm__("msr ttbr0_el1, %0" : : "r" (topgd));
	__asm__("tlbi vmalle1is");
  	__asm__("dsb ish");
	__asm__("isb");
}

void on_return()
{
	curr -> state = DONE;
	while(1) {}
}

struct task* fork(void *to, uint32_t argc, uint64_t *argv);
