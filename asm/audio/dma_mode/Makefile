all:
	arm-linux-gcc -c -o start.o start.s -fno-builtin -Wall
	arm-linux-gcc -c -o main.o main.c -fno-builtin -Wall
	arm-linux-gcc -c -o dma.o dma.c -fno-builtin -Wall
	arm-linux-gcc -c -o wm8976.o wm8976.c -fno-builtin -Wall
	arm-linux-ld -o audio start.o main.o dma.o wm8976.o -T audio.lds
	arm-linux-objcopy -O binary audio /tftpboot/audio.bin
clean:
	rm -rf *.o audio /tftpboot/audio.bin

