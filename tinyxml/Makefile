#****************************************************************************
#
# Makefile for TinyXml test.
# Lee Thomason
# www.grinninglizard.com
#
# This is a GNU make (gmake) makefile
# mods by peter.milne@d-tacq.com to complete cross compilation
#****************************************************************************

CROSS := YES
# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG          := NO

# PROFILE can be set to YES to include profiling info, or NO otherwise
PROFILE        := NO

# TINYXML_USE_STL can be used to turn on STL support. NO, then STL
# will not be used. YES will include the STL files.
TINYXML_USE_STL := NO

#****************************************************************************

include ../d-top.mak



#****************************************************************************
# Preprocessor directives
#****************************************************************************

ifeq (YES, $(TINYXML_USE_STL))
  DEFS := -DTIXML_USE_STL
else
  DEFS :=
endif

#****************************************************************************
# Include paths
#****************************************************************************

#INCS := -I/usr/include/g++-2 -I/usr/local/include
INCS :=


#****************************************************************************
# Makefile code common to all platforms
#****************************************************************************

CFLAGS   := $(CFLAGS)   $(DEFS)
CXXFLAGS := $(CXXFLAGS) $(DEFS)

#****************************************************************************
# Targets of the build
#****************************************************************************

APPS := xmltest ptest

XAPPS := $(patsubst %,$(EXEDIR)/%,$(APPS))

LIBNAME=libtinyxml
SONAME=$(LIBNAME).so.1
LIBFILE=$(SONAME).0.1

EXTRA_LIBS=-L$(LIBDIR) -ltinyxml

all: lib apps


#****************************************************************************
# Source files
#****************************************************************************

LSRCS := tinyxml.cpp tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp
LOBJS := $(patsubst %,$(OBJDIR)/%,$(LSRCS:.cpp=.o))

SRCS  := xmltest.cpp ptest.cpp

OBJS := $(patsubst %,$(OBJDIR)/%,$(SRCS:.cpp=.o))



#****************************************************************************
# Output
#****************************************************************************


$(XAPPS): $(OBJS) $(LIBDIR)/$(LIBFILE)
	$(CXX) -o $@ $(CSSFLAGS) $(OBJDIR)/$(notdir $@.o) $(LIBS) $(EXTRA_LIBS)



apps: $(XAPPS)


$(LIBDIR)/$(LIBFILE) : $(LOBJS)
	$(CXX) -shared -Wl,-soname,$(SONAME) -fPIC -o $@ $^ \
	-lstdc++
	-cd $(LIBDIR); \
		$(STRIP) $(LIBFILE); \
		ln -s $(LIBFILE) $(SONAME); \
		ln -s $(LIBFILE) $(LIBNAME).so


lib: $(LIBDIR)/$(LIBFILE)

zynq: all
	cp -a a9/lib/* ../../lib/

#****************************************************************************
# common rules
#****************************************************************************


clean2:
	-rm -Rf $(OUTDIR)

clean: 
	make CROSS=NO clean2
	make CROSS=YES clean2
	-rm -f core $(OBJS) $(OUTPUT) $(LOBJS) $(LIBFILE) $(SONAME)

depend:
	#makedepend $(INCS) $(SRCS)


.PHONY: clean lib apps

release: all
	cp -a *.h ../include
ifeq (YES, $(CROSS))
	cp -a $(LIBDIR)/$(LIBNAME)* ../lib
	cp -a $(LIBDIR)/$(LIBNAME)* ../../rootfs/extra/local/lib
else
	cp -a $(LIBDIR)/$(LIBNAME)* ../lib/$(LIBDIR)
endif


include ../d-bot.mak
