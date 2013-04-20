#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>
#include "nw_conf.h"

struct ap_info {
	pthread_t bcm_pth_id;
	struct client_info {
		uint16_t id;
	} c[CLIENT_MAX_NUM];
} ap;

/**< 设置广播周期，单位为秒*/
static void beacam_fun(int sig) {
	fprintf(stderr, "work.\n");
}

/**< 设置广播定时器 */
static void set_beacam_interval(uint16_t tim) {
	struct itimerval val;

   val.it_value.tv_sec = tim;
   val.it_value.tv_usec = 0;
   val.it_interval.tv_sec = tim;
   val.it_interval.tv_usec = 0;

   setitimer(ITIMER_REAL,&val,NULL);
   signal(SIGALRM,beacam_fun);
}

/**< 返回下一个广播时间，单位为毫秒 */
static uint32_t next_beacam_time(void) {
	struct itimerval val;

	getitimer(ITIMER_REAL, &val);

	return (BEACAM_INTERVAL_S - val.it_value.tv_sec) * 1000 + val.it_value.tv_usec / 1000;
}

int main(int argc, char *argv[]) {
	int i;
	set_beacam_interval(BEACAM_INTERVAL_S);

	while (1) {
		for (i = 0; i < 100000; i++)
			;
		printf("next_beacam_time: %d\n", next_beacam_time());
	}

	return 0;
}
