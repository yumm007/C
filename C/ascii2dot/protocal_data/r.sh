#!/bin/bash
gcc ascii2dot.c -o ascii2dot
gcc protocal_pack.c -o protocal_pack
./ascii2dot | ./protocal_pack > dot.bin
b2c dot.bin > dot.c
rm dot.bin

