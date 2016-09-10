CC       = gcc
SRC_PATH = .

INC      = -I /c/MinGW/include -I ./include
LIB      = -L /c/MinGW/lib
STATIC   = -c
CFLAGS   = -w -Wall -std=c11 -pedantic
LINKER   = 

BINARY = substring

all : $(SOURCES) $(BINARY)

$(BINARY) : substring.c
	$(CC) $(SRC_PATH)/substring.c $(INC) $(LIB) $(CFLAGS) $(LINKER) -o $(BINARY)
