#!/bin/bash
sed -e 's/-//g' $1 | awk '{printf "insert into TagUploadInfo(Barcode,EslId,Status,InsertDate,Complete) VALUES(\"%s\",\"%s\",\"创建\",\"2013-04-22 13:10:00\",0);\n",  $1, substr($2, 1, 6)}' | sqlite3 $2

