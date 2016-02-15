SHELL=/bin/sh
CC=gcc
LD=ld
top_srcdir=..
RM=$(top_srcdir)/devscripts/rm_if
prefix=/usr/local/mdsplus
exec_prefix=/usr/local/mdsplus
X_CFLAGS=-D_NO_DXm
CFLAGS=-g -O2   -I$(top_srcdir)/include -fpic $(X_CFLAGS)
LDFLAGS=
LIBS=-lpthread -lc  -lm
X_LIBS=
X_EXTRA_LIBS=
X_PRE_LIBS= -lSM -lICE
INSTALL=../conf/install-sh -c
MOTIF_LIBS =  -lMrm -lXm   -lXext
UIL=uil -I$(top_srcdir)/include
MATH_LIBS=-L$(top_srcdir)/lib -lMdsMath
LIBDC1394_LIBS=-Llibdc1394/.libs/ -ldc1394_control -lraw1394
LIBDC1394_INCS=-I
UIDS = \
$(top_srcdir)/uid/A12.uid \
$(top_srcdir)/uid/A14.uid \
$(top_srcdir)/uid/A3204.uid \
$(top_srcdir)/uid/B2408.uid \
$(top_srcdir)/uid/B3224.uid \
$(top_srcdir)/uid/B5910A.uid \
$(top_srcdir)/uid/CHS_A14.uid \
$(top_srcdir)/uid/DC1394.uid \
$(top_srcdir)/uid/DT101.uid \
$(top_srcdir)/uid/DT196.uid \
$(top_srcdir)/uid/DT200.uid \
$(top_srcdir)/uid/DT_ACQ16.uid \
$(top_srcdir)/uid/DT200DA.uid \
$(top_srcdir)/uid/DSP2904.uid \
$(top_srcdir)/uid/EC727.uid \
$(top_srcdir)/uid/FERA.uid \
$(top_srcdir)/uid/H908.uid \
$(top_srcdir)/uid/H911.uid \
$(top_srcdir)/uid/H912.uid \
$(top_srcdir)/uid/HM650.uid \
$(top_srcdir)/uid/HV1440.uid \
$(top_srcdir)/uid/HV1443.uid \
$(top_srcdir)/uid/HV4032.uid \
$(top_srcdir)/uid/HV4032A1.uid \
$(top_srcdir)/uid/IDL.uid \
$(top_srcdir)/uid/INCAA16.uid \
$(top_srcdir)/uid/INCAA4.uid \
$(top_srcdir)/uid/INCAA6.uid \
$(top_srcdir)/uid/INCAA_TR10.uid \
$(top_srcdir)/uid/J1819.uid \
$(top_srcdir)/uid/J221.uid \
$(top_srcdir)/uid/J412.uid \
$(top_srcdir)/uid/JOERGER_ADCP.uid \
$(top_srcdir)/uid/JOERGER_CG.uid \
$(top_srcdir)/uid/JOERGER_DAC16.uid \
$(top_srcdir)/uid/JOERGER_TR16.uid \
$(top_srcdir)/uid/JOERGER_TR612.uid \
$(top_srcdir)/uid/JOERGER_TR812.uid \
$(top_srcdir)/uid/JRG_TR1612.uid \
$(top_srcdir)/uid/L2232.uid \
$(top_srcdir)/uid/L2256.uid \
$(top_srcdir)/uid/L2415.uid \
$(top_srcdir)/uid/L3512.uid \
$(top_srcdir)/uid/L4202.uid \
$(top_srcdir)/uid/L6810.uid \
$(top_srcdir)/uid/L6810A.uid \
$(top_srcdir)/uid/L6810B.uid \
$(top_srcdir)/uid/L8100.uid \
$(top_srcdir)/uid/L8201.uid \
$(top_srcdir)/uid/L8206.uid \
$(top_srcdir)/uid/L8210.uid \
$(top_srcdir)/uid/L8210_HEADERS.uid \
$(top_srcdir)/uid/L8212.uid \
$(top_srcdir)/uid/L8212_HEADERS.uid \
$(top_srcdir)/uid/L8501.uid \
$(top_srcdir)/uid/L8590.uid \
$(top_srcdir)/uid/L8590_MEM.uid \
$(top_srcdir)/uid/L8590_SCLR.uid \
$(top_srcdir)/uid/L8818.uid \
$(top_srcdir)/uid/L8828.uid \
$(top_srcdir)/uid/MDSDCL.uid \
$(top_srcdir)/uid/MIT_CLOCK.uid \
$(top_srcdir)/uid/MIT_DCLOCK.uid \
$(top_srcdir)/uid/MIT_DECODER.uid \
$(top_srcdir)/uid/MIT_ENCODER.uid \
$(top_srcdir)/uid/MIT_GATE.uid \
$(top_srcdir)/uid/MIT_PULSE.uid \
$(top_srcdir)/uid/MIT__CLOCK.uid \
$(top_srcdir)/uid/MIT__DCLOCK.uid \
$(top_srcdir)/uid/MIT__GATE.uid \
$(top_srcdir)/uid/MPB__DECODER.uid \
$(top_srcdir)/uid/PARAGON_HIST.uid \
$(top_srcdir)/uid/PARAGON_RPT.uid \
$(top_srcdir)/uid/PREAMP.uid \
$(top_srcdir)/uid/RETICON120.uid \
$(top_srcdir)/uid/T4012.uid \
$(top_srcdir)/uid/UMCCD.uid \
$(top_srcdir)/uid/U_OF_M_SPECT.uid



