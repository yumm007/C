#!/bin/bash

for testfile in /usr/include/*.h ; do
	./msg_client $testfile  ~/tmp/testfile3.h;
	md5sum $testfile ~/tmp/testfile3.h
	sleep 1
done
