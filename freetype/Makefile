LDFLAGS=-lfreetype -lz #-no-undefined
CFLAGS=-Wall -I/usr/include/freetype2 -g -O0
CC=gcc

EXE=freetype_test
OBJS=freetype_test.o
SRCS=$(OBJ:.o=.c)

all: $(EXE)

$(EXE): $(OBJS)
	$(LIBTOOL) $(CC) -o $@ $< $(LDFLAGS)

.c.o:
	$(CC) -c $< $(CFLAGS)

clean:
	$(RM) *.o *~ *.exe $(EXE)

