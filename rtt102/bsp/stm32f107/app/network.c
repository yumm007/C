#include <string.h>
#include <rtthread.h>
#include <lwip/netdb.h>
#include <lwip/sockets.h>

static const char send_data[] = "this is udp data from RF_Thread.\n";
static char recv_data[128];

void udp_send_demo(void* parameter)
{
   int sock;
   struct sockaddr_in server_addr;

   if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
   {
       rt_kprintf("Socket error\n");
       return;
   }

   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(6000);
   server_addr.sin_addr.s_addr = inet_addr("192.168.1.30");
   //rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

   while (1) {
  	 //sendto(sock, send_data, strlen(send_data), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
  	 //rt_kprintf("sock send = %s\n", send_data);
	 rt_thread_delay(RT_TICK_PER_SECOND);
   }
   lwip_close(sock);
}

void udp_recv_demo(void* parameter)
{
   int sock;
   int n = 0, off;
   struct sockaddr_in server_addr;
   struct sockaddr_in remote_addr;
   socklen_t len = sizeof(remote_addr);

   if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
   {
       rt_kprintf("Socket error\n");
       return;
   }

   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(6000);
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  // rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

   if (bind(sock, (void *)&server_addr, sizeof(server_addr)) == -1) {
       rt_kprintf("Bind error\n");
	   lwip_close(sock);
       return;   		
   }
   
   rt_kprintf("udp_recv_demo start.\n");
   while (1) {
   		//recv_data[0] = '\0';	//有offset计算，则丢包率在2.3%
   		//off = sprintf(recv_data, "%d\t", n++);
		//基站发包流速控制在1.8ms，丢包率在0.1%
		off = 0;
   		off += recvfrom(sock, recv_data + off, sizeof(recv_data) - off, 0, (void *)&remote_addr, &len);
		sendto(sock, recv_data, off, 0, (void *)&remote_addr, len);
		//rt_kprintf("%d\tsock recv = %s\n", n++, recv_data);
		//rt_thread_delay(RT_TICK_PER_SECOND);
   }
   lwip_close(sock);
}
