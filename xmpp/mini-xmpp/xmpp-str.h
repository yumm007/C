#ifndef __XMPP_STR_H
#define __XMPP_STR_H

char *get_open_str(char *buf);
char *get_register_str(char *buf);
char *get_register_ack(char *buf);
char *get_online_str(char *buf);
char *get_send_msg(char *buf, const char *to, const char *msg);

#endif
