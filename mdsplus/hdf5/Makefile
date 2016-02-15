SHELL=/bin/sh
CC=gcc
LD=ld
prefix=/usr/local/mdsplus
top_srcdir=..
exec_prefix=/usr/local/mdsplus
RM=$(top_srcdir)/devscripts/rm_if
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic  -I.
LDFLAGS=
LIBS=-L$(top_srcdir)/lib/ -lTdiShr -lTreeShr -lMdsShr  -lhdf5 -lpthread -lc 
INSTALL=../conf/install-sh -c

SOURCES = \
        hdf5tdi.c  \
        hdf5ToMds.c

OBJECTS = $(SOURCES:.c=.o)
EXPORTS = hdf5tdi.exports

all : $(top_srcdir)/lib/ $(top_srcdir)/lib/ $(top_srcdir)/lib/libhdf5tdi.so $(top_srcdir)/lib/libhdf5tdi.a \
	$(top_srcdir)/bin/ $(top_srcdir)/bin/hdf5ToMds

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)

clean :
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libhdf5tdi.a 
	@ $(RM) $(top_srcdir)/lib/libhdf5tdi.so 
	@ $(RM) $(top_srcdir)/bin/hdf5ToMds

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdirhier ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libhdf5tdi.so ${exec_prefix}/lib
	if (test .so != .a) then \
		$(INSTALL) -m 644  $(top_srcdir)/lib/libhdf5tdi.a ${exec_prefix}/lib; \
	fi;

$(top_srcdir)/lib/libhdf5tdi.so : hdf5tdi.o 
	$(LD) -o $@ -shared  hdf5tdi.o $(LIBS)

$(top_srcdir)/lib/libhdf5tdi.a : hdf5tdi.o 
	$(AR) -cr $@ hdf5tdi.o 
	ranlib $@

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/

$(top_srcdir)/bin/ :
	mkdir $(top_srcdir)/bin/



hdf5tdi.exports: hdf5tdi.def
	sed -e /LIBRARY/\;/\;/d MdsShr.def > $@

$(top_srcdir)/bin/hdf5ToMds : hdf5ToMds.o 
	$(CC) -o $@ hdf5ToMds.o $(LIBS)
