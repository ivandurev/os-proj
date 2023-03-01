#ifndef _M_MEM_H
#define _M_MEM_H

extern char __KERNEL_END;

void init_memory();
uint64_t allocate_physical_section();
bool free_physical_section(uint64_t addr);
uint64_t allocate_section(uint64_t *pgd_addr);
bool free_section(uint64_t *pgd_addr, uint64_t va);
extern void mzero(uint64_t start, uint32_t size);

#endif // _M_MEM_H