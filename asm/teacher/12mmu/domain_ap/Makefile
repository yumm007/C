

all:
	@arm-linux-gcc -c -o start.o start.s
	@arm-linux-gcc -c -o main.o main.c -fno-builtin
	@arm-linux-gcc -c -o stat.o stat.c -fno-builtin
	@arm-linux-ld -o mmu start.o  main.o stat.o -T test.lds
	@arm-linux-objcopy -O binary mmu /tftpboot/mmu.bin
	@rm -f mmu *.o
