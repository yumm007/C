all:
	@arm-linux-gcc -c -o key.o key.c -fno-builtin
	@arm-linux-ld -Tkey.lds -o key key.o
	@arm-linux-objcopy -O binary key /tftpboot/key.bin
	
clean:
	rm -f *.o key

