#!/bin/bash
#Select测试

select i in 'ls' 'date' 'exit'
do
	$i
done

