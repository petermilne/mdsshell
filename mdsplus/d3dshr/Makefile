SHELL=/bin/sh
FC=f95
LD=ld
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
FFLAGS=-g -O2  -I$(top_srcdir)/include -fpic -Wno-globals -fno-backslash 
D3DLIB=
LDFLAGS=
LIBS=-lpthread -lc 
INSTALL=../conf/install-sh -c

SOURCES = mdsptdata.f
OBJECTS = $(SOURCES:.f=.o)

all: $(top_srcdir)/lib/ $(top_srcdir)/lib/libMdsD3D.so  

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)

clean :
	@ $(RM) $(OBJECTS)
	@ $(RM) $(top_srcdir)/lib/libMdsD3D.so

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsD3D.so ${exec_prefix}/lib

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/

$(top_srcdir)/lib/libMdsD3D.so : $(OBJECTS) 
	$(LD) -o $@ -shared  $(OBJECTS)  $(D3DLIB) -ld3 $(LIBS)  
