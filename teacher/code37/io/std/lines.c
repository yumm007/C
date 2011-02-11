#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main()
{
	FILE *fp;
	static char *bufp;
	static size_t bufsize;
	int count;

	fp = fopen("/etc/hosts", "r");
	if (fp==NULL) {
		perror("fopen()");
		exit(1);
	}

	count = 0;
	bufp = NULL;
	bufsize = 0;
	while (1) {
		if (getline(&bufp, &bufsize, fp)<0) {
			break;
		}
		printf("%d\n", strlen(bufp));
		count++;
	}

	printf("lines = %d\n", count);

	fclose(fp);

	free(bufp);

	return 0;
}

