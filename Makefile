TOPTARGETS=all subdirs
SUBDIRS=algorithm eupa mutexfromsem netbios eupa/semaphore
#CC=gcc
#RM=rm

CCFLAGS=-g 
WARNINGFLAGS=-Wall
INCFLAGS=
LIBFLAGS=-Wall

EXES=cal dos2unix echo2 getch gets mktitle mlname name2url pointer quiz01 quiz02 readit prime_factor test libtimefake.so timefake_test

.PHONY: all clean subdirs $(SUBDIRS)

.c.o:
	$(CC) $(CCFLAGS) -c $< $(WARNINGFLAGS) $(INCFLAGS)

all: $(EXES) $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

cal: cal.o
	$(CC) -o $@ $? $(LIBFLAGS) 
dos2unix: dos2unix.o
	$(CC) -o $@ $? $(LIBFLAGS) 
echo2: echo2.o
	$(CC) -o $@ $? $(LIBFLAGS) 
getch: getch.o
	$(CC) -o $@ $? $(LIBFLAGS) 
gets: gets.o
	$(CC) -o $@ $? $(LIBFLAGS) 
mktitle: mktitle.o
	$(CC) -o $@ $? $(LIBFLAGS) 
mlname: mlname.o
	$(CC) -o $@ $? $(LIBFLAGS) 
name2url: name2url.o
	$(CC) -o $@ $? $(LIBFLAGS) 
pointer: pointer.o
	$(CC) -o $@ $? $(LIBFLAGS) 
quiz01: quiz01.o
	$(CC) -o $@ $? $(LIBFLAGS) 
quiz02: quiz02.o
	$(CC) -o $@ $? $(LIBFLAGS) 
readit: readit.o
	$(CC) -o $@ $? $(LIBFLAGS) 
prime_factor: prime_factor.o
	$(CC) -o $@ $? $(LIBFLAGS) -lm
test: test.o
	$(CC) -o $@ $? $(LIBFLAGS) 
libtimefake.so: timefake.c
	$(CC) -o $@ $? $(LIBFLAGS) -shared -fPIC
timefake_test: timefake_test.o
	$(CC) -o $@ $? $(LIBFLAGS)
	@echo "run 'env LD_PRELOAD=`pwd`/libtimefake.so ./timefake_test'"

clean: $(SUBDIRS)
	$(RM) -f *.o *.c~ core *.h~ $(EXES) 

