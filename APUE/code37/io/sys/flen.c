#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

off_t
flen(int fd)
{
	off_t save, pos;

	save = lseek(fd, 0, SEEK_CUR);
	pos = lseek(fd, 0, SEEK_END);
	lseek(fd, save, SEEK_SET);

	return pos;
}

int
main(int argc, char **argv)
{
	int fd;

	if (argc<2) {
		fprintf(stderr, "Usage...\n");
		exit(1);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd<0) {
		perror("open()");
		exit(1);
	}

	printf("%lld\n", flen(fd));

	close(fd);

	return 0;
}

