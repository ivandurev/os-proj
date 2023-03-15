#ifndef _MMIO_BASE_H
#define _MMIO_BASE_H

// Base virtual address of MMIO

#include "mem/mem_values.h"

#define MMIO_BASE KERNEL_MEMORY_BASE + DEVICE_OFFSET // virtual address

#endif // _MMIO_BASE_H