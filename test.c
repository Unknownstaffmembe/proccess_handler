#include <stdio.h>
#include "proc_handler.h"

void proccess(void *arg)
{
	int *i;

	i = (int *) arg;
	printf("%d\n", *i);

	return;
}

int main()
{
	struct proc_handler p;
	int i, n;

	n = 100000;
	
	proc_create(&p, &proccess, 10, 4, sizeof(long int));
	for (i = 0; i < 4; i++) {
		proc_add(&p, &n);
	};

	pthread_exit(0);
}
