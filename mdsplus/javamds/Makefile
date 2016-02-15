SHELL=/bin/sh
CC=gcc
LD=ld
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic -I/usr/java/jdk1.7.0_25/include -I/usr/java/jdk1.7.0_25/include/linux
LDFLAGS=
LIBS=-L/usr/java/jdk1.7.0_25/jre/lib/i386/client -ljvm -L/usr/java/jdk1.7.0_25/jre/lib/i386  -ljava -L/usr/java/jdk1.7.0_25/jre/lib/i386/native_threads -lhpi -lpthread -lc 
INSTALL=../conf/install-sh -c

SOURCES = javamds.c ServerSupport.c JavaData.c JavaTrav.c
OBJECTS = $(SOURCES:.c=.o)
EXPORTS = javamds.export

all: 	$(top_srcdir)/lib/ $(top_srcdir)/lib/libJavaMds.so

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)

clean:
	@ $(RM) $(CLASSES)
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libJavaMds.so

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libJavaMds.so ${exec_prefix}/lib

$(top_srcdir)/lib/libJavaMds.so : $(OBJECTS) 
	$(LD) -o $@ -shared  $(OBJECTS) -L$(top_srcdir)/lib/ -lMdsShr -lTreeShr -lTdiShr -lpthread $(LIBS)

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/
