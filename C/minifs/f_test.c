#include <stdlib.h>
#include <stdio.h>
#include "mini_fs.h"

static char test_str[800];
struct  f_test_arr_t {
 	int offset;
	int len;
};

static void rand_800(char *buf, int len) {
	int i, r;	
	for (i = 0; i < len; i++) {
		r = rand();
		buf[i] = r % 52 + 'A';
		srand(r);
	}
}

static void rand_arr(struct  f_test_arr_t *arr, int len) {
	int id, r;
	for (id = FILE1; id <= FILE_ID_END; id++) {
		r = rand();
		arr[id].offset = r % fs.file[id].file_size;
		srand(r);
		r = rand();
		arr[id].len = r % fs.file[id].file_size;
		srand(r);
		if (arr[id].offset > arr[id].len)
			arr[id].len = fs.file[id].file_size - arr[id].offset;
		if (arr[id].offset + arr[id].len > fs.file[id].file_size)
			arr[id].len = fs.file[id].file_size - arr[id].offset;
	}		
}

#define TEST_COUNT  1000
f_test(void) {
	int i = TEST_COUNT, id, count = 0, failed;
  	struct  f_test_arr_t arr[FILE_ID_END + 1];
	
	printf("\n\n=================BEGIN TEST==============\n");
	while (i--) {
		rand_800(test_str, 800);
    	rand_arr(arr, FILE_ID_END + 1);
		failed = 0;
		printf("\n");
		for (id = FILE1; id <= FILE_ID_END; id++) {
			f_write(id, arr[id].offset, test_str, arr[id].len);
			printf("f_write(FILE%d, \toffset %d, \tlen %d)\t...\n", id + 1, arr[id].offset, arr[id].len);
		}
		for (id = FILE1; id <= FILE_ID_END; id++) {
			if (memcmp(f_read(id, arr[id].offset, arr[id].len), test_str,  arr[id].len) != 0) {
				fprintf(stderr, "memcmy failed. FILE%d, offset %d, len %d\n", id +1, arr[id].offset, arr[id].len);
				failed = 1;
      	} else {
      		printf("f_read(FILE%d, \toffset %d, \tlen %d)\tok\n", id + 1, arr[id].offset, arr[id].len);
      	}
		}
		if (!failed)
			count++;
	}
	printf("=====Test %d, pass %d, failed %d.=====\n\n", TEST_COUNT, count, TEST_COUNT - count);
}

#if 0
/*******************************************************
***	ÓÃ»§²ã²âÊÔº¯Êý
*******************************************************/
void f_test(void) {
	UINT8 tmp_data[20] = "this is a test line";
	//UINT8 tmp_read[20];

	//erase ²âÊÔ
	f_erase(FILE1);
	fprintf(stderr, "f_erase(FILE1) Finished.\n");
	f_erase(FILE2);
	fprintf(stderr, "f_erase(FILE2) Finished.\n");
	f_erase(FILE3);
	fprintf(stderr, "f_erase(FILE3) Finished.\n");

#if 1
	f_write(FILE1, 2, tmp_data, 5);
	fprintf(stderr, "f_write(FILE1) Finished.\n");
	if (memcmp(f_read(FILE1, 2, 5), tmp_data, 5) != 0)
		fprintf(stderr, "%s:%d erase failed.\n", __FUNCTION__, __LINE__);
	//¶ÁÐ´²âÊÔ
	fprintf(stderr, "f_write(FILE1) Begin.\n");
	f_write(FILE1, 2, tmp_data, 6);
	fprintf(stderr, "f_write(FILE1) Finished.\n");
	if (memcmp(f_read(FILE1, 2, 6), tmp_data, 6) != 0)
		fprintf(stderr, "%s:%d f_write failed.\n", __FUNCTION__, __LINE__);
	//Á¬ÐøÐ´²âÊÔ
	fprintf(stderr, "f_write continue mode test.\n");
	f_write(FILE2, 0, tmp_data, 15);
	f_write(FILE1, 9, tmp_data, 1);
	f_write(FILE2, 10, tmp_data, 10);
	if (memcmp(f_read(FILE2, 10, 10), tmp_data, 10) != 0) 
		fprintf(stderr, "%s:%d more write failed.\n", __FUNCTION__, __LINE__);
	fprintf(stderr, "f_write continue mode test comp.\n");
	//¿çÇøÐ´²âÊÔ
#endif
}

int main(void) {
	UINT8 i;
	UINT8 tmp[] = "this is a test";

	memset(DISK_MAP, 1, sizeof(DISK_MAP));
	memset(DISK, '0', sizeof(DISK));
	f_init();
	fprintf(stderr, "f_init comp.\n");
	//f_test();
	#if 1
	f_sync();
	fprintf(stderr, "f_sync comp.\n");
	//f_init();
	fprintf(stderr, "f_init comp.\n");
	for (i = 0; i < 10; i++) {
		fprintf(stderr, "1.\n");
		f_write(FILE2, 2, tmp, sizeof(tmp) -1);
		#if 1
		fprintf(stderr, "2.\n");
		f_write(FILE1, 2, (UINT8 *)"ABCDEFGABC", 8);
		fprintf(stderr, "3.\n");
		f_write(FILE3, 35, (UINT8 *)"1234567", 7);
		fprintf(stderr, "4.\n");
		f_write(FILE3, 2, (UINT8 *)"ABCDEFGABCDEFGABCDEFGABCDEFGABCDEFGABCDEFGABCDEFGABCDEFGABCDEFG", 63);
		fprintf(stderr, "5.\n");
		f_write(FILE3, 14, (UINT8 *)"ZZZ", 3);
		fprintf(stderr, "6.\n");
		f_write(FILE3, 14, (UINT8 *)"DDDDDDDDDDDDDDDDDDD", 19);
		#endif
		fprintf(stderr, "7.\n");
		f_write(FILE2, 1, (UINT8 *)"abcdefg", 7);
		fprintf(stderr, "8.\n");
		f_write(FILE3, 7 * i + i * 2, (UINT8 *)"ABCDEFG", 7);
	}
	
	f_dump();
	#endif
	return 0;
}

#else
int main(void) {
	f_init();
	f_test();
	return 0;
}
#endif
