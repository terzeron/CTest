TOPTARGETS=all 

CFLAGS=-g -Wall
LDFLAGS=-Wall -lgdbm

EXES=count

.PHONY: all clean 

all: $(EXES) 

.c.o:
	$(CC) $(CFLAGS) -c $< 

$(EXES): count.o
	$(CC) -o $@ $? $(LDFLAGS)

clean: 
	$(RM) -f *.o *.c~ core *.h~ $(EXES) 

