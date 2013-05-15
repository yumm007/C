#!/bin/bash
gcc ascii2dot.c -o ascii2dot -Wall
gcc protocal_pack.c -o protocal_pack -Wall
./ascii2dot 172 72 | ./protocal_pack > dot.bin
b2c dot.bin > dot.c
rm dot.bin

