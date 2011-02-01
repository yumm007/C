#include "main.h"

int main(int argc, char *argv[])
{
	int ret;
	BMP_FH bmp_fh;		
	BMP_FI bmp_fi;
	FILE *fp;
	
	ret = fb_open();
	if (ret != 0) {
	  fprintf(stderr, "fb_open() error.\n");
	  exit(1);
	}

	if ((fp = fopen(argv[1],"r")) == NULL) {
		printf("fopen: %s error!\n", argv[1]);
		return -1;
	}

	if (init_bmp(&bmp_fh, &bmp_fi, fp) == -1) {
		printf("read picture information faild!\n");
		fclose(fp);
		return -1;
	} 

	draw_pic(&bmp_fi, fp, 0, 0);

	fclose(fp);
   fb_close();
   return 0;
}
