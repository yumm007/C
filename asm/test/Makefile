all:
	arm-linux-gcc -c -o label.o label.S
	arm-linux-ld -o label label.o -Ttext 0x30000000
	arm-linux-objcopy -O binary label /tftpboot/label.bin
clean:
	rm *.o label

