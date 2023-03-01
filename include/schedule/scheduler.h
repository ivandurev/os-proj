#ifndef _SCHED_SCHEDULER_H
#define _SCHED_SCHEDULER_H

#include "schedule/task.h"


void schedule();
void tick();

void init_task(struct task *t);
void queue_task(struct task *t);

void switch_to(struct task *to);

void on_return();
struct task* fork(void *to, uint32_t argc, uint64_t *argv);

struct task* get_current_task();

#endif // _SCHED_SCHEDULER_H