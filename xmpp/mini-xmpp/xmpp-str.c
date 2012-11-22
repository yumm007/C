#include <stdio.h>
#include <string.h>
#include "mini-xmpp.h"

char *get_open_str(char *buf) {
	sprintf(buf, "\"<stream:stream to=\"%s\" xmlns=\"jabber:client\" xmlns:stream=\"http://etherx.jabber.org/streams\" version=\"1.0\">\"", SERVER_IP);
	return buf;
}

char *get_register_str(char *buf) {
	sprintf(buf, "\"<iq type='get' to='%s' id='auth1'> <query xmlns='jabber:iq:auth'/> </iq>\"", SERVER_IP);
	return buf;
}

char *get_register_ack(char *buf) {
	sprintf(buf, "\"<iq type='set' id='auth2'><query xmlns='jabber:iq:auth'><username>%s</username><password>%s</password><resource>%s</resource></query></iq>\"", JID_NAME, JID_PSWD, RESOURCE);
	return buf;
}
char *get_online_str(char *buf) {
	sprintf(buf, "\"<presence><status>Online</status></presence>\"");
	return buf;
}
char *get_send_msg(char *buf, const char *to, const char *msg) {
	sprintf(buf, "<message to=\"%s@%s\" type=\"chat\"> <body>%s</body></message>", to, SERVER_IP, msg);
	return buf;
}

#if 0
char *get_send_init(char *buf, const char *to, const char *id, const char *sid) {
	sprintf(buf, "<iq from=\"%s@%s\" id=\"%s\" to=\"%s@%s\" type=\"set\"> <open xmlns=\"http://jabber.org/protocol/ibb\" block-size=\"4096\" sid=\"%s\" stanza=\"iq\"/></iq>", JID_NAME, SERVER_IP, id, to, SERVER_IP, sid);
	return buf;
}
#endif

char *get_send_init(char *buf, const char *to, const char *id, const char *sec_id, \
							const char *file_name, int file_size) {
	char *file_type;
	if (strstr(file_name , ".xml"))
		file_type="application/xml";
	else
		file_type="application/xml";

	sprintf(buf, "<iq id=\"%s\" from=\"%s@%s/%s\" to=\"%s@%s/Spark 2.6.3\" type=\"set\"><si xmlns=\"http://jabber.org/protocol/si\" id=\"%s\" mime-type=\"%s\" profile=\"http://jabber.org/protocol/si/profile/file-transfer\"><file xmlns=\"http://jabber.org/protocol/si/profile/file-transfer\" name=\"%s\" size=\"%d\" ><desc>Sending file</desc></file><feature xmlns=\"http://jabber.org/protocol/feature-neg\"><x xmlns=\"jabber:x:data\" type=\"form\"><field var=\"stream-method\" type=\"list-single\"><option><value>http://jabber.org/protocol/bytestreams</value></option><option><value>http://jabber.org/protocol/ibb</value></option></field></x></feature></si></iq>" , id, JID_NAME, SERVER_IP, RESOURCE, to, SERVER_IP, sec_id, file_type, file_name, file_size);
	return buf;
}


char *get_send_disco(char *buf, const char *to, const char *id) {
	sprintf(buf, "<iq id=\"%s\" to=\"%s@%s/Spark 2.6.3\" type=\"get\"><query xmlns=\"http://jabber.org/protocol/disco#info\"></query></iq>", id, to, SERVER_IP);
	return buf;
}

char *get_send_block_size(char *buf, const char *to, const char *id, const char *sid) {
	sprintf(buf, "<iq id=\"%s\" to=\"%s@%s/Spark 2.6.3\" type=\"set\"><open xmlns=\"http://jabber.org/protocol/ibb\" block-size=\"4096\" sid=\"%s\" stanza=\"iq\"/></iq>", id, to, SERVER_IP, sid);
	return buf;
}

char *get_send_close(char *buf, const char *id, const char *to, const char *sid) {
	sprintf(buf, "<iq id=\"%s\" to=\"%s@%s/Spark 2.6.3\" type=\"set\"><close xmlns=\"http://jabber.org/protocol/ibb\" sid=\"%s\"/></iq>", id, to, SERVER_IP, sid);
	return buf;
}


