#include <stdio.h>
#include <errno.h>
#include <pthread.h>

static void*
thr(void *p)
{
	puts("Thread is running.");
	return NULL;
}

int
main()
{
	pthread_t tid;
	int err;

	puts("Before pthread_create().");
	err = pthread_create(&tid, NULL, thr, NULL);
	if (err) {
		fprintf(stderr, "pthread_create(): %s\n", strerror(err));
		return 1;
	}
	puts("After pthread_create().");
	pthread_exit(NULL);
}

