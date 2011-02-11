#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

int
ftype(const char*fname)
{
	int ch;
	struct stat stat_res;

	if (lstat(fname, &stat_res)<0) {
		perror("fstat()");
		exit(1);
	}

	switch (stat_res.st_mode&S_IFMT) {
		case S_IFSOCK:
			ch='s';
			break;
		case S_IFLNK:
			ch='l';
			break;
		case S_IFREG:
			ch='-';
			break;
		case S_IFBLK:
			ch='b';
			break;
		case S_IFDIR:
			ch='d';
			break;
		case S_IFCHR:
			ch='c';
			break;
		case S_IFIFO:
			ch='p';
			break;
		default:
			ch='?';
			break;
	}

	return ch;
}

int
main(int argc, char **argv)
{
	if (argc<2) {
		fprintf(stderr, "Usage...\n");
		exit(1);
	}

	printf("%c\n", ftype(argv[1]));

	return 0;
}

