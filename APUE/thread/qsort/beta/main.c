#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "pth_queue.h"

#define	ARR_SIZE	1<<10

unsigned int arr[ARR_SIZE];
PTHQ *pthq;

typedef int cmp_fun(const void *data1, const void *data2);

struct sort_data{
	void *base;
	int left;
	int right;
	size_t size;
	cmp_fun *cmp;
};

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

static int swap_data(void *data1, void *data2, size_t size) {
	void *tmp;
	
	if ((tmp = malloc(size)) == NULL)
		return -1;

	memcpy(tmp, data2, size);
	memcpy(data2, data1, size);
	memcpy(data1,tmp, size);

	return 0;
}

static void *pth_qsort_des(void *pth) {
	int l, r, tmp;
	void *data1, *data2;
	PTHQ *pthq = pth;
	struct sort_data data;
	pthread_t pid;

	pthq_read(pthq, &data, sizeof(data));

	if (data.left >= data.right)
		pthread_exit(NULL);

	l = data.left;
	r = data.right;

	while (l < r) {
		data1 = (char *)(data.base) + l * data.size;
		for (;l < r ; r--) {
			data2 = (char *)(data.base) + r * data.size;
			if (data.cmp(data1, data2) > 0){
		  		swap_data(data1, data2, data.size);
				break;
			}
		}
		data2 = (char *)(data.base) + r * data.size;
		  for (;l < r; ++l) {
			 data1 = (char *)(data.base) + l * data.size;
			 if (data.cmp(data1, data2) > 0) {
				swap_data(data1, data2, data.size);
				break;
			 }
	  }
	}

	tmp = data.right;
	data.left = data.left;
	data.right = l -1;
	pthq_write(pthq, &data, sizeof(data));	
	pthread_create(&pid, NULL, pth_qsort_des, pth);
	pthread_detach(pid);

	data.left = l + 1;
	data.right = tmp;
	pthq_write(pthq, &data, sizeof(data));	
	pthread_create(&pid, NULL, pth_qsort_des, pth);
	pthread_detach(pid);

	pthread_exit(NULL);
}

static void buy(void) {
	print_arr(arr, ARR_SIZE);
	pthq_delete(pthq);
}

int main(void)
{
	struct sort_data data;
	pthread_t pid;

	if ((pthq = pthq_open(1024)) == NULL) {
		fprintf(stderr, "open queue failed.\n");
		return -1;
	}

	rand_arr(arr, ARR_SIZE);
	atexit(buy);

	data.base = arr;
	data.left =	0;
	data.right = (ARR_SIZE) -1;
	data.size = sizeof(arr[0]);
	data.cmp = int_cmp;

	pthq_write(pthq, &data, sizeof(data));	
	pthread_create(&pid, NULL, pth_qsort_des, (void *)pthq);
	pthread_detach(pid);

	pthread_exit(NULL);
}
