

all:
	@arm-linux-gcc -c -o und.o und.s
	@arm-linux-ld -o und und.o -T test.lds
	@arm-linux-objcopy -O binary und /tftpboot/und.bin
	@rm -f und und.o
	@arm-linux-gcc -c -o vector.o vector.s
	@arm-linux-ld -o vector vector.o -T vector.lds
	@arm-linux-objcopy -O binary vector /tftpboot/vector.bin
	@rm -f vector vector.o
