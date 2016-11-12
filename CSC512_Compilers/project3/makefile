CC       = gcc
SRC_PATH = .

INC      = -I /c/MinGW/include -I .
LIB      = -L /c/MinGW/lib
STATIC   = -c
CFLAGS   = -w -Wall -std=c99 -pedantic -g
LINKER   = 

BINARY = compile

all : generateTable.py $(BINARY) cleanup

$(BINARY) : symbol.o token.o scanner.o buffer.o parserTable.o generate.o parser.c transform.o
	$(CC) $(SRC_PATH)/parser.c symbol.o token.o scanner.o buffer.o generate.o transform.o parserTable.o $(INC) $(LIB) $(CFLAGS) $(LINKER) -o $(BINARY)

generateTable.py: 
	python ./generateTable.py

cleanup:
	rm -f ./*.o; rm -f ./*.gch; 

symbol.o : symbol.c
	$(CC) $(STATIC) $(SRC_PATH)/symbol.c $(INC) $(LIB) $(CFLAGS) $(LINKER) -o symbol.o

scanner.o : scanner.c
	$(CC) $(STATIC) $(SRC_PATH)/scanner.c $(INC) $(LIB) $(CFLAGS) $(LINKER) -o scanner.o

generate.o : generate.c
	$(CC) $(STATIC) $(SRC_PATH)/generate.c $(INC) $(LIB) $(CFLAGS) $(LINKER) -o generate.o

buffer.o : buffer.c
	$(CC) $(STATIC) $(SRC_PATH)/buffer.c $(INC) $(LIB) $(CFLAGS) $(LINKER) -o buffer.o

token.o : token.c
	$(CC) $(STATIC) $(SRC_PATH)/token.c $(INC) $(LIB) $(CFLAGS) $(LINKER) -o token.o

transform.o : transform.c
	$(CC) $(STATIC) $(SRC_PATH)/transform.c $(INC) $(LIB) $(CFLAGS) $(LINKER) -o transform.o

parserTable.o : parserTable.c 
	$(CC) $(STATIC) $(SRC_PATH)/parserTable.c $(INC) $(LIB) $(CFLAGS) $(LINKER) -o parserTable.o
