all:
	arm-linux-gcc -c -o mrc.o mrc.s -fno-builtin
	arm-linux-ld -o mrc mrc.o -Ttext 0x30000000
	arm-linux-objcopy -O binary mrc /tftpboot/mrc.bin

