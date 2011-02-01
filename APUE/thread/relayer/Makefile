CFLAGS+=-Wall -g3 -pthread
relay:	main.o	relayer.o 	rel_init_clos.o pth_queue.o
	gcc -o $@ $^ $(CFLAGS)
pth_queue.o.o:	pth_queue.c pth_queue.h
	gcc -c $< $(CFLAGS)
%.o:	%.c rel_puplic.h relayer.h
	gcc -c $< $(CFLAGS)
clean:
	rm -rf *.o relay

