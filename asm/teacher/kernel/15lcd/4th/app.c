#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <strings.h>
#include "lcd.h"

#define	SIZE	(272 * 480 * 2)
int fd;
int pic_fd;
void fill_screen(void)
{
	short buf[272][480];
	pic_fd = open("./test.bin", O_RDWR);	
	read(pic_fd, buf, SIZE);
	write(fd, buf, SIZE);
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
	fill_screen();

	return 0;
}
