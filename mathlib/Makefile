CC=gcc #-pipe
CFLAGS=-O6 -Wall # -DDEBUG
#CFLAGS=-Wall -g -DDEBUG

MATHLIB=mathlib.a

OBJS=alloc.o list.o vectarg.o vector.o hashtab.o set.o

def: $(MATHLIB)

all: $(MATHLIB) getmax

mathlib.a: $(OBJS)
	ar cru mathlib.a $(OBJS)
	ranlib mathlib.a

LDLIBS=mathlib.a

getmax: getmax.o mathlib.a

clean:
	rm -f $(MATHLIB) core *.a *.o *.out

realclean:
	rm -f $(MATHLIB) core *.a *.o *.out *~ dot.* dependencies *.s

backup:
	./mkbackup.sh "Backup/schur-%y%m%d-%H%M%S.tar"

dep:
	gcc -MM *.c > dependencies

dependencies:
	gcc -MM *.c > dependencies

include dependencies