SOURCES = \
dt200.c \
dt_acq16.c \
DevRoutines.c \
a12_gen.c a12.c \
a14_gen.c a14.c \
a3204_gen.c a3204.c \
b2408_gen.c b2408.c \
b3224_gen.c b3224.c \
b5910a_gen.c b5910a.c \
dsp2904_gen.c dsp2904.c \
ec727_gen.c ec727.c \
fera_gen.c fera.c \
h908_gen.c h908.c \
h911_gen.c h911.c\
h912_gen.c h912.c \
hm650_gen.c hm650.c \
hv1440_gen.c hv1440.c \
hv1443_gen.c hv1443.c \
hv4032a1_gen.c hv4032a1.c \
hv4032_gen.c hv4032.c \
idl_gen.c idl.c \
incaa16_gen.c incaa16.c \
incaa4_gen.c incaa4.c \
incaa6_gen.c incaa6.c \
j1819_gen.c j1819.c \
j221_gen.c j221.c \
j412_gen.c j412.c \
joerger_adc_gen.c joerger_adc.c \
joerger_adcp_gen.c joerger_adcp.c \
joerger_cg_gen.c joerger_cg.c \
joerger_dac16_gen.c joerger_dac16.c \
joerger_tr16_gen.c joerger_tr16.c \
joerger_tr612_gen.c joerger_tr612.c \
joerger_tr812_gen.c joerger_tr812.c \
l2232_gen.c l2232.c \
l2256_gen.c l2256.c \
l2415_gen.c l2415.c \
l3512a_gen.c l3512a.c \
l3512_gen.c l3512.c \
l4202_gen.c l4202.c \
l6810a_gen.c l6810a.c \
l6810b_gen.c l6810b.c \
l6810c_gen.c l6810c.c \
l6810_gen.c l6810.c \
l8100_gen.c l8100.c \
l8201_gen.c l8201.c \
l8206_gen.c l8206.c \
l8210_gen.c l8210.c \
l8212_04_gen.c l8212.c \
l8212_08_gen.c \
l8212_16_gen.c \
l8212_32_gen.c \
l8501_gen.c l8501.c \
l8590_gen.c l8590.c \
l8590_mem_gen.c l8590_mem.c \
l8590_sclr_gen.c \
l8818_gen.c l8818.c \
l8828_gen.c l8828.c \
mdsdcl_gen.c mdsdcl.c \
mit__clock_gen.c mit__clock.c \
mit_clock_gen.c mit_clock.c \
mit__dclock_gen.c mit__dclock.c \
mit_dclock_gen.c mit_dclock.c \
mit_decoder_gen.c mit_decoder.c \
mit_encoder_gen.c mit_encoder.c \
mit__gate_gen.c mit__gate.c \
mit_gate_gen.c \
mit_gclock_gen.c mit_gclock.c\
mit_pulse_gen.c mit_pulse-gate.c \
mpb__decoder_gen.c mpb__decoder.c \
paragon_hist_gen.c paragon_hist.c \
paragon_rpt_gen.c paragon_rpt.c \
preamp_gen.c preamp.c \
reticon120_gen.c reticon120.c \
t2812_gen.c \
t2814_gen.c \
t2824_gen.c \
t2825_gen.c \
t2860_gen.c \
t4012_gen.c t4012.c \
umccd_gen.c \
u_of_m_spect_gen.c u_of_m_spect.c


IO_SOURCES = \
io.c\
curvefit.c 

OBJECTS = $(SOURCES:.c=.o)

IO_OBJECTS = $(IO_SOURCES:.c=.o)

DC1394_SOURCES = dc1394_support.c

DC1394_OBJECTS = $(DC1394_SOURCES:.c=.o)

EXPORTS = MitDevices.exports


all :  $(top_srcdir)/lib/ $(top_srcdir)/lib/ $(top_srcdir)/bin/ \
       $(top_srcdir)/lib/libMitDevices.so \
       $(top_srcdir)/lib/libMitDevicesMsg.so \
       $(top_srcdir)/lib/libMitDevices.a \
       $(top_srcdir)/lib/libMitDevicesIO.so $(top_srcdir)/lib/libMitDevicesIO.a $(top_srcdir)/bin/daq_server $(UIDS) \
       


depend:
	@makedepend -- $(CFLAGS) -- $(SOURCES) $(IO_SOURCES) mitdevices_msg.c

clean:
	@ $(RM) $(UIDS)
	@ $(RM) $(OBJECTS)
	@ $(RM) $(IO_OBJECTS)
	@ $(RM) so_locations
	@ $(RM) $(top_srcdir)/lib/libMitDevices.a
	@ $(RM) $(top_srcdir)/lib/libMitDevices.so
	@ $(RM) $(top_srcdir)/lib/libMitDevicesIO.so
	@ $(RM)	$(top_srcdir)/lib/libMIT\$$DEVICES.so
	@ $(RM) $(top_srcdir)/lib/libMitDevicesMsg.so

