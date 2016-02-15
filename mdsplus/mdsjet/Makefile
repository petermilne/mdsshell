SHELL=/bin/sh
CC=gcc
LD=ld
prefix=/usr/local/mdsplus
top_srcdir=..
exec_prefix=/usr/local/mdsplus
RM=$(top_srcdir)/devscripts/rm_if
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic -I.
LDFLAGS=
LIBS=-lpthread -lc  -ljetRdaApi
INSTALL=../conf/install-sh -c

SOURCES = mdsjet.c

OBJECTS = $(SOURCES:.c=.o)
EXPORTS = mdsjet.exports

all : $(top_srcdir)/lib/ $(top_srcdir)/lib/ $(top_srcdir)/lib/libmdsjet.so $(top_srcdir)/lib/libmdsjet.a

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)

clean :
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libmdsjet.a 
	@ $(RM) $(top_srcdir)/lib/libmdsjet.so 

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdirhier ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libmdsjet.so ${exec_prefix}/lib
	if (test .so != .a) then \
		$(INSTALL) -m 644  $(top_srcdir)/lib/libmdsjet.a ${exec_prefix}/lib; \
	fi;

$(top_srcdir)/lib/libmdsjet.so : $(OBJECTS) 
	$(LD) -o $@ -shared  $(OBJECTS) $(LIBS)

$(top_srcdir)/lib/libmdsjet.a : $(OBJECTS)
	$(AR) -cr $@ $(OBJECTS)
	ranlib $@

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/



mdsjet.exports: mdsjet.def
	sed -e /LIBRARY/\;/\;/d mdsjet.def > $@
