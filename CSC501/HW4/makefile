#!/bin/make 

CC=gcc
CFlags=-Wall -Werror -pedantic -std=c99 -D_FILE_OFFSET_BITS=64
Inc=-I/fuse-2.9.3/include
Static=-fPIC -c

greg_fs : test.c
	${CC} ${CFlags} ./test.c ${Inc} -o greg_fs
