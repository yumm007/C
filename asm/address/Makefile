all:
	@arm-linux-gcc -c -o lds.o lds.s
	@arm-linux-ld -o lds lds.o -Ttext 0x30000000
	@arm-linux-objcopy -O binary lds /tftpboot/lds.bin
	
