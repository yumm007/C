COBJECT=	main.o minisip.o msg_to_str.o md5.o sip_cmd.o
CFLAGS=		-Wall -Werror -Os
BIN=		sip-client

sip-client:	$(COBJECT)
	cc -o $@ $^ ${CFLAGS}

clean:
	@rm -rf a.out *.o ${BIN}

