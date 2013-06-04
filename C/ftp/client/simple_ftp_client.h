#ifndef __SIMPLE_FTP_CLIENT_H
#define __SIMPLE_FTP_CLIENT_H

#include <stdint.h>

#define FILE_JOB_NAME	"ap_jobs.bin"
#define FILE_JOB_RCVED	"ap_jobs_recved.txt"
#define FILE_KICKOFF		"ap_kickoff.txt"
#define FILE_ACK			"ap_ack.bin"

#define TRY_TIMES	3
#define WAIT_SEC	5

//读取ftp文件到buf中,返回文件大小
int ftp_file_get(const char *file_name, uint8_t *buf, int len);
//上传文件后检查文件大小，如果不对则重传三次
int ftp_file_put(const char *file_name, const uint8_t *buf, int buf_len);
//检查文件大小，文件不存在则返回-1
int ftp_file_size(const char *file_name);
//删除文件大小
int ftp_file_del(const char *file_name);
#endif
