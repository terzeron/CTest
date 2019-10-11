TOPTARGETS=all subdirs
SUBDIRS=algorithm eupa mutexfromsem netbios eupa/semaphore gdbm

CFLAGS=-g -Wall
LDFLAGS=-Wall

EXES=cal dos2unix echo2 getch gets mktitle mlname name2url pointer quiz01 quiz02 readit prime_factor test libtimefake.so timefake_test binary_search allenter date locked lotto shm shm1 shm2 struct_copy time w2xedit

.PHONY: all clean subdirs $(SUBDIRS)

all: $(EXES) $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.c.o:
	$(CC) $(CFLAGS) -c $< 

cal: cal.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

dos2unix: dos2unix.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

echo2: echo2.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

getch: getch.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

gets: gets.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

mktitle: mktitle.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

name2url: name2url.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

pointer: pointer.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

quiz01: quiz01.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

quiz02: quiz02.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

readit: readit.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

test: test.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

prime_factor: prime_factor.o
	$(CC) -o $@ $? $(LDFLAGS) -lm

libtimefake.so: timefake.c
	$(CC) -o $@ $? $(LDFLAGS) -shared -fPIC

timefake_test: timefake_test.o
	$(CC) -o $@ $? $(LDFLAGS)
	@echo "run 'env LD_PRELOAD=`pwd`/libtimefake.so ./timefake_test'"

allenter: allenter.o
	$(CC) -o $@ $? $(LDFLAGS)

binary_search: binary_search.o
	$(CC) -o $@ $? $(LDFLAGS)

date: date.o
	$(CC) -o $@ $? $(LDFLAGS)

locked: locked.o
	$(CC) -o $@ $? $(LDFLAGS)

lotto: lotto.o
	$(CC) -o $@ $? $(LDFLAGS)

mlname: mlname.o
	$(CC) -o $@ $? $(LDFLAGS)

shm: shm.o
	$(CC) -o $@ $? $(LDFLAGS)

shm1: shm1.o
	$(CC) -o $@ $? $(LDFLAGS)

shm2: shm2.o
	$(CC) -o $@ $? $(LDFLAGS)

struct_copy: struct_copy.o
	$(CC) -o $@ $? $(LDFLAGS)

time: time.o
	$(CC) -o $@ $? $(LDFLAGS)

w2xedit: w2xedit.o
	$(CC) -o $@ $? $(LDFLAGS)

clean: $(SUBDIRS)
	$(RM) -f *.o *.c~ core *.h~ $(EXES) 

