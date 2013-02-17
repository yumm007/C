#ifndef __REL_ENCODE_H__
#define __REL_ENCODE_H__

/*	压缩算法简介
** 将多个连续的0x00或者0xff表示为2个字节，第一个字节为0x00或者0xff，
** 第二个字节为0x00或者0xff连续出现的次数。
** 比如需要压缩的数据流为 "0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x34"
**	则压缩后的数据流为"0x00, 0x03, 0xfe, 0xff, 0x02, 0x34"
**	当输入流中存在大量的ox00或者0xff时，压缩比率越高
*/

#define KEY1	0x00	//	0x00需要压缩
#define KEY2	0xff	//	0xff需要压缩

int rel8_encode(const unsigned char *in, int insize, unsigned char *out); 
int rel8_decode(const unsigned char *data, int len, unsigned char *out);

#endif
