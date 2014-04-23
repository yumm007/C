#include "main.h"

static const struct _font_bit_size font_bit_size[] = {
{12, "12.bin", FONT_12_DATA_SIZE},
{16, "16.bin", FONT_16_DATA_SIZE},
{20, "20.bin", FONT_20_DATA_SIZE},
{24, "24.bin", FONT_24_DATA_SIZE},
};

static uint8_t _get_font_size(uint16_t unicode, const struct _font_bit_size *f_z, uint32_t *width, uint32_t *height)
{
	FILE	*font_file;
	font_file = fopen(f_z->fn, "rb");
	fseek(font_file, (uint32_t)(FILE_INSIDE_FONT_SIZE*unicode), SEEK_SET);
	fread(width, 1, 1, font_file);
	*width = *width&0x000000FF;

	//fseek(font_file, (uint32_t)(FILE_INSIDE_FONT_SIZE*unicode + 1), SEEK_SET);
	fread(height, 1, 1, font_file);
	*height = *height&0x000000FF;

	fclose(font_file);

	return 0;
}

static int _select_font(uint8_t size, const struct _font_bit_size *f_z, int num)
{
	int sub = 0;
	int i = 0;
	int find = 0;

	for (i = 0; i < num; i++){
		//compare font size
		if (size == f_z[i].font_s){
			sub = i;
			find = 1;
			goto done;
		}
	}

	if (find == 0)
		return -1;

done:
	return sub;
}

static void _get_font_data(uint16_t unicode, const struct _font_bit_size *f_z, char	*data)
{
	FILE *font_file;
	char buf[FILE_INSIDE_FONT_SIZE-2];	//remove width and height
	int i = 0;
	int j = 0;

	font_file = fopen(f_z->fn, "rb");
	//skip width and height
	fseek(font_file, (uint32_t)(FILE_INSIDE_FONT_SIZE*unicode + 2), SEEK_SET);
	fread(buf, sizeof(buf), 1, font_file);
	fclose(font_file);

	memset(data, 0, MAX_FONT_DATA_SIZE);

	switch (f_z->font_s) {
		case 12:
			for (i = 0, j = 0; j < (FONT_12_DATA_SIZE - 2); i++) {
				if ((i % 4) < 2) {
					data[j] = buf[i];
					j++;
				}
			}
			break;
		case 16:
			for (i = 0, j = 0; j < FONT_16_DATA_SIZE; i++) {
				if ((i % 4) < 2) {
					data[j] = buf[i];
					j++;
				}
			}
			break;
		case 20:
			for (i = 0, j = 0; j < (FONT_20_DATA_SIZE - 3); i++) {
				if ((i % 4) < 3) {
					data[j] = buf[i];
					j++;
				}
			}
			break;
		case 24:
			for (i = 0, j = 0; j < FONT_24_DATA_SIZE; i++) {
				if ((i % 4) < 3) {
					data[j] = buf[i];
					j++;
				}
			}
			break;
	}
}

static void _print_2(char val2)
{
	int i = 0;

	for (i = 7; i >= 0; i--) {
		if (val2&(1<<i)) {
			fprintf(stderr, "**");
		}
		else {
			fprintf(stderr, "  ");
		}
	}
}

static void _print_out(const struct _font_bit_size *f_z, const char *data)
{
	int i = 0;

	switch (f_z->font_s) {
		case 12:
			fprintf(stderr, "bit map:\n");
			for (i = 0; i < f_z->data_s; i++) {
				if ((i % 2) == 0) {
					_print_2(data[i]);
				}
				else {
					_print_2(data[i]);
					fprintf(stderr, "\n");
				}
			}
			break;
		case 16:
			fprintf(stderr, "bit map:\n");
			for (i = 0; i < f_z->data_s; i++) {
				if ((i % 2) == 0) {
					_print_2(data[i]);
				}
				else {
					_print_2(data[i]);
					fprintf(stderr, "\n");
				}
			}
			break;
		case 20:
			fprintf(stderr, "bit map:\n");
			for (i = 0; i < f_z->data_s; i++) {
				if ((i % 3) != 2) {
					_print_2(data[i]);
				}
				else {
					_print_2(data[i]);
					fprintf(stderr, "\n");
				}
			}
			break;
		case 24:
			fprintf(stderr, "bit map:\n");
			for (i = 0; i < f_z->data_s; i++) {
				if ((i % 3) != 2) {
					_print_2(data[i]);
				}
				else {
					_print_2(data[i]);
					fprintf(stderr, "\n");
				}
			}
			break;
	}
}
static void _read_data(uint8_t *size, uint16_t *unicode)
{
	char buf[10] = {0};
	int read_size;
	unsigned int read_unicode;

	memset(buf, 0, sizeof(buf));
	fprintf(stderr, "\ninput font size (12, 16, 20, 24):");
	fgets(buf, 10, stdin);
	sscanf(buf, "%d", &read_size);
	*size = (uint8_t)read_size;

	memset(buf, 0, sizeof(buf));
	fprintf(stderr, "input font unicode (0400 -- 052F):");
	fgets(buf, 10, stdin);
	sscanf(buf, "%x", &read_unicode);
	*unicode = (uint16_t)read_unicode;
}

int old_main(int argc, char **argv)
{
	uint8_t font_size = 24;
	uint16_t font_unicode = 0x040d;
	uint32_t char_width, char_height;
	char font_data[MAX_FONT_DATA_SIZE];
	int font_num;
	int find_error = 0;

	while(1){
		_read_data(&font_size, &font_unicode);
		if ((font_unicode < 0x0400) || (font_unicode > 0x052f)) {
			find_error = 1;			
			goto error;
		}
		//select font size and *.bin path
		font_num = _select_font(font_size, font_bit_size, sizeof(font_bit_size)/sizeof(font_bit_size[0]));
		if (font_num == -1) {
			find_error = 1;
			goto error;
		}
		//get font width and height (not necessary! why?)
		_get_font_size(font_unicode, &font_bit_size[font_num], &char_width, &char_height);

		//get font data
		_get_font_data(font_unicode, &font_bit_size[font_num], font_data);

		_print_out(&font_bit_size[font_num], font_data);
error:
		if (find_error == 1) {
			fprintf(stderr, "\nError!\n");
			find_error = 0;
		}
		else {
			fprintf(stderr, "\nPass!\n");
		}
	}
	return 0;
}

int main(int argc, char **argv) {
	char font_data[MAX_FONT_DATA_SIZE];
	int font_size = atoi(argv[1]);
	char *unicode = argv[2];
	int font_num, c, i;
	uint32_t char_width, char_height;
	uint8_t read_buf[4096];

	memset(read_buf, 0, 4096);
	for (i = 0; c != EOF; i++) {
		c = getchar()
		read_buf[i] = c;
	}


	while ((c = getchar()) != EOF)
		unicode


	while (*unicode != '\0') {
		font_num = _select_font(font_size, font_bit_size, sizeof(font_bit_size)/sizeof(font_bit_size[0]));
		_get_font_size(*(uint16_t *)unicode, &font_bit_size[font_num], &char_width, &char_height);
	 	_get_font_data(*(uint16_t *)unicode, &font_bit_size[font_num], font_data);
		_print_out(&font_bit_size[font_num], font_data);
		unicode +=2;
	}
}

