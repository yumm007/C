#include <string.h>
#include <rtthread.h>
#include <lwip/netdb.h>
#include <lwip/sockets.h>
#include "rf_driver.h"

#pragma pack(1)
struct PKG_DATA_T {
	rt_uint32_t id;
	rt_uint8_t rcv_len;
	rt_uint8_t data_len;
	rt_uint8_t data[64];
} data;
#pragma pack()

void udp_transpond_demo(void* parameter)
{
   int sock;
   //int n, off;
   struct sockaddr_in server_addr;
   struct sockaddr_in remote_addr;
   socklen_t addr_len = sizeof(remote_addr);
   rt_uint8_t run_flag = 1;

   if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
       rt_kprintf("Socket error\n");
       goto __exit;
   }

   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(6000);
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

   if (bind(sock, (void *)&server_addr, sizeof(server_addr)) == -1) {
       rt_kprintf("Bind error\n");
	   lwip_close(sock);
       run_flag = 0;   		
   }
   
   rt_kprintf("udp_recv_demo start.\n");
   while (run_flag) {
		//基站发包流速控制在1.8ms，丢包率在0.1%
   		recvfrom(sock, &data, sizeof(data), 0, (void *)&remote_addr, &addr_len);
		rf_send((void *)&data.id, (void *)&data.data, data.data_len);
		//如果需要检查ACK，则打开接收
		if (data.rcv_len > 0 && rf_recv((void *)&data.data, data.rcv_len) == 0)
			sendto(sock, data.data, data.rcv_len, 0, (void *)&remote_addr, addr_len);		
   }
   lwip_close(sock);
__exit:
   rt_kprintf("udp_recv_demo end.\n");
}
