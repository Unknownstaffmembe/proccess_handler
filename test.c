#include <stdio.h>
#include "proc_handler.h"

void proccess(void *arg)
{
	printf("Hello, World!\n");
	return;
}

int main()
{
	struct proc_handler p;
	void *ptr;
	ptr = NULL;
	
	proc_create(&p, &proccess, 10, 4, sizeof(void *));
	proc_add(&p, &ptr);

	pthread_exit(0);
}
