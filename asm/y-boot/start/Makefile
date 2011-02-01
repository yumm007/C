COBJS		:= gpio_init.o
#COBJS	:=	exception.o
#COBJS 	+= interrupt.o

START	:= start.o
START	+= nand.o

all:	$(START) $(COBJS)
	$(LD) -r -o $(LIBNAME) $(COBJS)

%.o:	%.c
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo " CC		.$(CURDIR)/$@"

%.o:	%.S
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo " CC		.$(CURDIR)/$@"

%.o:	%.s
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo " CC		.$(CURDIR)/$@"

.PHONY:	clean
clean:
	@rm -f $(LIBNAME) $(COBJS) $(START)
