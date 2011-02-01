#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stddef.h>

#include "pth_stack.h"

#define S_EMPTY		0
#define S_FULL			1
#define S_CLOSED		2

typedef struct {
	unsigned int	magic;
	size_t buf_size;
	int	 status;
	size_t point;
	pthread_cond_t		cond;
	pthread_mutex_t 	mute;
	void *dap[1]; /*每个指针指向的空间都四个字节是size，后面的字节是数据*/
} pth_st;

PTHS *pths_open(size_t bufsize) {
	pth_st *ret = NULL;
	int i;
	
	if ((ret = malloc(sizeof(pth_st)+sizeof(void *)*(bufsize-1))) != NULL){
		ret->magic = 0x7645AACC;
		ret->buf_size = bufsize;
		ret->status = S_EMPTY;
		ret->point = 0;
		pthread_mutex_init(&ret->mute, NULL);
		pthread_cond_init(&ret->cond, NULL);
		
		for (i = 0; i < bufsize; i++)
			ret->dap[i] = NULL;
	}	

	return ret;
}

int pths_close_wr(PTHS *pth) {
	pth_st *cur = pth;

	if (cur->magic != 0x7645AACC)
		return E_CLSD_UN;

	pthread_mutex_lock(&cur->mute);
	cur->point = 0;
	cur->status = S_CLOSED;
	pthread_cond_broadcast(&cur->cond);
	pthread_mutex_unlock(&cur->mute);

	return 0;
}

int pths_close(PTHS *pth) {
	int i;
	pth_st *cur = pth;

	if (cur->magic != 0x7645AACC)
		return E_CLSD_UN;

	for (i = 0; i < cur->buf_size; i++) 
		free(cur->dap[i]);

	pthread_mutex_destroy(&cur->mute);
	pthread_cond_destroy(&cur->cond);
	free(pth);

	return 0;
}

static size_t move_data(void *buf, size_t bufsize, void *sour) {
	size_t datas;

	datas = *(size_t *)sour;
	if (datas > bufsize)
		datas = bufsize;
	memcpy(buf, (char *)sour + sizeof(size_t), datas);
	free(sour);

	return datas;
}

size_t pths_pop(PTHS *pth, void *buf, size_t bufsize) {
	pth_st *cur = pth;
	void *tmp;

	pthread_mutex_lock(&cur->mute);
	while (cur->point == 0 && cur->status != S_CLOSED)
		pthread_cond_wait(&cur->cond, &cur->mute);
	/* 栈空则退出*/
	if (cur->point == 0 && cur->status == S_CLOSED) {
		pthread_mutex_unlock(&cur->mute);
		return E_EMPTY;
	}

	cur->point--;
	tmp = cur->dap[cur->point];
	cur->dap[cur->point] = NULL;
	pthread_cond_broadcast(&cur->cond);
	pthread_mutex_unlock(&cur->mute);

	return move_data(buf, bufsize, tmp);
}

int pths_push(PTHS *pth, const void *data, size_t size) {
	void *tmp;
	pth_st *cur = pth;

	if ((tmp = malloc(sizeof(size_t) + size)) == NULL)
		return E_MALLOC;
	*(int *)tmp = size;
	memcpy((char *)tmp + sizeof(size_t), data, size);

	pthread_mutex_lock(&cur->mute);
	/*栈满则等待*/
	while (cur->point == cur->buf_size)
		pthread_cond_wait(&cur->cond, &cur->mute);

	cur->dap[cur->point] = tmp;
	cur->point++;

	pthread_cond_broadcast(&cur->cond);
	pthread_mutex_unlock(&cur->mute);

	return 0;
}

