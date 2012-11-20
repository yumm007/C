COBJECT:=main.o xmpp-str.o
CFLAGS:=-Wall

xmpp-client: ${COBJECT}
	cc -o $@ $^ ${CFLAGS}

clean:
	rm -rf *.o xmpp-client

