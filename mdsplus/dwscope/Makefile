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
UIL=uil -I$(top_srcdir)/include
X_LIBS=
X_EXTRA_LIBS=
X_PRE_LIBS= -lSM -lICE
INSTALL=../conf/install-sh -c
MOTIF_LIBS =  -lMrm -lXm   -lXext
MLIBS = $(X_LIBS) $(MOTIF_LIBS) $(X_PRE_LIBS) -lXt -lX11 $(X_EXTRA_LIBS) $(LIBS)
SOURCES = dwscope.c dwscopesubs.c dwpad.c RemoteDummies.c evaluate.c
OBJECTS = $(SOURCES:.c=.o) evaluate_local.o

all : $(top_srcdir)/bin/ $(top_srcdir)/bin/dwscope $(top_srcdir)/bin/ScopePrinters $(top_srcdir)/bin/dwscopePrint $(top_srcdir)/uid/dwscope.uid $(top_srcdir)/bin/dwpad $(top_srcdir)/uid/dwpad.uid $(top_srcdir)/bin/dwscope_remote $(top_srcdir)/lib/dwscope_setup.ps

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)

clean :
	@ $(RM) $(OBJECTS) XmdsWidgetCallbacks.o
	@ $(RM) $(top_srcdir)/bin/dwscope
	@ $(RM) $(top_srcdir)/bin/dwscope_remote
	@ $(RM) $(top_srcdir)/bin/ScopePrinters
	@ $(RM) $(top_srcdir)/bin/dwscopePrint
	@ $(RM) $(top_srcdir)/uid/dwscope.uid
	@ $(RM) $(top_srcdir)/bin/dwpad
	@ $(RM) $(top_srcdir)/uid/dwpad.uid
	@ $(RM) $(top_srcdir)/lib/dwscope_setup.ps

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/bin/dwscope ${exec_prefix}/bin
	$(INSTALL) -m 755  $(top_srcdir)/bin/ScopePrinters ${exec_prefix}/bin
	$(INSTALL) -m 755  $(top_srcdir)/bin/dwscopePrint ${exec_prefix}/bin
	$(INSTALL) -m 755  $(top_srcdir)/bin/dwscope_remote ${exec_prefix}/bin
	$(INSTALL) -m 755  $(top_srcdir)/bin/dwpad ${exec_prefix}/bin
	$(INSTALL) -m 644  $(top_srcdir)/uid/dwscope.uid /usr/local/mdsplus/uid
	$(INSTALL) -m 644  $(top_srcdir)/uid/dwpad.uid /usr/local/mdsplus/uid
	$(INSTALL) -m 644  $(top_srcdir)/lib/dwscope_setup.ps ${exec_prefix}/lib

$(top_srcdir)/bin/ :
	mkdir $(top_srcdir)/bin/

$(top_srcdir)/bin/dwscope : dwscope.o dwscopesubs.o evaluate_local.o
	$(CC) -o $@ $(CFLAGS) dwscope.o dwscopesubs.o  evaluate_local.o -L$(top_srcdir)/lib/ -lTdiShr -lTreeShr -lMdsShr -lXmdsShr $(MLIBS)
                        
$(top_srcdir)/bin/ScopePrinters : ScopePrinters
	cp ScopePrinters $@

$(top_srcdir)/bin/dwscopePrint : dwscopePrint
	cp dwscopePrint $@

$(top_srcdir)/lib/dwscope_setup.ps : dwscope_setup.ps
	cp dwscope_setup.ps $@

$(top_srcdir)/bin/dwscope_remote : dwscope.o dwscopesubs.o evaluate.o RemoteDummies.o XmdsWidgetCallbacks.o
	$(CC) -o $@ $(CFLAGS) dwscope.o  dwscopesubs.o evaluate.o RemoteDummies.o XmdsWidgetCallbacks.o \
              -L$(top_srcdir)/lib/ -Wl,-Bstatic  -lXmdsShr -Wl,-Bdynamic  -L$(top_srcdir)/lib/ -lMdsIpShr $(MLIBS) -lm -L$(top_srcdir)/lib/ 

$(top_srcdir)/uid/dwscope.uid : dwscope.uil
	$(UIL) -o $@ dwscope.uil

$(top_srcdir)/bin/dwpad : dwpad.o dwscopesubs.o XmdsWidgetCallbacks.o
	$(CC) -o $@ $(CFLAGS) dwpad.o dwscopesubs.o XmdsWidgetCallbacks.o $(MLIBS) -L$(top_srcdir)/lib/ 

XmdsWidgetCallbacks.o: ../xmdsshr/XmdsWidgetCallbacks.c
	$(CC) -c $(CFLAGS) -D_NO_XDS -o $@ ../xmdsshr/XmdsWidgetCallbacks.c

$(top_srcdir)/uid/dwpad.uid : dwpad.uil
	$(UIL) -o $@ dwpad.uil
 
evaluate_local.o : evaluate.c
	$(CC) -c $(CFLAGS) -D_LOCAL_ACCESS -o $@ evaluate.c
