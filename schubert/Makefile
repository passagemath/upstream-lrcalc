CC=gcc -pipe
CFLAGS=-Wall -g -DDEBUG -I ../mathlib -I ../figlib -DFAIL_SIGNALS
CFLAGS=-Wall -O6 -I ../mathlib -I ../figlib #-DFAIL_SIGNALS #-DDEBUG
#CFLAGS=-Wall -g -I ../mathlib -I ../figlib -DFAIL_SIGNALS
LDFLAGS=-s
LDLIBS=../mathlib/mathlib.a -lm

OBJS=maple.o schublib.o lincomb.o

PROGS=schubmult

def: $(PROGS)

mpi: $(MPIPROGS)

all: $(PROGS) $(MPIPROGS)

# lnum.a: $(LNUM_OBJS)
#	rm -f lnum.a
#	ar rcu lnum.a $(LNUM_OBJS)
#	ranlib lnum.a

schubmult: schubmult.o $(OBJS)

clean:
	rm -f $(PROGS) core *.a *.o dependencies

realclean:
	rm -f $(PROGS) core *.a *.o *.out *~ dot.* dependencies *.s

backup:
	./mkbackup.sh "Backup/weyl-%y%m%d-%H%M%S.tar"

dep:
	gcc -MM $(CFLAGS) *.c > dependencies

dependencies:
	gcc -MM $(CFLAGS) *.c > dependencies

include dependencies
