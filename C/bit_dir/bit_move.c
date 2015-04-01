#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define CACHE_SIZE		6
#define DISK_SIZE			10
#define SCREEN_HEIGHT	3

typedef struct image_t {
	uint16_t start_x;
	uint16_t start_y;
	uint16_t end_x;
	uint16_t end_y;
} image_t;

typedef uint8_t file_id_t;

static uint8_t disk_src[DISK_SIZE] = {0xf0,0xff, 0xff}, disk_dst[DISK_SIZE] = {0};

static void dump_arr(const uint8_t *arr, uint32_t n) {
	int i, k;

	fprintf(stderr, "dump arr to bit\n");
	for (i = 0; i < n; i++) {
		for (k = 0; k < 8; k++) {
			fprintf(stderr, "%c", (arr[i] & (1<< k)) ? '1' : '0');
		}
		fprintf(stderr, " ");
	}
	fprintf(stderr, "\n");
}

static void f_read(file_id_t src, uint32_t offset, uint8_t *buf, uint32_t n) {
	uint8_t *disk[] = {disk_src, disk_dst};
	memcpy(buf, disk[src] + offset, n);
}

static void f_write(file_id_t dst, uint32_t offset, const uint8_t *buf, uint32_t n) {
	uint8_t *disk[] = {disk_src, disk_dst};
	memcpy(disk[dst] + offset, buf, n);
}

static void refresh_cache(uint8_t *cache, uint32_t *cache_index, file_id_t fd, uint32_t n) {
	if (*cache_index == 0xFFFFFFFF || (n < *cache_index - CACHE_SIZE) || (n >= *cache_index)) { 
		*cache_index = n / CACHE_SIZE * CACHE_SIZE; 
		f_read(fd, *cache_index, cache, CACHE_SIZE); 
		*cache_index += CACHE_SIZE; 
	} 
}

static uint8_t get_bit(file_id_t src, uint32_t bitn, uint8_t *cache, uint32_t *cache_index) { 
	uint32_t n = bitn /8; 
	refresh_cache(cache, cache_index, src, n);
	return (cache[n % CACHE_SIZE] & (1 << (bitn % 8))) ? 1 : 0; 
} 

static void set_bit(file_id_t dst, uint32_t bitn, uint8_t val, uint8_t *cache, uint32_t *cache_index) { 
	uint32_t n = bitn /8;
	if (*cache_index != 0xFFFFFFFF && ((n < *cache_index - CACHE_SIZE) || (n >= *cache_index)))
		f_write(dst, *cache_index - CACHE_SIZE, cache, CACHE_SIZE);
	refresh_cache(cache, cache_index, dst, n);
	cache[n % CACHE_SIZE] |= (val << (bitn % 8));
} 

static uint8_t src_cache[CACHE_SIZE], dst_cache[CACHE_SIZE], bit;
static uint32_t i, src_cache_index = 0xffffffff, dst_cache_index = 0xffffffff;

static void bit_move(file_id_t dst, uint32_t dst_bitn, file_id_t src, uint32_t src_bitn, uint32_t bit_len) {
	uint32_t i;

	for (i = 0; i < bit_len; i++) {
		bit = get_bit(src, src_bitn + i, src_cache, &src_cache_index); 
		set_bit(dst, dst_bitn + i, bit, dst_cache, &dst_cache_index); 
	}
	f_write(dst, dst_cache_index - CACHE_SIZE, dst_cache, CACHE_SIZE);
}

static void xy_move(const image_t *p, file_id_t dst, file_id_t src) {
	uint32_t i, src_bitn = 0, dst_bitn, bit_len;
	src_cache_index = 0xffffffff;
	dst_cache_index = 0xffffffff;

	for (i = p->start_x; i <= p->end_x; i++, src_bitn += bit_len) {
		dst_bitn = SCREEN_HEIGHT * i + p->start_y;
		bit_len = ((p->end_y - p->start_y +1) +7 )/8*8;
		bit_move(dst, dst_bitn, src, src_bitn, bit_len);
	}
}

int main(void) {
	file_id_t src = 0, dst = 1;
	image_t p = {0,0, 2, 7};

	bit_move(dst, 8, src, 0, 40);
	dump_arr(disk_src, DISK_SIZE);
	dump_arr(disk_dst, DISK_SIZE);
	return 0;
}

