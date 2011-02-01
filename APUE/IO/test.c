#include <stdio.h>
int main(void)
{
	FILE *fp;
	int buf[20];

	fp = fopen("test", "r");
	printf("%d", fread(buf, 4, 20, fp));

	return 0;
}
