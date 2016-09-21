CC       = gcc
SRC_PATH = .

INC      = -I /c/MinGW/include -I ./include
LIB      = -L /c/MinGW/lib
STATIC   = -c
CFLAGS   = -w -Wall -std=c11 -pedantic
LINKER   = 

BINARY = scanner

all : $(SOURCES) $(BINARY)

$(BINARY) : scanner.c
	$(CC) $(STATIC) $(SRC_PATH)/scanner.c $(INC) $(LIB) $(CFLAGS) $(LINKER) -o scanner.o
