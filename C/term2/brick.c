#include "myhead.h"
#define BG 		0x00

extern int brick_num[7][4][8];
extern int board[MAXBOARD][MAXBOARD];
extern int maxm, maxn, scope;

void move_brick(struct brickbox *b, int flag) {
	int i, x, y;

	x = b->bn * D + 50;	
	y = b->bm * D + 100;

	if (flag == CLEAN) 
		for (i = 0; i < 7; i += 2)
			clean_box( x + brick_num[b->b1][b->b2][i+1] \
			* D, y + brick_num[b->b1][b->b2][i] * D, D);
	else
		for (i = 0; i < 7; i +=2) 
			draw_box( x + brick_num[b->b1][b->b2][i+1] *\
			 D, y + brick_num[b->b1][b->b2][i] * D, D);
	
}

int wait_min() { 
	int i = 0, c = 0;
	if ((i = read(0, &c, 1)) == 1)
		write(0, &c, 1);
	return i;
}

void print_board(void) {
	int m, n;
	for (m = 0; m <= maxm; m++){
		printf("%d  ",m );
		for (n = 0; n <= maxn; n++)
			printf("%d ",board[m][n]);
		printf("\n");
	}
}

void cleanline(int line) {
	int m, n, x, y;
	
	for (m = line; m >0; m--)
		for (n = 0; n < maxn; n++){
			board[m][n] = board[m-1][n];
			x = n * D + 50;	
			y = m * D + 100;
			if (board[m][n] == 1)
				draw_box(x, y, D);
			else
				clean_box(x, y, D);
		}
}

void print_scope(int num) {
	int x, y;

	for (x = 0; x<= 5 * D; x++ )
		for (y = 0; y <= 1 * D; y++)
			fb_draw_point(x + (maxn + 1) * D + 50, y + 1 * D + 100, BG);

	printdig((maxn + 5) * D + 50, 1 * D + 100, scope);
}

int set_board(struct brickbox *b) {
	int i, n, m, cleanflag, step = 0;
	
	for (i = 0; i < 7; i +=2) 
		board[b->bm + brick_num[b->b1][b->b2][i]][b->bn + brick_num[b->b1][b->b2][i+1]] = 1;
	//print_board();
	for (m = b->bm; m <= maxm; m++) {
		cleanflag = 1;
		for (n = 0; n < maxn; n++)
			if (board[m][n] == 0)
				cleanflag = 0;
		if (cleanflag) {
			cleanline(m);
			step += 1;
		}
	}

	switch (step) {
		case 0:
			return 1;
			break;
		case 1:
			scope += 100;
			break;
		case 2:
			scope += 300;
			break;
		case 3:
			scope += 500;
			break;
		case 4:
			scope += 800;
			break;
	}
	
	print_scope(scope);		

	return 1;
}

int check_up_ava(struct brickbox *b, int oldb2){
	int i, m, n;
	for (i = 0; i < 7; i +=2)
		if (brick_num[b->b1][b->b2][i] != brick_num[b->b1][oldb2][i] && brick_num[b->b1][b->b2][i+1] != brick_num[b->b1][oldb2][i+1]) {
			m = b->bm + brick_num[b->b1][b->b2][i];
			n = b->bn + brick_num[b->b1][b->b2][i+1];
			if (m > maxm || n > maxn || board[m][n] == 1) {
				return -1;
				}
	}
	return 0;
}

int check_next_step(struct brickbox *b, int way) {
	int i, m, n, j, m1=0, n1=0, flag = 0;
	switch (way) {
		case LEFT:
			m1 = 0, n1 = -1;
			break;
		case RIGHT:
			m1 = 0, n1 = 1;
			break;
		case DOWN:
			m1 = 1, n1 = 0;
			break;
		case UP:
			return 0;
			break;
		default:
			break;
	}

	for (i = 0; i < 7; i +=2) {
		m = brick_num[b->b1][b->b2][i] + m1;
		n = brick_num[b->b1][b->b2][i+1] + n1;
		flag = 0;
		for (j = 0; j < 7; j +=2) 
			if (brick_num[b->b1][b->b2][j] == m \
				&& brick_num[b->b1][b->b2][j+1] == n )
					flag = 1;
		if (flag == 0 && board[b->bm + m][b->bn + n] == 1) 
				return -1;
	}
	return 0;
}
