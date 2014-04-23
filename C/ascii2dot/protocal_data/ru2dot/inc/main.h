#ifndef _MAIN_H
#define _MAIN_H

#include	<stdio.h>
#include	<stdlib.h>
#include <stdint.h>
#include	<string.h>

#define FONT_24_DATA_SIZE 72
#define FONT_20_DATA_SIZE 60
#define FONT_16_DATA_SIZE 32
#define FONT_12_DATA_SIZE 24

#define FILE_INSIDE_FONT_SIZE 98
#define MAX_FONT_DATA_SIZE FONT_24_DATA_SIZE

struct _font_bit_size {
	uint8_t font_s;	//font size
	const char *fn;	//font lib path
	uint8_t data_s;	//data size
};

#endif
