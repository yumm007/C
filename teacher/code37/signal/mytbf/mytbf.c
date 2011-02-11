#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "mytbf.h"

struct mytbf_st {
	int cps;
	int burst;
	sig_atomic_t token;
	int pos;
};

static struct mytbf_st *tbf_job[TBF_MAX];

static int
find_free_pos(void)
{
	int i;
	for (i=0;i<TBF_MAX;++i) {
		if (tbf_job[i]==NULL) {
			return i;
		}
	}
	return -1;
}

static void
alrm_handler(int s)
{
	int i;

	for (i=0;i<TBF_MAX;++i) {
		if (tbf_job[i]!=NULL) {
			tbf_job[i]->token +=  tbf_job[i]->cps;
			if (tbf_job[i]->token > tbf_job[i]->burst) {
				tbf_job[i]->token = tbf_job[i]->burst;
			}
		}
	}
}

static void
global_destruct(void)
{
	int i;
	struct itimerval itv;

	itv.it_interval.tv_sec = 0;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 0;
	itv.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &itv, NULL);

	signal(SIGALRM, SIG_DFL);
	for (i=0;i<TBF_MAX;++i) {
		free(tbf_job[i]);
	}
}

static int tbf_inited=0;
static void
global_contruct(void)
{
	int i;
	struct itimerval itv;

	for (i=0;i<TBF_MAX;++i) {
		tbf_job[i] = NULL;
	}
	signal(SIGALRM, alrm_handler);

	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 1;
	itv.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &itv, NULL);

	atexit(global_destruct);
}

MYTBF *tbf_context_new(int cps, int burst)
{
	struct mytbf_st *newnode;
	int pos;

	if (!tbf_inited) {
		global_contruct();
		tbf_inited=1;
	}

	pos = find_free_pos();
	if (pos<0) {
		return NULL;
	}

	newnode = malloc(sizeof(struct mytbf_st));
	if (newnode==NULL) {
		return NULL;
	}
	newnode->token = 0;
	newnode->cps = cps;
	newnode->burst = burst;
	newnode->pos = pos;

	tbf_job[pos] = newnode;
	return newnode;
}

int tbf_context_delete(MYTBF *ptr)
{
	struct mytbf_st *p=ptr;
	tbf_job[p->pos] = NULL;
	free(p);
	return 0;
}

static int
min(int a, int b)
{
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

	asize = min(size, p->token);
	ret = write(fd, bufp, asize);
	p->token -= ret;
	return ret;
}

