/*******************************
改menu.lst
  title XXXXOS (2.6.XX)
  root (hd0,X)
  kernel /boot/vmlinuz-2.6.XXX ro root=XXX quiet vga=0x318

在kernel那行最后添加vga=0x318或vga=792 （前面十六进制，后面十进制，参考下表）

vga=可设置的值可以用工具fbset（没有的话用sudo apt-get install fbset装）,/etc/fb.modes,或sudo hwinfo --framebuffer参考，主要还是以hwinfo为主（sudo apt-get install hwinfo）

下表列出一些常用值：

# FRAMEBUFFER RESOLUTION SETTINGS
# +----------------------------------------------------------------+
# |    640x480    800x600   1024x768 1280x1024 1280x800   1600x1200
# ----+-------------------------------------------------------------
# R6 | 0x301=769 0x303=771 0x305=773 0x307=775            0x31C=796
# 32K | 0x310=784 0x313=787 0x316=790 0x319=793 0x360＝864 0x31D=797
# 64K | 0x311=785 0x314=788 0x317=791 0x31A=794 0x361＝865 0x31E=798
# 16M | 0x312=786 0x315=789 0x318=792 0x31B=795 0x362＝866 0x31F=799
# +----------------------------------------------------------------+
*******************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "fb_draw.h"

#define R 25

//xres(), yres()
/*
void slowly(void) {
	int i, j;
	for (i = 0; i< 1000; i++)
		for (j = 0; j < 5000; j++)
			;
}
*/
void clean(int x, int y) {
	int x1, y1;

     for (x1 = x; x1 < y; x1++)
	  for (y1 = x; y1 < y; y1++) {
		if ( ((x1-(x+R))*(x1-(x+R))+(y1-(y-R))*(y1-(y-R))) <= R*R)
	       fb_draw_point(x1, y1, 0x00);
	  }
}
void clear(int x, int y) {
	 int x1, y1;

     for (x1 = x; x1 <= y; x1++)
	  for (y1 = x; y1 < y; y1++) {
	       fb_draw_point(x1, y1, 0x00);
	  }

}

#if 0
void dram(int x, int y) {
	int x1, y1;

     for (x1 = x; x1 < y; x1++)
	  for (y1 = x; y1 < y; y1++) {
	       fb_draw_point(x1, y1, 0xaf);
	  }
}
#else
void dram(int x, int y) {
	int x1, y1;

     for (x1 = x; x1 < y; x1++)
	  for (y1 = x; y1 < y; y1++) {
		if ( ((x1-(x+R))*(x1-(x+R))+(y1-(y-R))*(y1-(y-R))) <= R*R)
	       fb_draw_point(x1, y1, 0xaf);
	  }
}

#endif
void move(void) {
	int x, y, r, l, limitx, limity;
	
	srand(getpid());
	//r = rand() % 8;
	r = 2;
	y = x = rand() % 500;
	limitx = xres() - 2*R;
	limity = yres() - 2*R;

	
	while (l = 4) {
		switch (l){
			case 1:			/* x++, y++ */	
				for (; x <= limitx  && y <= limity; x++, y += r) {
					dram(x,y+R*2);
					usleep(10000);
					clean(x, y+R*2);
				}
				break;
			case 2:		/* x--, y-- */
				for (; x >= 0 && y >= 0; x--, y -= r) {
					dram(x,y+R*2);
					usleep(10000);
					clean(x, y+R*2);
				}
				break;
			case 3:		/* x--, y++ */
				for (; x >= 0 && y <= limity; x--, y += r) {
					dram(x,y+R*2);
					usleep(10000);
					clean(x, y+R*2);
				}
				break;
			case 4:		/* x++, y-- */
				for (; x <= limitx && y >= 0; x++, y -= r) {
					dram(x,y+R*2);
					usleep(10000);
					clean(x, y+R*2);
				}
				break;
			default:
				break;
		}	
	}
}

int main(void)
{
     int ret;

     ret = fb_open();
     if (ret != 0) {
	  fprintf(stderr, "fb_open() error.\n");
	  exit(1);
     }
	 
	 //srand(getpid());
	 //r = rand() % 10;
	 //printf("x=%d, y=%d, rand=%d\n",xres(),yres(),r);		

	 clear(0,yres());
	 move();

     fb_close();
     return 0;
}
