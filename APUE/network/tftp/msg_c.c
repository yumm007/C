#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "msg.h"

enum {sta_chk = 1, sta_err, sta_ter, sta_rd, sta_rec, sta_wr, sta_eof};
static int fd_sock = -1;
static int stop = 0;
static FILE *msg_fp = NULL;
static pthread_mutex_t rec_mut = PTHREAD_MUTEX_INITIALIZER;
static int resend = 0;
static int time_to_exit = TIME_TO_EXIT;

static int send_msg(int fd_req, struct msg_st *msg) {
	struct sockaddr_in server_addr;	

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(REQPT);
	inet_pton(AF_INET, SERVER, &server_addr.sin_addr);

	while (sendto(fd_req, msg, sizeof(*msg), 0, (void *)&server_addr, sizeof(server_addr)) == -1) {
		if (errno != EINTR)
			return -1;
	}
	return 0;
}

static FILE *open_check(int fd_sock, const char *filename, const char *newfile, int wrflag, struct msg_st *msg) {
	FILE *fp;

	if (wrflag == 'w') {
		fp = fopen(newfile, "w");
		msg->msg_len = 0;
	} else if (wrflag == 'a') {
		fp = fopen(newfile, "a");
		fseeko(fp, 0, SEEK_END);
		msg->msg_len = ftello(fp);
		fseeko(fp, 0, SEEK_SET);
	} else 
		return NULL;

	if (fp == NULL)
		return NULL;

	msg->mtype = htons(REQ);
	memset((char *)(msg->content), 0, MAXLEN);	
	strncpy((char *)(msg->content), filename, MAXLEN);

	if (send_msg(fd_sock, msg) != 0) {
		fclose(fp);
		return NULL;
	}	

	return fp;
}

static int write_data(const struct msg_st *msg, FILE *fp) {
	int ret;
	
	ret = fwrite(msg->content, 1, ntohl(msg->msg_len), fp);
	if (ret != ntohl(msg->msg_len)) {
		return ferror(fp);
	}
	return 0;
}

static void *re_send(void *arg) {
	struct msg_st *msg = arg;
	struct timeval tim;

	tim.tv_usec = 0;

	while (time_to_exit > 0) {
		tim.tv_sec = TIME_TO_EXIT - time_to_exit + 1;
		select(0, NULL, NULL, NULL, &tim);
		pthread_mutex_lock(&rec_mut);
		if (resend) {
			send_msg(fd_sock, msg);
			fprintf(stderr, "time out, resend.\n");
		}
		time_to_exit--;
		if (time_to_exit == 0) {
			fprintf(stderr, "time_out, exit....\n");
			stop = 1;
			kill(getpid(), SIGINT);
		}
		pthread_mutex_unlock(&rec_mut);
	}
	
	pthread_exit(NULL);
}

static void rec_file(int fd_sock, const char *filename, const char *newfile, int wrflag) {
	int status = sta_chk, ret;
	int started = 0;
	struct msg_st msg;
	pthread_t pth_timer;

	pthread_create(&pth_timer, NULL, re_send, &msg);
	pthread_detach(pth_timer);

	while (!stop) {
		started = 1;
		switch (status) {
		case sta_chk:
			msg_fp = open_check(fd_sock, filename, newfile, wrflag, &msg);
			status = (msg_fp == NULL ? sta_err : sta_rd);
			break;
		case sta_err:
			fprintf(stderr,"check file %s failed: %s\n",filename, msg.content);
			status = sta_ter;
			break;
		case sta_ter:
			stop = 1;
			break;
		case sta_rd:
			if (recvfrom(fd_sock, &msg,sizeof(msg), 0, NULL, NULL) == -1) {
				status = (errno == EINTR ? sta_rd : sta_ter);
				break;
			}
			pthread_mutex_lock(&rec_mut);
			resend = 0;
			pthread_mutex_unlock(&rec_mut);
			if ((int)ntohl(msg.msg_len) < 0) {
				status = sta_err;
			} else 
				status = (ntohl(msg.msg_len) == 0 ? sta_ter: sta_wr);
			break;
		case sta_wr:
			if ((ret = write_data(&msg, msg_fp)) == 0) {
				status = sta_rec;
			} else {
				fprintf(stderr, "fwrite to %s failed, error code: %d\n ", newfile, ret);
				status = sta_ter;
			}
			break;
		case sta_rec:
			msg.mtype = htons(REC);
			memset((char *)(msg.content), 0, MAXLEN);
			send_msg(fd_sock, &msg);
			pthread_mutex_lock(&rec_mut);
			resend = 1;
			time_to_exit = TIME_TO_EXIT;
			pthread_mutex_unlock(&rec_mut);
			status = sta_rd;
			break;
		default:
			status = sta_ter;
			break;
		}
	}

	if (msg_fp != NULL) fclose(msg_fp);
	if (started) {
		msg.mtype = htons(STP);
		memset(msg.content, 0, MAXLEN);
		send_msg(fd_sock, &msg);
	}

	return;
}

static void sig_han(int unused) {
	stop = 1;
}

static void install_sig(void) {
	struct sigaction sig;

	sig.sa_handler = sig_han;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	
	sigaction(SIGINT, &sig, NULL);

	return;
}

int main(int argc, char *argv[])
{

	if (argc < 3) {
		fprintf(stderr, "usage %s: file path.\n", argv[0]);
		exit(1);
	}

	install_sig();

	if ((fd_sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
		fprintf(stderr, "create socket to %s failed.\n", SERVER);
		exit(1);
	}

	rec_file(fd_sock, argv[1], argv[2], 'w');

	close(fd_sock);

	return 0;
}
