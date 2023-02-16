#ifndef _M_MEM_H
#define _M_MEM_H

uint64_t malloc();
void mfree(uint64_t addr);

extern void mzero(uint64_t start, uint32_t size);

#endif // _M_MEM_H