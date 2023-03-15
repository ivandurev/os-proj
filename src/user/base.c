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
	printf("123 Hello!\n");
	printf("will return\n");
}
void second_func()
{
	printf("\t\t\t456\n");
}