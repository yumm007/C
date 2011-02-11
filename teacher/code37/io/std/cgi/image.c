#include <stdio.h>

#define FNAME "/usr/share/backgrounds/images/Purple.jpg"

#define BUFSIZE 1024

int
main()
{
	int ret;
	FILE *fp;
	char buf[BUFSIZE];

	fp = fopen(FNAME, "rb");
	if (fp==NULL) {
		exit(1);
	}

	/* MIME */
	printf("Content-type: image/jpeg\r\n\r\n");

	while (1) {
		ret = fread(buf, 1, BUFSIZE, fp);
		if (ret<=0) {
			break;
		}
		fwrite(buf, 1, ret, stdout);
	}

	fclose(fp);

	return 0;
}

