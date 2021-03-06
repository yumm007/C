#include <stdio.h>
#include <string.h>
#include "rel_encode.h"

const unsigned char gImage_1bit_arr[1548] = { /* 0X01,0X01,0XAC,0X00,0X48,0X00, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X20,0X30,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X20,0XC0,0X00,
0X0D,0X80,0X21,0X00,0X00,0X00,0X27,0X80,0X00,0X1F,0XC0,0X7F,0X00,0X00,0X07,0XFF,
0XFF,0X00,0X12,0X40,0X7F,0X00,0X00,0X07,0XFF,0XFF,0X00,0X12,0X40,0X01,0X00,0X00,
0X02,0X6C,0X21,0X00,0X1F,0XC0,0X00,0X00,0X00,0X00,0X6E,0X43,0X00,0X09,0X80,0X00,
0X00,0X00,0X01,0X2A,0XE2,0X00,0X00,0X00,0X3E,0X00,0X00,0X01,0X29,0XA4,0X00,0X00,
0X40,0X63,0X00,0X00,0X01,0X2F,0XBC,0X00,0X00,0X40,0X41,0X00,0X00,0X01,0X3E,0XB0,
0X00,0X00,0X00,0X41,0X00,0X00,0X07,0XF9,0XFF,0X00,0X00,0X00,0X7F,0X00,0X00,0X07,
0XE9,0XFF,0X00,0X00,0X00,0X3C,0X00,0X00,0X03,0X2B,0X28,0X00,0X00,0X00,0X00,0X00,
0X00,0X01,0X2F,0X28,0X00,0X00,0X00,0X3E,0X00,0X00,0X01,0X4B,0X6C,0X00,0X0F,0X80,
0X63,0X00,0X00,0X03,0X69,0XEC,0X00,0X1B,0XC0,0X41,0X00,0X00,0X01,0X99,0XA6,0X00,
0X12,0X40,0X41,0X00,0X00,0X00,0X98,0X82,0X00,0X12,0X40,0X7F,0X00,0X00,0X00,0X08,
0X80,0X00,0X13,0XC0,0X3C,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X80,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X0B,0X80,0X00,0X00,0X04,0X00,0X00,0X0F,0X80,0X1F,
0X80,0X00,0X00,0X04,0X00,0X00,0X18,0XC0,0X15,0X80,0X00,0X00,0X04,0X00,0X00,0X10,
0X40,0X15,0X80,0X00,0X02,0X04,0X00,0X00,0X10,0X40,0X1D,0X80,0X00,0X02,0X04,0X00,
0X00,0X1F,0XC0,0X11,0X00,0X00,0X02,0X04,0X00,0X00,0X0F,0X00,0X00,0X00,0X00,0X02,
0X04,0X02,0X00,0X00,0X00,0X00,0X00,0X00,0X02,0X04,0X02,0X00,0X00,0X00,0X00,0X00,
0X00,0X02,0X04,0X03,0X00,0X00,0X00,0X00,0X00,0X00,0X02,0X3F,0XFF,0X00,0X00,0X00,
0X00,0X00,0X00,0X02,0X3F,0XFE,0X00,0X04,0X20,0X00,0X00,0X00,0X02,0X74,0X00,0X00,
0X24,0X00,0X00,0X00,0X00,0X02,0X44,0X00,0X00,0X24,0X00,0X00,0X00,0X00,0X02,0XC4,
0X00,0X00,0X24,0X00,0X00,0X00,0X00,0X03,0X84,0X00,0X00,0X24,0X00,0X00,0X00,0X00,
0X03,0X04,0X00,0X00,0X24,0X00,0X00,0X00,0X00,0X06,0X04,0X00,0X00,0X24,0X20,0X00,
0X00,0X00,0X03,0X0C,0X00,0X00,0X24,0X20,0X00,0X00,0X00,0X00,0X0C,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X06,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X18,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X03,0X98,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X02,0X38,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X02,0X68,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X03,0XD8,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X18,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XF0,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0X18,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X02,0X08,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X02,0X08,0X00,0X50,0X00,0X00,0X00,0X00,0X00,0X01,
0XF8,0X00,0XF7,0XF0,0X00,0X00,0X00,0X00,0X01,0XE0,0X00,0XF7,0XF0,0X00,0X4C,0X80,
0X00,0X00,0X00,0X00,0X55,0X50,0X00,0X6C,0X80,0X11,0X00,0X00,0X00,0XF7,0XF0,0X00,
0X6C,0X80,0X12,0X01,0X08,0X00,0XF7,0XF0,0X00,0X2C,0X00,0X16,0X81,0XF8,0X00,0X18,
0X10,0X00,0X6C,0X00,0X16,0X81,0XF8,0X00,0XFF,0XB0,0X00,0X2C,0X00,0X56,0X80,0X08,
0X00,0XF0,0X60,0X01,0X6D,0X00,0X16,0X80,0X00,0X00,0X1F,0XE0,0X00,0X6C,0X80,0X16,
0XA0,0X00,0X00,0X1F,0XB0,0X00,0X10,0X80,0X10,0X01,0X18,0X00,0X10,0X10,0X00,0X00,
0X00,0X20,0X01,0X98,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X38,0X00,0X00,0X00,
0X00,0XA2,0X00,0X00,0X00,0X68,0X00,0X85,0X20,0X00,0XA0,0X00,0X10,0X21,0XD8,0X00,
0XCD,0X20,0X00,0XB8,0X00,0X10,0X41,0X18,0X00,0X19,0X40,0X00,0X28,0X00,0X21,0X00,
0X00,0X00,0XFD,0X70,0X01,0X28,0X00,0X12,0XA1,0X90,0X00,0XFD,0XF0,0X00,0X20,0X00,
0X10,0X20,0XF0,0X00,0X27,0XB0,0X00,0X40,0X80,0X00,0X40,0XF0,0X00,0X27,0XA0,0X00,
0X00,0X80,0X48,0X80,0X90,0X00,0XFD,0XC0,0X01,0X00,0X80,0X08,0X40,0X90,0X00,0XFD,
0X20,0X00,0X00,0X00,0X08,0X21,0XFC,0X00,0X25,0X70,0X00,0X00,0X00,0X00,0X00,0X90,
0X00,0X25,0X50,0X00,0X00,0X80,0X00,0X00,0X90,0X00,0X21,0X10,0X00,0X02,0X80,0X11,
0X20,0XA0,0X00,0X00,0X00,0X00,0X02,0X80,0X1B,0X80,0X30,0X00,0X00,0X00,0X00,0X0A,
0X80,0X1B,0X00,0X00,0X00,0X00,0X00,0X00,0X8E,0X80,0X1B,0X00,0X00,0X00,0X00,0X00,
0X00,0XB2,0X80,0X11,0X20,0X00,0X00,0X00,0X00,0X00,0XA2,0X80,0X01,0X81,0X08,0X00,
0X00,0X00,0X00,0X92,0X80,0X29,0X01,0XF8,0X00,0X00,0X00,0X00,0X08,0X80,0X29,0X21,
0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X29,0X20,0X08,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X10,0X00,0X00,0X01,0X18,0X00,0X00,0X00,0X00,0X34,0X80,0X00,0X01,0X98,0X00,0X00,
0X00,0X00,0X14,0X80,0X00,0X00,0X38,0X00,0X00,0X00,0X01,0X14,0X80,0X00,0X00,0X68,
0X00,0X00,0X00,0X00,0X54,0X80,0X00,0X01,0XD8,0X00,0X00,0X00,0X00,0X34,0X80,0X00,
0X01,0X18,0X00,0X00,0X00,0X00,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X10,
0X00,0X08,0X40,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0X0C,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X4F,0XF8,0X00,0X00,0X00,0X00,0X08,0X80,0X00,0X07,0XA0,0X00,
0X00,0X00,0X00,0X10,0X00,0X00,0X05,0X20,0X00,0X00,0X00,0X00,0X30,0X00,0X10,0XC5,
0X24,0X00,0X00,0X00,0X00,0X82,0X80,0X11,0X45,0X24,0X00,0X00,0X00,0X01,0X00,0X80,
0X11,0X4F,0XFC,0X00,0X00,0X00,0X00,0X40,0X00,0X18,0X44,0X00,0X00,0X00,0X00,0X00,
0X30,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X11,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X10,0X80,0X02,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X04,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X08,0X01,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X03,0XF8,0X00,0X00,0X00,0X01,0X02,0X80,0X48,0X03,0XF8,0X00,0X00,0X00,0X01,0X52,
0X80,0X10,0X00,0X08,0X00,0X00,0X00,0X01,0X52,0X80,0X08,0X00,0X00,0X00,0X00,0X00,
0X01,0X52,0X80,0X04,0X00,0X00,0X00,0X00,0X00,0X01,0X52,0X80,0X04,0X01,0X18,0X00,
0X00,0X00,0X01,0X52,0X80,0X00,0X03,0X98,0X00,0X00,0X00,0X01,0X42,0X80,0X00,0X02,
0X38,0X00,0X00,0X00,0X01,0X00,0X00,0X00,0X22,0X68,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X03,0XD8,0X00,0X00,0X00,0X00,0X00,0X00,0X29,0X01,0X18,0X00,0X00,0X00,0X00,
0X00,0X00,0X29,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X29,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X29,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X29,0X20,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,};

const unsigned char gImage_2bit_arr[1548] = { /* 0X01,0X01,0XAC,0X00,0X48,0X00, */
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XDF,0XCF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0X3F,0XFF,
0XF2,0X7F,0XDE,0XFF,0XFF,0XFF,0XD8,0X7F,0XFF,0XE0,0X3F,0X80,0XFF,0XFF,0XF8,0X00,
0X00,0XFF,0XED,0XBF,0X80,0XFF,0XFF,0XF8,0X00,0X00,0XFF,0XED,0XBF,0XFE,0XFF,0XFF,
0XFD,0X93,0XDE,0XFF,0XE0,0X3F,0XFF,0XFF,0XFF,0XFF,0X91,0XBC,0XFF,0XF6,0X7F,0XFF,
0XFF,0XFF,0XFE,0XD5,0X1D,0XFF,0XFF,0XFF,0XC1,0XFF,0XFF,0XFE,0XD6,0X5B,0XFF,0XFF,
0XBF,0X9C,0XFF,0XFF,0XFE,0XD0,0X43,0XFF,0XFF,0XBF,0XBE,0XFF,0XFF,0XFE,0XC1,0X4F,
0XFF,0XFF,0XFF,0XBE,0XFF,0XFF,0XF8,0X06,0X00,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XF8,
0X16,0X00,0XFF,0XFF,0XFF,0XC3,0XFF,0XFF,0XFC,0XD4,0XD7,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFE,0XD0,0XD7,0XFF,0XFF,0XFF,0XC1,0XFF,0XFF,0XFE,0XB4,0X93,0XFF,0XF0,0X7F,
0X9C,0XFF,0XFF,0XFC,0X96,0X13,0XFF,0XE4,0X3F,0XBE,0XFF,0XFF,0XFE,0X66,0X59,0XFF,
0XED,0XBF,0XBE,0XFF,0XFF,0XFF,0X67,0X7D,0XFF,0XED,0XBF,0X80,0XFF,0XFF,0XFF,0XF7,
0X7F,0XFF,0XEC,0X3F,0XC3,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X7F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF4,0X7F,0XFF,0XFF,0XFB,0XFF,0XFF,0XF0,0X7F,0XE0,
0X7F,0XFF,0XFF,0XFB,0XFF,0XFF,0XE7,0X3F,0XEA,0X7F,0XFF,0XFF,0XFB,0XFF,0XFF,0XEF,
0XBF,0XEA,0X7F,0XFF,0XFD,0XFB,0XFF,0XFF,0XEF,0XBF,0XE2,0X7F,0XFF,0XFD,0XFB,0XFF,
0XFF,0XE0,0X3F,0XEE,0XFF,0XFF,0XFD,0XFB,0XFF,0XFF,0XF0,0XFF,0XFF,0XFF,0XFF,0XFD,
0XFB,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XFB,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFD,0XFB,0XFC,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XC0,0X00,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFD,0XC0,0X01,0XFF,0XFB,0XDF,0XFF,0XFF,0XFF,0XFD,0X8B,0XFF,0XFF,
0XDB,0XFF,0XFF,0XFF,0XFF,0XFD,0XBB,0XFF,0XFF,0XDB,0XFF,0XFF,0XFF,0XFF,0XFD,0X3B,
0XFF,0XFF,0XDB,0XFF,0XFF,0XFF,0XFF,0XFC,0X7B,0XFF,0XFF,0XDB,0XFF,0XFF,0XFF,0XFF,
0XFC,0XFB,0XFF,0XFF,0XDB,0XFF,0XFF,0XFF,0XFF,0XF9,0XFB,0XFF,0XFF,0XDB,0XDF,0XFF,
0XFF,0XFF,0XFC,0XF3,0XFF,0XFF,0XDB,0XDF,0XFF,0XFF,0XFF,0XFF,0XF3,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF9,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XE7,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X67,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XC7,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0X97,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFC,0X27,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XE7,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X0F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFC,0XE7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XF7,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XF7,0XFF,0XAF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,
0X07,0XFF,0X08,0X0F,0XFF,0XFF,0XFF,0XFF,0XFE,0X1F,0XFF,0X08,0X0F,0XFF,0XB3,0X7F,
0XFF,0XFF,0XFF,0XFF,0XAA,0XAF,0XFF,0X93,0X7F,0XEE,0XFF,0XFF,0XFF,0X08,0X0F,0XFF,
0X93,0X7F,0XED,0XFE,0XF7,0XFF,0X08,0X0F,0XFF,0XD3,0XFF,0XE9,0X7E,0X07,0XFF,0XE7,
0XEF,0XFF,0X93,0XFF,0XE9,0X7E,0X07,0XFF,0X00,0X4F,0XFF,0XD3,0XFF,0XA9,0X7F,0XF7,
0XFF,0X0F,0X9F,0XFE,0X92,0XFF,0XE9,0X7F,0XFF,0XFF,0XE0,0X1F,0XFF,0X93,0X7F,0XE9,
0X5F,0XFF,0XFF,0XE0,0X4F,0XFF,0XEF,0X7F,0XEF,0XFE,0XE7,0XFF,0XEF,0XEF,0XFF,0XFF,
0XFF,0XDF,0XFE,0X67,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC7,0XFF,0XFF,0XFF,
0XFF,0X5D,0XFF,0XFF,0XFF,0X97,0XFF,0X7A,0XDF,0XFF,0X5F,0XFF,0XEF,0XDE,0X27,0XFF,
0X32,0XDF,0XFF,0X47,0XFF,0XEF,0XBE,0XE7,0XFF,0XE6,0XBF,0XFF,0XD7,0XFF,0XDE,0XFF,
0XFF,0XFF,0X02,0X8F,0XFE,0XD7,0XFF,0XED,0X5E,0X6F,0XFF,0X02,0X0F,0XFF,0XDF,0XFF,
0XEF,0XDF,0X0F,0XFF,0XD8,0X4F,0XFF,0XBF,0X7F,0XFF,0XBF,0X0F,0XFF,0XD8,0X5F,0XFF,
0XFF,0X7F,0XB7,0X7F,0X6F,0XFF,0X02,0X3F,0XFE,0XFF,0X7F,0XF7,0XBF,0X6F,0XFF,0X02,
0XDF,0XFF,0XFF,0XFF,0XF7,0XDE,0X03,0XFF,0XDA,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0X6F,
0XFF,0XDA,0XAF,0XFF,0XFF,0X7F,0XFF,0XFF,0X6F,0XFF,0XDE,0XEF,0XFF,0XFD,0X7F,0XEE,
0XDF,0X5F,0XFF,0XFF,0XFF,0XFF,0XFD,0X7F,0XE4,0X7F,0XCF,0XFF,0XFF,0XFF,0XFF,0XF5,
0X7F,0XE4,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X71,0X7F,0XE4,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X4D,0X7F,0XEE,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5D,0X7F,0XFE,0X7E,0XF7,0XFF,
0XFF,0XFF,0XFF,0X6D,0X7F,0XD6,0XFE,0X07,0XFF,0XFF,0XFF,0XFF,0XF7,0X7F,0XD6,0XDE,
0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XD6,0XDF,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XEF,0XFF,0XFF,0XFE,0XE7,0XFF,0XFF,0XFF,0XFF,0XCB,0X7F,0XFF,0XFE,0X67,0XFF,0XFF,
0XFF,0XFF,0XEB,0X7F,0XFF,0XFF,0XC7,0XFF,0XFF,0XFF,0XFE,0XEB,0X7F,0XFF,0XFF,0X97,
0XFF,0XFF,0XFF,0XFF,0XAB,0X7F,0XFF,0XFE,0X27,0XFF,0XFF,0XFF,0XFF,0XCB,0X7F,0XFF,
0XFE,0XE7,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,
0XFF,0XF7,0XBF,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0XF3,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XB0,0X07,0XFF,0XFF,0XFF,0XFF,0XF7,0X7F,0XFF,0XF8,0X5F,0XFF,
0XFF,0XFF,0XFF,0XEF,0XFF,0XFF,0XFA,0XDF,0XFF,0XFF,0XFF,0XFF,0XCF,0XFF,0XEF,0X3A,
0XDB,0XFF,0XFF,0XFF,0XFF,0X7D,0X7F,0XEE,0XBA,0XDB,0XFF,0XFF,0XFF,0XFE,0XFF,0X7F,
0XEE,0XB0,0X03,0XFF,0XFF,0XFF,0XFF,0XBF,0XFF,0XE7,0XBB,0XFF,0XFF,0XFF,0XFF,0XFF,
0XCF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XEF,0X7F,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFE,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFC,0X07,0XFF,0XFF,0XFF,0XFE,0XFD,0X7F,0XB7,0XFC,0X07,0XFF,0XFF,0XFF,0XFE,0XAD,
0X7F,0XEF,0XFF,0XF7,0XFF,0XFF,0XFF,0XFE,0XAD,0X7F,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFE,0XAD,0X7F,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XAD,0X7F,0XFB,0XFE,0XE7,0XFF,
0XFF,0XFF,0XFE,0XAD,0X7F,0XFF,0XFC,0X67,0XFF,0XFF,0XFF,0XFE,0XBD,0X7F,0XFF,0XFD,
0XC7,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XDD,0X97,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFC,0X27,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XD6,0XFE,0XE7,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XD6,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XD6,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XD6,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XD6,0XDF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,};


