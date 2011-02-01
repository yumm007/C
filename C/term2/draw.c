#include "myhead.h"
#include <unistd.h>

#define BOX 	0xaf
#define BG 		0x00
#define FG		244
	
int startx, starty, endx, endy, maxm, maxn, scope;

extern int brick_num[7][4][8];
extern int board[MAXBOARD][MAXBOARD];
extern void print_board();

void draw_box(int x, int y, int size) {
	int i, j;
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			if ((i < size - 1) && (j < size - 1))
				fb_draw_point(i + x, j + y, BOX);
			 else
				fb_draw_point(i + x, j + y, BG);
}

void clean_box(int x, int y, int size) {
	int i, j;
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			if ((i < size - 1) && (j < size - 1))
				fb_draw_point(i + x, j + y, FG);
			 else
				fb_draw_point(i + x, j + y, BG);
}

void pause_fun(void) {
	struct termios old, new;
	int c;

	tcgetattr(0, &old);
	tcgetattr(0, &new);

	new.c_lflag &= ~(ICANON | ECHO);
	new.c_cc[VTIME] = 0;
	new.c_cc[VMIN] = 1;

	while ((c = getchar()) != ' ')
		;
	
	tcsetattr(0, TCSANOW, &old);
}

int read_key(void) {
	int c1, c2, c3, r;
	
	while ( (r = read(0, &c1, 1)) != -1 ) {
		if (r == 1 && c1 == 0x20) 
			pause_fun();
		else if ( r == 1 && c1 == 27 && (c2 = getchar()) == 91) {
				if((c3 = getchar()) >= UP && c3 <= LEFT)
					return c3;
		}else if (r == 0)
			return DOWN;
	}
	return -1;
}

int _max(int b1, int b2, int flag) {
	int i, max = 0;
	for (i = 0; i <= 6; i +=2)
		if (brick_num[b1][b2][i+flag] > max)
			max = brick_num[b1][b2][i+flag];
	return max;
}

void rand_box(struct brickbox *b) {
	static unsigned next = 0;
	static struct brickbox oldbox;
	int b1, x, y;
	
	if (next == 0) {
		next = getpid();
		oldbox.b1 = next % 7;
		oldbox.b2 = next % 4;
	}

	for (x = 0; x<= 4 * D; x++ )
		for (y = 0; y <= 4 * D +100; y++)
			fb_draw_point(x + (maxn +4) * D + 50, y + 2 * D + 100, BG);

	b->b1 = oldbox.b1;
	b->b2 = oldbox.b2;
	b->bm = 0;
	b->bn = maxn / 2;
	
	do {
		next = next * 283 / 65 + 453;
		srand(next);
		if (next > 200000)
			next = rand() % 28;
		b1 = rand() % 7;
	} while (b1 == oldbox.b1);

	oldbox.b1 = b1;
	oldbox.b2 = rand() % 4;

	oldbox.bm = 2;
	oldbox.bn = maxn + 4;
	
	move_brick(&oldbox, DRAW);

}

int check_way(int way, int flag) {
	int m, n, oldm, oldn, oldb2;
	static struct brickbox b;
	int temp, ret;
	
	if (flag == 1)
		rand_box(&b);
	
	oldm = m = b.bm, oldn = n = b.bn, oldb2 = b.b2;
	
	switch (way) {
		case LEFT:
			n--;
			if (n < 0)
				n = 0;
			break;
		case RIGHT:
			n++;
			if (n > (maxn - _max(b.b1,b.b2,1))) 
				n--;
			break;
		case UP:
			b.b2++;
			if (b.b2 == 4)
				b.b2 = 0;
			if (n > (maxn - _max(b.b1,b.b2,1)))
				n--;
			if (m > (maxm - _max(b.b1,b.b2,0)))
				m--;
			b.bm = m, b.bn = n;
			if (check_up_ava(&b, oldb2) == -1) {
				b.bm = oldm;
				b.bn = oldn;
				b.b2 = oldb2;
				return 0;
			}
			break;
		case DOWN:
			m++;
			if (m > (maxm - _max(b.b1,b.b2,0))) 
				m--;
			break;
		default:
			break;
	}

	temp = b.b2;
	b.bm = oldm, b.bn = oldn, b.b2 = oldb2;

	if ((ret = check_next_step(&b, way)) == -1) {
		if (b.bm < 3)
			return FINISH;
		if (way != DOWN)
			return 0;
		return set_board(&b);
	} 

	move_brick(&b, CLEAN);

	b.b2 = temp;
	b.bm = m, b.bn = n;
	move_brick(&b, DRAW);

	if (m == (maxm - _max(b.b1,b.b2,0)) && wait_min() == 0) 
		return set_board(&b);
	return 0;
}

void draw_board(void) {
	int i, j;

	for (; startx <= endx - 400 ; startx += D) 
		for (starty = 100; starty <= endy; starty +=D) 
			for (i = 0; i <= D; i++)
				for (j = 0; j <= D; j++)
					if ((i < D - 1) && (j < D -1) ) {
						fb_draw_point(i+startx ,j+starty , 244);
					} else
						fb_draw_point(i+startx ,j+starty ,BG);
}

void init_board(void) {
	int i, j;
	startx = 50, starty = 50, endx=xres() - 50, endy = yres() - 50;
	maxn = (xres() - 500) / D, maxm = (yres() - 150) / D;
	if (maxn > MAXBOARD)
		maxn = MAXBOARD;
	if (maxm > MAXBOARD)
		maxm = MAXBOARD;
	
	system("clear");
	
	scope = 0;

	//init board to zero
	for (i = 0; i< maxn; i++)
		for (j = 0; j< maxm; j++)
			board[j][i] = 0;
	//draw board
	draw_board();
	print_scope(0);
}
