#include "drivers/mini_uart.h"
#include "utils/cpu_tools.h"
#include "utils/printf.h"

void kernel_main()
{
    uart_init();
    init_printf(0, putc);

    int el = get_exception_level();
    printf("Exception level %d \n", el);
    
    // echo everything back
    while(1) {
    	uart_putc(uart_getc());
    }
}