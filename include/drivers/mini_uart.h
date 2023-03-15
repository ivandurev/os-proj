#ifndef _D_MINI_UART_H
#define _D_MINI_UART_H

// Driver for the miniUART

// needs to be called to initialise the GPIO pins
void uart_init();

// outputs a single character
void uart_putc(char c);

// reads a single character
char uart_getc();

// outputs a string
void uart_puts(char *s);

// outputs a single character and converts \n to \n\r - used by printf
void putc(void *p, char c); // for printf

// handles interrupts on a character
void uart_irq_enable();
void uart_irq_handle();

#endif // _D_MINI_UART_H