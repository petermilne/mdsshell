include makefile-header

top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
SHELL=/bin/sh
CFLAGS = -g -O2   -I$(top_srcdir)/include -fpic $(GLOBUS_CFLAGS) $(GLOBUS_INCLUDES) -I$(top_srcdir)/include
LDFLAGS = $(GLOBUS_LDFLAGS) $(GLOBUS_LIBS) $(GLOBUS_PKG_LIBS)
EXPORTS = mdsipshr.export
INSTALL=../conf/install-sh -c
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus

ifeq "x" "x"
	modules =
else
	modules = $(top_srcdir)/lib/libMdsIpShr 
endif




SERVER_SOURCES = \
	mdsip_accept_cb.c \
	mdsip_authenticate_cb.c \
	mdsip_become_user.c \
	mdsip_close_cb.c \
	mdsip_current_time.c \
	mdsip_do_message_cb.c \
	mdsip_initialize_io.c \
	mdsip_io_wait.c \
	mdsip_new_client.c \
	mdsip_open_cb.c \
        mdsip_parse_command.c \
	mdsip_process_message.c \
	mdsip_read_header_cb.c \
	mdsip_test_result.c \
	mdsip_write.c \
        mdsip_server.c

CLIENT_SOURCES = \
	mdsip_connect.c \
	mdsip_test_status.c \
	mdsip_send_message.c \
	mdsip_get_message.c \
	mdsip_client_type.c \
	mdsip_flip_header.c \
	mdsip_flip_data.c \
	mdsip_send_arg.c \
	mdsip_compatible_api.c \
	mdsip_compatible_api_2.c \
	mdsip_host_to_ipaddr.c \
	mdsip_get_result.c \
	mdsip_value.c \
	mdsip_put.c \
	mdsip_arglen.c \
	mdsip_open.c \
	mdsip_make_descrip.c \
	mdsip_close.c \
	mdsip_set_default.c \
	mdsip_event_ast.c \
	mdsip_event_can.c \
	mdsip_dispatch_event.c

UTIL_SOURCES = \
	mdsip_connect.c \
	mdsip_test_status.c \
	mdsip_send_message.c \
	mdsip_get_message.c \
	mdsip_client_type.c \
	mdsip_flip_header.c \
	mdsip_flip_data.c \
	mdsip_send_arg.c \
	mdsip_compatible_api.c \
	mdsip_host_to_ipaddr.c \
	mdsip_get_result.c \
	mdsip_value.c \
	mdsip_put.c \
	mdsip_arglen.c \
	mdsip_open.c \
	mdsip_make_descrip.c \
	mdsip_close.c \
	mdsip_set_default.c \
	mdsip_event_ast.c \
	mdsip_event_can.c \
	mdsip_dispatch_event.c

SERVER_OBJECTS = $(SERVER_SOURCES:.c=.o)
CLIENT_OBJECTS = $(CLIENT_SOURCES:.c=.o)
UTIL_OBJECTS = $(UTIL_SOURCES:.c=.o)

all : makefile-header \
      $(top_srcdir)/lib/libMdsIpShr.so \
      $(modules) \
      $(top_srcdir)/bin/ $(top_srcdir)/lib/ $(top_srcdir)/etc/ \
      $(top_srcdir)/bin/mdsiptest $(top_srcdir)/bin/mdsip \
      $(top_srcdir)/bin/mdsipd $(top_srcdir)/etc/mdsip.hosts \
      $(top_srcdir)/bin/mkdirhier \
      $(top_srcdir)/lib/libMdsIpShr.a $(top_srcdir)/lib/libMdsIpUtil.a 

depend:
	@makedepend -- $(CFLAGS) -- $(SERVER_SOURCES) $(CLIENT_SOURCES)

