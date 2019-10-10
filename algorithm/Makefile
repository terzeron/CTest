CC = gcc
RM = rm

CCFLAGS = -g 
WARNINGFLAGS = -Wtraditional
INCFLAGS = 
LIBFLAGS = 

EXE = brutesearch.o kmpsearch.o euclid euclid2 fraction convert
.c.o:
	$(CC) $(CCFLAGS) -c $< $(WARNINGFLAGS) $(INCFLAGS)

all:	$(EXE)

fraction: fraction.o
	$(CC) -o $@ $? $(LIBFLAGS) 
convert: convert.o
	$(CC) -o $@ $? $(LIBFLAGS) 
euclid: euclid.o
	$(CC) -o $@ $? $(LIBFLAGS) 
euclid2: euclid2.o
	$(CC) -o $@ $? $(LIBFLAGS) 

clean:
	$(RM) -f *.o *.c~ core *.h~ $(EXE) 
