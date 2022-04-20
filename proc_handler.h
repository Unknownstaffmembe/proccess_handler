#ifndef PROC_HANDLER_INCLUDED
#define PROC_HANDLER_INCLUDED
// process handler
#include <pthread.h>
#include <uqueue.h>

struct proc_handler;
struct proc_thrd;

extern int proc_create(struct proc_handler *p, void (*func)(void *), unsigned long int qsize, unsigned long int nthrds, size_t ts); // process handler struct, individual queue size (per thread), number of threads, type size
extern int proc_add(struct proc_handler *p, void *vptr);

struct proc_handler {
	struct proc_thrd *thrds;
	pthread_mutex_t cl; // counter lock
	unsigned int nt, ct; // number of threads, current thread
	size_t ts; // type size
};

struct proc_thrd {
	struct uqueue q; // queue
	pthread_t thrd; // thread
	pthread_cond_t evnt; // event
	pthread_mutex_t mtx; // event mutex (used to indicate whether evnt is being used to pause a thread)
	void (*func)(void *);
	void *vptr; // value pointer (used to store values which will be passed to the handler ufnction)
	char paused; // indicated whether the thread thrd is paused
};

#endif
