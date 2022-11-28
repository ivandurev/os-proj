#ifndef _MMIO_MINI_UART_H
#define _MMIO_MINI_UART_H

#include "mmio/base.h"

#define AUXIRQ			((volatile uint32_t*)(MMIO_BASE+0x00215000))
#define AUX_ENABLE      ((volatile uint32_t*)(MMIO_BASE+0x00215004))

#define AUX_MU_IO       ((volatile uint32_t*)(MMIO_BASE+0x00215040))
#define AUX_MU_IER      ((volatile uint32_t*)(MMIO_BASE+0x00215044))
#define AUX_MU_IIR      ((volatile uint32_t*)(MMIO_BASE+0x00215048))
#define AUX_MU_LCR      ((volatile uint32_t*)(MMIO_BASE+0x0021504C))
#define AUX_MU_MCR      ((volatile uint32_t*)(MMIO_BASE+0x00215050))
#define AUX_MU_LSR      ((volatile uint32_t*)(MMIO_BASE+0x00215054))
#define AUX_MU_MSR      ((volatile uint32_t*)(MMIO_BASE+0x00215058))
#define AUX_MU_SCRATCH  ((volatile uint32_t*)(MMIO_BASE+0x0021505C))
#define AUX_MU_CNTL     ((volatile uint32_t*)(MMIO_BASE+0x00215060))
#define AUX_MU_STAT     ((volatile uint32_t*)(MMIO_BASE+0x00215064))
#define AUX_MU_BAUD     ((volatile uint32_t*)(MMIO_BASE+0x00215068))

#endif // _MMIO_MINI_UART_H