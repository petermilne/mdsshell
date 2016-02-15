SHELL=/bin/sh
CC=gcc
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic
LDFLAGS=
LIBS=-lpthread -lc 
INSTALL=../conf/install-sh -c

SOURCES = setevent.c
OBJECTS = $(SOURCES:.c=.o)

all : $(top_srcdir)/bin/ $(top_srcdir)/bin/setevent

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)

install:
	if [ ! -d ${exec_prefix}/bin ] ; then \
                mkdir ${exec_prefix}/bin; \
                chmod 755 ${exec_prefix}/bin; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/bin/setevent ${exec_prefix}/bin

clean :
	@ $(RM) $(OBJECTS)
	@ $(RM) $(top_srcdir)/bin/setevent

$(top_srcdir)/bin/setevent : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(CFLAGS) -L$(top_srcdir)/lib/ -lMdsShr $(LIBS)

$(top_srcdir)/bin/ :
	mkdir $(top_srcdir)/bin/

