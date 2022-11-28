#include "drivers/mini_uart.h"
#include "drivers/timer.h"

#include "irq/irq.h"

#include "utils/cpu_tools.h"
#include "utils/printf.h"

#include "mem/mem.h"

#include "schedule/task.h"
#include "schedule/scheduler.h"

void func1()
{
    for(int i = 0; i < 1000; i ++)
    {
        printf("340958349843\n");
    }
}
void func2()
{
    printf("lololol\n");
    return;
}



extern uint64_t irq_handlers;

void kernel_main()
{
    uart_init();
    uart_irq_enable();

    init_printf(0, putc);

    int el = get_exception_level();
    int sp = get_stack_pointer_level();

    printf("Exception level %d %d\n", el, sp);

    // initialise further tasks to switch to - derived from the idle task
    struct task *idle = idle_task();
    struct task *_func1 = copy(idle, func1, 0, NULL);
    struct task *_func2 = copy(idle, func2, 0, NULL);

    if(idle && _func1 && _func2)
    {
        _func1 -> priority = 1;
        _func2 -> priority = 1;
        init_task(NULL);
        queue_task(_func1);
        queue_task(_func2);
        queue_task(idle);
    }
    else
        printf("Error while initialising init task!\n");
    
    timer_init();
    timer_irq_enable();

    irq_init();
    printf("irq enabling\n");
    irq_enable();
    
    // NO STACK AFTER THIS POINT SINCE IT IS NOW USED BY INTERRUPTS
    // TO CONTINUE USING THE KERNEL ADD AN INIT TASK TO THE QUEUE
    
    while(1)
    {}
}