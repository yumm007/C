#include <stdio.h>

int printarr(int (*arr)[], int len[2]) {
	int i, j, p = 0;;

	for (j = 0; j < len[0]; j++) {
		for (i = 0; i <len[1]; i++) {
			printf("%d\t",(*arr)[p++]);
		}
		printf("\n");
	}
	return p - 1;
}

int main(void)
{
	int arr[3][8] ={{0,1,2,3,4,5,6,7},{2,4,5},{8,11,56,89}};
	//int arr[8] ={0,1,2,3,4,5,6,7};
	int len[2] = {3,8};
	
	printarr(&arr[0], len);

	return 0;
}
