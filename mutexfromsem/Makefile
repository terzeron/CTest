CFLAGS=-g -Wall
LDFLAGS=-Wall -pthread

EXE=mutex
OBJS=mutex.o semops.o

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) -o $@ $? $(LDFLAGS)

clean:
	$(RM) -f *~ *.o $(EXE) 
