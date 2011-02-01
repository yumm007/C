MAKE		= make
INSTALL		= $(MAKE) install
UNINSTALL	= $(MAKE) uninstall
CLEAN		= $(MAKE) clean

SUBDIRS	= client server

netradio:
	for i in $(SUBDIRS); do $(MAKE) -C $$i; done

install:	install_client install_server
install_client:
	cd client && make install
	cp -r doc /usr/local/netradio/
	cp HELP README /usr/local/netradio/
install_server:
	cd server && make install
	cp HELP README /usr/local/netradio/

uninstall:
	for i in $(SUBDIRS); do $(UNINSTALL) -C $$i; done
	rm -rf /usr/local/netradio/
clean:	
	for i in $(SUBDIRS); do $(CLEAN) -C $$i; done
	
