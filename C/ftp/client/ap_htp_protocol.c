#include "simple_ftp_client.h"

int htp_recv_job(uint8_t *buf, int len) {
	//如果JOB文件不存在或者buf空间不够则退出
	if (ftp_file_get(FILE_JOB_NAME, buf, len) == -1)
		return -1;
	//上传状态文件，告知已经接收完数据
	return ftp_file_put(FILE_JOB_RCVED, (uint8_t *)" ", 1);
}

int htp_recv_kickoff(void) {
	int i;
	//等待KICKOFF通知, 最多等WAIT_SEC秒
	for (i = 0; i < WAIT_SEC * 100; i++) {
		if (ftp_file_size(FILE_KICKOFF) >= 0)
			break;
	}
	return i < WAIT_SEC * 100 ? 0 : -1;
}

int htp_send_ack(const uint8_t *ack, int len) {
	//上传ACK文件，告知已经接收完数据
	return ftp_file_put(FILE_ACK, ack, len);
}

#include <unistd.h>
#include <stdlib.h>
//假设的rf任务处理函数，随机休眠2～10秒
int do_rf_task(uint8_t *buf, int len, uint8_t *ret) {
	int sec = rand() % 8 + 2;
	sleep(sec);
	return 0;
}

int main(void) {
	//监听端口，获得连接
	//端口连接，开始工作
	int len;
	uint8_t buf[1024], ack[256] = {0};

	if ((len = htp_recv_job(buf, sizeof(buf))) > 0 \
		 && htp_recv_kickoff() == 0)
	{
		//do_rf_task,任务数据在buf中, task返回值在ret中
		len = do_rf_task(buf, len, ack);
		htp_send_ack(ack, len);
	}
	
	return 0;
}
