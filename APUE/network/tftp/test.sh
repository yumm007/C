#!/bin/bash

for testfile in /usr/include/*.h ; do
	./msg_client $testfile  ~/tmp/testfile.h;
	md5sum $testfile  ~/tmp/testfile.h
	sleep 1
done
