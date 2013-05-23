#include <stdio.h>
#include <string.h>
#include "datatype.h"

/** Ìî³äwritedata Êı¾İÄÚÈİ  */
int fill_write_data(struct HS_PKT_OP_WRITEDATA_T *buf) {
	struct data_area_t *data_area;
	struct sleep_area_t *sleep_area;

	buf->ctrl = 0x1;
	buf->para = 0x1;
	buf->powermode = 0x1;

	buf->wakeup_id = 0x56000000;
	buf->rf_ch = 0;

	buf->data_esl_num = 2;
	buf->sleep_esl_num = 2;

	data_area = &buf->data[0];
	sleep_area = (void *)&buf->data[buf->data_esl_num];

	(void) data_area;
	(void) sleep_area;
	return ((char *)&(buf->sleep[buf->sleep_esl_num]) - (char *)buf);
}

int fill_header(struct HS_PKT_HEADER_T *h) {

	h->version = 0x3;
	h->version_s = ~h->version;
	memcpy(h->version_str, "AP_V3", sizeof(h->version_str));
	
	h->opcode = 0x2;
	h->opcode_s = ~h->opcode;

	h->para = 0x3;
	h->para_s = ~h->para;

	h->len = 100;
	h->len_s = ~h->para_s;

	return 0;
}
