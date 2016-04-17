#!/bin/bash

MT=${1}
if [ "X${1}" = "X" ]; then 
   exit 1; 
fi

echo RM
rm -fR ${MT}/*
echo 
echo RMDIR
rmdir  ${MT}
echo 
echo MKDIR
mkdir  ${MT}
echo
echo GREG_FS
./greg_fs ${MT} 
echo 
echo LS
ls -ls ${MT};
echo 
echo CAT LOG
cat *.log
