#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>

#include "linux_header.h"
#include "datatype.h"

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

	if ((*data_ids = malloc(sizeof(struct dot_info_t) * 25)) == NULL \
		|| (*sleep_ids = malloc(sizeof(uint32_t) * 25)) == NULL) 
	{
		free(*data_ids);
		free(*sleep_ids);
		return -1;
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

	char *ap_list[] = {"192.168.1.100", NULL};
	uint8_t *ap_data[] = {(uint8_t *)"192.168.1.100", NULL};
	int ap_len[] = {sizeof("192.168.1.100"), 0};

	while (1) {
		//生成随机的测试数据
		n = make_rand_dot_info(&data_ids, &sleep_ids);
		if (n <= 0) {
			fprintf(stderr, "make rand failed.\n");
			ret = -1;
			break;
		}
		data_n = n;
		sleep_n = n;
		//填写内容区
		n = fill_write_data(&pkt->buf.write, sizeof(bufs) - sizeof(pkt->header),\
								data_ids, data_n, sleep_ids, sleep_n);
		if (n <= 0) {
			fprintf(stdout, "fill_write_data() failed.\n");
			ret = -1;
			break;
		}
		//填写header
		fill_header_data(&pkt->header, 1, 0, n);

		//开始任务
		assign_ap_task(ap_list, ap_data, ap_len);
		
		//读取ACK

		//清楚ftp文件区

		free(data_ids);
		free(sleep_ids);
	}

	return ret;
}
