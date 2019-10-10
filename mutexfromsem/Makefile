EXE=mutex
OBJS=mutex.o semops.o
CFLAGS=-g -Wall
LDFLAGS=-Wall -pthread

.c.o:
	$(CC) -c $< $(CFLAGS)

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) -o $@ $? $(LDFLAGS)

clean:
	$(RM) -f *~ *.o $(EXE) 