const unsigned char zhenzi1bit[1548 + 8] = { /* 0X01,0X01,0XAC,0X00,0X48,0X00, */
0X00,0X00,0X00,0X00, 0X08, 0X00, 0XAB, 0X00,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0XCF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,
0XBF,0XF8,0X0F,0XF8,0X70,0XFF,0XFF,0XFF,0XBC,0X7F,0XFF,0X7F,0XE0,0X20,0X3F,0X7F,
0XFF,0XA1,0XFF,0XFF,0X7F,0XE0,0X00,0X3E,0X07,0XF0,0X00,0X00,0X7F,0X7F,0XC3,0X06,
0X1F,0XFF,0XF7,0XB7,0XFF,0XF8,0X0F,0XC7,0X8F,0X1F,0X0F,0XFF,0X39,0XDF,0X7F,0XFF,
0XC7,0X8F,0X1E,0XF7,0XFF,0XB1,0XBE,0XFF,0XFF,0XC7,0X8F,0X1F,0X0F,0XFD,0XF7,0X5E,
0XFF,0XCF,0XC3,0X06,0X1F,0XFF,0XFD,0XB6,0XDD,0XFE,0XAF,0XE0,0X00,0X3F,0X0F,0XFD,
0XB5,0X5B,0XFE,0XAF,0XE0,0X20,0X3E,0XF7,0XFD,0XA3,0X47,0XFE,0X0F,0XF8,0X70,0XFF,
0X0F,0XFD,0X0F,0X4F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X36,0X00,0X7E,0XEF,0XFF,
0XFF,0XFF,0X89,0XF5,0XB6,0XDF,0XFE,0XCF,0XFF,0XFF,0X1F,0X75,0XFD,0XB4,0XD7,0XFE,
0XAF,0XFF,0XFF,0X1F,0X75,0XFD,0XB0,0XDB,0XFE,0X6F,0XFF,0XFF,0X1F,0X01,0XFD,0X34,
0X9B,0XFE,0XEF,0XFF,0XFF,0XFF,0XFF,0XF9,0XB6,0X59,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFD,0XEF,0X7C,0XFF,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0X7E,0XFE,0XAF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X7F,0XFE,0XAF,0XFC,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X2F,
0XF0,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0X3F,0XFF,0XFF,0XFB,0XFF,
0XFF,0XFF,0XE3,0X8E,0X1F,0XFF,0XFF,0XFB,0XFF,0XF8,0X0F,0XC7,0X1F,0X1F,0XFF,0XFF,
0XFB,0XFF,0XFF,0XFF,0XC7,0X1F,0X1F,0XFF,0XFB,0XFB,0XFF,0XFF,0XFF,0XC7,0X1F,0X1F,
0XFF,0XFB,0XFB,0XFF,0XFE,0X0F,0XC3,0X0E,0X1F,0XFF,0XFB,0XFB,0XFF,0XFE,0XFF,0XE1,
0X80,0X3F,0XFF,0XFB,0XFB,0XFF,0XFE,0XFF,0XE1,0X80,0X7F,0XFF,0XFB,0XFB,0XFE,0XFF,
0X0F,0XF9,0XE0,0XFF,0XFF,0XFB,0XFB,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XFB,
0XFE,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0X80,0X00,0X7E,0X1F,0XFC,0X01,0XFF,0XFF,
0XFB,0XC0,0X00,0XFF,0XEF,0XF0,0X00,0X7F,0XFF,0XFB,0XBB,0XFF,0XFF,0XEF,0XE0,0X00,
0X3F,0XFF,0XFB,0X7B,0XFF,0XFE,0X0F,0XC3,0XFE,0X1F,0XFF,0XFB,0X7B,0XFF,0XFF,0XFF,
0XC7,0XFF,0X1F,0XFF,0XFA,0XFB,0XFF,0XFE,0XFF,0XC7,0XFF,0X1F,0XFF,0XF9,0XFB,0XFF,
0XF8,0X0F,0XC7,0XFF,0X1F,0XFF,0XF1,0XFB,0XFF,0XFE,0XEF,0XC3,0XFE,0X1F,0XFF,0XFB,
0XFB,0XFF,0XFF,0XFF,0XE0,0X00,0X3F,0XFF,0XFF,0XF3,0XFF,0XFF,0XFF,0XF0,0X00,0X7F,
0XFF,0XFF,0XF3,0XFF,0XFF,0XFF,0XFC,0X01,0XFF,0XFF,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFD,0X7F,0XFF,0XAD,0XF3,0X9F,0XBE,0XFC,0XE7,0XF8,0X40,0X7A,0XAD,0XEF,
0X5F,0X00,0XFB,0XD7,0XFD,0X55,0XEA,0XAB,0XEE,0XDF,0XFE,0XFB,0XB7,0XFD,0X55,0X72,
0XAB,0XED,0XDF,0XFF,0XFB,0X77,0XF8,0X40,0X7A,0XA7,0XF3,0XDF,0XFF,0XFC,0XF7,0XFD,
0X7F,0XF8,0X0F,0XFF,0XFF,0X9C,0XFF,0XFF,0XFF,0XBF,0X7A,0XA7,0XF0,0X3F,0X7A,0XFF,
0XFF,0XF8,0X42,0XF2,0XAB,0XEF,0XDF,0X76,0XFD,0XF7,0XFF,0X7D,0XEA,0XAB,0XEF,0XDF,
0X6E,0XF8,0X07,0XFF,0X02,0XFA,0XAD,0XEF,0XDF,0X9E,0XFF,0XF7,0XFF,0X7F,0X7F,0XAD,
0XF0,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XFF,0XFF,0XFB,0XD6,
0XE0,0X01,0XFF,0XFF,0XEF,0XFC,0X0F,0XFD,0XB6,0XEF,0XEB,0XF7,0XDF,0XDF,0XFB,0XF7,
0XFF,0X75,0XEB,0X6B,0XE0,0X1F,0XBF,0XFB,0XF7,0XF8,0X14,0X6B,0X6B,0XFF,0XDF,0X7F,
0XFB,0XF7,0XFE,0XF3,0X6B,0X6B,0XFF,0XFC,0XC0,0X3C,0X0F,0XFE,0XC6,0XE8,0X0B,0XFF,
0XFF,0X7F,0XFF,0XFF,0XFE,0XD3,0XEB,0X6B,0XF3,0X9F,0XBF,0XFC,0XE7,0XF8,0X15,0XEB,
0X2B,0XEF,0X5F,0XDF,0XFB,0XD7,0XFE,0XD6,0XEB,0X4B,0XEE,0XDF,0XEF,0XFB,0XB7,0XFE,
0XD5,0X6F,0XEB,0XED,0XDF,0XEF,0XFB,0X77,0XFE,0XF7,0X60,0X01,0XF3,0XDF,0XFF,0XFC,
0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XBF,
0XFE,0X7C,0XE7,0XFF,0XFF,0XFF,0XFF,0XF7,0XBE,0X01,0XFB,0XD7,0XFF,0XFF,0XFF,0XFF,
0X8C,0X3E,0XDB,0XFB,0XB7,0XFF,0XFF,0XFF,0XFF,0XED,0XBE,0XDB,0XFB,0X77,0XFF,0XFF,
0XFF,0XFF,0XED,0XBE,0XDB,0XFC,0XF7,0XFF,0XFF,0XFF,0XFF,0XED,0XBE,0XDB,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XE0,0X06,0XDB,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XED,0XBE,0XDB,
0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XED,0XBE,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XED,
0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF7,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X1F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF3,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0X5F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XEE,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XED,0XDF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF3,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XCF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XDB,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDB,0X7F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XDB,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDB,0X7F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XDB,0X77,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDB,0X77,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XDF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XE0,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF3,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XEF,0X5F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEE,0XDF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XED,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF3,0XDF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XDD,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDD,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XDD,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDD,0XEF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XDD,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDD,0XEF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDD,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,
0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};

