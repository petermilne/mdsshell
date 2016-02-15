SHELL=/bin/sh
CC=gcc
LD=ld
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic
LDFLAGS=
LIBS=-L$(top_srcdir)/lib/ -lMdsShr -lpthread -lc 
INSTALL=../conf/install-sh -c

SOURCES = \
dummies.c \
TreeAddNode.c \
TreeAddTag.c \
TreeCallHook.c \
TreeCleanDatafile.c \
TreeCreatePulseFile.c \
TreeDeleteNode.c \
TreeDeletePulseFile.c \
TreeDoMethod.c \
TreeFindNode.c \
TreeFindTagWild.c \
TreeGetDbi.c \
TreeGetNci.c \
TreeGetRecord.c \
TreeGetSetShotId.c \
TreeOpen.c \
TreePutRecord.c \
TreeRenameNode.c \
TreeRemoveNodesTags.c\
TreeSerializeNci.c \
TreeSetDefault.c \
TreeSetNci.c \
TreeVerify.c \
RemoteAccess.c

OBJECTS = $(SOURCES:.c=.o)
EXPORTS = TreeShr.exports

all : $(top_srcdir)/lib/ $(top_srcdir)/lib/ $(top_srcdir)/lib/libTreeShr.so $(top_srcdir)/lib/libTreeShr.a

clean:
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libTreeShr.so
	@ $(RM) $(top_srcdir)/lib/libTreeShr.a

depend:
	 @makedepend -- $(CFLAGS) -- $(SOURCES)

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libTreeShr.so ${exec_prefix}/lib
	if test .so != .a ; then \
		$(INSTALL) -m 644  $(top_srcdir)/lib/libTreeShr.a ${exec_prefix}/lib; \
	fi;


$(top_srcdir)/lib/libTreeShr.so : $(OBJECTS)  
	$(LD) -o $@ -shared  $(OBJECTS) $(LIBS)

$(top_srcdir)/lib/libTreeShr.a : $(OBJECTS)
	$(AR) -cr $@ $(OBJECTS)
	ranlib $@

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/

TreeShr.exports: TreeShr.def
	sed -e /LIBRARY/\;/\;/d TreeShr.def > $@
