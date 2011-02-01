#!/bin/bash
#此脚本调用passwd更改用户密码，必须使用root身份执行,不需做密码复杂度要求.
#2010-08-23 17:35 Yu

#判断参数输入的合法性
if [ $# -gt 1 ]
then
	echo passwd: Only one user name may be specified.
else
	#参数小于或等于1
	if [ $# -ge 1 ]	
	then     
		user=$1
	else
		user=$USER
	fi
    #判断输入的用户是否存在
	if [ ` cut -d: -f1 /etc/passwd | grep ^$user$ | wc -l` -eq 0 ]
	then
		echo passwd: Unknown user name \'$user\'.
	else
		#密码输入验证.循环三次，只要两次密码输入匹配则调用passwd更改密码，
		#并退出循环
		echo Changing password for user $user.
		for i in 1 2 3
		do
			changed=0
			stty -echo
			trap '' INT
			read -p "New UNIX passwd: " temppass1
			echo
			read -p "Retype new UNIX password: " temppass2
			echo
			stty echo
			if [ $temppass1 = $temppass2 ]
			then
				#开始改密码
				echo $temppass1 | passwd --stdin $user
				changed=1
				break
			else
				echo Sorry, passwords do not match.
			fi	
		done
		trap INT
		#如果尝试三次仍然失败
		if [ $changed -ne 1 ]
		then
			sleep 2
			echo passwd: Authentication information cannot be recovered
		fi
	fi
fi

