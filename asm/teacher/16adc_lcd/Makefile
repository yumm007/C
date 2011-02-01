

all:
	@arm-linux-gcc -c -o start.o start.s
	@arm-linux-gcc -c -o adc_ts.o adc_ts.c -fno-builtin
	@arm-linux-gcc -c -o main.o main.c
	@arm-linux-ld -o adc_lcd start.o adc_ts.o main.o -T test.lds
	@arm-linux-objcopy -O binary adc_lcd /tftpboot/adc_lcd.bin
	@rm -f adc_lcd *.o
