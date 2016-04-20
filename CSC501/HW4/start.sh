#!/bin/bash

function L {
   echo "START_SCRIPT ############"  >> ./greg_fs.log
   echo "#    ${1}" >> ./greg_fs.log
   echo "#########################"  >> ./greg_fs.log
}
MT=${1}
if [ "X${1}" = "X" ]; then 
   exit 1; 
fi

mkdir ${MT}
./greg_fs ${MT} 
ls -ls ${MT};

mkdir ${MT}/d1
mkdir ${MT}/d2
mkdir ${MT}/d3
mkdir ${MT}/d4
mkdir ${MT}/d2 #dup dir
rmdir ${MT}/d3 #rmdir
rmdir ${MT}/d4 #rm and recreate dir
mkdir ${MT}/d4

> ${MT}/f1
> ${MT}/f2
> ${MT}/f3
> ${MT}/f4

> ${MT}/d1/f1
> ${MT}/d1/f2
> ${MT}/d1/f3
> ${MT}/d1/f4

mkdir ${MT}/d1/d1

> ${MT}/d1/d1/f1
> ${MT}/d1/d1/f2
> ${MT}/d1/d1/f3
> ${MT}/d1/d1/f4

echo test > ${MT}/test_file
echo test2 >> ${MT}/test_file

