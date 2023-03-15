#include "user/syscall.h"

void idle()
{
	while(1) {}
}

// gets called automatically when a program exits
void on_return()
{
	printf("EXITING\n");
	exit();
	idle();
}

void first_func()
{
	char *buf = (char *) malloc();
	if(!buf)
		printf("Error\n");

	for(int i = 0; i < 10; i ++)
	{
		*(buf + i) = ('a' + i);
	}
	*(buf + 10) = '\n';
	printf(buf);

	mfree((uint64_t) buf);
	printf("Freed the memory\n");
}
void second_func()
{
	for(int i = 0; i < 1000; i ++)
	{
		printf("\t\t\t456\n");
	}	
}