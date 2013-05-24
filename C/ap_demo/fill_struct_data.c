#include <stdio.h>
#include <string.h>
#include "datatype.h"

static const struct dot_info_t ids[] = {
	{20, 0x56780001, 11111111, 7.9, "烟台苹果", "山东"},
	{20, 0x56780001, 11111111, 7.9, "烟台苹果", "山东"},
	{29, 0x56780002, 11111112, 3.2, "进口香蕉", "越南"},
};

static const uint32_t sleep_id[] = {0x56780003, 0x56780004};

static void fill_write_data_data_sleep(const struct dot_info_t * info,\
													const uint32_t *sleep_id, struct HS_PKT_OP_WRITEDATA_T *w) {
   struct data_area_t *d = (void *)&w->data[0];
   struct sleep_area_t *s; 
   int i;

   for (i = 0; i < w->data_esl_num; i++, info++) {
		d->id = info->dot_id;
		d->len = lcd_display(info, d->data);
      d = (void *)((char *)d + d->len);
   }

   s = (void *)d;
   for (i = 0; i < w->sleep_esl_num; i++, s++, sleep_id++)
		s->id = *sleep_id;
}


/** 填充writedata 数据内容  */
int fill_write_data(struct HS_PKT_OP_WRITEDATA_T *buf) {
	buf->ctrl = 0x1;
	buf->para = 0x1;
	buf->powermode = 0x1;

	buf->wakeup_id = 0x56000000;
	buf->rf_ch = 0;

	buf->data_esl_num = sizeof(ids) / sizeof(ids[0]);
	buf->sleep_esl_num = sizeof(sleep_id) / sizeof(sleep_id[0]);

	fill_write_data_data_sleep(ids, sleep_id, buf);

	return ((char *)&(buf->sleep[buf->sleep_esl_num]) - (char *)buf);
}

int fill_header(struct HS_PKT_HEADER_T *h) {

	h->version = 0x3;
	h->version_s = ~h->version;
	strncpy((char *)h->version_str, "AP_V3", sizeof(h->version_str));
	
	h->opcode = HS_OPCODE_WRITEDATA;
	h->opcode_s = ~h->opcode;

	h->para = 0x3;
	h->para_s = ~h->para;

	h->len = 100;
	h->len_s = ~h->para_s;

	return sizeof(*h);
}
