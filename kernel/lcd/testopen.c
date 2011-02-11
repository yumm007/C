#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void) {
	int fd;

	if ((fd = open("/dev/my_lcd", O_RDWR)) < 0) {
		perror("open:");
		return -1;
	}

	close(fd);
	return 0;
}
