#include <stdio.h>
#include <string.h>
#include "minisip.h"
#include "sip_conf.h"
#include "md5.h"

#if 1
int sip_send(SIP_CMD_T cmd, SIP_T *sip) {
#else
int main(int argc, char *argv[]) {
	SIP_CMD_T cmd = SIP_SEND_REGISTER;
	SIP_T sip_1, *sip;
	sip = &sip_1;
#endif
	int len;
	switch (cmd) {
		case SIP_SEND_REGISTER: 
			{
			char branch[64], tag[36], call_id[36];
			sip->cseq += 1;
			len = sprintf(sip->temp, "%s%d", SERVER_IP, sip->cseq);
			md5(sip->temp, len, tag);
			len = sprintf(sip->temp, "%s%d", LOCAL_IP, sip->cseq);
			md5(sip->temp, len, call_id);
			snprintf(branch, 42, "%s%s", tag, call_id);

			memset(sip->temp, 0, sizeof(sip->temp));
			len = sprintf(sip->temp, "REGISTER sip:%s SIP/2.0\r\nVia: SIP/2.0/UDP %s:5060;rport;branch=%s\r\nMax-Forwards: 70\r\nFrom: <sip:%s@%s>;tag=%s\r\nTo: <sip:%s@%s>\r\nCall-ID: %s\r\nCSeq: %d REGISTER\r\nContact: <sip:%s@%s:5060;ob>\r\nExpires: 300\r\nAllow: PRACK, INVITE, ACK, BYE, CANCEL, UPDATE, SUBSCRIBE, NOTIFY, REFER, MESSAGE, OPTIONS\r\nContent-Length: 0\r\n\r\n", \
				SERVER_IP, LOCAL_IP, branch, USER_NAME, SERVER_IP, tag, \
				USER_NAME, SERVER_IP, call_id, sip->cseq, USER_NAME, LOCAL_IP);
			}
			//fprintf(stderr, "%s", sip->temp);
			break;
		case SIP_SEND_REG_RSP: 
			{
			char val1[36], val2[26], responsed[36];
			len = sprintf(sip->temp, "%s:%s:%s", USER_NAME, sip->msg.www_auth_realm, USER_PASS);
			md5(sip->temp, len, val1);
			len = sprintf(sip->temp, "REGISTER:sip:%s", SERVER_IP);
			md5(sip->temp, len, val2);

			len = sprintf(sip->temp, "%s:%s:%s", val1, sip->msg.www_auth_nonce,val2);
			md5(sip->temp, len, responsed);

			sip->cseq += 1;
			memset(sip->temp, 0, sizeof(sip->temp));
			len = sprintf(sip->temp, "REGISTER sip:%s SIP/2.0\r\nVia: SIP/2.0/UDP %s:5060;rport;branch=%s\r\nMax-Forwards: 70\r\nFrom: <sip:%s@%s>;tag=%s\r\nTo: <sip:%s@%s>\r\nCall-ID: %s\r\nCSeq: %d REGISTER\r\nContact: <sip:%s@%s:5060;ob>\r\nExpires: 300\r\nAllow: PRACK, INVITE, ACK, BYE, CANCEL, UPDATE, SUBSCRIBE, NOTIFY, REFER, MESSAGE, OPTIONS\r\nAuthorization: Digest username =\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"sip:%s\", response=\"%s\", algorithm=MD5\r\nContent-Length: 0\r\n\r\n", \
				SERVER_IP, LOCAL_IP, sip->msg.via_branch, USER_NAME, SERVER_IP, \
				sip->msg.from_tag,USER_NAME, SERVER_IP, sip->msg.call_id, sip->cseq, USER_NAME, LOCAL_IP, USER_NAME, sip->msg.www_auth_realm, sip->msg.www_auth_nonce, SERVER_IP, responsed);
			}
			//fprintf(stderr, "%s", sip->temp);
			break;
		case SIP_OPTIONS_RSP:
			break;
		default:
			break;
	
	}

	printf("-------send msg------%s\n", sip->temp);
	sendto(sip->sd, sip->temp, len, 0, (void *)&sip->ser_addr, sizeof(sip->ser_addr));

	return 0;
}
