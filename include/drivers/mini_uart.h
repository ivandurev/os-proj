#ifndef _D_MINI_UART_H
#define _D_MINI_UART_H

void uart_init();
void uart_putc(char c);
char uart_getc();
void uart_puts(char *s);
void putc(void *p, char c); // for printf

void uart_irq_enable();
void uart_irq_handle();

#endif // _D_MINI_UART_H