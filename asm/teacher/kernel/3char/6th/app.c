#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include "test.h"

int main(int argc, char *argv[])
{
	int fd;	
	char buf[20];
	fd = open("/dev/test", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}
	
	write(fd, "hello", 6);
	read(fd, buf, 6);	
	printf("<app> [%s]\n", buf);
	return 0;
}
