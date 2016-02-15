SHELL=/bin/sh
CC=gcc
LD=ld
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic  -D_NO_DXm 
LDFLAGS= 
X_LIBS=
X_EXTRA_LIBS=
X_PRE_LIBS= -lSM -lICE
XLIBS = $(X_LIBS) $(X_PRE_LIBS) -lXt -lX11 $(X_EXTRA_LIBS)
MOTIF_LIBS =  -lMrm -lXm   -lXext
LIBS=  -lpthread -lc    -lMdsIpShr -lMdsShr -lMdsMath
INSTALL=../conf/install-sh -c
UIL=uil -I$(top_srcdir)/include

SOURCES = \
mdsevent.c \
cw_wveditv5.c \
memmoveext.c

OBJECTS = $(SOURCES:.c=.o)

EXPORTS = IdlMdsWidgets.export

all : $(top_srcdir)/lib/ $(top_srcdir)/lib/libIdlMdsWidgets.so $(top_srcdir)/lib/libIdlMdsWidgets.a $(top_srcdir)/uid/cw_wvedit.uid


clean:
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libIdlMdsWidgets.so
	@ $(RM) $(top_srcdir)/lib/libIdlMdsWidgets.a

depend:
	 @makedepend -- $(CFLAGS) -- $(SOURCES)

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 644  $(top_srcdir)/lib/libIdlMdsWidgets.a ${exec_prefix}/lib
	$(INSTALL) -m 755  $(top_srcdir)/lib/libIdlMdsWidgets.so ${exec_prefix}/lib
	$(INSTALL) -m 644 $(top_srcdir)/uid/cw_wvedit.uid /usr/local/mdsplus/uid


$(top_srcdir)/lib/libIdlMdsWidgets.so : $(OBJECTS) 
	$(LD) -o $@ -shared  $(OBJECTS) -L$(top_srcdir)/lib/ $(LDFLAGS)  -L/usr/X11R6/lib/ -lMrm $(LIBS) -lXmdsShr

$(top_srcdir)/lib/libIdlMdsWidgets.a : $(OBJECTS)
	$(AR) -cr $@ $(OBJECTS)
	ranlib $@


$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/

$(top_srcdir)/uid/cw_wvedit.uid : cw_wvedit.uil ; $(UIL) -o $@ cw_wvedit.uil
