#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "datatype.h"

static int fill_write_data_data_sleep(const struct dot_info_t * info,\
													const uint32_t *sleep_id, struct HS_PKT_OP_WRITEDATA_T *w, int len) {
   struct data_area_t *d = (void *)&w->data[0];
   struct sleep_area_t *s; 
   int i;

   for (i = 0; i < w->data_esl_num; i++, info++) {
		d->id = info->dot_id;
		d->len = lcd_display(info, d->data, len);
		if (d->len == 0)
			return -1;
		len -= d->len;
		esl_data_dump(info, stderr, d->len);
      d = (void *)((char *)d + d->len);
   }

   s = (void *)d;
   for (i = 0; i < w->sleep_esl_num; i++, s++, sleep_id++)
		s->id = *sleep_id;
	
	return 0;
}


/** Ìî³äwritedata Êý¾ÝÄÚÈÝ  */
int fill_write_data(struct HS_PKT_OP_WRITEDATA_T *w, int buf_len, const struct dot_info_t * data_ids, int data_n, \
							const uint32_t *sleep_ids, int sleep_n) {
	w->ctrl = 0x1;
	w->para = 0x1;
	w->powermode = 0x1;

	w->wakeup_id = 0x56000000;
	w->rf_ch = 0;

	w->data_esl_num = data_n;
	w->sleep_esl_num = sleep_n;

	buf_len -= offsetof(struct HS_PKT_OP_WRITEDATA_T, data);

	if (fill_write_data_data_sleep(data_ids, sleep_ids, w, buf_len) != 0)
		return 0;
	return ((char *)&(w->sleep[w->sleep_esl_num]) - (char *)w);
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
