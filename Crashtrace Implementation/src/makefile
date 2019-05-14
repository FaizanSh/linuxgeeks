IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lunwind-ptrace -lunwind-generic

_DEPS = helpingFunctions.h data.h die.h list.h spawnChild.h stacktrace.h traceChild.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = list.o main.o die.o stacktrace.o helpingFunctions.o spawnChild.o traceChild.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

crashtrace: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