clean: 
	@ $(RM) $(SERVER_OBJECTS) $(CLIENT_OBJECTS)
	@ $(RM) $(top_srcdir)/lib/libMdsIpShr.so
	@ $(RM) $(top_srcdir)/bin/mdsip
	@ $(RM) makefile-header
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libMdsIpShr.a
	@ $(RM) $(top_srcdir)/lib/libMdsIpUtil.a
	@ $(RM) $(top_srcdir)/bin/mdsiptest
	@ $(RM) $(top_srcdir)/bin/mdsip
	@ $(RM) $(top_srcdir)/bin/mdsipd
	@ $(RM) $(top_srcdir)/bin/mkdirhier
	@ $(RM) $(top_srcdir)/etc/mdsip.hosts


install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	if [ ! -d ${exec_prefix}/bin ] ; then \
	        mkdir ${exec_prefix}/bin; \
	chmod 755 ${exec_prefix}/bin; \
	fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsIpShr.so ${exec_prefix}/lib
        ifdef modules
		$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsIpShr ${exec_prefix}/lib
        endif
	$(INSTALL) -m 755  $(top_srcdir)/bin/mdsip	${exec_prefix}/bin
	$(INSTALL) -m 755  $(top_srcdir)/bin/mdsipd	${exec_prefix}/bin
	$(INSTALL) -m 755  $(top_srcdir)/bin/mkdirhier ${exec_prefix}/bin
	if test ".so" != ".a" ; then \
		$(INSTALL) -m 644  $(top_srcdir)/lib/libMdsIpShr.a ${exec_prefix}/lib; \
		$(INSTALL) -m 644  $(top_srcdir)/lib/libMdsIpUtil.a ${exec_prefix}/lib; \
	fi;
	if [ ! -d $(exec_prefix)/etc ] ; then \
                mkdirhier $(exec_prefix)/etc; \
                chmod 755 $(exec_prefix)/etc; \
        fi;
	$(INSTALL) -m 755  mdsip.hosts $(exec_prefix)/etc


$(top_srcdir)/lib/libMdsIpShr.so : $(CLIENT_OBJECTS)
	$(LD) -o $@ -shared  $(CLIENT_OBJECTS) $(LDFLAGS)

$(top_srcdir)/bin/mdsip : $(SERVER_OBJECTS)
	$(CC) -o $@ $(SERVER_OBJECTS) $(LDFLAGS) -L$(top_srcdir)/lib -lMdsIpShr -lMdsShr -lTdiShr

makefile-header:  
	GLOBUS_LOCATION=; $$GLOBUS_LOCATION/bin/globus-makefile-header --flavor= globus_xio > makefile-header

$(top_srcdir)/bin/ :
	mkdir $(top_srcdir)/bin/

$(top_srcdir)/bin/mdsiptest : t.c $(top_srcdir)/lib/libMdsIpShr.so
	$(CC) -o $@ $(CFLAGS) t.c -L$(top_srcdir)/lib/ -lMdsIpShr -lMdsShr $(LIBS) $(GLOBUS_LDFLAGS) $(GLOBUS_LIBS) $(GLOBUS_PKG_LIBS)

$(top_srcdir)/bin/mdsipd : mdsipd
	cp mdsipd $(top_srcdir)/bin/mdsipd

$(top_srcdir)/lib/libMdsIpShr.a : $(CLIENT_OBJECTS)
	$(AR) -cr $@ $(CLIENT_OBJECTS)
	ranlib $@

ifdef modules
$(top_srcdir)/lib/libMdsIpShr : $(CLIENT_OBJECTS)
	$(LD) -o $@  $(CLIENT_OBJECTS) $(LIBS) 
endif

$(top_srcdir)/lib/libMdsIpUtil.a : $(UTIL_OBJECTS)
	$(AR) -cr $@ $(UTIL_OBJECTS)
	ranlib $@

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/

$(top_srcdir)/etc/ :
	mkdir $(top_srcdir)/etc/

$(top_srcdir)/etc/mdsip.hosts : mdsip.hosts
	cp mdsip.hosts $(top_srcdir)/etc/mdsip.hosts

$(top_srcdir)/bin/mkdirhier : mkdirhier
	cp mkdirhier $(top_srcdir)/bin/mkdirhier
