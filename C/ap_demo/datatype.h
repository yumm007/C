#ifndef __DATA_TYPE_H
#define __DATA_TYPE_H

#include <stdint.h>

/** opcode ±àºÅ */
enum {
	HS_OPCODE_WRITEDATA,
	HS_OPCODE_KICKOFF,
	HS_OPCODE_CANCAL,
	HS_OPCODE_PING,
	HS_OPCODE_ACK,
	HS_OPCODE_NACK,

	HS_OPCODE_MAX,
};

#pragma pack(1)
/** ÍøÂçÊý¾Ý°ü¸ñÊ½  */
typedef struct HS_PKT_T {

	/** Êý¾Ý°üÍ·  */
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

	/** Êý¾Ý°üÄÚÈÝ  */
	union {
		struct HS_PKT_OP_WRITEDATA_T {
			uint8_t ctrl;
			uint8_t para;
			uint8_t powermode;
			uint32_t wakeup_id;
			uint8_t rf_ch;
			uint16_t data_esl_num;
			uint16_t sleep_esl_num;

			//²»¶¨³¤µÄÖÕ¶ËÊý¾Ý
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
#define PORDUCT_ORIGIN_MAX_LEN	128
struct dot_info_t {
   int type;    //¿¿¿¿¿2.0¿2.9¿¿4.3¿
   uint32_t dot_id;   //¿¿¿ID

   int product_id;   //¿¿ID
   float price;   //¿¿
   char name[PRODUCT_NAME_MAX_LEN];  //¿¿¿
   char origin[PORDUCT_ORIGIN_MAX_LEN];   //¿¿   
   //¿¿¿¿¿¿
};

#pragma pack()

int fill_write_data(struct HS_PKT_OP_WRITEDATA_T *buf);
int fill_header(struct HS_PKT_HEADER_T *header);

void write_dump(const HS_PKT_T *pkg, FILE *fp);
int lcd_display(const struct dot_info_t *info, uint8_t *out_buf);

#endif
