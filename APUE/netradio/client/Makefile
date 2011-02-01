CFLAGS+= -I../include -Iinclude -Llib -Wall -g3 -D_GNU_SOURCE
LDFLAGS+=-lpthread -lclient
player	= mplayer

all:	libclient.a client

libclient.a: client.o
	ar -cr $@ $^
	ranlib $@
	mv $@ lib/

client.o: client.c
	gcc -c -o $@ $< $(CFLAGS) $(LDFLAGS)

client: main.o arg.o
	gcc -o $@ $^ $(CFLAGS) $(LDFLAGS)

arg.o:	arg.c
	gcc -c $< $(CFLAGS) -DDEFAULT_PLAYER=\"$(player)\"

install:
	cp client /usr/local/bin/netradio

uninstall:
	rm -f client /usr/local/bin/netradio

clean:
	rm -f client *.o *~
	rm -f lib/libclient.a
