#include <string.h>
#include <rtthread.h>
#include <lwip/netdb.h>
#include <lwip/sockets.h>

static const char send_data[] = "this is udp data from RF_Thread.\n";
static char recv_data[60];

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
  	 sendto(sock, send_data, strlen(send_data), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
  	 //rt_kprintf("sock send = %s\n", send_data);
	 rt_thread_delay(RT_TICK_PER_SECOND);
   }
   lwip_close(sock);
}

void udp_recv_demo(void* parameter)
{
   int sock;
   int n = 0;
   struct sockaddr_in server_addr;

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
   		recvfrom(sock, recv_data, sizeof(recv_data), 0, NULL, 0);
		rt_kprintf("%d\tsock recv = %s\n", n++, recv_data);
		rt_thread_delay(RT_TICK_PER_SECOND);
   }
   lwip_close(sock);
}
