#include <stdio.h>

int main(void){

	int a = 255;

	printf(" Set bit 3 of 255 to 1: \t%d\n", 1 << 3 | a );
	printf(" Set bit 3 of 255 to 0: \t%d\n", ~(1 << 3) & a );
	printf(" Get bit 3 from 255: \t\t%d\n", a >> 3 & 1 );

	return 0;
}
