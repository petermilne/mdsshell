SHELL=/bin/sh
CC=gcc
LD=ld
prefix=/usr/local/mdsplus
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
exec_prefix=/usr/local/mdsplus
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic
LDFLAGS=
LIBS=-lpthread -lc  -L$(top_srcdir)/lib/ -lMdsIpShr -lTdiShr -lMdsShr
TDIC_LIBS= -L$(top_srcdir)/lib/ -lTdiShr -lTreeShr -lMdsdcl  -lMdsShr -lreadline -lcurses -lpthread -lc 
INSTALL=../conf/install-sh -c

SOURCES = \
        TdiShrExt.c

OBJECTS = $(SOURCES:.c=.o)
EXPORTS = tdishrext.exports

all : $(top_srcdir)/lib/ $(top_srcdir)/lib/ $(top_srcdir)/bin/ $(top_srcdir)/lib/libTdiShrExt.so $(top_srcdir)/lib/libTdiShrExt.a $(top_srcdir)/bin/tdic

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)

clean :
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libTdiShrExt.a
	@ $(RM) $(top_srcdir)/lib/libTdiShrExt.so
	@ $(RM) $(top_srcdir)/bin/tdic

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libTdiShrExt.so ${exec_prefix}/lib
	if (test .so != .a) then \
		$(INSTALL) -m 644  $(top_srcdir)/lib/libTdiShrExt.a ${exec_prefix}/lib; \
	fi;
	$(INSTALL) -m 755  $(top_srcdir)/bin/tdic ${exec_prefix}/bin

$(top_srcdir)/lib/libTdiShrExt.so : $(OBJECTS) 
	$(LD) -o $@ -shared  $(OBJECTS) $(LIBS)

$(top_srcdir)/lib/libTdiShrExt.a : $(OBJECTS)
	$(AR) -cr $@ $(OBJECTS)
	ranlib $@

$(top_srcdir)/bin/tdic : tdic.c
	$(CC) -o $@ tdic.c $(CFLAGS) $(TDIC_LIBS)

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/

$(top_srcdir)/bin/ :
	MKDIR $(top_srcdir)/bin/



tdishrext.exports: tdishrext.def
	sed -e /LIBRARY/\;/\;/d tdishrext.def > $@
