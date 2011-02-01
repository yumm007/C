all:
	arm-linux-gcc -c -o start.o swi.s -fno-builtin -Wall
	arm-linux-gcc -c -o main.o main.c -fno-builtin -Wall
	arm-linux-gcc -c -o ts.o ts.c -fno-builtin -Wall
	arm-linux-ld -o pic start.o main.o ts.o -T pic.lds
	arm-linux-objcopy -O binary pic /tftpboot/pic.bin
clean:
	rm -rf *.o pic /tftpboot/pic.bin

