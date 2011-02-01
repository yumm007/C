#ifndef HS_TERM_H
#define HS_TERM_H

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "fb_draw.h"
#include "pic.h"

#define MAXHEIGHT 30
#define MAXWIDTH	15
#define CLEAN 		0
#define DRAW 		1
#define FINISH 	2
#define D 			30
#define DEFAULT 	3
#define HIGHLEVEL	1
#define MEDIUM		2
#define COLOR		8
#define MEDIUM_SCORE		2000
#define HIGH_SCORE		5000

enum WAY {UP = 65, DOWN, RIGHT, LEFT};

struct brickbox {
	int b1;
	int b2;
	int bm;
	int bn;
	int c;
};

void init_board(void);
int check_way(int way, int flag);
int read_key(void);
void draw_box_v2(int x, int y, int size, struct color_v *c);
void move_brick_v2(struct brickbox *b, int flag);
int set_board(struct brickbox *b);
void set_lev(int min);
int check_next_step(struct brickbox *b, int way);
int check_up_ava(struct brickbox *b, int oldb2);
void rand_box(struct brickbox *b);
int wait_min(void);
void pause_fun(void);
void print_board(void);
void draw_board(void);
void print_scope(int num);
void pause_fun(void);
int _max(int b1, int b2, int flag);
void printlet(int x, int y, char *word);
void printdig(int x, int y, int num);
extern int background_pic(char *filepath);

#endif
