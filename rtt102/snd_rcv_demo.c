#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/time.h>

static int ns = 0, nr = 0;
#if 0
static const char send_data[] = "this is udp data from RF_Thread.";
static char recv_data[60];

static void *udp_send_demo(void* parameter)
{
   int sock;
   struct sockaddr_in server_addr;

   if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
   {
       printf("Socket error\n");
       return NULL;
   }

   bzero(&server_addr, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(6000);
   server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	while (1) {
  		sendto(sock, send_data, strlen(send_data), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
   	printf("sock send = %s\n", send_data);
		sleep(1);
	}
   close(sock);
	return NULL;
}

static void *udp_recv_demo(void* parameter)
{
   int sock;
   struct sockaddr_in server_addr, remote_addr;
	socklen_t len = sizeof(remote_addr);
	fd_set rfds;
	struct timeval tim;
	char buf[256];

   printf("udp_recv_demo start.\n");

   if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
   {
       printf("Socket error\n");
       return NULL;
   }

   bzero(&server_addr, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(6000);
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

   if (bind(sock, (void *)&server_addr, sizeof(server_addr)) == -1) {
       printf("Bind error\n");
	    close(sock);
       return NULL;   		
   }
   
   while (1) {
			FD_ZERO(&rfds);
			FD_SET(sock, &rfds);
			tim.tv_sec = 0;
			tim.tv_usec = 10000;

			if (select(sock +1, &rfds, NULL, NULL, &tim) > 0) {
   			recvfrom(sock, buf, sizeof(buf), 0, (void *)&remote_addr, &len);
				nr++;
			} else {
			
   			bzero(&remote_addr, sizeof(remote_addr));
   			remote_addr.sin_family = AF_INET;
   			remote_addr.sin_port = htons(6000);
   			remote_addr.sin_addr.s_addr = inet_addr("192.168.1.30");
			}
			sendto(sock, send_data, sizeof(send_data), 0, (void *)&remote_addr, sizeof(remote_addr));
			ns++;
   }
   close(sock);

	return NULL;
}

#endif

typedef struct SRV_T {
	int sock;
	struct sockaddr_in ap_addr;
	struct SND_DATA_T {
		uint8_t id[4];
		uint8_t rcv_len;
		uint8_t snd_len;
		uint8_t snd_buf[64];
	} snd_data;
	struct RCV_DATA_T {
		uint8_t id[4];
		uint8_t rcv_len;
		uint8_t rcv_buf[64];
	} rcv_data;
} SRV_T;

static void send_wakeup_frame(SRV_T *srv, const uint8_t *id){
	struct timeval tim;
	int n = 9 * 125, i = 9 * 1000;
	
	//每隔1ms发送一次wakeup帧，共发送9秒，每126毫秒递减一次slot值

	memcpy(srv->snd_data.id, id, sizeof(srv->snd_data.id));
	srv->snd_data.rcv_len = 0;
	srv->snd_data.snd_len = 2;
	srv->snd_data.snd_buf[0] = 0xAA | ((n >> 8) & 1);
	srv->snd_data.snd_buf[1] = n;

	while (i--) {

		tim.tv_sec = 0;
		tim.tv_usec = 1000;
		select(0, NULL, NULL, NULL, &tim);
		
		sendto(srv->sock, &srv->snd_data, sizeof(srv->snd_data), 0, \
					(void *)&srv->ap_addr, sizeof(srv->ap_addr));
		
		if (i % 125 == 0) {
			n--;
			srv->snd_data.snd_buf[0] = 0xAA | ((n >> 8) & 1);
			srv->snd_data.snd_buf[1] = n;
		}

	}
}

static int ap_init(SRV_T *srv) {
	struct sockaddr_in server_addr;

   printf("udp_recv_demo start.\n");

	memset(srv, 0, sizeof(*srv));
   if ((srv->sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
       printf("Socket error\n");
       return 1;
   }

   bzero(&server_addr, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(6000);
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

   if (bind(srv->sock, (void *)&server_addr, sizeof(server_addr)) == -1) {
       printf("Bind error\n");
	    close(srv->sock);
       return 1;   		
   }

   bzero(&srv->ap_addr, sizeof(srv->ap_addr));
   srv->ap_addr.sin_family = AF_INET;
   srv->ap_addr.sin_port = htons(6000);
   srv->ap_addr.sin_addr.s_addr = inet_addr("192.168.1.103");

	return 0;
}

int main(int argc, char **argv) {
	SRV_T srv;
	uint8_t client_id[] = {0x74, 0x01, 0x02, 0x04};
	//pthread_t psend, precv;

	//pthread_create(&psend, NULL, udp_send_demo, NULL);
	//pthread_create(&precv, NULL, udp_recv_demo, NULL);

	if (ap_init(&srv) != 0)
		return 1;

	while (1) {
		send_wakeup_frame(&srv, client_id);
		sleep(3);
		printf("send %d, recv %d, miss %0.4f.\n", ns, nr, (ns - nr) / (float)ns);
	}

	return 0;
}

