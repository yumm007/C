#include <stdio.h>
#define MAX 10

int printarr(int *arr,int m, int n) {
	int i, j, p = 0;;

	for (j = 0; j < m; j++) {
		for (i = 0; i < n; i++) {
			printf("%d\t",*(arr + j * n + i));
		}
		printf("\n");
	}
	return p - 1;
}

int main(void)
{
	int arr[3][8] ={{0,1,2,3,4,5,6,7},{2,4,5},{8,11,56,89}};
	//int arr[8] ={0,1,2,3,4,5,6,7};
	int m = 3, n = 8;
	
	printarr((int *)arr,m, n);

	return 0;
}