install:
	for uid in $(UIDS) ; do\
	  $(INSTALL) -m 644 $${uid} /usr/local/mdsplus/uid;\
	done
	if [ ! -d ${exec_prefix}/lib ] ; then \
	        mkdir ${exec_prefix}/lib; \
	        chmod 755 ${exec_prefix}/lib; \
	fi;
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMitDevices.so ${exec_prefix}/lib
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMitDevicesIO.so ${exec_prefix}/lib
	$(INSTALL) -m 755  $(top_srcdir)/lib/libMitDevicesMsg.so ${exec_prefix}/lib
	ln -sf libMitDevices.so ${exec_prefix}/lib/libMIT\$$DEVICES.so
	if (test .so != .a) then \
	  $(INSTALL) -m 644  $(top_srcdir)/lib/libMitDevices.a ${exec_prefix}/lib; \
	fi;

a12_gen.c : a12.gen
	../bin/gen_device a12.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - a12_gentmp.c > a12_gen.c
	rm a12_gentmp.c

a14_gen.c : a14.gen
	../bin/gen_device a14.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - a14_gentmp.c > a14_gen.c
	rm a14_gentmp.c

a3204_gen.c : a3204.gen
	../bin/gen_device a3204.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - a3204_gentmp.c > a3204_gen.c
	rm a3204_gentmp.c

b2408_gen.c : b2408.gen
	../bin/gen_device b2408.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - b2408_gentmp.c > b2408_gen.c
	rm b2408_gentmp.c

b3224_gen.c : b3224.gen
	../bin/gen_device b3224.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - b3224_gentmp.c > b3224_gen.c
	rm b3224_gentmp.c

b5910a_gen.c : b5910a.gen
	../bin/gen_device b5910a.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - b5910a_gentmp.c > b5910a_gen.c
	rm b5910a_gentmp.c

dsp2904_gen.c : dsp2904.gen
	../bin/gen_device dsp2904.gen sxa _gentmp
	echo '#include <mitdevices_msg.h>' | cat - dsp2904_gentmp.c > dsp2904_gen.c
	rm dsp2904_gentmp.c

ec727_gen.c : ec727.gen
	../bin/gen_device ec727.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - ec727_gentmp.c > ec727_gen.c
	rm ec727_gentmp.c

fera_gen.c : fera.gen
	../bin/gen_device fera.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - fera_gentmp.c > fera_gen.c
	rm fera_gentmp.c

h908_gen.c : h908.gen
	../bin/gen_device h908.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - h908_gentmp.c > h908_gen.c
	rm h908_gentmp.c

h911_gen.c : h911.gen
	../bin/gen_device h911.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - h911_gentmp.c > h911_gen.c
	rm h911_gentmp.c

h912_gen.c : h912.gen
	../bin/gen_device h912.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - h912_gentmp.c > h912_gen.c
	rm h912_gentmp.c

hm650_gen.c : hm650.gen
	../bin/gen_device hm650.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - hm650_gentmp.c > hm650_gen.c
	rm hm650_gentmp.c

hv1440_gen.c : hv1440.gen
	../bin/gen_device hv1440.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - hv1440_gentmp.c > hv1440_gen.c
	rm hv1440_gentmp.c

hv1443_gen.c : hv1443.gen
	../bin/gen_device hv1443.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - hv1443_gentmp.c > hv1443_gen.c
	rm hv1443_gentmp.c

hv4032a1_gen.c : hv4032a1.gen
	../bin/gen_device hv4032a1.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - hv4032a1_gentmp.c > hv4032a1_gen.c
	rm hv4032a1_gentmp.c

hv4032_gen.c : hv4032.gen
	../bin/gen_device hv4032.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - hv4032_gentmp.c > hv4032_gen.c
	rm hv4032_gentmp.c

idl_gen.c : idl.gen
	../bin/gen_device idl.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - idl_gentmp.c > idl_gen.c
	rm idl_gentmp.c

incaa16_gen.c : incaa16.gen
	../bin/gen_device incaa16.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - incaa16_gentmp.c > incaa16_gen.c
	rm incaa16_gentmp.c

incaa4_gen.c : incaa4.gen
	../bin/gen_device incaa4.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - incaa4_gentmp.c > incaa4_gen.c
	rm incaa4_gentmp.c

incaa6_gen.c : incaa6.gen
	../bin/gen_device incaa6.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - incaa6_gentmp.c > incaa6_gen.c
	rm incaa6_gentmp.c

j1819_gen.c : j1819.gen
	../bin/gen_device j1819.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - j1819_gentmp.c > j1819_gen.c
	rm j1819_gentmp.c

j221_gen.c : j221.gen
	../bin/gen_device j221.gen sxa _gentmp
	echo '#include <mitdevices_msg.h>' | cat - j221_gentmp.c > j221_gen.c
	rm j221_gentmp.c

j412_gen.c : j412.gen
	../bin/gen_device j412.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - j412_gentmp.c > j412_gen.c
	rm j412_gentmp.c

joerger_adc_gen.c : joerger_adc.gen
	../bin/gen_device joerger_adc.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - joerger_adc_gentmp.c > joerger_adc_gen.c
	rm joerger_adc_gentmp.c

joerger_adcp_gen.c : joerger_adcp.gen
	../bin/gen_device joerger_adcp.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - joerger_adcp_gentmp.c > joerger_adcp_gen.c
	rm joerger_adcp_gentmp.c

