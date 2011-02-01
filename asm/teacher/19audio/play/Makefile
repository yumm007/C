

all:
	@arm-linux-gcc -c -o start.o start.s
	@arm-linux-gcc -c -o main.o main.c
	@arm-linux-gcc -c -o irq.o irq.c
	@arm-linux-gcc -c -o wm8976.o wm8976.c
	@arm-linux-ld -o audio start.o main.o irq.o wm8976.o -T test.lds
	@arm-linux-objcopy -O binary audio /tftpboot/audio.bin
	@rm -f audio *.o
