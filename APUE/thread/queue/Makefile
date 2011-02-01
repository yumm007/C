CFLAGS+=-Wall -g3 -pthread
queue:	main.o	pth_queue.o
	gcc $^ -o $@ $(CFLAGS)
%.o:	%.c pth_queue.h
	gcc -c $< $(CFLAGS)
clean:
	rm -rf *.o 1 2
test:
	./queue >1 2>2
	md5sum 1 2
	
