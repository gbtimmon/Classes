#!/bin/make 

CC=gcc
CFlags=-Wall -Werror -pedantic -std=c99
Inc=-I . -I /fuse-2.9.3/include
Static=-fPIC -c
Lib=-L /fuse-2.9.3/lib -pthread -lfuse

greg_fs : test.c operations.o
	${CC} ${CFlags} ./test.c ./operations.o ${Inc} ${Lib} -o greg_fs

operations.o: operations.c
	${CC} ${Static} ${CFlags} ./operations.c ${Inc} ${Lib} -o operations.o


