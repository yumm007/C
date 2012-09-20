COBJECTS:=sip-call.o
CFLAGS:=-I/home/yu/code/libosip2-3.1.0/out-bin/include
LFLAGS:=-L/home/yu/code/libosip2-3.1.0/out-bin/lib 
LD:=-losipparser2 -losip2

sip-call:	${COBJECTS}
	cc -o $@ $< ${CFLAGS} ${LFLAGS} ${LD}
clean:
	@rm *.o sip-call -rf
