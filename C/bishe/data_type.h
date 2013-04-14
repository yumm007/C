#ifndef __DATA_TYPE_H__
#define __DATA_TYPE_H__

#define AP
#define EndPoint

//基站和终端的子长可能不一样，因此需要单独定义它们的数据类型
#ifdef AP
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
#elif define EndPoint
typedef unsigned char UINT8;
typedef unsigned int UINT16;
typedef unsigned long UINT32;
#endif

typedef struct {
	UINT16	interval;
	UINT8		slot;
	UINT8		unused;
	UINT32	start_id;
	UINT8		bitmap[56];
} WK_T;			//广播包

typedef union {
	struct {
		UINT32 	id;
		UINT8		flag;
	} req;		//同步请求
	struct {
		UINT8		flag;
		UINT32	interval;
		UINT32	next_interval;
		UINT8		down_chn;
		UINT8		up_chn;
	} ack;		//同步ACK
} SY_T;			//同步请求包

typedef struct {
	UINT32	id;
	UINT8		flag;
} PU_T;			//PULL请求包

typedef union {
	struct {
		UINT8		flag;
		UINT8 	d[63];
	} data;		//数据
	struct {
		UINT32	id;
		UINT8		flag;
	} ack;		//数据ACK
} DT_T;			//数据包


typedef union {
	WK_T	wakeup;
	SY_T	sync;
	PU_T	pull;
	DT_T	data;
} PKT_T;

typedef enum {
	AP_INIT,        	//AP初始化
   AP_SEND_BEACOM,   //AP发送广播桢
   AP_RCVD_SYNC,     //AP收到同步请求桢
   AP_SEND_SYNC_ACK, //AP回复同步ACK
   AP_RCVD_PULL,     //AP收到PULL请求
   AP_SEND_DATA,     //AP发送数据
   AP_RECV_DATA_ACK, //AP收到数据ACK
   AP_SLEEP,         //AP进入休眠

   AP_TASK_END,
} AP_TASK_T;         //AP工作任务

typedef enum {
   EP_INIT,          //终端初始化
   EP_RECV_BEACOM,   //终端接收广播桢
   EP_SEND_SYNC,     //终端发送同步请求
   EP_RECV_SYNC_ACK, //终端受到同步ACK
   EP_SEND_PULL,     //终端发送PULL请求桢
   EP_RECV_DATA,     //终端收到数据
   EP_SEND_DATA_ACK, //终端回复数据ACK
   EP_SLEEP,         //终端进入休眠

   EP_TASK_END,
} EndPoint_TASK_T;   //终端工作任务

#endif
