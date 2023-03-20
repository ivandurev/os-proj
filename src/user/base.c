#include "user/syscall.h"

// idle function for when the CPU has nothing to do
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
	// test a malloc syscall
	char *buf = (char *) malloc();
	if(!buf)
		printf("Error\n");

	// write out the first 10 characters
	for(int i = 0; i < 10; i ++)
	{
		*(buf + i) = ('a' + i);
	}
	*(buf + 10) = '\n';
	printf(buf);

	// test freeing memory
	mfree((uint64_t) buf);
	printf("Freed the memory\n");


	// continue to demonstrate concurrency
	for(int i = 0; i < 1000; i ++)
	{
		printf("123\n");
	}
}
void second_func()
{
	for(int i = 0; i < 1000; i ++)
	{
		printf("\t\t\t456\n");
	}	
}

void third_func()
{
	for(int i = 0; i < 1000; i ++)
	{
		printf("\t\t\t\t\t\t789\n");
	}	
}