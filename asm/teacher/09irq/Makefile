

all:
	@arm-linux-gcc -c -o start.o start.s
	@arm-linux-gcc -c -o irq.o irq.c
	@arm-linux-gcc -c -o main.o main.c
	@arm-linux-ld -o irq start.o irq.o main.o -T test.lds
	@arm-linux-objcopy -O binary irq /tftpboot/irq.bin
	@rm -f irq *.o
