/*******************************
修改menu.lst
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
# 256 | 0x301=769 0x303=771 0x305=773 0x307=775            0x31C=796
# 32K | 0x310=784 0x313=787 0x316=790 0x319=793 0x360＝864 0x31D=797
# 64K | 0x311=785 0x314=788 0x317=791 0x31A=794 0x361＝865 0x31E=798
# 16M | 0x312=786 0x315=789 0x318=792 0x31B=795 0x362＝866 0x31F=799
# +----------------------------------------------------------------+
*******************************/

/*
**	桌面弹球
**	2010-09-07 yumm_007@163.com
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "fb_draw.h"

#define R 25	/* 球的半径，单位为像素 */
/* 参为分别为： 当前圆心的半径坐标／圆心所能到的最大坐标和最小坐标 */
static int curx, cury, maxx, maxy, minx, miny;

/* 清除点(x,y) */
void clear(int x, int y) {
	 int x1, y1;

     for (x1 = x; x1 <= y; x1++)
	  for (y1 = x; y1 < y; y1++) {
			fb_draw_point(x1, y1, 0x00);
	  }
}

/* 在坐标(x,y)处以半径R画圆 */
void draw(int x, int y, int flag) {
	int i, j, len;

	for (i = x - R; i <= x +R; i++)
		for (j = y - R; j <= y + R; j++) {
			len = sqrt((x - i)*(x - i) + (y -j)*(y -j));
			if (len <= R)
				if (flag)
					fb_draw_point(i, j, 0xfe - len/7);
				else
					fb_draw_point(i, j, 0x00);
		}
}

/* 先画一个圆,停留10000毫秒,再清除它 */
void __move(int x, int y){
	draw(x, y, 1);
	usleep(10000);
	draw(x, y, 0);
}

/* 延着一个方向移动圆心.到了边界返回 */
void move(int way){
	int x, y, step;
	srand(curx + cury);
	step = rand() % 4 + 1;	

	switch (way) {
		case 1:
			for (x = curx, y = cury; x < maxx && y < maxy; x++, y+=step)
				__move(x, y);
			break;
		case 2:	
			for (x = curx, y = cury; x < maxx && y > miny; x++, y-=step) 
				__move(x,y);
			break;
		case 3:	
			for (x = curx, y = cury; x > minx && y > miny; x--, y-=step) 
				__move(x,y);
			break;
		case 4:	
			for (x = curx, y = cury; x > minx && y < maxy; x--, y+=step) 
				__move(x,y);
			break;
		default:
			break;
	}
	curx = x; 
	cury = y;

	if (curx > maxx)
		curx = maxx;
	else if (curx < minx)
		curx = minx;

	if (cury > maxy)
		cury = maxy;
	else if (cury < miny)
		cury = miny;
}

/* 生成随机的方向值,并调用上面的函数 */
void makeway(void) {
	int i = getpid();
	srand(i);
	cury = curx = rand() % yres();

	while (i += 5) {
		srand(i);
		move(rand() % 4 + 1);
		if (i > 1000)
			i = 20;
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
	  maxx = xres() - R, maxy = yres() - R, curx = minx = R, cury = miny = R;

     clear(0,yres());
	  makeway();

     fb_close();
     return 0;
}
