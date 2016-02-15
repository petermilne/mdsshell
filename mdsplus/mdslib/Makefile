SHELL=/bin/sh
CC=gcc
FC=f95
LD=ld
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic
FFLAGS=-g -O2  -I$(top_srcdir)/include -fpic -Wno-globals -fno-backslash
LDFLAGS=
LIBS= -lpthread -lc    -lresolv -lm
MDSLIB_LIBS=-lTdiShr -lTreeShr -lMdsShr
INSTALL=../conf/install-sh -c

ifeq "mdstcpip" "mdsip_xio"
include makefile-header
CFLAGS = $(GLOBUS_CFLAGS) $(GLOBUS_INCLUDES) -I/usr/local/mdsplus/include
LDFLAGS = $(GLOBUS_LDFLAGS) $(GLOBUS_LIBS) $(GLOBUS_PKG_LIBS) --allow-multiple-definition
endif

SOURCES = MdsLib.c
OBJECTS = $(SOURCES:.c=.o) 
EXPORTS = MdsLib.export

OBJECTS_CLIENT = MdsLib_client.o 
OBJECTS_CLIENT_ARCHIVE = ../mdstcpip/mdsiputil.o

all: $(top_srcdir)/bin/ $(top_srcdir)/lib/ \
         $(top_srcdir)/lib/libMdsLib_client.so \
         $(top_srcdir)/lib/libMdsLib.so \
         $(top_srcdir)/lib/libMdsLib_client.a \
         $(top_srcdir)/lib/libMdsLib.a $(top_srcdir)/bin/mdslib_ctest  \
         $(top_srcdir)/bin/mdslib_ftest  $(top_srcdir)/bin/mdslib_fremotetest \
	 $(top_srcdir)/bin/dtype_test

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)

clean:
	@ $(RM) $(OBJECTS) $(OBJECTS_CLIENT) mdslib_ftest.o
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libMdsLib_client.so
	@ $(RM) $(top_srcdir)/lib/libMdsLib_client.a
	@ $(RM) $(top_srcdir)/lib/libMdsLib.so
	@ $(RM) $(top_srcdir)/lib/libMdsLib.a
	@ $(RM) $(top_srcdir)/bin//mdslib_*test
	@ $(RM) $(top_srcdir)/bin/dtype_test

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 644  $(top_srcdir)/lib/libMdsLib_client.a ${exec_prefix}/lib
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsLib_client.so ${exec_prefix}/lib
	$(INSTALL) -m 644  $(top_srcdir)/lib/libMdsLib.a ${exec_prefix}/lib
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsLib.so ${exec_prefix}/lib

$(top_srcdir)/bin/ :
	mkdir $(top_srcdir)/bin/

$(top_srcdir)/lib/libMdsLib.so : $(OBJECTS)
	$(LD) -o $@ -shared  $(LDFLAGS) $(OBJECTS) -L$(top_srcdir)/lib/ -lMdsIpUtil -L$(top_srcdir)/lib/ $(MDSLIB_LIBS) $(LIBS)

$(top_srcdir)/lib/libMdsLib.a : $(OBJECTS)
	$(AR) -cr $@ $(OBJECTS)
	ranlib $@

$(top_srcdir)/lib/libMdsLib_client.so : $(OBJECTS_CLIENT)
	$(LD) -o $@ -shared  $(LDFLAGS) $(OBJECTS_CLIENT) -L$(top_srcdir)/lib/ -lMdsIpUtil $(LIBS)   $(GLOBUS_LDFLAGS) $(GLOBUS_LIBS) $(GLOBUS_PKG_LIBS)

$(top_srcdir)/lib/libMdsLib_client.a : $(OBJECTS_CLIENT) $(OBJECTS_CLIENT_ARCHIVE)
	$(AR) -cr $@ $(OBJECTS_CLIENT) $(OBJECTS_CLIENT_ARCHIVE)

MdsLib_client.o : MdsLib.c
	$(CC) -o $@ -c $(CFLAGS) MdsLib.c -D_CLIENT_ONLY

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/

$(top_srcdir)/bin/mdslib_ctest : mdslib_ctest.c
	$(CC) -o $@ $(CFLAGS) mdslib_ctest.c -L$(top_srcdir)/lib/ -lMdsLib -lMdsIpShr $(MDSLIB_LIBS) $(LIBS) -lm $(GLOBUS_LDFLAGS) $(GLOBUS_LIBS) $(GLOBUS_PKG_LIBS)

$(top_srcdir)/bin/mdslib_ftest : mdslib_ftest.o
	$(FC) -o $@ $(FFLAGS) mdslib_ftest.f -L$(top_srcdir)/lib/ -lMdsLib -lMdsIpShr $(MDSLIB_LIBS) $(LIBS) $(GLOBUS_LDFLAGS) $(GLOBUS_LIBS) $(GLOBUS_PKG_LIBS)

$(top_srcdir)/bin/mdslib_fremotetest : mdslib_fremotetest.f
	$(FC) -o $@ $(FFLAGS) mdslib_fremotetest.f -L$(top_srcdir)/lib/ -lMdsLib -lMdsIpShr $(MDSLIB_LIBS) $(LIBS) $(GLOBUS_LDFLAGS) $(GLOBUS_LIBS) $(GLOBUS_PKG_LIBS)

$(top_srcdir)/bin/dtype_test : dtype_test.c
	$(CC) -o $@ $(CFLAGS) dtype_test.c -L$(top_srcdir)/lib/ -lMdsLib -lMdsIpShr $(MDSLIB_LIBS) $(LIBS) $(GLOBUS_LDFLAGS) $(GLOBUS_LIBS) $(GLOBUS_PKG_LIBS)

makefile-header:  
	/bin/globus-makefile-header --flavor= globus_xio > makefile-header
