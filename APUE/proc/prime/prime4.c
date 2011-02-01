#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define 	LEFT	50000000
#define 	RIGHT	50010202
#define 	N		6

static void is_prime(int fd) {
	int i = 0, j, flag;

	while(read(fd, &i, sizeof(i)) == sizeof(i)){
		flag = 1;
		for (j = 2; flag && j < i / 2;j++)
			flag =  i % j;
		if (flag)	
			printf("%d: %d is prime\n", getpid(), i);
	}

	return;
}

int main(void) {
	int i, fd[2];
	pid_t pid;
	
	if (pipe(fd) == -1) {
		fprintf(stderr, "pipe()\n");
		return -1;
	}


	for (i = LEFT; i <= RIGHT; i++) {
		if (write(fd[1], &i, sizeof(i)) < sizeof(i))
			return -1;
	}
	close(fd[1]);

	for (i = 0; i < N; i++) {
		if ((pid = fork()) < 0)
			return -1;
		else if (pid == 0){
			close(fd[1]);
			is_prime(fd[0]);
			close(fd[0]);
			return -1;
		}
	}
	close(fd[0]);

	for (i = 0; i < N; i++)
		wait(NULL);

	return 0;
}
