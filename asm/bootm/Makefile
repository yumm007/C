all:
	arm-linux-gcc -c -o bootm.o bootm.S
	arm-linux-ld -o bootm bootm.o -T bootm.lds
	arm-linux-objcopy -O binary bootm /tftpboot/bootm.bin
clean:
	rm *.o bootm /tftpboot/bootm.bin

