#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LEFT 30000000
#define RIGHT 30000200
#define THRNR (RIGHT-LEFT+1)

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
	pthread_exit(p);
//	return (p);
}

int
main()
{
	int i, j, mark;
	pthread_t tid[THRNR];
	int *argp;
	void *retp;

	for (i=LEFT;i<=RIGHT;++i) {
		argp = malloc(sizeof(int));
		if (argp==NULL) {
			perror("malloc()");
			exit(1);
		}
		*argp = i;
		pthread_create(tid+(i-LEFT), NULL, thr_chkprime, argp);
	}

	for (i=LEFT;i<=RIGHT;++i) {
		pthread_join(tid[i-LEFT], &retp);
		free(retp);
	}

	return 0;
}

