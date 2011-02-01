#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(void)
{
	int fd;	
	char buf[50];
	fd = open("/dev/test", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	read(fd, buf, 20);
	printf("<app>read from kernel:[%s]\n", buf);
	
	return 0;
}
