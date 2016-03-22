
CC=gcc
CFlags=-Wall -Werror -pedantic -std=c99
Inc=-I .
Static=-fPIC -c

gom: ./master.c ./potato.o ./socket.o ./connection.o goc
	${CC} ${CFlags} ./master.c ./potato.o ./socket.o ./connection.o ${Inc} -o ./master

goc: ./player.c ./potato.o ./socket.o ./connection.o
	${CC} ${CFlags} ./player.c ./potato.o ./socket.o ./connection.o ${Inc} -o ./player

./potato.o: ./potato.c
	${CC} ${Static} ${CFlags} ./potato.c ${Inc} -o ./potato.o

./socket.o: ./socket.c
	${CC} ${Static} ${CFlags} ./socket.c ${Inc} -o ./socket.o

./connection.o: ./connection.c
	${CC} ${Static} ${CFlags} ./connection.c ${Inc} -o ./connection.o


./random.o : ./random.c
	${CC} ${Static} ${CFlags} ./random.c ${Inc} -o ./random.o

