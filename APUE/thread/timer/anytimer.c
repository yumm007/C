#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

#include "link.h"
#include "anytimer.h"

#define JOB_ACTIVE	1
#define AT_INVAL		0
#define BIT_STEP		256

typedef void (*sighandler_t)(int);

struct at_job {
	int job_val;	
	int job_num;
	int job_org_val;
	int job_rep_time;
	int job_sta;
	at_cbfunc *job_fun;
	void *job_arg;
	size_t job_arg_size;
};

static void sub_each(int sec);

void *tim_link = NULL;
static int next_alm = 1;
static char *num_bit = NULL; 
static pthread_mutex_t mut_init = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mut_numb = PTHREAD_MUTEX_INITIALIZER;
static pthread_once_t once_control = PTHREAD_ONCE_INIT;

static void exit_hanlder(void) {
	link_erase(tim_link);
	free(num_bit);
	pthread_mutex_destroy(&mut_init);
	pthread_mutex_destroy(&mut_numb);
}

static int ins_cmp(const void *data1, const void *data2) {
	const struct at_job *job1 = data1;
	const struct at_job *job2 = data2;
	return (job1->job_val - job2->job_val);
}

static int del_cmp(const void *data1, const void *data2) {
	const struct at_job *job1 = data1;
	const struct at_job *job2 = data2;
	return (job1->job_num - job2->job_num);
}

static int emp_fun(void *data) {
	struct at_job *job = data;
	free(job->job_arg);
	return 0;
}

static void *pth_hanlder(void *);
static void init_timer(void) {
	pthread_t	pthd;

	if ((tim_link = link_init(sizeof(struct at_job), ins_cmp, del_cmp, emp_fun)) == NULL)
		return; /* 返回值待细定*/

	pthread_create(&pthd, NULL, pth_hanlder, NULL);
	pthread_detach(pthd);

	atexit(exit_hanlder);
	return;
}

static int op_node(void *job) {
	struct at_job *cur = job;

	cur->job_val -= next_alm;

	if (cur->job_val == 0) 
		(cur->job_fun)(cur->job_arg);
	return cur->job_val;
}

static void sub_each(int sec) {
	struct at_job  *cur = NULL;
	void *save=NULL;

//------------------------------------
	next_alm -= alarm(sec);
	while ((cur = link_each(tim_link, &save))!= NULL) 
		op_node(cur);
	next_alm = sec;
}

static int clean_node(struct at_job *cur) {
	struct at_job tmp;
	
		if (cur->job_rep_time == REP_FOREVER || --cur->job_rep_time > 0) {
			if (cur->job_org_val < next_alm) 
				sub_each(cur->job_org_val);
			memcpy(&tmp, cur, sizeof(*cur));
			tmp.job_val = cur->job_org_val;
			if ((tmp.job_arg = malloc(tmp.job_arg_size)) == NULL)
				return -1; 
			memcpy(tmp.job_arg, cur->job_arg, cur->job_arg_size);
			link_add(&tmp, tim_link);
			link_del(cur, tim_link);
		} else 
			cur->job_sta = AT_INVAL;
	return 0;
}

int at_cancel_job(int job_num) {
	struct at_job *cur = NULL;
	void *save = NULL;
	
	while ((cur = link_each(tim_link, &save))!= NULL)
		if (cur->job_num == job_num) {
			if (cur->job_val == 0 && cur->job_rep_time == 0) 
				return AT_INVAL;
			cur->job_rep_time = 0;
			cur->job_sta = AT_INVAL;
			return AT_OK;
		}

	return AT_ESRCH;
}

int at_get_sta(int job_num) {
	struct at_job *cur = NULL;
	void *save = NULL;
	
	while ((cur = link_each(tim_link, &save))!= NULL)
		if (cur->job_num == job_num) {
			if (cur->job_val == 0 && cur->job_rep_time == 0) {
				return 8;
			}
			return AT_OK;
		}
	return AT_ESRCH;
}

