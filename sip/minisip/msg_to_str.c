#include <stdio.h>
#include <string.h>
#include "minisip.h"
#include "sip_conf.h"


static void _append(const char *prefix, 
							const char *delim, 
							const char *point, 
							char *buf) 
{
	if (prefix) 
		strcat(buf, prefix);
	if (point) 
		strcat(buf, point);
	if (delim)
		strcat(buf, delim);
}

void sip_msg_to_str(SIP_T *sip, char *buf) {
	SIP_MSG_T *msg = &sip->msg;
#if 0
	#define app(prefix, point, delim) { \
			if (prefix) \
				strcat(buf, prefix);\
			if (point) \
				strcat(buf, point);\
			if (delim)\
				strcat(buf, delim);\
		}
#else
	#define app(prefix, point, delim) _append(prefix, delim, point, buf)
#endif
	switch(msg->msg_type) {
		case SIP_REGISTER:
			app(NULL, msg->type_str, "\r\n");
			app("Via: ", msg->via, ";");
			app("branch=", msg->via_branch, ";");
			app("rport=", msg->via_rport, ";");
			app("received=", msg->via_rcvd, NULL);
			app(NULL, NULL, "\r\n");
			app("Max-Forwards: ", msg->max_forwards, "\r\n");
			app("From: ", msg->from, ";");
			app("tag=", msg->from_tag, "\r\n");
			app("To: ", msg->to, "\r\n");
			app("Call-ID: ", msg->call_id, "\r\n");
			app("CSeq: ", msg->cseq_num, "\r\n");
			app("Contact: ", msg->contact, "\r\n");
			app("Expires: ", msg->expires, "\r\n");
			app("Allow: ", msg->allow, "\r\n");
			app("Content-Length: ", msg->content_length, "\r\n");
			app(NULL, NULL, "\r\n");
			break;
		case SIP_100_TRYING:
			break;
		case SIP_401_UNAUTH:
			app(NULL, msg->type_str, "\r\n");
			app("Via: ", msg->via, ";");
			app("branch=", msg->via_branch, ";");
			app("received=", msg->via_rcvd, ";");
			app("rport=", msg->via_rport, NULL);
			app(NULL, NULL, "\r\n");
			app("From: ", msg->from, ";");
			app("tag=", msg->from_tag, "\r\n");
			app("To: ", msg->to, ";");
			app("tag=", msg->to_tag, "\r\n");
			app("Call-ID: ", msg->call_id, "\r\n");
			app("CSeq: ", msg->cseq_num, "\r\n");
			app("User-Agent: ", msg->user_agent, "\r\n");
			app("Allow: ", msg->allow, "\r\n");
			app("Supported: ", msg->supported, "\r\n");
			app("WWW-Authenticate: ", msg->www_auth, "\r\n");
			app("Content-Length: ", msg->content_length, "\r\n");
			app(NULL, NULL, "\r\n");
			break;
		case SIP_200_OK:
			break;
		case SIP_OPTIONS:
			break;
		default:
			fprintf(stderr, "unknown msg: %d\n", msg->msg_type);
			break;
	}

	return;
}
