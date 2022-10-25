#ifndef	_IO_IRQ_H
#define	_IO_IRQ_H

#include "mmio/base.h"

#define IRQ_PENDING_BASIC	((volatile unsigned int*)(MMIO_BASE+0x0000B200))
#define IRQ_PENDING_1		((volatile unsigned int*)(MMIO_BASE+0x0000B204))
#define IRQ_PENDING_2		((volatile unsigned int*)(MMIO_BASE+0x0000B208))
#define FIQ_CONTROL			((volatile unsigned int*)(MMIO_BASE+0x0000B20C))
#define IRQS_ENABLE_1		((volatile unsigned int*)(MMIO_BASE+0x0000B210))
#define IRQS_ENABLE_2		((volatile unsigned int*)(MMIO_BASE+0x0000B214))
#define IRQS_ENABLE_BASIC	((volatile unsigned int*)(MMIO_BASE+0x0000B218))
#define IRQS_DISABLE_1		((volatile unsigned int*)(MMIO_BASE+0x0000B21C))
#define IRQS_DISABLE_2		((volatile unsigned int*)(MMIO_BASE+0x0000B220))
#define IRQS_DISABLE_BASIC	((volatile unsigned int*)(MMIO_BASE+0x0000B224))

#define IRQ_TIMER_1 	(1 << (1  - 0))
#define IRQ_AUX     	(1 << (29 - 0))

#endif  // _IO_IRQ_H