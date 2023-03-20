#include "user/syscall.h"

// All functions are wrappers for the system calls defining their parameters and return values

void printf(char *msg)
{
	__asm__("svc %0" : : "i" (SYS_PRINT));
}
uint64_t malloc()
{
	__asm__("svc %0" : : "i" (SYS_MALLOC));
}
void mfree(uint64_t addr)
{
	__asm__("svc %0" : : "i" (SYS_MFREE));
}
void exit()
{
	__asm__("svc %0" : : "i" (SYS_EXIT));
}