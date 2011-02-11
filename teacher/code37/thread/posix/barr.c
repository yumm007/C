#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THRNR 4

static int semnum[THRNR];
static pthread_mutex_t mut_sem;
static pthread_cond_t cond_sem;

static void*
thr_loop(void *p)
{
	int c, n;

	n=(int)p;
	c='a'+n;
	while (1) {
		pthread_mutex_lock(&mut_sem);
		while (semnum[n]<=0)
			pthread_cond_wait(&cond_sem, &mut_sem);
		semnum[n]--;
		write(1, &c, 1);
		pthread_cond_broadcast(&cond_sem);
		pthread_mutex_unlock(&mut_sem);

		usleep(rand()%500);
	}
	pthread_exit(p);
}

static int
sum(void)
{
	int i, sum=0;
	for (i=0;i<THRNR;++i) {
		sum += semnum[i];
	}
	return sum;
}

static void
postall(void)
{
	int i;
	for (i=0;i<THRNR;++i) {
//		printf("[%d]", semnum[i]);
		semnum[i]++;
	}
//	printf("\n");
	return;
}

int
main()
{
	int i, j, mark;
	pthread_t tid[THRNR];

	for (i=0;i<THRNR;++i) {
		semnum[i]=0;
	}
	pthread_mutex_init(&mut_sem, NULL);
	pthread_cond_init(&cond_sem, NULL);

	for (i=0;i<THRNR;++i) {
		pthread_create(tid+i, NULL, thr_loop, (void*)i);
	}

	while (1) {
		pthread_mutex_lock(&mut_sem);
		while (sum()>0)
			pthread_cond_wait(&cond_sem, &mut_sem);
		postall();
		write(1, "\n", 1);
		pthread_cond_broadcast(&cond_sem);
		pthread_mutex_unlock(&mut_sem);
	}

	alarm(15);
	while (1) pause();

	return 0;
}

