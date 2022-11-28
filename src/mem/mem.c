#include "mem/mem.h"
#include "utils/printf.h"

static bool allocated[PAGES_LEN] = {0};
uint64_t INIT_MEM = 1048576;

uint64_t page_alloc()
{
	for(int i = 0; i < PAGES_LEN; i++)
	{
		if(!allocated[i])
		{
			allocated[i] = true;
			printf("Allocating %d\n", i);
			return INIT_MEM + (uint64_t) (i)*PAGE_SIZE;
		}
	}
	return false;
}

void page_free(uint64_t addr)
{
	printf("Freeing %d\n", (addr - INIT_MEM) / PAGE_SIZE);
	allocated[(addr - INIT_MEM) / PAGE_SIZE] = false;
}