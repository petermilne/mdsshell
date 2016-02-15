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

LIB_SOURCES =		\
	cli_routines.c	\
	cli_util.c		\
	dasutil_routines1.c	\
	dasutil_routines2.c	\
	dasutil_routines3.c

MAIN_SOURCES = cdu.c cdu_util.c cdu_write.c

LIB_OBJECTS = $(LIB_SOURCES:.c=.o)
MAIN_OBJECTS = $(MAIN_SOURCES:.c=.o)

all:	$(top_srcdir)/lib/ $(top_srcdir)/lib/libCdu.a $(top_srcdir)/bin/cdu

clean:
	@ $(RM) $(LIB_OBJECTS) $(MAIN_OBJECTS)
	@ $(RM) $(top_srcdir)/bin/cdu
	@ $(RM) $(top_srcdir)/lib/libCdu.a

depend:
	 @makedepend -- $(CFLAGS) -- $(LIB_SOURCES) $(MAIN_SOURCES)

install:
	@ :

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/

$(top_srcdir)/lib/libCdu.a:	$(LIB_OBJECTS)
		$(AR) -cr $@ $(LIB_OBJECTS)
		ranlib $@

$(top_srcdir)/bin/cdu:	$(MAIN_OBJECTS)
	$(CC) $(CFLAGS) -o $@  $(MAIN_OBJECTS) -L$(top_srcdir)/lib/ -lCdu -L$(top_srcdir)/lib/ -lMdsShr $(LIBS)
