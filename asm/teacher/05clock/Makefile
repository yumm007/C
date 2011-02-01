

all:
	@arm-linux-gcc -c -o clock_init.o clock_init.S
	@arm-linux-ld -o clock_init clock_init.o -T test.lds
	@arm-linux-objcopy -O binary clock_init /tftpboot/clock_init.bin
	@rm -f clock_init *.o

