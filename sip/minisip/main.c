#include <stdio.h>
#include "minisip.h"

static void sip_msg_dump(SIP_MSG_T *msg) {
	printf("Via: %s\n", msg->via);
	printf("rport: %s\n", msg->via_rport);
	printf("received: %s\n", msg->via_rcvd);
	printf("branch: %s\n", msg->via_branch);
}

int main(int argc, char **argv) {
	SIP_T sip;

	//for test
	FILE *fp = fopen("log", "r");
	char tmp[2048];

	fread(tmp, 1, 2048, fp);
	fclose(fp);

	sip_init(&sip);
	sip_str_to_msg(&sip.msg, tmp);
	sip_msg_dump(&sip.msg);

	return 0;
}

