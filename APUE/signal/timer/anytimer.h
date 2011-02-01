#ifndef ANYTIMER_H
#define ANYTIMER_H

#include <errno.h>
#include "link.h"

#define REP_FOREVER	-1

typedef void at_cbfunc(void*);
typedef struct {
	int sec;
	int rep_time;
	at_cbfunc *fun;
	void *arg;
	size_t arg_size;
} job_des;

int at_get_sta(int job_num);
int at_add_job(const job_des *job);
int at_cancel_job(int); /* 返回值如下*/
enum {
	AT_OK=0,	
	AT_ESRCH=ESRCH,	/* 没有这个任务 */
	AT_INVAL=EINVAL,/* 任务已完成 */
};
int at_wait_job(int); /*释放资源*/

#endif

