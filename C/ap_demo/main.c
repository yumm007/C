#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>

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

#if 0
static const struct dot_info_t data_ids[] = { 
   {20, 0x56780001, 11111111, 7.9, "烟台苹果", "山东"},
   {20, 0x56780001, 11111111, 7.9, "烟台苹果", "山东"},
   {29, 0x56780002, 11111112, 3.2, "进口香蕉", "越南"},
};

static const uint32_t sleep_ids[] = {0x56780003, 0x56780004};
#endif

static void make_ascii(char *buf, int buf_len) {
	int i;

	buf_len = rand() % buf_len + 1;
	for (i = 0; i < buf_len; i++)
		buf[i] = rand() % 26 + 'a';
	buf[buf_len] = 0;
}

static int make_rand_dot_info(struct dot_info_t **data_ids, uint32_t **sleep_ids) {
	int i, n;
	static int seed = 0;
	
	if (seed == 0) {
		srand((int) getpid());
		seed = 1;
	}
	
	n = rand() % 20 + 1;

	if ((*data_ids = malloc(sizeof(struct dot_info_t) * n)) == NULL \
		|| (*sleep_ids = malloc(sizeof(uint32_t) * n)) == NULL) 
	{
		free(*data_ids);
		free(*sleep_ids);
		return -1;
	}

	for (i = 0; i < n; i++) {
		(*data_ids)[i].type = i % 2 == 0 ? 20 : 29;
		(*data_ids)[i].dot_id = 0x56780001 + i;
		(*data_ids)[i].product_id = 11111111 + i;
		(*data_ids)[i].price = rand() % 100 / 0.9;
		make_ascii((*data_ids)[i].name, PRODUCT_NAME_MAX_LEN);
		make_ascii((*data_ids)[i].origin, PORDUCT_ORIGIN_MAX_LEN);
		(*sleep_ids)[i] = (*data_ids)[i].dot_id;
	}

	return n;
}

int main(void) {
	int ret = 0, sd, len, data_n, sleep_n, n;
	char bufs[1024*1024];
	HS_PKT_T *pkt =(void *)bufs;

	struct dot_info_t *data_ids = NULL;
	uint32_t *sleep_ids = NULL;

	if ((sd = socket_open("127.0.0.1", 21)) == -1)
		return -1;
	
	while (1) {
		len = fill_header(&pkt->header);
		n = make_rand_dot_info(&data_ids, &sleep_ids);
		if (n == 0) {
			ret = -1;
			break;
		}

		data_n = n;
		sleep_n = n;
		n = fill_write_data(&pkt->buf.write, sizeof(bufs) - len, data_ids, data_n, sleep_ids, sleep_n);
		if (n != 0) {
			//socket_write(sd, (void *)pkt, len);
			pkt->header.len = n;
			pkt->header.len_s = ~n;
			//write_dump((void *)pkt, stderr);
		} else {
			fprintf(stdout, "fill_write_data() failed.\n");
			ret = -1;
			break;
		}
	}

	close(sd);
	return ret;
}
