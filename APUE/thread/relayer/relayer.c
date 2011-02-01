#include "rel_puplic.h"

#include <time.h>
#include <sys/time.h>

static int poll_count = 0;

static ssize_t read_write(int fd_read,int fd_write, struct databuf *buf) {
	ssize_t n, ret = 0;
	
	switch (buf->status) {
		case STA_RD:
			n = read(fd_read, buf->buf, BUFSIZE);
			if (n > 0) {
				buf->readc = n;
				buf->writec = 0;
				buf->status = STA_WR;
			} else if (n == 0) 
				buf->status = STA_TER;
			else if (n < 0 && errno == EAGAIN)
				buf->status = STA_RD; /* 下次接着读*/
			else
				buf->status = STA_TER;
			break;
		case STA_WR:
			ret = n = write(fd_write,buf->buf+buf->writec,buf->readc-buf->writec);
			if (n == buf->readc)
				buf->status = STA_RD;
			else if (n > 0) {
				buf->writec += n;
				buf->status = STA_WR; /* 下回接着从偏移量写*/
			} else if (n < 1 && errno == EAGAIN)
				buf->status = STA_WR; /* 下回接着从偏移量写*/
			else
				buf->status = STA_TER;
			break;
		case STA_EOF:
			buf->status = STA_TER;
			break;
		case STA_EXC:
			buf->status = STA_TER;
			break;
		case STA_TER:
			break;
		default:
			abort();
			break;
	}

	return ret > 0 ? ret : 0;
}

static void relay(struct relayer_job_st *job) {
	ssize_t ret;
	/*读左写右*/
	if (job->buf12.status != STA_TER) {
		ret = read_write(job->stat.fd1, job->stat.fd2, &job->buf12);
		job->stat.count12 += ret;
	}
	/*读右写左*/
	if (job->buf21.status != STA_TER) {
		ret = read_write(job->stat.fd2, job->stat.fd1, &job->buf21);
		job->stat.count21 += ret;
	}
}


static int status_changed(struct pollfd *poll_fd, int *status) {
	int ret = 0;

	if (poll_fd->revents & POLLIN) {
		*status = STA_RD;
		ret = 1;
	}
	if (poll_fd->revents & POLLOUT) {
		*status = STA_WR;
		ret = 1;
	}
	if (poll_fd->revents & POLLERR) {
		*status = STA_EXC;
		ret = 1;
	}
	if (poll_fd->revents & POLLHUP) {
		*status = STA_EOF;
		ret = 1;
	}

	return ret;
}

static void set_next_poll(struct relayer_job_st *job, int i) {

	if (job->buf12.status == STA_RD)
		poll_arr[i * 2].events = POLLIN;
	else if (job->buf12.status == STA_WR)
		poll_arr[i * 2].events = POLLOUT;
	else 
		poll_arr[i * 2].events = 0;

	if (job->buf21.status == STA_RD)
		poll_arr[i * 2 + 1].events = POLLIN;
	else if (job->buf21.status == STA_WR)
		poll_arr[i * 2 + 1].events = POLLOUT;
	else
		poll_arr[i * 2 + 1].events = 0;
	
	/*读写两端都结束，置为WAIT状态,并设OVER*/
	if (job->buf12.status == STA_TER && job->buf21.status == STA_TER) {
		job->stat.over = 1;

		pthread_mutex_lock(&arr_mut);
		job->node_status = NODE_WAIT;
		pthread_cond_broadcast(&wait_cond);
		pthread_mutex_unlock(&arr_mut);
	}
	
	return;
}

void *pth_sub_rel(void *arg) {
	int i;
	struct relayer_job_st *job_arr = relayer_arr;
	PTHQ *pthq = arg;

	while (pthq_read(pthq, &i, sizeof(i)) != EOF) {
		relay(job_arr + i);
		set_next_poll(job_arr + i, i);

		pthread_mutex_lock(&poll_mut);
		poll_count--;
		pthread_cond_broadcast(&poll_cond);
		pthread_mutex_unlock(&poll_mut);
	}

	pthread_exit(NULL);
}


void *pth_relayer(void *arg) {
	struct relayer_job_st *job_arr = arg;
	int i;
	struct pollfd *j;
	
	while (!job_finish) {
		if (poll(poll_arr, RELAYER_JOBMAX * 2, -1) == -1) {
			if (errno == EINTR)
				continue;
			else 
				break;
		}
		for (i = 0; i < RELAYER_JOBMAX; i++) {
			if (job_arr[i].node_status == NODE_USED) {
				j = poll_arr + i * 2;
				if (status_changed(j, &job_arr[i].buf12.status) || 
					status_changed(j + 1, &job_arr[i].buf21.status)) {
					pthread_mutex_lock(&poll_mut);
					poll_count++;
					pthread_mutex_unlock(&poll_mut);
					pthq_write(pthq, &i, sizeof(i));
				}
			}
		}
		/*定义个栅栏,任务都完成后再继续下一次POLL*/
		pthread_mutex_lock(&poll_mut);
		while (poll_count != 0)
			pthread_cond_wait(&poll_cond, &poll_mut);
		pthread_mutex_unlock(&poll_mut);
	}
		
	pthread_exit(NULL);
}


