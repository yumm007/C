#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

off_t
flen(int fd)
{
	struct stat stat_res;

	if (fstat(fd, &stat_res)<0) {
		perror("fstat()");
		exit(1);
	}

	return stat_res.st_size;
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

