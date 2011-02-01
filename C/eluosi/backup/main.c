#include "myhead.h"

int board[MAXHEIGHT][MAXWIDTH];
int main(void)
{
	struct termios old, new;
	int ret, way, flag = 1;

	tcgetattr(0, &old);
	tcgetattr(0, &new);

	new.c_lflag &= ~(ICANON | ECHO);
	new.c_cc[VTIME] = DEFAULT;
	new.c_cc[VMIN] = 0;

	tcsetattr(0, TCSANOW, &new);

   ret = fb_open();
   if (ret != 0) {
	fprintf(stderr, "fb_open() error.\n");
		exit(1);
   }
	
	init_board();
	
	while ((way = read_key()) != -1) {
		if ( (ret = check_way(way, flag)) == 1)
			flag = 1;
		else if (ret == 0)
			flag = 0;
		else if (ret == FINISH) {
			printlet(650, 450, "GAME OVER\nAGAIN Y OR N");
			new.c_cc[VTIME] = 0;
			new.c_cc[VMIN] = 1;
			tcsetattr(0, TCSANOW, &new);
			while ((ret = getchar()))
				if (ret == 'Y' || ret == 'y') {
					break;
				} else if (ret == 'N' || ret == 'n') {
					fb_close();
					tcsetattr(0, TCSANOW, &old);
					return 0;
				}
			new.c_cc[VTIME] = DEFAULT;
			new.c_cc[VMIN] = 0;
			tcsetattr(0, TCSANOW, &new);
			flag = 1;
			init_board();
		}
	}

	fb_close();
	tcsetattr(0, TCSANOW, &old);
	printf("\033[?25h"); //恢复光标

	return 0;
}
