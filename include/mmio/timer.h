#ifndef	_IO_TIMER_H
#define	_IO_TIMER_H

#include "mmio/base.h"

#define TIMER_CS        ((volatile uint32_t*)(MMIO_BASE+0x00003000))
#define TIMER_CLO       ((volatile uint32_t*)(MMIO_BASE+0x00003004))
#define TIMER_CHI       ((volatile uint32_t*)(MMIO_BASE+0x00003008))
#define TIMER_C1        ((volatile uint32_t*)(MMIO_BASE+0x00003010))

#define TIMER_CS_M1	(1 << 1)

#endif  // _IO_TIMER_H