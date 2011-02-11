#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int
main()
{
	int fd;

	fd = open("/etc/hosts", O_RDONLY);
	if (fd<0) {
		perror("open()");
		exit(1);
	}

	printf("%d\n", fd);
	while(1) pause();

	close(fd);

	return 0;
}

