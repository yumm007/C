
MAKE 	= make -s
TOPDIR 	= $(shell pwd)

export	TOPDIR

CROSS_COMPILE 	?= arm-linux-

LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy

export LD CC OBJCOPY

#########################################################################

.PHONY: all net.bin net subdirs

all:	net.bin
	@mv	net.bin /tftpboot
	@echo " MOVE		/tftpboot/net.bin"

net.bin: net
	@$(OBJCOPY) -O binary $< $@

net: subdirs
	@$(LD) -o $@ lib/*.o drivers/*.o -T include/net.lds

subdirs:
	@$(MAKE) -C lib
	@$(MAKE) -C drivers

########################################################################

.PHONY: clean
clean:
	@$(MAKE) -C lib clean
	@$(MAKE) -C drivers clean
	@rm -f net /tftpboot/net.bin
	@echo " Remove"

