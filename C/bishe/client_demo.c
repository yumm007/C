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

struct client_info {
	int sd;						//socket
	uint32_t id;
} client;

static void beacam_recv_fun(int unused) {
}

/**< 设置广播定时器 */
static void set_beacam_interval(uint16_t tim) {
	struct itimerval val;

   val.it_value.tv_sec = tim;
   val.it_value.tv_usec = 0;
   val.it_interval.tv_sec = tim;
   val.it_interval.tv_usec = 0;

   setitimer(ITIMER_REAL,&val,NULL);
   signal(SIGALRM, beacam_recv_fun);
}


static int recv_tm(int sd, void *buf, size_t len, int msec) {
	fd_set rfds;
	struct timeval tv;
	int retval;
	
	FD_ZERO(&rfds);
	FD_SET(sd, &rfds);

	tv.tv_sec = msec / 1000;
	tv.tv_usec = msec % 1000;

	retval = select(sd + 1, &rfds, NULL, NULL, &tv);
	fprintf(stderr, "select ret = %d\n", retval);
	if (retval > 0)
		return recvfrom(sd, buf, len, 0, NULL, 0);
	return 0;
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
	pkg.sync.req.flag = FLAG_WK_REQ;

	do {
		bzero(&sa, sizeof(sa));
		sa.sin_family = AF_INET;
		sa.sin_addr.s_addr = inet_addr("127.0.0.1");
		sa.sin_port = htons(AP_UP_PORT);	
		sendto(client->sd, &pkg, sizeof(pkg.sync.req), 0, (void *)&sa, len);
		fprintf(stderr, "send sync request\n");

	} while (recv_tm(client->sd, &pkg, sizeof(pkg.sync.ack), 200) <= 0);

	fprintf(stderr, "pkg.sync.ack.interval == \n");

}

int main(int argc, char *argv[]) {
	int i;

	client_init(&client);

	while (1) {
		for (i = 0; i < 100000; i++)
			;
		//printf("next_beacam_time: \n" );
	}

	close(client.sd);
	return 0;
}
