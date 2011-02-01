#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stddef.h>

#include "pth_queue.h"

#define Q_OPENED		0
#define Q_CLOSED		1

typedef struct {
	unsigned int	magic;
	size_t buf_size;
	int	 status;
	size_t rdp, wrp;
	pthread_cond_t		rcond;
	pthread_mutex_t 	rmute;
	pthread_cond_t		wcond;
	pthread_mutex_t 	wmute;
	void *dap[1]; /*每个指针指向的空间都四个字节是size，后面的字节是数据*/
} pth_st;

PTHQ *pthq_open(size_t bufsize) {
	pth_st *ret = NULL;
	int i;
	
	if ((ret = malloc(sizeof(pth_st)+sizeof(void *)*(bufsize-1))) != NULL){
		ret->magic = 0x7645AACC;
		ret->buf_size = bufsize;
		ret->status = Q_OPENED;
		ret->rdp = ret->wrp = 0;
		pthread_mutex_init(&ret->rmute, NULL);
		pthread_cond_init(&ret->rcond, NULL);
		pthread_mutex_init(&ret->wmute, NULL);
		pthread_cond_init(&ret->wcond, NULL);

		for (i = 0; i < ret->buf_size; i++)
			ret->dap[i] = NULL;
	}	

	return ret;
}

int pthq_close_wr(PTHQ *pth) {
	pth_st *cur = pth;

	pthread_mutex_lock(&cur->rmute);
	if (cur->magic != 0x7645AACC)
		return E_CLSD_UN;
	cur->status = Q_CLOSED;
	pthread_cond_broadcast(&cur->rcond);
	pthread_mutex_unlock(&cur->rmute);
	return 0;
}

int pthq_delete(PTHQ *pth) {
	int i;
	pth_st *cur = pth;

	if (cur->magic != 0x7645AACC)
		return E_CLSD_UN;

	for (i = 0; i < cur->buf_size; i++) 
		free(cur->dap[i]);

	pthread_mutex_destroy(&cur->rmute);
	pthread_cond_destroy(&cur->rcond);
	pthread_mutex_destroy(&cur->wmute);
	pthread_cond_destroy(&cur->wcond);
	free(pth);

	return 0;
}

static int is_closed_unlock(pth_st *cur) {
	if (cur->dap[cur->rdp] == NULL && cur->status == Q_CLOSED) {
		pthread_mutex_unlock(&cur->rmute);
		return 1;
	}
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

size_t pthq_read(PTHQ *pth, void *buf, size_t bufsize) {
	pth_st *cur = pth;
	void *tmp;

	pthread_mutex_lock(&cur->rmute);
	/* 队列空则阻塞之*/
	while (cur->dap[cur->rdp] == NULL && cur->status == Q_OPENED) {
		pthread_cond_wait(&cur->rcond, &cur->rmute);
	}
	if (is_closed_unlock(cur)) return 0;
	
	tmp = cur->dap[cur->rdp];
   cur->dap[cur->rdp] = NULL;

	if (++cur->rdp == cur->buf_size)
		cur->rdp = 0;

	pthread_cond_broadcast(&cur->wcond);
	pthread_mutex_unlock(&cur->rmute);

	return move_data(buf, bufsize, tmp);
}

int pthq_write(PTHQ *pth, const void *data, size_t size) {
	void *tmp;
	pth_st *cur = pth;

	if (cur->status == Q_CLOSED)
		return E_CLSD_Q;

	if ((tmp = malloc(sizeof(size_t) + size)) == NULL)
		return E_MALLOC;
	*(int *)tmp = size;
	memcpy((char *)tmp + sizeof(size_t), data, size);

	pthread_mutex_lock(&cur->wmute);
	while (cur->dap[cur->wrp] != NULL) /* 队列满则阻塞之*/
		pthread_cond_wait(&cur->wcond, &cur->wmute);

	cur->dap[cur->wrp] = tmp;

	if (++cur->wrp == cur->buf_size)
		cur->wrp = 0;
	pthread_cond_broadcast(&cur->rcond);
	pthread_mutex_unlock(&cur->wmute);

	return 0;
}

