#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>

static int time_to_exit = 0;

static void sig_han(int inused) {
	time_to_exit = 1;
	printf("received sig.\n");
}

int main(void) {
	char buf[1024];
	char *tmp = "send message from user space";
	int fd;

	signal(SIGIO, sig_han);

	if ((fd = open("/dev/testchar", O_RDWR)) < 0) {
		perror("open:");
		return -1;
	}

	fcntl(fd, F_SETOWN, getpid());
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_ASYNC);

	read(fd, buf, 30);
	printf("%s\n", buf);

	write(fd, tmp, 30);
	ioctl(fd, 0, 99);	
	ioctl(fd, 1, 99);

	while(!time_to_exit)
		;

	close(fd);
	return 0;
}
