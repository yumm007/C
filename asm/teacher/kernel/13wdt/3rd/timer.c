#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "wdt.h"
int main(int argc, char *argv[])
{
	int fd;
	fd = open("/dev/wdt", O_RDWR);
	if(fd < 0){
		perror("open");
		return -1;
	}
	if(!strncasecmp("start", argv[1], 5))
		ioctl(fd, WDT_START );
	if(!strncasecmp("stop", argv[1], 4))
		ioctl(fd, WDT_STOP );


	return 0;
}
