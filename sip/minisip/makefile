COBJECT=	main.o minisip.o
CFLAGS=		-Wall -g
BIN=		sip-client

sip-client:	$(COBJECT)
	cc -o $@ $^ ${CFLAGS}

clean:
	@rm -rf a.out *.o ${BIN}

