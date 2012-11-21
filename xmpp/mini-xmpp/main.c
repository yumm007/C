#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "xmpp-str.h"
#include "mini-xmpp.h"

static char buf[BUFSIZE];
static int open_tcp_sd(void) {
	int sd = -1;
	struct sockaddr_in addr;

	if ((sd = socket(AF_INET, SOCK_STREAM,0)) == -1) {
		perror("socket failed:");
		exit(-1);
	}

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.s_addr = inet_addr(SERVER_IP);

	if (connect(sd, (void *)&addr, sizeof(addr)) == -1) {
		close(sd);
		perror("socket failed:");
		exit(-1);
	}

	return sd;
}

static void send_recv(int sd, const char *str) {
	int n;
	n = write(sd, str, strlen(str));
	n = read(sd, buf, BUFSIZE);
	buf[n] = '\0';
	printf("%s\n", buf);
}

static void open_stream(int sd, const char *str) {
	int n;
	send_recv(sd, str);
	n=read(sd, buf, BUFSIZE);
	buf[n] = '\0';
	printf("%s\n",buf);
}

static void xmpp_login(int sd, const char *user_name, const char *passwd) {
	send_recv(sd, get_register_str(buf));
	send_recv(sd, get_register_ack(buf));
	send_recv(sd, get_online_str(buf));
}

static void send_msg(int sd, const char *to, const char *msg) {
	get_send_msg(buf, to, msg);
	write(sd, buf, strlen(buf));
}

int main(int argc, char **argv){
	int sd;

	sd = open_tcp_sd();

	open_stream(sd, get_open_str(buf));

	xmpp_login(sd, NULL, NULL);

	send_msg(sd, "201", "message from 211");

	while (1) {
		sleep(1);
	}

	close(sd);
	return 0;
}
