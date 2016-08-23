
CC=gcc
CFlags=-Wall -Werror -pedantic -std=c99 
Inc=-I ./inc
Static=-fPIC -c

ush : ./src/vsh.c ./obj/builtins.o ./obj/parse.o ./obj/util.o ./obj/job.o
	$(CC) $(CFlags) ./src/vsh.c ./obj/parse.o ./obj/builtins.o ./obj/util.o ./obj/job.o $(Inc) -o ./ush

./obj/util.o : ./src/util.c
	$(CC) $(CFlags) $(Static)  ./src/util.c $(Inc) -o ./obj/util.o

./obj/job.o : ./src/job.c
	$(CC) $(CFlags) $(Static)  ./src/job.c $(Inc) -o ./obj/job.o

./obj/builtins.o : ./src/builtins.c
	$(CC) $(CFlags) $(Static)  ./src/builtins.c $(Inc) -o ./obj/builtins.o

./obj/parse.o : ./src/parse.c
	$(CC) $(CFlags) $(Static)  ./src/parse.c $(Inc) -o ./obj/parse.o

