SHELL=/bin/sh
CC=gcc
LD=ld
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic 
LDFLAGS=
LIBS= -lpthread -lc   
INSTALL=../conf/install-sh -c

SOURCES = \
dynamic_syb.c \
dbstubs.c

OBJECTS = $(SOURCES:.c=.o)
EXPORTS = MdsSql.export

all : $(top_srcdir)/lib/ $(top_srcdir)/lib/libMdsSql.so $(top_srcdir)/lib/libMdsSql.a

clean:
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libMdsSql.so
	@ $(RM) $(top_srcdir)/lib/libMdsSql.a


depend:
	 @makedepend -- $(CFLAGS) -- $(SOURCES)

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 644  $(top_srcdir)/lib/libMdsSql.a ${exec_prefix}/lib
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsSql.so ${exec_prefix}/lib

$(top_srcdir)/lib/libMdsSql.so : $(OBJECTS) 
	$(LD) -o $@ -shared  $(OBJECTS) -L$(top_srcdir)/lib/ $(LDFLAGS) $(LIBS)

$(top_srcdir)/lib/libMdsSql.a : $(OBJECTS)
	$(AR) -cr $@ $(OBJECTS)
	ranlib $@

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/
