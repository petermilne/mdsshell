SHELL=/bin/sh
CC=gcc
LD=ld
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic
LDFLAGS=
LIBS=-L$(top_srcdir)/lib/ -lMdsdcl -lMdsShr -lCamShr -lpthread -lc 
INSTALL=../conf/install-sh -c

SOURCES = ccl_commands.c ccl_verbs.c
OBJECTS = $(SOURCES:.c=.o)
EXPORTS = ccl_commands.export

all:	$(top_srcdir)/lib/ $(top_srcdir)/lib/libccl_commands.so  $(top_srcdir)/bin/ $(top_srcdir)/bin/mdsccl

clean:
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) ccl_commands.c
	@ $(RM) $(top_srcdir)/lib/libccl_commands.so
	@ $(RM) $(top_srcdir)/bin/mdsccl

depend:
	 @makedepend -- $(CFLAGS) -- $(SOURCES)

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libccl_commands.so ${exec_prefix}/lib
	$(INSTALL) -m 755  $(top_srcdir)/bin/mdsccl ${exec_prefix}/bin

ccl_commands.c:	ccl_commands.ocld
	env LD_LIBRARY_PATH=$(top_srcdir)/lib/ $(top_srcdir)/bin/cdu < ccl_commands.ocld
	$(MAKE) depend

$(top_srcdir)/lib/libccl_commands.so:	$(OBJECTS)
	$(LD) -o $@ -shared  $(OBJECTS) $(LIBS)

$(top_srcdir)/bin/mdsccl:	ccl
	cp ccl $@
	chmod +x $@

