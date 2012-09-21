#include <stdio.h>
#include "minisip.h"

static void sip_msg_dump(SIP_MSG_T *msg) {
	printf("Via: %s\n", msg->via);
	printf("rport: %s\n", msg->via_rport);
	printf("received: %s\n", msg->via_rcvd);
	printf("branch: %s\n", msg->via_branch);
	printf("From: %s\n", msg->from);
	printf("From Tag: %s\n", msg->from_tag);
	printf("To: %s\n", msg->to);
	printf("To Tag: %s\n", msg->to_tag);

	printf("Cseq: %s\n", msg->cseq_num);
	printf("Cseq_version: %s\n", msg->cseq_method);
	printf("Allow: %s\n", msg->allow);
	printf("Accept: %s\n", msg->accept);
	printf("Accept_version: %s\n", msg->accept_version);
	printf("Supported: %s\n", msg->supported);
	printf("Allow_Events: %s\n", msg->allow_events);
	printf("Call-ID: %s\n", msg->call_id);
	printf("User-Agent: %s\n", msg->user_agent);
	printf("Contact: %s\n", msg->contact);
	printf("Expires: %s\n", msg->expires);

	printf("Max-Forwards: %s\n", msg->max_forwards);
	printf("Realm: %s\n", msg->www_auth_realm);
	printf("nonce: %s\n", msg->www_auth_nonce);
	printf("Date: %s\n", msg->date);
	printf("Content-Lenght: %s\n", msg->content_length);
}

static SIP_T sip;
int main(int argc, char **argv) {

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

