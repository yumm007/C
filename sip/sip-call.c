#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef enum {
	SIP_CMD_MIN,

	SIP_INIT = SIP_CMD_MIN, 	//init socket, status
	SIP_REG,	//register

	SIP_CMD_MAX,
} cmd_t;

#define BUFSIZE	1024

static void sip_send_cmd(cmd_t cmd);
static void udp_send(const char *date, int len);
struct sip_t {
	int 	sd;
	void (*cmd)(cmd_t);
	void (*send)(const char *, int);
	struct sockaddr_in ser_addr;

};
static struct sip_t sip = {-1, sip_send_cmd, udp_send, {0},};

static void sip_init(void) {
	int sd;
	
	if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) ==-1) {
		perror("socket:");
		return;
	}

	bzero(&sip.ser_addr, sizeof(sip.ser_addr));
	sip.ser_addr.sin_family = AF_INET;
	sip.ser_addr.sin_port = htons(5060);
	sip.ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sd, (void *)&sip.ser_addr, sizeof(sip.ser_addr)) == -1) {
		close(sd);
		perror("bind:");
		return;
	}

	sip.ser_addr.sin_addr.s_addr = inet_addr("210.51.10.237");

	sip.sd = sd;
}

static void udp_send(const char *date, int len) {
	sendto(sip.sd, date, len, 0, (void *)&sip.ser_addr, sizeof(sip.ser_addr));
}

static int read_file(const char *filename, char *buf) {
	int len = 0;
	FILE *fp;

	memset(buf, 0, BUFSIZE);
	if ((fp = fopen(filename, "r")) == NULL) {
		perror("fopen");
		return -1;
	}
	
	while (fgets(buf + len, BUFSIZE - len -1, fp) != NULL) {
		len = strlen(buf);
		buf[len-1] = '\r';
		buf[len] = '\n';
		len++;
	}
	fclose(fp);
	
	return 0;
}

static int md5_response(char *buf) {
	char *p, tmp_nonce[32] = {0};
	char cmd[BUFSIZE];

	if ((p = strstr(buf, "nonce=\"")) == NULL)
		return -1;
	memcpy(tmp_nonce, p+strlen("nonce=\""), 8);
	sprintf(cmd, "%s %s", "./msg/md5.sh", tmp_nonce);
	if (system(cmd) == -1)
		return -1;
	system("sync");
	fflush(NULL);

	memset(buf, 0, BUFSIZE);
	if (read_file("msg/response", buf) == -1) 
		return -1;
	return 0;
}

static void sip_reg(void) {
	char buf[BUFSIZE] = {0};

	if (read_file("msg/register", buf) == -1) 
		return;
	printf("send register\n %s\n", buf);
	sip.send(buf, strlen(buf));

	memset(buf, 0, BUFSIZE);
	recvfrom(sip.sd, buf, BUFSIZE, 0, NULL, NULL);
	printf("\n----recv trying----\n%s\n", buf);

	memset(buf, 0, BUFSIZE);
	recvfrom(sip.sd, buf, BUFSIZE, 0, NULL, NULL);
	printf("\n----recv unauth----\n%s\n", buf);

	if (md5_response(buf) == -1) {
		perror("md5:");
		return;
	}
	printf("send response\n %s\n", buf);
	sip.send(buf, strlen(buf));
	
	return;
}

static void process_session(void) {
	char buf[BUFSIZE] = {0};
	memset(buf, 0, BUFSIZE);
	recvfrom(sip.sd, buf, BUFSIZE, 0, NULL, NULL);
	printf("\n----recv package ----\n%s\n", buf);

	if (strstr(buf, "OPTIONS")) {
		char cmd[128] = {"./msg/opt.sh "}, *p;
		int i;
		if ((p = strstr(buf, "branch=\"")) == NULL)
			return;
		for (i = 0, p += strlen("branch=\""); p[i] != ';'; i++)
			;
		memcpy(cmd + strlen(cmd), p, i);
		if (system(cmd) == -1) {
			perror("system:");
			return;
		}
		system("sync");
		fflush(NULL);

		memset(buf, 0, BUFSIZE);
		read_file("./msg/options", buf);
		printf("send options\n %s\n", buf);
		sip.send(buf, strlen(buf));
	}	

	return;
}

static void (*CMD_ARR[])(void) = {
	sip_init,
	sip_reg,
};

static void sip_send_cmd(cmd_t cmd) {
	if (cmd < SIP_CMD_MIN || cmd >= SIP_CMD_MAX)
		fprintf(stderr, "unknown cmd :%d\n", cmd);
	else
		CMD_ARR[cmd]();
	return;
}


int main(int argc, char *argv[]) {

	sip.cmd(SIP_INIT);
	sip.cmd(SIP_REG);

	while (1) {
		process_session();
	}

	return 0;
}
