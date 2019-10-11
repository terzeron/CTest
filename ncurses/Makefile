CC=gcc
CFLAGS=-g -Wall -O0
LDFLAGS=-lcurses

EXES=src1 src2 src3 src4 src5 src6
OBJS=$(EXES:=.o)
SRCS=$(OBJS:.o=.c)

all: $(EXES)

src1:	src1.o
	$(CC) -o $@ $? $(LDFLAGS)

src2:	src2.o
	$(CC) -o $@ $? $(LDFLAGS)

src3:	src3.o
	$(CC) -o $@ $? $(LDFLAGS)

src4:	src4.o
	$(CC) -o $@ $? $(LDFLAGS)

src5:	src5.o
	$(CC) -o $@ $? $(LDFLAGS)

src6:	src6.o
	$(CC) -o $@ $? $(LDFLAGS)

clean:
	$(RM) *.o *.c~ $(EXES)
