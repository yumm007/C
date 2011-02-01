

all:
	@arm-linux-gcc -c -o swi.o swi.s
	@arm-linux-gcc -c -o syscall.o syscall.c -fno-builtin
	@arm-linux-ld -o swi swi.o syscall.o -T test.lds
	@arm-linux-objcopy -O binary swi /tftpboot/swi.bin
	@rm -f swi *.o
