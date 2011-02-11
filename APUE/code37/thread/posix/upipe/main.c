#include <stdio.h>
#include <stdlib.h>

#include "upipe.h"

#define RDTHRNR 1
#define WRTHRNR 1

#define TOTAL 1000000000

#define BUFSIZE 4096

static upipe_t *mypipe;

static char tempbuf[BUFSIZE];

static void*
thr_reader(void *ptr)
{
	char linebuf[BUFSIZE];


	pthread_exit(NULL);
}

static void*
thr_writer(void *ptr)
{
	int i, len, pos, total=0;

	for (i=0;total<TOTAL;++i) {
		len = rand()%BUFSIZE;
		if (total+len >TOTAL) {
			len = TOTAL-total;
		}
		pos=0;
		while (len>0) {
			ret = upipe_write(mypipe, tempbuf+pos, len);
			pos += ret;
			len -= ret;
			total += ret;
		}
	}

	pthread_exit(NULL);
}


int
main()
{
	int i, err;
	pthread_t rdtid[RDTHRNR], wrtid[WRTHRNR];

	for (i=0;i<BUFSIZE;++i) {
		tempbuf[i]='a'+i%26;
	}

	mypipe = upipe_new(1024);
	if (mypipe==NULL) {
		perror("upipe_new()");
		exit(1);
	}

	for (i=0;i<RDTHRNR;++i) {
		err = pthread_create(rdtid+i, NULL, thr_reader, NULL);
		if (err) {
			fprintf(stderr, "pthread_create(): %s\n", strerror(err));
			exit(1);
		}
	}
	for (i=0;i<WRTHRNR;++i) {
		err = pthread_create(wrtid+i, NULL, thr_writer, NULL);
		if (err) {
			fprintf(stderr, "pthread_create(): %s\n", strerror(err));
			exit(1);
		}
	}

	for (i=0;i<RDTHRNR;++i) {
		pthread_join(rdtid[i], NULL);
	}

	for (i=0;i<WRTHRNR;++i) {
		pthread_join(wrtid[i], NULL);
	}

	upipe_delete(mypipe);

	return 0;
}

