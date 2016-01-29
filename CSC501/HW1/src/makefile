IDIR =../inc
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=../obj
LDIR=../lib

LIBS=-lm

_DEPS = ctx.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = ctx.o mythread.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mythreadmake: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
