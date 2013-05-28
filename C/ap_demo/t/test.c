#include <stdio.h>
#include "linux_header.h"
#include "htp.h"
#include <signal.h>
#include <unistd.h>

static void sig_alarm(int signo) {
	alarm(1);
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


int main(void) {
	//htp_header_t h;
	char buf[1024];
	int n;
	int err_flag = 1;

	htp_socket_t s = {
		.ip_addr = "127.0.0.1",
		.port	= 10001,
	};
	
	reg_sig();
	alarm(1);

	while (1) {
		if (err_flag == 1 && htp_open(&s) != 0) {
			fprintf(stderr, "htp_open error.\n");
			return -1;
		}
		n = read_socket(s.socket, buf, sizeof(buf), 1000*2);
		if (n != sizeof(buf)) {
			fprintf(stderr, "read 1000*20, ret = %d\n", n);
			err_flag = 1;
			htp_close(&s);
		} else {
			printf("read_socket return %d\n", n);
			err_flag = 0;
		}
	}
	return 0;
}
