.SUFFIXES:
.SUFFIXES: .c .h .o

EXES=heap_test collect analyze

CC=gcc
CFLAGS=-O0 -Wall -g 
LDFLAGS=-L/usr/lib/x86_64-linux-gnu/ -ldb -lz

all: $(EXES)

collect: collect.o heap.o
	$(CC) -o $@ collect.o heap.o $(LDFLAGS)

analyze: analyze.o 
	$(CC) -o $@ analyze.o $(LDFLAGS)

heap_test: heap.o heap_test.o
	$(CC) -o $@ ./heap.o heap_test.o $(LDFLAGS)

.c.o:
	$(CC) -c $< $(CFLAGS)

clean:
	$(RM) -f *~ *.o $(EXES)
