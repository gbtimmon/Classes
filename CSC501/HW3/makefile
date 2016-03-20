
CC=gcc
CFlags=-Wall -Werror -pedantic -std=c99
Inc=-I .
Static=-fPIC -c

go: ./bounce.c ./potato.o ./socket.o ./connection.o
	${CC} ${CFlags} ./bounce.c ./potato.o ./socket.o ./connection.o ${Inc} -o ./go

./potato.o: ./potato.c
	${CC} ${Static} ${CFlags} ./potato.c ${Inc} -o ./potato.o

./socekt.o: ./socket.c
	${CC} ${Static} ${CFlags} ./socket.c ${Inc} -o ./socket.o

./connection.o: ./connection.c
	${CC} ${Static} ${CFlags} ./connection.c ${Inc} -o ./connection.o


