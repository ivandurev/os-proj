#include "mem/mem.h"
#include "mem/mem_values.h"
#include "utils/printf.h"

static bool allocated_section[PAGE_ENTRIES]; // makes up for 1gb of 2mb sections
static bool allocated_page[PAGE_ENTRIES]; // one 2MB section will be split in 4kB segments
uint64_t data_start_addr; // the calculated start of "free" space is here - anything before is occupied by kernel code

// Note that it is expected VM pages are exactly three - 1 for each level

void init_memory()
{
	// align to 4096 increments
	data_start_addr = ((((uint64_t) (&__KERNEL_END)) >> 21) + 1) << 21;
	allocated_section[0] = true; // reserved for 4kB pages
}

// return a kernel address for a 4KB page
uint64_t allocate_physical_page()
{
	for(int i = 0; i < PAGE_ENTRIES; i ++)
	{
		if(!allocated_page[i])
		{
			allocated_page[i] = true;

			// offset page from kernel end of code
			uint64_t pa = data_start_addr + i * PAGE_SIZE;

			if((pa ^ DESCRIPTOR_KERNEL_BITMASK) >= MEMORY_END_OFFSET / 2)
				return false; // check if we exceed memory - 512MB

			mzero(pa, PAGE_SIZE >> 3);
			return pa; // note it's a kernel address - leading ffff
		}
	}
	return false;
}

bool free_physical_page(uint64_t addr)
{
	addr |= DESCRIPTOR_KERNEL_BITMASK;
	int idx = (addr - data_start_addr) / PAGE_SIZE;
	if(idx >= 0 && idx < PAGE_ENTRIES)
	{
		allocated_page[idx] = false;
		return true;
	}
	return false;
}

