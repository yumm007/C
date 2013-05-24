#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include <arpa/inet.h>

#include "datatype.h"

int socket_open(const char *addr, int port) {
	int sd;
	struct sockaddr_in svr;

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket:");
		goto _exit;
	} 

	bzero(&svr, sizeof(svr));
	svr.sin_family = AF_INET;
	svr.sin_addr.s_addr = inet_addr(addr);
	svr.sin_port = htons(port);

	if (connect(sd, (void *)&svr, sizeof(svr)) == -1) {
		perror("connect:");
		goto _close;
	}
	
	return sd;

_close:
	close(sd);
_exit:
	return sd;
}

int socket_write(int sd, uint8_t *data, int len) {
	int n, err;
	struct timeval tim = {3, 0};

	//set send timeout
	setsockopt(sd, SOL_SOCKET, SO_SNDTIMEO, &tim, sizeof(tim));
	while (len > 0) {
		n = send(sd, data, len, 0);
		err = errno;
		if (n > 0 && (err == EAGAIN || err == EWOULDBLOCK)) {
			//timeout
			data += n;
			len -= n;
		} else if (err == EINTR) // intterupt
			continue;
		else 
			break; //other err
	}

	write_dump((void *)data, stderr);
	return n;
}


int main(void) {
	int sd, len;
	char bufs[1024*10];
	HS_PKT_T *pkt =(void *)bufs;

	if ((sd = socket_open("127.0.0.1", 21)) == -1)
		return -1;
	
	//read(sd, &pkt->buf.write, 1024);
	//socket_write(sd, (uint8_t *)&pkt->buf.write, 10);

	//printf("%s\n", (char *)&pkt->buf.write);
	len = fill_header(&pkt->header);
	len += fill_write_data(&pkt->buf.write);

	socket_write(sd, (void *)pkt, len);

	close(sd);
	return 0;
}
