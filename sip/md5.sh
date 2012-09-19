#!/bin/bash

username="8204"
password="8204"
realm="freeiris2"
method="REGISTER"
uri="sip:210.51.10.237"
nonce="36e4f56d"

val1=`echo -n $username:$realm:$password | md5sum | cut -d' ' -f1`
val2=`echo -n $method:$uri | md5sum | cut -d' ' -f1`
val=`echo  -n $val1:$nonce:$val2 | md5sum | cut -d' ' -f1`

echo $val1 "+" $val2 "+" $val
