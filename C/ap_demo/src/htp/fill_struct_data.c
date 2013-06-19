#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "datatype.h"

static int fill_write_data_data_sleep(const struct dot_info_t * info,\
													const uint32_t *sleep_id, struct HS_PKT_OP_WRITEDATA_T *w, int len) {
   struct data_area_t *d = (void *)&w->data[0];
   struct sleep_area_t *s; 
   int i, n = 0;

   for (i = 0; i < w->data_esl_num; i++, info++) {
		d->id = info->dot_id;
		d->len = lcd_display(info, d->data, len);
		if (d->len == 0)
			return -1;
		n += d->len + offsetof(struct data_area_t, data);
		len -= d->len + offsetof(struct data_area_t, data);
		//len -= d->len;
		esl_data_dump(info, stderr, d->len);
      d = (void *)((char *)d + d->len + offsetof(struct data_area_t, data));
   }

   s = (void *)d;
   for (i = 0; i < w->sleep_esl_num; i++, s++, sleep_id++, n += sizeof(s->id))
		s->id = *sleep_id;
	return n;
}


/** Ìî³äwritedata Êý¾ÝÄÚÈÝ  */
int fill_write_data(struct HS_PKT_OP_WRITEDATA_T *w, int buf_len, const struct dot_info_t * data_ids, int data_n, \
							const uint32_t *sleep_ids, int sleep_n) {
	int n;
	w->ctrl = CTRL_DOT20_UPDATA;
	w->para = 0x0;
	w->powermode = 0x0;

	w->wakeup_id = 0x78563452;
	w->rf_ch = 200;

	w->data_esl_num = data_n;
	w->sleep_esl_num = sleep_n;

	buf_len -= offsetof(struct HS_PKT_OP_WRITEDATA_T, data);

	if ((n = fill_write_data_data_sleep(data_ids, sleep_ids, w, buf_len)) == -1)
		return 0;
	//return ((char *)&(w->sleep[w->sleep_esl_num]) - (char *)w + n);
	return (offsetof(struct HS_PKT_OP_WRITEDATA_T, data) + n);
}

int fill_header_data(struct HS_PKT_HEADER_T *h, int op_code, int para, int data_len) {
	h->version = 16;
	h->version_s = ~h->version;
	strncpy((char *)h->version_str, "V16", sizeof(h->version_str));

	h->opcode = op_code; 
	h->opcode_s = ~h->opcode;

	h->para = para;
	h->para_s = ~h->para;

	h->len = data_len;
	h->len_s = ~h->len;
	
	return sizeof(*h) + data_len;
}

