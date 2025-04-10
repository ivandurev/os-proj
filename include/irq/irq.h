#ifndef	_IRQ_IRQ_H
#define	_IRQ_IRQ_H

// Handling interrupts

// define bits for interpreting an interrupt cause from ESR
#define IRQ_ESR_EC_SHIFT 	26
#define IRQ_ESR_EC_SVC_64 	21 // the code for SVC instr in AARCH64
#define IRQ_ISS_SVC_BITMASK ((1 << 17) - 1) // bits denoting the svc call

// irq enabling in ASM
extern void irq_init();
extern void irq_enable();
extern void irq_disable();

// handling of any interrupt - first parameter is the interrupt offset as defined in interrupts.h
void irq_handle(uint8_t interrupt, uint64_t esr, uint64_t elr, uint64_t sp);

#endif  // _IRQ_IRQ_H