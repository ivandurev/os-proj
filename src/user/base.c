#include "user/syscall.h"

void idle()
{
	while(1) {}
}

// gets called automatically when a program exits
void on_return()
{
	exit();
	idle();
}

void first_func()
{
	while(1)
	{
		printf("123 Hello!\n");
	}
}