#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>

#include "nw_conf.h"
#include "data_type.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

enum {STATUS_SLEEP, STATUS_WORKING};

struct client_info {
	int sd;						//socket
	int status;
	uint32_t id;
} client;

static int recv_tm(int sd, void *buf, size_t len, int msec) {
	fd_set rfds;
	struct timeval tv;
	int retval;
	
	FD_ZERO(&rfds);
	FD_SET(sd, &rfds);

	tv.tv_sec = msec / 1000;
	tv.tv_usec = msec;

	retval = select(sd + 1, &rfds, NULL, NULL, &tv);
	fprintf(stderr, "select ret = %d\n", retval);
	if (retval > 0)
		return recvfrom(sd, buf, len, 0, NULL, 0);
	return 0;
}

static void beacam_recv_fun(int unused) {
	PKT_T pkg;

	//如果已经处于工作状态，则直接返回
	if (client.status == STATUS_WORKING)
		return;
	//如果唤醒后5ms内收到广播桢，则回到工作状态，否则继续睡眠
	if (recv_tm(client.sd, &pkg, sizeof(pkg), 5) > 0 \
			&& FLAG_MASK(pkg.wakeup.flag) == FLAG_BCM_REQ) 
	{
		fprintf(stderr, "client resume to working status\n");
		client.status = STATUS_WORKING;
	} else {
		fprintf(stderr, "client return to sleep status\n");
		client.status = STATUS_SLEEP;
	}
}

/**< 设置查询广播定时器, 参数都为毫秒 */
static void set_beacam_interval(uint32_t beacam_tim_s, uint32_t next_tim_ms) {
	struct itimerval val;

   val.it_value.tv_sec = 0;
   val.it_value.tv_usec = next_tim_ms;
   val.it_interval.tv_sec = beacam_tim_s;
   val.it_interval.tv_usec = 0;

   setitimer(ITIMER_REAL,&val,NULL);
   signal(SIGALRM, beacam_recv_fun);
}


static void client_init(struct client_info *client) {
	struct sockaddr_in sa;
	socklen_t len = sizeof(sa);
	PKT_T pkg;

	memset(client, 0, sizeof(*client));
	
	//创建信道
	if ((client->sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		fprintf(stderr, "create socket error.\n");
		return;
	}

	bzero(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(CLIENT_START_PORT + client->id);	

	if (bind(client->sd, (void *)&sa, len) < 0) {
		fprintf(stderr, "bind:");
		close(client->sd);
		return;
	}


	memset(&pkg, 0, sizeof(pkg));
	pkg.sync.req.id = client->id;
	pkg.sync.req.flag = FLAG_SYNC_REQ;

	do {
		bzero(&sa, sizeof(sa));
		sa.sin_family = AF_INET;
		sa.sin_addr.s_addr = inet_addr("127.0.0.1");
		sa.sin_port = htons(AP_UP_PORT);	
		sendto(client->sd, &pkg, sizeof(pkg.sync.req), 0, (void *)&sa, len);
		fprintf(stderr, "send sync request\n");

	} while (recv_tm(client->sd, &pkg, sizeof(pkg), 200) <= 0);

	fprintf(stderr, "interval = %d, next = %d\n", pkg.sync.ack.interval, pkg.sync.ack.next_interval);
	set_beacam_interval(pkg.sync.ack.interval, pkg.sync.ack.next_interval);

}

int main(int argc, char *argv[]) {
	PKT_T pkg;
	struct sockaddr_in sa;
	socklen_t len = sizeof(sa);

	client_init(&client);

	while (1) {
		if (client.status != STATUS_WORKING)
			continue;
		recvfrom(client.sd, &pkg, sizeof(pkg), 0, (void *)&sa, &len);
		switch (FLAG_MASK(pkg.sync.req.flag)) {
			case FLAG_DATA_REQ:	//收到数据桢
				break;
			case FLAG_SYNC_ACK:	//中途收到同步ACK桢
				set_beacam_interval(pkg.sync.ack.interval, pkg.sync.ack.next_interval);
				break;
			default:
				fprintf(stderr, "work one times, now to sleep status\n");
				client.status = STATUS_SLEEP;
				break;
		}
	}

	close(client.sd);
	return 0;
}
