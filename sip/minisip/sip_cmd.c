#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "minisip.h"

#if 1
int sip_send(SIP_CMD_T cmd, SIP_T *sip) {
#else
int main(int argc, char *argv[]) {
	SIP_CMD_T cmd = SIP_SEND_REGISTER;
	SIP_T sip_1, *sip;
	sip = &sip_1;
#endif
	int len = 0;
	switch (cmd) {
		case SIP_SEND_REGISTER: 
			{
			char branch[64]={0}, tag[36]={0}, call_id[36]={0};
			char contact[256];
			sip->cseq += 1;
			len = sprintf(sip->temp, "%s%d", SERVER_IP, sip->cseq);
			md5(sip->temp, len, tag);
			len = sprintf(sip->temp, "%s%d", LOCAL_IP, sip->cseq);
			md5(sip->temp, len, call_id);
			snprintf(branch, 42, "z9hG4bK%s%s", tag, call_id);

			if (sip->nat_enable) {
				sprintf(contact, "Contact: <sip:%s@%s:%s;ob>\r\nContact: <sip:%s@%s:%d;ob>;expires=0", USER_NAME, sip->nat_addr, sip->nat_port, USER_NAME, LOCAL_IP, LOCAL_PORT);
			} else {
				sprintf(contact, "Contact: <sip:%s@%s:%d>;ob", USER_NAME, LOCAL_IP, LOCAL_PORT);
			}

			memset(sip->temp, 0, sizeof(sip->temp));
			len = sprintf(sip->temp, "REGISTER sip:%s SIP/2.0\r\nVia: SIP/2.0/UDP %s:%d;rport;branch=%s\r\nMax-Forwards: 70\r\nFrom: <sip:%s@%s>;tag=%s\r\nTo: <sip:%s@%s>\r\nCall-ID: %s\r\nCSeq: %d REGISTER\r\n%s\r\nExpires: 300\r\nAllow: PRACK, INVITE, ACK, BYE, CANCEL, UPDATE, SUBSCRIBE, NOTIFY, REFER, MESSAGE, OPTIONS\r\nContent-Length: 0\r\n\r\n", \
				SERVER_IP, sip->nat_enable ? sip->nat_addr : LOCAL_IP, sip->nat_enable ? atoi(sip->nat_port) : LOCAL_PORT, branch, USER_NAME, SERVER_IP, tag, \
				USER_NAME, SERVER_IP, call_id, sip->cseq, contact);
			}
			//fprintf(stderr, "%s", sip->temp);
			break;
		case SIP_SEND_REG_RSP: 
			{
			char val1[36]={0}, val2[36]={0}, responsed[36]={0};
			char contact[256];
			len = sprintf(sip->temp, "%s:%s:%s", USER_NAME, sip->msg.www_auth_realm, USER_PASS);
			md5(sip->temp, len, val1);

			len = sprintf(sip->temp, "REGISTER:sip:%s", SERVER_IP);
			md5(sip->temp, len, val2);

			len = sprintf(sip->temp, "%s:%s:%s", val1, sip->msg.www_auth_nonce,val2);
			md5(sip->temp, len, responsed);

			if (sip->nat_enable) {
				sprintf(contact, "Contact: <sip:%s@%s:%s;ob>\r\nContact: <sip:%s@%s:%d;ob>;expires=0", USER_NAME, sip->nat_addr, sip->nat_port, USER_NAME, LOCAL_IP, LOCAL_PORT);
			} else {
				sprintf(contact, "Contact: <sip:%s@%s:%d>;ob", USER_NAME, LOCAL_IP, LOCAL_PORT);
			}

			sip->cseq += 1;
			memset(sip->temp, 0, sizeof(sip->temp));
			len = sprintf(sip->temp, "REGISTER sip:%s SIP/2.0\r\nVia: SIP/2.0/UDP %s:%d;rport;branch=%s\r\nMax-Forwards: 70\r\nFrom: <sip:%s@%s>;tag=%s\r\nTo: <sip:%s@%s>\r\nCall-ID: %s\r\nCSeq: %d REGISTER\r\n%s\r\nExpires: 300\r\nAllow: PRACK, INVITE, ACK, BYE, CANCEL, UPDATE, SUBSCRIBE, NOTIFY, REFER, MESSAGE, OPTIONS\r\nAuthorization: Digest username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"sip:%s\", response=\"%s\", algorithm=MD5\r\nContent-Length: 0\r\n\r\n", \
				SERVER_IP, sip->nat_enable ? sip->nat_addr : LOCAL_IP, sip->nat_enable ? atoi(sip->nat_port) : LOCAL_PORT, sip->msg.via_branch, USER_NAME, SERVER_IP, \
				sip->msg.from_tag,USER_NAME, SERVER_IP, sip->msg.call_id, sip->cseq, contact, USER_NAME, sip->msg.www_auth_realm, sip->msg.www_auth_nonce, SERVER_IP, responsed);
			}
			//fprintf(stderr, "%s", sip->temp);
			break;
		case SIP_OPTIONS_RSP:
			{
			char tag[36] = {0};
			len = sprintf(sip->temp, "%s%s", LOCAL_IP, sip->msg.via_branch);
			md5(sip->temp, len, tag);

			memset(sip->temp, 0, sizeof(sip->temp));
			len = sprintf(sip->temp, "SIP/2.0 200 OK\r\nVia: SIP/2.0/UDP %s:%d;branch=%s;rport=%d\r\nFrom: %s;tag=%s\r\nTo: %s;tag=%s\r\nCall-ID: %s\r\nCseq: %s\r\nAllow: INVITE, ACK, BYE, CANCEL, OPTIONS, MESSAGE, SUBSCRIBE, NOTIFY, INFO\r\nAccept: application/sdb\r\nContent-Length: 0\r\n\r\n", \
				SERVER_IP, SERVER_PORT, sip->msg.via_branch, LOCAL_PORT, sip->msg.from, sip->msg.from_tag, sip->msg.to, tag, sip->msg.call_id, sip->msg.cseq_num);
			}
			break;
		case SIP_INVITE_TRYING:
			memset(sip->temp, 0, sizeof(sip->temp));
			len = sprintf(sip->temp, "SIP/2.0 100 Tring\r\nVia: SIP/2.0/UDP %s:%d;branch=%s;rport=%d\r\nFrom: %s;tag=%s\r\nTo: %s;\r\nCall-ID: %s\r\nCseq: %s\r\nUser-Agent: minisip\r\nContent-Length: 0\r\n\r\n", \
			    SERVER_IP, SERVER_PORT, sip->msg.via_branch, LOCAL_PORT, sip->msg.from, sip->msg.from_tag, sip->msg.to, sip->msg.call_id, sip->msg.cseq_num);
			break;
		case SIP_INVITE_RINGING:
			{
			char tag[36] = {0};
			char tmp[200] = {0};
			int tmp_len = 0;

			len = sprintf(sip->temp, "%s%s", LOCAL_IP, sip->msg.via_branch);
			md5(sip->temp, len, tag);
			memset(sip->temp, 0, sizeof(sip->temp));
			
			len = sprintf(sip->temp, "SIP/2.0 180 Ringing\r\nVia: SIP/2.0/UDP %s:%d;branch=%s;rport=%d\r\nFrom: %s;tag=%s\r\nTo: %s;tag=%s\r\nCall-ID: %s\r\nCseq: %s\r\nContact: <sip:%s@%s:%d>\r\nUser-Agent: minisip\r\nContent-Length: 0\r\n\r\n", \
			    SERVER_IP, SERVER_PORT, sip->msg.via_branch, LOCAL_PORT, sip->msg.from, sip->msg.from_tag, sip->msg.to, tag, sip->msg.call_id, sip->msg.cseq_num, USER_NAME, LOCAL_IP, LOCAL_PORT);
			//break;
		//case SIP_INVITE_OK:
			printf(">>>>>send msg>>>>>\n%s", sip->temp);
			sendto(sip->sd, sip->temp, len, 0, (void *)&sip->ser_addr, sizeof(sip->ser_addr));

			tmp_len = sprintf(tmp, "v=o\r\no=yu 123456 654321 IN IP4 %s\r\ns=A conversation\r\nc=IN IP4 %s\r\nt=0 0\r\nm=audio %d RTP/AVP 0 8 101\r\na=rtpmap:0 PCMU/8000/1\r\n", LOCAL_IP, LOCAL_IP, LOCAL_RTP_PORT);
			len = sprintf(sip->temp, "SIP/2.0 200 OK\r\nVia: SIP/2.0/UDP %s:%d;branch=%s;rport=%d\r\nFrom: %s;tag=%s\r\nTo: %s;tag=%s\r\nCall-ID: %s\r\nCseq: %s\r\nContact: <sip:%s@%s:%d>\r\nUser-Agent: minisip\r\nContent-Type: Application/sdp\r\nContent-Length: %d\r\n\r\n%s", \
			    SERVER_IP, SERVER_PORT, sip->msg.via_branch, LOCAL_PORT, sip->msg.from, sip->msg.from_tag, sip->msg.to, tag, sip->msg.call_id, sip->msg.cseq_num, USER_NAME, LOCAL_IP, LOCAL_PORT, tmp_len, tmp);
			}
			break;
		case SIP_BYE_RSP:
			memset(sip->temp, 0, sizeof(sip->temp));
			len = sprintf(sip->temp, "SIP/2.0 200 OK\r\nVia: SIP/2.0/UDP %s:%d;branch=%s;received=%s\r\nFrom: %s;tag=%s\r\nTo: %s;tag=%s\r\nCall-ID: %s\r\nCseq: %s\r\nContent-Length: 0\r\n\r\n",\
				SERVER_IP, SERVER_PORT, sip->msg.via_branch, SERVER_IP, sip->msg.from, sip->msg.from_tag, sip->msg.to, sip->msg.to_tag, sip->msg.call_id, sip->msg.cseq_num);
			break;
		default:
			break;
	
	}

	printf(">>>>>send msg>>>>>\n%s", sip->temp);

	if (sendto(sip->sd, sip->temp, len, 0, (void *)&sip->ser_addr, sizeof(sip->ser_addr)) < len) {
		perror("send to:");
	}

	return 0;
}
