#include "myhead.h"
#include <unistd.h>

#define BOX 	0xaf
#define BG 		0x00
#define FG		244

#define RESTORE 0
#define BACKUP	 1
	
int startx, starty, endx, endy, maxm, maxn, scope;

extern int brick_num[7][4][8];
extern int board[MAXHEIGHT][MAXWIDTH];
extern struct color_v box_color[COLOR];
extern void print_board();

void draw_box_v2(int x, int y, int size, struct color_v *color) {
	int i, j;
	struct color_v deeptmp, deepline, lighttmp;
	
	deeptmp.b = color->b - 7;
	deeptmp.g = color->g - 25;
	deeptmp.r = color->r - 7;

	deepline.b = color->b - 10;
	deepline.g = color->g - 30;
	deepline.r = color->r - 10;
	
	lighttmp.b = color->b + 7;
	lighttmp.g = (color->g + 15) > 255 ? 255 :(color->g + 15) ;
	lighttmp.r = color->r + 7;

	for (i = 0; i < size; i++)
		for (j = 0; y >= 100 && j < size; j++)
			if (i > 0 && j > 0 && (i < size - 3) && (j < size - 3)) {
				if ((i > 2 && i < size - 5 && (j == 2 || j == size - 5)) || \
					 (j > 2 && j < size - 5 && (i == 2 || i == size - 5)) )	
					fb_draw_point_v2(i+x, j+y, &deepline);// draw 4 deep line
				else if ((i == 2 || i == D - 5) && ( j == 2 || j == D - 5))
					fb_draw_point(i + x, j + y, BG); // draw 4 point
				else if (((i==D/4*1 || i == D/4*3-1) && j> D/4*1 && j < D/4*3-1) \
						||((j ==D/4*1 || j == D/4*3-1) && i > D/4*1 && i < D/4*3-1))
						fb_draw_point_v2(i+x ,j+y,&lighttmp); // draw light line
				else if (i > D/4*1 && i < D/4*3-1 && j > D/4*1 && j < D/4*3-1)
						fb_draw_point_v2(i+x ,j+y, &deeptmp); // draw deep box
				else
						fb_draw_point_v2(i+x ,j+y, color); // draw box
			} else if (i == size -1 || j == size -1) 
				fb_draw_point(i + x, j + y, BG);	// draw BG line
			else if (i == 0 || i == size-2 || j == 0 || j == size - 2)
				fb_draw_point_v2(i+x ,j+y, &deeptmp); // draw shadow
}

static void __init_nextbox(unsigned long *next, struct brickbox *oldbox) {
		*next = getpid();
		oldbox->b1 = *next % 7;
		oldbox->b2 = *next % 4;
		oldbox->c = *next % (COLOR - 1) + 1;
}

static void backup_box(int x, int y, int size, struct color_v *backup_bg, int flag){
	int i, j;
	void (*fun)(int , int , struct color_v * );

	fun = (flag == BACKUP ? get_fb_draw_point: fb_draw_point_v2);

	for (i = 0; i < size; i++)
		for (j = 0; y >= 100 && j < size; j++)
			fun(x+i, y+j, backup_bg + size * i + j);
}

static void __clear_noti_v2(struct brickbox *b, struct color_v *backup_bg, int flag) {
	int i, j, x, y;

	for (i = 0, j = 0, x = b->bn * D + 50, y = b->bm * D + 100; i < 7; i += 2, j++)
		backup_box(x + brick_num[b->b1][b->b2][i+1] * D, \
						y + brick_num[b->b1][b->b2][i] * D, \
						D, backup_bg + j * D * D, flag);
}

static void clear_noti_v2(struct brickbox *b) {
	static struct color_v backup_bg[4][D][D];
	static struct brickbox oldbox;
	static int first = 0;

	if (first == 0) {
		oldbox = *b;
		__clear_noti_v2(&oldbox, &backup_bg[0][0][0], BACKUP);
		first = 1;
	} else {
		__clear_noti_v2(&oldbox, &backup_bg[0][0][0], RESTORE);
		oldbox = *b;
		__clear_noti_v2(&oldbox, &backup_bg[0][0][0], BACKUP);
	}
}

static void __set_draw_nextbox(struct brickbox *nextbox, int b1, int c) {
	int i, x, y;

	nextbox->b1 = b1;
	nextbox->b2 = rand() % 4;
	nextbox->c = c;
	nextbox->bm = 2;
	nextbox->bn = maxn + 6;

	clear_noti_v2(nextbox);
	for (i = 0, x = nextbox->bn * D + 50, y = nextbox->bm * D + 100; i < 7; i +=2) {
		draw_box_v2( x + brick_num[nextbox->b1][nextbox->b2][i+1] *\
			D, y + brick_num[nextbox->b1][nextbox->b2][i] * D, D, box_color+(nextbox->c));
	}
}

void move_brick_v2(struct brickbox *b, int flag) {
	int i, x, y;

	flag = (flag == CLEAN ? 0: b->c);
	for (i = 0, x = b->bn * D + 50, y = b->bm * D + 100; i < 7; i +=2) 
			draw_box_v2( x + brick_num[b->b1][b->b2][i+1] *\
			 D, y + brick_num[b->b1][b->b2][i] * D, D, box_color+flag);
}

void rand_box(struct brickbox *b) {
	static unsigned long next = 0;
	static struct brickbox nextbox;
	int b1, c;
	
	if (next == 0)
		__init_nextbox(&next, &nextbox);

// Current box value
	*b= nextbox;
	b->bm = 0 - _max(b->b1, b->b2, 0) -1;
	b->bn = maxn / 2;

// Rand seed
	do {
		next = next * 283 / 61 + 453;
		srand((unsigned)((next/65536) % 32768));
		b1 = rand() % 7;
		c = rand() % (COLOR -1) + 1; 
	} while (b1 == nextbox.b1 || c == nextbox.c);

	__set_draw_nextbox(&nextbox, b1, c);
	save_box_to_file(b);
}


void draw_board(void) {
	int  x, y;
	for (x = startx; x <= startx + maxn *D ; x += D) 
		for (y = 100; y <= 100 + maxm *D; y +=D) 
			draw_box_v2(x, y, D, &box_color[0]);
}

void init_board(void) {
	int i, j;
	startx = 50, starty = 50, endx=xres() - 50, endy = yres() - 50, scope = 0;
	maxn = (xres() - 500) / D, maxm = (yres() - 150) / D;
	if (maxn >= MAXWIDTH)
		maxn = MAXWIDTH - 1;
	if (maxm >= MAXHEIGHT)
		maxm = MAXHEIGHT - 1;
	
	//init board to zero
	for (i = 0; i<= maxn; i++)
		for (j = 0; j<= maxm; j++)
			board[j][i] = 0;

	printf("\033[?25l"); //隐藏光标
	system("clear");
	background_pic("background.bmp");
	draw_board();
	printlet((maxn + 2) * D + 50, 2 * D + 100, "NEXT");
	print_scope(0);
}
