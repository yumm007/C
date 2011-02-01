CFLAGS+=-Wall -g3 -pthread
pth_qsort:	main.o pth_stack.o pth_qsort.o
	gcc -o $@ $^ $(CFLAGS)
%.o:	%.c
	gcc -c $< $(CFLAGS)
clean:
	rm -rf *.o pth_qsort

