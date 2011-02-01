#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "pth_queue.h"

#define	PTHQ_BUF		256
#define	BUFSIZE		1024
#define	PTH_RNUM		200
#define	PTH_WNUM		200
#define	WLOOP			23


PTHQ *pthq;

void *pth_wr(void *arg) {
	int i;
	char buf[BUFSIZE];
	
	srand(getpid());
	for (i = rand() % 256; i > 0; i--) {
		sprintf(buf, "add word %c in pth %d\n", 'a' + rand() % 26, (int)arg);
		pthq_write(pthq, (void *)buf, strlen(buf) + 1);
	}	

	pthread_exit(NULL);
}

void *pth_rd(void *arg) {
	char buf[BUFSIZE];
	
	while(pthq_read(pthq, (void *)buf, BUFSIZE) != EOF)
		;
	pthread_exit(NULL);
}


int main(void)
{
	pthread_t pthr[PTH_RNUM], pthw[PTH_WNUM];
	int i, pth_rnum, pth_wnum;

	srand(getpid());
	pth_rnum = rand() % 99 + 1;
	srand(pth_rnum);
	pth_wnum = rand() % 99 + 1;

	if ((pthq = pthq_open(PTHQ_BUF)) == NULL) {
		fprintf(stderr, "init pthq failed.\n");
		return -1;
	}
	
	for (i = 0; i < pth_rnum; i++) {
		pthread_create(&pthr[i],NULL, pth_rd, (void *)i);
	}
	for (i = 0; i < pth_wnum; i++) {
		pthread_create(&pthw[i],NULL, pth_wr, (void *)i);
	}

	for (i = 0; i < pth_wnum; i++) {
		pthread_join(pthw[i], NULL);
	}
	pthq_close_wr(pthq);

	for (i = 0; i < pth_rnum; i++) {
		pthread_join(pthr[i], NULL);
	}

	pthq_delete(pthq);
	return 0;
}
