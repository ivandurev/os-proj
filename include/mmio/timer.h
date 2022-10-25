#ifndef	_IO_TIMER_H
#define	_IO_TIMER_H

#include "mmio/base.h"

#define TIMER_CS        ((volatile unsigned int*)(MMIO_BASE+0x00003000))
#define TIMER_CLO       ((volatile unsigned int*)(MMIO_BASE+0x00003004))
#define TIMER_CHI       ((volatile unsigned int*)(MMIO_BASE+0x00003008))
#define TIMER_C1        ((volatile unsigned int*)(MMIO_BASE+0x00003010))

#define TIMER_CS_M1	(1 << 1)

#endif  // _IO_TIMER_H