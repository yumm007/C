#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>


int main(int argc, char *argv[])
{
	int fd;	
	char buf[50];
	fd = open("/dev/test", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	if(!strncasecmp("on", argv[1], 2))		
		ioctl(fd, 0);

	if(!strncasecmp("off", argv[1], 3))		
		ioctl(fd, 1);
	
	return 0;
}
