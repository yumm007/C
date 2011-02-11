#include <stdio.h>
#include <errno.h>
#include <pthread.h>

static void
clean_func(void*p)
{
	puts(p);
}

static void*
thr(void *p)
{
	pthread_cleanup_push(clean_func, "aaa");
	{
		pthread_cleanup_push(clean_func, "bbb");
		pthread_cleanup_push(clean_func, "ccc");
		puts("Thread is running.");
//		pthead_exit (NULL);
		while (1) pause();
		pthread_cleanup_pop(0);
		pthread_cleanup_pop(0);
	}
	pthread_cleanup_pop(0);
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

