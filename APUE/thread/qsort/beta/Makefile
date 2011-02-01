CFLAGS+=-Wall -g3 -pthread
pth_qsort:	main.o pth_queue.o
	gcc -o $@ $^ $(CFLAGS)
%.o:	%.c
	gcc -c $< $(CFLAGS)
clean:
	rm -rf *.o

