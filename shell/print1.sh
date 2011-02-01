#!/bin/bash
for i in {1..4}
do
	for j in {1..4}
	do
		echo -n "* "
	done
	echo
done

for i in {1..100}
do
	if [ `expr $i % 7` -eq 0 ]
	then
		echo -n "$i "
	fi
done
echo

for ((i=1;i<=9;i++))
do
	for ((j=1;j<=$i;j++))
	do
		echo -n -e "$i*$j=`expr $i \* $j`\t"
	done
	echo
done




