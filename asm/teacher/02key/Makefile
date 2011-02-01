

all:
	@arm-linux-gcc -c -o key.o key.c
	@arm-linux-ld -o key key.o -T key.lds
	@arm-linux-objcopy -O binary key /tftpboot/key.bin
	@rm -f key key.o

