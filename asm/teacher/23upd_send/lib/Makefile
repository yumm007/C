
obj	:= start.o
obj	+= serial.o
obj	+= stdio.o
obj	+= string.o
obj	+= vsprintf.o
obj	+= _udivsi3.o
obj	+= _umodsi3.o
obj	+= div0.o
obj	+= ctype.o
obj	+= exception.o
obj	+= interrupt.o


.PHONY: all
all:  $(obj)

#########################################################################
	
CPPFLAGS := -I$(TOPDIR)/include -nostdinc -fno-builtin -ffreestanding -pipe

CFLAGS := $(CPPFLAGS) -Wall -Wstrict-prototypes -fno-stack-protector \
	-march=armv4t -mabi=apcs-gnu -mno-thumb-interwork -Os

CURDIR := $(subst $(TOPDIR),,$(shell pwd))

%.o:	%.S
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo " CC		.$(CURDIR)/$@"

%.o:	%.s
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo " CC		.$(CURDIR)/$@"

%.o:	%.c
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo " CC		.$(CURDIR)/$@"

#########################################################################

.PHONY: clean
clean:
	@rm -f $(obj)

