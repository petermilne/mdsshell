SHELL=/bin/sh
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
JAVADIR = $(top_srcdir)/java/classes
JAVAC = /usr/bin/javac
JAR = /usr/bin/jar
INSTALL=../conf/install-sh -c

SOURCES = MdsPlusDescriptor.java\
MdsPlusException.java\
MdsPlusEvents.java\
MdsPlusEvent.java\
MdsPlus.java

CLASSES = $(SOURCES:%.java=MdsPlus/%.class)

all: $(top_srcdir)/java/ $(top_srcdir)/java/classes \
     $(top_srcdir)/java/classes/MdsPlus.jar

clean: 
	@ if [ -d MdsPlus ]; then (cd MdsPlus; ../$(RM) $(CLASSES)); fi
	@ $(RM) MdsPlus
	@ $(RM) $(top_srcdir)/java/classes/MdsPlus.jar

depend: ; @ :

install:
	if [ ! -d /usr/local/mdsplus/java/classes ] ; then \
                mkdirhier /usr/local/mdsplus/java/classes; \
                chmod -R 755 /usr/local/mdsplus/java; \
        fi;
	$(INSTALL) -m 644  $(top_srcdir)/java/classes/MdsPlus.jar /usr/local/mdsplus/java/classes

$(top_srcdir)/java/ : ; mkdir $(top_srcdir)/java/

$(top_srcdir)/java/classes : ; mkdir $(top_srcdir)/java/classes

$(top_srcdir)/java/classes/MdsPlus.jar: $(CLASSES)
	$(JAR) c0f $@ MdsPlus/*

$(CLASSES): 	$(SOURCES)
	$(JAVAC) -d . $(SOURCES)
