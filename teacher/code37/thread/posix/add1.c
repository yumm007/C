#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define FNAME "/tmp/count"

#define THRNR 20

#define BUFSIZE 1024

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

static void*
thr_add(void *ptr)
{
	FILE *fp;
	char linebuf[BUFSIZE];

	fp = fopen(FNAME, "r+");
	if (fp==NULL) {
		perror("fopen()");
		exit(1);
	}

	pthread_mutex_lock(&mut);

	if (fgets(linebuf, BUFSIZE, fp)==NULL) {
		perror("fgets()");
		exit(1);
	}
	fseek(fp, 0, SEEK_SET);
	sleep(1);
	fprintf(fp, "%d\n", atoi(linebuf)+1);
	fclose(fp);

	pthread_mutex_unlock(&mut);

	pthread_exit(NULL);
}

int
main()
{
	int i, err;
	pthread_t tid[THRNR];

	for (i=0;i<THRNR;++i) {
		err = pthread_create(tid+i, NULL, thr_add, NULL);
		if (err) {
			fprintf(stderr, "pthread_create(): %s\n", strerror(err));
			exit(1);
		}
	}

	for (i=0;i<THRNR;++i) {
		pthread_join(tid[i], NULL);
	}

	pthread_mutex_destroy(&mut);

	return 0;
}

