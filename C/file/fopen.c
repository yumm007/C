#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *fp;

	fp = fopen(argv[1],"r");
	
	printf("sizeof FILE %d\n", sizeof(*fp));
	
	return 0;
}
