#!/bin/bash
scons
./ascii2dot $1 | /home/yu/bin/b2c > dot.c

