#ifndef __WINDOWS_HEADER_T
#define __WINDOWS_HEADER_T

extern "C" 

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "cstdlib"

#include <WINSOCK2.H> 
#pragma comment(lib,"ws2_32.lib") 


//typedef __int8  INT8;
typedef __int16  INT16;
typedef __int32  INT32;

typedef unsigned __int8 UINT8;
typedef unsigned __int16 UINT16;
typedef unsigned __int32 UINT32;

#endif