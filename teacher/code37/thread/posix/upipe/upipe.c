#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "upipe.h"

struct upipe_st {
	pthread_mutex_t mut;
	pthread_cond_t cond;
	size_t pipesize;
	int head, tail;
	int count;
	char data[1];
};

upipe_t *upipe_new(size_t pipesize)
{
	struct upipe_st *newnode;

	newnode = malloc(sizeof(struct upipe_st)+pipesize-1);
	if (newnode==NULL) {
		return NULL;
	}

	newnode->pipesize = pipesize;
	newnode->head = 0;
	newnode->tail = 0;
	newnode->count = 0;
	pthread_mutex_init(&newnode->mut, NULL);
	pthread_cond_init(&newnode->cond, NULL);

	return newnode;
}

int upipe_delete(upipe_t *ptr)
{
	struct upipe_st *p=ptr;

	pthread_cond_destroy(&p->cond);
	pthread_mutex_destroy(&p->mut);
	free(p);
	return 0;
}

static int
upipe_read_byte(struct upipe_st *p, char *datap)
{
	if (p->count<=0) return -1;

	*datap = p->data[p->head];
	p->head = next(p->head);
	p->count--;
	return 0;
}

ssize_t upipe_read(upipe_t *ptr, void *datap, size_t size)
{
	struct upipe_st *p=ptr;
	int i;
	char *chardatap=datap;

	pthread_mutex_lock(&p->mut);

	while (p->count<=0)
		pthread_cond_wait(&p->cond, &p->mut);

	for (i=0;i<size;++i) {
		if (upipe_read_byte(ptr, chardatap+i)<0) {
			break;
		}
	}
	pthread_cond_broadcast(&p->cond);
	pthread_mutex_unlock(&p->mut);

	return i;
}

static int
upipe_write_byte(struct upipe_st *p, int c)
{
	if (p->count>=p->pipesize) return -1;

	p->data[p->tail] = c;
	p->tail = next(p->tail);
	p->count++;
	return 0;
}

ssize_t upipe_write(upipe_t *ptr, const void *datap, size_t size)
{
	struct upipe_st *p=ptr;
	int i;
	const char *chardatap=datap;

	pthread_mutex_lock(&p->mut);

	while (p->count>=p->pipesize-UPIPE_ATOMICSIZE)
		pthread_cond_wait(&p->cond, &p->mut);

	for (i=0;i<size;++i) {
		if (upipe_write_byte(ptr, chardatap[i])<0) {
			break;
		}
	}
	pthread_cond_broadcast(&p->cond);
	pthread_mutex_unlock(&p->mut);

	return i;
}

int
upipe_stat_unlocked(upipe_t *ptr, struct upipe_stat *res)
{
	res->count = p->count;
	return 0;
}

