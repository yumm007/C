all:
	arm-linux-gcc -c -o com0.o com.c -fno-builtin
	arm-linux-gcc -c -o com_init.o com_init.c -fno-builtin
	arm-linux-ld -Ttext 0x30000000 -o com0 com0.o com_init.o
	arm-linux-objcopy -O binary com0 /tftpboot/com0.bin

clean:
	rm -f *.o com0
