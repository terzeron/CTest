CFLAGS=-g -Wall
LDFLAGS=-Wall -lpthread

EXES=thread_test tsd

.PHONY: all clean 

all: $(EXES)

.c.o:
	$(CC) $(CFLAGS) -c $< 

thread_test: thread_test.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

tsd: tsd.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

clean:
	$(RM) -f *.o *.c~ core *.h~ $(EXES) 

