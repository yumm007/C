#ifndef __RF_DOT_MODULE_T__
#define __RF_DOT_MODULE_T__

#include <stdint.h>

#define RF_CTRL_TYPE	0x2	/**< ctrl字段高3位为2表示数据帧 */

typedef enum {
	rf_ret_default = 0,
	rf_ret_ok,
	rf_ret_timeout,
	rf_ret_crcerr,
} rf_ret_t;


/** 查询帧返回结果值 */
enum {
	//协议ACK值
	RF_ACK_DATA_ERRCRC 	= 1 << 0,	 		/**< RF数据CRC错误  */
	RF_ACK_DATA_ERRBAT 	= 1 << 1,	 		/**< 终端低电压 */
	RF_ACK_DATA_ERROK	= 1 << 6,	 		/**< RF数据正常 */

	//新增加的ACK值
	RF_ACK_DATA_ERR_FILE_CRC	= 1 << 2,	/**< 文件CRC错误 */
	RF_ACK_DATA_ERR_EPD			= 1 << 3,	/**< EPD屏幕通讯错误 */
	RF_ACK_DATA_ERR_FILE_ZIP	= 1 << 4,	/**< 文件压缩格式错误 */
	RF_ACK_DATA_ERR_FS_OVER		= 1 << 5,	/**< 终端文件系统溢出 */
	RF_ACK_DATA_ERR_LOSS_END	= 1 << 7,	/**< 未收到结束帧 */
} ;

/**
* 传输单个终端的数据
* @param[in]	rcv_id 终端的ID, 长度为4字节
* @param[in]	data 需要发送的数据地址
* @param[in]	len 需要发送的数据长度
* @param[out]	ack_buf 用于模块缓存每个终端的ACK值,低2位表示重传次数，
*				次低2位表示第一次传输结果，次高2位表示第2次传输结果，高2位表示第三次传输结果
* @return 		返回最后一个包的传送结果
*/
rf_ret_t rf_dot_transfer(const uint8_t *rcv_id, const uint8_t *data, int len, uint8_t *ack_buf);

/**
* 发送查询帧查询终端解析数据的结果
* @param[in]	rcv_id 终端的ID, 长度为4字节
* @return		传输结果, 等于0表示RF层出错，否则等于上面的enum值
*/
uint8_t rf_dot_transfer_queue(const uint8_t *rcv_id);

/**
* 库需要调用的外部函数，用于发送（接收）一个数据包给（从）指定终端，超时时间为timeout ms，返回RF发送/接收状态
* 以及延时函数，主要用于发送结束帧时的间隔延时
*/
extern rf_ret_t RF_TxBuf(const uint8_t *rcv_id, const uint8_t* data, uint8_t len, uint16_t timeout);
extern rf_ret_t RF_RxBuf(const uint8_t *rcv_id, uint8_t* buf, uint8_t len, uint16_t timeout);
extern void RF_Delay100us(uint16_t num);
#endif
