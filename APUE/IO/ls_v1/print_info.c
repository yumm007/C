#include "ls.h"
#include <termios.h>
#include <sys/ioctl.h>

struct field_len {
	int ino_len;
	int mode_len;
	int nlink_len;
	int uid_len;
	int gid_len;
	int size_len;
	int time_len;
	int name_len;
};
static struct field_len field_max_len = {0,0,0,0,0,0,0,0,};

static void init_field_max_len(struct field_len *field) {
	field->ino_len = field->mode_len = field->nlink_len = field->uid_len = 0;
	field->gid_len = field->size_len = field->time_len = field->name_len = 0;
}

void print_buf(struct outline_t *line) {
	int i;
	for (i = field_max_len.ino_len - strlen(line->ino); i > 0; i--)
		putchar(' ');
	if (line->ino[0]) printf("%s ", line->ino);

	for (i = field_max_len.mode_len - strlen(line->mode); i > 0; i--)
		putchar(' ');
	if (line->mode[0]) printf("%s ", line->mode);

	for (i = field_max_len.nlink_len - strlen(line->nlink); i > 0; i--)
		putchar(' ');
	if (line->nlink[0]) printf("%s ", line->nlink);

	for (i = field_max_len.uid_len - strlen(line->uid); i > 0; i--)
		putchar(' ');
	if (line->uid[0]) printf("%s ", line->uid);

	for (i = field_max_len.gid_len - strlen(line->gid); i > 0; i--)
		putchar(' ');
	if (line->gid[0]) printf("%s ", line->gid);

	for (i = field_max_len.size_len - strlen(line->size); i > 0; i--)
		putchar(' ');
	if (line->size[0]) printf("%s ", line->size);

	for (i = field_max_len.time_len - strlen(line->time); i > 0; i--)
		putchar(' ');
	if (line->time[0]) printf("%s ", line->time);

	if (!isatty(1) || print_color_name(line->mode, line->name) == 0)
		printf("%s", line->name);
	for (i = 0; i < field_max_len.name_len - strlen(line->name); i++)
		putchar(' ');
}

static void print_auto_inci(int i, int len, int num) {
	struct winsize size;
	static int columns = 0;
	
	if (columns == 0) {
		if (ioctl(1, TIOCGWINSZ, (char *) &size) < 0) {
			printf("  ");
			return;
		}
		columns = size.ws_col;
	}

	if (i % (columns / len) == 0 || i == num)
		putchar('\n');
	else
		printf("  ");
}

int cmp_char(const void *data1, const void *data2) {
	struct outline_t *d1 = ((struct outline_t *) data1);
	struct outline_t *d2 = ((struct outline_t *) data2);

	return strcmp(d1->name, d2->name);
}

static void sort_buf(struct outline_t *outl, int num, int sort_by) {
	int (*cmp_fun)(const void *, const void *);

	if (sort_by == DISABLE)
		return;

	switch (sort_by) {
	case SORT_BY_NAME:
		cmp_fun = cmp_char;
		break;
	default:
		return;
		break;
	}
	qsort(outl, num, sizeof(*outl), cmp_fun);
}

void flush_line(struct outline_t *outl, int num, const struct print_f *format) {
	int i;

	sort_buf(outl, num, format->sort_flag);

	for (i = 0; i < num; i++) {
		print_buf(outl + i);
		if (format->newl_flag == ENABLE || !isatty(1))
			putchar('\n');
		else
			print_auto_inci(i + 1, field_max_len.name_len + 2, num);
	}
}

int extend_line_buf(struct outline_t **buf,int * bufsize) {
	*bufsize = (*buf == NULL) ? MAXLINE : *bufsize << 1;
	if ((*buf = realloc(*buf, (*bufsize) * sizeof(**buf))) == NULL)
		return -1;
	return 0;
}

void print_single_record(const struct stat *file_st, const struct\
	print_f *format, const char *filename, int flag) {

	char *p;
	static struct outline_t *outl = NULL;
	static int i = 0, bufsize = 0, n = 0;

	if (i == bufsize)
		if (extend_line_buf(&outl, &bufsize) == -1)
			exit(1);//return; 内存错误

	if (flag == FLUSH) {
		flush_line(outl, i, format);
		i = 0;
		bufsize = 0;
		free(outl);
		outl = NULL;
		init_field_max_len(&field_max_len);
		return;
	}

	if (is_dot_file(filename) && (format->dot_flag) == DISABLE) 
		return;
	
	print_file_info(file_st, format, outl + i);
	if (!S_ISLNK(file_st->st_mode) && (p = strrchr(filename, '/')) != NULL && \
		p - filename + 1 != strlen(filename))
		n = snprintf(outl[i].name, NAME_LEN, "%s", p + 1);
	else
		n = snprintf(outl[i].name, NAME_LEN, "%s", filename);

	if (field_max_len.name_len < n) 
		field_max_len.name_len = n;

	i++;
	return;
}

void print_file_info(const struct stat *file_st, const struct print_f *format, struct outline_t *outl) {
	int n = 0;
	n = print_ino(file_st->st_ino, format->ino_flag, outl->ino);
	if (field_max_len.ino_len < n) 
		field_max_len.ino_len = n;

	n = print_mode(file_st->st_mode, format->mode_flag, outl->mode);
	if (field_max_len.mode_len < n) 
		field_max_len.mode_len = n;

	n = print_nlink(file_st->st_nlink, format->link_flag, outl->nlink);
	if (field_max_len.nlink_len < n) 
		field_max_len.nlink_len = n;

	n = print_uid(file_st->st_uid, format->id_name_flag, outl->uid);
	if (field_max_len.uid_len < n) 
		field_max_len.uid_len = n;

	n = print_gid(file_st->st_gid, format->id_name_flag, outl->gid);
	if (field_max_len.gid_len < n) 
		field_max_len.gid_len = n;

	n = print_size(file_st->st_size, format->size_flag, outl->size);
	if (field_max_len.size_len < n) 
		field_max_len.size_len = n;

	n = print_time(file_st, format->date_flag, format->time_form, outl->time);
	if (field_max_len.time_len < n) 
		field_max_len.time_len = n;

	return;
}
