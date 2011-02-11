#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

/* date > /tmp/asd */

int
main()
{
	pid_t pid;
	int fd;

	fd = open("/tmp/asd", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	if (fd<0) {
		perror("open()");
		exit(1);
	}
	fflush(NULL);
	pid = fork();
	if (pid<0) {
		perror("fork()");
		exit(1);
	}

	if (pid==0) {
		dup2(fd, 1);
		if (fd!=1) close(fd);
		execl("/bin/date", "date", NULL);
		perror("execl()");
		exit(1);
	}
	close(fd);
	wait(NULL);
	return 0;
}

