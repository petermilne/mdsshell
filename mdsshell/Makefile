# Makefile for mdsshell

CROSS := YES
DEBUG := NO


include ../d-top.mak


INCS=-I ../mdsplus/include -I../include -I../../../include/


ifeq (YES, $(CROSS))
CFLAGS += $(INCS)  -D__arm $(RELEASE_CFLAGS) 
LIBS=-L../../../lib/ -L../mdsplus/lib/ -l popt -l acq -lMdsIpShr
else
CFLAGS += $(INCS)
LIBS=-L../lib/$(ARCH)/lib -l popt -lMdsIpShr
endif

LFLAGS=$(LIBS)
#LFLAGS=-L${HOME}/PROJECTS/ACQ200/tools/lib -L../lib -l popt -lMdsIpShr
#CC=/usr/local/arm/2.95.3/bin/arm-linux-gcc
# uncomment next line for xcompile

CFLAGS += -D__DTACQ_2G__ 

LFLAGS_ACQ=-L${HOME}/PROJECTS/ACQ200/tools/lib -lpopt -L../lib -lacq

MDSSHELL_UTIL := readline usc timeout sendfile
MDSSHELL_CORE := mdsshell $(MDSSHELL_UTIL)

MDSSHELL_SRC := mdsshell.af_unix $(MDSSHELL_CORE)
MDSSHELL_OBJ := $(patsubst %,$(OBJDIR)/%.o,$(MDSSHELL_SRC))

MDSSHELL_INET_SRC := mdsshell.af_inet $(MDSSHELL_CORE)
MDSSHELL_INET_OBJ := $(patsubst %,$(OBJDIR)/%.o,$(MDSSHELL_INET_SRC))

MDSPUTCH_SRC := mdsPutCh $(MDSSHELL_UTIL) Timebase 
MDSPUTCH_OBJ := $(patsubst %,$(OBJDIR)/%.o,$(MDSPUTCH_SRC))

MDSPUTCAL_SRC := mdsPutCal $(MDSSHELL_UTIL)  
MDSPUTCAL_OBJ := $(patsubst %,$(OBJDIR)/%.o,$(MDSPUTCAL_SRC))

READVOLTS_SRC := readVolts Timebase
READVOLTS_OBJ := $(patsubst %,$(OBJDIR)/%.o,$(READVOLTS_SRC))


APPS := mdsshell mdsshell.inet 
ifeq (YES, $(CROSS))
APPS += mdsPutCh readVolts mdsPutCal
endif

XAPPS := $(patsubst %,$(EXEDIR)/%,$(APPS))

DC := $(shell date +%Y%m%d%H%M)
PACKAGE_FILE := mdsshell.$(DC).tgz
PACKOPTS := --exclude-from=mdsshell/dontpack

all: .depend $(XAPPS)

zynq:
	./make.zynq

# DO NOT USE TRANSFORM. Simple shell script does it better

$(patsubst %,$(EXEDIR)/%, mdsshell): $(MDSSHELL_OBJ)
	$(CC) -o $@ $^ $(LIBS)

$(patsubst %,$(EXEDIR)/%, mdsshell.inet):  $(MDSSHELL_INET_OBJ)
	$(CC) -o $@ $^ $(LIBS)

$(patsubst %,$(EXEDIR)/%, mdsPutCh): $(MDSPUTCH_OBJ)
	$(CXX)  -o $@ $^ $(LFLAGS) -lacq -liblock -ltinyxml

$(patsubst %,$(EXEDIR)/%, mdsPutCal): $(MDSPUTCAL_OBJ)
	$(CXX)  -o $@ $^ $(LFLAGS) -lacq -liblock -ltinyxml

$(patsubst %,$(EXEDIR)/%, readVolts): $(READVOLTS_OBJ)
	$(CXX)  -o $@ $^ $(LFLAGS) -lacq -liblock -ltinyxml


clean2:
	-rm -Rf $(OUTDIR)
clean: 
	make CROSS=YES clean2
	make CROSS=NO clean2
	@-rm mdsshell
	@-rm .depend	


package:
	(cd ..;tar cvzf /tmp/$(PACKAGE_FILE) $(PACKOPTS) *.mak mdsshell include)
	mkdir -p rel
	mv /tmp/$(PACKAGE_FILE) rel
	echo PACKAGE_FILE rel/$(PACKAGE_FILE) created

depend .depend dep:
	$(CC) $(CFLAGS) $(CPPFLAGS) -M *.c *.cpp >> .depend

install_canonical_tree_support:
	echo cp bin/* /usr/local/bin
	cp bin/* /usr/local/bin
	mkdir -p /usr/local/mdsplus/tdi/d-tacq
	echo cp tdi/* /usr/local/mdsplus/tdi/d-tacq
	cp tdi/* /usr/local/mdsplus/tdi/d-tacq

make_acqtree:
	@echo make patch for standard MDSplus dist for canonical tree
	mkdir -p tmp/bin
	mkdir -p tmp/mdsplus/tdi/d-tacq
	rsync -aC tdi/ tmp/mdsplus/tdi/d-tacq
	rsync -aC bin/ tmp/bin
	cd tmp;tar cvf ../make_acqtree.$(DC).tar .
	@echo make_acqtree.$(DC).tar created. extract from /usr/local

install_x86:
	cp x86/bin/* /usr/local/bin
	mkdir -p ~/.octave
	cp octave/*.m ~/.octave

EXTRA=${HOME}/PROJECTS/ACQ200/rootfs/extra

deploy: all
ifeq (YES, $(CROSS))
	$(STRIP) $(XAPPS)
	cp $(XAPPS) ../../rootfs/extra/local/bin
	sudo cp root/.mds* ../../rootfs/extra/local/etc/mdsshell
	cp CARE/* ../../rootfs/extra/local/CARE
endif


include ../d-bot.mak


.PHONEY: install_canonical_tree_support
