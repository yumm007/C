#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem2.h"

#define MAXLINE 512

int main(void)
{
	void *mem;
	char *str[60], *str2[30];
	int size, i, j;
	MEMLLIST *memllist;

	if ((mem = malloc((1 << 10) * 100)) == NULL) {
		fprintf(stderr, "malloc memory faild\n");
		return -1;
	}

	if ((memllist =(MEMLLIST *) init_mem(mem, (1 << 10) * 100)) == NULL) {
		fprintf(stderr, "size is not enough. %d <= %d\n",size, sizeof(*memllist) + sizeof(int));
		return -1;
	}

	for (i = 0; i < 60; i++ ) {
		if ((str[i] = request_mem(memllist, MAXLINE)) == NULL) {
			printf("request mem for str[%d] faild\n", i);
			getchar();
			break;
		}
		fgets(str[i], MAXLINE, stdin);
	}
	
	for (i = 0; i < 60; i++)
		printf("%s", str[i]);

	for (i = 0; i < 60; i += 2)
		free_mem(memllist, str[i]);

	travel_free_mem(memllist);

#if 0
	printf("request mem for str2[1]\n");
	if ((str2[1] = request_mem(memllist, MAXLINE - 10)) == NULL) {
		printf("request mem for str2[%d] faild\n", i);
		getchar();
	}
	fgets(str2[1], MAXLINE - 10, stdin);

	printf("request mem for str2[2]\n");
	if ((str2[2] = request_mem(memllist, MAXLINE - 10)) == NULL) {
		printf("request mem for str2[%d] faild\n", i);
		getchar();
	}
	fgets(str2[2], MAXLINE - 10, stdin);

	if ((str2[3] = request_mem(memllist, MAXLINE - 10)) == NULL) {
		printf("request mem for str3[%d] faild\n", i);
		getchar();
	}
	fgets(str2[3], MAXLINE - 10, stdin);
#else
	for (i = 0; i < 20; i++ ) {
		if ((str2[i] = request_mem(memllist, MAXLINE)) == NULL) {
			printf("request mem for str2[%d] faild\n", i);
			getchar();
			break;
		}
		fgets(str2[i], MAXLINE, stdin);
	}
	travel_free_mem(memllist);
#endif
	return 0;
}
