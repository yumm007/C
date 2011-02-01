all:
	arm-linux-gcc -c -o und.o und.S
	arm-linux-ld -o und und.o -Ttext 0x30000000
	arm-linux-objcopy -O binary und /tftpboot/und.bin

