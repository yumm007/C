

all:
	@arm-linux-gcc -c -o main.o main.c -fno-builtin
	@arm-linux-gcc -c -o dm9000.o dm9000.c -fno-builtin
	@arm-linux-ld -o main main.o dm9000.o -T net.lds
	@arm-linux-objcopy -O binary main /tftpboot/net.bin
	@rm -f main *.o

