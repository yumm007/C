#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fb_draw.h"

#define 	WEIDTH 20
#define 	WAIT	200

static void rand_color(struct color_v *cur) {
	static unsigned long next = 0;
	
	if (next == 0)
		next = getpid();

	next = (next * 1103515245 + 12345) / 65536 % 32768;
	srand(next);
	cur->r = rand() % 255;

	next = (next * 1103515245 + 12345) / 65536 % 32768;
	srand(next);
	cur->g = rand() % 255;

	next = (next * 1103515245 + 12345) / 65536 % 32768;
	srand(next);
	cur->b = rand() % 255;

}

static void turn_left(int n, struct color_v *cur) {
	int i, j;

	for (i = WEIDTH; i > 0; i-- ) {
		for (j = 0; i+n*WEIDTH<= xres() && j <= yres(); j++) 
				fb_draw_point_v2(i + n * WEIDTH, j, cur);
		usleep(WAIT);
	}
}

static void turn_right(int n, struct color_v *cur) {
	int i, j;

	for (i = 0; i < WEIDTH; i++ ) {
		for (j = 0; i+n*WEIDTH <= xres() && j <= yres(); j++) 
				fb_draw_point_v2(i + n * WEIDTH, j, cur);
		usleep(WAIT);
	}
}

static void turn_down(int n, struct color_v *cur) {
	int i, j;

	for (j = 0; j <= yres(); j++) {
		for (i = 0;i+n*WEIDTH <= xres() && i < WEIDTH; i++) 
				fb_draw_point_v2(i + n * WEIDTH, j, cur);
		usleep(WAIT);
	}
}

static void turn_up(int n, struct color_v *cur) {
	int i, j;

	for (j = yres(); j > 0; j--) {
		for (i = 0;i+n*WEIDTH <= xres() && i < WEIDTH; i++) 
				fb_draw_point_v2(i + n * WEIDTH, j, cur);
		usleep(WAIT);
	}
}

void draw_screen(void) {
	int x, y, i, j, p, n;
	struct color_v cur;
	void (*fun[4])(int, struct color_v *);

	fun[0] = turn_left;
	fun[1] = turn_right;
	fun[2] = turn_down;
	fun[3] = turn_up;

	rand_color(&cur);
	x = xres();

	n = (x % WEIDTH == 0 ? x / WEIDTH: x / WEIDTH + 1);

	fun[rand()%4](rand() % n, &cur);
}


int main(void)
{
	int ret;

   ret = fb_open();
   if (ret != 0) {
	fprintf(stderr, "fb_open() error.\n");
		exit(1);
   }

	system("clear");
	while (1)
		draw_screen();

	fb_close();
	return 0;
}
