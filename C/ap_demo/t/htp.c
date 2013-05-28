
//for linux header
#include "linux_header.h"

#include "htp.h"

/*
 * 功能：创建一个指定服务器IP的socket连接（TCP/IP）
 * 参数：ip_addr --> 指定服务器的IP地址
 *		 port    --> 指定服务器的端口
 *		 socket  --> 成功连接的socket句柄
 * 返回：0  --> 成功
 *		 -1 --> socket创建失败
 *		 -x --> connet连接失败,其中x=errno
 */
INT32 htp_open(htp_socket_t *htp_socket) {
	SOCKET sd;
	struct sockaddr_in svr;
	int ret;

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		ret = errno;
		goto _ret;
	}

	bzero(&svr, sizeof(svr));
	svr.sin_family = AF_INET;
	svr.sin_addr.s_addr = inet_addr(htp_socket->ip_addr);
	svr.sin_port = htons(htp_socket->port);

	if (connect(sd, (void *)&svr, sizeof(svr)) == -1) {
		ret = errno;
		goto _close;
	}

	htp_socket->socket = sd;

	return 0;

_close:
	close(sd);
_ret:
	return ret;
}


static int safe_read(SOCKET sd, void *buf, int len) {
	int n, i, err;

	for (i = 0; i < 3; i++) {
		n = recv(sd, buf, len, 0);
		if (n >= 0)
			return n;
		//read失败
		if (n == -1){
			err = errno;
			if (err == EINTR) {//被中断
				fprintf(stderr, "recv recv sig.\n");
				continue;
			} else if (err == ECONNREFUSED) //连接断开
				return 0;
		} else
			return -1;	//recv失败, errno返回其他错误
	}

	return -1;
}

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
INT32 read_socket(SOCKET socket, void* dst, INT32 len, UINT32 timeout) {
	struct timeval tim = {0, timeout * 1000};
	fd_set rfds;
	int n = 0, n1, ret;

	for (;;) {
		FD_ZERO(&rfds);
		FD_SET(socket, &rfds);

		ret = select(socket+1, &rfds, NULL, NULL, &tim);
		//fprintf(stderr, "read_socket: i = %d, ret = %d\n", i, ret);
		if (ret > 0) {	// read ready
			n1 = safe_read(socket, (char *)dst + n, len - n);
			//fprintf(stderr, "safe_read:%d\n", n1);
			if (n1 <= 0) //safe_read 失败 0 断开，-1其他错误
				return n;
			n += n1;
			if (n == len)	//成功接受完指定长的数据
				return n;
			//继续收
		} else if (ret == -1 && errno == EINTR) { //select interrupt
			fprintf(stderr, "select recv sig.\n");
			continue;
		} else if (ret == 0 && n > 0) {	//时间耗尽且收到过数据
			return n;
		} else
			break; //select got other error
	}

	return -1;
}

/*
 * 功能：发送指定长度的数据
 * 参数：socket --> 需要发送的socket句柄
 *		 src    --> 发送数据的首地址
 *		 len    --> 发送数据的长度
 * 返回：>0 --> 成功发送的数据个数
 *		 0  --> socket连接断开
 *		 -1 --> 发送数据错误
 */
INT32 write_socket(SOCKET socket, const void* src, INT32 len) {
   INT32 i, n, err;
	int rty_times = 3;

	for (i = 0; i < rty_times; i++) {
		n = send(socket, src, len, 0);
		if (n == len) //ok
			return n;
		err = errno;
		if (n < len && err == EINTR) //interrupt
			continue;
		else if (err == ECONNRESET) //dis connect
			return 0; 
		else
			break;	//other err
	}
   
   return -1;
}

/*
 * 功能：向指定的socket发送数据(先发htp_header，再发htp_socket->buf)
 * 参数：htp_socket->socket --> 需要发送的socket句柄
 *		 htp_socket->buf    --> 发送数据的首地址
 *		 htp_socket->len    --> 发送数据的长度
 *		 htp_header			--> htp包的数据头
 * 返回：TRUE  --> 发送成功
 *		 FALSE --> 发送失败
 */
bool htp_send(htp_socket_t *htp_socket, htp_header_t *htp_header) {
	htp_socket_t *s = htp_socket;
	htp_header_t *h = htp_header;

	if (write_socket(s->socket, h, sizeof(htp_header_t)) <= 0)
		return false;
	if (write_socket(s->socket, s->buf, s->len) <= 0)
		return false;
	
	return true;
}

static int htp_header_check(const htp_header_t *h) {
	
	if (h->version != HTP_VERSION \
		|| strncmp(h->vsrt, HTP_VERSION_S, strlen(HTP_VERSION_S) != 0))
		return -1;

	if (  (~(h->version)) != h->version_s \
		|| (~(h->opcode))  != h->opcode_s \
		|| (~(h->para))    != h->para_s \
		|| (~(h->len))     != h->len_s) {
			return -1;
		}
	
	return 0;
}

/*
 * 功能：接收指定的socket数据(将接收成功的htp数据头存在htp_header和数据体存在htp_socket->buf中)
 * 参数：htp_socket->socket --> 指定接收的socket句柄
 *		 htp_socket->buf    --> 接收数据的首地址,内部分配空间
 *		 htp_socket->len    --> 接收数据的长度
 *		 htp_header			--> htp包的数据头
 * 返回：TRUE  --> 接收成功(调用者释放)
 *		 FALSE --> 接收失败(内部释放)
 */
bool htp_recv(htp_socket_t *htp_socket, htp_header_t *htp_header) {
	htp_socket_t *s = htp_socket;
	htp_header_t *h = htp_header;
	UINT8 *data_buf;
	
	//接受头失败
	if (read_socket(s->socket, h,\
		sizeof(htp_header_t), 100) != sizeof(htp_header_t))
		goto _clean_sock_buf;

	//协议头不对
	if (htp_header_check(h) != 0) {
		goto _clean_sock_buf;
	}

	switch (h->opcode) {
		case HTP_OPCODE_WRITEDATA:
			data_buf = malloc(h->len);
			if (data_buf == NULL)
				goto _clean_sock_buf;
			s->buf = data_buf;
			s->len = h->len;
			//接收数据区域
			//BUGS: s->len为UINT32, 而参赛要求是INT32
			if (read_socket(s->socket, s->buf, s->len, 100) != s->len) {
				goto _clean_data_buf;
			}
			break;
		case HTP_OPCODE_KICKOFF:
		case HTP_OPCODE_CANCAL:
		case HTP_OPCODE_PING:
		case HTP_OPCODE_ACK:
		case HTP_OPCODE_NAK:
			//只有头信息
			break;
		default:
			goto _clean_sock_buf;
			break;
	}

	return true;

_clean_data_buf:
	free(data_buf);

_clean_sock_buf:
	return false;
}

/*
 * 功能：关闭指定服务器IP的socket连接（TCP/IP）
 * 参数：socket  --> 需要关闭的socket句柄
 * 返回：TRUE  --> 关闭成功
 *		 FALSE --> 关闭失败
 */
bool htp_close(htp_socket_t *htp_socket) {
	return close(htp_socket->socket) == 0 ? true : false;
}

/*
 * 功能：组htp包头
 * 参数：opcode --> htp包的命令字
 *		 para	--> htp包命令字的参数,默认为0
 *		 len	--> htp包数据体的长度,默认为0
 * 返回：数据头的首地址	--> 成功(结构体占用的存储空间内部申请，调用者释放)
 *		 NULL			--> 失败(内部释放)
 */
htp_header_t *htp_ass_header(UINT16 opcode, UINT16 para, UINT32 len) {
	return NULL;
}

