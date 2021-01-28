bin_PROGRAMS = lrcalc schubmult
check_PROGRAMS = tst_allparts tst_partiter tst_allperms tst_allstr \
  tst_schubmult

lrcalc_LDADD = liblrcalc.la
schubmult_LDADD = liblrcalc.la

tst_allparts_LDADD = liblrcalc.la
tst_partiter_LDADD = liblrcalc.la
tst_allperms_LDADD = liblrcalc.la
tst_allstr_LDADD = liblrcalc.la
tst_schubmult_LDADD = liblrcalc.la

lib_LTLIBRARIES = liblrcalc.la
liblrcalc_la_SOURCES = alloc.c ilist.c ivector.c ivlincomb.c ivlist.c \
  lriter.c maple.c optshape.c part.c perm.c schublib.c schur.c vectarg.c

liblrcalc_la_LDFLAGS = $(LIBLRCALC_LDFLAGS) -version-info 1:1:0

lrcalcincludedir = $(includedir)/lrcalc
lrcalcinclude_HEADERS = *.h