int at_wait_job(int job_num) {
	struct at_job cur;
	int ret;

	cur.job_num = job_num;
	if ((ret = link_del(&cur, tim_link)) == 0)
		num_bit[job_num] = 0;
	return ret;
}

static void *pth_hanlder(void *unused) {
	struct at_job *cur = NULL;
	void *save=NULL;
	pthread_mutex_t mut;
	pthread_cond_t cond;
	struct timespec valu, oldvalu;

	pthread_mutex_init(&mut, NULL);
	pthread_cond_init(&cond, NULL);

	valu.tv_sec = 1;
	valu.tv_nsec = 0;

	while (1) {
		oldvalu = valu;
		while(nanosleep(&oldvalu, &oldvalu) == -1)
				;
		save = NULL;
		while ((cur = link_each(tim_link, &save))!= NULL){
			if (cur->job_sta == AT_INVAL) continue;
			op_node(cur);
		}
		save = NULL;
		while ((cur = link_each(tim_link, &save))!= NULL) {
			if (cur->job_sta == AT_INVAL || cur->job_val > 0) continue;
			clean_node(cur);
		}
#ifdef DEBUG
		save = NULL;
		while ((cur = link_each(tim_link, &save))!= NULL) 
			printf("----\njob%d org_time=%d, time = %d, rep = %d\n-----\n",cur->job_num,cur->job_org_val, cur->job_val, cur->job_rep_time);
#endif
		save = NULL;
		while ((cur = link_each(tim_link,&save))!= NULL) 
			if (cur->job_sta != AT_INVAL) {
				next_alm = cur->job_val;
				valu.tv_sec = next_alm;
				//alarm(next_alm);
				break;
			 }
		//alarm(0);
		if (cur == NULL) break;
	}

	pthread_mutex_destroy(&mut);
	pthread_cond_destroy(&cond);

	return NULL;
}

static int get_ava_num(char **num_bit) {
	void *tmp;
	static size_t i = 0, job_limit = 0;
	int j;

	if (i == job_limit) {
		pthread_mutex_lock(&mut_init);
		if (i != job_limit) 
			pthread_mutex_unlock(&mut_init);
		else {
			job_limit += BIT_STEP;
			if ((tmp = realloc(*num_bit, job_limit)) == NULL)
				return -1;
			*num_bit = tmp;
			for (j = i; j < BIT_STEP; j++)
				(*num_bit)[j] = 0;
			pthread_mutex_unlock(&mut_init);
		}
	}

	for (i = 0; i < job_limit; i++) {
		if ((*num_bit)[i] == 0) {
			pthread_mutex_lock(&mut_numb);
			if ((*num_bit)[i] == 0) {
				(*num_bit)[i] = 1;
				pthread_mutex_unlock(&mut_numb);
				break;
			} else 
				pthread_mutex_unlock(&mut_numb);
		}
	}

	return (i == job_limit ? get_ava_num(num_bit) : i);
}


static int tran_job(struct at_job *job, const job_des *new_job) {
	void *tmp;

	job->job_val = new_job->sec;
	job->job_org_val = job->job_val;
	job->job_sta = JOB_ACTIVE;
	job->job_rep_time = new_job->rep_time;
	job->job_fun = new_job->fun;
	if ((tmp = malloc(new_job->arg_size)) == NULL)
		return -1;
	memcpy(tmp, new_job->arg, new_job->arg_size);
	job->job_arg = tmp;
	job->job_arg_size = new_job->arg_size;
	job->job_num = get_ava_num(&num_bit);

	return 0;
}
/* 这个函数是不会被信号干扰的 */
int at_add_job(const job_des *new_job) {
	int ret;
	static int isfirst = 1;
	struct at_job job;

	if (new_job->rep_time == 0 || new_job->rep_time < REP_FOREVER)
		return -1;
	pthread_once(&once_control, init_timer);

	if (new_job->sec < next_alm) 
		sub_each(new_job->sec);
	if (tran_job(&job, new_job) == -1)
		return -1;
	/*link_add返回任务号*/
	return link_add(&job, tim_link) == -1 ? -1:job.job_num;
}
