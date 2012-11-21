#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "xmpp-str.c"

static char buf[1024];
static int open_tcp_sd(const char *server_ip, unsigned short port) {
	int sd = -1;
	struct sockaddr_in addr;

	if ((sd = socket(AF_INET, SOCK_STREAM,0)) == -1) {
		perror("socket failed:");
		exit(-1);
	}

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(server_ip);

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
	n = read(sd, buf, 1024);
	buf[n] = '\0';
	printf("%s\n", buf);
}

static void open_stream(int sd, const char *str) {
	int n;
	send_recv(sd, str);
	n=read(sd, buf, 1024);
	buf[n] = '\0';
	printf("%s\n",buf);
}

static int xmpp_login(int sd, const char *user_name, const char *passwd) {

	send_recv(sd, register_str);
	send_recv(sd, register_ack);
	send_recv(sd, online);
	//write(sd, online, strlen(online));

	return 0;
}

static void send_msg(int sd, const char *to, const char *msg) {
	sprintf(buf, "<message to \"%s@192.168.1.127\"><body>%s</body></message>",\
				to, msg);
	(void)write(sd, buf, strlen(buf));
}

int main(int argc, char **argv){
	int sd;

	sd = open_tcp_sd("192.168.1.127", 5222);

	open_stream(sd, open_str);

	xmpp_login(sd, NULL, NULL);

	//send_msg(sd, "201", "message from 211");

	while (1) {
		sleep(1);
	}

	close(sd);
	return 0;
}
