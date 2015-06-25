#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include "aes.h"
#include "base64encode.h"

//const uint8_t AES128_CBC_KEY[]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
//const uint8_t AES128_CBC_KEY[]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
const uint8_t AES128_CBC_KEY[]={0x52,0x66,0x75,0x2a,0x33,0x34,0x76,0x5e,0x2f,0x61,0x65,0x61,0x72,0x6f,0x7b,0x76};
const uint8_t AES128_CBC_IV[]= {0x52,0x66,0x75,0x2a,0x33,0x34,0x76,0x5e,0x2f,0x61,0x65,0x61,0x72,0x6f,0x7b,0x76};

/*
* 将用户数据(AES128 CBC)加密，并转成base64编码，swap是交换空间，长度至少为srclen+16, 
* 最后的数据将回写到dst中，最大需要srclen*1.5字节。
*/
int data_aes128_cbc_base64(uint8_t *src, int srclen, uint8_t *swap, uint8_t *dst) {
	int n;

	n = AES128_CBC_encrypt_buffer(swap, src, srclen, AES128_CBC_KEY, AES128_CBC_IV);
	n = (int) base64_encode(swap, n, (void *)dst);

	return n;
}

#define POST_STR 	"POST / HTTP/1.1 \r\nUser-Agent: curl/7.35.0\r\nHost: 127.0.0.1:5000\r\n"\
						"Accept: */*\r\nContent-Length: %d\r\n"\
						"Content-Type: application/x-www-form-urlencoded\r\n\r\n%s"

int http_post(const char *serip, int port, const uint8_t *data, int datalen) {
	int sd, n;
	struct sockaddr_in addr;
	char buf[512];

	sd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(serip);
	addr.sin_port = htons(port);

	connect(sd, (void *)&addr, sizeof(addr));
	
	n = datalen+strlen("data=");
	n = sprintf(buf, POST_STR, n, "data=");
	send(sd, buf, n, 0);
	send(sd, data, datalen, 0);

	close(sd);
	return 0;
}

int main(int argc, char **argv) {
	uint8_t buf[1024] = {0};
	uint8_t b64[1024] = {0};
	uint8_t in[1024] = {0};
	int n = 0;

	memcpy(in, argv[1], strlen(argv[1]));

	n = data_aes128_cbc_base64(in, strlen(argv[1]), buf, (void *)b64);

	http_post("127.0.0.1", 5000, b64, n);

   return 0;
}

