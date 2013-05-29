#include "stdafx.h"

#include "windows_header.h"
#include "htp.h"
#include <signal.h>


static void sig_alarm(int signo) {
	//alarm(1);
}


#if 0 //不支持windows
static int reg_sig(void) {
	struct sigaction old_act;
	sigset_t old_mask, blc_alr_mask;

   struct sigaction new_act;
   sigset_t new_mask;

  /* 纭繚alarm淇″彿娌℃湁琚睆钄?*/
   sigemptyset(&new_mask);
   sigaddset(&new_mask, SIGALRM);
   sigprocmask(SIG_UNBLOCK, &new_mask, &old_mask);

   sigemptyset(&blc_alr_mask);
   sigaddset(&blc_alr_mask, SIGALRM);

   /* 瀹夎alarm淇″彿鎺ュ彈鍑芥暟*/
   new_act.sa_handler = sig_alarm;
   sigemptyset(&new_act.sa_mask);
   new_act.sa_flags = 0;
   if (sigaction(SIGALRM, &new_act, &old_act) == -1) 
      return -2; 

	return 0;
}
#endif

enum {HTP_OPEN, HTP_RECV, HTP_SEND, HTP_CLOSE};

int htp_test(void) {
	htp_header_t h;
	int status = HTP_OPEN;
	int n = 0;

	htp_socket_t s;

	strcpy_s(s.ip_addr,sizeof(s.ip_addr) ,"192.168.1.118");
	s.port = 10001;
	

#if 0 //不支持windows
	struct itimerval tim = {
		.it_interval = {0, 20},
		.it_value = {0, 20},
	};
	reg_sig();
	setitimer(ITIMER_REAL, &tim, NULL);
#endif

	while (1) {
		switch (status) {
		case HTP_OPEN:
			if (htp_open(&s) != 0) {
				fprintf(stderr, "htp_open error.\n");
				status = HTP_OPEN;
			} else {
				status = HTP_RECV;
			}
			break;
		case HTP_RECV:
			h.len %= 100;
			if (htp_recv(&s, &h) != true) {
				printf("htp_recv failed %d\n", n);
				status = HTP_CLOSE;
			} else {
				status = HTP_SEND;
				printf("htp_recv ok %d\n", n);
			}
			break;
		case HTP_SEND:
			n++;
			if (htp_send(&s, &h) != true) {
				printf("htp_send failed %d\n", n);
				status = HTP_CLOSE;
			} else {
				printf("htp_send %u bytes\n", (int)sizeof(h) + s.len);
				status = HTP_RECV;
			}
			if (s.len != 0)	//说明之前申请过内存，需要释放
				htp_free(s.buf);
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

using namespace System;

int main(array<System::String ^> ^args)
{
	WSADATA wsd;
	WSAStartup(MAKEWORD(2,2), &wsd);
    Console::WriteLine(L"Hello World");

	htp_test();

	WSACleanup();
    return 0;
}
