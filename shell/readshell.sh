#!/bin/bash
read -p "请输入帐号：" acct
stty -echo
read -p "请输入密码: " pass; echo
stty echo
echo -e "$acct \t $pass"