joerger_cg_gen.c : joerger_cg.gen
	../bin/gen_device joerger_cg.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - joerger_cg_gentmp.c > joerger_cg_gen.c
	rm joerger_cg_gentmp.c

joerger_dac16_gen.c : joerger_dac16.gen
	../bin/gen_device joerger_dac16.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - joerger_dac16_gentmp.c > joerger_dac16_gen.c
	rm joerger_dac16_gentmp.c

joerger_tr16_gen.c : joerger_tr16.gen
	../bin/gen_device joerger_tr16.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - joerger_tr16_gentmp.c > joerger_tr16_gen.c
	rm joerger_tr16_gentmp.c

joerger_tr612_gen.c : joerger_tr612.gen
	../bin/gen_device joerger_tr612.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - joerger_tr612_gentmp.c > joerger_tr612_gen.c
	rm joerger_tr612_gentmp.c

joerger_tr812_gen.c : joerger_tr812.gen
	../bin/gen_device joerger_tr812.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - joerger_tr812_gentmp.c > joerger_tr812_gen.c
	rm joerger_tr812_gentmp.c

l2232_gen.c : l2232.gen
	../bin/gen_device l2232.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l2232_gentmp.c > l2232_gen.c
	rm l2232_gentmp.c

l2256_gen.c : l2256.gen
	../bin/gen_device l2256.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l2256_gentmp.c > l2256_gen.c
	rm l2256_gentmp.c

l2415_gen.c : l2415.gen
	../bin/gen_device l2415.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l2415_gentmp.c > l2415_gen.c
	rm l2415_gentmp.c

l3512a_gen.c : l3512a.gen
	../bin/gen_device l3512a.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l3512a_gentmp.c > l3512a_gen.c
	rm l3512a_gentmp.c

l3512_gen.c : l3512.gen
	../bin/gen_device l3512.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l3512_gentmp.c > l3512_gen.c
	rm l3512_gentmp.c

l4202_gen.c : l4202.gen
	../bin/gen_device l4202.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l4202_gentmp.c > l4202_gen.c
	rm l4202_gentmp.c

l6810a_gen.c : l6810a.gen
	../bin/gen_device l6810a.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l6810a_gentmp.c > l6810a_gen.c
	rm l6810a_gentmp.c

l6810b_gen.c : l6810b.gen
	../bin/gen_device l6810b.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l6810b_gentmp.c > l6810b_gen.c
	rm l6810b_gentmp.c

l6810c_gen.c : l6810c.gen
	../bin/gen_device l6810c.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l6810c_gentmp.c > l6810c_gen.c
	rm l6810c_gentmp.c

l6810_gen.c : l6810.gen
	../bin/gen_device l6810.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l6810_gentmp.c > l6810_gen.c
	rm l6810_gentmp.c

l8100_gen.c : l8100.gen
	../bin/gen_device l8100.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l8100_gentmp.c > l8100_gen.c
	rm l8100_gentmp.c

l8201_gen.c : l8201.gen
	../bin/gen_device l8201.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l8201_gentmp.c > l8201_gen.c
	rm l8201_gentmp.c

l8206_gen.c : l8206.gen
	../bin/gen_device l8206.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l8206_gentmp.c > l8206_gen.c
	rm l8206_gentmp.c

l8210_gen.c : l8210.gen
	../bin/gen_device l8210.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l8210_gentmp.c > l8210_gen.c
	rm l8210_gentmp.c

l8212_04_gen.c : l8212_04.gen
	../bin/gen_device l8212_04.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l8212_04_gentmp.c > l8212_04_gen.c
	rm l8212_04_gentmp.c

l8212_08_gen.c : l8212_08.gen
	../bin/gen_device l8212_08.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l8212_08_gentmp.c > l8212_08_gen.c
	rm l8212_08_gentmp.c

l8212_16_gen.c : l8212_16.gen
	../bin/gen_device l8212_16.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l8212_16_gentmp.c > l8212_16_gen.c
	rm l8212_16_gentmp.c

l8212_32_gen.c : l8212_32.gen
	../bin/gen_device l8212_32.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l8212_32_gentmp.c > l8212_32_gen.c
	rm l8212_32_gentmp.c

l8501_gen.c : l8501.gen
	../bin/gen_device l8501.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l8501_gentmp.c > l8501_gen.c
	rm l8501_gentmp.c

l8590_gen.c : l8590.gen
	../bin/gen_device l8590.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l8590_gentmp.c > l8590_gen.c
	rm l8590_gentmp.c

l8590_mem_gen.c : l8590_mem.gen l8590_sclr_gen.c
	../bin/gen_device l8590_mem.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l8590_mem_gentmp.c > l8590_mem_gen.c
	rm l8590_mem_gentmp.c

l8590_sclr_gen.c : l8590_sclr.gen
	../bin/gen_device l8590_sclr.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l8590_sclr_gentmp.c > l8590_sclr_gen.c
	rm l8590_sclr_gentmp.c

l8818_gen.c : l8818.gen
	../bin/gen_device l8818.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l8818_gentmp.c > l8818_gen.c
	rm l8818_gentmp.c

l8828_gen.c : l8828.gen
	../bin/gen_device l8828.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - l8828_gentmp.c > l8828_gen.c
	rm l8828_gentmp.c

