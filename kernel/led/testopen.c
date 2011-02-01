#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
char tmp[] = "10101010101010101101101010101010010101011";

int main(void) {
	int fd;

	if ((fd = open("/dev/testled", O_RDWR)) < 0) {
		perror("open:");
		return -1;
	}

	printf("%c\n",read(fd, tmp, 1));

	write(fd, tmp, strlen(tmp));

	close(fd);
	return 0;
}
