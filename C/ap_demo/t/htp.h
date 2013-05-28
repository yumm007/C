#ifndef _HTP_H_
#define _HTP_H_

#define IP_ADDRESS_LEN	16
#define VERSION_LEN		8

#define HTP_VERSION 16
#define HTP_VERSION_S "16"

typedef struct htp_socket_t
{
	SOCKET socket; // 网络通信套接字
	char	ip_addr[IP_ADDRESS_LEN]; // IP地址
	UINT16 port; // 端口号
	UINT8 *buf; // 收发数据的首地址
	UINT32 len; // 收发数据的长度
} htp_socket_t;

typedef struct htp_header_t
{
	UINT16 version;
	UINT16 version_s;
	char vsrt[VERSION_LEN];
	UINT16 opcode;
	UINT16 opcode_s;
	UINT16 para;
	UINT16 para_s;
	UINT32 len;
	UINT32 len_s;
	UINT16 reserved;
	UINT16 reserved_s;
} htp_header_t;

/*
 * 功能：创建一个指定服务器IP的socket连接（TCP/IP）
 * 参数：ip_addr --> 指定服务器的IP地址
 *		 port    --> 指定服务器的端口
 *		 socket  --> 成功连接的socket句柄
 * 返回：0  --> 成功
 *		 -1 --> socket创建失败
 *		 -x --> connet连接失败,其中x=errno
 */
INT32 htp_open(htp_socket_t *htp_socket);

/*
 * 功能：接收指定长度的数据
 * 参数：socket --> 需要发送的socket句柄
 *		 dst    --> 发送数据的首地址
 *		 len    --> 发送数据的长度
 *		 timeout --> 超时时间,单位 ms
 * 返回：>0 --> 成功接收的数据个数
 *		 0  --> socket连接断开
 *		 -1 --> 接收数据错误
 */
INT32 read_socket(SOCKET socket, void* dst, INT32 len, UINT32 timeout);

/*
 * 功能：发送指定长度的数据
 * 参数：socket --> 需要发送的socket句柄
 *		 src    --> 发送数据的首地址
 *		 len    --> 发送数据的长度
 * 返回：>0 --> 成功发送的数据个数
 *		 0  --> socket连接断开
 *		 -1 --> 发送数据错误
 */
INT32 write_socket(SOCKET socket, const void* src, INT32 len);

/*
 * 功能：向指定的socket发送数据(先发htp_header，再发htp_socket->buf)
 * 参数：htp_socket->socket --> 需要发送的socket句柄
 *		 htp_socket->buf    --> 发送数据的首地址
 *		 htp_socket->len    --> 发送数据的长度
 *		 htp_header			--> htp包的数据头
 * 返回：TRUE  --> 发送成功
 *		 FALSE --> 发送失败
 */
bool htp_send(htp_socket_t *htp_socket, htp_header_t *htp_header);

/*
 * 功能：接收指定的socket数据(将接收成功的htp数据头存在htp_header和数据体存在htp_socket->buf中)
 * 参数：htp_socket->socket --> 指定接收的socket句柄
 *		 htp_socket->buf    --> 接收数据的首地址,内部分配空间
 *		 htp_socket->len    --> 接收数据的长度
 *		 htp_header			--> htp包的数据头
 * 返回：TRUE  --> 接收成功(调用者释放)
 *		 FALSE --> 接收失败(内部释放)
 */
bool htp_recv(htp_socket_t *htp_socket, htp_header_t *htp_header);

/*
 * 功能：关闭指定服务器IP的socket连接（TCP/IP）
 * 参数：socket  --> 需要关闭的socket句柄
 * 返回：TRUE  --> 关闭成功
 *		 FALSE --> 关闭失败
 */
bool htp_close(htp_socket_t *htp_socket);

/*
 * 功能：组htp包头
 * 参数：opcode --> htp包的命令字
 *		 para	--> htp包命令字的参数,默认为0
 *		 len	--> htp包数据体的长度,默认为0
 * 返回：数据头的首地址	--> 成功(结构体占用的存储空间内部申请，调用者释放)
 *		 NULL			--> 失败(内部释放)
 */
htp_header_t *htp_ass_header(UINT16 opcode, UINT16 para, UINT32 len);

#endif

