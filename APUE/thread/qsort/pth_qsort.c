#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "pth_qsort.h"
#include "pth_stack.h"

#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN 16384
#endif

#define	CONTINUE	1
#define	BREAK		0

static int job_sta[PTH_NUM], job_num;
static pthread_mutex_t job_mut;
static pthread_cond_t  job_cond;

static int swap_data(void *data1, void *data2, size_t size) {
	void *tmp;
	
	if ((tmp = malloc(size)) == NULL)
		return -1;

	memcpy(tmp, data2, size);
	memcpy(data2, data1, size);
	memcpy(data1,tmp, size);

	free(tmp);

	return 0;
}

static int find_mid(struct sort_data *data) {
	void *data1, *data2;
	int l, r;

	l = data->left;
	r = data->right;

  while (l < r) {
	 data1 = (char *)(data->base) + l * data->size;
	 for (;l < r ; r--) {
		data2 = (char *)(data->base) + r * data->size;
		if (data->cmp(data1, data2) > 0){
		  swap_data(data1, data2, data->size);
		  break;
		}
	 }
	 data2 = (char *)(data->base) + r * data->size;
		 for (;l < r; ++l) {
			data1 = (char *)(data->base) + l * data->size;
			if (data->cmp(data1, data2) > 0) {
			  swap_data(data1, data2, data->size);
			  break;
			}
		 }
  }
	return l;
}

static void *pop1_push2(void *pth);
static void tmp_pthread(void *pth){
	pthread_t pid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);

	if (pthread_create(&pid, &attr, pop1_push2, pth) != 0)
		pthread_detach(pid);

	pthread_attr_destroy(&attr);
}

static void *pop1_push2(void *pth) {
	int mid, tmp;
	PTHS *pths = pth;
	struct sort_data data;

	if (pths_pop(pths, &data, sizeof(data)) == E_EMPTY)
		return (void *)BREAK;
	if (data.left >= data.right)
		return (void *)CONTINUE;
	mid = find_mid(&data);

	tmp = data.right;
	data.left = data.left;
	data.right = mid -1;
	while (pths_push(pths, &data, sizeof(data)) == E_FULL)
		tmp_pthread(pth);

	data.left = mid + 1;
	data.right = tmp;
	while (pths_push(pths, &data, sizeof(data)) == E_FULL)
		tmp_pthread(pth);

	return (void *)CONTINUE;
}


static void *pth_qsort_des(void *pth) {
	int num;

	pthread_mutex_lock(&job_mut);
	job_num++;
	num = job_num;
	pthread_mutex_unlock(&job_mut);

	while((int)pop1_push2(pth)) {
		if (job_sta[num] == 0) {
			job_sta[num] = 1;
			pthread_cond_signal(&job_cond);
		}
	}

	job_sta[num] = 1;
	pthread_cond_signal(&job_cond);

	pthread_exit(NULL);
}

int pth_sort(struct sort_data *data) {
	pthread_t pid[PTH_NUM];
	int i;
	PTHS *pths;

	job_num = -1;
	pthread_mutex_init(&job_mut, NULL);
	pthread_cond_init(&job_cond, NULL);

	if ((pths = pths_open(STACK_SIZE)) == NULL) 
		return EMALLOC;

	pths_push(pths, data, sizeof(*data));	

	for (i = 0; i < PTH_NUM; i++)
		job_sta[i] = 0;

	for (i = 0; i < PTH_NUM; i++) {
		pthread_mutex_lock(&job_mut);
		pthread_create(pid + i, NULL, pth_qsort_des, (void *)pths);
		while (job_sta[i] == 0)
			pthread_cond_wait(&job_cond, &job_mut);
		pthread_mutex_unlock(&job_mut);
	}

	for (i = 0; i < PTH_NUM; i++) 
		pthread_join(pid[i], NULL);

	pthread_mutex_destroy(&job_mut);
	pthread_cond_destroy(&job_cond);
	pths_close(pths);
	return 0;
}
