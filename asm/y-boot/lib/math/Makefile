COBJS	:=	math.o

all:	$(COBJS)
	$(LD) -r -o $(LIBNAME) $(COBJS)

%.o:	%.c
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo " CC		.$(CURDIR)/$@"

%.o:	%.s
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo " CC		.$(CURDIR)/$@"

.PHONY:	clean
clean:
	@rm -f $(LIBNAME) $(COBJS)

