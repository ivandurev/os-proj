#ifndef _USER_SYSCALL_H
#define _USER_SYSCALL_H

#define SYS_PRINT 	(0)
#define SYS_MALLOC 	(1)
#define SYS_MFREE 	(2)
#define SYS_EXIT    (3)

void printf(char *msg);
uint64_t malloc();
void exit();

#endif // _USER_SYSCALL_H