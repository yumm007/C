#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
int main(void)
{
	int ret;
	fd_set rset;
	int fd;	
	fd = open("/dev/test", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	for(; ;)
	{
		FD_ZERO(&rset);
		FD_SET(fd, &rset);	
	
		printf("<app> before select \n");
		ret = select(fd + 1, &rset, NULL, NULL, NULL);
		printf("<app> [%d]\n", ret);

		if(FD_ISSET(fd, &rset))
			printf("readable\n");

		sleep(2);		
	}
	
	return 0;
}
