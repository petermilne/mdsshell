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

SOURCES = \
        librtl.c \
        dasutil_routines4.c \
        mdsmsg.c \
        MDSprintf.c \
        MdsCmprs.c \
        MdsCompareXd.c \
        MdsCompress.c \
        MdsEvents.c \
        MdsGet1DxA.c \
        MdsGet1DxS.c \
        MdsGetSetShotId.c \
        MdsPk.c \
        MdsSerialize.c \
        MdsShrThreadSafe.c \
        MdsXdRoutines.c \
	MdsMalloc.c \
	mds_dsc_string.c

OBJECTS = $(SOURCES:.c=.o)
EXPORTS = MdsShr.exports

all : $(top_srcdir)/lib/ $(top_srcdir)/lib/ $(top_srcdir)/lib/libMdsShr.so $(top_srcdir)/lib/libMdsShr.a

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)

clean :
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libMdsShr.a 
	@ $(RM) $(top_srcdir)/lib/libMdsShr.so 

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdirhier ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsShr.so ${exec_prefix}/lib
	if (test .so != .a) then \
		$(INSTALL) -m 644  $(top_srcdir)/lib/libMdsShr.a ${exec_prefix}/lib; \
	fi;

$(top_srcdir)/lib/libMdsShr.so : $(OBJECTS) 
	$(LD) -o $@ -shared  $(OBJECTS) $(LIBS)

$(top_srcdir)/lib/libMdsShr.a : $(OBJECTS)
	$(AR) -cr $@ $(OBJECTS)
	ranlib $@

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/



MdsShr.exports: MdsShr.def
	sed -e /LIBRARY/\;/\;/d MdsShr.def > $@
