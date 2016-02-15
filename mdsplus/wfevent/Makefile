SHELL=/bin/sh
CC=gcc
LD=ld
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic
LDFLAGS=
LIBS=-lpthread -lc 
INSTALL=../conf/install-sh -c

SOURCES = wfevent.c
OBJECTS = $(SOURCES:.c=.o)

all: $(top_srcdir)/bin/ $(top_srcdir)/bin/wfevent 

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)

clean :
	@ $(RM) $(OBJECTS)
	@ $(RM) $(top_srcdir)/bin/wfevent

install:
	if [ ! -d ${exec_prefix}/bin ] ; then \
                mkdir ${exec_prefix}/bin; \
                chmod 755 ${exec_prefix}/bin; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/bin/wfevent	${exec_prefix}/bin

$(top_srcdir)/bin/ :
	mkdir $(top_srcdir)/bin/

$(top_srcdir)/bin/wfevent : $(OBJECTS)
	$(CC) -o $@ $(CFLAGS) $(OBJECTS)  -L$(top_srcdir)/lib/ -lMdsShr $(LIBS)
