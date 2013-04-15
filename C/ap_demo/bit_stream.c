
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef enum 
{
	rf_ret_default = 0,
	rf_ret_ok,
	rf_ret_timeout,
	rf_ret_crcerr,
}rf_ret_t;

/**<  将字节x从offset位开始的2位清空 */
#define clr_2bit(x, offset) 		(x &= (~(3 << (offset))))
/**<  将字节x从offset位开始的2位与上值v */
#define set_2bit(x, offset, v) 	(x |= (((v) & 3) << (offset)))

void log_detail_ack(UINT16 pkt_num, rf_ret_t ret, UINT8 ack_buf[64]) {
	UINT8 ack_arr[4] = {rf_ret_default, rf_ret_ok, \
							rf_ret_timeout, rf_ret_crcerr};
	UINT8 i = (pkt_num * 2) / 8;
	UINT8 off = (pkt_num * 2) % 8;

	clr_2bit(ack_buf[i], off);
	set_2bit(ack_buf[i], off, ack_arr[ret & 0x3]);
}

rf_ret_t get_detail_ack(UINT16 pkt_num, const UINT8 *ack_buf) {
	UINT8 i = (pkt_num * 2) / 8;
	UINT8 off = (pkt_num * 2) % 8;
	return ((ack_buf[i] >> off) & 0x3);
}

//** 测试函数  */
#include <stdio.h>
#include <string.h>

static rf_ret_t test_ack[]={3,2,1,0,3,2,1,0,3,2,1,0,3,2,1,0,3};
static UINT8 ack_bit[]={0x1b,0x1b,0x1b,0x1b,0x3};

int main(void) {
	UINT8 ack_buf[64];
	int i, n;
	rf_ret_t ret;

	n = sizeof(test_ack) / sizeof(test_ack[0]);

	memset(ack_buf, 0, n);
	for (i = 0; i < n; i++) {
		log_detail_ack(i, test_ack[i], ack_buf);
		printf("%d,", test_ack[i]);
	}

	if (memcmp(ack_buf, ack_bit, sizeof(ack_bit)) != 0)
		printf("\nack_bit failed.\n");

	printf("\n");
	for (i = 0; i < n; i++) {
		ret = get_detail_ack(i, ack_buf);
		printf("%d,", ret);
	}
	printf("\n");

	return 0;
}
