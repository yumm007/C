/* 	Hello World
*/
#include <stdio.h>
#include <string.h>
int main(){
	
	int a[4];
	int i;
	int d = 9;

	for (i = -2; i < 6; i++)
		printf("a[%d] = %d\n", i, a[i]);
	
	const int c = 6;
	const int *p = &c;

	printf("c = %d\t", c);
	p = &d;
	printf("d = %d\n", *p);
 	
	printf("string 'hello' lenght is %d.\n",strlen("hello"));
	
	int aa = (3, 4);
	printf("aa=%d\n", aa);

	struct {
		int a:3;
	}x;

	x.a = 1;
	x.a += 1;
	x.a += 1;
	x.a += 1;
	printf("%d\n",x.a);

	return 0;
}
