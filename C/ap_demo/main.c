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


static const struct dot_info_t data_ids[] = { 
   {20, 0x56780001, 11111111, 7.9, "烟台苹果", "山东"},
   {20, 0x56780001, 11111111, 7.9, "烟台苹果", "山东"},
   {29, 0x56780002, 11111112, 3.2, "进口香蕉", "越南"},
};

static const uint32_t sleep_ids[] = {0x56780003, 0x56780004};

int main(void) {
	int ret = 0, sd, len, data_n, sleep_n, n;
	char bufs[1024*10];
	HS_PKT_T *pkt =(void *)bufs;

	if ((sd = socket_open("127.0.0.1", 21)) == -1)
		return -1;
	
	len = fill_header(&pkt->header);
	data_n = sizeof(data_ids) / sizeof(data_ids[0]);
	sleep_n = sizeof(sleep_ids) / sizeof(sleep_ids[0]);
	n = fill_write_data(&pkt->buf.write, sizeof(bufs) - len, data_ids, data_n, sleep_ids, sleep_n);
	if (n != 0)
		socket_write(sd, (void *)pkt, len);
	else {
		fprintf(stderr, "fill_write_data() failed.\n");
		ret = -1;
	}

	close(sd);
	return ret;
}
