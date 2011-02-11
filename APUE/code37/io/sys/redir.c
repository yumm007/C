#include <stdio.h>
#include <fcntl.h>

int
main()
{
	int fd;

	fd = open("/tmp/aaa", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	if (fd<0) {
		perror("open()");
		exit(1);
	}

	dup2(fd, 1);
	if (fd!=1) close(fd);

/****************************/
	puts("Hello!");
	return 0;
}

