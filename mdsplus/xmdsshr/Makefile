SHELL=/bin/sh
CC=gcc
LD=ld
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
X_CFLAGS=-D_NO_DXm
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic $(X_CFLAGS)
LDFLAGS=
LIBS=-lpthread -lc  -lm
UIL=uil
X_LIBS=
X_EXTRA_LIBS=
X_PRE_LIBS= -lSM -lICE
INSTALL=../conf/install-sh -c
MOTIF_LIBS =  -lMrm -lXm   -lXext

SOURCES = \
ListTree.c \
XmdsBells.c \
XmdsComplain.c \
XmdsDeviceSetup.c \
XmdsDigChans.c \
XmdsDisplay.c \
XmdsExpr.c \
XmdsGetPutNid.c \
XmdsGetSetOptionIdx.c \
XmdsInitialize.c \
XmdsInput.c \
XmdsInputs.c \
XmdsNidOptionMenu.c \
XmdsOkButtons.c \
XmdsOnOffToggleButton.c \
XmdsPath.c \
XmdsPopupMenuPosition.c \
XmdsSetSubvalues.c \
XmdsSupport.c \
XmdsUsageIcons.c \
XmdsWavedraw.c \
XmdsWaveform.c \
XmdsWidgetCallbacks.c \
XmdsXdBox.c \
XmdsXdBoxDialog.c \
XmdsXdBoxDialogButton.c \
XmdsXdBoxOnOffButton.c

OBJECTS = $(SOURCES:.c=.o)
EXPORTS = xmdsshr.export

all:    $(top_srcdir)/uid/ \
        $(top_srcdir)/uid/xmds_usage_icons.uid \
        $(top_srcdir)/uid/XmdsDigChans.uid \
        $(top_srcdir)/uid/XmdsInput.uid \
        $(top_srcdir)/uid/XmdsXdBox.uid \
	$(top_srcdir)/lib/ \
        $(top_srcdir)/lib/libXmdsShr.a \
	$(top_srcdir)/lib/libXmdsShr.so

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)

clean :
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libXmdsShr.a
	@ $(RM) $(top_srcdir)/lib/libXmdsShr.so
	@ $(RM) $(top_srcdir)/uid/xmdsuil.wmd
	@ $(RM) $(top_srcdir)/uid/xmds_usage_icons.uid
	@ $(RM) $(top_srcdir)/uid/XmdsDigChans.uid
	@ $(RM) $(top_srcdir)/uid/XmdsInput.uid
	@ $(RM) $(top_srcdir)/uid/XmdsXdBox.uid


install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	if [ ! -d /usr/local/mdsplus/uid ] ; then \
	        mkdir /usr/local/mdsplus/uid; \
	        chmod 755 /usr/local/mdsplus/uid; \
	fi;
	$(INSTALL) -m 644  $(top_srcdir)/uid/xmds_usage_icons.uid /usr/local/mdsplus/uid
	$(INSTALL) -m 644  $(top_srcdir)/uid/XmdsDigChans.uid /usr/local/mdsplus/uid
	$(INSTALL) -m 644  $(top_srcdir)/uid/XmdsInput.uid /usr/local/mdsplus/uid
	$(INSTALL) -m 644  $(top_srcdir)/uid/XmdsXdBox.uid /usr/local/mdsplus/uid
	$(INSTALL) -m 755  $(top_srcdir)/lib/libXmdsShr.so ${exec_prefix}/lib
	if test ".so" != ".a" ; then \
		$(INSTALL) -m 644  $(top_srcdir)/lib/libXmdsShr.a ${exec_prefix}/lib; \
	fi;

$(top_srcdir)/uid/:
	mkdir $(top_srcdir)/uid/

$(top_srcdir)/lib/:
	mkdir $(top_srcdir)/lib/

$(top_srcdir)/uid/xmds_usage_icons.uid: xmds_usage_icons.uil
	$(UIL) -o $@ xmds_usage_icons.uil

$(top_srcdir)/uid/XmdsDigChans.uid : XmdsDigChans.uil
	$(UIL) -o $@ XmdsDigChans.uil

$(top_srcdir)/uid/XmdsInput.uid : XmdsInput.uil
	$(UIL) -o $@ XmdsInput.uil

$(top_srcdir)/uid/XmdsXdBox.uid : XmdsXdBox.uil
	$(UIL) -o $@ XmdsXdBox.uil

$(top_srcdir)/lib/libXmdsShr.so : $(OBJECTS)
	- $(LD) -o $@ -shared  $(OBJECTS) \
           -L$(top_srcdir)/lib/ -lMdsShr -lTdiShr -lTreeShr $(X_LIBS) $(MOTIF_LIBS) $(X_PRE_LIBS) -lXt -lX11 $(X_EXTRA_LIBS) $(LIBS)

$(top_srcdir)/lib/libXmdsShr.a : $(OBJECTS)
	$(AR) -cr $@ $(OBJECTS)
	ranlib $@

$(top_srcdir)/lib/xmdsuil.wmd : $(WMD_RULE)

$(top_srcdir)/bin/wmdcreate : wmldbcreate-xmdsshr.c UilConst.h
	$(CC) -o $@ -DMDS_EXTENSION $(CFLAGS) wmldbcreate-xmdsshr.c $(MOTIF_LIBS) $(X_PRE_LIBS) -lX11 $(X_EXTRA_LIBS)

UilConst.h : xmdsuil.wml 
	$(WML) 

