SHELL=/bin/sh
CC=gcc
LD=ld
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic
LDFLAGS=
LIBS=-lpthread -lc  -lm
INSTALL=../conf/install-sh -c

SOURCES = MdsLibIdl.c
OBJECTS = $(SOURCES:.c=.o)
EXPORTS = mdslibidl.export

all : $(top_srcdir)/lib/ $(top_srcdir)/lib/libMdsLibIdl.so

$(top_srcdir)/lib/libMdsLibIdl.so : $(OBJECTS) 
	$(LD) -o $@ -shared  $(OBJECTS) -L$(top_srcdir)/lib/ -lTdiShr -lTreeShr -lMdsShr $(LIBS)

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/

clean :
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libMdsLibIdl.so

depend:
	 @makedepend -- $(CFLAGS) -- $(SOURCES)

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsLibIdl.so ${exec_prefix}/lib
