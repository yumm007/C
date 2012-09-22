#ifndef __MINISIP_
#define __MINISIP_

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef enum MSG_TYPE_T {
	SIP_REGISTER,
	SIP_100_TRYING,
	SIP_401_UNAUTH,
	SIP_200_OK,
	SIP_OPTIONS,
	SIP_INCOMMING_CALL,
	SIP_OUTING_CALL,
	SIP_HUNGING,

} MSG_TYPE_T;

typedef struct {

	MSG_TYPE_T msg_type;
	char *type_str;

	char *via;
	char *via_rport;
	char *via_rcvd;
	char *via_branch;

	char *from;
	char *from_tag;
	char *to;
	char *to_tag;

	char *cseq_num;
	char *cseq_method;
	char *allow;
	
	char *accept;
	char *accept_version;

	char *supported;
	char *allow_events;
	char *content_length;

	char *call_id;
	char *user_agent;
	char *contact;

	char *expires;
	char *max_forwards;
	
	char *www_auth;
	char *www_auth_realm;
	char *www_auth_nonce;

	char *date;

} SIP_MSG_T;

typedef enum {
	SIP_INIT,
	SIP_SEND_REGISTER,
	SIP_SEND_REG_RSP,
	SIP_OPTIONS_RSP,
	SIP_MAKE_CALL,
	SIP_BYE,
} SIP_CMD_T;

typedef struct {
	int sd;

	// account information
	char user_name[32];
	char user_pawd[32];
	char local_ip[32];

	// server information
	char server_ip[32];
	struct sockaddr_in ser_addr;

	char temp[2048];	//for once use
	//session information
	SIP_MSG_T msg;
	int	cseq;

} SIP_T;

int sip_init(SIP_T *sip);
int sip_str_to_msg(SIP_MSG_T *msg, const char *data);
void sip_msg_to_str(SIP_T *sip, char *buf);
int sip_send(SIP_CMD_T cmd, SIP_T *sip);

#endif
