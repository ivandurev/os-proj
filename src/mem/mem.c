#include "mem/mem.h"
#include "mem/mem_values.h"
#include "utils/printf.h"

static bool allocated[PAGE_ENTRIES]; // makes up for 1gb of 2mb sections
uint64_t data_start_addr;

void init_memory()
{
	data_start_addr = ((((uint64_t) (&__KERNEL_END)) >> 12) + 1) << 12;
}
uint64_t allocate_physical_section()
{
	for(int i = 0; i < PAGE_ENTRIES; i ++)
	{
		if(!allocated[i])
		{
			allocated[i] = true;
			uint64_t pa = data_start_addr + i * SECTION_SIZE;
			if(pa >= MEMORY_END_OFFSET / 2)
				return false; // check if we exceed memory - 512MB
			return pa; // note it's a kernel address - leading ffff
		}
	}
	return false;
}
bool free_physical_section(uint64_t addr)
{
	addr |= DESCRIPTOR_KERNEL_BITMASK;
	int idx = (addr - data_start_addr) / SECTION_SIZE;
	if(idx >= 0 && idx < PAGE_ENTRIES)
	{
		allocated[idx] = false;
		return true;
	}
	return false;
}

uint64_t allocate_section(uint64_t *pgd_addr)
{
	if(pgd_addr == NULL)
	{
		return false;	
	}
	uint64_t *pud_addr = (uint64_t *) ((*pgd_addr) & DESCRIPTOR_ADDR_BITMASK);
	uint64_t *pmd_addr = (uint64_t *) ((*pud_addr) & DESCRIPTOR_ADDR_BITMASK);

	for(int i = 8; i < PAGE_SIZE; i += 8)
	{
		if(*(pmd_addr + i) & DESCRIPTOR_VALID_BITMASK)
			continue;
		uint64_t descriptor = allocate_physical_section();
		if(!descriptor)
			return false;

		descriptor ^= DESCRIPTOR_KERNEL_BITMASK; // remove ffff
		descriptor |= PMD_DESCRIPTOR_VALID_BLOCK_NORMAL;
		*(pmd_addr + i) = descriptor;

		return i << VA_PMD_SHIFT;
	}
	return false;
}

bool free_section(uint64_t *pgd_addr, uint64_t va)
{
	uint64_t *pud_addr = (uint64_t *) ((*pgd_addr) & DESCRIPTOR_ADDR_BITMASK);
	uint64_t *pmd_addr = (uint64_t *) ((*pud_addr) & DESCRIPTOR_ADDR_BITMASK);

	uint16_t entry = (va & VA_PMD_BITMASK) >> VA_PMD_SHIFT;

	uint64_t pa = (*(pmd_addr + entry)) & DESCRIPTOR_ADDR_BITMASK;
	pa |= DESCRIPTOR_KERNEL_BITMASK;

	bool res = free_physical_section(pa);
	if(!res) return false;

	*(pmd_addr + entry) = 0;
	return true;
}
