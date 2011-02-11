#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <strings.h>

#include "lcd.h"
#define	RGB(r, g, b)	((r << 11) | ((g >> 2) << 5) | (b >> 3))

int fd;
void fill_screen(unsigned short color)
{
	unsigned short p[272][480];
	int i, j;
	/* fill memory(framebuffer) */
	for(i = 0; i < 272; i++)
	{
		for(j = 0; j < 480; j++)
		{
			p[i][j] = color;	
		}
	}
	write(fd, p, 480*272*2);	
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

	if(!strncasecmp("red", argv[1], 3)){
		fill_screen(RGB(255, 0, 0));		
	}
	if(!strncasecmp("green", argv[1], 5)){
		fill_screen(RGB(0, 255, 0));		
	}
	if(!strncasecmp("blue", argv[1], 4)){
		fill_screen(RGB(0, 0, 255));		
	}

	return 0;
}
