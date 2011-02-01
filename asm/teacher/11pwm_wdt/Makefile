

all:
	@arm-linux-gcc -c -o start.o start.s
	@arm-linux-gcc -c -o pwm.o pwm.c -fno-builtin
	@arm-linux-gcc -c -o main.o main.c
	@arm-linux-ld -o pwm start.o pwm.o main.o -T test.lds
	@arm-linux-objcopy -O binary pwm /tftpboot/pwm.bin
	@rm -f pwm *.o
