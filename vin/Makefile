#****************************************************************************
#
# Makefile for vin
# This is a GNU make (gmake) makefile
#****************************************************************************

CROSS := YES
# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG          := NO

# PROFILE can be set to YES to include profiling info, or NO otherwise
PROFILE        := NO

TINYXML_USE_STL := NO
#****************************************************************************

include ../d-top.mak


LIBNAME=libiblock
SONAME=$(LIBNAME).so.1
LIBFILE=$(SONAME).0.1



ifeq (YES, $(CROSS))
EXTRA_LIBS=-L../../../lib -ltinyxml -lacq -lpopt -lrt
LIBS := -L../lib/$(ARCH)/lib -l$(patsubst lib%,%, $(LIBNAME))
else
EXTRA_LIBS=-L../tinyxml/$(ARCH)/lib -l tinyxml \
	-L../lib/$(ARCH)/lib -lacq -lpopt
LIBS :=-L./$(ARCH)/lib -l$(patsubst lib%,%, $(LIBNAME))
endif

#****************************************************************************
# Preprocessor directives
#****************************************************************************


#****************************************************************************
# Include paths
#****************************************************************************

INCS := -I../include -I../../../include/

CFLAGS += $(INCS)
CXXFLAGS += $(INCS)

#********************************************d********************************
# Targets of the build
#****************************************************************************

#APPS += vin calibrate xml_lookup cbm 
APPS += vin calibrate
#test test_cs input_offsets set_nacc

XAPPS := $(patsubst %,$(EXEDIR)/%,$(APPS))

OUTPUT := $(XAPPS)

all:  lib apps crossed-wires


lib: $(LIBDIR)/$(LIBFILE)

apps: $(XAPPS)

.PHONY: lib apps

#****************************************************************************
# Source files
#****************************************************************************
LSRCS  := InputBlock.cpp Switch.cpp DvmReader.cpp \
		VoltageSource.cpp RawLimits.cpp
LOBJS := $(patsubst %,$(OBJDIR)/%,$(LSRCS:.cpp=.o))

#****************************************************************************
# Output
#****************************************************************************


$(XAPPS) :  $(patsubst %,$(OBJDIR)/%.o,$(APPS))
	$(LD) -o $@ $(patsubst %,$(OBJDIR)/%.o,$(notdir $@))  \
	$(LDFLAGS) $(LIBS) $(EXTRA_LIBS)



$(LIBDIR)/$(LIBFILE) : $(LOBJS)
	$(CXX) -shared -Wl,-soname,$(SONAME) -fPIC -o $@ $^ \
	-lstdc++
	-cd $(LIBDIR); \
		$(STRIP) $(LIBFILE); \
		ln -sf $(LIBFILE) $(SONAME); \
		ln -sf $(LIBFILE) $(LIBNAME).so


clean2:
	-rm -Rf $(OUTDIR)
clean:
	make CROSS=NO clean2
	make CROSS=YES clean2	
	-rm -f core *.o $(OUTPUT) $(LOBJS) lib$(LIBNAME)*

zynq: lib apps
	echo zynq cp -va a9/lib/* ../../../lib/
	cp -va a9/lib/* ../../../lib/
	
depend:
	makedepend $(INCS) $(SRCS)


release:
ifeq (YES, $(CROSS))
	$(STRIP) $(OUTPUT)
	$(STRIP) crossed-wires
	cp $(OUTPUT) ../../rootfs/extra/local/bin
	cp *.xml ../../rootfs/extra/local/etc/cal/
	./makelinks ../../rootfs/extra/local/bin
	cp -a $(LIBDIR)/*.so* ../../rootfs/extra/local/lib
	cp -a $(LIBDIR)/*.so* ../lib
	cp crossed-wires ../../rootfs/extra/local/CARE
else
	cp -a $(LIBDIR)/*.so* ../lib/$(LIBDIR)
endif

include ../d-bot.mak

.PHONY: clean clean2

