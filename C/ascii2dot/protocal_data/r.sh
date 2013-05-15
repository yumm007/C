#!/bin/bash
gcc ascii2dot.c protocal_pack.c -o ascii2dot -Wall
./ascii2dot 172 72 | b2c > dot.c

