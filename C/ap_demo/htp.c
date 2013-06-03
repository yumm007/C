// htp.cpp: 主项目文件。

//#include "stdafx.h"

//for windows header
//#include "windows_header.h"

//for linux header
#include "linux_header.h"

#include "htp.h"

INT32 htp_open(htp_socket_t *htp_socket) {
	SOCKET sd;
	struct sockaddr_in svr;
	int ret;
	int i;

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		ret = errno;
		goto _ret;
	}

	memset(&svr, 0, sizeof(svr));
	svr.sin_family = AF_INET;
	svr.sin_addr.s_addr = inet_addr(htp_socket->ip_addr);
	svr.sin_port = htons(htp_socket->port);

	for (i = 0; i < 100; i++) {
		if (connect(sd, (void *)&svr, sizeof(svr)) == -1) {
			ret = errno;
			if (ret != EINTR)
				goto _close;
		} else
			break;	//connect ok
	}

	htp_socket->socket = sd;
	htp_socket->buf = NULL;
	htp_socket->len = 0;

	return 0;

_close:
	close(sd);
_ret:
	return ret;
}

static int safe_read(SOCKET sd, void *buf, int len) {
	int n, i, err;

	for (i = 0; i < 3; i++) {
		n = recv(sd, (char *)buf, len, 0);
		if (n >= 0)
			return n;
		//read失败
		if (n == -1){
			err = errno;
			if (err == EINTR) {//被中断
				continue;
			} else if (err == ECONNREFUSED ) //连接断开
				return 0;
		} else
			return -1;	//recv失败, errno返回其他错误
	}

	return -1;
}

INT32 read_socket(SOCKET socket, void* dst, INT32 len, UINT32 timeout) {
	struct timeval tim = {0, timeout * 1000};
	fd_set rfds;
	int n = 0, n1, ret;

	//double start_tim;
	//long els_tim; 

	for (;;) {
		FD_ZERO(&rfds);
		FD_SET(socket, &rfds);

		//start_tim = GetTickCount();
		ret = select(socket+1, &rfds, NULL, NULL, &tim);

		//减去消逝的时间
		//els_tim = (long)(GetTickCount() - start_tim);
		//tim.tv_usec -= els_tim;

		if (ret > 0) {	// read ready
			n1 = safe_read(socket, (char *)dst + n, len - n);
			if (n1 <= 0) //safe_read 失败 0 断开，-1其他错误
				return n;
			n += n1;
			if (n == len)	//成功接受完指定长的数据
				return n;
			//继续收
		} else if (ret == -1 && errno == EINTR) { //select interrupt
			//只有在被中断打断的情况下才会一直循环
			continue;
		} else if (ret == 0 && n > 0) {	//时间耗尽且收到过数据
			return n;
		} else
			break; //select got other error
	}

	return -1;
}

INT32 write_socket(SOCKET socket, const void* src, INT32 len) {
   INT32 i, n, err;
	int rty_times = 3;

	for (i = 0; i < rty_times; i++) {
		n = send(socket, (const char *)src, len, 0);
		if (n == len) //ok or disconnect
			return n;
		err = errno;
		if (n < len && err == EINTR) //interrupt
			continue;
		else
			break;	//other err
	}
   
   return -1;
}

bool htp_send(htp_socket_t *htp_socket, htp_header_t *htp_header) {
	htp_socket_t *s = htp_socket;
	htp_header_t *h = htp_header;

	if (write_socket(s->socket, h, sizeof(htp_header_t)) <= 0)
		return false;
	if (s->len != 0 && write_socket(s->socket, s->buf, s->len) <= 0)
		return false;
	
	static int n = 0;
	n++;
	char buf[10];

	sprintf(buf, "file%d", n);
	FILE *fp = fopen(buf, "wb");
	fwrite(h, 1, sizeof(htp_header_t), fp);
	fwrite(s->buf, 1, s->len, fp);
	fclose(fp);

	return true;
}

static int htp_header_check(const htp_header_t *h) {
	
	if (h->version != HTP_VERSION \
		|| strncmp(h->vsrt, HTP_VERSION_S, strlen(HTP_VERSION_S) != 0))
		return -1;

	if (   ((~(h->version)) & 0xFFFF) != h->version_s \
		|| ((~(h->opcode)) & 0xFFFF) != h->opcode_s \
		|| ((~(h->para))   & 0xFFFF) != h->para_s \
		|| ((~(h->len))    & 0xFFFFFFFF) != h->len_s) {
			return -1;
		}
	
	return 0;
}

UINT8 *htp_new(UINT32 len) {
	return (UINT8 *)malloc(len);
}

void htp_free(UINT8 *pMem) {
	free(pMem);
}

bool htp_recv(htp_socket_t *htp_socket, htp_header_t *htp_header) {
	htp_socket_t *s = htp_socket;
	htp_header_t *h = htp_header;
	UINT8 *data_buf = NULL;
	
	//接收头失败
	if ( read_socket(s->socket, h,\
		sizeof(htp_header_t), READ_TIMEOUT_MS) != sizeof(htp_header_t)) {
		goto _clean_sock_buf;
	}

	//协议头不对
	if (htp_header_check(h) != 0) {
		goto _clean_sock_buf;
	}

	if (h->len == 0) {
		s->buf = NULL;
		s->len = 0;
		return true;	//后面没有数据了
	}

	//申请内存
	data_buf = htp_new(h->len);
	if (data_buf == NULL)
		goto _clean_sock_buf;
	s->buf = data_buf;
	s->len = h->len;

	//接收数据区域
	//BUGS: s->len为UINT32, 而参赛要求是INT32
	if (read_socket(s->socket, s->buf, s->len, READ_TIMEOUT_MS) != s->len) {
		goto _clean_data_buf;
	}

	return true;

_clean_data_buf:
	htp_free(data_buf);

_clean_sock_buf:
	//读完剩余的错误数据
	while (read_socket(s->socket, htp_header, sizeof(htp_header_t), 1) > 0)
		;
	return false;
}


bool htp_close(htp_socket_t *htp_socket) {
	return close(htp_socket->socket) == 0 ? true : false;
}

bool htp_ass_header(htp_header_t *htp_header, UINT16 opcode, UINT16 para , UINT32 len ) {

   htp_header_t *h = htp_header;

   h->version = HTP_VERSION;
   h->version_s = ~h->version;
   strncpy((char *)h->vsrt, HTP_VERSION_S, sizeof(h->vsrt));
   
   h->opcode = opcode;
   h->opcode_s = ~h->opcode;

   h->para = para;
   h->para_s = ~h->para;

   h->len = len;
   h->len_s = ~h->len;

   return true;
}




