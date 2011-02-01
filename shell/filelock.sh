#!/bin/bash
#文件锁测试

lockdir=/tmp/lock/subsys
lockfile=$lockdir/lock_file_test.lck

if [ ! -d $lockdir ];then
	mkdir -p $lockdir
fi

if [ -f $lockfile ];then
	read tempid < $lockfile
	if [ -d /proc/$tempid ];then
		echo "Program is running"
		exit
	fi
fi

echo $$ > $lockfile
echo "Program begin runnung"
sleep 200
rm -rf $lockfile

