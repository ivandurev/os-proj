#include "user/syscall.h"

void idle()
{
	while(1) {}
}

void first_func()
{
	while(1)
	{
		printf("123 Hello!\n");
	}
}