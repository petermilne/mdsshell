SHELL=/bin/sh
prefix=/usr/local/mdsplus
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
JAVADIR = $(top_srcdir)/java/classes
JAVAC = /usr/bin/javac
JAR = /usr/bin/jar
INSTALL=../conf/install-sh -c
DOCS=ConnectionEvent.html      DataProvider.html         UpdateEvent.html          WaveData.html\
ConnectionListener.html   FrameData.html            UpdateEventListener.html  jScope.html    data_setup.jpg \
frame_popup.jpg  image_setup.jpg  jScope.jpg       popup.jpg CompositeWaveDisplay.html


APPLETSOURCES = \
FtuDataProvider.java        JiVarImpl.java              Signal.java                 WaveformManager.java \
Base64.java                 Grid.java  SignalCache.java            WaveformMetrics.java \
JetDataProvider.java        MdsAccess.java   \
JetMdsDataProvider.java     MdsConnection.java          TwuAccess.java \
ConnectionEvent.java        JiDataSource.java           MdsDataProvider.java        TwuDataProvider.java  \
ConnectionListener.java     JiDim.java                  MdsMessage.java             UpdateEvent.java \
DataAccess.java             JiNcSource.java             UpdateEventListener.java    	 \
DataAccessURL.java          JiNcVar.java  \
DataCacheObject.java        JiNcVarByte.java            MultiWavePopup.java\
DataCached.java             JiNcVarChar.java            MultiWaveform.java          WaveData.java  \
DataProvider.java           JiNcVarDouble.java          WaveDisplay.java  \
DataServerItem.java         JiNcVarFloat.java           RandomAccessData.java       WaveInterface.java         	\
JiNcVarImp.java             RdaAccess.java              WavePopup.java             	 \
Descriptor.java             JiNcVarInt.java             SshTunneling.java          Waveform.java             	 \
JiNcVarShort.java  \
FrameData.java              JiSlab.java 		WaveformEditor.java	    WaveformEditorListener.java\
JiSlabIterator.java         			    WaveformEvent.java  	   SetupWaveformParams.java 	SignalBox.java\
Frames.java                 JiVar.java              UniversalDataProvider.java     WaveformListener.java NotConnectedDataProvider.java \
ASCIIDataProvider.java      ImageTransferable.java



SOURCES = AboutWindow.java            FtuDataProvider.java        JiVarImpl.java              Signal.java                 WaveformManager.java \
Base64.java                 Grid.java                   LocalDataProvider.java      SignalCache.java            WaveformMetrics.java \
ColorDialog.java            JetDataProvider.java        MdsAccess.java              TextorBrowseSignals.java     \
CompositeWaveDisplay.java   JetMdsDataProvider.java     MdsConnection.java          TwuAccess.java              jScope.java \
ConnectionEvent.java        JiDataSource.java           MdsDataProvider.java        TwuDataProvider.java        jScopeBrowseSignals.java \
ConnectionListener.java     JiDim.java                  MdsMessage.java             UpdateEvent.java            jScopeBrowseUrl.java \
DataAccess.java             JiNcSource.java             MdsPlusBrowseSignals.java   UpdateEventListener.java    jScopeDefaultValues.java \
DataAccessURL.java          JiNcVar.java                MdsWaveInterface.java       WaveContainerEvent.java      \
DataCacheObject.java        JiNcVarByte.java            MultiWavePopup.java         WaveContainerListener.java  \
DataCached.java             JiNcVarChar.java            MultiWaveform.java          WaveData.java               jScopeMultiWave.java \
DataProvider.java           JiNcVarDouble.java          ProfileDialog.java          WaveDisplay.java             \
DataServerItem.java         JiNcVarFloat.java           RandomAccessData.java       WaveInterface.java          jScopeProperties.java \
DemoDataProvider.java       JiNcVarImp.java             RdaAccess.java              WavePopup.java              jScopeWaveContainer.java \
Descriptor.java             JiNcVarInt.java             SshTunneling.java            Waveform.java                \
FontSelection.java          JiNcVarShort.java           RowColumnContainer.java     WaveformContainer.java      jScopeWavePopup.java \
FrameData.java              JiSlab.java                 RowColumnLayout.java           \
JiSlabIterator.java         SetupDataDialog.java        WaveformEvent.java          DemoAccess.java		UniversalDataProvider.java\
Frames.java                 JiVar.java                  SetupDefaults.java          WaveformListener.java	NotConnectedDataProvider.java\
SignalsBoxDialog.java	    SignalBox.java		MdsDataClient.java	    MdsIOException.java		SetupWaveformParams.java \
TSDataProvider.java	    AsdexDataProvider.java	WaveformEditor.java	    WaveformEditorListener.java MdsplusParser.java \
ASCIIDataProvider.java      ImageTransferable.java

