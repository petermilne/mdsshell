SHELL=/bin/sh
CC=gcc
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
X_CFLAGS=-D_NO_DXm
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic $(X_CFLAGS)
LDFLAGS=
LIBS=-lpthread -lc 
UIL=uil
X_LIBS=
X_EXTRA_LIBS=
X_PRE_LIBS= -lSM -lICE
INSTALL=../conf/install-sh -c
MOTIF_LIBS =  -lMrm -lXm   -lXext
MLIBS = $(X_LIBS) $(MOTIF_LIBS) $(X_PRE_LIBS) -lXt -lX11 $(X_EXTRA_LIBS) $(LIBS)

SOURCES = actions.c
OBJECTS = $(SOURCES:.c=.o)

all : $(top_srcdir)/bin/ $(top_srcdir)/bin/actions $(top_srcdir)/uid/actions.uid $(top_srcdir)/bin/actmon $(top_srcdir)/uid/actmon.uid

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)

clean :
	@ $(RM) *.o
	@ $(RM) $(top_srcdir)/bin/actions
	@ $(RM) $(top_srcdir)/uid/actions.uid
	@ $(RM) $(top_srcdir)/bin/actmon
	@ $(RM) $(top_srcdir)/uid/actmon.uid

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/bin/actions ${exec_prefix}/bin
	$(INSTALL) -m 644  $(top_srcdir)/uid/actions.uid /usr/local/mdsplus/uid
	$(INSTALL) -m 755  $(top_srcdir)/bin/actmon ${exec_prefix}/bin
	$(INSTALL) -m 644  $(top_srcdir)/uid/actmon.uid /usr/local/mdsplus/uid

$(top_srcdir)/bin/actions : $(OBJECTS) 
	 $(CC) -o $@ $(CFLAGS) $(OBJECTS)  -L$(top_srcdir)/lib/ -lTdiShr -lTreeShr -lMdsShr -lXmdsShr $(MLIBS) $(LIBS)

$(top_srcdir)/bin/ :
	mkdir $(top_srcdir)/bin/

$(top_srcdir)/uid/actions.uid : actions.uil
	$(UIL) -o $@ actions.uil

$(top_srcdir)/bin/actmon : actmon.o
	 $(CC) -o $@ $(CFLAGS) actmon.o  -L$(top_srcdir)/lib/ -lTdiShr -lTreeShr -lMdsShr -lXmdsShr -lMdsServerShr -lMdsLib  $(MLIBS) $(LIBS)

$(top_srcdir)/uid/actmon.uid : actmon.uil
	$(UIL) -o $@ actmon.uil
