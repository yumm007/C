

all:
	@arm-linux-gcc -c -o start.o start.s
	@arm-linux-gcc -c -o wdt.o wdt.c
	@arm-linux-gcc -c -o main.o main.c
	@arm-linux-ld -o wdt start.o wdt.o main.o -T test.lds
	@arm-linux-objcopy -O binary wdt /tftpboot/wdt.bin
	@rm -f wdt *.o
