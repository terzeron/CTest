CC=gcc
RM=rm

CFLAGS=-g -Wall
LDFLAGS=-Wall -DTEST

EXE=netbios

.PHONY: all clean 

.c.o:
	$(CC) $(CFLAGS) -c $<

all: $(EXE)

clean:
	$(RM) -f *.o *~ $(EXE)
