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

// void func1()
// {
//     while(1)
//     {
//         __printf("EL: %d\n\r", get_exception_level());
//     }
// }
// void func2()
// {
//     while(1)
//     {
//         __printf("\t\t\tuser\n\r");
//     }
// }

// void temporary()
// {
//     uart_init();
//     uart_irq_enable();
//     init___printf(0, putc);
//     __printf("Init.\n");
// }
// void temporary_print(uint64_t value)
// {
//     __printf("Value: %x\n", value);
// }

void kernel_main()
{
    __asm__("msr ttbr0_el1, xzr"); // reset this now that PC is updated - will be used by user processes
    uart_init();
    uart_irq_enable();

    init_printf(0, putc);
    __printf("Booted to C and uart configured!\n");

    int el = get_exception_level();
    int sp = get_stack_pointer_level();

    __printf("Exception level %d %d\n", el, sp);

    init_memory();
    __printf("Virtual memory allocator initialised!\n");

        
    // initialise further tasks to switch to - derived from the idle task
    // struct task *idle = idle_task();
    // struct task *_func1 = copy(idle, func1, 0, NULL);
    // struct task *_func2 = copy(idle, func2, 0, NULL);

    // if(idle && _func1 && _func2)
    // {
    //    set_priority(_func1, 1);
    //    set_priority(_func2, 1);
    //    drop_to_user(_func2);
    //    init_task(NULL); // it is fine to overwrite the first few bytes - it will only be done once 
    //    queue_task(idle);
    //    queue_task(_func1);
    //    queue_task(_func2);
    // }
    // else
    //     __printf("Error while initialising init task!\n");
    
    // timer_init();
    // timer_irq_enable();

    struct task *idle_task = new_task(&idle, NULL, 0, NULL);
    struct task *first_task = new_task(&first_func, &on_return, 0, NULL);
    if(!first_task && !idle_task)
    {
        __printf("ERROR WHEN INITIALISING TASK\n");
        while(1) {}
    }

    queue_task(idle_task);
    queue_task(first_task);

    // initialise scheduling based on the timer and interrupts
    timer_init();
    timer_irq_enable();

    irq_init();
    irq_enable();
    // end of code in this function - now everything is managed through the scheduler
    

    // NO STACK AFTER THIS POINT SINCE IT IS NOW USED BY INTERRUPTS
    // TO CONTINUE USING THE KERNEL ADD AN INIT TASK TO THE QUEUE
    while(1)
    {}
}