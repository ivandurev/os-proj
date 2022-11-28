#ifndef	_IRQ_IRQ_H
#define	_IRQ_IRQ_H

extern void irq_init();
extern void irq_enable();
extern void irq_disable();
void irq_handle(uint8_t interrupt, uint64_t esr, uint64_t elr);

#endif  // _IRQ_IRQ_H