#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define DELLINE	9

static int split(FILE *fp, FILE *fptemp, size_t delline) {
	size_t linebufsize = 0, linenum = 0;
	char *linebuf = NULL;
	off_t offset = 0;
	int readc;

	while ((readc = getline(&linebuf, &linebufsize, fp)) > 0) {
		linenum++;
		if (linenum == delline) 
			offset = ftello(fp) - readc;
		else if (linenum > delline)
			fwrite(linebuf, 1, readc, fptemp);
	}

	fseeko(fp, offset, SEEK_SET);
	fseeko(fptemp, 0, SEEK_SET);

	return 0;
}

static int append(FILE *fp, FILE *fptemp) {
	int fd, readc;	
	char *linebuf = NULL;
	size_t linebufsize = 0;
	off_t offset = 0;

	while ((readc = getline(&linebuf, &linebufsize, fptemp)) > 0) 
		fwrite(linebuf, 1, readc, fp);
	offset = ftello(fp);

	fd = fileno(fp);
	return ftruncate(fd, offset);
}

int main(int argc, char *argv[])
{
	FILE *fp, *fptemp;
	
	if (argc != 2) {
		fprintf(stderr, "%s: filename\n", argv[0]);
		return -1;
	}
	
	if ((fp = fopen(argv[1], "r+b")) == NULL) {
		perror("open()");
		return -1;
	}

	if ((fptemp = tmpfile()) == NULL) {
		fclose(fp);
		perror("tmpfile()");
		return -1;
	}
	
	split(fp, fptemp, DELLINE);
	if (append(fp, fptemp) == -1) {
		perror("ftruncate()");		
		fclose(fptemp);
		fclose(fp);
		return -1;
	}

	fclose(fptemp);
	fclose(fp);
	
	return 0;
}
