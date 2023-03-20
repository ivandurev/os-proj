#include "drivers/mini_uart.h"
#include "mmio/mini_uart.h"
#include "mmio/gpio.h"
#include "mmio/irq.h"

// Initialise the uart device
void uart_init()
{
    register uint32_t r;

    r = *GPFSEL1;
    r &= ~((7<<12) | (7<<15)); // zero out the selector for pin 14 (bits 14-12) and pin 15 (bits 17-15)
    r |= ((2<<12) | (2<<15)); // enter function 5 for miniuart
    *GPFSEL1 = r;

    // removing pull-up / down values of the pins - described in the BCM2835 manual
    *GPPUD = 0;
    r = 150;
    while(r --)
        asm volatile("nop");
    *GPPUDCLK0 = (1<<14)|(1<<15);
    r = 150;
    while(r --)
        asm volatile("nop");
    *GPPUDCLK0 = 0;

    // initialize mini UART
    *AUX_ENABLE |=1;       // enable UART1, AUX mini uart
    *AUX_MU_CNTL = 0;
    *AUX_MU_LCR = 3;       // 8 bits
    *AUX_MU_MCR = 0;
    *AUX_MU_IER = 0;
    *AUX_MU_IIR = 0xc6;    // disable interrupts
    *AUX_MU_BAUD = 270;    // 115200 baud
    *AUX_MU_CNTL = 3;      // enable Tx, Rx
}

// send a single character
void uart_putc(char c) {
    // wait until it can be sent
    do
        asm volatile("nop");
    while(!(*AUX_MU_LSR& (1 << 5)));

    // write
    *AUX_MU_IO = c;
}

// read a single character
char uart_getc() {
    char r;
    // block until the buffer is non-empty
    do
        asm volatile("nop");
    while(!(*AUX_MU_LSR & 1));
    
    // read
    r = (char) (*AUX_MU_IO);
    
    // conversion needed for the terminal
    return r == '\r' ? '\n' : r;
}

// print a string
void uart_puts(char *s) {
    while(*s) {
        
        // conversion as UART recognises \r
        if(*s == '\n')
            uart_putc('\r');
        uart_putc(*s ++);
    }
}

// wrapper for printf
void putc (void *p, char c) {
    uart_putc(c);

    // perform carriage return on a newline
    if(c == '\n')
        uart_putc('\r');
}


// could be rewritten in the future for FIFOs and write/read interrupt buffers

// this enables all AUX interrupts
void uart_irq_enable() {
    *AUX_MU_IER = 1;
    *IRQS_ENABLE_1 = IRQ_AUX;
}

// handle an interrupt from the UART
void uart_irq_handle() {
    // check if its another AUX device
    if((*(AUXIRQ) & 1) != 1) {
        uart_puts("UNRECOGNISED AUX DATA\n\r");
        return;
    }

    // for now the action is printing what we received
    while(((*AUX_MU_IIR) & 4) == 4) {
        uart_putc(uart_getc());
    }
}