static int get_free_node(void) {
	int i;
	
	pthread_mutex_lock(&arr_mut);
	for (i = 0; i < RELAYER_JOBMAX; i++)
		if (relayer_arr[i].node_status == NODE_FREE)
			break;
	if (i != RELAYER_JOBMAX)
		relayer_arr[i].node_status = NODE_INIT;
	pthread_mutex_unlock(&arr_mut);
	
	return i == RELAYER_JOBMAX ? -1: i;
}

static void add_job_to_arr(int fd1, int fd2, int i) {

	relayer_arr[i].stat.fd1 = fd1;
	relayer_arr[i].stat.fd2 = fd2;
	relayer_arr[i].stat.count12 = 0;	
	relayer_arr[i].stat.count21 = 0;
	relayer_arr[i].stat.start = time(NULL);
	relayer_arr[i].stat.over = 0;
	relayer_arr[i].fd1_saveflg = fcntl(fd1, F_GETFL);
	relayer_arr[i].fd2_saveflg = fcntl(fd2, F_GETFL);
	relayer_arr[i].buf12.status = STA_RD;
	relayer_arr[i].buf21.status = STA_RD; 
	relayer_arr[i].buf12.readc = 0;
	relayer_arr[i].buf12.writec = 0;
	relayer_arr[i].buf21.readc = 0;
	relayer_arr[i].buf21.writec = 0;

	fcntl(fd1, F_SETFL, relayer_arr[i].fd1_saveflg | O_NONBLOCK);
	fcntl(fd2, F_SETFL, relayer_arr[i].fd2_saveflg | O_NONBLOCK);

	poll_arr[i * 2].fd = fd1;
	poll_arr[i * 2].events = POLLIN;
	poll_arr[i * 2 + 1].fd = fd2;
	poll_arr[i * 2 + 1].events = POLLIN;

	/*在不改成NODE_USED之前，所有的轮询都会忽略它*/
	pthread_mutex_lock(&arr_mut);
	relayer_arr[i].node_status = NODE_USED;
	pthread_mutex_unlock(&arr_mut);

	return;
}

int relayer_addjob(int fd1, int fd2, int flags) {
	int ret;

	pthread_once(&once_control, relayer_init);
	if (init_ret != 0)
		return E_REL_INIT;
	
	if ((ret = get_free_node()) == -1)
		return E_REL_FULL;

	add_job_to_arr(fd1, fd2, ret);
	pthread_kill(relayer_pthd, SIGPOLL);

	return ret;
}

int relayer_statjob(int jobnum, struct relayer_stat_st* st) {
	if (jobnum < 0 || jobnum >= RELAYER_JOBMAX)
		return E_REL_NF;

	pthread_mutex_lock(&arr_mut);
	if (relayer_arr[jobnum].node_status == NODE_FREE \
		|| relayer_arr[jobnum].node_status == NODE_INIT) {
		pthread_mutex_unlock(&arr_mut);
		return E_REL_NU;
	}
	memcpy(st, &relayer_arr[jobnum].stat, sizeof(*st));
	pthread_mutex_unlock(&arr_mut);

	return 0;
}

int relayer_canceljob(int jobnum) {
	if (jobnum < 0 || jobnum >= RELAYER_JOBMAX)
		return E_REL_NF;

	pthread_mutex_lock(&arr_mut);
	if (relayer_arr[jobnum].node_status == NODE_FREE \
		|| relayer_arr[jobnum].node_status == NODE_INIT) {
		pthread_mutex_unlock(&arr_mut);
		return E_REL_NU;
	}
	relayer_arr[jobnum].node_status = NODE_WAIT;
	pthread_cond_broadcast(&wait_cond);
	pthread_mutex_unlock(&arr_mut);

	return 0;
}

int relayer_waitjob(int jobnum) {
	if (jobnum < 0 || jobnum >= RELAYER_JOBMAX)
		return E_REL_NF;

	pthread_mutex_lock(&arr_mut);
	while (relayer_arr[jobnum].node_status == NODE_USED)
		pthread_cond_wait(&wait_cond, &arr_mut);
	if (relayer_arr[jobnum].node_status == NODE_FREE \
		|| relayer_arr[jobnum].node_status == NODE_INIT) {
		pthread_mutex_unlock(&arr_mut);
		return E_REL_NU;
	}

	fcntl(relayer_arr[jobnum].stat.fd1, F_SETFL, relayer_arr[jobnum].fd1_saveflg);
	fcntl(relayer_arr[jobnum].stat.fd2, F_SETFL, relayer_arr[jobnum].fd2_saveflg);

	relayer_arr[jobnum].node_status = NODE_FREE;
	pthread_mutex_unlock(&arr_mut);

	return 0;
}