// same as for the page but allocating 2MB sections, ignoring the 1st one
uint64_t allocate_physical_section()
{
	for(int i = 1; i < PAGE_ENTRIES; i ++)
	{
		if(!allocated_section[i])
		{
			allocated_section[i] = true;
			uint64_t pa = data_start_addr + i * SECTION_SIZE;
			if((pa ^ DESCRIPTOR_KERNEL_BITMASK) >= MEMORY_END_OFFSET / 2)
				return false; // check if we exceed memory - 512MB
			mzero(pa, SECTION_SIZE >> 3);
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
		allocated_section[idx] = false;
		return true;
	}
	return false;
}

// converts a VA to a kernel PA for a given set of tables
uint64_t read_pa(uint64_t *pgd_addr, uint64_t va)
{
	uint64_t *pud_addr = (uint64_t *) (((*pgd_addr) & DESCRIPTOR_ADDR_BITMASK) | DESCRIPTOR_KERNEL_BITMASK);
	uint64_t *pmd_addr = (uint64_t *) (((*pud_addr) & DESCRIPTOR_ADDR_BITMASK) | DESCRIPTOR_KERNEL_BITMASK);

	uint64_t section_offset = va & VA_SECTION_OFFSET_BITMASK;
	uint64_t entry = (va & VA_PMD_BITMASK) >> VA_PMD_SHIFT;
	// account that each entry takes 8 bytes
	uint64_t pa = (*(pmd_addr + (entry))) & DESCRIPTOR_ADDR_BITMASK;
	pa += section_offset;

	return pa | DESCRIPTOR_KERNEL_BITMASK;
}

// create the 3 pages for mapping memory - no entries are mapped
uint64_t allocate_pages()
{
	// storing the pages for the new memory space
	uint64_t *pgd_addr 	= (uint64_t *) allocate_physical_page();
	uint64_t *pud_addr 	= (uint64_t *) allocate_physical_page();
	uint64_t *pmd_addr 	= (uint64_t *) allocate_physical_page();
	if(!pgd_addr || !pud_addr || !pmd_addr)
		return false;

	*pgd_addr = ((uint64_t) pud_addr ^ DESCRIPTOR_KERNEL_BITMASK) | PGD_DESCRIPTOR_VALID_TABLE;
	*pud_addr = ((uint64_t) pmd_addr ^ DESCRIPTOR_KERNEL_BITMASK) | PUD_DESCRIPTOR_VALID_TABLE;
	return (uint64_t) pgd_addr;	
}

bool free_pages(uint64_t *pgd_addr)
{
	uint64_t *pud_addr = (uint64_t *) (((*pgd_addr) & DESCRIPTOR_ADDR_BITMASK) | DESCRIPTOR_KERNEL_BITMASK);
	uint64_t *pmd_addr = (uint64_t *) (((*pud_addr) & DESCRIPTOR_ADDR_BITMASK) | DESCRIPTOR_KERNEL_BITMASK);

	for(int i = 0; i < PAGE_ENTRIES; i ++)
	{
		if(*(pmd_addr + i) & DESCRIPTOR_VALID_BITMASK)
			if(!free_section(pgd_addr, i << VA_PMD_SHIFT))
				return false;
	}

	if(!free_physical_page((uint64_t) pmd_addr))
		return false;

	if(!free_physical_page((uint64_t) pud_addr))
		return false;

	if(!free_physical_page((uint64_t) pgd_addr))
		return false;

	return true;
}

// add an entry to a set of pages for EL1/El0
uint64_t allocate_section(uint64_t *pgd_addr, bool user_access)
{
	uint64_t *pud_addr = (uint64_t *) (((*pgd_addr) & DESCRIPTOR_ADDR_BITMASK) | DESCRIPTOR_KERNEL_BITMASK);
	uint64_t *pmd_addr = (uint64_t *) (((*pud_addr) & DESCRIPTOR_ADDR_BITMASK) | DESCRIPTOR_KERNEL_BITMASK);

	// ignoring first entry due to reserved false value
	for(int i = 1; i < PAGE_ENTRIES; i ++)
	{
		if(*(pmd_addr + i) & DESCRIPTOR_VALID_BITMASK)
			continue;
		uint64_t descriptor = allocate_physical_section();
		if(!descriptor)
			return false;

		descriptor ^= DESCRIPTOR_KERNEL_BITMASK; // remove ffff
		descriptor |= PMD_DESCRIPTOR_VALID_BLOCK_NORMAL;

		// if user code is to be located here we need to allow access
		if(user_access)
			descriptor |= PMD_DESCRIPTOR_AP_EL0RWE;

		// save the entry
		*(pmd_addr + i) = descriptor;

		// return the virtual address
		return i << VA_PMD_SHIFT;
	}
	return false;
}

bool free_section(uint64_t *pgd_addr, uint64_t va)
{
	uint64_t pa = read_pa(pgd_addr, va);

	bool res = free_physical_section(pa);
	if(!res) return false;

	*((uint64_t *) pa) = 0;
	return true;
}

void mcopy(uint64_t *from_addr, uint32_t size, uint64_t *to_addr)
{
	for(; size > 0; size --)
	{
		*to_addr = *from_addr;
		to_addr ++; // copy 64 bits each time
		from_addr ++;
	}
}

// allow user access for all of a mapping defined by a set of pages
void allow_user_access(uint64_t *pgd_addr)
{
	uint64_t *pud_addr = (uint64_t *) (((*pgd_addr) & DESCRIPTOR_ADDR_BITMASK) | DESCRIPTOR_KERNEL_BITMASK);
	uint64_t *pmd_addr = (uint64_t *) (((*pud_addr) & DESCRIPTOR_ADDR_BITMASK) | DESCRIPTOR_KERNEL_BITMASK);

	// ignoring first entry due to false value
	for(int i = 1; i < PAGE_ENTRIES; i ++)
	{
		if(*(pmd_addr + i) & DESCRIPTOR_VALID_BITMASK)
		{
			*(pmd_addr + i) |= PMD_DESCRIPTOR_AP_EL0RWE;
		}
	}
}