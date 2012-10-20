#ifndef __MINISIP_
#define __MINISIP_

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


//#define USER_NAME	"8204"
#define USER_NAME	"212"
//#define USER_PASS	"8204"
#define USER_PASS	"welcome"
//#define SERVER_IP	"210.51.10.237"
#define SERVER_IP	"127.0.0.1"
#define SERVER_PORT 5060
//#define LOCAL_IP	"192.168.1.104"
#define LOCAL_IP	"127.0.0.1"
#define LOCAL_PORT  5062
#define LOCAL_RTP_PORT	60000

typedef enum MSG_TYPE_T {
	SIP_UNKNOWN_TYPE,

	SIP_REGISTER,
	SIP_100_TRYING,
	SIP_401_UNAUTH,
	SIP_200_OK,
	SIP_OPTIONS,
	SIP_INVITE,
	SIP_BYE,
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
	SIP_INVITE_TRYING,
	SIP_INVITE_RINGING,
	SIP_INVITE_OK,
	SIP_BYE_RSP,
} SIP_CMD_T;

typedef struct {
	int sd;

	// account information
	//char user_name[32];
	//char user_pawd[32];
	//char local_ip[32];

	int nat_enable;
	char nat_addr[32];
	char nat_port[12];

	// server information
	//char server_ip[32];
	struct sockaddr_in ser_addr;

	char temp[2048];	//for once use
	//session information
	SIP_MSG_T msg;
	int	cseq;

} SIP_T;

char * md5(const char *data, int len, char *out);
int sip_init(SIP_T *sip);
int sip_str_to_msg(SIP_MSG_T *msg, const char *data);
void sip_msg_to_str(SIP_T *sip, char *buf);
int sip_send(SIP_CMD_T cmd, SIP_T *sip);

#endif
