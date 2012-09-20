#include <stdio.h>
#include <string.h>

#include "minisip.h"
#include "sip_conf.h"

#define BUF_SIZE 1024

int sip_init(SIP_T *sip) {
	static int init_flag = 0;

	if (init_flag == 0) {
		//init socker, server addr
		memset(sip, 0, sizeof(SIP_T));
		init_flag = 1;
	}

	//init other

	return 0;
}


static int process_key(const char *line, const char *key, char delim, char *space) {
	char *p, *e;
	int len;

	if ((p = strstr(line, key)) != NULL) {
		e = strchr(p, delim);
		len = e - (p + strlen(key));
		memcpy(space, p + strlen(key), len);
		return len;
	}
	return -1;
}

static void process_line(const char *line, char *space, struct SIP_MSG_T *msg) {
	int len;

	//head
	if (strstr(line, "Trying")) {
		msg->status = SIP_TRYING;
		return;
	} else if (strstr(line, "Unauthorized")) {
		msg->status = SIP_UNAUTH;
		return;
	} else if (strstr(line, "OPTIONS")) {
		msg->status = SIP_OPTIONS;
		return;
	} else if (strstr(line, "200 OK")) {
		msg->status = SIP_OK;
		return;
	}

	//via
	if ((len = process_key(line, "Via: ", ';', space)) > 1) {
		msg->via = space;
		space += len +1;
	}
	if ((len = process_key(line, "rport=", ';', space)) > 1) {
		msg->via_rport = space;
		space += len +1;
	}
	if ((len = process_key(line, "received=", ';', space)) > 1) {
		msg->via_rcvd = space;
		space += len +1;
	}
	if ((len = process_key(line, "branch=", ';', space)) > 1) {
		msg->via_branch = space;
		space += len +1;
	}


	return;
}

static int read_line(char *line, const char *buf) {
	int i = 0;

	for (i = 0; buf[i] != '\0'; i++) {
		//if (buf[i] == '\r' && buf[i+1] == '\n') {
		if (buf[i] == '\n') {
			memcpy(line, buf, i);
			return i;
		}
	}

	return 0;
}

int sip_str_to_msg(SIP_MSG_T *msg, const char *data) {
	char line[BUF_SIZE] = {0};
	int n = 0, len = 0;
	static char buf[BUF_SIZE * 2];

	//read line
	while ( (n = read_line(line, data + len)) > 0 ) {
		strcat(line, ";");
		process_line(line, buf + strlen(buf) + 1, msg);
		len += n + 1;	// \r\n
		memset(line, 0, sizeof(line));
	}

	return 0;
}
