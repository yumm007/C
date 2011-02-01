#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>

char buf[50];
int fd;	
int flag;

void sig_handle(int sig)
{
	read(fd, buf, 12);		
	flag = 1;
}

int main(void)
{
	fd = open("/dev/test", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	signal(SIGIO, sig_handle);
	fcntl(fd, F_SETOWN, getpid());			
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_ASYNC);	

	for(; ;)
	{
		printf("continue...\n");
		sleep(1);
		if(flag)
			break;
	}

	printf("finish\n");
	printf("<app>read after signal action:[%s]\n", buf);
	return 0;
}
