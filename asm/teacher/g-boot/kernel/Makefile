
start_obj := start.o nand_read.o mem_init.o clock_init.o

start:	$(start_obj)

clean_start:
	@rm -f $(start_obj)

.PHONY: start clean_start


obj-y	:= interrupt.o
obj-y	+= exception.o
obj-y	+= serial.o
obj-y	+= console.o
obj-y	+= shell.o

include $(TOPDIR)/config.mk
