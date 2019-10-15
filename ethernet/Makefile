CFLAGS=-g -c -Wall
LDFLAGS=-g -Wall

EXES=ethernet_test bit_pack_test udpsend
OBJS=util.o udpsend.o bit_pack_test.o main.o
SRCS=$(OBJS:.o=.c)

.c.o:
	$(CC) -c $< $(CFLAGS)

all: $(EXES)
	@chown root $(EXES)
	@chmod u+s $(EXES)

ethernet_test: util.o main.o
	$(CC) -o $@ $? $(LDFLAGS) 

bit_pack_test: bit_pack_test.o
	$(CC) -o $@ $? $(LDFLAGS) 

udpsend: udpsend.o
	$(CC) -o $@ $? $(LDFLAGS) 

clean:
	$(RM) *~ *.o  $(EXES) *.core
