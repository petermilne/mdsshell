SHELL=/bin/sh
CC=gcc
LD=g77
prefix=/usr/local/mdsplus
top_srcdir=..
exec_prefix=/usr/local/mdsplus
RM=$(top_srcdir)/devscripts/rm_if
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic
LDFLAGS=
INSTALL=../conf/install-sh -c
FC=f95
FFLAGS=-g -O2  -I$(top_srcdir)/include -fpic -Wno-globals -fno-backslash
LIBS=-lpthread -lc  -lm

SOURCES = \
	caxpy.f \
	cgeco.f \
	cmul.f  \
	csint.f  \
	csval.f  \
	daxpy.f  \
	dcsint.f  \
	dcsval.f  \
	dgeco.f  \
	dmul.f  \
	dpermu.f  \
	dppval.f  \
	dsrch.f  \
	dsvrgn.f  \
	gn.f  \
	isrch.f  \
	msceez.f \
	mskrv2.f \
	mstrms.f \
	mskrv1.f \
	msshch.f \
	permu.f  \
	ppval.f  \
	saxpy.f  \
	sgeco.f  \
	smul.f  \
	srch.f  \
	ssrch.f  \
	svign.f  \
	svrgn.f  \
	toms510.f \
	umach.f  \
	zaxpy.f  \
	zgeco.f  \
	zmul.f

OBJECTS = $(SOURCES:.f=.o) boxsmooth.o
EXPORTS = MdsMath.export

all: $(top_srcdir)/lib/ $(top_srcdir)/lib/libMdsMath.so 

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)

clean:
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libMdsMath.so

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsMath.so ${exec_prefix}/lib

$(top_srcdir)/lib/libMdsMath.so : $(OBJECTS) 
	$(LD) -o $@ -shared  $(OBJECTS) -L$(top_srcdir)/lib/ -lMdsShr  $(LIBS)

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/
