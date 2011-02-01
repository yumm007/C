#include "myhead.h"

#define FG 250
#define BG 0x00
	
int startx, starty, endx, endy;

enum {UP = 65, DOWN, RIGHT, LEFT};
extern int brick_num[7][4][4][4];

void draw_box(int x, int y, int size) {
	int i, j;
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			//if ((i + x) >= startx)
			if ((i < size - 1) && (j < size - 1))
				fb_draw_point(i + x, j + y, FG);
			 else
				fb_draw_point(i + x, j + y, BG);
}

void clean_box(int x, int y, int size) {
	int i, j;
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			//if ((i + x) >= startx)
			if ((i < size - 1) && (j < size - 1))
				fb_draw_point(i + x, j + y, 244);
			 else
				fb_draw_point(i + x, j + y, 0x00);
}

int read_key(void) {
	int c1, c2, c3;
	while ((c1 = getchar()) != EOF )
		if ((c1 == 27) && (c2 = getchar()) == 91) {
			if((c3 = getchar()) >= UP && c3 <= LEFT) 
					return c3;
			 else 
				ungetc(c3, stdin);
		} else
			ungetc(c2, stdin);
	return -1;
}

void move_box(int way) {
	static int x = 0, y = 0, oldx, oldy, maxx, maxy;
	static struct brick b = {0,0,};
	int s;

	s = 4 - brick_num[b.b1][b.b2][0][0] -1;
	maxx = (xres() - 500) / D;
	maxy = (yres() - 150) / D;

	oldx = x, oldy =y;
	
	switch (way) {
		case LEFT:
			x--;
			if (x < 0)
				x = 0;
			break;
		case RIGHT:
			x++;
			if (x > (maxx - s))
				x = maxx -s ;
			break;
		case UP:
			switch (b.b2++) {
				default:
					if (b.b2 == 4)
						b.b2 = 0;
				case 0:
					x = x - 2;
					y = y + 1;
					break;
				case 1:
					x = x - 1;
					y = y + 2;
					break;
				case 2:
					x = x - 2;
					y = y + 1;
					break;
				case 3:
					x = x - 1;
					y = y + 2;
					break;
			}
			break;
		case DOWN:
			y++;
			if (y > maxy )
				y = maxy ;
			break;
		default:
			break;
	}
	
	b.x = oldx * D + 50;
	b.y = oldy * D +100;
	move_brick(&b, CLEAN);

	b.x = x * D + 50;
	b.y = y * D +100;
	move_brick(&b, DRAW);
}

void init_board(void) {
	int i, j;
   
	startx = 50, starty = 50;
 	endx=xres() - 50;
	endy = yres() - 50;

	system("clear");

	for (; startx <= endx - 400 ; startx += D) 
		for (starty = 100; starty <= endy; starty +=D) 
			for (i = 0; i <= D; i++)
				for (j = 0; j <= D; j++)
					if ((i < D - 1) && (j < D -1) ) {
						fb_draw_point(i+startx ,j+starty , 244);
					} else
						fb_draw_point(i+startx ,j+starty ,0x00);
			
}
