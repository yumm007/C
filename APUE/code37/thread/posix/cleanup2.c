#include <stdio.h>
#include <errno.h>
#include <pthread.h>

static void
clean_freeheap(void*p)
{
	free(p);
}

static void
clean_closefd(int *p)
{
	close((int)p);
}

static void*
thr(void *p)
{
	void *x, *y;
	int fd;

	x = malloc();
	pthread_cleanup_push(clean_freeheap, x);
	...;
	y = malloc();
	pthread_cleanup_push(clean_freeheap, y);
	...;
	fd = open();
	pthread_cleanup_push(clean_closefd, (void*)fd);

	...;

	pthread_cleanup_pop(1); //close(fd);
	pthread_cleanup_pop(1); //free(y);
	pthread_cleanup_pop(1); //free(x);
	pthead_exit (NULL);
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
	sleep(2);
	pthread_cancel(tid);
	pthread_join(tid, NULL);
	puts("After pthread_create().");
	return 0;
}

