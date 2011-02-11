#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define LEFT 30000000
#define RIGHT 30000200
#define THRNR (RIGHT-LEFT+1)
#define WORKNR 4

static sem_t sem;

static void*
thr_chkprime(void *p)
{
	int mark, j, i;

	i=*(int*)p;
	mark = 1;
	for (j=2;j<i/2;++j) {
		if (i%j==0) {
			mark=0;
			break;
		}
	}
	if (mark==1) {
		printf("%d is a prime.\n", i);
	}
	sem_post(&sem);
	pthread_exit(p);
//	return (p);
}

static void
exitfunc(void)
{
	sem_destroy(&sem);
}

int
main()
{
	int i, j, mark;
	pthread_t tid[THRNR];
	int *argp, err;
	void *retp;

	atexit(exitfunc);

	sem_init(&sem, 0, WORKNR);

	for (i=LEFT;i<=RIGHT;++i) {
		sem_wait(&sem);
		argp = malloc(sizeof(int));
		if (argp==NULL) {
			perror("malloc()");
			exit(1);
		}
		*argp = i;
		err = pthread_create(tid+(i-LEFT), NULL, thr_chkprime, argp);
		if (err) {
			fprintf(stderr, "pthread_create(): %s\n", strerror(err));
			exit(1);
		}
		pthread_detach(tid[i-LEFT]);
	}

	pthread_exit(NULL);

/*
	for (i=LEFT;i<=RIGHT;++i) {
		pthread_join(tid[i-LEFT], &retp);
		free(retp);
	}

	return 0;
*/
}

