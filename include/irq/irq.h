#ifndef	_IRQ_IRQ_H
#define	_IRQ_IRQ_H

extern void irq_init();
extern void irq_enable();
extern void irq_disable();
void irq_handle(unsigned int interrupt, unsigned long esr, unsigned long elr);

#endif  // _IRQ_IRQ_H