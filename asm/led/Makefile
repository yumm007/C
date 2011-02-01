all:
	@sync
	@arm-linux-gcc -c -o led.o led.c -fno-builtin
	@arm-linux-ld -o led led.o -T led.lds
	@arm-linux-objcopy -O binary led /tftpboot/led.bin
clean:
	rm -f *.o led
	
