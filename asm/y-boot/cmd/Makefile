
include ../.config

obj-y := ushell.o
obj-$(CONFIG_ECHO) += echo.o
obj-$(CONFIG_NAND) += nand.o
obj-$(CONFIG_BOOTM) += bootm.o
obj-$(CONFIG_BOOTM) += bootnfs.o
obj-$(CONFIG_BOOTM) += audio.o

all:	$(obj-y)
	@$(LD) -r -o $(LIBNAME) $(obj-y)

%.o:	%.S
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo " CC		.$(CURDIR)/$@"

%.o:	%.c
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo " CC		.$(CURDIR)/$@"

.PHONY:	clean
clean:
	@rm -f $(LIBNAME) *.o
