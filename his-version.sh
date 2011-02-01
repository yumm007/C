#!/bin/bash
#脚本将指定目录下的文件备份到另外目录下,可以加上参数指定需要备份的目录
#备份目录仍按照原先目录格式，备份文件名加上后缀.date_time
#BUGS: 在打包旧文件时，文件名不能含有空格。
#2010-08-25  yumm_007@163.com

#如果命令没有加参数，默认使用当前目录
#BACKUPDIR=${1:-`pwd`}
BACKUPDIR="/home/yu/code/"
BACKUPTO="/home/yu/codebackup"

#如果需要备份的目录不存在，提示错误并退出
if [ ! -d $BACKUPDIR ]; then
	echo "Directory $BACKUPDIR not exist!"
	exit 1
else    #如果存在，取它的全路径
	cd $BACKUPDIR
	BACKUPDIR=`pwd`
fi

#将目录名最后的/取掉
BACKUPDIR=`echo $BACKUPDIR | sed 's!/$!!'`
BACKUPTO=`echo $BACKUPTO | sed 's!/$!!'`

#将要备份的目录名字中首个/替换成_，把它作为BACKUPTO的子目录名
subfolder=${BACKUPDIR/\//_}
BACKUPTO=$BACKUPTO/$subfolder

#如果BACKUPTO目录不存在，创建之
if [ ! -d $BACKUPTO ]; then
	mkdir -p $BACKUPTO	
fi

#开始备份,并把两天前的备份挪到压缩包里
# cp-S 参数指定备份文件名的后缀格式
cp -p -r -u -S \.`date +%F_%T` --backup=simple $BACKUPDIR/* $BACKUPTO
find $BACKUPTO -regextype posix-extended -regex '.*[-_:0-9]{19}$' -mtime +2 | xargs tar --remove-files -uvf /home/yu/codebackup/backup.tar
