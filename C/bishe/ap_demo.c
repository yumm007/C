#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>

#include "nw_conf.h"
#include "data_type.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct ap_info {
	int sd;						//socket
	int client_num;	
	struct client_info {
		uint16_t id;
	} c[CLIENT_MAX_NUM];
} ap;

/**< 设置广播周期，单位为秒*/
static void beacam_fun(int sig) {
	struct sockaddr_in sa;
	socklen_t len = sizeof(sa);
	int i, j = BEACAM_TIMES;
	PKT_T beacam;
	
	memset(&beacam, 0xff, sizeof(beacam));

	beacam.wakeup.flag = FLAG_BCM_REQ;
	beacam.wakeup.slot = 0;
	beacam.wakeup.interval = 2;
	beacam.wakeup.start_id = 0;
	
	while (j--) {
		for (i = 0; i < ap.client_num; i++) {
			bzero(&sa, sizeof(sa));
			sa.sin_family = AF_INET;
			sa.sin_addr.s_addr = inet_addr("127.0.0.1");
			sa.sin_port = htons(ap.c[i].id + CLIENT_START_PORT);	
			sendto(ap.sd, &beacam, sizeof(beacam), 0, (void *)&sa, len);
			fprintf(stderr, "send brocast to %d\n", i);
		}
		beacam.wakeup.slot++;
	}
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
	return val.it_value.tv_sec * 1000 + val.it_value.tv_usec / 1000;
}

static void ap_init(struct ap_info *ap) {
	struct sockaddr_in sa;
	int sock_d;

	memset(ap, 0, sizeof(*ap));
	
	//创建监听信道
	if ((sock_d = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		fprintf(stderr, "create socket error.\n");
		return;
	}

	bzero(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(AP_UP_PORT);		//should be from configrution file

	if (bind(sock_d, (const struct sockaddr *)&sa, sizeof(sa)) < 0) {
		fprintf(stderr, "bind error.\n");
		close(sock_d);
		sock_d = -1;
	}

	ap->sd = sock_d;
}

int main(int argc, char *argv[]) {
	PKT_T pkg;
	struct sockaddr_in sa;
	socklen_t len = sizeof(sa);

	ap_init(&ap);
	set_beacam_interval(BEACAM_INTERVAL_S);

	while (1) {
		recvfrom(ap.sd, &pkg, sizeof(pkg.sync.req), 0, (void *)&sa, &len);
		switch (FLAG_MASK(pkg.sync.req.flag)) {
			case FLAG_SYNC_REQ:
				fprintf(stdout, "SYNC_REQ form %d\n", pkg.sync.req.id);
				//靠靠靠靠list
				ap.c[ap.client_num].id = pkg.sync.req.id;
				ap.client_num++;
				//靠靠ACK�
				memset(&pkg, 0, sizeof(pkg));
				pkg.sync.ack.flag = FLAG_SYNC_ACK;
				pkg.sync.ack.interval = BEACAM_INTERVAL_S;
				pkg.sync.ack.next_interval = next_beacam_time();
				sendto(ap.sd, &pkg, sizeof(pkg), 0, (void *)&sa, len);
				break;
			default:
				fprintf(stdout, "unknown flag form %d\n", pkg.sync.req.id);
				break;
		}
	}

	close(ap.sd);
	return 0;
}
