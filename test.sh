#!/bin/bash

if [ $# -lt 1 ]
then

    echo "Usage: $0 filename"
    exit 1
fi

echo "Test file: $1"

for i in 4
do
    ./bin/$1 $i
done
