all: hello.cgi time.cgi image.cgi login.cgi

%.cgi: %.c
	gcc -o $@ $^ $(CFLAGS) $(LDFLAGS)

install:
	cp *.cgi /var/www/cgi-bin/
	cp *.html /var/www/html/

