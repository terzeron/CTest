TOPTARGETS=all

CFLAGS=-g -Wall
LDFLAGS=-Wall -lpthread

EXES=test1 test2 test3 test4 thread_test condvar1 condvar2 tsd

.PHONY: all clean 

all: $(EXES) 

.c.o:
	$(CC) $(CFLAGS) -c $< 

test1: test1.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

test2: test2.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

test3: test3.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

test4: test4.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

thread_test: thread_test.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

tsd: tsd.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

condvar1: condvar1.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

condvar2: condvar2.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

clean: 
	$(RM) -f *.o *.c~ core *.h~ $(EXES) 

