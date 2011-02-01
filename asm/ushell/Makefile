############################################################
MAKE     = make -s
TOPDIR	= $(shell pwd)
CURDIR 	= $(subst $(TOPDIR),,$(shell pwd))
export MAKE TOPDIR CURDIR

CROSS_COMPILE	?=	arm-linux-
AS	=	$(CROSS_COMPILE)as
LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
CPP	= $(CC) -E
AR	= $(CROSS_COMPILE)ar
NM	= $(CROSS_COMPILE)nm
LDR	= $(CROSS_COMPILE)ldr
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

CPPFLAGS	:=	-I$(TOPDIR)/include -fno-builtin
CFLAGS	:=	$(CPPFLAGS)	-Wall
LIBNAME	:=	build-in.o

export AS LD CC CPP AR NM LDR STRIP OBJCOPY OBJDUMP CPPFLAGS CFLAGS LIBNAME
############################################################

LDSCRIPT	:=	$(TOPDIR)/ushell.lds
OUT_BIN	:=	ushell.bin
OUT_ELF	:=	ushell.elf

SUBDIRS	:=	cmd/
SUBDIRS	+=	drivers/

SUBLIBS	:=	$(addsuffix $(LIBNAME), $(SUBDIRS))

.PHONY:	$(OUT_BIN) $(OUT_ELF) $(SUBDIRS)
############################################################

all:	$(OUT_BIN)
	@mv	$(OUT_BIN)	/tftpboot
	@echo	" MOVE	/tftpboot/$(OUT_BIN)"

$(OUT_BIN):	$(OUT_ELF)
	@$(OBJCOPY) -O	binary $< $@

$(OUT_ELF):	$(SUBDIRS)
	@$(LD) $(SUBLIBS) -o $@ -T $(LDSCRIPT)

$(SUBDIRS):
	@$(MAKE)	-C	$@	all

.PHONY:	clean
clean:
	@for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done
	@rm -f $(OUT_ELF) $(OUT_BIN)
	@echo	"Remove"

