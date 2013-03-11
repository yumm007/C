#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned int ID;

#define DATA_LEN		61
#define RF_RTY_MAX 	3			//重试3次视为发送失败
static bool RF_SEND_FAILED;	//全局RF发送超时标志
extern void rf_recv(UINT8 *data, UINT32 index);
extern UINT8 rf_send(ID id, const UINT8 *data, UINT32 len);
extern UINT8 wait_ack(void);

struct RF_DATA_T {
	UINT8 ctrl:3;
	UINT8 id:5;
	union {
		UINT8 pack_id[2];
		UINT8 slot;
	};
	UINT8 data[DATA_LEN];
};

struct RF_ACK_T {
	UINT8 ctrl:3;
	UINT8 slot:4;
	UINT8 id[4];
	UINT8 err;
	UINT8 pack_id[2];
};

//每次输入2个bit，凑齐8个bit后输出一个字节到out
//初始化bit流，bit_count表示一次传入几个bit，out为输出流
struct BIT_STREAM_T {
	UINT32 byte_count;
	UINT8 *bit_stream;
	UINT8 bit_mask;
	UINT8 bit_count;
	UINT8 val;
};

static struct BIT_STREAM_T BIT = {0, NULL, 0, 0};
static void init_bit_stream(int bit_count, UINT8 *out) {
	memset(&BIT, 0, sizeof(BIT));
	BIT.bit_stream = out;
	BIT.bit_mask = bit_count;
}
//保存2个bit
static void input_bit(UINT8 val){
	int i;

	fprintf(stderr, "input_bit:\n");
	for (i = 0; i < BIT.bit_mask; i++) {
		fprintf(stderr, "b[%d]=%d, ", BIT.bit_count, (val >> i) & 0x1);
		BIT.val |= ((val >> i) & 0x1) << BIT.bit_count;
		BIT.bit_count++;
		if (BIT.bit_count == 8) {
			BIT.bit_count = 0;
			BIT.bit_stream[BIT.byte_count++] = BIT.val;
			BIT.val = 0;
		}

	}
	fprintf(stderr, "\n");
}
//刷新流 输出最后缓存的bit
//返回当前共写入的字节数
static UINT32 flush_bit_stream(void) {
	BIT.bit_stream[BIT.byte_count++] = BIT.val;
	BIT.bit_count = 0;
	BIT.val = 0;
	return BIT.byte_count;
}

static bool send_packet(ID to, UINT32 pack_id, const UINT8 *data, UINT16 len) {
	UINT8 rty = RF_RTY_MAX, ack;
	struct RF_DATA_T rf_data;

	rf_data.ctrl = 0x3;
	rf_data.id = 0xf;
	rf_data.pack_id[0] = pack_id & 0xff;
	rf_data.pack_id[1] = (pack_id >> 8) & 0xff;
	memcpy(rf_data.data, data, len);

	while (rty--) {
		rf_send(to, (void *)&rf_data, sizeof(rf_data));
		if (wait_ack() == 0) {
			ack = 0;
			input_bit(ack);
			return ack ? 1 : 0;
		}
	}

	RF_SEND_FAILED = 1;
	return 1;	//发送错误
}

UINT16 send_data(ID id, const UINT8 *data, UINT32 len, UINT8 *ack) {
	int i, err_count = 0, pack_count = len / DATA_LEN;
	
	RF_SEND_FAILED = false;
	init_bit_stream(2, ack);
	//拆包处理 处理单个数据包
	for (i = 0; i < pack_count && !RF_SEND_FAILED; i++, data += DATA_LEN, len -= DATA_LEN) 
		err_count += send_packet(id, i, data, DATA_LEN);

	//最后一个包
	if (len > 0 && !RF_SEND_FAILED)
		err_count += send_packet(id, i, data, DATA_LEN);

	flush_bit_stream();
	//结束发送
	if (!RF_SEND_FAILED)
		err_count += send_packet(id, 0xFFFF, data, 0);

	return err_count;
}

UINT16 recv_data(UINT8 *buf, UINT32 len) {
	struct RF_DATA_T rf_data;
	UINT32 i = 0, offset;
	
	while (1) {
		rf_recv((void *)&rf_data, i);
		if (rf_data.pack_id == 0xFFFF)
			break;
		offset = rf_data.pack_id * DATA_LEN;
		if (offset + DATA_LEN < len)
			memcpy(buf + offset, rf_data.data, DATA_LEN);
		i++;
	}

	return 0;
}

//------------------------------------------------------------------
//------------------------------------------------------------------

//RF层测试函数
#define RF_BUF_SIZE	1000			//保存1000个RF包
static UINT32 RF_PACK_COUNT = 0; //记录收到了多少个RF包
static UINT8 RF_BUF[1000 * sizeof(struct RF_DATA_T)];
void rf_recv(UINT8 *data, UINT32 index) {
	memcpy(data, RF_BUF + index * sizeof(struct RF_DATA_T), sizeof(struct RF_DATA_T));
	fprintf(stderr, "rf_recv %d\n", index);
}

//超时检测函数，返回非0表示等待ACK超时
UINT8 wait_ack(void) {
	return 0;
}
enum {OK, ECC_ERROR, BAT_LOW, FLASH_ERR};
UINT8 rf_send(ID id, const UINT8 *data, UINT32 len) {
	struct RF_DATA_T *rf_data = (void *)data;
	memcpy(RF_BUF + RF_PACK_COUNT * sizeof(struct RF_DATA_T), data, len);
	fprintf(stderr, "rf_send %d\n", rf_data->pack_id);
	RF_PACK_COUNT++;
	return 0;
}

