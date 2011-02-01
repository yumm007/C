

all:
	@arm-linux-gcc -c -o dma.o dma.c -fno-builtin
	@arm-linux-ld -o dma dma.o -T dma.lds
	@arm-linux-objcopy -O binary dma /tftpboot/dma.bin
	@rm -f dma *.o

