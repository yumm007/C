#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define FNAME "/tmp/big"

int
main()
{
	int fd;
	off_t offset, len;

	fd = open(FNAME, O_WRONLY|O_CREAT|O_TRUNC, 0600);
	if (fd<0) {
		perror("open()");
		exit(1);
	}

	offset = 5ll*1024LL*1024LL*1024LL;

	ftruncate(fd, offset);
	
	close(fd);

	return 0;
}

