#!/bin/bash

#output file
log_file=temp_file
esl_list=esl_list_file
esl_data=esl_data_file

#找到符合要求的所有log条目
grep '^[$]\{3\}' test_log > $log_file
#生成终端ID清单
esl_list=`cut -d':' -f7 $log_file | sort | uniq`

for id in $esl_list
do
	echo -n ID=$id
	#当前终端所有的ACK条目记录在文件data中
	grep $id $log_file | grep -E '*:*:*:3:*' > $esl_data
	#统计帧总数和ACK的成功数和PACKET数
	awk -F: 'BEGIN{FRAME=0;ACK_OK=0;PACKET=0}{FRAME++;if ($8=="OK") {ACK_OK++;};PACKET+=$9;}END{printf ":FRAME=%d:OK=%d%%:PACKET=%d:", FRAME, ACK_OK*100/FRAME, PACKET}' $esl_data

	#统计ACK类型
	cut -d'[' -f2 $esl_data | sed -e 's/\];//' -e 's/,/\n/    g' | cut -d'-' -f3 | sort | uniq -c | awk '{printf "%s=%d,", $2, $1}'
	#输出结果 ESL:帧总数:帧成功率:RF包总数:RF包总类型分布
	echo
done

#删除临时文件
rm -rf $log_file $esl_list $esl_data -rf