CLASSES = $(SOURCES:.java=.class)

all:	$(top_srcdir)/bin/ $(top_srcdir)/bin/jScope $(top_srcdir)/java/ $(top_srcdir)/java/classes $(top_srcdir)/java/classes/docs $(top_srcdir)/java/classes/jScope.jar  $(top_srcdir)/java/classes/WaveDisplay.jar docs  $(top_srcdir)/java/classes/jScope.properties $(top_srcdir)/java/classes/MindTerm.jar

clean: 
	@ $(RM) *.class
	@ $(RM) $(top_srcdir)/bin/jScope
	@ $(RM) $(top_srcdir)/java/classes/jScope.jar
	@ $(RM) $(top_srcdir)/java/classes/WaveDisplay.jar

depend: ; @ :

install:
	if [ ! -d /usr/local/mdsplus/java/classes ] ; then \
                mkdirhier /usr/local/mdsplus/java/classes ; \
                chmod -R 755 /usr/local/mdsplus/java; \
        fi;
	$(INSTALL) -m 644  $(top_srcdir)/java/classes/jScope.jar /usr/local/mdsplus/java/classes
	$(INSTALL) -m 644  $(top_srcdir)/java/classes/WaveDisplay.jar /usr/local/mdsplus/java/classes
	$(INSTALL) -m 644  jScope.properties /usr/local/mdsplus/java/classes
	$(INSTALL) -m 755  $(top_srcdir)/bin/jScope /usr/local/mdsplus/bin

$(top_srcdir)/java/ : ; mkdir $(top_srcdir)/java/

$(top_srcdir)/java/classes : ; mkdir $(top_srcdir)/java/classes

$(top_srcdir)/java/classes/docs : ; mkdir $(top_srcdir)/java/classes/docs

$(top_srcdir)/java/classes/jScope.jar: jScope.class CompositeWaveDisplay.class
	$(JAR) c0f $@ *.class

$(top_srcdir)/java/classes/jScope.properties: jScope.properties
	cp jScope.properties ../java/classes

$(top_srcdir)/java/classes/MindTerm.jar: MindTerm.jar
	cp MindTerm.jar ../java/classes

$(top_srcdir)/java/classes/WaveDisplay.jar: $(APPLETSOURCES)
	rm *.class
	$(JAVAC) -g -classpath .:$(top_srcdir)/javascope/MindTerm.jar $(APPLETSOURCES) 
	$(JAR) c0f $@ *.class

jScope.class: 	$(SOURCES)
	$(JAVAC) -g  -classpath .:$(top_srcdir)/javascope/MindTerm.jar jScope.java *BrowseSignals.java *DataProvider.java *Listener.java WaveformEditor.java

CompositeWaveDisplay.class: 	$(SOURCES)
	$(JAVAC) -g  -classpath .:$(top_srcdir)/javascope/MindTerm.jar CompositeWaveDisplay.java

$(top_srcdir)/bin/ : ; mkdir $(top_srcdir)/bin/

$(top_srcdir)/bin/jScope : jScope.template
	cp jScope.template $@
	chmod a+x $@

docs: $(DOCS) 
	cp $(DOCS) $(top_srcdir)/java/classes/docs
