#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include "test.h"

int main(int argc, char *argv[])
{
	int fd;	
	struct my_t data;
	data.header = 123;	
	memcpy(data.buf, "movie", 15);

	fd = open("/dev/test", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	if(!strncasecmp("on", argv[1], 2))		
		ioctl(fd, LED_ON, &data);

	if(!strncasecmp("off", argv[1], 3))		
		ioctl(fd, LED_OFF);
	
	return 0;
}
