#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "rf_dot_module.h"

static bool RF_SEND_FAILED;	//全局RF发送超时标志
#define DATA_LEN	61
#define RF_RTY_MAX 	3
#define END_PACKAGE_NUM	3

#define TX_TIMER_OUT	20
#define RX_TIMER_OUT	80	

//结构体都按照1个字节对齐
#pragma pack (1)
typedef struct{
	uint8_t id:5;
	uint8_t ctrl:3;
	union {
		uint16_t pkt_id;
		uint8_t slot;
	}b;
	uint8_t data[DATA_LEN];
}PKT_T;

typedef struct  {
	uint8_t slot:4;
	uint8_t ctrl:3;
	uint8_t ack;
	uint32_t id;
	uint16_t pkt_id;
	uint8_t rev[2];
}ACK_T;

typedef struct {
	uint8_t count:2;
	uint8_t ack:6;	
} ACK_DETAIL_T;
#pragma pack ()

enum {END_PACKET = 0xFFFF, QUEUE_PACKET = 0xFFFE};

static void send_end_package(const uint8_t* slaver_id, uint8_t n){
	PKT_T dpkt;
	int i;

	dpkt.ctrl = RF_CTRL_TYPE;
	dpkt.id = slaver_id[0];
	dpkt.b.pkt_id = END_PACKET;

	for (i = 0; i < n; i++) {
		RF_TxBuf(slaver_id, (uint8_t *)&dpkt, sizeof(PKT_T), TX_TIMER_OUT);
		RF_Delay100us(20);
	}
}

static ACK_DETAIL_T send_packaet_rty(const uint8_t* slaver_id, uint16_t pack_id, const uint8_t *data)
{
	int i;
	rf_ret_t rf_ret = rf_ret_default;
	ACK_DETAIL_T pck_ret;

	PKT_T dpkt;
	ACK_T dack;

	pck_ret.count = 0;
	pck_ret.ack = 0;

	dpkt.ctrl = RF_CTRL_TYPE;
	dpkt.id = slaver_id[0];
	dpkt.b.pkt_id  = pack_id;

	memcpy(dpkt.data, data, DATA_LEN);

	for (i = 0; i < RF_RTY_MAX && rf_ret != rf_ret_ok; i++) {
		RF_TxBuf(slaver_id, (uint8_t *)&dpkt, sizeof(PKT_T), TX_TIMER_OUT);
		rf_ret = RF_RxBuf(slaver_id, (uint8_t *)&dack, sizeof(ACK_T), RX_TIMER_OUT);
		//自记录RF层的结果,重传3次及每次的结果
		//ret的低2位表示重传次数，次低2位表示第一次传输结果，
		//次高2位表示第2次传输结果，高2位表示第三次传输结果
		pck_ret.ack |= (rf_ret << (pck_ret.count * 2));
		pck_ret.count++;	
	}

	//一次都没有成功
	if (i == RF_RTY_MAX && rf_ret != rf_ret_ok)
		RF_SEND_FAILED = 1;	
		
	return pck_ret;
}

rf_ret_t rf_dot_transfer(const uint8_t *rcv_id, const uint8_t *data, int len, uint8_t *pck_ack_buf) {
	int i, n;
	ACK_DETAIL_T pkt_ret, *p;

	n = (len + DATA_LEN -1) / DATA_LEN; //对齐次数

	RF_SEND_FAILED = false;

	for (i = 0; i < n && !RF_SEND_FAILED; i++) {
		pkt_ret = send_packaet_rty(rcv_id, i, &data[DATA_LEN*i]);	//发送第i个数据包
		p =  (ACK_DETAIL_T *)(pck_ack_buf + i);
		p->count = pkt_ret.count;	//记录第i个数据包的记录
		p->ack = pkt_ret.ack;
   	}
	//发送结束帧
	if (!RF_SEND_FAILED) {
		send_end_package(rcv_id, END_PACKAGE_NUM);
	}

	//最后一个包的发送结果即是整体传送的结果
	return (rf_ret_t)((pkt_ret.ack >> ((pkt_ret.count -1) * 2)) & 0x3);
}

uint8_t rf_dot_transfer_queue(const uint8_t *rcv_id) {
	PKT_T dpkt;
	ACK_T dack;
	int i;
	rf_ret_t rf_ret = rf_ret_default;
	uint8_t ret = 0;

	dpkt.ctrl = RF_CTRL_TYPE;
	dpkt.id = rcv_id[0];
	dpkt.b.pkt_id = QUEUE_PACKET;
	dpkt.data[0] = 0;
	
	for (i = 0; i < RF_RTY_MAX && rf_ret != rf_ret_ok; i++) {
		memset(&dack, 0, sizeof(dack));
		RF_TxBuf(rcv_id, (uint8_t *)&dpkt, sizeof(dpkt), TX_TIMER_OUT);
		rf_ret = RF_RxBuf(rcv_id, (uint8_t *)&dack, sizeof(dack), RX_TIMER_OUT);
	}
		
	if (rf_ret == rf_ret_ok) {
		dpkt.data[0] = 1;	//表示收到
		RF_TxBuf(rcv_id, (uint8_t *)&dpkt, sizeof(dpkt), TX_TIMER_OUT);
		ret = dack.ack;
	}

	printf("\t\t\t\t\t\tqueue = 0x%02X\r\n", ret);
	return ret;	
}

