SHELL=/bin/sh
CC=gcc
LD=ld
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic
LDFLAGS=
LIBS=-L$(top_srcdir)/lib/  -lTreeShr -lMdsShr -lpthread -lc  -lm
SYBASE_INC=

INSTALL=../conf/install-sh -c

SOURCES = \
CvtConvertFloat.c \
DummyRoutines.c \
IsRoprand.c \
TdiAbs.c \
TdiAdd.c \
TdiAnd.c \
TdiArray.c \
TdiBinary.c \
TdiBound.c \
TdiBuild.c \
TdiCall.c \
TdiChar.c \
TdiCompileDependency.c \
TdiCompile.c \
TdiConstant.c \
TdiConvert.c \
TdiCull.c \
TdiCvtArgs.c \
TdiCvtDxDx.c \
TdiDecompileDependency.c \
TdiDecompileR.c \
TdiDecompile.c \
TdiDecompress.c \
TdiDefCat.c \
TdiDefFunction.c \
TdiDim.c \
TdiDivide.c \
TdiDoTask.c \
TdiDtypeRange.c \
TdiEq.c \
TdiEvaluate.c \
TdiExponent.c \
TdiExpt.c \
TdiExtFunction.c \
TdiFaultHandler.c \
TdiGetArgs.c \
TdiGetData.c \
TdiGetDbi.c \
TdiGetNci.c \
TdiGetShape.c \
TdiGetSlope.c \
TdiHash.c \
TdiIand.c \
TdiIntrinsic.c \
TdiIo.c \
TdiItoX.c \
TdiLex.c \
TdiMakeFunctionTable.c \
TdiMasterData.c \
TdiMath1.c \
TdiMath2.c \
TdiMatrix.c \
TdiMaxVal.c \
TdiMinMax.c \
TdiOpcDef.c \
TdiPack.c \
TdiPower.c \
TdiSame.c \
TdiScalar.c \
TdiSetRange.c \
TdiShowVm.c \
TdiSort.c \
TdiSql.c \
TdiSquare.c \
TdiStatement.c \
TdiSubscript.c \
TdiThreadSafe.c \
TdiTrans.c \
TdiTrim.c \
TdiUnary.c \
TdiUndef.c \
TdiVar.c \
TdiVector.c \
TdiXxxOf.c \
TdiXxx.c \
TdiYaccSubs.c \
TdiYacc.c

OBJECTS = $(SOURCES:.c=.o)
EXPORTS = TdiShr.exports

all: $(top_srcdir)/lib/ $(top_srcdir)/lib/ $(top_srcdir)/lib/libTdiShr.so $(top_srcdir)/lib/libTdiShr.a

depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES)
	@mv Makefile Makefile.tmp
	@grep -v '\.y' Makefile.tmp > Makefile   
	@rm Makefile.tmp

clean:
	@ $(RM) $(OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libTdiShr.so
	@ $(RM) $(top_srcdir)/lib/libTdiShr.a

install:
	if [ ! -d ${exec_prefix}/lib ] ; then \
                mkdir ${exec_prefix}/lib; \
                chmod 755 ${exec_prefix}/lib; \
        fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libTdiShr.so ${exec_prefix}/lib
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMdsShr.so ${exec_prefix}/lib
	if test ".so" != ".a" ; then \
		$(INSTALL) -m 644  $(top_srcdir)/lib/libTdiShr.a ${exec_prefix}/lib; \
	fi;

$(top_srcdir)/lib/libTdiShr.so : $(OBJECTS) 
	$(LD) -o $@ -shared  $(OBJECTS) $(LIBS)

$(top_srcdir)/lib/libTdiShr.a : $(OBJECTS)
	$(AR) -cr $@ $(OBJECTS)
	ranlib $@

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/

TdiShr.exports: TdiShr.def
	sed -e /LIBRARY/\;/\;/d TdiShr.def > $@

TdiSql.o : TdiSql.c
	$(CC) -o $@ TdiSql.c $(CFLAGS) -c $(SYBASE_INC)
