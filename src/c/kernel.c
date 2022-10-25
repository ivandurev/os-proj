#include "drivers/mini_uart.h"
#include "irq/irq.h"
#include "irq/timer.h"

#include "utils/cpu_tools.h"
#include "utils/printf.h"

void kernel_main()
{
    uart_init();
    init_printf(0, putc);

    int el = get_exception_level();
    int sp = get_stack_pointer_level();
    printf("Exception level %d %d\n", el, sp);

    irq_init();
    printf("init\n");
    irq_timer_init();
    printf("timer_init\n");
    irq_timer_enable();
    printf("timer_emable\n");
    irq_enable();
    printf("enable\n");
    
    // echo everything back
    while(1) {
    	uart_putc(uart_getc());
    }
}