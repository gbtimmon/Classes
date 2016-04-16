#!/bin/make 

CC=gcc
CFlags=-Wall -Werror -pedantic -std=c99
Inc=-I . -I /fuse-2.9.3/include
Static=-fPIC -c
Lib=-L /fuse-2.9.3/lib -pthread -lfuse

greg_fs : test.c oper.o
	${CC} ${CFlags} ./test.c ./oper.o ${Inc} ${Lib} -o greg_fs

oper.o: oper.c
	${CC} ${Static} ${CFlags} ./oper.c ${Inc} ${Lib} -o oper.o


