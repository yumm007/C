#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>

static int my_connect(int sd, const char *addr, int port){
	struct sockaddr_in svr;

	bzero(&svr, sizeof(svr));
	svr.sin_family = AF_INET;
	svr.sin_addr.s_addr = inet_addr(addr);
	svr.sin_port = htons(port);

	if (connect(sd, (void *)&svr, sizeof(svr)) == -1) {
		perror("connect:");
		return -1;
	}

	return 0;
}


int socket_open(const char *addr, int port) {
	int sd;

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket:");
		goto _exit;
	} 

	if (my_connect(sd, addr, port) == -1)
		goto _close;
	
	return sd;

_close:
	close(sd);
_exit:
	return sd;
}

#include "datatype.h"

int main(void) {
	int sd;
	char bufs[1024];
	HS_PKT_T *pkt =(void *)bufs;

	if ((sd = socket_open("127.0.0.1", 21)) == -1)
		return -1;
	
	read(sd, &pkt->buf.write, 1024);


	printf("%s\n", (char *)&pkt->buf.write);

	//my_connect(sd, "127.0.0.1", 22);
	shutdown(sd, SHUT_RDWR);
	//my_connect(sd, "127.0.0.1", 22);
	fill_write_data(&pkt->buf.write);
	close(sd);
	return 0;
}
