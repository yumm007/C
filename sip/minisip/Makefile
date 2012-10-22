COBJECT=	main.o minisip.o msg_to_str.o md5.o sip_cmd.o
CFLAGS=		-Wall -Os -Werror
BIN=		sip-client rtp

all:	${BIN}

sip-client:	$(COBJECT)
	cc -o $@ $^ ${CFLAGS}

rtp:	rtp_recv.o
	gcc -o $@ $^ ${CFLAGS}

clean:
	@rm -rf a.out *.o ${BIN}

