#include "drivers/mini_uart.h"

void kernel_main()
{
    // set up serial console
    uart_init();
    
    // say hello
    uart_puts("Hello World!\n");
    
    // echo everything back
    while(1) {
    	uart_putc(uart_getc());
    }
}