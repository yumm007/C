#include "myhead.h"
#define	BG 		0x00
#define	BACKUP	1
#define	RESTORE	2

extern int startx, starty, endx, endy, maxm, maxn, scope;
extern int brick_num[7][4][8];
extern int board[MAXHEIGHT][MAXWIDTH];
extern struct color_v box_color[COLOR];

int reset(void) {
	struct termios new;
	int ret;

	tcgetattr(0, &new);

	printlet(650, 450, "GAME OVER\nAGAIN Y OR N");
	new.c_cc[VTIME] = 0;
	new.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &new);

	while ((ret = getchar()))
		if (ret == 'Y' || ret == 'y')
			break;
		else if (ret == 'N' || ret == 'n')
			return -1;

	new.c_cc[VTIME] = DEFAULT;
	new.c_cc[VMIN] = 0;
	tcsetattr(0, TCSANOW, &new);

	return 0;
}

void set_lev(int min) {
	struct termios cur;

	tcgetattr(0, &cur);
	cur.c_cc[VTIME] = min;
	tcsetattr(0, TCSANOW, &cur);
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

int read_key(int flag) {
	int c1, c2, c3, r;
	
	while ((r = read(0, &c1, 1)) != -1 ) {
		if (r == 1 && c1 == 0x20) 
			pause_fun();
		else if ( r == 1 && c1 == 27 && (c2 = getchar()) == 91) {
				if((c3 = getchar()) >= UP && c3 <= LEFT){
					save_key_to_file(c3);
					return c3;
				}
		}else if (r == 0) {
			save_key_to_file(DOWN);
			return DOWN;
		}
	}
	save_key_to_file(-1);
	return -1;
}

int wait_min(void) { 
	int i = 0, c = 0;
	if ((i = read(0, &c, 1)) == 1)
		write(0, &c, 1);
	return i;
}

void print_board(void) {
	int m, n;
	for (m = 0; m < maxm; m++){
		printf("%d  ",m );
		for (n = 0; n < maxn; n++)
			printf("%d ",board[m][n]);
		printf("\n");
	}
}

static void save_log(struct brickbox *b) {
	int m, n;
	FILE *fp;

	if ((fp = fopen("log.txt", "a" )) == NULL)
		return;

	fprintf(fp, "------------Struct of Box-------------\n");
		fprintf(fp, "b->bm=%d, b->bn=%d\n", b->bm, b->bn);
	fprintf(fp, "------------Content of Area-------------\n");
	for (m = 0; m <= maxm; m++){
		fprintf(fp, "%d  ",m );
		for (n = 0; n <= maxn; n++)
			fprintf(fp, "%d ",board[m][n]);
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
	
	fclose(fp);
}

static void __print_scope(struct color_v *bg, int flag) {
	int x, y;
	void (*fun)(int , int , struct color_v * );

	fun = (flag == BACKUP ? get_fb_draw_point: fb_draw_point_v2);

	for (y = 1 * D + 100; y <2 * D + 100; y++)
		for (x = maxn * D + 50; x < (maxn + 8) * D + 50; x++)
			fun(x, y, bg++);
}

void print_scope(int num) {
	static struct color_v bg[D*D*8];
	static int first = 0;

	if (first == 0) {
		__print_scope(bg, BACKUP);
		first = 1;
	}else 
		__print_scope(bg, RESTORE);

	printlet((maxn + 2) * D + 50, 1 * D + 100, "SCORE");
	printdig((maxn + 8) * D + 50, 1 * D + 100, scope);
}

int _max(int b1, int b2, int flag) {
	int i, max = 0;
	for (i = 0; i <= 6; i +=2)
		if (brick_num[b1][b2][i+flag] > max)
			max = brick_num[b1][b2][i+flag];
	return max;
}

int check_way(int way, int flag, struct brickbox box) {
	int m, n, oldm, oldn, oldb2;
	static struct brickbox b;
	int temp, ret;
	
	if (flag == 1)
		b = box;

	oldm = m = b.bm, oldn = n = b.bn, oldb2 = b.b2;
	
	switch (way) {
		case LEFT:
			if (m + _max(b.b1, b.b2,0) < 0)
				break;
			n--;
			if (n < 0)
				n = 0;
			break;
		case RIGHT:
			if (m + _max(b.b1, b.b2, 0) < 0)
				break;
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
		//save_log(&b);
		if (b.bm < 1) 
			return FINISH;
		if (way != DOWN)
			return 0;
		return set_board(&b);
	} 
	
	move_brick_v2(&b, CLEAN);

	b.b2 = temp;
	b.bm = m, b.bn = n;
	move_brick_v2(&b, DRAW);
	//save_key_to_file(way);

	if (m == (maxm - _max(b.b1,b.b2,0)) && wait_min() == 0) 
		return set_board(&b);
	return 0;
}

