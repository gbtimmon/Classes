
CC=gcc
CFlags=-Wall -Werror -pedantic -std=c99 
Inc=-I ./inc
Static=-fPIC -c

ush : ./src/ush.c
	$(CC) $(CFlags) ./src/ush.c ./obj/parse.o $(Inc) -o ./ush

go : ./src/main.c
	$(CC) $(CFlags) ./src/main.c  ./obj/parse.o $(Inc) -o ./go

parse.o : ./src/parse.c
	$(CC) $(CFlags) $(Static)  ./src/parse.c $(Inc) -o ./obj/parse.o
