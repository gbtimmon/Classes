
CC=gcc
CFlags=-Wall -Werror -pedantic -std=c99
Inc=-I .
Static=-fPIC -c

go: ./bounce.c ./potato.o ./socket.o
	${CC} ${CFlags} ./bounce.c ./potato.o ./socket.o ${Inc} -o ./go

./potato.o: ./potato.c
	${CC} ${Static} ${CFlags} ./potato.c ${Inc} -o ./potato.o

./socekt.o: ./socket.o
	${CC} ${Static} ${CFlags} ./socket.c ${Inc} -o ./socket.o

