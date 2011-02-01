#include "myhead.h"

extern int brick_num[7][4][4][4];

void move_brick(struct brick *b, int flag) {
	int i, j, s, xlim, xmax, ylim;

	if (flag == CLEAN) 
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++) {
					xlim = b->x +(i-0) * D;
						clean_box(xlim, b->y + j * D, D);
			}
	else
		for (i = s; i < 4; i++)
			for (j = 0; j < 4; j++) {
					xlim = b->x +(i-s) * D;
					ylim = b->y + j * D;
					if (xlim >= 50 && xlim <= (xres() - 450) && brick_num[b->b1][b->b2][j][i] == 9)
						draw_box(xlim, ylim, D);
			}
	
}

