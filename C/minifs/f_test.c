#include<stdlib.h>

static char test_str[800];
struct  f_test_arr_t {
  UINT16 offset;
	UINT16 len;
}

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
	for (id = FILE1; id <= FILE_ID_END; i++) {
		r = rand();
		arr[id].offset = r % fs.info.file_size;
		srand(r);
		r = rand();
		arr[id].len = r % fs.info.file_size;
		srand(r);
		if (arr[id].offset > arr[id].len)
			arr[id].len = arr[id].offset;
	}		
}

#define TEST_COUNT  100
f_test(void) {
	int i = TEST_COUNT, id, count = 0;
  struct  f_test_arr_t arr[FILE_ID_END + 1];
	
	printf("\n\n=================BEGIN TEST==============\n);
	while (i--) {
		rand_800(test_str, 800);//生成1个800字节的随机数
    rand_arr(&arr, FILE_ID_END + 1);//以文件最大长度为种子生成offset 和 len，存入数组
		for (id = FILE1; id <= FILE_ID_END; i++) {
			f_write(id, arr[id].offset, test_str, arr[id].len);
			printf("f_write(FILE$d, offset %d, len %d)\n", id + 1, arr[id].offset, arr[id].len);
		}
		for (id = FILE1; id <= FILE_ID_END; i++) {
			if (memcmp(f_read(id, arr[id].offset, arr[id].len), test_str,  arr[id].len) != 0)
				fprintf(stderr, "memcmy failed. FILE%d, offset %d, len %d\n", id +1, arr[id].offset, arr[id].len);
      else {
      	printf("f_read(FILE$d, offset %d, len %d)\n", id + 1, arr[id].offset, arr[id].len);
        count++;
      }
		}
	}
	printf("=====Test %d, pass %d, failed %d.=====\n\n", TEST_COUNT, count, TEST_COUNT - count);
}
