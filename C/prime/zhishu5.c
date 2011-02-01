#include <stdio.h>
#include <math.h>
#define MAXNUM 100

int main(void)
{
	int num[MAXNUM] = {1,1,};
	int i, j;

	for (i = 2; i <= sqrt(MAXNUM) + 1; i++)
		if (num[i] == 0) 
			for (j = i + i; j < MAXNUM; j += i)
				num[j] = 1;	

	for (i = 0; i < MAXNUM; i++)
		if (num[i] == 0)
			printf("%d ",i);
	printf("\n");
}

