#include "drivers/mini_uart.h"
#include "drivers/timer.h"

#include "irq/irq.h"

#include "utils/cpu_tools.h"
#include "utils/printf.h"

void kernel_main()
{
    uart_init();
    uart_irq_enable();

    init_printf(0, putc);

    int el = get_exception_level();
    int sp = get_stack_pointer_level();
    printf("Exception level %d %d\n", el, sp);
    
    timer_init();
    timer_irq_enable();

    irq_init();
    irq_enable();
    printf("irq enabled\n");
    
    while(1) {}
}