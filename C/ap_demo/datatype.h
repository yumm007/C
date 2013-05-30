#ifndef __DATA_TYPE_H
#define __DATA_TYPE_H

#include <stdint.h>

/** opcode 编号 */
enum {
	HS_OPCODE_MIN,
	HS_OPCODE_WRITEDATA,
	HS_OPCODE_KICKOFF,
	HS_OPCODE_CANCAL,
	HS_OPCODE_PING,
	HS_OPCODE_ACK = 0x0100,
	HS_OPCODE_NACK = 0x0200,

	HS_OPCODE_MAX,
};

#define CTRL_NORMAL_UPDATA		0x01
#define CTRL_NORMAL_NETLINK		0x02
#define CTRL_NORMAL_QUERY		0x03

//2.0寸点阵价签的各种数据
#define CTRL_DOT20_UPDATA		0x04
#define CTRL_DOT20_NETLINK		0x05
#define CTRL_DOT20_QUERY		0x06

#define CTRL_DOT24_UPDATA		0x07
#define CTRL_DOT24_NETLINK		0x08
#define CTRL_DOT24_QUERY		0x09

#define CTRL_DOT29_UPDATA		0x0A
#define CTRL_DOT29_NETLINK		0x0B
#define CTRL_DOT29_QUERY		0x0C

#define CTRL_DOT43_UPDATA		0x0D
#define CTRL_DOT43_NETLINK		0x0E
#define CTRL_DOT43_QUERY		0x0F

#pragma pack(1)
/** 网络数据包格式  */
typedef struct HS_PKT_T {

	/** 数据包头  */
	struct HS_PKT_HEADER_T {
		uint16_t version;
		uint16_t version_s;
		uint8_t version_str[8];

		uint16_t opcode;
		uint16_t opcode_s;

		uint16_t para;
		uint16_t para_s;
		
		uint32_t len;
		uint32_t len_s;

		uint16_t reserved0;
		uint16_t reserved1;

	} header;

	/** 数据包内容  */
	union {
		struct HS_PKT_OP_WRITEDATA_T {
			uint8_t ctrl;
			uint8_t para;
			uint8_t powermode;
			uint32_t wakeup_id;
			uint8_t rf_ch;
			uint16_t data_esl_num;
			uint16_t sleep_esl_num;

			//不定长的终端数据
			struct data_area_t {
				uint32_t id;
				uint32_t len;
				uint8_t data[0];
			} data[0];
			struct sleep_area_t {
				uint32_t id;
			} sleep[0];
		} write;

		struct HS_PKT_OP_KICKOFF {
			uint8_t d;
		} kickoff;

		struct HS_PKT_OP_CANCL {
			uint8_t d;
		} cancel;

		struct HS_PKT_OP_PING {
			uint8_t d;
		} ping;

		struct HS_PKT_OP_ACK {
			uint8_t d;
		} ack;

		struct HS_PKT_OP_NACK {
			uint8_t d;
		} nack;
	} buf;
	

} HS_PKT_T;

#define PRODUCT_NAME_MAX_LEN	128
#define PORDUCT_ORIGIN_MAX_LEN	24
struct dot_info_t {
   int type;    //靠靠?2.0?2.9靠4.3?
   uint32_t dot_id;   //靠縄D

   int product_id;   //靠ID
   float price;   //靠
   char name[PRODUCT_NAME_MAX_LEN];  //靠?
   char origin[PORDUCT_ORIGIN_MAX_LEN];   //靠   
   //靠靠靠
};

#pragma pack()

int fill_header(struct HS_PKT_HEADER_T *header);
int fill_write_data(struct HS_PKT_OP_WRITEDATA_T *buf, int buf_len, const struct dot_info_t * data_ids, int data_n, \
                    const uint32_t *sleep_ids, int sleep_n);
void write_dump(const HS_PKT_T *pkg, FILE *fp);
void esl_data_dump(const struct dot_info_t *info, FILE *fp, int len);
int lcd_display(const struct dot_info_t *info, uint8_t *out_buf, int out_len);

#endif
