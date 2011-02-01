#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>



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
	
	read(fd, buf, 12);		
	printf("<app>read after wake up:[%s]\n", buf);
	
	return 0;
}
