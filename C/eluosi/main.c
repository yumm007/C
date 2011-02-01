#include "myhead.h"

int board[MAXHEIGHT][MAXWIDTH];
FILE *fp, *fplog, *fplog2;

void eluosi(int mod) {
	int ret, way, flag = 1, save_way = 1;
	static struct brickbox b;

	int (*col_key)(int flag);
	void (*col_box)(struct brickbox *b);


	if (mod == KEYBOARD) {
		col_key = read_key;
		col_box = rand_box;
		if ((fplog = fopen("testlog", "w+")) == NULL) {
			printf("open testlog error\n");
			return;
		}	
		if ((fp = fopen(SAVEFILE, "w+")) == NULL) {
			printf("open savefiel for write faild\n");
			return;
		}
	} else {
		col_key = read_key_from_file;
		col_box = read_box_from_file;
		if ((fplog2 = fopen("testlog2", "w+")) == NULL){
			printf("open testlog2 error\n");
			return; 
		}
		if ((fp = fopen(SAVEFILE, "rb")) == NULL) {
			printf("open savefiel for read faild\n");
			return;	
		}
		printf(" I am here\n");
	}

	init_board();
	
	while ((way = col_key(save_way)) != -1) {
		if (flag == 1) {
			col_box(&b);
			save_way = 1;
		}
		if ( (ret = check_way(way, flag, b)) == 1) {
			flag = 1;
			save_way = 0;
		}else if (ret == 0) 
			flag = 0;
		else if (ret == FINISH) {
			if (reset() == -1)
				break;
			flag = 1;
			init_board();
		}
	}
	

	if (mod == KEYBOARD)
		fclose(fplog);
	else
		fclose(fplog2);
	fclose(fp);
}

int main(int argc, char *argv[])
{
	struct termios old, new;


	int ret;

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

	if (argc == 1) {
		printf("hear KEY\n");
		eluosi(KEYBOARD);
	} else if (*argv[1] == 'l' ) {
		printf("hear 99\n");
		eluosi(99);
	}
	fb_close();
	tcsetattr(0, TCSANOW, &old);
	printf("\033[?25h"); //恢复光标

	return 0;
}
