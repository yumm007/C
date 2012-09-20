#!/bin/bash

cseq=$$
branch=`date +%N | md5sum | cut -d' ' -f1`
tag=`echo $cseq | md5sum | cut -d' ' -f1`
call_id=`grep MemFree /proc/meminfo | md5sum | cut -d' ' -f1`

branch=`echo -n $branch$tag | cut -b 1-41`
ip=`ifconfig wlan0 | grep "inet addr" | cut -d':' -f2 | cut -d' ' -f1`
port="5060"

server_ip="210.51.10.237"
sip_id="8204"
sip_pw="8204"

# output 
outfile="register"
echo "REGISTER sip:$server_ip SIP/2.0" > $outfile
echo "Via: SIP/2.0/UDP "$ip":"$port";rport;branch="$branch >> $outfile
echo "Max-Forwards: 70" >> $outfile
echo "From: <sip:$sip_id@$server_ip>;tag="$tag >> $outfile
echo "To: <sip:$sip_id@$server_ip>" >> $outfile
echo "Call-ID: "$call_id >> $outfile
echo "CSeq: "$cseq" REGISTER" >> $outfile
echo "Contact: <sip:$sip_id@"$ip"":"$port;ob>" >> $outfile
echo "Expires: 300" >> $outfile
echo "Allow: PRACK, INVITE, ACK, BYE, CANCEL, UPDATE, SUBSCRIBE, NOTIFY, REFER, MESSAGE, OPTIONS" >> $outfile
echo "Content-Length:  0" >> $outfile
echo  >> $outfile

outfile="response"

branch=`date +%N | md5sum | cut -d' ' -f1`
branch=`echo -n $branch$tag | cut -b 1-41`
cseq=`echo "$cseq + 1" | bc`

echo "REGISTER sip:$server_ip SIP/2.0" > $outfile
echo "Via: SIP/2.0/UDP "$ip":"$port";rport;branch="$branch >> $outfile
echo "Max-Forwards: 70" >> $outfile
echo "From: <sip:$sip_id@$server_ip>;tag="$tag >> $outfile
echo "To: <sip:$sip_id@$server_ip>" >> $outfile
echo "Call-ID: "$call_id >> $outfile
echo "CSeq: $cseq REGISTER" >> $outfile
echo "Contact: <sip:$sip_id@"$ip"":"$port;ob>" >> $outfile
echo "Expires: 300" >> $outfile
echo "Allow: PRACK, INVITE, ACK, BYE, CANCEL, UPDATE, SUBSCRIBE, NOTIFY, REFER, MESSAGE, OPTIONS" >> $outfile
echo "Authorization: Digest username=\"$sip_id\", realm=\"freeiris2\", nonce=\"4d788e6e\", uri=\"sip:$server_ip\", response=\"5810eccc12ea4e5d58b66f05b944d1cb\", algorithm=MD5" >> $outfile
echo "Content-Length:  0" >> $outfile
echo  >> $outfile

outfile=md5.sh

echo username=\"8204\" > $outfile 
echo password=\"8204\" >> $outfile 
echo realm=\"freeiris2\" >> $outfile 
echo method=\"REGISTER\" >> $outfile 
echo uri=\"sip:210.51.10.237\" >> $outfile 
echo 'nonce=$1' >> $outfile 
echo  >> $outfile 
echo 'val1=`echo -n $username:$realm:$password | md5sum | cut -d" " -f1` ' >> $outfile 
echo 'val2=`echo -n $method:$uri | md5sum | cut -d" " -f1` ' >> $outfile 
echo 'val=`echo  -n $val1:$nonce:$val2 | md5sum | cut -d" " -f1` ' >> $outfile 
echo cd  `pwd` >> $outfile
echo 'sed -e s/4d788e6e/$1/ -e s/5810eccc12ea4e5d58b66f05b944d1cb/$val/ response -i' >> $outfile
chmod +x $outfile

