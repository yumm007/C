

all:
	@arm-linux-gcc -c -o lcd.o lcd.c -fno-builtin
	@arm-linux-ld -o lcd lcd.o -T lcd.lds
	@arm-linux-objcopy -O binary lcd /tftpboot/lcd.bin
	@rm -f lcd *.o

