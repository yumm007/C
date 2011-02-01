#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define	FILEPATH	"/dev/testchar"
#define	BUFSIZE	256



int main(void) {
	int fd;
	char buf[BUFSIZE];

	if ((fd = open(FILEPATH, O_RDWR)) < 0) {
		perror("open:");
		return -1;
	}

	read(fd, buf, BUFSIZE);
	printf("%s", buf);

	return 0;
}
