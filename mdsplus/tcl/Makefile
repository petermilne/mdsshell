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

SOURCES = 			\
	tcl_dummy.c		\
	tcl_add_node.c	\
	tcl_add_tag.c		\
	tcl_clean_datafile.c		\
	tcl_close.c		\
	tcl_commands.c	\
	tcl_compress_datafile.c		\
	tcl_create_pulse.c	\
	tcl_decompile.c	\
	tcl_delete_node.c	\
	tcl_delete_pulse.c	\
	tcl_directory.c	\
	tcl_directory_tag.c	\
	tcl_dispatch.c		\
	tcl_do_method.c	\
	tcl_do_node.c	\
	tcl_edit.c		\
	tcl_put_expression.c		\
	tcl_remove_tag.c	\
	tcl_rename.c		\
	tcl_set_callbacks.c	\
	tcl_set_current.c	\
	tcl_set_default.c	\
	tcl_set_node.c	\
	tcl_set_tree.c	\
	tcl_setevent.c \
	tcl_show_current.c	\
	tcl_show_data.c	\
	tcl_show_db.c	\
	tcl_show_version.c \
	tcl_verify_tree.c	\
	tcl_wfevent.c \
	tcl_write.c

OBJECTS = $(SOURCES:.c=.o)
EXPORTS = tcl_commands.export

all:	$(top_srcdir)/bin/mdstcl $(top_srcdir)/lib/libtcl_commands.so

depend: 
	 @makedepend -- $(CFLAGS) -- $(SOURCES) 

clean:
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) tcl_commands.c
	@ $(RM) $(top_srcdir)/bin/mdstcl
	@ $(RM) $(top_srcdir)/lib/libtcl_commands.so

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libtcl_commands.so ${exec_prefix}/lib
	$(INSTALL) -m 755  $(top_srcdir)/bin/mdstcl ${exec_prefix}/bin

$(top_srcdir)/bin/mdstcl:	tcl
	cp  tcl $@
	chmod +x $@

tcl_commands.c:	tcl_commands.ocld
	env LD_LIBRARY_PATH=$(top_srcdir)/lib/ $(top_srcdir)/bin/cdu < tcl_commands.ocld
	$(MAKE) depend

$(top_srcdir)/lib/libtcl_commands.so:	$(OBJECTS)
	$(LD)  -o $@ -shared  $(OBJECTS) -L$(top_srcdir)/lib/ -lMdsdcl -lMdsServerShr -lMdsShr -lTreeShr -lTdiShr $(LIBS)
