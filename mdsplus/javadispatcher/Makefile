SHELL=/bin/sh
prefix=/usr/local/mdsplus
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
JAVADIR = $(top_srcdir)/java/classes
JAVAC = /usr/bin/javac -classpath .:../java/classes/jScope.jar:../java/classes/jTraverser.jar
JAR = /usr/bin/jar
INSTALL=../conf/install-sh -c

SOURCES = Action.java   MdsHelper.java          MdsServerEvent.java     NetworkListener.java    jDispatcher.java \
ActionServer.java       MdsIp.java              MdsServerListener.java  Server.java             jDispatcherIp.java \
Balancer.java           MdsMonitor.java         MonitorEvent.java       ServerEvent.java \
MdsMonitorEvent.java    MonitorListener.java    ServerListener.java \
InfoServer.java         MdsServer.java          NetworkEvent.java       jDispatchMonitor.java


CLASSES = $(SOURCES:.java=.class)

all:	$(top_srcdir)/bin/ $(top_srcdir)/bin/jDispatcherIp $(top_srcdir)/bin/jDispatchMonitor $(top_srcdir)/java/ $(top_srcdir)/java/classes  $(top_srcdir)/java/classes/jDispatcher.jar 

clean: 
	@ $(RM) *.class
	@ $(RM) $(top_srcdir)/bin/jDispatcherIp
	@ $(RM) $(top_srcdir)/bin/jDispatchMonitor
	@ $(RM) $(top_srcdir)/java/classes/jDispatcher.jar

depend: ; @ :

install:
	if [ ! -d /usr/local/mdsplus/java/classes ] ; then \
                mkdirhier /usr/local/mdsplus/java/classes ; \
                chmod -R 755 /usr/local/mdsplus/java; \
        fi;
	$(INSTALL) -m 644  $(top_srcdir)/java/classes/jDispatcher.jar /usr/local/mdsplus/java/classes
	$(INSTALL) -m 644  jDispatcher.properties /usr/local/mdsplus/java/classes
	$(INSTALL) -m 755  $(top_srcdir)/bin/jDispatcherIp /usr/local/mdsplus/bin
	$(INSTALL) -m 755  $(top_srcdir)/bin/jDispatchMonitor /usr/local/mdsplus/bin

$(top_srcdir)/java/ : ; mkdir $(top_srcdir)/java/

$(top_srcdir)/java/classes : ; mkdir $(top_srcdir)/java/classes

$(top_srcdir)/java/classes/jDispatcher.jar: jDispatcherIp.class jDispatchMonitor.class
	$(JAR) c0f $@ *.class

jDispatcherIp.class: 	$(SOURCES)
	$(JAVAC) -g jDispatcherIp.java

jDispatchMonitor.class: 	$(SOURCES)
	$(JAVAC) -g jDispatchMonitor.java

$(top_srcdir)/bin/ : ; mkdir $(top_srcdir)/bin/

$(top_srcdir)/bin/jDispatcherIp : jDispatcherIp.template
	cp jDispatcherIp.template $@
	chmod a+x $@

$(top_srcdir)/bin/jDispatchMonitor : jDispatchMonitor.template
	cp jDispatchMonitor.template $@
	chmod a+x $@