mdsdcl_gen.c : mdsdcl.gen
	../bin/gen_device mdsdcl.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - mdsdcl_gentmp.c > mdsdcl_gen.c
	rm mdsdcl_gentmp.c

mit__clock_gen.c : mit__clock.gen
	../bin/gen_device mit__clock.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - mit__clock_gentmp.c > mit__clock_gen.c
	rm mit__clock_gentmp.c

mit_clock_gen.c : mit_clock.gen
	../bin/gen_device mit_clock.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - mit_clock_gentmp.c > mit_clock_gen.c
	rm mit_clock_gentmp.c

mit__dclock_gen.c : mit__dclock.gen
	../bin/gen_device mit__dclock.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - mit__dclock_gentmp.c > mit__dclock_gen.c
	rm mit__dclock_gentmp.c

mit_dclock_gen.c : mit_dclock.gen
	../bin/gen_device mit_dclock.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - mit_dclock_gentmp.c > mit_dclock_gen.c
	rm mit_dclock_gentmp.c

mit_decoder_gen.c : mit_decoder.gen
	../bin/gen_device mit_decoder.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - mit_decoder_gentmp.c > mit_decoder_gen.c
	rm mit_decoder_gentmp.c

mit_encoder_gen.c : mit_encoder.gen
	../bin/gen_device mit_encoder.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - mit_encoder_gentmp.c > mit_encoder_gen.c
	rm mit_encoder_gentmp.c

mit__gate_gen.c : mit__gate.gen
	../bin/gen_device mit__gate.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - mit__gate_gentmp.c > mit__gate_gen.c
	rm mit__gate_gentmp.c

mit_gate_gen.c : mit_gate.gen
	../bin/gen_device mit_gate.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - mit_gate_gentmp.c > mit_gate_gen.c
	rm mit_gate_gentmp.c

mit_gclock_gen.c : mit_gclock.gen
	../bin/gen_device mit_gclock.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - mit_gclock_gentmp.c > mit_gclock_gen.c
	rm mit_gclock_gentmp.c

mit_pulse_gen.c : mit_pulse.gen
	../bin/gen_device mit_pulse.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - mit_pulse_gentmp.c > mit_pulse_gen.c
	rm mit_pulse_gentmp.c

mpb__decoder_gen.c : mpb__decoder.gen
	../bin/gen_device mpb__decoder.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - mpb__decoder_gentmp.c > mpb__decoder_gen.c
	rm mpb__decoder_gentmp.c

paragon_hist_gen.c : paragon_hist.gen
	../bin/gen_device paragon_hist.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - paragon_hist_gentmp.c > paragon_hist_gen.c
	rm paragon_hist_gentmp.c

paragon_rpt_gen.c : paragon_rpt.gen
	../bin/gen_device paragon_rpt.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - paragon_rpt_gentmp.c > paragon_rpt_gen.c
	rm paragon_rpt_gentmp.c

preamp_gen.c : preamp.gen
	../bin/gen_device preamp.gen sxa _gentmp
	echo '#include <mitdevices_msg.h>' | cat - preamp_gentmp.c > preamp_gen.c
	rm preamp_gentmp.c

reticon120_gen.c : reticon120.gen
	../bin/gen_device reticon120.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - reticon120_gentmp.c > reticon120_gen.c
	rm reticon120_gentmp.c

t2812_gen.c : t2812.gen
	../bin/gen_device t2812.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - t2812_gentmp.c > t2812_gen.c
	rm t2812_gentmp.c

t2814_gen.c : t2814.gen
	../bin/gen_device t2814.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - t2814_gentmp.c > t2814_gen.c
	rm t2814_gentmp.c

t2824_gen.c : t2824.gen
	../bin/gen_device t2824.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - t2824_gentmp.c > t2824_gen.c
	rm t2824_gentmp.c

t2825_gen.c : t2825.gen
	../bin/gen_device t2825.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - t2825_gentmp.c > t2825_gen.c
	rm t2825_gentmp.c

t2860_gen.c : t2860.gen
	../bin/gen_device t2860.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - t2860_gentmp.c > t2860_gen.c
	rm t2860_gentmp.c

t4012_gen.c : t4012.gen
	../bin/gen_device t4012.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - t4012_gentmp.c > t4012_gen.c
	rm t4012_gentmp.c

umccd_gen.c : umccd.gen
	../bin/gen_device umccd.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - umccd_gentmp.c > umccd_gen.c
	rm umccd_gentmp.c

u_of_m_spect_gen.c : u_of_m_spect.gen
	../bin/gen_device u_of_m_spect.gen sx _gentmp
	echo '#include <mitdevices_msg.h>' | cat - u_of_m_spect_gentmp.c > u_of_m_spect_gen.c
	rm u_of_m_spect_gentmp.c

$(top_srcdir)/bin/ :
	mkdir $(top_srcdir)/bin/

$(top_srcdir)/bin/daq_server : daq_server
	cp daq_server $(top_srcdir)/bin/daq_server

$(top_srcdir)/lib/libMitDevices.so : $(OBJECTS) 
	$(LD) -o $@ -shared  $(OBJECTS)  -L$(top_srcdir)/lib -lCamShr -lXmdsShr -lGenDevShr -lMdsdcl -lTdiShr -lTreeShr -lMdsShr $(X_LIBS) $(MOTIF_LIBS) $(X_PRE_LIBS) -lXt -lX11 $(X_EXTRA_LIBS) $(LIBS) $(MATH_LIBS)
	ln -sf libMitDevices.so $(top_srcdir)/lib/libMIT\$$DEVICES.so

