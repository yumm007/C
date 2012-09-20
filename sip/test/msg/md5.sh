username="8204"
password="8204"
realm="freeiris2"
method="REGISTER"
uri="sip:210.51.10.237"
nonce=$1

val1=`echo -n $username:$realm:$password | md5sum | cut -d" " -f1` 
val2=`echo -n $method:$uri | md5sum | cut -d" " -f1` 
val=`echo  -n $val1:$nonce:$val2 | md5sum | cut -d" " -f1` 
cd /home/yu/code/c/sip/msg
sed -e s/4d788e6e/$1/ -e s/5810eccc12ea4e5d58b66f05b944d1cb/$val/ response -i
