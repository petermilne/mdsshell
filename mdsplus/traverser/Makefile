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

SOURCES = \
	main-uil.c \
	bxutils-uil.c \
	CallbacksUil.c \
        ReadInt.c \
        ReadString.c \
        GetNciString.c \
        GetSupportedDevices.c

OBJECTS = $(SOURCES:.c=.o)

all : $(top_srcdir)/bin/ \
$(top_srcdir)/bin/traverser \
$(top_srcdir)/uid/ \
$(top_srcdir)/uid/traverser.uid

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)

clean :
	@ $(RM) $(OBJECTS)
	@ $(RM) $(top_srcdir)/bin/traverser
	@ $(RM) $(top_srcdir)/uid/traverser.uid


install:
	$(INSTALL) -m 644  $(top_srcdir)/uid/traverser.uid /usr/local/mdsplus/uid
	$(INSTALL) -m 755  $(top_srcdir)/bin/traverser ${exec_prefix}/bin


$(top_srcdir)/bin/traverser : $(OBJECTS)
	 $(CC) -o $@ $(CFLAGS) $(OBJECTS) -L$(top_srcdir)/lib/ -lMdsdcl -lTdiShr -lTreeShr -lMdsShr -lXmdsShr $(MLIBS) $(LIBS)

$(top_srcdir)/bin/ :
	mkdir $(top_srcdir)/bin/

$(top_srcdir)/uid/ :
	mkdir $(top_srcdir)/uid/

$(top_srcdir)/uid/traverser.uid : traverser.uil
	$(UIL) -o $@ traverser.uil
