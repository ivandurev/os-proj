#ifndef _M_MEM_VALUES_H
#define _M_MEM_VALUES_H

#define PAGE_SIZE (1 << 12) // each page is 4KB
#define PAGES_TOTAL_SIZE (3 * (1 << 12)) // we have one PGD, PUD and one PMD

#define PAGE_ENTRIES 512 // 4KB pages

#define SECTION_SIZE (1 << 21)

#define PROC_STACK_SIZE 0x8000 // default stack size for user processes - 32KB

#define KERNEL_MEMORY_BASE 0xffff000000000000 // virtual kernel space start
#define KERNEL_STACK_SIZE 0x80000
#define DEVICE_OFFSET 0x3f000000 // mmio offset
#define MEMORY_END_OFFSET (1 << 30) // 1GB

#define DESCRIPTOR_KERNEL_BITMASK 0xffff000000000000 // use to preserve / remove the kernel memory bits
#define DESCRIPTOR_ADDR_BITMASK (DESCRIPTOR_KERNEL_BITMASK | ((((uint64_t) 1 << (48 - 12)) - 1) << 12)) // bitmask for address bits
#define DESCRIPTOR_VALID_BITMASK (DESCRIPTOR_KERNEL_BITMASK | (1 << 0)) // find if an entry is valid

#define VA_PMD_SHIFT 21
#define VA_PMD_BITMASK (0x1ff << VA_PMD_SHIFT)

#define PGD_DESCRIPTOR_VALID_TABLE 			(3 << 0) // descriptor flags at PGD LEVEL of va translation
#define PUD_DESCRIPTOR_VALID_TABLE 		  	(3 << 0) // PUD LEVEL
#define PMD_DESCRIPTOR_VALID_BLOCK_NORMAL 	(1 << 10 | 1 << 2 | 1 << 0) // PMD level - access flag, mair attribute number, valid block
#define PMD_DESCRIPTOR_VALID_BLOCK_DEVICE 	(1 << 10 | 0 << 2 | 1 << 0) // PMD level - access flag, mair attribute number, valid block

#endif // _M_MEM_VALUES_H