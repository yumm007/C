CFLAGS+=-Wall -g3 -pthread
timer:	main.o	anytimer.o link.o
	gcc -o $@ $^ $(CFLAGS)
%.o:	 %.c  link.h anytime.h 
	gcc -c $< -o $@ $(CFLAGS)
clean:
	rm -rf *.o

