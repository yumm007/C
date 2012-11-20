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

static int xmpp_login(int sd, const char *user_name, const char *passwd) {
	int n;
	write(sd, register_str, strlen(register_str));
	n= read(sd, buf, 1024);
	buf[n] = '\0';
	printf("%s\n", buf);
	return 0;
}

int main(int argc, char **argv){
	int sd;
	int n;

	sd = open_tcp_sd("192.168.1.127", 5222);

	//step 1
	write(sd, open_str, strlen(open_str));
	n=read(sd, buf, 1024);
	buf[n] = '\0';
	printf("%s\n",buf);
	n=read(sd, buf, 1024);
	buf[n] = '\0';
	printf("%s\n",buf);

	xmpp_login(sd, NULL, NULL);

	close(sd);
	return 0;
}
