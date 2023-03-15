#ifndef _IRQ_INTERRUPTS_H
#define _IRQ_INTERRUPTS_H

// Values for interrupt handling

// Offsets for different interrupt handlers
#define SYNC_EL1t		0 
#define IRQ_EL1t		1 
#define FIQ_EL1t		2 
#define ERROR_EL1t		3 

#define SYNC_EL1h		4 
#define IRQ_EL1h		5 
#define FIQ_EL1h		6 
#define ERROR_EL1h		7 

#define SYNC_EL0_64	    8 
#define IRQ_EL0_64	    9 
#define FIQ_EL0_64		10 
#define ERROR_EL0_64	11 

#define SYNC_EL0_32		12 
#define IRQ_EL0_32		13 
#define FIQ_EL0_32		14 
#define ERROR_EL0_32	15 

// Saved register space - on the stack 32*8 bytes each
#define STACK_ALLOC     256

// offsets for each register from SP
#define IRQ_X0		(8 * 0)
#define IRQ_X1		(8 * 1)
#define IRQ_X2		(8 * 2)
#define IRQ_X3		(8 * 3)
#define IRQ_X4		(8 * 4)
#define IRQ_X5		(8 * 5)
#define IRQ_X6		(8 * 6)
#define IRQ_X7		(8 * 7)
#define IRQ_X8		(8 * 8)
#define IRQ_X9		(8 * 9)
#define IRQ_X10		(8 * 10)
#define IRQ_X11		(8 * 11)
#define IRQ_X12		(8 * 12)
#define IRQ_X13		(8 * 13)
#define IRQ_X14		(8 * 14)
#define IRQ_X15		(8 * 15)
#define IRQ_X16		(8 * 16)
#define IRQ_X17		(8 * 17)
#define IRQ_X18		(8 * 18)
#define IRQ_X19		(8 * 19)
#define IRQ_X20		(8 * 20)
#define IRQ_X21		(8 * 21)
#define IRQ_X22		(8 * 22)
#define IRQ_X23		(8 * 23)
#define IRQ_X24		(8 * 24)
#define IRQ_X25		(8 * 25)
#define IRQ_X26		(8 * 26)
#define IRQ_X27		(8 * 27)
#define IRQ_X28		(8 * 28)
#define IRQ_X29		(8 * 29)
#define IRQ_X30		(8 * 30)

#define IRQ_FP		IRQ_X29
#define IRQ_LR		IRQ_X30

#endif // _IRQ_INTERRUPTS_H