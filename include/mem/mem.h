#ifndef _M_MEM_H
#define _M_MEM_H

extern char __KERNEL_END;

void init_memory();

uint64_t allocate_physical_page();
bool free_physical_page(uint64_t addr);
uint64_t allocate_physical_section();
bool free_physical_section(uint64_t addr);
uint64_t allocate_section(uint64_t *pgd_addr);
bool free_section(uint64_t *pgd_addr, uint64_t va);
uint64_t allocate_pages();
bool free_pages(uint64_t *pgd_addr);
void mcopy(uint64_t *from_addr, uint32_t size, uint64_t *to_addr);

uint64_t read_pa(uint64_t *pgd_addr, uint64_t va);
extern void mzero(uint64_t start, uint32_t size);

#endif // _M_MEM_H