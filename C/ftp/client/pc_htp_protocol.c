#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include "simple_ftp_client.h"

#define NAME_BUF_LEN	256
#define RTY_TIMES	500

static void sleep_ms(int ms) {
	struct timeval tim = {0, ms * 1000};
	select(0, NULL, NULL, NULL, &tim);
}

int htp_send_start(const char *ap_list[], int n) {
	FILE *fp;
	
	fp = fopen("TEMPFILE", "w");
	if (fp == NULL)
		return -1;
	fwrite("connecting...", 1, sizeof("connecting..."), fp);
	fclose(fp);

	return 0;
}

int htp_send_job(const char *ap_name, uint8_t *buf, int len) {
	char names[NAME_BUF_LEN];
	snprintf(names, NAME_BUF_LEN, "%s_%s", ap_name, FILE_JOB_NAME);
	return ftp_file_put(names, buf, len);
}

//等待基站收完数据
int htp_check_job_rcved(const char *ap_list[], int n) {
	char buf[NAME_BUF_LEN];
	int i, ok_n, rty_times = RTY_TIMES;
	
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
	remove("TEMPFILE");
	return ftp_file_put(FILE_KICKOFF, (uint8_t *)"kickoff", sizeof("kickoff"));
}

//检查各个AP是否有返回ACK
int htp_check_ack_ready(const char *ap_list[], int n) {
	char buf[NAME_BUF_LEN];
	int i, ok_n, rty_times = RTY_TIMES;
	
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

static const char *ap_list[] = {"ap0", "ap1", "ap2",};
static void del_all_ftp_file(void) {
	ftp_file_del("ap0_ap_jobs.bin");ftp_file_del("ap0_ap_jobs_recved.txt");ftp_file_del("ap0_ap_ack.bin");
	ftp_file_del("ap1_ap_jobs.bin");ftp_file_del("ap1_ap_jobs_recved.txt");ftp_file_del("ap1_ap_ack.bin");
	ftp_file_del("ap2_ap_jobs.bin");ftp_file_del("ap2_ap_jobs_recved.txt");ftp_file_del("ap2_ap_ack.bin");
	ftp_file_del("ap_kickoff.txt");
}

int main(void) {
	//监听端口，获得连接
	//端口连接，开始工作
	int i, ret = -1, n = sizeof(ap_list) / sizeof(ap_list[0]);
	uint8_t buf[1024];
	int count = 0;

	del_all_ftp_file();

	while (1) {
		//上传任务文件
		for (i = 0; i < n; i++)
			if (htp_send_job(ap_list[i], buf, sizeof(buf)) == -1)
				goto _err;
		//发起连接成功 + 等待AP接收完任务成功 + 发送kickoff成功 + 等待ACK成功
		if (htp_send_start(ap_list, n) == 0 \
			&& htp_check_job_rcved(ap_list, n) == 0 \
			&& htp_send_kickoff(ap_list, n) != -1 \
			&& htp_check_ack_ready(ap_list, n) == 0)
		{
			//任务完成，检测结果
			ret = 0;
			count++;
			printf("task %d ok\n", count);
			del_all_ftp_file();
		} else {
			printf("task %d failed\n", count);
			break; //出现错误，退出
		}
	}
_err:
	return ret;
}
