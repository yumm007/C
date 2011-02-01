all:
	arm-linux-gcc -c -o dma.o dma.c -fno-builtin -Wall
	arm-linux-ld -o dma dma.o -T dma.lds
	arm-linux-objcopy -O binary dma /tftpboot/dma.bin
clean:
	rm -rf *.o dma /tftpboot/dma.bin

