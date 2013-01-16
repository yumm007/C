#include<stdlib.h>

const char test_str[800];
struct  f_test_arr_t {
  int offset;
	int len;
}

#define TEST_COUNT  100
f_test(void) {
	int i = TEST_COUNT, id, count = 0;
  struct  f_test_arr_t arr[FILE_ID_END + 1];
	
	while (i--) {
		//生成1个800字节的随机数
    //以文件最大长度为种子生成offset 和 len，存入数组
		for (id = FILE1; id <= FILE_ID_END; i++) {
			f_write(id, arr[id].offset, test_str, arr[id].len);
		}
		for (id = FILE1; id <= FILE_ID_END; i++) {
			if (memcmp(f_read(id,  arr[id].offset, arr[id].len), test_str,  arr[id].len) != 0)
				fprintf(stderr, "memcmy failed. FILE%d, offset %d, len %d\n", id +1, arr[id].offset, arr[id].len);
      else
        count++;
		}
		
		printf("Test %d, pass %d, failed %d.\n", TEST_COUNT, count, TEST_COUNT - count);
	}

}
