CFLAGS+=-Wall -g3 -pthread
all:	server client
server:	msg_s.o hash_link.o pth_queue.o
	gcc -o msg_server msg_s.o hash_link.o pth_queue.o $(CFLAGS)
client: msg_c.o 
	gcc -o msg_client msg_c.o $(CFLAGS)
%.o:	%.c %.h
	gcc -c $< $(CFLAGS)
clean:
	rm -rf *.o msg_client msg_server

