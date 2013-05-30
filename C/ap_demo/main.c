#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>

#include "linux_header.h"
#include "htp.h"
#include "datatype.h"

#if 0
int socket_open(const char *addr, int port) {
	int sd = -1;
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
	sd = -1;
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

	//write_dump((void *)data, stderr);
	return n;
}
#endif

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
		if ((*data_ids = malloc(sizeof(struct dot_info_t) * 25)) == NULL \
			|| (*sleep_ids = malloc(sizeof(uint32_t) * 25)) == NULL) 
		{
			fprintf(stderr, "err free %p\n", *data_ids);
			fprintf(stderr, "err free %p\n", *sleep_ids);
			free(*data_ids);
			free(*sleep_ids);
			return -1;
		}
	}
	
	n = rand() % 20 + 1;

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
	int ret = 0, data_n, sleep_n, n;
	char bufs[1024*1024];
	HS_PKT_T *pkt =(void *)bufs;

	struct dot_info_t *data_ids = NULL;
	uint32_t *sleep_ids = NULL;

	htp_socket_t s = {
		.ip_addr = "127.0.0.1",
		.port = 10001
	};

	htp_header_t *h = (void *)bufs;

	if (htp_open(&s) != 0) {
		fprintf(stderr, "htp_open failed.\n");
		return -1;
	}
	
	while (1) {
		//生成随机的测试数据
		n = make_rand_dot_info(&data_ids, &sleep_ids);
		if (n <= 0) {
			ret = -1;
			break;
		}
		data_n = n;
		sleep_n = n;
		//填写内容区
		n = fill_write_data(&pkt->buf.write, sizeof(bufs) - sizeof(htp_header_t), data_ids, data_n, sleep_ids, sleep_n);
		if (n <= 0) {
			fprintf(stdout, "fill_write_data() failed.\n");
			ret = -1;
			break;
		}
		//将要发送的内容区地址存入socket->buf中,供htp_send用
		s.buf = (void *)(h+1);
		s.len = n;

		//填写header
		//len = fill_header(&pkt->header);
		htp_ass_header(h, HS_OPCODE_WRITEDATA, 2, n);
		if (htp_send(&s, h) == false) {
			fprintf(stderr, "htp_send failed.\n");
			break;
		}

		fprintf(stderr, "send %d esl data, total %d\n", data_n, sizeof(*h) + s.len);
		//fprintf(stderr, "free %p\n", data_ids);
		//fprintf(stderr, "free %p\n", sleep_ids);
		//free(data_ids);
		//free(sleep_ids);
	}

	htp_close(&s);
	return ret;
}
