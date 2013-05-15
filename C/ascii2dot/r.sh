#!/bin/bash
gcc ascii2dot.c -Wall -g
./a.out 2> pic.bin
b2c pic.bin > pic_arr.c
#rm a.out pic.bin

