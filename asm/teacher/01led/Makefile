

all:
	@arm-linux-gcc -c -o led.o led.S
	@arm-linux-ld -o led led.o -T led.lds
	@arm-linux-objcopy -O binary led /tftpboot/led.bin
	@rm -f led led.o

