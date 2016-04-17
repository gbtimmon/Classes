#!/bin/bash

MT=${1}
rm -fR ${MT}/*
rmdir  ${MT}
mkdir  ${MT}

./greg_fs ${MT} 
ls -ls ${MT};
cat *.log
