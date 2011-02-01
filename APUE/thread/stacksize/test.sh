#!/bin/bash

max=16*1024*10;
for(( i = 16; i < $max; i +=4))
do
	gcc main.c -pthread -DSTACHS_SIZE=$i
	./a.out
done

