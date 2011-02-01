

all:
	@arm-linux-gcc -c -o swi.o swi.s
	@arm-linux-ld -o swi swi.o -T test.lds
	@arm-linux-objcopy -O binary swi /tftpboot/swi.bin
	@rm -f swi swi.o
	
	@arm-linux-gcc -c -o vector.o vector.s
	@arm-linux-gcc -c -o syscall.o syscall.c -fno-builtin
	@arm-linux-ld -o vector vector.o syscall.o -T vector.lds
	@arm-linux-objcopy -O binary vector /tftpboot/vector.bin
	@rm -f vector *.o
