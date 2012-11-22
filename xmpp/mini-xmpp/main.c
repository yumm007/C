#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "xmpp-str.h"
#include "mini-xmpp.h"

#include <sys/types.h>
#include <unistd.h>

static char buf[BUFSIZE];
static int open_tcp_sd(void) {
	int sd = -1;
	struct sockaddr_in addr;

	if ((sd = socket(AF_INET, SOCK_STREAM,0)) == -1) {
		perror("socket failed:");
		exit(-1);
	}

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.s_addr = inet_addr(SERVER_IP);

	if (connect(sd, (void *)&addr, sizeof(addr)) == -1) {
		close(sd);
		perror("socket failed:");
		exit(-1);
	}

	return sd;
}

static void send_recv(int sd, const char *str) {
	int n;
	printf("===>\n%s\n", buf);
	n = write(sd, str, strlen(str));
	n = read(sd, buf, BUFSIZE);
	buf[n] = '\0';
	printf("<===\n%s\n", buf);
}

static void open_stream(int sd, const char *str) {
	int n;
	send_recv(sd, str);
	n=read(sd, buf, BUFSIZE);
	buf[n] = '\0';
	printf("%s\n",buf);
}

static void xmpp_login(int sd, const char *user_name, const char *passwd) {
	send_recv(sd, get_register_str(buf));
	send_recv(sd, get_register_ack(buf));
	send_recv(sd, get_online_str(buf));
}

static void send_msg(int sd, const char *to, const char *msg) {
	get_send_msg(buf, to, msg);
	write(sd, buf, strlen(buf));
}

static char *new_id(void) {
	static unsigned int id = 0x8864;
	static char str[16];

	if (id == 0x8864)
		id = (unsigned int)getpid();

	sprintf(str, "x%X", id);
	id++;
	return str;
}

static char *new_sid(void) {
	static unsigned int sid = 0xAA64;
	static char str[16];
	if (sid == 0xAA64)
		sid = (unsigned int)getpid() + 0xAA00;

	sprintf(str, "x%X", sid);
	sid++;
	return str;
}

static char sock_buf[1460];
static void send_data(int sd, const char *to, const char *data, int len) {
	int i, n, seq;
	char *sid = new_sid();

	send_recv(sd, get_send_init(buf, to, new_id(), sid, "log.xml", 619));
	send_recv(sd, get_send_disco(buf, to, new_id()));
	send_recv(sd, get_send_block_size(buf, to, new_id(), sid));

	sprintf(buf, "<iq id=\"%s\" to=\"%s@%s/Spark 2.6.3\" type=\"set\"><data xmlns=\"http://jabber.org/protocol/ibb\" seq=\"0\" sid=\"%s\">PD94bWwgdmVyc2lvbj0nMS4wJyBlbmNvZGluZz0nVVRGLTgnPz48c3RyZWFtOnN0cmVhbSB4bWxuczpzdHJlYW09Imh0dHA6Ly9ldGhlcnguamFiYmVyLm9yZy9zdHJlYW1zIiB4bWxucz0iamFiYmVyOmNsaWVudCIgZnJvbT0iMTkyLjE2OC4xLjEyNyIgaWQ9IjE1ZDc0NTAzIiB4bWw6bGFuZz0iZW4iIHZlcnNpb249IjEuMCI+PHN0cmVhbTpmZWF0dXJlcz48bWVjaGFuaXNtcyB4bWxucz0idXJuOmlldGY6cGFyYW1zOnhtbDpuczp4bXBwLXNhc2wiPjxtZWNoYW5pc20+RElHRVNULU1ENTwvbWVjaGFuaXNtPjxtZWNoYW5pc20+SklWRS1TSEFSRURTRUNSRVQ8L21lY2hhbmlzbT48bWVjaGFuaXNtPlBMQUlOPC9tZWNoYW5pc20+PG1lY2hhbmlzbT5DUkFNLU1ENTwvbWVjaGFuaXNtPjwvbWVjaGFuaXNtcz48Y29tcHJlc3Npb24geG1sbnM9Imh0dHA6Ly9qYWJiZXIub3JnL2ZlYXR1cmVzL2NvbXByZXNzIj48bWV0aG9kPnpsaWI8L21ldGhvZD48L2NvbXByZXNzaW9uPjxhdXRoIHhtbG5zPSJodHRwOi8vamFiYmVyLm9yZy9mZWF0dXJlcy9pcS1hdXRoIi8+PHJlZ2lzdGVyIHhtbG5zPSJodHRwOi8vamFiYmVyLm9yZy9mZWF0dXJlcy9pcS1yZWdpc3RlciIvPjwvc3RyZWFtOmZlYXR1cmVzPg==</data></iq>", new_id(), to, SERVER_IP, sid);
	send_recv(sd, buf);

	send_recv(sd, get_send_close(buf, new_id(), to, sid));

	return;
#if 0
	for (i = 0, seq = 0; i < len; i += 400, seq++) {
		n = sprintf(sock_buf, "<iq id=\"%s\" to=\"%s@%s/Spark 2.6.3\" type=\"set\"><data xmlns=\"http://jabber.org/protocol/ibb\" seq=\"%d\" sid=\"%s\">", new_id(), to, SERVER_IP, seq, sid);
		base64_encode(sock_buf + n, data+i, (len - i) > 400 ? 400 : (len -i));
		strcat(sock_buf, "</data></iq>");
		
		send_recv(sd, sock_buf);
	}

	send_recv(sd, get_send_close(buf, to, id, sid));
#endif
}


int main(int argc, char **argv){
	int sd;

	sd = open_tcp_sd();

	open_stream(sd, get_open_str(buf));

	xmpp_login(sd, NULL, NULL);

	//send_msg(sd, "201", "message from 211");

	while (1) {
		sleep(10);
		send_data(sd, "201", "text from 211", strlen("text from 211"));
	}

	close(sd);
	return 0;
}
