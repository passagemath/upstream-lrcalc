ACLOCAL_AMFLAGS=-I m4
SUBDIRS=mathlib lrcoef schubert

lib_LTLIBRARIES = liblrcalc.la
liblrcalc_la_SOURCES =
liblrcalc_la_LDFLAGS = $(LIBLRCALC_LDFLAGS) -version-info 1:0:0
liblrcalc_la_LIBADD = mathlib/libmath.la lrcoef/libsymfcn.la schubert/libschub.la

lrcalcincludedir = $(includedir)/lrcalc
lrcalcinclude_HEADERS=mathlib/*.h lrcoef/*.h schubert/*.h

TESTS=testsuite
