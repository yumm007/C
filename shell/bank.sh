#!/bin/bash
#银行ATM机模拟界面
#2010-08-23 yumm_007@163.com

funprintmenu(){
	#清屏幕并且显示主菜单
	clear
	echo "----------------------------"
	echo "****************************"
	echo "*     1) 余额查询          *"
	echo "*     2) 存    款          *"
	echo "*     3) 取    款          *"
	echo "*     4) 取    卡          *"
	echo "****************************"
}

#显示余额函数
funprintsub(){
	#echo
	#funprintmenu
	echo "当前余额为:$total"
	read -n 1 -p  "按任意键返回"
}

#存款函数
funadd(){
	#echo
	#funprintmenu
	read -p "请输入存款金额:" a
	if [ `echo $a | grep [^0-9\.] | wc -l` -eq 0 ]; then
			total=`echo $total + $a | bc`
			echo $total > /tmp/back.db
			echo -n "存款成功!"
			funprintsub
	else
		read -p "输入金额: $a 格式错误"
	fi
}

#取款函数
fundel(){
	#echo
	#funprintmenu
	read -p "请输入取款金额:" a
	if [ `echo $a | grep [^0-9\.] | wc -l` -eq 0 ]; then
		if [ `echo $a \<= $total | bc` -eq 1 ]; then
			total=`echo $total - $a | bc`
			echo -n "取款成功!"
			echo $total > /tmp/back.db
			funprintsub
		else
			read -p "输入金额: $a 超出允许金额"
		fi
	else
		read -p "输入金额: $a 格式错误"
	fi
}

#程序从这里开始运行

#数据库文件
if [ ! -f /tmp/back.db ]; then
	echo 1000 > /tmp/back.db	
fi
read total < /tmp/back.db

#主程序无限循环
while [ : ]
do
funprintmenu
read -p "请按数字键1,2,3,4选择:" choose
case $choose in
	1)
		funprintsub
		;;
	2)
		funadd
		;;
	3)
		fundel
		;;
	4)
		clear
		exit
		;;
	*)
		echo 输入有误，请按数字键1,2,3,4
		sleep 2
		;;
esac
done

