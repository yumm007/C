
OBJS 	:= $(notdir $(obj-y))
SUBDIRS := $(subst ./,,$(dir $(obj-y)))
SUBLIBS	:= $(addsuffix built-in.o,$(SUBDIRS))

#########################################################################

.PHONY: all
ifeq ($(obj-y),)

all:
	@rm -f built-in.o
	@$(AR) rcs built-in.o
else

all: $(SUBDIRS) $(OBJS)
	$(LD) -r -o built-in.o $(OBJS) $(SUBLIBS)

endif

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@$(MAKE) -C $@ all	

#########################################################################
	
CPPFLAGS := -I$(TOPDIR)/include -nostdinc -fno-builtin -ffreestanding -pipe

CFLAGS := $(CPPFLAGS) -Wall -Wstrict-prototypes -fno-stack-protector \
	-march=armv4 -mabi=apcs-gnu -mno-thumb-interwork -Os

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
	@for i in $(SUBDIRS);do $(MAKE) -C $$i clean;done
	@rm -f $(OBJS) built-in.o