//用户层测试函数
#define BUF_SIZE	4096
static UINT8 in_data[] = {
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDB,0XFF,
0XDE,0XFF,0XF0,0X3F,0XF0,0X3F,0XFF,0XDB,0XFE,0XE8,0X3F,0XEF,0XDF,0XED,0XDF,0XF0,
0X1B,0XFF,0X3F,0XFF,0XEF,0XDF,0XED,0XDF,0XF5,0X57,0XFF,0XF7,0XFF,0XEF,0XDF,0XE5,
0XDF,0XE4,0XCF,0XFF,0XB7,0X3F,0XF0,0X3F,0XFE,0X3F,0XF0,0X1F,0XFF,0XB6,0XBF,0XFF,
0XFF,0XFF,0XFF,0XF4,0XCF,0XFF,0XB1,0XBF,0XFF,0XFF,0XFF,0XFF,0XF1,0X57,0XFE,0X07,
0X7F,0XF7,0XDF,0XFF,0XFF,0XF5,0XDB,0XFF,0XB5,0X7F,0XE0,0X1F,0XFF,0XFF,0XF0,0X1B,
0XFF,0XB6,0X7F,0XFF,0XDF,0XFF,0XFF,0XFF,0XDB,0XFF,0XB7,0X3F,0XFF,0XFF,0XFF,0XFF,
0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE8,0X03,0XFF,0XFF,0XFF,0XF0,0X3F,
0XFF,0XFF,0XEB,0X7F,0XFE,0X00,0X3F,0XEF,0XDF,0XFF,0XFF,0XEB,0XBB,0XFE,0XFE,0XBF,
0XEF,0XDF,0XFF,0XFF,0XE8,0X03,0XFE,0XAE,0XBF,0XEF,0XDF,0XFF,0XFF,0XEF,0XFF,0XFE,
0XAE,0XBF,0XF0,0X3F,0XFF,0XFF,0XE8,0X03,0XFE,0X80,0XBF,0XFF,0XFF,0XFF,0XFF,0XEB,
0X7F,0XFE,0XAE,0XBF,0XFF,0XFF,0XFF,0XFF,0XEB,0XBB,0XFE,0XAA,0XBF,0XF7,0XDF,0XFF,
0XFF,0XE8,0X03,0XFE,0XAC,0XBF,0XE0,0X1F,0XFF,0XFF,0XEF,0XFF,0XFE,0XFE,0XBF,0XFF,
0XDF,0XFF,0XFF,0XF4,0X6F,0XFE,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XE3,0X6F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF4,0X03,0XFF,0XFF,0XFF,0XF0,0X3F,0XFF,0XFF,0XF7,0X5F,
0XFF,0XFF,0XFF,0XEF,0XDF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XEF,0XDF,0XFF,0XFF,
0XF7,0X7F,0XFF,0XFF,0XFF,0XEF,0XDF,0XFF,0XFF,0XF7,0X7F,0XFF,0XFF,0XFF,0XF0,0X3F,
0XFF,0XFF,0XF0,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0X7F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF7,0X7F,0XFF,0XFF,0XFF,0XF7,0XDF,0XFF,0XFF,0XF7,0X7F,0XFF,
0XFF,0XFF,0XE0,0X1F,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XEF,
0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEE,0X5F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XE9,0X5F,0XFF,0XFF,0XFF,0XF0,0X3F,0XFF,0XFF,0XEF,0X5F,0XFF,0XFF,0XFF,0XEF,
0XDF,0XFF,0XFF,0XEF,0X5F,0XFF,0XFF,0XFF,0XEF,0XDF,0XFF,0XFF,0XEF,0X5B,0XFF,0XFF,
0XFF,0XEF,0XDF,0XFF,0XFF,0XEC,0X5B,0XFF,0XFF,0XFF,0XF0,0X3F,0XFF,0XFF,0XE3,0X7B,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X07,0XFF,0XFF,0XFF,0XF3,0X9F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0X5F,0XFF,0XFF,0XF4,0XFF,0XFF,0XFF,0XFF,0XEE,0XDF,
0XFF,0XFF,0XF5,0X5B,0XFF,0XFF,0XFF,0XED,0XDF,0XFF,0XFF,0XF5,0X2B,0XFF,0XFF,0XFF,
0XF3,0XDF,0XFF,0XFF,0XE1,0X57,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF5,0X2B,0XFF,
0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XF5,0X5B,0XFF,0XFF,0XFF,0XEE,0X9F,0XFF,0XFF,0XF5,
0X67,0XFF,0XFF,0XFF,0XEE,0XDF,0XFF,0XFF,0XE1,0X4F,0XFF,0XFF,0XFF,0XEE,0XDF,0XFF,
0XFF,0XF5,0X37,0XFF,0XFF,0XFF,0XF0,0X3F,0XFF,0XFF,0XF5,0X7B,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF4,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0X7F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF6,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XED,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XD3,0XFF,0XFF,0XCE,0XFF,0XFF,0XFB,0XFF,
};
static UINT8 out_data[BUF_SIZE + DATA_LEN];
static UINT8 out_ack[BUF_SIZE / 4 +1];

int main(void) {
	int i;
	send_data(0x12345678, in_data, sizeof(in_data), out_ack);

	for (i = 0; i < 10; i++) {
		fprintf(stderr, "%02x, ",out_ack[i]);
	}
	fprintf(stderr, "\n");

	recv_data(out_data, sizeof(out_data));

	if (memcmp(in_data, out_data, sizeof(in_data)) != 0)
		fprintf(stderr, "memcmp failed.\n");
	else 
		fprintf(stderr, "recv success.\n");

	return 0;
}
