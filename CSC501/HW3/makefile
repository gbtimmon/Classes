
CC=gcc
CFlags=-Wall -Werror -pedantic -std=c99
Inc=-I .
Static=-fPIC -c

gom: ./master.c ./potato.o ./socket.o ./connection.o goc
	${CC} ${CFlags} ./master.c ./potato.o ./socket.o ./connection.o ${Inc} -o ./gom

goc: ./bounce.c ./potato.o ./socket.o ./connection.o
	${CC} ${CFlags} ./bounce.c ./potato.o ./socket.o ./connection.o ${Inc} -o ./goc

./potato.o: ./potato.c
	${CC} ${Static} ${CFlags} ./potato.c ${Inc} -o ./potato.o

./socekt.o: ./socket.c
	${CC} ${Static} ${CFlags} ./socket.c ${Inc} -o ./socket.o

./connection.o: ./connection.c
	${CC} ${Static} ${CFlags} ./connection.c ${Inc} -o ./connection.o


