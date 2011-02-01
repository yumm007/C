#ifndef HS_FB_DRAW_H
#define HS_FB_DRAW_H

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <linux/fb.h>

struct color_v {
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char t;
};

int fb_open(void);
void fb_close(void);
void fb_draw_point(int x, int y, int color);
void fb_draw_point_v2(int x, int y, struct color_v *color);
void fb_draw_point_v3(int x, int y, struct color_v *color);
void get_fb_draw_point(int x, int y, struct color_v *color);
int xres(void);
int yres(void);

#endif
