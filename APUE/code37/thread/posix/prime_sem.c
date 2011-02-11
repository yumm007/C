#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define LEFT 30000000
#define RIGHT 30000200
#define THRNR (RIGHT-LEFT+1)
#define WORKNR 4

static sem_t sem;
static pthread_t tid[THRNR];
static pthread_t gc_tid;

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

static void*
thr_gc(void *unused)
{
	int i;
	void *retp;

	while (1) {
		queue_read();
		pthread_join(tid[i], &retp);
		free(retp);
	}
}

int
main()
{
	int i, j, mark;
	int *argp, err;

	atexit(exitfunc);

	pthread_create(gc_tid, NULL, thr_gc, NULL);

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

