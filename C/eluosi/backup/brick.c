#include "myhead.h"
#define 	BG 	0x00

extern int brick_num[7][4][8];
extern int board[MAXHEIGHT][MAXWIDTH];
extern struct color_v box_color[COLOR];
extern int maxm, maxn, scope;

void cleanline(int line) {
	int m, n, x, y;
	
	for (m = line; m >0; m--)
		for (n = 0; n <= maxn; n++){
			board[m][n] = board[m-1][n];
			x = n * D + 50;	
			y = m * D + 100;
			if (board[m][n] > 0 && board[m][n] < COLOR) 
				draw_box_v2(x, y, D, box_color + board[m][n]);
			else
				draw_box_v2(x, y, D, box_color + 0);
		}
}

int set_board(struct brickbox *b) {
	int i, n, m, cleanflag, step = 0;
	
	for (i = 0; i < 7; i +=2) 
		board[b->bm + brick_num[b->b1][b->b2][i]][b->bn + brick_num[b->b1][b->b2][i+1]] = b->c;

	for (m = b->bm; m <= maxm; m++) {
		cleanflag = 1;
		for (n = 0; n <= maxn; n++)
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

	if (scope > MEDIUM_SCORE && scope < HIGH_SCORE)
		set_lev(MEDIUM);
	else if (scope >= HIGH_SCORE)	
		set_lev(HIGHLEVEL);

	return 1;
}

int check_up_ava(struct brickbox *b, int oldb2){
	int i, m, n, m1, n1;
	for (i = 0; i < 7; i +=2) {
		m1 = brick_num[b->b1][b->b2][i];
		n1 = brick_num[b->b1][b->b2][i+1];
		if (m1 != brick_num[b->b1][oldb2][i] && n1 != brick_num[b->b1][oldb2][i+1]) {
			m = b->bm + m1;
			n = b->bn + n1;
			if (m > maxm || n > maxn || (board[m][n] > 1 && board[m][n] < COLOR))
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
		if (flag == 0 && b->bm + m > 0 && \
			board[b->bm + m][b->bn + n] > 0 && \
			board[b->bm + m][b->bn + n] < COLOR)
			return -1;

	}
	return 0;
}
