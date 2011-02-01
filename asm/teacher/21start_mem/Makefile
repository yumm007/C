

all:
	@arm-linux-gcc -c -o  start.o start.S
	@arm-linux-gcc -c -o  clock_init.o clock_init.S
	@arm-linux-gcc -c -o  mem_init.o mem_init.s
	@arm-linux-gcc -c -o  nand_read.o nand_read.c
	@arm-linux-gcc -c -o  main.o main.c -fno-builtin
	@arm-linux-gcc -c -o  serial.o serial.c -fno-builtin
	@arm-linux-ld -o start main.o serial.o  -T test.lds
	@arm-linux-objcopy -O binary start /tftpboot/start.bin
	@rm -f *.o start

