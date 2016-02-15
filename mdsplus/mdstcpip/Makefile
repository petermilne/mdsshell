SHELL=/bin/sh
CC=gcc
LD=ld

COMPRESSION_SOURCES=adler32.c compress.c deflate.c infblock.c infcodes.c\
inffast.c inflate.c inftrees.c infutil.c trees.c uncompr.c zutil.c
COMPRESSION_SWITCH=
#COMPRESS_SOURCES=
#COMPRESSION_SWITCH=-DNOCOMPRESSION

top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic $(COMPRESSION_SWITCH) 
LDFLAGS=
LIBS= -lpthread -lc    -lresolv 
INSTALL=../conf/install-sh -c

ifeq "x" "x"
	modules =
else
	modules = $(top_srcdir)/lib/libMdsIpShr 
endif

SOURCES = \
	mdsipshr.c mdsiputil.c vmshpwd.c mdsip_socket_io.c $(COMPRESSION_SOURCES)

COMPRESSION_OBJECTS = $(COMPRESSION_SOURCES:.c=.o)
OBJECTS = $(SOURCES:.c=.o)
EXPORTS = mdsipshr.export

all : $(top_srcdir)/lib/libMdsIpShr.so \
      $(modules) \
      $(top_srcdir)/bin/ $(top_srcdir)/lib/ $(top_srcdir)/etc/ \
      $(top_srcdir)/bin/mdsiptest $(top_srcdir)/bin/mdsip \
      $(top_srcdir)/bin/mdsipd $(top_srcdir)/etc/mdsip.hosts \
      $(top_srcdir)/bin/mkdirhier \
      $(top_srcdir)/lib/libMdsIpShr.a $(top_srcdir)/lib/libMdsIpUtil.a 

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES) mdsip_u.c

clean :
	@ $(RM) $(OBJECTS) $(COMPRESSION_OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libMdsIpShr.so
	@ $(RM) $(top_srcdir)/lib/libMdsIpShr.a
	@ $(RM) $(top_srcdir)/lib/libMdsIpUtil.a
	@ $(RM) $(top_srcdir)/bin/mdsiptest
	@ $(RM) $(top_srcdir)/bin/mdsip
	@ $(RM) $(top_srcdir)/bin/mdsipd
	@ $(RM) $(top_srcdir)/bin/mkdirhier
	@ $(RM) $(top_srcdir)/etc/mdsip.hosts

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsIpShr.so ${exec_prefix}/lib
        ifdef modules
		$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsIpShr ${exec_prefix}/lib
        endif
	$(INSTALL) -m 755  $(top_srcdir)/bin/mdsip	${exec_prefix}/bin
	$(INSTALL) -m 755  $(top_srcdir)/bin/mdsipd	${exec_prefix}/bin
	$(INSTALL) -m 755  $(top_srcdir)/bin/mkdirhier ${exec_prefix}/bin
	if test ".so" != ".a" ; then \
		$(INSTALL) -m 644  $(top_srcdir)/lib/libMdsIpShr.a ${exec_prefix}/lib; \
		$(INSTALL) -m 644  $(top_srcdir)/lib/libMdsIpUtil.a ${exec_prefix}/lib; \
	fi;
	if [ ! -d $(exec_prefix)/etc ] ; then \
                mkdirhier $(exec_prefix)/etc; \
                chmod 755 $(exec_prefix)/etc; \
        fi;
	$(INSTALL) -m 755  mdsip.hosts $(exec_prefix)/etc

$(top_srcdir)/bin/ :
	mkdir $(top_srcdir)/bin/

$(top_srcdir)/bin/mdsiptest : t.c $(top_srcdir)/lib/libMdsIpShr.so
	$(CC) -o $@ $(CFLAGS) t.c -L$(top_srcdir)/lib/ -lMdsIpShr $(LIBS)

$(top_srcdir)/bin/mdsip : mdsip_u.c
	$(CC) -o $@ $(CFLAGS) mdsip_u.c -L$(top_srcdir)/lib/ -lMdsIpShr -lMdsShr -lTreeShr -lTdiShr $(LIBS)

$(top_srcdir)/bin/mdsipd : mdsipd
	cp mdsipd $(top_srcdir)/bin/mdsipd

$(top_srcdir)/lib/libMdsIpShr.so : $(OBJECTS)
	$(LD) -o $@ -shared  $(OBJECTS) $(LIBS) 

$(top_srcdir)/lib/libMdsIpShr.a : $(OBJECTS)
	$(AR) -cr $@ $(OBJECTS)
	ranlib $@

ifdef modules
$(top_srcdir)/lib/libMdsIpShr : $(OBJECTS)
	$(LD) -o $@  $(OBJECTS) $(LIBS) 
endif

$(top_srcdir)/lib/libMdsIpUtil.a : mdsiputil.o mdsip_socket_io.o $(COMPRESSION_OBJECTS)
	$(AR) -cr $@ mdsiputil.o mdsip_socket_io.o $(COMPRESSION_OBJECTS)
	ranlib $@

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/

$(top_srcdir)/etc/ :
	mkdir $(top_srcdir)/etc/

$(top_srcdir)/etc/mdsip.hosts : mdsip.hosts
	cp mdsip.hosts $(top_srcdir)/etc/mdsip.hosts

$(top_srcdir)/bin/mkdirhier : mkdirhier
	cp mkdirhier $(top_srcdir)/bin/mkdirhier
