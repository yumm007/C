#!/bin/bash

#output file
log_file=temp_file
esl_list=esl_list_file
esl_data=esl_data_file

#找到符合要求的所有log条目
grep -E '^[^:]{3}:[^:]{1,}:[^:]{1,}:3:7:*' $1 > $log_file
#去掉case7的统计
#grep -E '^[^:]{3}:[^:]{1,}:[^:7]{1,}:3:7:*' $1 > $log_file
#生成终端ID清单
cut -d':' -f7 $log_file | sort | uniq > $esl_list

echo -e 'ID\t\t\tFRAME\tPASS\t\tPACKET\tACK[0:DEFAULT, 1:TIMOUT, 2:CRCERR, 3:OK]'
for id in `cat $esl_list`
do
	echo -n -e $id '\t'
	#当前终端所有的ACK条目记录在文件data中
	grep $id $log_file > $esl_data
	#统计帧总数和ACK的成功数和PACKET数
	awk -F: 'BEGIN{FRAME=0;ACK_OK=0;PACKET=0}{FRAME++;if ($8=="PASS") {ACK_OK++;};PACKET+=$9;}END{printf "%d\t%04f\t%d\t", FRAME, ACK_OK/FRAME, PACKET}' $esl_data

	#统计ACK类型
	cut -d'[' -f2 $esl_data | cut -d']' -f1 | sed -e 's/,/\n/g' | grep -v '^$'  | cut -d'-' -f2 | sort | uniq -c | awk '{printf "%s=%d,", $2, $1}'
	echo
done

#删除临时文件
rm -rf $log_file $esl_list $esl_data -rf
