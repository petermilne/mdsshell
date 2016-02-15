SHELL=/bin/sh
CC=gcc
LD=ld
prefix=/usr/local/mdsplus
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
exec_prefix=/usr/local/mdsplus
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic
LDFLAGS=
LIBS=-lpthread -lc  -ldl 
INSTALL=../conf/install-sh -c

SOURCES = \
        mdsvme.c

OBJECTS = $(SOURCES:.c=.o)
EXPORTS = MdsVme.exports

all : $(top_srcdir)/lib/ $(top_srcdir)/lib/ $(top_srcdir)/lib/libMdsVme.so $(top_srcdir)/lib/libMdsVme.a

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)

clean :
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libMdsVme.a
	@ $(RM) $(top_srcdir)/lib/libMdsVme.so

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsVme.so ${exec_prefix}/lib
	if (test .so != .a) then \
		$(INSTALL) -m 644  $(top_srcdir)/lib/libMdsVme.a ${exec_prefix}/lib; \
	fi;

$(top_srcdir)/lib/libMdsVme.so : $(OBJECTS) 
	$(LD) -o $@ -shared  $(OBJECTS) $(LIBS)

$(top_srcdir)/lib/libMdsVme.a : $(OBJECTS)
	$(AR) -cr $@ $(OBJECTS)
	ranlib $@

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/



MdsVme.exports: MdsVme.def
	sed -e /LIBRARY/\;/\;/d MdsVme.def > $@
