#!/bin/bash
sed -e 's/-//g' $1 | awk '{printf "insert into TagUploadInfo(Barcode,EslId,Complete) VALUES('%s','%s',0);\n",  $1, substr($2, 1, 6)}' | sqlite3 $2

