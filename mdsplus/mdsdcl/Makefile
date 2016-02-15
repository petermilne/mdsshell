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

SOURCES = 		\
mdsdcl_dcl_parse.c		\
mdsdcl_do_command.c	\
mdsdcl_dummy.c		\
mdsdcl_general.c		\
mdsdcl_get_input.c		\
mdsdcl_initialize.c	\
mdsdcl_insert_symbols.c	\
mdsdcl_tables.c		\
mdsdcl_macros.c		\
mdsdcl_set_command.c	\
mdsdcl_show_vm.c        \
mdsdcl_spawn.c		\
mdsdcl_wait.c

OBJECTS = $(SOURCES:.c=.o)
EXPORTS = mdsdclshr.export

all:	$(top_srcdir)/lib/ $(top_srcdir)/bin/ $(top_srcdir)/lib/libMdsdcl.a  $(top_srcdir)/lib/libMdsdcl.so $(top_srcdir)/bin/mdsdcl

clean:
	@ $(RM) $(OBJECTS) mdsdcl.o
	@ $(RM) so_locations
	@ $(RM) mdsdcl_tables.c
	@ $(RM) $(top_srcdir)/lib/libMdsdcl.a
	@ $(RM) $(top_srcdir)/lib/libMdsdcl.so
	@ $(RM) $(top_srcdir)/bin/mdsdcl

depend: 
	 @makedepend -- $(CFLAGS) -- $(SOURCES) mdsdcl.c

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsdcl.so ${exec_prefix}/lib
	$(INSTALL) -m 755  $(top_srcdir)/bin/mdsdcl	${exec_prefix}/bin

mdsdcl_tables.c:	mdsdcl_tables.ocld
	env LD_LIBRARY_PATH=$(top_srcdir)/lib/ $(top_srcdir)/bin/cdu < mdsdcl_tables.ocld

$(top_srcdir)/lib/libMdsdcl.a:	$(OBJECTS)
		$(AR) -cr $@ $(OBJECTS)
		ranlib $@

$(top_srcdir)/lib/libMdsdcl.so:	$(OBJECTS)
	$(LD)  -o $@ -shared  $(OBJECTS) -L$(top_srcdir)/lib/ -lCdu -L$(top_srcdir)/lib/ -lMdsShr  $(LIBS)

$(top_srcdir)/bin/mdsdcl:	mdsdcl.o
	$(CC) $(CFLAGS) -o $@ mdsdcl.o -L$(top_srcdir)/lib/ -lMdsdcl -lMdsShr -L$(top_srcdir)/lib/ -lCdu
