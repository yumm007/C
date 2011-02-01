#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pth_qsort.h"

#define	ARR_SIZE		1<<10<<10

static void rand_arr(unsigned int *arr, size_t arr_size) {
	int i;

	srand(getpid());
	for (i = 0; i < arr_size; i++) {
		arr[i] = rand();
		srand(arr[i]);
	}
	return;
}

static int int_cmp(const void *data1, const void *data2) {
	return *(unsigned int *)data1 - *(unsigned int *)data2;
}

static void print_arr(unsigned int *arr, size_t arr_size) {
	int i;
	for (i = 0; i < arr_size; i++) {
		fprintf(stdout,"%15u",arr[i]);
		if ((i+1) % 7 == 0)
		 	fprintf(stdout, "\n");
	}
	
	fprintf(stdout, "\n");
	return;
}

int main(void)
{
	unsigned int arr[ARR_SIZE];
	struct sort_data data;

	rand_arr(arr, ARR_SIZE);

	data.base = arr;
	data.left =	0;
	data.right = (ARR_SIZE) -1;
	data.size = sizeof(arr[0]);
	data.cmp = int_cmp;

	pth_sort(&data);
	print_arr(arr, ARR_SIZE);

	return 0;
}
