#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <syslog.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include "msg.h"
#include "hash_link.h"
#include "pth_queue.h"

#ifndef BUFSIZE
#define BUFSIZE	1024
#endif

#define HASHSIZE	2<<10
#define ERRSIZE	1024

struct client_queue {
	struct sockaddr_in client;
	pthread_t pthd;
	pthread_mutex_t rsd_mut;
	int resend;
//	int stop;
	int time_to_exit;
	PTHQ *queue;
};

struct job_st {
	struct sockaddr_in client;
	struct msg_st msg;
	struct client_queue *par;
};


static int fd_data = -1;
static pthread_t pid_for_req;
static HASHLINK *client_link = NULL;
static PTHQ *que_for_new, *que_for_stp;

static off_t flen(FILE *fp) {
	off_t old, len;

	old = ftello(fp);
	fseeko(fp, 0, SEEK_END);
	len = ftello(fp);
	fseeko(fp, old, SEEK_SET);

	return len;
}

static int cmp(const void *data, const void *key) {
	return memcmp((void *)&((struct job_st *)data)->client, key, sizeof(struct sockaddr_in));
}

enum {STA_OPEN = 1, STA_READ, STA_SEND, STA_FAIL, STA_EOF, STA_ACT, STA_TER};

static void *re_send(void *arg) {
	struct job_st *job = arg;
	struct timeval tim;

	tim.tv_usec = 0;

	while (job->par->time_to_exit > 0) {
		tim.tv_sec = TIME_TO_EXIT - job->par->time_to_exit + 1;
		select(0, NULL, NULL, NULL, &tim);
		pthread_mutex_lock(&job->par->rsd_mut);
		if (job->par->resend) {
			while (sendto(fd_data, &job->msg, sizeof(job->msg), 0, (void *)&job->client, sizeof(job->client)) == -1)
				if (errno != EINTR) {
					syslog(LOG_ERR, "time out but re_send message failed.");
					break;
				}
		}
		job->par->time_to_exit--;
		if (job->par->time_to_exit == 0) {
			syslog(LOG_ERR, "time_to_exit....");
			if (pthq_write(que_for_stp, job, sizeof(*job)) != 0)
				syslog(LOG_ERR, "time_to_exit but close queue failed.");
		}
		pthread_mutex_unlock(&job->par->rsd_mut);
	}
	
	pthread_exit(NULL);
}

static void *handler_client(void *arg) {
	struct job_st job;
	struct msg_st *ret;
	PTHQ *que = arg;
	FILE *fp = NULL;
	int n = -1, stop = 0, test_pth = 1;
	int sta = STA_ACT;
	char errmsg[ERRSIZE];
	off_t len;
	pthread_t pth_resend;
	struct client_queue *par = NULL;
	
	ret = &job.msg;

	while (!stop) {
		switch (sta) {
		case STA_ACT:
			if (pthq_read(que, &job, sizeof(job)) == 0)
				sta = STA_TER;
			else {
				if (par == NULL) {
					par = job.par;
					if (pthread_create(&pth_resend, NULL, re_send, &job) != 0) {
						syslog(LOG_ERR, "create resend pthread for handler_client failed.");
						test_pth = 0;
					}
				}
				job.par = par;
				pthread_mutex_lock(&job.par->rsd_mut);
				job.par->resend = 0;
				pthread_mutex_unlock(&job.par->rsd_mut);
				switch (ntohs(job.msg.mtype)) {
				case REQ:
					sta = STA_OPEN;
					break;
				case REC:
					sta = STA_READ;
					break;
				default:
					sta = STA_TER;
					break;
				}
			}
			break;
		case STA_OPEN:
			if ((fp = fopen((char *)(ret->content), "r")) == NULL) {
				strerror_r(errno, errmsg, ERRSIZE);
				sta = STA_FAIL;
			} else if (ntohl(ret->msg_len) > (len = flen(fp))) {
				strcpy(errmsg, "size of file wrong.");
				sta = STA_FAIL;
			} else if (ntohl(ret->msg_len) == len){
				sta = STA_EOF;
			} else {
				fseeko(fp, ntohl(ret->msg_len), SEEK_SET);	
				sta = STA_READ;
			}
			break;
		case STA_FAIL:
			ret->msg_len = htonl(-1);
			strncpy((char *)ret->content, errmsg, MAXLEN);
			while (sendto(fd_data, ret, sizeof(*ret), 0, (void *)&job.client, sizeof(job.client)) == -1) {
				if (errno != EINTR) break;
			}
			sta = STA_TER;
			break;
		case STA_EOF:
			ret->msg_len = htonl(0);
			strncpy((char *)ret->content, "file transfer completed.", MAXLEN);
			while (sendto(fd_data, ret, sizeof(*ret), 0, (void *)&job.client, sizeof(job.client)) == -1) {
				if (errno != EINTR) break;
			}
			sta = STA_TER;
			break;
		case STA_TER:
			if (fp != NULL) fclose(fp);
			stop = 1;
			break;
		case STA_READ:
			memset(ret->content, 0, MAXLEN);
			n = fread(ret->content, 1, MAXLEN, fp);
			if (n == 0) {
				if (ferror(fp)) {
					strcpy(errmsg, "ferror()");
					sta = STA_FAIL;
				} else
					sta = STA_EOF;
				break;
			}
			ret->msg_len = htonl(n);
			sta = STA_SEND;
			break;
		case STA_SEND:
			while (sendto(fd_data, ret, sizeof(*ret), 0, (void *)&job.client, sizeof(job.client)) == -1) {
				if (errno != EINTR) {
					strerror_r(errno, errmsg, ERRSIZE);
					sta = STA_FAIL;
					break;
				}
			}
			pthread_mutex_lock(&job.par->rsd_mut);
			job.par->resend = 1;
			job.par->time_to_exit = TIME_TO_EXIT;
			pthread_mutex_unlock(&job.par->rsd_mut);
			if (sta != STA_FAIL)
				sta = STA_ACT;
			//fprintf(stderr, "sended, waiting for reback.\n");
			break;
		default:
			syslog(LOG_ERR, "unknown status in handler_client.");
			stop = 1;
			break;
		}	
	}

	if (test_pth) {
		job.par->time_to_exit = 0;
		job.par->resend = 0;
		pthread_cancel(pth_resend);
		pthread_join(pth_resend, NULL);
	}
	pthread_exit(NULL);
}

