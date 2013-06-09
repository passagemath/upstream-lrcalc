CC=gcc #-pipe
CFLAGS=-O6 -I ../mathlib -Wall # -DDEBUG
#CFLAGS=-Wall -g -DDEBUG -DSAGE -I ../mathlib
LDFLAGS=-s
LDLIBS=../mathlib/mathlib.a

OBJS=symfcn.o maple.o

PROGS=lrcalc lrcoef skew mult coprod lrskew sat

all: $(PROGS)

lrcalc: lrcalc.o $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) lrcalc.o $(LDLIBS) -o lrcalc

lrcoef: lrcoef.o $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) lrcoef.o $(LDLIBS) -o lrcoef

skew: skew.o $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) skew.o $(LDLIBS) -o skew

mult: mult.o $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) mult.o $(LDLIBS) -o mult

coprod: coprod.o $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) coprod.o $(LDLIBS) -o coprod

lrskew: lrskew.o $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) lrskew.o $(LDLIBS) -o lrskew

sat: sat.o $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) sat.o $(LDLIBS) -o sat

clean:
	rm -f $(PROGS) core *.a *.o

realclean:
	rm -f $(PROGS) core *.a *.o *.out *~ dot.* dependencies *.s

backup:
	./mkbackup.sh "Backup/weyl-%y%m%d-%H%M%S.tar"

dep:
	gcc -MM $(CFLAGS) *.c > dependencies

dependencies:
	gcc -MM $(CFLAGS) *.c > dependencies

include dependencies
