

all:
	@arm-linux-gcc -c -o mem_init.o mem_init.S -fno-builtin
	@arm-linux-ld -o mem_init mem_init.o -T mem_init.lds
	@arm-linux-objcopy -O binary mem_init /tftpboot/mem_init.bin
	@rm -f mem_init *.o

