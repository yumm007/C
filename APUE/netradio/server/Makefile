prefix	= /usr/local
PROJNAME	= netradio
bindir	= $(prefix)/bin

DEFINS	= DDEFAULT_INSTALLPATH
CFLAGS	= -Wall -g3 -D_GNU_SOURCE
CFLAGS2	= -lpthread
LDFLAGS	= -I../include -Iinclude
CC	= gcc

radio_server:	server.o channel0_pth.o list_init.o channel_path.o argconf.o
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS) $(CFLAGS2)
%.o:	%.c
	$(CC) -c $< $(LDFLAGS) $(CFLAGS)
install:
	cp radio_server $(bindir)/netradio_server
	./install_service
	cp -u -r medialib $(prefix)/$(PROJNAME)
uninstall:
	rm -rf $(bindir)/netradio_server
	rm -rf $(prefix)/$(PROJNAME)
	rm -rf /etc/init.d/netradio
	rm -rf /etc/service/netradio.conf
	
clean:
	rm -rf *.o radio_server

