
obj	:= main.o
obj	+= dm9000.o
obj	+= net.o
obj	+= pack.o
obj	+= unpack.o
obj	+= ping.o


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

