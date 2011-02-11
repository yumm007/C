#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <strings.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>


#include "lcd.h"

#define	SIZE	(272 * 480 * 2)
int fd;
int pic_fd;
char *pic[6] = {"0.bin", "1.bin", "2.bin", "3.bin", "4.bin", "5.bin"};
void change_pic_handle(int sig)
{
	static int i = 0;
	short buf[272][480];
	i = i % 6;
	pic_fd = open(pic[i], O_RDWR);
	read(pic_fd, buf, SIZE);
	write(fd, buf, SIZE);
	i++;
	printf("change ...\n");
}

int main(int argc, char *argv[])
{
	fd = open("/dev/lcd", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	ioctl(fd, CONFIG_LCD);
	ioctl(fd, ENABLE_LCD);

	signal(SIGIO, change_pic_handle);
	fcntl(fd, F_SETOWN, getpid());
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_ASYNC);
	
	for(; ;)
	{
		
	}
	return 0;
}
