#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "minisip.h"
#include "sip_conf.h"

#define BUF_SIZE 1024

int sip_init(SIP_T *sip) {
	int sd;

	memset(sip, 0, sizeof(SIP_T));

	if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) ==-1) {
        perror("socket:");
        return -1;
    }   

    bzero(&sip->ser_addr, sizeof(sip->ser_addr));
    sip->ser_addr.sin_family = AF_INET;
    sip->ser_addr.sin_port = htons(5060);
    sip->ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sd, (void *)&sip->ser_addr, sizeof(sip->ser_addr)) == -1) {
        close(sd);
        perror("bind:");
        return -1;
    }   

    sip->ser_addr.sin_addr.s_addr = inet_addr("210.51.10.237");

    sip->sd = sd; 
	return 0;
}


static int process_key(const char *line, const char *key, char delim, char *space) {
	char *p = NULL, *e = NULL;
	int i, len = 0, flag = 0;

	if ((p = strstr(line, key)) != NULL) {
		p += strlen(key);
		for (i = 0; p[i] != '\0'; i++) {
			if (p[i] == '<')
				flag++;
			else if (p[i] == '>')
				flag--;
			else if (p[i] == delim && !flag) {
				e = p + i;
				break;
			}
		}
		len = e - p;
		memcpy(space, p, len);
	}
	return len;
}

static int process_line(const char *line, char *space, SIP_MSG_T *msg) {
	int len = 0, n = 0;

	//printf("process_line: %s\n", line);
	//head
	#define ifhead(key, t) \
		if (strstr(line, key)) {\
			msg->msg_type = t;\
			len = strlen(line);\
			memcpy(space, line, len -1);\
			msg->type_str = space;\
			return len;\
		}
	
	ifhead("REGISTER ", SIP_REGISTER);
	ifhead("100 Trying", SIP_100_TRYING);
	ifhead("401 Unauthorized", SIP_401_UNAUTH);
	ifhead("OPTIONS ", SIP_OPTIONS);
	ifhead("200 OK", SIP_200_OK);

	#define find_key(key, delim, point) \
		if ((n = process_key(line, key, delim, space)) > 0) {\
			point = space;\
			space = space + n +1;\
			len += n + 1;\
		}\

	//via
	if (strstr(line, "Via: ")) {
		find_key("Via: ", ';', msg->via);
		find_key("rport=", ';', msg->via_rport);
		find_key("received=", ';', msg->via_rcvd);
		find_key("branch=", ';', msg->via_branch);
		return len;
	}

	if (strstr(line, "From: ")) {
		find_key("From: ", ';', msg->from);
		find_key("tag=", ';', msg->from_tag);
		return len;
	}

	if (strstr(line, "To: ")) {
		find_key("To: ", ';', msg->to);
		find_key("tag=", ';', msg->to_tag);
		return len;
	}

	if (strstr(line, "CSeq: ")) {
		find_key("CSeq: ", ';', msg->cseq_num);
		return len;
	}

	if (strstr(line, "Contact: ")) {
		find_key("Contact: ", ';', msg->contact);
		return len;
	}

	if (strstr(line, "Call-ID: ")) {
		find_key("Call-ID: ", ';', msg->call_id);
		return len;
	}

	if (strstr(line, "Expires: ")) {
		find_key("Expires: ", ';', msg->expires);
		return len;
	}

	if (strstr(line, "Allow: ")) {
		find_key("Allow: ", ';', msg->allow);
		return len;
	}

	if (strstr(line, "Content-Length: ")) {
		find_key("Content-Length: ", ';', msg->content_length);
		return len;
	}

	if (strstr(line, "Max-Forwards: ")) {
		find_key("Max-Forwards: ", ';', msg->max_forwards);
		return len;
	}

	if (strstr(line, "User-Agent: ")) {
		find_key("User-Agent: ", ';', msg->user_agent);
		return len;
	}
	
	if (strstr(line, "Supported: ")) {
		find_key("Supported: ", ';', msg->supported);
		return len;
	}

	if (strstr(line, "WWW-Authenticate: ")) {
		find_key("WWW-Authenticate: ", ';', msg->www_auth);
		find_key("realm=\"", '"', msg->www_auth_realm);
		find_key("nonce=\"", '"', msg->www_auth_nonce);
		return len;
	}

	if (strstr(line, "Date: ")) {
		find_key("Date: ", ';', msg->date);
		return len;
	}

	if (strstr(line, "Accept: ")) {
		find_key("Accept: ", ';', msg->accept);
		find_key("version=", ';', msg->accept_version);
		return len;
	}

	return 0;
}

static int read_line(char *line, const char *buf) {
	int i = 0;

	for (i = 0; buf[i] != '\0'; i++) {
		if (buf[i] == '\r' && buf[i+1] == '\n') {
			memcpy(line, buf, i);
			return i;
		}
	}

	return 0;
}

static char space_buf[BUF_SIZE * 2] = {0};
int sip_str_to_msg(SIP_MSG_T *msg, const char *data) {
	char line[BUF_SIZE] = {0};
	int n = 0, len = 0, space_n = 0, n1;

	//read line
	memset(space_buf, 0 , sizeof(space_buf));
	while ( (n = read_line(line, data + len)) > 0 ) {
		line[n] = ';';
		if ((n1 = process_line(line, space_buf + space_n, msg)) > 0) {
			space_n = space_n + n1 + 1;
		}
		len += n + 2;	// \r\n
		memset(line, 0, sizeof(line));
	}

	return 0;
}
