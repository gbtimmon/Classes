#!/bin/bash

MT=${1}
if [ X$MT = X ]; then
    MT=~/mnt
fi 
  
rm -fR ${MT}/*
rmdir  ${MT}
mkdir  ${MT}

./greg_fs ${MT} 