extern const unsigned char tiaoma[4736+8];
extern const unsigned char putaojiu[4736];

#define BUFSIZE 40960
static unsigned char imgcode[BUFSIZE];
static unsigned char imgdecode[BUFSIZE];
int main(void) {
	int i, len, len1;
#if 0
	len = rel8_encode(gImage_1bit_arr, sizeof(gImage_1bit_arr), imgcode);
	printf("encode %ld to %d bytes\n", sizeof(gImage_1bit_arr), len);
	len1 = rel8_decode(imgcode, len, imgdecode);
	printf("decode %d to %d bytes, memcmp %s.\n", len, len1, \
		memcmp(gImage_1bit_arr, imgdecode, len) == 0 ? "success": "failed");

	len = rel8_encode(gImage_2bit_arr, sizeof(gImage_2bit_arr), imgcode);
	printf("encode %ld to %d bytes\n", sizeof(gImage_2bit_arr), len);
	len1 = rel8_decode(imgcode, len, imgdecode);
	printf("decode %d to %d bytes, memcmp %s.\n", len, len1, \
		memcmp(gImage_2bit_arr, imgdecode, len) == 0 ? "success": "failed");


	len = rel8_encode(zhenzi1bit, sizeof(zhenzi1bit), imgcode);
	fwrite(imgcode, 1, len, stderr);
#else
	len = rel8_encode(tiaoma, 4736+8, imgcode);
	printf("const unsigned char tiaoma_rel8[%d]={", len);
	for (i = 0; i < len; i++) {
		if (i % 16 == 0)
			printf("\n");
		printf("0x%02X,", imgcode[i]);
	}
	printf("};\n");
#endif
	return 0;
}

