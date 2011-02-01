

all:
	arm-linux-gcc -c -o start.o start.c -fno-builtin -Wall
	arm-linux-gcc -c -o nand.o nand.c -fno-builtin -Wall
	arm-linux-gcc -c -o ctype.o ctype.c -fno-builtin -Wall
	arm-linux-ld -o nand start.o nand.o ctype.o -T nand.lds
	arm-linux-objcopy -O binary nand /tftpboot/nand.bin
	rm -f nand *.o

