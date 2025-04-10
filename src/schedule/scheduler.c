#include "schedule/scheduler.h"
#include "schedule/task.h"
#include "mem/mem.h"
#include "mem/mem_values.h"
#include "utils/printf.h"

struct task *tasks[100]; // all tasks - assumes idx 0 is the idle task - special cases apply
// the above can be improved beyond a static array, but that suffices for now
uint8_t tasks_len = 0;

struct task *curr = NULL; // current running task

#ifdef DEBUG
int __stats[100]; // number of switches to each task - debugging only
#endif

struct task* get_current_task()
{
	return curr;
}

void schedule()
{
	int16_t maxp = -1;
	int16_t next = -1;
	if(curr)
	{
		preempt_disable(curr);
		maxp = curr -> priority;
	}

	// delete DONE tasks from the queue - DELETE operation on the data structure
	if (curr && curr -> state == DONE)
	{
		bool found = false;
		for (int i = 0; i < tasks_len - 1; i ++)
		{
			if (tasks[i] == curr)
			{
				free_task(curr);
				found = true;
			}
			if (found)
				tasks[i] = tasks[i+1];
		}
		tasks_len --;
		maxp = -1;
		curr = NULL;
	}

#ifdef DEBUG
	__printf("Stats: ");
#endif

	// below is a POP and PUSH to the back operation on the data structure
	// chooses the highest priority task that differs from the current one - fairness
	for (int i = 0; i < tasks_len; i ++)
	{
#ifdef DEBUG
		__printf("%d times, ", __stats[i]);
#endif
		if (tasks[i] -> state == READY && tasks[i] -> priority >= maxp && curr != tasks[i])
		{
			maxp = tasks[i] -> priority;
			next = i;
		}
	}
#ifdef DEBUG
	__printf("\n");
#endif

	// in case all priorities expire and we have more than 1 task, we need to reset the priorities to their starting values
	// this way we ensure priorities define a ratio of runtime instead of exclusivity - each program runs at least 1 time
	if (maxp == 0 && tasks_len > 1)
	{
		next = -1;
		// starting from 1 to ignore the idle task which also has a priority of 0
		for (int i = 1; i < tasks_len; i ++)
		{
			if (tasks[i] -> state == READY)
			{
				tasks[i] -> priority = tasks[i] -> global_priority; // restore priority
				// make a new choice on what to run
				if (tasks[i] -> priority >= maxp && curr != tasks[i])
				{
					maxp = tasks[i] -> priority;
					next = i;
				}
			}
		}
	}

	// only switch the CPU context if actually needed
	if (next >= 0)
	{
		preempt_disable(tasks[next]);
		if (tasks[next] -> priority)
			tasks[next] -> priority --; // next time try to execute something else
		switch_to(tasks[next]);	// the actual context switch
		struct task *old = curr;
		curr = tasks[next];

#ifdef DEBUG
		__stats[next] ++;
#endif

		// account for the old task being deleted
		if(old)
			preempt_enable(old);
	}

	preempt_enable(curr);	
}

void queue_task(struct task *t)
{
	tasks[tasks_len ++] = t;
}

// called by the timer interrupt
void tick()
{
	// account for not having a task to run
	if(!curr || !(curr -> preempt_block))
		schedule();
}

void switch_to(struct task *to)
{
	// save previous state
	if (curr)
	{
		__asm__("mrs %0, sp_el0" : "=r" (curr -> cpu_context.sp));
		__asm__("mrs %0, elr_el1" : "=r" (curr -> cpu_context.pc));
		__asm__("mrs %0, spsr_el1" : "=r" (curr -> cpu_context.el));
	}

	// restore the state of the next task - its registers will be recovered from its stack in the interrupt exit
	uint64_t volatile register tosp = to -> cpu_context.sp;
	uint64_t volatile register topc = to -> cpu_context.pc;
	uint64_t volatile register toel = to -> cpu_context.el;
	uint64_t volatile register topgd = to -> cpu_context.pgd ^ DESCRIPTOR_KERNEL_BITMASK;
	
	__asm__("msr sp_el0, %0" : : "r" (tosp));
	__asm__("msr elr_el1, %0" : : "r" (topc));
	__asm__("msr spsr_el1, %0" : : "r" (toel));

	__asm__("msr ttbr0_el1, %0" : : "r" (topgd)); // switch root mapping page address

	// invalidate caches for virtual memory
	__asm__("tlbi vmalle1is");
  	__asm__("dsb ish");
	__asm__("isb");
}

