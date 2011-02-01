/*
**
**
**
**
*/
#include <stdio.h>

int search( int key, int num[], int len ){
		
	int start, end, mid;
	
	start = 0; 
	end = len - 1;

	while (start <= end) {
		mid = (start + end) / 2;
		if (key == num[mid])
			return mid;
		else if (key > num[mid])
			start = mid + 1;
		else
			end = mid -1;
	}
	return -1;
}

int main(void)
{
	int ind, num[15]={1,3,5,7,9,11,13,15,17,18,20,33,35,56,23};
	
	ind = search(1, num, 15);

	printf("Ind = %d\n", ind);

	return 0;
}
