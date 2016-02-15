SHELL=/bin/sh
CC=gcc
LD=ld
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic  
LDFLAGS= 
X_LIBS=
X_EXTRA_LIBS=
X_PRE_LIBS= -lSM -lICE
XLIBS = $(X_LIBS) $(X_PRE_LIBS) -lXt -lX11 $(X_EXTRA_LIBS)
LIBS= $(XLIBS) -lpthread -lc    -lMdsIpShr -lMdsShr
INSTALL=../conf/install-sh -c

SOURCES = \
mdsevent.c 

OBJECTS = $(SOURCES:.c=.o)
EXPORTS = IdlMdsEvent.export

all : $(top_srcdir)/lib/ $(top_srcdir)/lib/libIdlMdsEvent.so $(top_srcdir)/lib/libIdlMdsEvent.a

clean:
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libIdlMdsEvent.so
	@ $(RM) $(top_srcdir)/lib/libIdlMdsEvent.a

depend:
	 @makedepend -- $(CFLAGS) -- $(SOURCES)

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 644  $(top_srcdir)/lib/libIdlMdsEvent.a ${exec_prefix}/lib
	$(INSTALL) -m 755  $(top_srcdir)/lib/libIdlMdsEvent.so ${exec_prefix}/lib

$(top_srcdir)/lib/libIdlMdsEvent.so : $(OBJECTS) 
	$(LD) -o $@ -shared  $(OBJECTS) -L$(top_srcdir)/lib/ $(LDFLAGS) $(LIBS) 

$(top_srcdir)/lib/libIdlMdsEvent.a : $(OBJECTS)
	$(AR) -cr $@ $(OBJECTS)
	ranlib $@

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/