static int send_rec_to_pth(struct job_st *job) {
	HL_DATA data;
	struct client_queue *cli;

	data.key = &(job->client);
	data.key_size = sizeof(job->client);

	if (link_fnd_node(client_link, &data) != 0) {
		syslog(LOG_ERR, "send_rec(): can't find AAA in client list.");
		return -1;
	}
	cli = data.data;
	if (pthq_write(cli->queue, job, sizeof(*job)) != 0) {
		syslog(LOG_ERR, "can't send rec to queue.");
		return -1;
	}
	return 0;
}

static void *pth_stp(void *arg) {
	struct job_st job;
	PTHQ *que = arg;
	HL_DATA data;
	struct client_queue *cli;

	data.key = &job.client;
	data.key_size = sizeof(job.client);

	while (pthq_read(que, &job, sizeof(job)) > 0) {
		if (link_fnd_node(client_link, &data) != 0) {
			syslog(LOG_ERR, "pth_stp(): can't find AAA in client list.");
			continue;
		}
		cli = data.data;
		pthq_close_wr(cli->queue);
		if (pthread_join(cli->pthd, NULL) != 0)
			syslog(LOG_ERR, "pth_stp(): join pthread failed.");
		pthq_delete(cli->queue);
		pthread_mutex_destroy(&cli->rsd_mut);
		link_del_node(client_link, &data);
	}

	pthread_exit(NULL);
}

static void pth_sleep(long sec, long usec) {
	struct timeval tim;
	tim.tv_sec = sec;
	tim.tv_usec = usec;
	select(0, NULL, NULL, NULL, &tim);
}

static void *pth_new(void *arg) {
	struct job_st job;
	PTHQ *que = arg;
	struct client_queue cli;
	HL_DATA data;
	int failed;

	while (pthq_read(que, &job, sizeof(job)) > 0) {
		/*collect client information for client queue*/
		failed = 0;
		if (!failed)
			if ((cli.queue = pthq_open(BUFSIZE)) == NULL) {
				syslog(LOG_ERR, "create queue for new client failed.");
				failed = 1;
			}
		if (!failed)
			while (pthread_create(&cli.pthd, NULL, handler_client, cli.queue) != 0) {
				if (errno == EAGAIN) {
					strerror_r(errno, (char *)job.msg.content, MAXLEN);
					syslog(LOG_ERR, "create pthread for client fialed, %s", (char *)job.msg.content);
					pth_sleep(0, 200);
					continue;
				}
				
				syslog(LOG_ERR, "create pthread for new client failed. %s", (char *)job.msg.content);
				pthq_delete(cli.queue);		
				failed = 1;
			}
		cli.client = job.client;
		cli.resend = 0;
		cli.time_to_exit = TIME_TO_EXIT;

		if (!failed) {
			/*add them to client queue*/
			data.key = &cli.client;
			data.key_size = sizeof(cli.client);
			data.data = &cli;
			data.data_size = sizeof(cli);

			if (link_add_node(client_link, &data) != 0) {
				syslog(LOG_ERR, "add client information to queue failed.");
				pthq_close_wr(cli.queue);
				pthread_join(cli.pthd, NULL);
				pthq_delete(cli.queue);
				failed = 1;
			}
			link_fnd_node(client_link, &data);
			job.par = data.data; /* save the point*/
			pthread_mutex_init(&job.par->rsd_mut, NULL);
		}
		/*send client request to queue for handler_client*/
		if (!failed)
			if (pthq_write(cli.queue, &job, sizeof(job)) != 0) {
				syslog(LOG_ERR, "add client request to handler fun failed.");
				pthq_close_wr(cli.queue);
				pthread_join(cli.pthd, NULL);
				pthq_delete(cli.queue);
				link_del_node(client_link, &data);
				failed = 1;
			}

		if (failed) {
			job.msg.msg_len = htonl(-1);
			strncpy((char *)job.msg.content, "add client request failed.", MAXLEN);
			while (sendto(fd_data, &job.msg, sizeof(job.msg), 0, (void *)&job.client, sizeof(job.client)) == -1) {
				if (errno != EINTR) { 
					syslog(LOG_ERR, "can't send failed information to client.");
					break;
				}
			}
		}
	}

	pthread_exit(NULL);
}

