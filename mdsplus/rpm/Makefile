SHELL=/bin/sh
top_srcdir=..
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
INSTALL=../conf/install-sh -c


all : 
	:
      

depend:
	:

clean :
	:

install:
	if [ ! -d $(exec_prefix)/etc/profile.d ] ; then \
                mkdirhier $(exec_prefix)/etc/profile.d; \
                chmod -R 755 $(exec_prefix)/etc; \
	fi;
	if [ ! -d $(exec_prefix)/rpm ] ; then \
		mkdirhier $(exec_prefix)/rpm; \
		chmod -R 755 $(exec_prefix)/rpm; \
	fi;          
	$(INSTALL) -m 755  envsyms $(exec_prefix)/etc
	$(INSTALL) -m 755  mdsplus.conf.template $(exec_prefix)/etc
	$(INSTALL) -m 755  post_uninstall_script $(exec_prefix)/rpm
	$(INSTALL) -m 755  post_install_script   $(exec_prefix)/rpm
	$(INSTALL) -m 755  mdsipd.xinetd	 $(exec_prefix)/rpm