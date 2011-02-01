#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#ifndef STACHS_SIZE
#define STACHS_SIZE	16
#endif

static void *fun(void *unused) {
	struct timespec tim;

	tim.tv_sec = 10;
	tim.tv_nsec = 0;

	nanosleep(&tim, NULL);

	pthread_exit(NULL);
}

int main(void)
{
	size_t i = 0;
	int ret;
	pthread_t pthd;
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);
	if (pthread_attr_setstacksize(&attr, STACHS_SIZE<<10) != 0)
		printf("set stach size failed.\n");
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	while (++i) 
		if ((ret = pthread_create(&pthd, &attr, fun, NULL)) != 0) {
			break;
		}

	printf("STACHS_SIZE = %d K, pthread_num = %d\n", STACHS_SIZE, i);

	pthread_attr_destroy(&attr);
	return i;
}
