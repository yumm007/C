#ifndef	REL_PUPLIC_H
#define	REL_PUPLIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <poll.h>

#include "relayer.h"
#include "pth_queue.h"

#define 	NODE_FREE 	0
#define	NODE_USED	1
#define	NODE_WAIT	2
#define	NODE_INIT	3

#define	BUFSIZE		1024
#define	SUB_RELAYER	4

enum {STA_RD = 0, STA_WR, STA_EOF, STA_EXC, STA_TER};

struct databuf {
	int  status;
	char buf[BUFSIZE];
	ssize_t readc, writec;
};

struct relayer_job_st {
	int node_status;
   struct relayer_stat_st stat;
   int fd1_saveflg, fd2_saveflg;
	struct databuf buf12, buf21;
};

extern struct relayer_job_st relayer_arr[RELAYER_JOBMAX];
extern struct pollfd poll_arr[RELAYER_JOBMAX * 2];
extern pthread_mutex_t arr_mut;
extern pthread_cond_t wait_cond;
extern pthread_mutex_t poll_mut;
extern pthread_cond_t poll_cond;
extern pthread_once_t once_control;
extern PTHQ *pthq;
extern pthread_t relayer_pthd;
extern int job_finish;
extern int init_ret;

void relayer_init(void);
void *pth_sub_rel(void *arg);
void *pth_relayer(void *arg);

#endif
