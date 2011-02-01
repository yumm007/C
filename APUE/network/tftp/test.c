#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "hash_link.h"

#define BUFSIZE	256
#define PTHDNR		10

static int cmp(const void *data1, const void *data2) {
	return memcmp(data1, data2, BUFSIZE);
}

static void test(const HASHLINK *link) {
	char randbuf[BUFSIZE];
	int i, j;
	HL_DATA data;

	data.key = randbuf;
	data.key_size = BUFSIZE;
	data.data = randbuf;
	data.data_size = BUFSIZE;

	srand(getpid());
	for (i = 0; i < BUFSIZE; i++) {
		for (j = 0; j < BUFSIZE; j++) {
			randbuf[j] = 'a' + rand() % 26;
			srand(rand() % 101);
		}
		randbuf[j-1] = '\0';
		data.data = randbuf;
		if (link_add_node(link, &data) != 0 )
			fprintf(stderr, "add failed\n");
		if (link_fnd_node(link, &data) != 0 )
			fprintf(stderr, "fnd failed\n");
		if (link_del_node(link, &data) != 0 )
			fprintf(stderr, "dele failed\n");
	}
}

void *pth_fun(void *arg) {
	int i;
	HASHLINK *link;

	for (i = 0; i < 100; i++) {
		if ((link = link_init(2 << 16, cmp)) == NULL) {
			fprintf(stderr, "init error.\n");
			continue;
		}
		test(link);
		link_dstr(link);
		fprintf(stderr, "pthd = %d, i = %d\n", (int)arg, i);
	}

	pthread_exit(NULL);
}

int main(void)
{
	int i;
	pthread_t pthd[PTHDNR];

	for (i = 0; i < PTHDNR; i++) 
		pthread_create(pthd + i, NULL, pth_fun, (void *)i);
	for (i = 0; i < PTHDNR; i++) 
		pthread_join(pthd[i], NULL);

	return 0;
}
