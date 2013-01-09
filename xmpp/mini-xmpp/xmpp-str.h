#ifndef __XMPP_STR_H
#define __XMPP_STR_H

char *get_open_str(char *buf);
char *get_register_str(char *buf);
char *get_register_ack(char *buf);
char *get_online_str(char *buf);
char *get_send_msg(char *buf, const char *to, const char *msg);

char *get_send_disco(char *buf, const char *to, const char *id);
char *get_send_init(char *buf, const char *to, const char *id, const char *sec_id, \
							const char *file_name, int file_size);
char *get_send_block_size(char *buf, const char *to, const char *id, const char *sid);
char *get_send_close(char *buf, const char *to, const char *id, const char *sid);

#endif
