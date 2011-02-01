#include <stdio.h>
#include <unistd.h>

#ifndef BUFSIZE
#define BUFSIZE 1024
#endif

static size_t buf_nil(int *buf) {
	int i;
	for (i = 0; buf[i] == 0 && i < BUFSIZE; i++)
		;
	return i;
}

static off_t flen(FILE *fp) {
	off_t old, len;

	old = ftell(fp);
	fseeko(fp, 0, SEEK_END);
	len = ftell(fp);
	fseeko(fp, old, SEEK_SET);

	return len;
}

static int fast_cp(FILE *fpr, FILE *fpw) {
	int buf[BUFSIZE];
	int nil = 0, nilline = 0;

	while (1) {
		fread(buf, BUFSIZE * 4, 1, fpr);
		if (feof(fpr))
			break;
		if ((nil = buf_nil(buf)) == BUFSIZE) {
			nilline++;
		} else {
			fseeko(fpw, nilline * BUFSIZE * sizeof(int) + nil * sizeof(int), SEEK_CUR);
			fwrite(buf + nil, sizeof(int), BUFSIZE - nil, fpw); /* if error */
			nilline = 0;
			nil = 0;
		}
	}
	fseeko(fpw, nilline * BUFSIZE * sizeof(int) + nil * sizeof(int), SEEK_CUR);
	fwrite(buf, 1, flen(fpr) % (BUFSIZE * sizeof(int)), fpw); /*last line*/

	return 0;
}

int main(int argc, char **argv)
{
	FILE *fpr, *fpw;

	if (argc != 3) {
		fprintf(stderr, "argment...\n");
		return 1;
	}

	fpr = fopen(argv[1], "rb");
	if (fpr == NULL) {
		perror(argv[1]);
		return 1;
	}

	fpw = fopen(argv[2], "wb");
	if (fpw == NULL) {
 		perror(argv[2]);
		fclose(fpr);
		return 1;
	}

	fast_cp(fpr, fpw);

	fclose(fpw);
	fclose(fpr);

	return 0;
}
