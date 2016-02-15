SHELL=/bin/sh
CC=gcc
LD=ld
prefix=/usr/local/mdsplus
top_srcdir=..
exec_prefix=/usr/local/mdsplus
RM=$(top_srcdir)/devscripts/rm_if
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic 
LDFLAGS=
LIBS=-lpthread -lc  -ldl 
INSTALL=../conf/install-sh -c

SOURCES = boxsmooth.c \
          bwfilter.c  \
          getnidarray.c  \
          getreferences.c  \
          resample.c  \
          step_resample.c \
	  fformat.c

OBJECTS = $(SOURCES:.c=.o)
EXPORTS = MdsMisc.exports

all : $(top_srcdir)/lib/ $(top_srcdir)/lib/ $(top_srcdir)/lib/libMdsMisc.so $(top_srcdir)/lib/libMdsMisc.a

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)

clean :
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libMdsMisc.a 
	@ $(RM) $(top_srcdir)/lib/libMdsMisc.so 

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdirhier ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsMisc.so ${exec_prefix}/lib
	if (test .so != .a) then \
		$(INSTALL) -m 644  $(top_srcdir)/lib/libMdsMisc.a ${exec_prefix}/lib; \
	fi;

$(top_srcdir)/lib/libMdsMisc.so : $(OBJECTS) 
	$(LD) -o $@ -shared  $(OBJECTS) $(LIBS) -L$(top_srcdir)/lib/ -lTdiShr -lTreeShr -lMdsShr -lm

$(top_srcdir)/lib/libMdsMisc.a : $(OBJECTS)
	$(AR) -cr $@ $(OBJECTS)
	ranlib $@

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/



MdsMisc.exports: MdsMisc.def
	sed -e /LIBRARY/\;/\;/d MdsMisc.def > $@
