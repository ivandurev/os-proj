#ifndef _M_MEM_H
#define _M_MEM_H

// Dynamic memory operations using virtual memory

// linker symbol - address is end of the used space
extern char __KERNEL_END;

// call first before using dynamic memory
void init_memory();

// Responsible for managing physical addresses and their availability (prefixed with 0xffff for kernel use)
uint64_t allocate_physical_page(); 			// 4KB - uses the first 2MB section for 4KB pages
bool free_physical_page(uint64_t addr);
uint64_t allocate_physical_section();		// 2MB
bool free_physical_section(uint64_t addr);

// Responsible for managing virtual address mapping for a given set of page tables - supports covering 1GB memory with 2MB entries
uint64_t allocate_section(uint64_t *pgd_addr, bool user_access);
bool free_section(uint64_t *pgd_addr, uint64_t va);

// Manages the 3 levels of pages for a new process - no actual memory is mapped - see allocate_section
uint64_t allocate_pages();
bool free_pages(uint64_t *pgd_addr);

// Copies a chunk of memory
void mcopy(uint64_t *from_addr, uint32_t size, uint64_t *to_addr);

// Zeroes a chunk of memory
extern void mzero(uint64_t start, uint32_t size);

// Converts a VA to a PA given translation tables
uint64_t read_pa(uint64_t *pgd_addr, uint64_t va);

// allows EL0 read/write/execute, disallows EL1 execute
void allow_user_access(uint64_t *pgd_addr);

#endif // _M_MEM_H