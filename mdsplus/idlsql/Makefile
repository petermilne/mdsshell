SHELL=/bin/sh
CC=gcc
LD=ld
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic   
LDFLAGS= 
LIBS= -lMdsSql  -lpthread -lc   
INSTALL=../conf/install-sh -c

SOURCES = \
idlsql_syb.c \
user_syb.c

OBJECTS = $(SOURCES:.c=.o)
EXPORTS = IdlSql.export

all : $(top_srcdir)/lib/ $(top_srcdir)/lib/libIdlSql.so $(top_srcdir)/lib/libIdlSql.a  $(top_srcdir)/bin/ $(top_srcdir)/bin/logbook

clean:
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libIdlSql.so
	@ $(RM) $(top_srcdir)/lib/libIdlSql.a
	@ $(RM) $(top_srcdir)/bin/logbook


depend:
	 @makedepend -- $(CFLAGS) -- $(SOURCES)

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 644  $(top_srcdir)/lib/libIdlSql.a ${exec_prefix}/lib
	$(INSTALL) -m 755  $(top_srcdir)/lib/libIdlSql.so ${exec_prefix}/lib
	$(INSTALL) -m 755  $(top_srcdir)/bin/logbook ${exec_prefix}/bin

$(top_srcdir)/lib/libIdlSql.so : $(OBJECTS) 
	$(LD) -o $@ -shared  $(OBJECTS) -L$(top_srcdir)/lib/ $(LDFLAGS) $(LIBS)

$(top_srcdir)/lib/libIdlSql.a : $(OBJECTS)
	$(AR) -cr $@ $(OBJECTS)
	ranlib $@

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/

$(top_srcdir)/bin/ :
	mkdir $(top_srcdir)/bin/

$(top_srcdir)/bin/logbook:     logbook
	cp logbook $@
	chmod +x $@
