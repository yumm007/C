#!/bin/bash
b=`expr $RANDOM % 10 + 1`
read -p "请输入一个数字[0]" a
a=${a:-0}

echo a=$a, b=$b

if [ $a -gt $b ] 
then
	echo "$a>$b"
elif [ $a -lt $b ]
then
	echo "$a<$b"
else
	echo "$a=$b"
fi
 
