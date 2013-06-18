#include <stdio.h>
#include <stddef.h>
#include "datatype.h"

static const char *VER_STR[] = {
   "HS_OPCODE_WRITEDATA",
   "HS_OPCODE_KICKOFF",
   "HS_OPCODE_CANCAL",
   "HS_OPCODE_PING",
   "HS_OPCODE_ACK",
   "HS_OPCODE_NACK",
};

static void data_sleep_dump(FILE *fp, const struct HS_PKT_OP_WRITEDATA_T *w) {
	struct data_area_t *d = (void *)&w->data[0];
	struct sleep_area_t *s;
	int i;

	fprintf(fp, ":data_esl_data[");
	for (i = 0; i < w->data_esl_num; i++) {
		fprintf(fp, "ID=0x%08X,LEN=%d,CRC=0x%04X,CRC_LEN=%d;", \
					d->id, d->len, *((uint16_t *)d->data), *(((uint16_t *)d->data)+1));
		d = (void *)((char *)d + d->len + offsetof(struct data_area_t, data));
	}

	s = (void *)d;
	fprintf(fp, "]");
	fprintf(fp, ":sleep_esl_data[");
	for (i = 0; i < w->sleep_esl_num; i++, s++)
		fprintf(fp, "ID=0x%08X;", s->id);
	fprintf(fp, "]");
}

void write_dump(const HS_PKT_T *pkg, FILE *fp) {
	const struct HS_PKT_HEADER_T *h = &pkg->header;
	const struct HS_PKT_OP_WRITEDATA_T *w = &pkg->buf.write;

	fprintf(fp, "version=%d:ver-str=%s:opcode=%s:para=%d:len=%d", \
					h->version, h->version_str, h->opcode >= HS_OPCODE_MAX ? "wrong opcode" : VER_STR[h->opcode], \
					h->para, h->len);
	
	switch (h->opcode) {
		case HS_OPCODE_WRITEDATA:
			fprintf(fp, ":CTRL=%d:PARA=%d:POWERMODE=%d:WKUP_ID=0x%08X:RF_CH=%d:DATA_ESL_NUM=%d:SLEEP_ESL_NUM=%d",\
						w->ctrl, w->para, w->powermode, w->wakeup_id, w->rf_ch, w->data_esl_num, w->sleep_esl_num);
			data_sleep_dump(fp, w);
			break;
		case HS_OPCODE_KICKOFF:
			break;
		case HS_OPCODE_CANCAL:
			break;
		case HS_OPCODE_PING:
			break;
		case HS_OPCODE_ACK:
			break;
		case HS_OPCODE_NACK:
			break;
		default:
			fprintf(fp, ":unknown opcode %d", h->opcode);
			break;
	}

	fprintf(fp, "\n");
}

void esl_data_dump(const struct dot_info_t *info, FILE *fp, int len) {
	fprintf(fp, "DOT_TYPE = %0.2f\t Price = %0.2f\t CRC_LEN = %d\t NAME = %s\t Origin = %s\n",\
				info->type /10.0, info->price, len, info->name, info->origin);
}

