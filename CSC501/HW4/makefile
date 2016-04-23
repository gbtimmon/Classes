#!/bin/make 

CC     =gcc
CFlags =-Wall -Werror -pedantic -std=c99
Inc    =-I . -I /fuse-2.9.3/include
Static =-fPIC -c
Lib    =-L /fuse-2.9.3/lib -pthread -lfuse

greg_fs : ramdisk.c global.o oper.o file.o
	${CC} ${CFlags} ./ramdisk.c ./oper.o ./global.o ./file.o ${Inc} ${Lib} -o ramdisk

oper.o: oper.c 
	${CC} ${Static} ${CFlags} ./oper.c   ${Inc} ${Lib} -o oper.o

global.o: global.c
	${CC} ${Static} ${CFlags} ./global.c ${Inc} ${Lib} -o global.o

file.o : file.c
	${CC} ${Static} ${CFlags} ./file.c   ${Inc} ${Lib} -o file.o

