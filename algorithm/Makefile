CFLAGS=-g -Wall -O0
LDFLAGS=-Wall -g

EXE=brutesearch kmpsearch euclid euclid2 fraction convert

all: $(EXE)

fraction: fraction.o
	$(CC) -o $@ $? $(LDFLAGS) 

convert: convert.o
	$(CC) -o $@ $? $(LDFLAGS) 

euclid: euclid.o
	$(CC) -o $@ $? $(LDFLAGS) 

euclid2: euclid2.o
	$(CC) -o $@ $? $(LDFLAGS) 

clean:
	$(RM) -f *.o *.c~ core *.h~ $(EXE) 
