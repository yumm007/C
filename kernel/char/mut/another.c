#include <stdio.h>
#include <unistd.h>


int main(void) {
	char buf[1024];
	int fd;

	if ((fd = open("/dev/testchar", O_RDWR)) < 0) {
		perror("open:");
		return -1;
	}

	read(fd, buf, 30);
	printf("%s\n", buf);

	close(fd);
	return 0;
}
