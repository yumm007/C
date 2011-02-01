

all:
	@arm-linux-gcc -c -o  uart.o uart.c -fno-builtin
	@arm-linux-gcc -c -o  serial.o serial.c -fno-builtin
	@arm-linux-gcc -c -o  speed.o speed.c -fno-builtin
	@arm-linux-ld -o uart uart.o serial.o speed.o -T test.lds
	@arm-linux-objcopy -O binary uart /tftpboot/uart.bin
	@rm -f *.o uart

