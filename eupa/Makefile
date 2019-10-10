CC=gcc
RM=rm

CFLAGS=-g -Wall
LDFLAGS=-Wall -pthread

EXE=jmp_test1 jmp_test2 jmp_test3 jmp_test4 sem_test01 sem_test02 setjmp sighandler eupa0105

.PHONY: all clean 

.c.o:
	$(CC) $(CFLAGS) -c $<

all: $(EXE)

sem_test01: sem_test01.o
	$(CC) $(LDFLAGS) -o $@ $?

clean:
	$(RM) -f *.o *~ $(EXE)
