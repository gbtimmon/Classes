CC       = gcc
SRC_PATH = .

INC      = -I /c/MinGW/include -I .
LIB      = -L /c/MinGW/lib
STATIC   = -c
CFLAGS   = -w -Wall -std=c99 -pedantic -g
LINKER   = 

BINARY = parser

all : generateTable.py $(BINARY) cleanup

$(BINARY) : token.o scanner.o buffer.o parserTable.o parser.c transform.o
	$(CC) $(SRC_PATH)/parser.c token.o scanner.o buffer.o transform.o parserTable.o $(INC) $(LIB) $(CFLAGS) $(LINKER) -o $(BINARY)

generateTable.py: 
	python ./generateTable.py

cleanup:
	rm -f ./*.o; rm -f ./*.gch; 

scanner.o : scanner.c
	$(CC) $(STATIC) $(SRC_PATH)/scanner.c $(INC) $(LIB) $(CFLAGS) $(LINKER) -o scanner.o

buffer.o : buffer.c
	$(CC) $(STATIC) $(SRC_PATH)/buffer.c $(INC) $(LIB) $(CFLAGS) $(LINKER) -o buffer.o

token.o : token.c
	$(CC) $(STATIC) $(SRC_PATH)/token.c $(INC) $(LIB) $(CFLAGS) $(LINKER) -o token.o

transform.o : transform.c
	$(CC) $(STATIC) $(SRC_PATH)/transform.c $(INC) $(LIB) $(CFLAGS) $(LINKER) -o transform.o

parserTable.o : parserTable.c 
	$(CC) $(STATIC) $(SRC_PATH)/parserTable.c $(INC) $(LIB) $(CFLAGS) $(LINKER) -o parserTable.o
