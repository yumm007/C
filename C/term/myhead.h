#ifndef HS_TERM_H
#define HS_TERM_H

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include "fb_draw.h"

#define CLEAN 0
#define DRAW 1
#define D 25

void init_board(void);
void move_box(int way);
int read_key(void);
void clean_box(int x, int y, int size);
void draw_box(int x, int y, int size);

struct brick {
	int b1;
	int b2;
	int x;
	int y;
};


#endif
