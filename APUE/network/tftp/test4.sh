#!/bin/bash

for testfile in /usr/include/*.h ; do
	./msg_client $testfile  ~/tmp/testfile4.h;
	md5sum $testfile  ~/tmp/testfile4.h
	sleep 1
done
