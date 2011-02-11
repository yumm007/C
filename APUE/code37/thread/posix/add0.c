#include <stdio.h>
#include <stdlib.h>

#define FNAME "/tmp/count"

#define LOOPNR 20
#define BUFSIZE 1024

static int
do_add(void)
{
	FILE *fp;
	char linebuf[BUFSIZE];

	fp = fopen(FNAME, "r+");
	if (fp==NULL) {
		perror("fopen()");
		exit(1);
	}
	if (fgets(linebuf, BUFSIZE, fp)==NULL) {
		perror("fgets()");
		exit(1);
	}
	fseek(fp, 0, SEEK_SET);
	fprintf(fp, "%d\n", atoi(linebuf)+1);
	fclose(fp);
	return 0;
}

int
main()
{
	int i;

	for (i=0;i<LOOPNR;++i) {
		do_add();
	}

	return 0;
}
