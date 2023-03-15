#ifndef _SCHED_SCHEDULER_H
#define _SCHED_SCHEDULER_H

// Process Scheduler

#include "schedule/task.h"

// responsible for assigning next task on a timer tick
void schedule();
void tick();

// add a task to the queue
void queue_task(struct task *t);

// switch the CPU context to the new task
void switch_to(struct task *to);

// return the current task
struct task* get_current_task();

#endif // _SCHED_SCHEDULER_H