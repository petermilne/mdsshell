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

SOURCES = \
      ServerDispatchAction.c\
      ServerFreeDispatchTable.c\
      ServerSetDetailProc.c\
      ServerAbortServer.c\
      ServerDispatchClose.c\
      ServerGetInfo.c\
      ServerSetLogging.c\
      ServerBuildDispatchTable.c\
      ServerDispatchCommand.c\
      ServerMonitorCheckin.c\
      ServerStartServer.c\
      ServerCloseTrees.c\
      ServerDispatchPhase.c\
      ServerStopServer.c\
      ServerCreatePulse.c\
      ServerFindServers.c\
      ServerSendMessage.c\
      ServerQAction.c

OBJECTS = $(SOURCES:.c=.o)
EXPORTS = servershr.export

all: $(top_srcdir)/lib/ $(top_srcdir)/lib/libMdsServerShr.so $(top_srcdir)/lib/libMdsServerShr.a

clean:
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libMdsServerShr.so
	@ $(RM) $(top_srcdir)/lib/libMdsServerShr.a

depend:
	 @makedepend -- $(CFLAGS) -- $(SOURCES)

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsServerShr.so ${exec_prefix}/lib
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsServerShr.a ${exec_prefix}/lib

$(top_srcdir)/lib/libMdsServerShr.so : $(OBJECTS) 
	$(LD) -o $@ -shared  $(OBJECTS) -L$(top_srcdir)/lib/ -lMdsdcl -lTdiShr -lTreeShr -lMdsShr -lMdsIpShr $(LIBS)

$(top_srcdir)/lib/libMdsServerShr.a : $(OBJECTS)
	$(AR) -cr $@ $(OBJECTS)
	ranlib $@

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/

