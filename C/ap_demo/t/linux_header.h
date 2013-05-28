#ifndef __LINUX_HEADER_T
#define __LINUX_HEADER_T

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/select.h>
#include <sys/time.h>

typedef int8_t INT8;
typedef int16_t INT16;
typedef int32_t INT32;

typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;

typedef int SOCKET;

enum {
   HTP_OPCODE_WRITEDATA,
   HTP_OPCODE_KICKOFF,
   HTP_OPCODE_CANCAL,
   HTP_OPCODE_PING,
   HTP_OPCODE_ACK,
   HTP_OPCODE_NAK,

   HTP_OPCODE_MAX,
};



#endif
