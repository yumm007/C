#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LEFT	 30000001
#define RIGHT 	 30000002
#define THRNR 	 80

static int num=0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t mut_cond = PTHREAD_COND_INITIALIZER;

static void*
thr_chkprime(void *p)
{
	int mark, j, i;

	while (1) {
		pthread_mutex_lock(&mut_num);
		while (num==0) {
			pthread_cond_wait(&mut_cond, &mut_num);
		}
		i=num;
		if (i<0) {
			pthread_mutex_unlock(&mut_num);
			pthread_exit(NULL);
		}
		num=0;
		pthread_cond_broadcast(&mut_cond);
		pthread_mutex_unlock(&mut_num);

		mark = 1;
		for (j=2;j<i/2;++j) {
			if (i%j==0) {
				mark=0;
				break;
			}
		}
		if (mark==1) {
			printf("[%d]: %d is a prime.\n", (int)p, i);
		}
	}
	pthread_exit(p);
}

int
main()
{
	int i;
	pthread_t tid[THRNR];

	for (i=0;i<THRNR;++i) {
		pthread_create(tid+i, NULL, thr_chkprime, (void*)i);
	}

	sleep(2);

	for (i=LEFT;i<=RIGHT;++i) {
		pthread_mutex_lock(&mut_num);
		while (num!=0) {
			pthread_cond_wait(&mut_cond, &mut_num);
		}
		num=i;
		pthread_cond_signal(&mut_cond);
		pthread_mutex_unlock(&mut_num);
	}

	pthread_mutex_lock(&mut_num);
	while (num!=0) {
		pthread_cond_wait(&mut_cond, &mut_num);
	}
	num=-1;
	pthread_cond_broadcast(&mut_cond);
	pthread_mutex_unlock(&mut_num);

	for (i=0;i<THRNR;++i) {
		pthread_join(tid[i], NULL);
		printf("join %d OK\n", i);
	}

	return 0;
}

