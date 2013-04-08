#!/bin/bash

#output file
log_file=temp_file
esl_list=esl_list_file
esl_data=esl_data_file

echo -e '\nData Packet'
#找到符合要求的所有log条目 $$$:x:x:3:7:*, 但排除case13
grep -a -E '^[^:]{3}:[^:]{1,}:[^:]{1,}:3:7:*' $1 | grep -v -E '^[^:]{3}:[^:]{1,}:13:3:7:*' > $log_file
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

echo -e '\nQueue Packet'
grep -a -E '^[^:]{3}:[^:]{1,}:[^:]{1,}:3:8:*' $1 > $log_file
echo -e 'ID\t\t\tFRAME\tPASS'
for id in `cat $esl_list`
do
	echo -n -e $id '\t'
	grep $id $log_file > $esl_data
	awk -F: 'BEGIN{FRAME=0;ACK_OK=0}{FRAME++; if (match($8, "PASS")) {ACK_OK++;};}END{printf "%d\t%04f\n", FRAME, ACK_OK/FRAME}' $esl_data
done

echo -e '\nNetlink Packet'
grep -a -E '^[^:]{3}:[^:]{1,}:[^:]{1,}:3:9:*' $1 > $log_file
echo -e 'ID\t\t\tFRAME\tPASS'
for id in `cat $esl_list`
do
	echo -n -e $id '\t'
	grep $id $log_file > $esl_data
	awk -F: 'BEGIN{FRAME=0;ACK_OK=0}{FRAME++; if (match($8, "PASS")) {ACK_OK++;};}END{printf "%d\t%04f\n", FRAME, ACK_OK/FRAME}' $esl_data
done

#统计每个case的成功率, 第3个打印点，第6种统计数
#$$$:23:10:3:6:ESL000:ID=0xa1,0x01,0x02,0x01:PASS(0x04);355;
echo -e '\nCase'
for ((i = 0; i < 15; i++))
do
	echo -n -e $i: '\t'
	grep -a -E "^[^:]{3}:[^:]{1,}:$i:3:6:*" $1 | awk -F: 'BEGIN{OK=0;TOTAL=0}{TOTAL++; if (match($8,"PASS")) {OK++;}; } END{printf "%4f\n", OK/TOTAL}'
done

#统计每种错误的概率，第2个打印点，第6中统计数，为FAIL的
#但要排除case13：随机内容测试
echo -e '\nFailed Reason'
grep -a -E "^[^:]{3}:[^:]{1,}:[^:]{1,}:3:6:*" $1 | grep -v -E '^[^:]{3}:[^:]{1,}:13:3:6:*' | grep "FAIL" | cut -d':' -f8 | cut -b6-9 | sort | uniq -c

#删除临时文件
rm -rf $log_file $esl_list $esl_data -rf
