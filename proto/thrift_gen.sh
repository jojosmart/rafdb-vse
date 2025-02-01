#!/bin/sh
for file in `ls *.thrift`
do
    echo $file
    ../third_party/thrift/bin/thrift --gen cpp $file
done

