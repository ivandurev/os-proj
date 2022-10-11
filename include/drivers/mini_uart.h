#ifndef _D_MINI_UART_H
#define _D_MINI_UART_H

void uart_init();
void uart_putc(char c);
char uart_getc();
void uart_puts(char *s);

#endif // _D_MINI_UART_H