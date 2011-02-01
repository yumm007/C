COBJS	=	string.o
COBJS += ctype.o
COBJS += printf.o

all:	$(COBJS)
	$(LD) -r -o $(LIBNAME) $(COBJS)

%.o:	%.c
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo " CC		.$(CURDIR)/$@"

.PHONY:	clean
clean:
	@rm -f $(LIBNAME) $(COBJS)

