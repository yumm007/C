gcc -c $1 -o $1.o
ld $1.o -o $1.out 2> /dev/null
objcopy -O binary $1.out $1.bin
rm $1.o $1.out
cat $1.bin | ./crc_val

