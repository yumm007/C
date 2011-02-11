#ifndef ANYTIMER_H
#define ANYTIMER_H

#define AT_MAX 1024

typedef void at_cbfunc(void*);

int at_add_job(struct timeval tv, at_cbfunc *funcp, void *arg);


/****************** Additional *****************/

int at_add_repeatjob(struct timeval tv, at_cbfunc *funcp, void *arg);
int at_cancel_job(int);
int at_wait_job(int);

#endif