static pthread_t pth_for_new, pth_for_stp;
static void end_q(void *arg) {
	pthq_close_wr((PTHQ *)arg);
}
static void del_q(void *arg) {
	pthq_delete((PTHQ *)arg);
}
static void joi_p(void *arg) {
	pthread_join(*(pthread_t *)arg, NULL);
}

static void *pth_main(void *unused) {
	struct job_st job;
	socklen_t len;

	if ((que_for_new = pthq_open(BUFSIZE)) == NULL) {
		syslog(LOG_ERR, "create queue for new request failed.");
		pthread_exit(NULL);
	};
	pthread_cleanup_push(del_q, que_for_new);

	if ((que_for_stp = pthq_open(BUFSIZE)) == NULL) {
		syslog(LOG_ERR, "create queue for cancel request failed.");
		pthread_exit(NULL);
	};		
	pthread_cleanup_push(del_q, que_for_stp);

	if (pthread_create(&pth_for_new, NULL, pth_new, que_for_new) != 0) {
		syslog(LOG_ERR, "create pthread for new request failed.");
		pthread_exit(NULL);
	}
	pthread_cleanup_push(end_q, que_for_new);
	pthread_cleanup_push(joi_p, &pth_for_new);

	if (pthread_create(&pth_for_stp, NULL, pth_stp, que_for_stp) != 0) {
		syslog(LOG_ERR, "create pthread for cancel request failed.");
		pthread_exit(NULL);
	}

	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);

	pthread_cleanup_push(joi_p, &pth_for_stp);
	pthread_cleanup_push(joi_p, &pth_for_new);
	pthread_cleanup_push(end_q, que_for_stp);
	pthread_cleanup_push(end_q, que_for_new);

	len = sizeof(job.client);
	while (recvfrom(fd_data, &job.msg, sizeof(job.msg), 0, (void *)&job.client, &len) != -1)
		switch (ntohs(job.msg.mtype)) {
			case REQ:			
				if (pthq_write(que_for_new, &job, sizeof(job)) != 0)
					syslog(LOG_INFO, "write to que_for_new failed.");
				break;
			case STP:
				if (pthq_write(que_for_stp, &job, sizeof(job)) != 0)
					syslog(LOG_INFO, "write to que_for_stp failed.");
				break;
			case REC:
				if (send_rec_to_pth(&job) != 0)
					syslog(LOG_INFO, "get rec answer form unknown AAA.");
				break;
			default: /*unkonw command*/
				syslog(LOG_INFO, "get unknown message form AAA.");
				break;
		}
/* processing by pthread_cleanup_push()

	pthq_close_wr(que_for_new);
	pthq_close_wr(que_for_stp);
	pthread_join(pth_for_new, NULL);
	pthread_join(pth_for_stp, NULL);
	pthq_delete(que_for_new);
	pthq_delete(que_for_stp);
*/
	pthread_exit(NULL);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
}

static void byebye(int unused) {
	syslog(LOG_INFO, "received SIGINT, msg daemon end.");
	close(fd_data);
	pthread_cancel(pid_for_req);
}

static int get_socket(int *fd_datap) {
	struct sockaddr_in myaddr;	

	if ((*fd_datap = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
		return -1;

	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(REQPT);
	inet_pton(AF_INET, CLIENT, &myaddr.sin_addr);
	if (bind(*fd_datap, (void *)&myaddr, sizeof(myaddr)) == -1) {
		close(*fd_datap);
		return -1;
	}
	
	return 0;
}

int main(void)
{
	struct sigaction sig;

	sig.sa_handler = byebye;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;

	openlog("msg deamon", LOG_PID, LOG_DAEMON);
	syslog(LOG_INFO, "msg daemon start.");

	if (get_socket(&fd_data) != 0) {
		syslog(LOG_ERR, "create cocket failed.");
		exit(1);
	}

	if (sigaction(SIGINT, &sig, NULL) != 0) {
		syslog(LOG_WARNING, "install SIGINT failed.");
	} else
		syslog(LOG_INFO, "install SIGINT OK.");

	if ((client_link = link_init(HASHSIZE, cmp)) == NULL) {
		syslog(LOG_ERR, "create client link failed.");
		close(fd_data);
		exit(1);
	}

	pthread_create(&pid_for_req, NULL, pth_main, NULL);
	pthread_join(pid_for_req, NULL);

	syslog(LOG_INFO, "msg daemon end.");
	link_dstr(client_link);
	closelog();
	/*fd_data closed by SIGINT*/
	//close(fd_data);

	return 0;
}
