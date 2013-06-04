#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include "simple_ftp_client.h"

#define NAME_BUF_LEN	256

static char *id;

static void sleep_ms(int ms) {
   struct timeval tim = {0, ms * 1000};
   select(0, NULL, NULL, NULL, &tim);
}

int htp_recv_start(void) {
	FILE *fp;
	char buf[128];

	while (1) {
		memset(buf, 0, sizeof(buf));
		fp = fopen("TEMPFILE", "r");
		if (fp == NULL)
			continue;
		fread(buf, 1, sizeof(buf), fp);
		if (strcmp(buf, "connecting...") == 0) {
			fclose(fp);
			return 0;
		}
		fclose(fp);
		sleep_ms(10);
	}

	return 1;
}

int htp_recv_job(uint8_t *buf, int len) {
	char names[NAME_BUF_LEN];
	//如果JOB文件不存在或者buf空间不够则退出
	snprintf(names, NAME_BUF_LEN, "%s_%s", id, FILE_JOB_NAME);
	if (ftp_file_get(names, buf, len) == -1)
		return -1;
	//上传状态文件，告知已经接收完数据
	snprintf(names, NAME_BUF_LEN, "%s_%s", id, FILE_JOB_RCVED);
	return ftp_file_put(names, (uint8_t *)" ", 1);
}

int htp_recv_kickoff(void) {
	int i;
	//等待KICKOFF通知, 最多等WAIT_SEC秒
	for (i = 0; i < WAIT_SEC * 100; i++) {
		if (ftp_file_size(FILE_KICKOFF) >= 0)
			break;
		sleep_ms(10);
	}
	return i < WAIT_SEC * 100 ? 0 : -1;
}

int htp_send_ack(const uint8_t *ack, int len) {
	char names[NAME_BUF_LEN];
	//上传ACK文件，告知已经接收完数据
	snprintf(names, NAME_BUF_LEN, "%s_%s", id, FILE_ACK);
	return ftp_file_put(names, ack, len);
}

#include <unistd.h>
#include <stdlib.h>
//假设的rf任务处理函数，随机休眠2～4秒
int do_rf_task(uint8_t *buf, int len, uint8_t *ret) {
	int sec = rand() % 2000 + 2000;
	sleep_ms(sec);
	return 0;
}

int main(int argc, char **argv) {
	//监听端口，获得连接
	//端口连接，开始工作
	int len, count = 0;
	uint8_t buf[1024], ack[256] = {0};

	id = argv[1];
	srand(getpid());
	
	while (1) {
		if (htp_recv_start() != 0)
			return -1;

		if ((len = htp_recv_job(buf, sizeof(buf))) > 0 \
			 && htp_recv_kickoff() == 0)
		{
			//do_rf_task,任务数据在buf中, task返回值在ret中
			len = do_rf_task(buf, len, ack);
			htp_send_ack(ack, len);
			count++;
			printf("task %d ok\n", count);
		} else
			break;
	}
	
	return 0;
}
