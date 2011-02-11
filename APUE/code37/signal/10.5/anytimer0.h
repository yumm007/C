#ifndef ANYTIMER_H
#define ANYTIMER_H

#include <errno.h>

#define AT_MAX 1024

typedef void at_cbfunc(void*);

int at_add_job(int sec, at_cbfunc *funcp, void *arg);
int at_cancel_job(int);
enum {
	AT_OK=0,	
	AT_ESRCH=ESRCH,	/* 没有这个任务 */
	AT_INVAL=EINVAL,/* 任务已完成 */
};
int at_wait_job(int);

/****************** Additional *****************/

int at_add_repeatjob(int sec, at_cbfunc *funcp, void *arg);

#endif

