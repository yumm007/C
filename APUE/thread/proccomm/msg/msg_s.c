#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <syslog.h>

#include "msg.h"

#ifndef BUFSIZE
#define BUFSIZE	1024
#endif

static int msgid = 0, msglisten = 0;
static pthread_t pid_for_req, pid_for_stp;

static off_t flen(FILE *fp) {
	off_t old, len;

	old = ftello(fp);
	fseeko(fp, 0, SEEK_END);
	len = ftello(fp);
	fseeko(fp, old, SEEK_SET);

	return len;
}

static void free_fp(void *arg) {
	if (arg != NULL)
		fclose((FILE *)arg);
}

static void free_arg(void *arg) {
	free(arg);
}

static void *send_file(void *arg) {
	FILE *fp = NULL;
	struct msg_st *ret = arg;
	int n = 0;

	pthread_cleanup_push(free_arg, arg);

	ret->msg_pthd = pthread_self();

	if ((fp = fopen(ret->content, "r")) == NULL || ret->msg_len > flen(fp)) {
		ret->mtype = ret->msg_identify;
		ret->msg_len = -1;
		strerror_r(errno, ret->content, MAXLEN);
		msgsnd(msgid, ret, sizeof(*ret) - sizeof(long), 0);
		if (fp != NULL)
			fclose(fp);
		pthread_exit(NULL);
	}

	pthread_cleanup_push(free_fp, (void *)fp);

	fseeko(fp, ret->msg_len, SEEK_SET);	

	ret->mtype = ret->msg_identify;
	ret->msg_len = 1;
	msgsnd(msgid, ret, sizeof(*ret) - sizeof(long), 0);
	
	while (ret->msg_len != 0) {
		ret->mtype = ret->msg_identify;
		n = fread(ret->content, 1, MAXLEN, fp);
		ret->msg_len = n;
		msgsnd(msgid, ret, sizeof(*ret) - sizeof(long), 0);
	}

	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_exit(NULL);
}

static int get_newjob(struct msg_st *msg) {
	struct msg_st *ret;
	pthread_t pthd;
	char buf[BUFSIZE];

	if ((ret = malloc(sizeof(*ret))) == NULL) {
		strerror_r(errno, buf, BUFSIZE);
		syslog(LOG_ERR, "%s", buf);
		return -1;
	}
	memcpy(ret, msg, sizeof(*ret)); /*need free by pthread*/
	if (pthread_create(&pthd, NULL, send_file, ret) != 0) {
		strerror_r(errno, buf, BUFSIZE);
		syslog(LOG_ERR, "%s", buf);
		free(ret);
		return -2;
	}
	
	return 0;
}

static void *pth_req(void *unused) {
	struct msg_st msg;

	while (msgrcv(msglisten, &msg, sizeof(msg) - sizeof(long), REQ, 0) != -1) {
		//fprintf(stderr, "received job from PID %u, %s\n", msg.msg_identify, msg.content);
		if (get_newjob(&msg) != 0) {
			syslog(LOG_INFO, "msg unnormally ended.");
			break;
		}
	}
	pthread_exit(NULL);
}

static void *clean_queue(void *arg) {
	long mtype = (unsigned)arg;
	struct msg_st msg;

	//printf("clean msg for %ld.\n", mtype);
	while (msgrcv(msgid, &msg, sizeof(struct msg_st) - sizeof(long), mtype, IPC_NOWAIT) != -1)
		;//printf("clean...\n");
	pthread_exit(NULL);
}

static void *pth_stp(void *unused) {
	struct msg_st msg;
	pthread_t pthd;

	while (msgrcv(msglisten, &msg, sizeof(msg) - sizeof(long), STP, 0) != -1) {
		//printf("received cancel job for PID: %u.\n", msg.msg_identify);
		pthread_cancel(msg.msg_pthd);
		pthread_join(msg.msg_pthd, NULL);
		while (pthread_create(&pthd, NULL, clean_queue, (void *)(msg.msg_identify)) != 0)
			;
		pthread_detach(pthd);
	}

	pthread_exit(NULL);
}

static void byebye(int unused) {
//	struct msg_st msg;
//	while (msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 0, IPC_NOWAIT) != -1) {
//		fprintf(stderr, "received job from PID %u, %lu\n", msg.msg_identify, msg.msg_len);
//	}
//
	msgctl(msgid, IPC_RMID, 0);
	msgctl(msglisten, IPC_RMID, 0);

	pthread_join(pid_for_req, NULL);
	pthread_join(pid_for_stp, NULL);

	syslog(LOG_INFO, "received SIGINT, msg daemon end.");
	closelog();
	exit(0);
}

int main(void)
{
	key_t key;
	struct sigaction sig;
	char buf[BUFSIZE];

	sig.sa_handler = byebye;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;

	openlog("msg deamon", LOG_PID, LOG_DAEMON);
	syslog(LOG_INFO, "msg daemon start.");

	if (sigaction(SIGINT, &sig, NULL) != 0) {
		syslog(LOG_WARNING, "install SIGINT failed.");
	} else
		syslog(LOG_INFO, "install SIGINT OK.");

	if ((key = ftok(KEYPATH, KEYPROJ)) < 0) {
		strerror_r(errno, buf, BUFSIZE);
		syslog(LOG_ERR, "%s", buf);
		exit(1);
	}

	if ((msgid = msgget(key, IPC_CREAT|0600)) < 0) {
		strerror_r(errno, buf, BUFSIZE);
		syslog(LOG_ERR, "%s", buf);
		exit(1);
	}

	if ((key = ftok(KEYPATH, KEYREQ)) < 0) {
		strerror_r(errno, buf, BUFSIZE);
		syslog(LOG_ERR, "%s", buf);
		exit(1);
	}

	if ((msglisten = msgget(key, IPC_CREAT|0600)) < 0) {
		strerror_r(errno, buf, BUFSIZE);
		syslog(LOG_ERR, "%s", buf);
		msgctl(msgid, IPC_RMID, 0);
		exit(1);
	}
	
	pthread_create(&pid_for_req, NULL, pth_req, NULL);
	pthread_create(&pid_for_stp, NULL, pth_stp, NULL);
	pthread_join(pid_for_req, NULL);
	pthread_join(pid_for_stp, NULL);

	syslog(LOG_INFO, "msg daemon end.");
	closelog();
	return 0;
}
