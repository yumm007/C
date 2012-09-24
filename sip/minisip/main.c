#include <stdio.h>
#include <string.h>
#include "minisip.h"

static SIP_T sip;
int main(int argc, char **argv) {

	if (sip_init(&sip) != 0) {
		fprintf(stderr, "sip init failed.\n");
		return -1;
	}

	sip_send(SIP_SEND_REGISTER, &sip);
	
	while(1) {
		memset(&sip.temp, 0, sizeof(sip.temp));
		recvfrom(sip.sd, sip.temp, sizeof(sip.temp), 0, NULL, NULL);
		sip_str_to_msg(&sip.msg, sip.temp);
		switch (sip.msg.msg_type) {
			case SIP_100_TRYING:
				printf("<<<<<<<SIP_100_TRYING<<<<<\n%s", sip.temp);
				break;
			case SIP_401_UNAUTH:
				printf("<<<<<SIP_401_UNAUT<<<<<\n%s", sip.temp);
				sip_send(SIP_SEND_REG_RSP, &sip);
				break;
			case SIP_OPTIONS:
				printf("<<<<<<SIP_OPTIONS<<<<<<\n%s", sip.temp);
				sip_send(SIP_OPTIONS_RSP, &sip);
				break;
			case SIP_200_OK:
				printf("<<<<<<SIP_200_OK<<<<<<\n%s", sip.temp); {
				if (strcmp(sip.msg.via_rcvd, LOCAL_IP) != 0 \
					&& strcmp(sip.nat_addr, sip.msg.via_rcvd) != 0) 
				{
					sip.nat_enable = 1;
					strcpy(sip.nat_addr, sip.msg.via_rcvd);
					strcpy(sip.nat_port, sip.msg.via_rport);
					sip_send(SIP_SEND_REGISTER, &sip);
				} else {
					sip.nat_enable = 0;
				}
				}
			
			default:
				break;
		}
	}
	
	close(sip.sd);
	return 0;
}

