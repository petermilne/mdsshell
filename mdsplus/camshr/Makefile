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
LIBRARYNAME=CamShr

#MAJOR	=	$(shell awk '{split($$0,a,"_"); print a[2];}' CVS/Tag)
#MINOR	=	$(shell awk '{split($$0,a,"_"); print a[3];}' CVS/Tag)

MAJOR = 1
MINOR = 8

SOURCES	= \
		add_entry.c              \
		bisearch.c               \
		cam_functions.c           \
		check_for_file.c         \
		check_sema4.c            \
		commit_entry.c           \
		compare_str.c            \
		copy.c                   \
		create_sema4.c           \
		create_tmp_file.c        \
		debug.c                  \
		expand_db.c              \
		get_crate_status.c       \
		get_db_file_size.c       \
		get_file_count.c         \
		get_file_name.c          \
		get_scsi_device_number.c \
		global.c                 \
		issort.c                 \
		lock_file.c              \
		lookup_entry.c           \
		map_data_file.c          \
		map_scsi_device.c        \
		msg.c 				 	 \
		parse_crate_db.c         \
		parse_cts_db.c           \
		QueryHighwayType.c 		 \
		remove_entry.c           \
		scsi_io.c                 \
		ScsiSystemStatus.c		 \
		turn_crate_on_off_line.c \
		unlock_file.c            \
		xlate_logicalname.c

OBJECTS =	$(SOURCES:.c=.o)

CTS_SOURCES = cts_commands.c verbs.c

CTS_OBJECTS = $(CTS_SOURCES:.c=.o)

all : $(top_srcdir)/lib/ $(top_srcdir)/lib/lib$(LIBRARYNAME).so \
      $(top_srcdir)/lib/libcts_commands.so $(top_srcdir)/bin/mdscts

.PHONY: clean
clean:
	@ $(RM) $(OBJECTS)
	@ $(RM) $(top_srcdir)/lib/lib$(LIBRARYNAME).so
	@ $(RM) $(top_srcdir)/lib/lib$(LIBRARYNAME).so.$(MAJOR)
	@ $(RM) $(top_srcdir)/lib/lib$(LIBRARYNAME).so.$(MAJOR).$(MINOR)
	@ $(RM) $(top_srcdir)/lib/libRem$(LIBRARYNAME).so
	@ $(RM) so_locations
	@ $(RM) cts_commands.c
	@ $(RM) $(top_srcdir)/bin/mdscts
	@ $(RM) $(top_srcdir)/lib/libcts_commands.so

depend:
	 @makedepend -- $(CFLAGS) -- $(SOURCES)

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/lib$(LIBRARYNAME).so.$(MAJOR).$(MINOR) ${exec_prefix}/lib
	ln -sf lib$(LIBRARYNAME).so.$(MAJOR).$(MINOR) ${exec_prefix}/lib/lib$(LIBRARYNAME).so.$(MAJOR)
	ln -sf lib$(LIBRARYNAME).so.$(MAJOR) ${exec_prefix}/lib/lib$(LIBRARYNAME).so
	$(INSTALL) -m 755  $(top_srcdir)/lib/libcts_commands.so ${exec_prefix}/lib
	$(INSTALL) -m 755  $(top_srcdir)/bin/mdscts ${exec_prefix}/bin


$(top_srcdir)/bin/mdscts:	cts
	cp  cts $@
	chmod +x $@

cts_commands.c:	cts_commands.ocld
	env LD_LIBRARY_PATH=$(top_srcdir)/lib/ $(top_srcdir)/bin/cdu < cts_commands.ocld
	$(MAKE) depend

$(top_srcdir)/lib/libcts_commands.so:	$(CTS_OBJECTS) $(top_srcdir)/lib/lib$(LIBRARYNAME).so
	$(LD)  -o $@ -shared  $(CTS_OBJECTS) -L$(top_srcdir)/lib/ -lMdsdcl -lCamShr $(LIBS)

$(top_srcdir)/lib/lib$(LIBRARYNAME).so : $(OBJECTS)
	$(CC) -g -nostartfiles -shared -W1,-soname,lib$(LIBRARYNAME).so.$(MAJOR) \
		-o $(top_srcdir)/lib/lib$(LIBRARYNAME).so.$(MAJOR).$(MINOR) $(OBJECTS) -L$(top_srcdir)/lib/ -lMdsShr -lc
	if [ ! -d $(top_srcdir)/lib/ ] ; then \
		mkdir $(top_srcdir)/lib/; \
		chmod 755 $(top_srcdir)/lib/; \
	fi;
	ln -sf lib$(LIBRARYNAME).so.$(MAJOR).$(MINOR) $(top_srcdir)/lib/lib$(LIBRARYNAME).so.$(MAJOR)
	ln -sf lib$(LIBRARYNAME).so.$(MAJOR) $(top_srcdir)/lib/lib$(LIBRARYNAME).so

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/
