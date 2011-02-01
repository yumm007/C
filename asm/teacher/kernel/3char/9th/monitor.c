#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
int main(void)
{
	fd_set wset, rset;
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
		FD_ZERO(&wset);
		FD_SET(fd, &rset);	
		FD_SET(fd, &wset);	
	
		select(fd + 1, &rset, &wset, NULL, NULL);

		if(FD_ISSET(fd, &rset))
			printf("readable\n");

		if(FD_ISSET(fd, &wset))
			printf("writeable\n");
	
		sleep(2);		
	}
	
	return 0;
}
