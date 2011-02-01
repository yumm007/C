#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/select.h>

#define	BUFSIZE 6<<10<<10

int fd[2];

static void *read_data(void *arg) {
	int fd = (int)arg;
	char buf[BUFSIZE];
	int ret;

	while ((ret = read(fd, buf, BUFSIZE)) != 0) {
		if (ret == -1) {
			if (errno == EAGAIN)
				continue;
			else
				break;
		}
		write(2, buf, ret);
	}

	pthread_exit(NULL);
}

static void *write_data(void *arg) {
	int n = 0, ret = 0;
	char buf[BUFSIZE];
	struct timeval	tim;

	memset(buf, ((char *)arg)[0], BUFSIZE);

	do{
		tim.tv_sec = 2;
		tim.tv_usec = 0;
		select(0, NULL, NULL, NULL, &tim);
	
		n = write(fd[1], buf + ret, (BUFSIZE) - ret);
		printf("write %d bytes one time.\n", n);
		if (n == -1) {
			if (errno == EAGAIN)
				continue;
			else
				break;
		}
		ret += n;
	} while (ret != BUFSIZE);

	pthread_exit(NULL);
}

int main(void)
{
	
	int i;
	pthread_t pthd[3];
	long	flag;

	if (pipe(fd) < 0) {
		perror("pipe");
		exit(1);
	}

	if ((flag = fcntl(fd[0], F_GETFL)) < 0) {
		perror("fcntl F_GETFL");
		pthread_exit(NULL);
	}

	if (fcntl(fd[0], F_SETFL, flag | O_NONBLOCK) < 0) {
		perror("fcntl F_GETFL");
		pthread_exit(NULL);
	}

	if ((flag = fcntl(fd[1], F_GETFL)) < 0) {
		perror("fcntl F_GETFL");
		pthread_exit(NULL);
	}

	if (fcntl(fd[1], F_SETFL, flag | O_NONBLOCK) < 0) {
		perror("fcntl F_GETFL");
		pthread_exit(NULL);
	}

	pthread_create(pthd+0, NULL, read_data, (void *)(fd[0]));
	pthread_create(pthd+1, NULL, write_data, (void *)"A");
	pthread_create(pthd+2, NULL, write_data, (void *)"B");

	for (i = 1; i < 3; i++)
		pthread_join(pthd[i], NULL);
	close(fd[1]);

	pthread_join(pthd[0], NULL);
	close(fd[0]);

	return 0;
}

