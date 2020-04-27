#!/bin/bash

max=$1
for i in $@
do
    if [ $max -lt $i ]
    then
        max=$i
    fi
done
echo $max
