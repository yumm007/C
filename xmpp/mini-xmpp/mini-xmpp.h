#ifndef __MINI_XMPP_H
#define __MINI_XMPP_H

#define SERVER_IP		"192.168.1.127"
#define SERVER_PORT	5222

#define JID_NAME		"211"
#define JID_PSWD		"211"
#define RESOURCE		"mini-xmmp"

#define BUFSIZE		1024

#define FULL_JID_LEN	64
#define JID_LEN	16

typedef enum {
	NO_INIT,

	FREE,
	
	SI_DISCOVER,
	SI_OFFERS,

	IBB_INIT,
	IBB_SEND,
	IBB_CLOSE,

	MSG_RECV,

	UNKNOWN,

} xmpp_status_t;

typedef struct {
	
	//status
	xmpp_status_t status;

	//client information
	char jid_name[JID_LEN];
	char jid_pswd[JID_LEN];
	char jid_resource[JID_LEN];

	//to full jid address
	char jid_to[FULL_JID_LEN];
	char msg_from_jid[FULL_JID_LEN];

	//server ip
	unsigned short *server_port;
	char server_ip[JID_LEN];

	//public functions
	void (*send_msg)(int sd, const char *to, const char *text);
	void (*send_file)(int sd, const char *to, const char *file_name);
	void (*recv_msg)(char *text);
	

	//public temp resouece
	char buf[BUFSIZE];

} xmpp_t;
extern xmpp_t xmpp;

char *base64_encode(char *dst, const char *src, int len);
#endif
