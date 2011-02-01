all:
	arm-linux-gcc -c -o swi.o swi.S
	arm-linux-ld -o swi swi.o -Ttext 0x30000000
	arm-linux-objcopy -O binary swi /tftpboot/swi.bin
clean:
	rm *.o swi

