AM_CFLAGS = -I .. -I ../mathlib -I ../lrcoef

bin_PROGRAMS = schubmult

noinst_LTLIBRARIES = libschub.la
libschub_la_SOURCES = schublib.c lincomb.c

schubmult_LDADD = ../mathlib/libmath.la ../lrcoef/libsymfcn.la libschub.la
