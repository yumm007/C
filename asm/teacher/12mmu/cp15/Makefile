

all:
	@arm-linux-gcc -c -o mmu.o mmu.c -fno-builtin
	@arm-linux-ld -o mmu mmu.o -T mmu.lds
	@arm-linux-objcopy -O binary mmu /tftpboot/mmu.bin
	@rm -f mmu *.o

