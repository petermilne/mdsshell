SHELL=/bin/sh
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
INSTALL=../conf/install-sh -c

SCRIPTS = \
        $(top_srcdir)/bin/remote_submit \
	$(top_srcdir)/bin/unix_submit \
	$(top_srcdir)/bin/synchronize_unix

all : $(top_srcdir)/bin/ \
	$(SCRIPTS)

clean :
	@ $(RM) $(SCRIPTS)

install:
	for script in $(SCRIPTS) ; do\
	  $(INSTALL) -m 755 $${script} ${exec_prefix}/bin;\
	done

$(top_srcdir)/bin/remote_submit : remote_submit
	cp remote_submit $(top_srcdir)/bin/remote_submit

$(top_srcdir)/bin/unix_submit : unix_submit
	cp unix_submit $(top_srcdir)/bin/unix_submit

$(top_srcdir)/bin/synchronize_unix : synchronize_unix
	cp synchronize_unix $(top_srcdir)/bin/synchronize_unix

$(top_srcdir)/bin/ :
	mkdir $(top_srcdir)/bin/
