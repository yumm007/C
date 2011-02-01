#include "rel_puplic.h"

#include <sys/resource.h>

int init_ret = 0;
int job_finish = 0;

struct relayer_job_st relayer_arr[RELAYER_JOBMAX];
struct pollfd poll_arr[RELAYER_JOBMAX * 2];

pthread_mutex_t arr_mut;
pthread_cond_t wait_cond;
pthread_mutex_t poll_mut;
pthread_cond_t poll_cond;

pthread_once_t once_control = PTHREAD_ONCE_INIT;
PTHQ *pthq;
pthread_t relayer_pthd;

static pthread_t pthd[SUB_RELAYER];
static struct rlimit savelimit;
static struct sigaction old_sig;

static void restore_fd(void *arg) {
	int i;
	struct relayer_job_st *job_arr = arg;

	for (i = 0; i < RELAYER_JOBMAX; i++)
		if (job_arr[i].node_status != NODE_FREE) {
			fcntl(job_arr[i].stat.fd1, F_SETFL, job_arr[i].fd1_saveflg);
			fcntl(job_arr[i].stat.fd2, F_SETFL, job_arr[i].fd2_saveflg);
		}
}

static void sig_poll(int unused) {
	return;
}

void relayer_init(void) {
	int i, ret;
	struct rlimit tmplimit;
	struct sigaction sig;

	if ((pthq = pthq_open(RELAYER_JOBMAX / SUB_RELAYER)) == NULL) {
		init_ret = E_REL_INIT;
		return;
	}

	for (i = 0; i < SUB_RELAYER; i++) 
		if (pthread_create(pthd + i, NULL, pth_sub_rel, pthq) != 0)
			i--;

	pthread_mutex_init(&arr_mut, NULL);
	pthread_cond_init(&wait_cond, NULL);
	pthread_mutex_init(&poll_mut, NULL);
	pthread_cond_init(&poll_cond, NULL);

	sig.sa_handler = sig_poll;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	sigaction(SIGPOLL,&sig, &old_sig);

	getrlimit(RLIMIT_NOFILE, &savelimit);
	tmplimit = savelimit;
	tmplimit.rlim_max += RELAYER_JOBMAX * 2;
	tmplimit.rlim_cur += RELAYER_JOBMAX * 2;

	if((ret = setrlimit(RLIMIT_NOFILE, &tmplimit)) != 0) {
		init_ret = errno;
		return;
	}
	
	for (i = 0; i < RELAYER_JOBMAX; i++) {
		relayer_arr[i].node_status = NODE_FREE;

		poll_arr[i * 2].fd = 0;
		poll_arr[i * 2].events = 0;
		poll_arr[i * 2 + 1].fd = 0;
		poll_arr[i * 2 + 1].events = 0;
	}

	ret = pthread_create(&relayer_pthd, NULL, pth_relayer, (void *)relayer_arr);
	if (ret != 0) {
		init_ret = errno;
		setrlimit(RLIMIT_NOFILE, &savelimit);
		return;
	}

	return;
}

static void pth_rel_exit(void *arg) {
	restore_fd(arg);
	sigaction(SIGPOLL, &old_sig, NULL);

	pthread_mutex_destroy(&arr_mut);
	pthread_cond_destroy(&wait_cond);
	pthread_mutex_destroy(&poll_mut);
	pthread_cond_destroy(&poll_cond);

	setrlimit(RLIMIT_NOFILE, &savelimit);
}

void relayer_finished(void) {
	int i;

	job_finish = 1;
	pthread_kill(relayer_pthd, SIGPOLL);

	pthq_close_wr(pthq);
	for (i = 0; i < SUB_RELAYER; i++) 
		pthread_join(pthd[i], NULL);
	pthread_join(relayer_pthd, NULL);

	pthq_delete(pthq);
	pth_rel_exit(relayer_arr);
}

