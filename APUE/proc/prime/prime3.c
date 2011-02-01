#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define 	LEFT	50000000
#define 	RIGHT	50050202
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
	int i, fd;
	pid_t pid;
	FILE *fp;
	
	if ((fp = tmpfile()) == NULL) {
		perror("tmpfile()");
		return -1;	
	}
	fd = fileno(fp);

	for (i = LEFT; i <= RIGHT; i++) {
		if (write(fd, &i, sizeof(i)) < sizeof(i))
			return -1;
	}
	lseek(fd, 0, SEEK_SET);

	for (i = 0; i < N; i++) {
		if ((pid = fork()) < 0)
			return -1;
		else if (pid == 0){
			is_prime(fd);
			return -1;
		}
	}

	for (i = 0; i < N; i++)
		wait(NULL);

	fclose(fp);
	return 0;
}
