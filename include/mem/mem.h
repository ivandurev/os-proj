#ifndef _M_MEM_H
#define _M_MEM_H

#define PAGES_LEN 1000
#define PAGE_SIZE (1 << 12) // 4 KiB

//#define INIT_MEM 262144 // 256 KiB

extern uint8_t _end;

uint64_t page_alloc();
void page_free(uint64_t addr);

#endif // _M_MEM_H