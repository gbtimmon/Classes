#!/bin/bash

MT=${1}
if [ "X${1}" = "X" ]; then 
   exit 1; 
fi

mkdir ${MT}
echo
echo GREG_FS
./greg_fs ${MT} 
echo 
echo LS
ls -ls ${MT};
echo MKDIR
mkdir ${MT}/f1
echo LS2
ls -ls ${MT}; 
echo 
echo CAT LOG
cat *.log
