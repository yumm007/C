#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void)
{
	int fd;
	int n = 123;
	int *p = &n;

	fd = open("db.db", O_RDWR|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	if (fd == -1) {
		printf("Open file failed.\n");
		return -1;
	}
	
	if (write(fd, p, 4) < 4) {
		printf("Write file failed.\n");
		return -1;
	}
}
