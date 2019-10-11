CFLAGS=-g -Wall
LDFLAGS=-Wall -lpthread

EXE=longjmp jmp_test1 jmp_test2 jmp_test3 jmp_test4 sem_test01 sem_test02 setjmp sighandler eupa0105

.PHONY: all clean 

.c.o:
	$(CC) -c $< $(CFLAGS)

all: $(EXE)

longjmp: longjmp.o
	$(CC) -o $@ $? $(LDFLAGS)

jmp_test1: jmp_test1.o
	$(CC) -o $@ $? $(LDFLAGS)

jmp_test2: jmp_test2.o
	$(CC) -o $@ $? $(LDFLAGS)

jmp_test3: jmp_test3.o
	$(CC) -o $@ $? $(LDFLAGS)

jmp_test4: jmp_test4.o
	$(CC) -o $@ $? $(LDFLAGS)

sem_test01: sem_test01.o
	$(CC) -o $@ $? $(LDFLAGS)

sem_test02: sem_test02.o
	$(CC) -o $@ $? $(LDFLAGS)

setjmp: setjmp.o
	$(CC) -o $@ $? $(LDFLAGS)

sighandler: sighandler.o
	$(CC) -o $@ $? $(LDFLAGS)

eupa0105: eupa0105.o
	$(CC) -o $@ $? $(LDFLAGS)

clean:
	$(RM) -f *.o *~ $(EXE)
