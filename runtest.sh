#!/bin/bash

if [ $# -lt 1 ]
then

    echo "Usage: $0 filename"
    exit 1
fi

FILE=$1
TYPE=$2
ARG=$3
echo "Test file: $FILE"

STATS_FOLDER="./stats"
BIN_FOLDER="./bin"
BIN="${BIN_FOLDER}/${FILE}"


if [[ !(-e ${BIN}) ]];
then
    echo "Binary file: ${BIN} doesn't exits"
    exit 1
fi

if [[ !(-d ${STATS_FOLDER}) ]];
then
    mkdir ${STATS_FOLDER}
fi


echo "Type: ${TYPE}"
command time -f "%U\t%S\t%e\t${ARG}\t%X" ${BIN} ${TYPE} ${ARG} 2>> "${STATS_FOLDER}/${FILE}_${TYPE}.txt"
