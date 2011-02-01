#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <errno.h>

#include "msg.h"

enum {sta_chk = 1, sta_err, sta_ter, sta_rd, sta_wr, sta_eof};
static int stop = 0;
static FILE *msg_fp = NULL;

static FILE *open_check(int msgid, int msglisten, const char *filename, const char *newfile, int wrflag, struct msg_st *msg) {
	pid_t pid;
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

	msg->mtype = REQ;	
	pid = getpid();
	msg->msg_identify = pid;
	strncpy(msg->content, filename, MAXLEN);

	if (msgsnd(msglisten, msg, sizeof(*msg) - sizeof(long), 0) < 0) {
		fclose(fp);
		return NULL;
	}

	while (msgrcv(msgid, msg, sizeof(*msg) - sizeof(long), (long)pid, 0) < 0) {
		if (errno != EINTR) {
			fclose(fp);
			return NULL;
		}
	}

	if (msg->msg_len < 0){
		fclose(fp);
		return NULL;
	}

	return fp;
}

static int write_data(const struct msg_st *msg, FILE *fp) {
	int ret;
	
	ret = fwrite(msg->content, 1, msg->msg_len, fp);
	if (ret != msg->msg_len) {
		return ferror(fp);
	}
	return 0;
}

static void rec_file(int msgid, int msglisten, const char *filename, const char *newfile, int wrflag) {
	int status = sta_chk, ret;
	struct msg_st msg;
	pid_t pid;

	pid = getpid();
	msg.msg_pthd = (pthread_t)0; /*避免信号来时，误传给server一个不存在的线程号*/

	while (!stop)
		switch (status) {
		case sta_chk:
			if ((msg_fp = open_check(msgid, msglisten, filename, newfile, wrflag, &msg)) == NULL) {
				status = sta_err;
			} else {
				status = sta_rd;
			}
			break;
		case sta_err:
			fprintf(stderr, "check file %s failed: %s\n", filename, msg.content);
			status = sta_ter;
			break;
		case sta_ter:
			stop = 1;
			break;
		case sta_rd:
			if (msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), pid, 0) < 0){
				status = sta_err;
			} else if (msg.msg_len == 0) {
				status = sta_ter;
			} else {
				status = sta_wr;
			}
			break;
		case sta_wr:
			if ((ret = write_data(&msg, msg_fp)) == 0) {
				status = sta_rd;
			} else {
				fprintf(stderr, "fwrite to %s failed, error code: %d\n ", newfile, ret);
				status = sta_ter;
			}
			break;
		default:
			status = sta_ter;
			break;
	}

	if (msg_fp != NULL) fclose(msg_fp);
	if (msg.msg_pthd != (pthread_t)0) {
		msg.mtype = STP;
		msg.msg_identify = pid;
		while (msgsnd(msglisten, &msg, sizeof(msg) - sizeof(long), 0) < 0)
			if (errno != EINTR) break;
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
	key_t key;
	int msgid, msglisten;


	if (argc < 3) {
		fprintf(stderr, "usage %s: file path.\n", argv[0]);
		exit(1);
	}

	install_sig();

	if ((key = ftok(KEYPATH, KEYPROJ)) < 0) {
		perror("ftok()");
		exit(1);
	}

	if ((msgid = msgget(key, 0)) < 0) {
		perror("msgget()");
		exit(1);
	}

	if ((key = ftok(KEYPATH, KEYREQ)) < 0) {
		perror("ftok()");
      exit(1);
   }   

   if ((msglisten = msgget(key, IPC_CREAT|0600)) < 0) {
		perror("msgget()");
      exit(1);
   }  
	
	rec_file(msgid, msglisten, argv[1], argv[2], 'w');

	return 0;
}
