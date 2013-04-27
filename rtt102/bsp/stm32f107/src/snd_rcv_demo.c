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

#pragma pack(1)
struct PKG_DATA_T {
	uint32_t id;
	uint8_t rcv_len;
	uint8_t data_len;
	uint8_t data[64];
} data = {
	.id = 0x00000056,
	.rcv_len = 0,
	.data_len = 2,
	.data = "this is udp data from RF_THREAD",
};

struct PKG_ACK_T {
	uint8_t	ack;
	uint32_t id;
} ack;
#pragma pack()


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

static int ns = 0, nr = 0;
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
	#if 0
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
	#else
		tim.tv_sec = 0;
		tim.tv_usec = 2000;
		select(0, NULL, NULL, NULL, &tim);
   	bzero(&remote_addr, sizeof(remote_addr));
   	remote_addr.sin_family = AF_INET;
   	remote_addr.sin_port = htons(6000);
   	remote_addr.sin_addr.s_addr = inet_addr("192.168.1.30");
		sendto(sock, (void *)&data, sizeof(data), 0, (void *)&remote_addr, sizeof(remote_addr));
		ns++;
	#endif
   }
   close(sock);

	return NULL;
}


int main(int argc, char **argv) {
	pthread_t psend, precv;

	//pthread_create(&psend, NULL, udp_send_demo, NULL);
	pthread_create(&precv, NULL, udp_recv_demo, NULL);

	while (1) {
		sleep(3);
		printf("send %d, recv %d, miss %0.4f.\n", ns, nr, (ns - nr) / (float)ns);
	}

	return 0;
}

