#!/bin/ksh
#Script for running out algorithm
if(($# != 2)) then
   echo "USAGE: run.ksh <input_graph> <output_file>"
   exit 1
fi

echo "compiling ... "
g++ -I . -std=c++0x ClusterTest.cpp -O3
echo "running ... "
./a.out ${1} > ${2}
echo "done!"
