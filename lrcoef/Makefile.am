AM_CFLAGS = -I .. -I ../mathlib

noinst_LTLIBRARIES = libsymfcn.la
libsymfcn_la_SOURCES = symfcn.c maple.c

bin_PROGRAMS = coprod lrcoef lrskew mult skew
coprod_LDADD = ../mathlib/libmath.la libsymfcn.la
lrskew_LDADD = ../mathlib/libmath.la libsymfcn.la
lrcoef_LDADD = ../mathlib/libmath.la libsymfcn.la
mult_LDADD = ../mathlib/libmath.la libsymfcn.la
skew_LDADD   = ../mathlib/libmath.la libsymfcn.la
