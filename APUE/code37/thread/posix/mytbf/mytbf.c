#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#include "mytbf.h"

struct mytbf_st {
	int cps;
	int burst;
	int token;
	int pos;
};

static struct mytbf_st *tbf_job[TBF_MAX];
static pthread_mutex_t tbf_job_mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_t timer_tid;

static int
find_free_pos_locked(void)
{
	int i;

	pthread_mutex_lock(&tbf_job_mut);
	for (i=0;i<TBF_MAX;++i) {
		if (tbf_job[i]==NULL) {
			return i;
		}
	}
	return -1;
}

static void*
alrm_thr(void *unused)
{
	int i;
	struct timespec t, tleft;

	t.tv_sec = 1;
	t.tv_nsec = 0;
	while (1) {
		tleft=t;
		while (nanosleep(&tleft, &tleft)<0);
		pthread_mutex_lock(&tbf_job_mut);
		for (i=0;i<TBF_MAX;++i) {
			if (tbf_job[i]!=NULL) {
				tbf_job[i]->token +=  tbf_job[i]->cps;
				if (tbf_job[i]->token > tbf_job[i]->burst) {
					tbf_job[i]->token = tbf_job[i]->burst;
				}
			}
		}
		pthread_mutex_unlock(&tbf_job_mut);
	}
}

static void
global_destruct(void)
{
	int i;

	pthread_cancel(timer_tid);
	pthread_join(timer_tid, NULL);

	pthread_mutex_destroy(&tbf_job_mut);

	for (i=0;i<TBF_MAX;++i) {
		free(tbf_job[i]);
	}
}

static pthread_once_t tbf_init_once = PTHREAD_ONCE_INIT;
static void
global_contruct(void)
{
	int i;
	struct itimerval itv;

	for (i=0;i<TBF_MAX;++i) {
		tbf_job[i] = NULL;
	}

	pthread_create(&timer_tid, NULL, alrm_thr, NULL);

	atexit(global_destruct);
}

MYTBF *tbf_context_new(int cps, int burst)
{
	struct mytbf_st *newnode;
	int pos;

	pthread_once(&tbf_init_once, global_contruct);

	newnode = malloc(sizeof(struct mytbf_st));
	if (newnode==NULL) {
		return NULL;
	}
	newnode->token = 0;
	newnode->cps = cps;
	newnode->burst = burst;

	pos = find_free_pos_locked();
	if (pos<0) {
		free(newnode);
		pthread_mutex_unlock(&tbf_job_mut);
		return NULL;
	}

	newnode->pos = pos;
	tbf_job[pos] = newnode;

	pthread_mutex_unlock(&tbf_job_mut);
	return newnode;
}

int tbf_context_delete(MYTBF *ptr)
{
	struct mytbf_st *p=ptr;

	pthread_mutex_lock(&tbf_job_mut);
	tbf_job[p->pos] = NULL;
	pthread_mutex_unlock(&tbf_job_mut);

	free(p);
	return 0;
}

static int
wsize_min(int a, int b)
{
	if (a<0 || b<0) {
		return 0;
	}
	if (a<b) {
		return a;
	}
	return b;
}

int tbf_write(int fd, const char *bufp, size_t size, MYTBF *context)
{
	struct mytbf_st *p=context;
	int asize;
	int ret;

	pthread_mutex_lock(&tbf_job_mut);
	asize = wsize_min(size, p->token);
	pthread_mutex_unlock(&tbf_job_mut);

	ret = write(fd, bufp, asize);

	pthread_mutex_lock(&tbf_job_mut);
	p->token -= ret;
	pthread_mutex_unlock(&tbf_job_mut);

	return ret;
}

