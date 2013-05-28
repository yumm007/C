#include <stdio.h>
#include "linux_header.h"
#include "htp.h"
#include <signal.h>
#include <unistd.h>

static void sig_alarm(int signo) {
	//alarm(1);
}

static int reg_sig(void) {
	struct sigaction old_act;
	sigset_t old_mask, blc_alr_mask;

   struct sigaction new_act;
   sigset_t new_mask;

  /* 确保alarm信号没有被屏蔽 */
   sigemptyset(&new_mask);
   sigaddset(&new_mask, SIGALRM);
   sigprocmask(SIG_UNBLOCK, &new_mask, &old_mask);

   sigemptyset(&blc_alr_mask);
   sigaddset(&blc_alr_mask, SIGALRM);

   /* 安装alarm信号接受函数*/
   new_act.sa_handler = sig_alarm;
   sigemptyset(&new_act.sa_mask);
   new_act.sa_flags = 0;
   if (sigaction(SIGALRM, &new_act, &old_act) == -1) 
      return -2; 

	return 0;
}

enum {HTP_OPEN, HTP_RECV, HTP_SEND, HTP_CLOSE};

int main(void) {
	htp_header_t h;
	int status = HTP_OPEN;
	int n = 0;
	struct itimerval tim = {
		.it_interval = {0, 20},
		.it_value = {0, 20},
	};

	htp_socket_t s = {
		.ip_addr = "127.0.0.1",
		.port	= 10001,
	};
	
	reg_sig();
	setitimer(ITIMER_REAL, &tim, NULL);

	while (1) {
		switch (status) {
		case HTP_OPEN:
			if (htp_open(&s) != 0) {
				fprintf(stderr, "htp_open error.\n");
				status = HTP_OPEN;
			} else {
				status = HTP_RECV;
				sleep(1);
			}
			break;
		case HTP_RECV:
			if (htp_recv(&s, &h) != true) {
				printf("htp_recv failed %d\n", n);
				status = HTP_CLOSE;
			} else {
				status = HTP_SEND;
				printf("htp_recv ok %d\n", n);
			}
			break;
		case HTP_SEND:
			if (htp_send(&s, &h) != true) {
				printf("htp_send failed %d\n", n);
				status = HTP_CLOSE;
			} else {
				printf("htp_send %d bytes\n", s.len);
				status = HTP_RECV;
			}
			n++;
			free(s.buf);
			break;
		case HTP_CLOSE:
			htp_close(&s);
			status = HTP_OPEN;
			break;
		default:
			break;
		}
	}

	return 0;
}
