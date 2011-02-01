#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define	PTH_MAX		100000

void *pth_fun(void *arg) {
	int i;
	i++;

	pthread_exit(NULL);
}

int main(void)
{
	pthread_t pthd;
	int i;

	for (i = 0; i < PTH_MAX; i++) {
		pthread_create(&pthd, NULL, pth_fun, NULL);
		pthread_join(pthd, NULL);
	}

	return 0;
}