$(top_srcdir)/lib/libMitDevicesMsg.so : mitdevices_msg.o
	$(LD) -o $@ -shared  mitdevices_msg.o

$(top_srcdir)/lib/libMitDevices.a : $(OBJECTS)
	$(AR) -cr $@ $(OBJECTS)
	ranlib $@

$(top_srcdir)/lib/libMitDevicesIO.so : $(IO_OBJECTS)
	$(LD) -o $@ -shared  $(IO_OBJECTS) $(LIBS) $(MATH_LIBS)

$(top_srcdir)/lib/libMitDevicesIO.a : $(IO_OBJECTS)
	$(AR) -cr $@ $(IO_OBJECTS)
	ranlib $@

$(top_srcdir)/lib/ :
	mkdir $(top_srcdir)/lib/

MitDevices.exports: MitDevices.def
	sed -e /LIBRARY/\;/\;/d io.def > $@

$(top_srcdir)/uid/A12.uid : A12.uil ; $(UIL) -o $(top_srcdir)/uid/A12.uid A12.uil
$(top_srcdir)/uid/A14.uid : A14.uil ; $(UIL) -o $(top_srcdir)/uid/A14.uid A14.uil
$(top_srcdir)/uid/A3204.uid : A3204.uil ; $(UIL) -o $(top_srcdir)/uid/A3204.uid A3204.uil
$(top_srcdir)/uid/B2408.uid : B2408.uil ; $(UIL) -o $(top_srcdir)/uid/B2408.uid B2408.uil
$(top_srcdir)/uid/B3224.uid : B3224.uil ; $(UIL) -o $(top_srcdir)/uid/B3224.uid B3224.uil
$(top_srcdir)/uid/B5910A.uid : B5910A.uil ; $(UIL) -o $(top_srcdir)/uid/B5910A.uid B5910A.uil
$(top_srcdir)/uid/CHS_A14.uid : CHS_A14.uil ; $(UIL) -o $(top_srcdir)/uid/CHS_A14.uid CHS_A14.uil
$(top_srcdir)/uid/DC1394.uid : DC1394.uil ; $(UIL) -o $(top_srcdir)/uid/DC1394.uid DC1394.uil
$(top_srcdir)/uid/DT101.uid : DT101.uil ; $(UIL) -o $(top_srcdir)/uid/DT101.uid DT101.uil
$(top_srcdir)/uid/DT196.uid : DT196.uil ; $(UIL) -o $(top_srcdir)/uid/DT196.uid DT196.uil
$(top_srcdir)/uid/DT200.uid : DT200.uil ; $(UIL) -o $(top_srcdir)/uid/DT200.uid DT200.uil
$(top_srcdir)/uid/DT200DA.uid : DT200DA.uil ; $(UIL) -o $(top_srcdir)/uid/DT200DA.uid DT200DA.uil
$(top_srcdir)/uid/DT_ACQ16.uid : DT_ACQ16.uil ; $(UIL) -o $(top_srcdir)/uid/DT_ACQ16.uid DT_ACQ16.uil
$(top_srcdir)/uid/DSP2904.uid : DSP2904.uil ; $(UIL) -o $(top_srcdir)/uid/DSP2904.uid DSP2904.uil
$(top_srcdir)/uid/EC727.uid : EC727.uil ; $(UIL) -o $(top_srcdir)/uid/EC727.uid EC727.uil
$(top_srcdir)/uid/FERA.uid : FERA.uil ; $(UIL) -o $(top_srcdir)/uid/FERA.uid FERA.uil
$(top_srcdir)/uid/H908.uid : H908.uil ; $(UIL) -o $(top_srcdir)/uid/H908.uid H908.uil
$(top_srcdir)/uid/H911.uid : H911.uil ; $(UIL) -o $(top_srcdir)/uid/H911.uid H911.uil
$(top_srcdir)/uid/H912.uid : H912.uil ; $(UIL) -o $(top_srcdir)/uid/H912.uid H912.uil
$(top_srcdir)/uid/HM650.uid : HM650.uil ; $(UIL) -o $(top_srcdir)/uid/HM650.uid HM650.uil
$(top_srcdir)/uid/HV1440.uid : HV1440.uil ; $(UIL) -o $(top_srcdir)/uid/HV1440.uid HV1440.uil
$(top_srcdir)/uid/HV1443.uid : HV1443.uil ; $(UIL) -o $(top_srcdir)/uid/HV1443.uid HV1443.uil
$(top_srcdir)/uid/HV4032.uid : HV4032.uil ; $(UIL) -o $(top_srcdir)/uid/HV4032.uid HV4032.uil
$(top_srcdir)/uid/HV4032A1.uid : HV4032A1.uil ; $(UIL) -o $(top_srcdir)/uid/HV4032A1.uid HV4032A1.uil
$(top_srcdir)/uid/IDL.uid : IDL.uil ; $(UIL) -o $(top_srcdir)/uid/IDL.uid IDL.uil
$(top_srcdir)/uid/INCAA16.uid : INCAA16.uil ; $(UIL) -o $(top_srcdir)/uid/INCAA16.uid INCAA16.uil
$(top_srcdir)/uid/INCAA4.uid : INCAA4.uil ; $(UIL) -o $(top_srcdir)/uid/INCAA4.uid INCAA4.uil
$(top_srcdir)/uid/INCAA6.uid : INCAA6.uil ; $(UIL) -o $(top_srcdir)/uid/INCAA6.uid INCAA6.uil
$(top_srcdir)/uid/INCAA_TR10.uid : INCAA_TR10.uil ; $(UIL) -o $(top_srcdir)/uid/INCAA_TR10.uid INCAA_TR10.uil
$(top_srcdir)/uid/J1819.uid : J1819.uil ; $(UIL) -o $(top_srcdir)/uid/J1819.uid J1819.uil
$(top_srcdir)/uid/J221.uid : J221.uil ; $(UIL) -o $(top_srcdir)/uid/J221.uid J221.uil
$(top_srcdir)/uid/J412.uid : J412.uil ; $(UIL) -o $(top_srcdir)/uid/J412.uid J412.uil
$(top_srcdir)/uid/JOERGER_ADCP.uid : JOERGER_ADCP.uil ; $(UIL) -o $(top_srcdir)/uid/JOERGER_ADCP.uid JOERGER_ADCP.uil
$(top_srcdir)/uid/JOERGER_CG.uid : JOERGER_CG.uil ; $(UIL) -o $(top_srcdir)/uid/JOERGER_CG.uid JOERGER_CG.uil
$(top_srcdir)/uid/JOERGER_DAC16.uid : JOERGER_DAC16.uil ; $(UIL) -o $(top_srcdir)/uid/JOERGER_DAC16.uid JOERGER_DAC16.uil
$(top_srcdir)/uid/JOERGER_TR16.uid : JOERGER_TR16.uil ; $(UIL) -o $(top_srcdir)/uid/JOERGER_TR16.uid JOERGER_TR16.uil
$(top_srcdir)/uid/JOERGER_TR612.uid : JOERGER_TR612.uil ; $(UIL) -o $(top_srcdir)/uid/JOERGER_TR612.uid JOERGER_TR612.uil
$(top_srcdir)/uid/JOERGER_TR812.uid : JOERGER_TR812.uil ; $(UIL) -o $(top_srcdir)/uid/JOERGER_TR812.uid JOERGER_TR812.uil
$(top_srcdir)/uid/JRG_TR1612.uid : JRG_TR1612.uil ; $(UIL) -o $(top_srcdir)/uid/JRG_TR1612.uid JRG_TR1612.uil
$(top_srcdir)/uid/L2232.uid : L2232.uil ; $(UIL) -o $(top_srcdir)/uid/L2232.uid L2232.uil
$(top_srcdir)/uid/L2256.uid : L2256.uil ; $(UIL) -o $(top_srcdir)/uid/L2256.uid L2256.uil
$(top_srcdir)/uid/L2415.uid : L2415.uil ; $(UIL) -o $(top_srcdir)/uid/L2415.uid L2415.uil
$(top_srcdir)/uid/L3512.uid : L3512.uil ; $(UIL) -o $(top_srcdir)/uid/L3512.uid L3512.uil
$(top_srcdir)/uid/L4202.uid : L4202.uil ; $(UIL) -o $(top_srcdir)/uid/L4202.uid L4202.uil
$(top_srcdir)/uid/L6810.uid : L6810.uil ; $(UIL) -o $(top_srcdir)/uid/L6810.uid L6810.uil
$(top_srcdir)/uid/L6810A.uid : L6810A.uil ; $(UIL) -o $(top_srcdir)/uid/L6810A.uid L6810A.uil
$(top_srcdir)/uid/L6810B.uid : L6810B.uil ; $(UIL) -o $(top_srcdir)/uid/L6810B.uid L6810B.uil
$(top_srcdir)/uid/L8100.uid : L8100.uil ; $(UIL) -o $(top_srcdir)/uid/L8100.uid L8100.uil
$(top_srcdir)/uid/L8201.uid : L8201.uil ; $(UIL) -o $(top_srcdir)/uid/L8201.uid L8201.uil
$(top_srcdir)/uid/L8206.uid : L8206.uil ; $(UIL) -o $(top_srcdir)/uid/L8206.uid L8206.uil
$(top_srcdir)/uid/L8210.uid : L8210.uil ; $(UIL) -o $(top_srcdir)/uid/L8210.uid L8210.uil
$(top_srcdir)/uid/L8210_HEADERS.uid : L8210_HEADERS.uil ; $(UIL) -o $(top_srcdir)/uid/L8210_HEADERS.uid L8210_HEADERS.uil
$(top_srcdir)/uid/L8212.uid : L8212.uil ; $(UIL) -o $(top_srcdir)/uid/L8212.uid L8212.uil
$(top_srcdir)/uid/L8212_HEADERS.uid : L8212_HEADERS.uil ; $(UIL) -o $(top_srcdir)/uid/L8212_HEADERS.uid L8212_HEADERS.uil
$(top_srcdir)/uid/L8501.uid : L8501.uil ; $(UIL) -o $(top_srcdir)/uid/L8501.uid L8501.uil
$(top_srcdir)/uid/L8590.uid : L8590.uil ; $(UIL) -o $(top_srcdir)/uid/L8590.uid L8590.uil
$(top_srcdir)/uid/L8590_MEM.uid : L8590_MEM.uil ; $(UIL) -o $(top_srcdir)/uid/L8590_MEM.uid L8590_MEM.uil
$(top_srcdir)/uid/L8590_SCLR.uid : L8590_SCLR.uil ; $(UIL) -o $(top_srcdir)/uid/L8590_SCLR.uid L8590_SCLR.uil
$(top_srcdir)/uid/L8818.uid : L8818.uil ; $(UIL) -o $(top_srcdir)/uid/L8818.uid L8818.uil
$(top_srcdir)/uid/L8828.uid : L8828.uil ; $(UIL) -o $(top_srcdir)/uid/L8828.uid L8828.uil
$(top_srcdir)/uid/MDSDCL.uid : MDSDCL.uil ; $(UIL) -o $(top_srcdir)/uid/MDSDCL.uid MDSDCL.uil
$(top_srcdir)/uid/MIT_CLOCK.uid : MIT_CLOCK.uil ; $(UIL) -o $(top_srcdir)/uid/MIT_CLOCK.uid MIT_CLOCK.uil
$(top_srcdir)/uid/MIT_DCLOCK.uid : MIT_DCLOCK.uil ; $(UIL) -o $(top_srcdir)/uid/MIT_DCLOCK.uid MIT_DCLOCK.uil
$(top_srcdir)/uid/MIT_DECODER.uid : MIT_DECODER.uil ; $(UIL) -o $(top_srcdir)/uid/MIT_DECODER.uid MIT_DECODER.uil
$(top_srcdir)/uid/MIT_ENCODER.uid : MIT_ENCODER.uil ; $(UIL) -o $(top_srcdir)/uid/MIT_ENCODER.uid MIT_ENCODER.uil
$(top_srcdir)/uid/MIT_GATE.uid : MIT_GATE.uil ; $(UIL) -o $(top_srcdir)/uid/MIT_GATE.uid MIT_GATE.uil
$(top_srcdir)/uid/MIT_PULSE.uid : MIT_PULSE.uil ; $(UIL) -o $(top_srcdir)/uid/MIT_PULSE.uid MIT_PULSE.uil
$(top_srcdir)/uid/MIT__CLOCK.uid : MIT__CLOCK.uil ; $(UIL) -o $(top_srcdir)/uid/MIT__CLOCK.uid MIT__CLOCK.uil
$(top_srcdir)/uid/MIT__DCLOCK.uid : MIT__DCLOCK.uil ; $(UIL) -o $(top_srcdir)/uid/MIT__DCLOCK.uid MIT__DCLOCK.uil
$(top_srcdir)/uid/MIT__GATE.uid : MIT__GATE.uil ; $(UIL) -o $(top_srcdir)/uid/MIT__GATE.uid MIT__GATE.uil
$(top_srcdir)/uid/MPB__DECODER.uid : MPB__DECODER.uil ; $(UIL) -o $(top_srcdir)/uid/MPB__DECODER.uid MPB__DECODER.uil
$(top_srcdir)/uid/PARAGON_HIST.uid : PARAGON_HIST.uil ; $(UIL) -o $(top_srcdir)/uid/PARAGON_HIST.uid PARAGON_HIST.uil
$(top_srcdir)/uid/PARAGON_RPT.uid : PARAGON_RPT.uil ; $(UIL) -o $(top_srcdir)/uid/PARAGON_RPT.uid PARAGON_RPT.uil
$(top_srcdir)/uid/PREAMP.uid : PREAMP.uil ; $(UIL) -o $(top_srcdir)/uid/PREAMP.uid PREAMP.uil
$(top_srcdir)/uid/RETICON120.uid : RETICON120.uil ; $(UIL) -o $(top_srcdir)/uid/RETICON120.uid RETICON120.uil
$(top_srcdir)/uid/T4012.uid : T4012.uil ; $(UIL) -o $(top_srcdir)/uid/T4012.uid T4012.uil
$(top_srcdir)/uid/UMCCD.uid : UMCCD.uil ; $(UIL) -o $(top_srcdir)/uid/UMCCD.uid UMCCD.uil
$(top_srcdir)/uid/U_OF_M_SPECT.uid : U_OF_M_SPECT.uil ; $(UIL) -o $(top_srcdir)/uid/U_OF_M_SPECT.uid U_OF_M_SPECT.uil

#
# This build must be done on a glibc 2.2.2 system (RedHat 7.x) or idl will have undefined references
#
$(top_srcdir)/lib/libIdl.so : callidl.c
	cc -o $(top_srcdir)/lib/libIdl.so callidl.c -shared \
               -I/usr/local/rsi/idl/external \
               -L/usr/local/rsi/idl/bin/bin.linux.x86/ -lidl \
               -L/usr/X11R6/lib -lXm -lXp -lXpm -lXext -lXt -lSM -lICE -lX11 \
               -ldl -ltermcap -lrt -lm -lpthread
#
# Support for libdc1394 connected cameras
#
 : dc1394_support.o
	$(LD) -o $@ -shared   dc1394_support.o $(LIBDC1394_LIBS)


dc1394_support.o : dc1394_support.c
	$(CC) -c -o $@ $(CFLAGS) $< $(LIBDC1394_INCS)
