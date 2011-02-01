#ifndef HS_TERM_H
#define HS_TERM_H

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include "fb_draw.h"

#define MAXBOARD 50
#define CLEAN 0
#define FINISH 2
#define DRAW 1
#define D 25

enum WAY {UP = 65, DOWN, RIGHT, LEFT};

struct brickbox {
	int b1;
	int b2;
	int bm;
	int bn;
};

void init_board(void);
int check_way(int way, int flag);
int read_key(void);
void clean_box(int x, int y, int size);
void draw_box(int x, int y, int size);
void move_brick(struct brickbox *b, int flag);
int set_board(struct brickbox *b);
int end_of_down(struct brickbox *b);
int check_next_step(struct brickbox *b, int way);
int check_up_ava(struct brickbox *b, int oldb2);
void rand_box(struct brickbox *b);
int wait_min(void);
void pause_fun(void);
void print_board(void);
void draw_board(void);
void print_scope(int num);

#endif
