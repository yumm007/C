#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THRNR 4

static pthread_mutex_t mut[THRNR];

static int
next(int n)
{
	n++;
	if (n==THRNR) {
		return 0;
	}
	return n;
}

static void*
thr_loop(void *p)
{
	int c, n;

	n=(int)p;
	c='a'+n;
	while (1) {
		pthread_mutex_lock(mut+n);
		write(1, &c, 1);
		pthread_mutex_unlock(mut+next(n));
	}
	pthread_exit(p);
}

int
main()
{
	int i, j, mark;
	pthread_t tid[THRNR];

	for (i=0;i<THRNR;++i) {
		pthread_mutex_init(mut+i, NULL);
		pthread_mutex_lock(mut+i);
		pthread_create(tid+i, NULL, thr_loop, (void*)i);
	}

	pthread_mutex_unlock(mut+0);

/*
	for (i=0;i<THRNR;++i) {
		pthread_join(tid[i], NULL);
	}
*/
	alarm(5);
	while (1) pause();

	return 0;
}

