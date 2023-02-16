#ifndef _M_MEM_VALUES_H
#define _M_MEM_VALUES_H

#define PAGE_SIZE (1 << 12) // each page is 4KB
#define PAGES_TOTAL_SIZE 3 * (1 << 12) // we have one PGD, PUD and one PMD

#define KERNEL_MEMORY_BASE 0xFFFF000000000000 // virtual kernel space start
#define KERNEL_STACK_SIZE 0x80000
#define DEVICE_OFFSET 0x3F000000 // mmio offset
#define MEMORY_END_OFFSET (1 << 30) // 1GB

#endif // _M_MEM_VALUES_H