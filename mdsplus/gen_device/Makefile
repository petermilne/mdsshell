SHELL=/bin/sh
CC=gcc
LD=ld
prefix=/usr/local/mdsplus
top_srcdir=..
exec_prefix=/usr/local/mdsplus
RM=$(top_srcdir)/devscripts/rm_if
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic 
LDFLAGS=
EXE_LIBS=-L$(top_srcdir)/lib/ -lTdiShr -lTreeShr -lMdsShr -lpthread -lc 
LIB_LIBS=-L$(top_srcdir)/lib/ -lTdiShr -lTreeShr -lMdsShr -lpthread -lc  
INSTALL=../conf/install-sh -c

EXE_SOURCES = gen_device.c gen_device.tab.c lex.gen_device.c
LIB_SOURCES = GenDeviceCallData.c GenDeviceCvtFloatCode.c GenDeviceCvtIntCode.c GenDeviceCvtStringCode.c \
              GenDeviceFree.c GenDeviceHeadNid.c GenDeviceSignal.c gen_device_msg.c

EXE_OBJECTS = $(EXE_SOURCES:.c=.o)
LIB_OBJECTS = $(LIB_SOURCES:.c=.o)
EXPORTS = GenDevShr.exports

all : $(top_srcdir)/bin/ $(top_srcdir)/bin/gen_device $(top_srcdir)/lib/ $(top_srcdir)/lib/ $(top_srcdir)/lib/libGenDevShr.so $(top_srcdir)/lib/libGenDevShr.a

depend:
	@makedepend -- $(CFLAGS) -- $(EXE_SOURCES) $(LIB_SOURCES)

clean :
	@ $(RM) $(EXE_OBJECTS) $(LIB_OBJECTS)
	@ $(RM) $(top_srcdir)/bin/gen_device
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libGenDevShr.a 
	@ $(RM) $(top_srcdir)/lib/libGenDevShr.so 

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdirhier ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libGenDevShr.so ${exec_prefix}/lib
	if (test .so != .a) then \
		$(INSTALL) -m 644  $(top_srcdir)/lib/libGenDevShr.a ${exec_prefix}/lib; \
	fi;


$(top_srcdir)/bin/gen_device : $(EXE_OBJECTS)
	$(CC) -o $@ $(EXE_OBJECTS) $(EXE_LIBS)

gen_device.tab.c : gen_device.y
	yacc -d -p gen_device -b gen_device gen_device.y

lex.gen_device.c : gen_device.x
	lex -Pgen_device gen_device.x

$(top_srcdir)/lib/libGenDevShr.so : $(LIB_OBJECTS) 
	$(LD) -o $@ -shared  $(LIB_OBJECTS) $(LIB_LIBS)

$(top_srcdir)/lib/libGenDevShr.a : $(LIB_OBJECTS)
	$(AR) -cr $@ $(LIB_OBJECTS)
	ranlib $@

$(top_srcdir)/bin/ :
	mkdir $(top_srcdir)/bin/

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/



MdsShr.exports: MdsShr.def
	sed -e /LIBRARY/\;/\;/d MdsShr.def > $@
