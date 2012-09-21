for i in `ls`
do
	../sip-client $i > temp
	md5sum $i temp
	echo
done
@rm -rf temp
