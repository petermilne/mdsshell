SHELL=/bin/sh
CC=gcc
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
GROFF_FLAGS=-t -man -Tascii
GROFF=groff
INSTALL=../conf/install-sh -c

manpages = $(top_srcdir)/man/man1/mdsplus.1.gz \
           $(top_srcdir)/man/man1/traverser.1.gz \
           $(top_srcdir)/man/man5/mdsplus.conf.5.gz \
           $(top_srcdir)/man/man5/.mdsplus.5.gz \
           $(top_srcdir)/man/man5/mdsip.hosts.5.gz

directories = $(top_srcdir)/man/man1 \
              $(top_srcdir)/man/man5

all : $(directories) $(manpages)


depend:
	@ :

clean :
	@ $(RM) $(manpages)
	@ $(RM) $(directories)
	@ $(RM) $(top_srcdir)/man

install: 
	- mkdirhier $(exec_prefix)/man/man1
	$(INSTALL) -m 755  $(top_srcdir)/man/man1/mdsplus.1.gz $(exec_prefix)/man/man1
	$(INSTALL) -m 755  $(top_srcdir)/man/man1/traverser.1.gz $(exec_prefix)/man/man1
	- mkdirhier $(exec_prefix)/man/man5
	$(INSTALL) -m 755  $(top_srcdir)/man/man5/mdsplus.conf.5.gz $(exec_prefix)/man/man5
	$(INSTALL) -m 755  $(top_srcdir)/man/man5/.mdsplus.5.gz $(exec_prefix)/man/man5
	$(INSTALL) -m 755  $(top_srcdir)/man/man5/mdsip.hosts.5.gz $(exec_prefix)/man/man5


$(top_srcdir)/man/man1 :
	mkdirhier $@

$(top_srcdir)/man/man5 :
	mkdirhier $@

$(top_srcdir)/man/man1/traverser.1.gz : traverser.groff
	$(GROFF) $(GROFF_FLAGS) traverser.groff | gzip - -c > $@

$(top_srcdir)/man/man1/mdsplus.1.gz : mdsplus.groff
	$(GROFF) $(GROFF_FLAGS) mdsplus.groff | gzip - -c > $@

$(top_srcdir)/man/man5/mdsplus.conf.5.gz : mdsplus.conf.groff
	$(GROFF) $(GROFF_FLAGS) mdsplus.conf.groff | gzip - -c > $@

$(top_srcdir)/man/man5/.mdsplus.5.gz : .mdsplus.groff
	$(GROFF) $(GROFF_FLAGS) .mdsplus.groff | gzip - -c > $@

$(top_srcdir)/man/man5/mdsip.hosts.5.gz : mdsip.hosts.groff
	$(GROFF) $(GROFF_FLAGS) mdsip.hosts.groff | gzip - -c > $@
