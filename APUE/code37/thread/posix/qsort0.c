#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int arr[]={7,3,1,4,1,9,6,8,2,  -1};

static void
my_int_qsort(int *datap, int left, int right)
{
	int l,r;
	int tmp;

	if (left>=right || left<0 || right<0) {
		return;
	}

	l=left;
	r=right;
	while (r>l) {
		for (;datap[r]>=datap[l]&&r>l;--r);
		tmp = datap[r];
		datap[r] = datap[l];
		datap[l] = tmp;
		for (;datap[r]>=datap[l]&&r>l;++l);
		tmp = datap[r];
		datap[r] = datap[l];
		datap[l] = tmp;
	}
//	printf("l=%d, m=%d, r=%d\n", left, r, right);
	my_int_qsort(datap, left, r-1);
	my_int_qsort(datap, r+1, right);
}

int
main()
{
	int i, c;

	c=0;
	for (i=0;arr[i]>=0;++i) {
		printf("%d,", arr[i]);
		c++;
	}
	putchar('\n');
	my_int_qsort(arr, 0, c-1);
	for (i=0;arr[i]>=0;++i) {
		printf("%d,", arr[i]);
	}
	putchar('\n');

	return 0;
}

