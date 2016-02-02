IDIR =../inc
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=../obj
LDIR=../lib

LIBS=-lm

_DEPS = mythread.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = mythread.o test.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -Wall -Werror -c -o $@ $< $(CFLAGS)

go: $(OBJ)
	$(CC) -Wall -Werror -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
