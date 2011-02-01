#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "pth_stack.h"

#define	PTHS_BUF		256
#define	BUFSIZE		1024
#define	PTH_MAX_RNUM		100
#define	PTH_MAX_WNUM		100


PTHS *pths;

void *pth_wr(void *arg) {
	int i;
	char buf[BUFSIZE];
	
	srand(getpid());
	for (i = rand() % 255 + 1; i > 0; i--) {
		sprintf(buf, "add word %c in pth %d\n", 'a' + rand() % 26, (int)arg);
		pths_push(pths, (void *)buf, strlen(buf) + 1);
	}	

	pthread_exit(NULL);
}

void *pth_rd(void *arg) {
	char buf[BUFSIZE];
	
	while(pths_pop(pths, (void *)buf, BUFSIZE) != E_EMPTY)
		printf("in main_rd: %s", buf);
	pthread_exit(NULL);
}


int main(void)
{
	pthread_t pthr[PTH_MAX_RNUM], pthw[PTH_MAX_WNUM];
	int i, pth_rnum, pth_wnum;

	srand(getpid());
	pth_rnum = rand() % PTH_MAX_RNUM + 1;
	srand(pth_rnum);
	pth_wnum = rand() % PTH_MAX_WNUM + 1;

	if ((pths = pths_open(PTHS_BUF)) == NULL) {
		fprintf(stderr, "init pthq failed.\n");
		return -1;
	}
	
	for (i = 0; i < pth_wnum; i++) {
		pthread_create(&pthw[i],NULL, pth_wr, (void *)i);
	}
	sleep(1);

	for (i = 0; i < pth_rnum; i++) {
		pthread_create(&pthr[i],NULL, pth_rd, (void *)i);
	}

	for (i = 0; i < pth_wnum; i++) {
		pthread_join(pthw[i], NULL);
	}
	pths_close_wr(pths);

	for (i = 0; i < pth_rnum; i++) {
		pthread_join(pthr[i], NULL);
	}

	pths_close(pths);
	return 0;
}
