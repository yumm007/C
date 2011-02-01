all:
	arm-linux-gcc -c -o intr.o intr.S -fno-builtin
	arm-linux-gcc -c -o main.o main.c -fno-builtin
	arm-linux-ld -o intr intr.o main.o -T intr.lds
	arm-linux-objcopy -O binary intr /tftpboot/intr.bin

