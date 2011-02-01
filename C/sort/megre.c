#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void __megresort(int num[], int tmp[], size_t mid, size_t n) {

	int p1, p2, ind;

	p1 = 0;
	p2 = mid;
	ind = 0;

	while (p1 < mid && p2 < n) {
		if (num[p1] < num[p2]) {
			tmp[ind] = num[p1];
			p1++;
		}	else {
			tmp[ind] = num[p2];
			p2++;
		}
		ind++;	
	}	

	for (;p1 < mid; p1++, ind++)
		tmp[ind] = num[p1];
	for (;p2 < n; p2++, ind++)
		tmp[ind] = num[p2];
	
	memcpy(num, tmp, n * sizeof(num[0]));
}

void __megre(int num[], int tmp[], size_t n) {
	int mid;

	if (n == 1)
		return;

	mid = n / 2;

	__megre(num, tmp, mid);
	__megre(num + mid, tmp + mid, n - mid);

	__megresort(num, tmp, mid, n);
	
	return;
}

int  megre(int num[], size_t n) {

	int *tmp, mid;

	if ((tmp =(int *) malloc(n)) == NULL)
		return -1;

	__megre(num, tmp, n);

	free(tmp);	

	return 0;
}

int main(void)
{
	int i;
	int num[] = {2,56,34,100,45,78,234,567,98,56,345,12,33,68,236,678};

	megre(num, sizeof(num) / sizeof(num[0]));

	for(i = 0; i < sizeof(num) / sizeof(num[0]); i++)
		printf("%d ",num[i]);	
	printf("\n");	

	return 0;
}
