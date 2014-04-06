#!/bin/bash
#gcc ascii2dot.c protocal_pack.c fb_draw.c -o ascii2dot -Wall
./ascii2dot $1 | /home/yu/bin/b2c > dot.c

