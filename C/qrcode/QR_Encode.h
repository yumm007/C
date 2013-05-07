/*
 * QR_Encode.h
 *
 *  Created on: Jan 18, 2012
 *      Author: swex
 */

#include <string.h>
#include <stdlib.h>


// Constant

// Error correction level
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3


//Data Mode
#define QR_MODE_NUMERAL		0
#define QR_MODE_ALPHABET	1
#define QR_MODE_8BIT		2
#define QR_MODE_KANJI		3


//Group version (model number)
#define QR_VERSION_S	0 // 1 ~ 9
#define QR_VERSION_M	1 // 10 ~ 26
#define QR_VERSION_L	2 // 27 ~ 40


//max possible bits resolution 177*177/8+1 = 3917

#if 0
#define MAX_ALLCODEWORD	 3706 //The maximum total number of code words
#define MAX_DATACODEWORD 2956 //Maximum data word code (version 40-L)

#define MAX_CODEBLOCK	  153 //(Including RS code word) the maximum number of block data code word
#define MAX_MODULESIZE	  177 //Maximum number of modules in a side
#define MAX_BITDATA		 3917 // finfile data
#else

#define MAX_BITS	25		//����֧�ֵ������Ԫ�� P11

#define MAX_ALLCODEWORD	 44 //��������,ԭΪ3706 P11
#define MAX_DATACODEWORD 34 //������������ԭΪ2956. P20

#define MAX_CODEBLOCK	  153 //(Including RS code word) the maximum number of block data code word
#define MAX_MODULESIZE	  MAX_BITS //Maximum number of modules in a side
#define MAX_BITDATA		  (MAX_BITS * MAX_BITS / 8 + 1) // finfile data

#endif

//Margin when drawing a bitmap
//#define QR_MARGIN	4


/////////////////////////////////////////////////////////////////////////////

typedef struct tagRS_BLOCKINFO
{
	int ncRSBlock;		//RS block number
	int ncAllCodeWord;	//The number of codewords in the block
	int ncDataCodeWord;	//The number of data code words (the number of code words - the number of RS code word)

} RS_BLOCKINFO, *LPRS_BLOCKINFO;



/////////////////////////////////////////////////////////////////////////////
//Version code-related information (model number)

typedef struct tagQR_VERSIONINFO
{
	int nVersionNo;
	int ncAllCodeWord;

	// Error correction levels (0 = L, 1 = M, 2 = Q, 3 = H)
	int ncDataCodeWord[4];	// data len

	int ncAlignPoint;	// position
	int nAlignPoint[6];	// numberof

	RS_BLOCKINFO RS_BlockInfo1[4]; // EC pos
	RS_BLOCKINFO RS_BlockInfo2[4]; // EC pos

} QR_VERSIONINFO, *LPQR_VERSIONINFO;


typedef unsigned short WORD;

typedef unsigned char BYTE;

typedef const BYTE* LPBYTE;

typedef const char* LPCSTR;

#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
int EncodeData(int nLevel, int nVersion , LPCSTR lpsSource, int sourcelen,unsigned char QR_m_data[]);

/*nLevel - уровень коррекции ошибок
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3

nVersion - версия кода 1 - 40 разрешение итд 0 - автоопределение исходя из размера

lpsSource - данные на вход
sourcelen - длинна строки 0 - автоопределение, strlen \0 оборвет.
QR_m_data - массив для записи, пишет в биты.

функция возвращает сторону квадрата.
*/
