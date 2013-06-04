
int htp_recv_job(uint8_t *buf, int len) {
	ftp_t ftp;
	int i, nr;
	
	//如果JOB文件不存在或者buf空间不够则退出
	if ((nr = ftp_file_get(&ftp, FILE_JOB_NAME, buf, len)) == -1)
		return -1;
	//上传状态文件，告知已经接收完数据, 最多上传3次
	for (i = 0; i < TRY_TIMES; i++)
		if (ftp_file_put(&ftp, FILE_JOB_RCVED, " ", 1) == 0)
			break;
	return i < TRY_TIMES ? nr : -1;
}

int htp_recv_kickoff(void) {
	ftp_t ftp;
	int i;

	//等待KICKOFF通知, 最多等WAIT_SEC秒
	for (i = 0; i < WAIT_SEC * 100; i++) {
		if (ftp_file_size(&ftp, FILE_KICKOFF) >= 0)
			break;
		time_sleep_ms(10);
	}
	return i < WAIT_SEC * 100 ? 0 : -1;
}

int htp_send_ack(const uint8_t *ack, int len) {
	ftp_t ftp;
	int i;

	//上传ACK文件，告知已经接收完数据, 最多上传3次
	for (i = 0; i < TRY_TIMES; i++)
		if (ftp_file_put(&ftp, FILE_ACK, ack, len) == 0)
			break;
	return i < TRY_TIMES ? 0 : -1;
}

int main_exam(void) {
	//监听端口，获得连接
	//端口连接，开始工作
	int len;
	uint8_t buf[1024], ret[256] = {0};

	if ((len = htp_recv_job(buf, sizeof(buf))) > 0 \ 
		 && htp_recv_kickoff() == 0)
	{
		//do_rf_task,任务数据在buf中, task返回值在ret中
		//len = do_rf_task(buf, len, ret);
		;
	}
	
	htp_send_ack(ack, len);
}
