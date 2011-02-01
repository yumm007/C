#!/bin/bash
#Usage: ./aaddb.sh a [+-*/%] b
#脚本输出并返回计算结果.结果为0时
#2010-08-23 yumm_007@163.com

#判断输入的数值和运算符是否合格
if [ `echo $1 | grep [^0-9-] | wc -l` -eq 0 -a `echo $3 | grep [^0-9-] | wc -l` -eq 0 \
	-a `echo "$2" | grep [\-\+\*/%] | wc -l` -eq 1 ]
then
		unset c
		case $2 in
			+)
				c=`expr $1 + $3`
				;;
			-)
				c=`expr $1 - $3`
				;;
			"*")
				c=`expr $1 \* $3`
				;;
			/)
				c=`expr $1 / $3`
				;;
			%)
				c=`expr $1 % $3`
				;;
		esac
else
		echo Usage: ./aaddb.sh INTEGER1 \[+-\*/%\] INTEGER2
fi
#如果c被设置，则说明有计算结果，并输出
${c:+echo $c}
