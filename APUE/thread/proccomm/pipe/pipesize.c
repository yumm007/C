#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define	BUFSIZE 16<<10<<10

int main(void)
{
	
	int i, fd[2];

	if (pipe(fd) < 0) {
		perror("pipe");
		exit(1);
	}

	for (i = 0; i < BUFSIZE; i++) {
		write(fd[1], "A", 1);
		printf("write %d bytes.\n", i);
	}

	return 0;
}

