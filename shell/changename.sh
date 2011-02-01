#!/bin/bash
#批量更改文件名. -r 参数表示遍历目录
#Usage: ./changename.sh [-r] dirname ext newext
#2010-08-23 yumm_007@163.com

#判断参数是否合格
if [ $# -ne 3 -a $# -ne 4 ]; then      #参数不等于3或4时退出
	echo Usage: ./changename.sh [-r] dirname ext newext 
	exit 1
elif [ $# -eq 3 ]; then				   #参数等于3时候
	DIR=$1 ext=$2 newext=$3
elif [ $1 = "-R" -o $1 = "-r" ];then   #参数等于4的时候,即遍历目录
	DIR=$2 ext=$3 newext=$4
else
	echo Usage: ./changename.sh [-r] dirname ext newext 
    exit 1
fi

#确定目录是否存在
if [ ! -d $DIR ]; then
	echo Error: $DIR: No such directory
	exit 1
fi

#过滤输入的后缀名,屏蔽"."号
ext=${ext//\./}
newext=${newext//\./}

#变量filename记录了待操作的文件列表
if [ $# -eq 3 ]; then
	filename=`find $DIR -name \*.$ext -maxdepth 1 2> /dev/null`
else 	
	filename=`find $DIR -name \*.$ext -depth 2> /dev/null`
fi

#没有找到合适的文件
if [ ${#filename} -le 0 ]; then
	echo "can not find .$ext files in directory $DIR"
	exit 1
fi

#执行替换命令
for i in $filename ; do
	extlen=`expr ${#i} - ${#ext}`
	echo /bin/mv $i ${i:0:$extlen}$newext 2>/dev/null	
done

