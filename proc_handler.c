#include <stdlib.h>
#include <pthread.h>
#include <uqueue.h>
#include "proc_handler.h"

// local functions
static inline int init_thrd(struct proc_thrd *thrd, unsigned long int qsize, void (*func)(void *), size_t ts);
void *handler();

static inline int init_thrd(struct proc_thrd *thrd, unsigned long int qsize, void (*func)(void *), size_t ts)
{
	uqueue_init(&(thrd->q), qsize, qsize, ts);
	pthread_mutex_init(&(thrd->mtx), NULL);
	pthread_mutex_lock(&(thrd->mtx));
	thrd->vptr = malloc(ts);
	thrd->func = func;
	thrd->paused = 1;
	return 0;
}

void *handler(void *arg)
{
	struct proc_thrd *thrd;
	int ret;

	thrd = (struct proc_thrd *) arg;
	pthread_cond_wait(&(thrd->evnt), &(thrd->mtx));
	while (1) {
		ret = uqueue_get(&(thrd->q), thrd->vptr);
		if (ret == -1) {
			thrd->paused = 1;
			pthread_cond_wait(&(thrd->evnt), &(thrd->mtx));
			continue;
		};
		(thrd->func)(thrd->vptr);
	};

	pthread_exit(0);
}

// global functions
int proc_create(struct proc_handler *p, void (*func)(void *), unsigned long int qsize, unsigned long int nthrds, size_t ts)
{
	struct proc_thrd *thrds;
	struct proc_thrd *cur_thrd;
	unsigned long int i;

	p->nt = nthrds;
	p->ct = 1;
	p->ts = ts;
	pthread_mutex_init(&(p->cl), NULL);
	thrds = malloc(sizeof(struct proc_thrd) * nthrds);
	if (thrds == NULL) return -1;
	p->thrds = thrds;

	for (i = 0; i < nthrds; i++) {
		init_thrd(thrds + i, qsize, func, ts);
	};
	// two loops were used so that error handling can be done before creating threads
	for (i = 0; i < nthrds; i++) {
		cur_thrd = (thrds + i);
		pthread_create(&(cur_thrd->thrd), NULL, &handler, (void *) cur_thrd);
	};

	return 0;
}

int proc_add(struct proc_handler *p, void *vptr)
{
	struct proc_thrd *thrd;
	int ct;

	pthread_mutex_lock(&(p->cl));
	ct = p->ct;
	if (ct == p->nt) {
		ct = 0;	
		p->ct = 1;
	} else {
		p->ct++;
	};
	pthread_mutex_unlock(&(p->cl));

	thrd = p->thrds;
	thrd += ct;
	uqueue_add(&(thrd->q), vptr);
	if (thrd->paused) {
		pthread_mutex_lock(&(thrd->mtx));
		if (thrd->paused) {
			pthread_cond_signal(&(thrd->evnt));
			thrd->paused = 0;
		}
		pthread_mutex_unlock(&(thrd->mtx));
	};

	return 0;
}
