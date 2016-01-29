IDIR =../inc
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=../obj
LDIR=../lib

LIBS=-lm

_DEPS = thread.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = thread.o mythread.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

go: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
