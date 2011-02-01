#include <stdio.h>
#include <stdlib.h>

int main(void) 
{
	int *p1, *p2;
	int arr[25];	
#if 0
	p2 = p1 = malloc(sizeof(int));
	*p1 = 4;

	free(p1);

	printf("%d\n", *p2);

	for (p1=&arr[0]; p1<&arr[25];)
		*++p1 = 0;
#endif
	if ( -3 * (-3) == 9)
		printf("Yes\n");

	return 0;
}
