#include "drivers/mini_uart.h"
#include "drivers/timer.h"

#include "irq/irq.h"

#include "utils/cpu_tools.h"
#include "utils/printf.h"

#include "mem/mem.h"

#include "schedule/task.h"
#include "schedule/scheduler.h"

#include "user/syscall.h"
#include "user/base.h"

void kernel_main()
{
    __asm__("msr ttbr0_el1, xzr"); // reset this now that PC is updated - will be used by user processes

    // initialise miniUART and printf
    uart_init();
    uart_irq_enable();

    init_printf(0, putc);

#ifdef DEBUG
    __printf("\n\nBooted to C and uart configured!\n");

    int el = get_exception_level();
    int sp = get_stack_pointer_level();

    __printf("Exception level %d %d\n", el, sp);
#endif

    // initialise virtual memory manager
    init_memory();
#ifdef DEBUG
    __printf("Virtual memory allocator initialised!\n");
#endif


    // create tasks for user functions
    // for now since those are stored in RAM and static this is a simple way of running processes
    // might be extended to run based on user input and selection of tasks
    // filesystem ELF loading is also possible
    struct task *idle_task = new_task(&idle, NULL, 0, NULL);
    struct task *first_task = new_task(&first_func, &on_return, 0, NULL);
    struct task *second_task = new_task(&second_func, &on_return, 0, NULL);
    struct task *third_task = new_task(&third_func, &on_return, 0, NULL);
    if(!first_task || !idle_task || !second_task || !third_task)
    {
        __printf("ERROR WHEN INITIALISING TASKS\n");
        while(1) {}
    }

    // those settings are for demonstration purposes
    // the only requirement is to create and queue an idle task for when there are no tasks
    
    set_priority(idle_task, 0);
    set_priority(first_task, 1);
    set_priority(second_task, 2);
    set_priority(third_task, 3);

    drop_to_user(first_task);
    drop_to_user(third_task);

    queue_task(idle_task);
    queue_task(first_task);
    queue_task(second_task);
    queue_task(third_task);

    // initialise scheduling based on the timer and interrupts
    timer_init();
    timer_irq_enable();

    irq_init();
    irq_enable();
    // end of code in this function - now everything is managed through the scheduler
    // note this is because the stack pointer we were using will now be switched dynamically to each task
    // this function itself is not a task and will therefore lose its SP after this line
    // any code to follow needs to be a scheduled task

    while(1) {}
}