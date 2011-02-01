all:
	arm-linux-gcc -c -o timer.o timer.S -fno-builtin
	arm-linux-gcc -c -o main.o main.c -fno-builtin
	arm-linux-ld -o timer timer.o main.o -T timer.lds
	arm-linux-objcopy -O binary timer /tftpboot/timer.bin

