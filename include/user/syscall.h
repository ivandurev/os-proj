#ifndef _USER_SYSCALL_H
#define _USER_SYSCALL_H

// System Calls definitions

// System calls numbers
#define SYS_PRINT 	(0) // print a string
#define SYS_MALLOC 	(1) // get 2MB memory section
#define SYS_MFREE 	(2) // free the section
#define SYS_EXIT    (3) // exit the process (calls the return function)

// System call methods corresponding to the above numbers
void printf(char *msg);
uint64_t malloc();
void mfree(uint64_t addr);
void exit();

#endif // _USER_SYSCALL_H