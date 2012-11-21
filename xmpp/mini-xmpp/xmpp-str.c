#include <stdio.h>
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

