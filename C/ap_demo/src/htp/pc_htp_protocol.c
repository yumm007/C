#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "datatype.h"

#include "simple_ftp_client.h"

#define NAME_BUF_LEN	256

static void sleep_ms(int ms) {
	struct timeval tim = {0, ms * 1000};
	select(0, NULL, NULL, NULL, &tim);
}

int htp_send_start(const char *ap_list[], int n) {
	int i, sd, ret = -1;
	struct sockaddr_in addr;

	for (i = 0; i < n; i++) {
		if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			goto _ret;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(ap_list[i]);
		addr.sin_port = htons(AP_LISTEN_PORT);

		if (connect(sd, (void *)&addr, sizeof(addr)) == -1) {
			close(sd);
			goto _ret;
		}
		close(sd); //靠靠靠靠靠靠
	}
	ret = 0;
_ret:
	return ret;
}

int htp_send_job(const char *ap_name, const uint8_t *buf, int len) {
	char names[NAME_BUF_LEN];
	snprintf(names, NAME_BUF_LEN, "%s_%s", ap_name, FILE_JOB_NAME);
	return ftp_file_put(names, buf, len);
}

//等待基站收完数据
int htp_check_job_rcved(const char *ap_list[], int n) {
	char buf[NAME_BUF_LEN];
	int i, ok_n, rty_times = CHECK_ACK_10MS_N;
	
	while (rty_times--) {
		for (i = 0, ok_n = 0; i < n; i++) {
			snprintf(buf, NAME_BUF_LEN, "%s_%s", ap_list[i], FILE_JOB_RCVED);
			if (ftp_file_size(buf) != -1)
				ok_n++;
		}
		if (ok_n == n)
			return 0;
		sleep_ms(10);
	}

	return -1;
}

//发送kickoff
int htp_send_kickoff(const char *ap_list[], int n) {
	//临时代码，删除连接文件
	return ftp_file_put(FILE_KICKOFF, (uint8_t *)"kickoff", sizeof("kickoff"));
}

//检查各个AP是否有返回ACK
int htp_check_ack_ready(const char *ap_list[], int n) {
	char buf[NAME_BUF_LEN];
	int i, ok_n, rty_times = CHECK_ACK_10MS_N;
	
	while (rty_times--) {
		for (i = 0, ok_n = 0; i < n; i++) {
			snprintf(buf, NAME_BUF_LEN, "%s_%s", ap_list[i], FILE_ACK);
			if (ftp_file_size(buf) != -1)
				ok_n++;
		}
		if (ok_n == n)
			return 0;
		sleep_ms(10);
	}

	return -1;
}

//取得各个AP的ACK内容
static int htp_dump_ret(struct AP_TASK_T *task, int task_n) {
	int i, n;
	char buf[NAME_BUF_LEN];

	for (i = 0; i < n; i++) {
		snprintf(buf, NAME_BUF_LEN, "%s_%s", task[i].ap_name, FILE_ACK);
		n = ftp_file_get(buf, (uint8_t *)&task[i].data.ack, sizeof(task[i].data.ack));
		task[i].data_len =  n == -1 ? 0 : n;
	}

	return 0;
}

static void del_all_ftp_file(const char *ap_list[], int n) {
	int i;
	char buf[NAME_BUF_LEN];

	for (i = 0; i < n; i++) {
		snprintf(buf, NAME_BUF_LEN, "%s_%s", ap_list[i], FILE_JOB_NAME);
		ftp_file_del(buf);
		snprintf(buf, NAME_BUF_LEN, "%s_%s", ap_list[i], FILE_JOB_RCVED);
		ftp_file_del(buf);
		snprintf(buf, NAME_BUF_LEN, "%s_%s", ap_list[i], FILE_ACK);
		ftp_file_del(buf);
	}
	ftp_file_del(FILE_KICKOFF);
}

int assign_ap_task(struct AP_TASK_T *task, int task_n) {
	//监听端口，获得连接
	//端口连接，开始工作
	int i, ret = -1, n = task_n;
	const char *ap_list[task_n];

	//获得ap list数目
	for (i = 0; i < n; i++)
		ap_list[i] = task[i].ap_name;

	//上传任务文件
	for (i = 0; i < n; i++)
		if (htp_send_job(task[i].ap_name, (uint8_t *)&task[i].data.tosend, \
			task[i].data_len) == -1)
			goto _err;
	//发起连接成功 + 等待AP接收完任务成功 + 发送kickoff成功 + 等待ACK成功 + 读取ACK成功
	if (htp_send_start(ap_list, n) == 0 \
		&& htp_check_job_rcved(ap_list, n) == 0 \
		&& htp_send_kickoff(ap_list, n) != -1 \
		&& htp_check_ack_ready(ap_list, n) == 0\
		&& htp_dump_ret(task, task_n) == 0)
	{
		//任务完成，检测结果
		ret = 0;
	}
	printf("task %s\n", ret == 0 ? "ok" : "failed");

_err:
	//del_all_ftp_file(ap_list, n);
	return ret;
}
