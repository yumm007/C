#include "rel_encode.h"

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

int rel8_encode(const unsigned char *in, int insize, unsigned char *out) {
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

int rel8_decode(const unsigned char *data, int len, unsigned char *out) {
	int i, j;
	int n = 0;

	for (i = 0; i < len;) {
		switch (data[i]) {
			case KEY1:	//碰到00或者ff则连续输出n个，n由下一个字节决定
			case KEY2:
				for (j = data[i+1]; j > 0; j--)
					*out++ = data[i];
				n += data[i+1];
				i += 2;
				break;
			default:
				*out++ = data[i];
				i++;
				n++;
				break;
		}
	}

	return n;
}

