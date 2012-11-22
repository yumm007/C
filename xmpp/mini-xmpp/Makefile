COBJECT:= 	main.o xmpp-str.o base64.o
CFLAGS:= 	-Wall -Os

xmpp-client: ${COBJECT}
	cc -o $@ $^ ${CFLAGS}

clean:
	rm -rf *.o xmpp-client

