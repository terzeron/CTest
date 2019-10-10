CFLAGS+=-Wall -g 
LDFLAGS+=-Wall -pthread
EXE=semtest
OBJS=seminit.o semfinal.o semlock.o semunlock.o test.o

.c.o:
	$(CC) -c $< $(CFLAGS)

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(LDFLAGS)

clean:
	$(RM) -f *~ $(EXE) *.o
