
CC=gcc
CFlags=-Wall -Werror -pedantic -std=c99
Inc=-I .
Static=-fPIC -c

go: ./test.c ./potato.o
	${CC} ${CFlags} ./test.c ./potato.o ${Inc} -o ./go

./potato.: ./potato.c
	${CC} ${Static} ${CFlags} ./potato.c ${Inc} -o ./potato.o

