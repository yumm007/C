#include <stdio.h>
#include <stdint.h>

#define KEY1 0xff
#define KEY2 0x00

//输出若干个0x00或者0xff
//采用宏的方式替代函数，因为要同时修改好几个值
#define out_00ff do {\
						if (find_key1 || find_key2) {\
							*out = find_key1 ? KEY1 : KEY2;\
							out++;\
							*out = find_c;\
							out++;\
							n += 2;\
							find_c = 0;\
							find_key1 = find_key2 = 0;\
						}\
					} while(0);

static int rel8_encode(const unsigned char *in, int insize, unsigned char *out) {
	//连续找到find_c个0x00或者0xff, 只使用1个字节，即最大能表示找到256个相同的字符
	unsigned char find_c = 0;
	int find_key1 = 0, find_key2 = 0; //找到0x00或者找到0xff标志
	int n = 0;

	while (insize--) {
		switch (*in) {
			case KEY1:
			case KEY2:
				if (*in == KEY1) {
					if (find_key2)	//如果上个字符是0xff
						out_00ff;
					find_key1 = 1;
				} else if (*in == KEY2) {
					if (find_key1)	//如果上个字符是0x00
						out_00ff;
					find_key2 = 1;
				}

				find_c++;
				if (find_c == 0)
					out_00ff;	//find_c计数超过255则截断,重新计数
				in++;
				break;
			default:
				out_00ff;
				*out = *in;
				in++;
				out++;
				n += 1;
				break;
		}
	}
	out_00ff;
	return n;
}

static uint16_t cal_crc(const unsigned char *data, int len) {
   unsigned short crc = 0;
   int i, j;
   
   for (j = 0; j < len; j++) {
      for(i = 0x80; i != 0; i /= 2) {
         crc *= 2;
         if((crc & 0x8000) != 0)
            crc ^= 0x1021;
         if((data[j] & i) != 0)  
            crc ^= 0x1021;
      }    
   }   
	fprintf(stderr, "crc %d bytes = %02X.\n", len, crc);
	return crc;
}

#define MAX_CONTENT_LEN	1024*1024	//1M

#pragma pack(1)
struct protocal_t {
	uint16_t crc;
	uint32_t size;
	uint8_t	flag;
	uint8_t	content[MAX_CONTENT_LEN];
};
#pragma pack()

int protocal_data(const uint8_t *content, int len) {
	struct protocal_t prt_data;

	//while (len < MAX_CONTENT_LEN && (c = getchar()) != EOF)
	//	content[len++] = c;

	fprintf(stderr, "read %d bytes.\n", len);

	len = rel8_encode(content, len, prt_data.content);

	fprintf(stderr, "encode %d bytes.\n", len);

	prt_data.size = len + sizeof(prt_data.size) + sizeof(prt_data.flag);
	prt_data.flag = 0x40;	//REL-8压缩格式
	prt_data.crc = cal_crc((void *)&prt_data.size, prt_data.size);

	fwrite(&prt_data, 1, prt_data.size + sizeof(prt_data.crc), stdout);

	return 0;
}
