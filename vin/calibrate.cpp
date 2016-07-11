/*****************************************************************************
 *
 * File: compute-gain.c
 *
 * $RCSfile: calibrate.cpp,v $
 * 
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: calibrate.cpp,v 1.41 2010/11/09 20:58:14 pgm Exp $
 * $Log: calibrate.cpp,v $
 * Revision 1.41  2010/11/09 20:58:14  pgm
 * set-zero avoids update when not needed
 *
 * Revision 1.40  2010/11/09 20:42:22  pgm
 * set-zero option eliminates zero offset on the fly
 *
 * Revision 1.39  2009/11/12 15:44:11  pgm
 * works with unipolar input
 *
 * Revision 1.38  2009/09/18 13:36:44  pgm
 * includes firmware also
 *
 * Revision 1.37  2009/09/18 12:58:55  pgm
 * Knob interface - works for temp
 *
 * Revision 1.36  2009/09/18 07:47:57  pgm
 * cope with short read bug .. when sample_read_start!=0 to avoid dead band
 *
 * Revision 1.35  2009/09/17 21:00:07  pgm
 * works with acq164
 *
 * Revision 1.34  2009/06/26 06:37:30  pgm
 * basic  does conversion
 *
 * Revision 1.33  2008/05/20 13:12:37  pgm
 * finer step on VS good for transfer-function also auto-dvm reader
 *
 * Revision 1.32  2008/05/09 10:23:39  pgm
 * DVM_ERROR environment to control DVM error threshold
 *
 * Revision 1.31  2008/03/11 18:56:42  pgm
 * DCVoltageSource becomes default
 *
 * Revision 1.30  2007/08/17 12:10:41  pgm
 * *** empty log message ***
 *
 * Revision 1.29  2007/08/01 11:39:48  pgm
 * --zerovolts V option - calibration with zero code at fixed DC offset
 *
 * Revision 1.28  2007/07/18 16:33:34  pgm
 * sleep(1) works better
 *
 * Revision 1.27  2007/03/31 14:38:02  pgm
 * clear volts to zero on exit, refactor VoltageSource class
 *
 * Revision 1.26  2007/03/31 11:33:58  pgm
 * ensure mean of 1000000 doesnt overflow
 *
 * Revision 1.25  2007/03/31 11:19:03  pgm
 * allocate raw buf _after_ nmean decided
 *
 * Revision 1.24  2007/03/31 10:18:30  pgm
 * CAL_NMEAN control by environ
 *
 * Revision 1.23  2007/03/30 14:03:53  pgm
 * acqcmd timeout
 *
 * Revision 1.22  2007/03/30 14:03:20  pgm
 * transfer-function
 *
 * Revision 1.21  2007/03/21 18:58:13  pgm
 * means index from 1, reduce new[] load
 *
 * Revision 1.20  2007/03/21 17:08:51  pgm
 * CAPCOM env opt
 *
 * Revision 1.19  2007/03/15 20:06:22  pgm
 * VoltageSource goes full auto
 *
 * Revision 1.18  2007/02/22 17:32:58  pgm
 * *** empty log message ***
 *
 * Revision 1.17  2007/02/22 15:41:44  pgm
 * *** empty log message ***
 *
 * Revision 1.16  2007/02/13 20:31:36  pgm
 * code_min, code_max raw coding opts
 *
 * Revision 1.15  2007/02/04 10:57:57  pgm
 * DvmReader support
 *
 * Revision 1.14  2006/11/01 10:41:26  pgm
 * --zero offset adjust
 *
 * Revision 1.13  2006/01/25 11:42:31  pgm
 * fmt attribute to change output precision
 *
 * Revision 1.12  2006/01/23 20:30:49  pgm
 * *** empty log message ***
 *
 * Revision 1.11  2006/01/23 20:19:17  pgm
 * kludge acqcmd --until ST_STOP
 *
 * Revision 1.10  2006/01/13 14:44:16  pgm
 * add warning when near rail
 *
 * Revision 1.9  2005/11/11 14:46:08  pgm
 * *** empty log message ***
 *
 * Revision 1.8  2005/11/03 14:28:53  pgm
 * *** empty log message ***
 *
 * Revision 1.7  2005/03/31 15:27:11  pgm
 * *** empty log message ***
 *
 * Revision 1.6  2005/03/31 11:40:07  pgm
 * vin works with wacky acq216 ADC_RANGE. M2 needs test
 *
 * Revision 1.5  2005/03/10 17:21:46  pgm
 * reverse incorrect double comp
 *
 * Revision 1.4  2005/03/08 16:04:20  pgm
 * *** empty log message ***
 *
 * Revision 1.3  2005/03/08 15:59:51  pgm
 * *** empty log message ***
 *
 * Revision 1.2  2005/03/08 15:53:33  pgm
 * *** empty log message ***
 *
 * Revision 1.1  2005/03/08 11:11:08  pgm
 * calibrate is go
 *
 * Revision 1.4  2005/01/28 16:44:37  pgm
 * *** empty log message ***
 *
 * Revision 1.3  2005/01/28 16:44:00  pgm
 * *** empty log message ***
 *
 * Revision 1.2  2005/01/28 16:32:36  pgm
 * *** empty log message ***
 *
 * Revision 1.1  2005/01/28 16:26:12  pgm
 * *** empty log message ***
 *
 *
 *
\*****************************************************************************/

/*
 * compute-gain
 * capture data
 * calculate gain factor
 */

#include "local.h"

#include <assert.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "popt.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


#include <errno.h>


#include "InputBlock.h"
#include "DvmReader.h"
#include "VoltageSource.h"

int acq200_debug;

int site;

static int acq200_zero;			/** adjust for code 0x0000 = 0V */
static double acq200_zcode_volts;	/** code 0x000 = xV */

#define VERID "$RCSfile: calibrate.cpp,v $ $Revision: 1.41 $ B1058\n"


char *calscript = "set.autozero";

#define MAXSHORT 0x7fff

static FILE* freport = stdout;

#define WORDSIZE_KNOB	"/dev/dtacq/word_size"

class DataSource {

	static int getWordSize(void);

 protected:
	const int ch;
	FILE* fp;
	const int maxval;

 public:
	DataSource(int _ch, int _maxval) : ch(_ch), maxval(_maxval) {
		char fname[80];
		sprintf(fname, "/dev/acq200/data/%02d", ch);

		fp = fopen(fname, "r");
		assert(fp);
	}
	virtual ~DataSource() {
		fclose(fp);
	}

	virtual double computeMean(int nmean, int &touches_rail) = 0;

	static DataSource *create(int _ch);
};

template <class W> class DataSourceImpl: public DataSource {
	int read(W* buffer, int nread){
		// hack for 256 sample read start to avoid proto deadband 
		if (sizeof(W) == 4){
			nread += 256;
		}
		return fread(buffer, sizeof(W), nread, fp);
	}
	static W* rawbuf;
 public:
	DataSourceImpl(int _ch, int _maxval) : DataSource(_ch, _maxval) {
	}

	virtual double computeMean(int nmean, int &touches_rail) {
		dbg(2, "01");
		if (rawbuf == 0){
			rawbuf = new W[nmean];
			dbg(2, "rawbuf %p nmean %d", rawbuf, nmean);
		}
				
		int nread = read(rawbuf, nmean);

		dbg(2, "nmean %d nread %d", nmean, nread);

		if (nread < nmean){
			if (nread < nmean/2){
				err("read %d nmean %d", nread, nmean);
				exit(-1);
			}else{
				/* it's a problem, but not end of world */
				nmean = nread;
			}
		}

		long long total = 0;
		W *praw = rawbuf;

		for (int togo = nmean; togo--; ){
			W value = *praw++;
			if (abs(value) > maxval){
				if (!touches_rail){
					err("[%02d] touches_rail %d %d", 
					    ch, value, maxval);
				}
				touches_rail = 1;	
			}  
			total += value;
		}		

		dbg(2, "99 returns %f", (double)total/nmean);
		return (double)total/nmean;
	}

};

template <class W> W* DataSourceImpl<W>::rawbuf;

int DataSource::getWordSize(void)
{
	static int word_size = 0;

	if (word_size == 0){
		FILE *fp = fopen(WORDSIZE_KNOB, "r");
		if (!fp){
			perror(WORDSIZE_KNOB);
			exit(errno);
		}
		if (fscanf(fp, "%d", &word_size) != 1){
			fprintf(stderr, "FAILED to scan %s\n", WORDSIZE_KNOB);
			exit(-1);
		}
		fclose(fp);

		dbg(1, "word_size set %d\n", word_size);
		assert(word_size == 2 || word_size == 4);
	}
	return word_size;
}

DataSource* DataSource::create(int _ch){
	if (getWordSize() == 4){
		return new DataSourceImpl<int>(_ch, INT_MAX-1000);
	}else{
		return new DataSourceImpl<short>(_ch, SHRT_MAX-10);
	}
}

class DataSet {
	ChannelSelection& selection;
	double* means;
	int nmean;
	int reversed;
	int *touches_rail;
	

protected:
	virtual double computeMean(int ch, int nmean){
		return 0;
	}	


 public:
	DataSet(ChannelSelection& _selection, int _nmean) : 
		selection(_selection), nmean(_nmean) {

		int maxcount = selection.getMaxChan()+1;
		dbg(2, "channels %d nmean %d", selection.getMaxChan(), nmean);

		touches_rail = new int[maxcount];
		memset(touches_rail, 0, maxcount*sizeof(int));
		
		// index from 1
		means = new double[maxcount];
		memset(means, 0, maxcount*sizeof(double));
	}

	virtual ~DataSet() {
		delete [] means;
		delete [] touches_rail;
	}
	void load(InputBlock& inputBlock){
		ChannelSelectionIterator it(selection);
		reversed = inputBlock.polarityIsReversed();
		
		for (int ch; (ch = it.getNext()) > 0; ){
			DataSource *source = DataSource::create(ch);
			means[ch] = 
				source->computeMean(nmean, touches_rail[ch]);
			if (reversed){
				means[ch] = -means[ch];
			}

			dbg(2, "Dataset::load() [%2d] mean %.1f %s",
			    ch, means[ch], reversed? "reversed": "");
			delete source;

		}

		dbg(2, "99");
	}

	int getNMean() { return nmean; }
	double getMean(int ch) { return means[ch]; }
	int touchesRail(int ch) {
		return touches_rail[ch];	
	}

	ChannelSelection& getSelection() {
		return selection;
	}
};



/**
static char *data_file(int ichan, char fname[])
{
	sprintf(fname, "/dev/acq200/data/%02d", ichan);
	return fname;
}
*/

#define DVM_ERROR 0.1


static double getDvmError(void)
{
	static int dvm_error_set;
	static double dvm_error;

	if (!dvm_error_set){
		if (getenv("DVM_ERROR")){
			dvm_error = atof(getenv("DVM_ERROR"));
		}else{
			dvm_error = DVM_ERROR;
		}
		dvm_error_set = 1;
	}

	return dvm_error;
}

static const char* get_result(const char* cmd, char buf[], int len)
{
	FILE *fp = popen(cmd, "r");

	fgets(buf, len, fp);
	fclose(fp);
//	chomp(buf);
	return buf;
}
static const char* hostname(void)
{
	static char hn[20];

	return get_result("hostname", hn, 20);
}

static const char* date(void)
{
	static char date[80];

	return get_result("date", date, 80);
}

static int getChannelCount(void)
{
	static char cmd[80];
	int channel_count = 0;

	get_result("acqcmd getAvailableChannels", cmd, 80);
	if (sscanf(cmd, "ACQ32:getAvailableChannels AI=%d", 
		   &channel_count) == 1){
		return channel_count;
	}

	return 0;
}

struct Globs {
	int nmean;
	double v1;
	double v2;
	int obcoding;
};

#define CAPCOM "acqcmd setArm;acqcmd --timeout 10 --until ST_STOP"

static int act_capture(void)
{
	char *capcom = CAPCOM;
	if (getenv("CAPCOM")){
		capcom = getenv("CAPCOM");
	}
	system(capcom);
	return 0;
}



static void _capture(
	InputBlock& inputblock, // ** to get nominal ranges 
	DataSet& ds,
	double* voltage, 
	const char* txt )
{
	DvmReader* r = DvmReader::create(voltage, txt);	
	act_capture();
	ds.load(inputblock);
	delete r;
}		
static void capture(
	InputBlock& inputblock, // ** to get nominal ranges 
	DataSet& ds,
	double* voltage, 
	const char* txt )
{
#define VSETFMT "99 vset: %6.3f V dvm: %6.3f V delta %6.3f V th %5.3f V"
	double vset = *voltage;

	VoltageSource::create(vset, txt);
	_capture(inputblock, ds, voltage, txt);

	double dvm = *voltage;
	double dv = dvm - vset;
	if (dv > getDvmError()){
		err(VSETFMT, vset, dvm, dv, getDvmError());
		exit(-1);
	}else{
		dbg(1, VSETFMT, vset, dvm, dv, getDvmError());
	}
}



static inline void printRange(const Range& r) {
	printf("%6.4f,%6.4f", r.rmin, r.rmax);
}


static inline double straightLine(
	double x1, double x2, double y1, double y2, double x)
{
	dbg(1, "x1:%.4f x2 %.4f y1:%.4f y2: %.4f x:%.4f",
	       x1, x2, y1, y2, x);
	return y1 + (x - x1) * (y2 - y1)/(x2 -x1);
}

#define WARNING_TH	96

static void checkEndstops(double x, Range &limit)
{
	if (x > limit.rmax * WARNING_TH/100){
		fprintf(stderr, "WARNING close to high rail\n");
	}else if (x < limit.rmin * WARNING_TH/100){
		fprintf(stderr, "WARNING close to low rail\n");
	}
}


static void compute(
	InputBlock& inputblock, 
	DataSet& ds1, double* v1,
	DataSet& ds2, double* v2)
/** compute apparent Voltage range by extrapolating linear formula.
 *  (y - y1)/(x - x1) = (y2 - y1)/(x2 - x1)
 *
 *  y = y1 + (x - x1)*(y2 - y1)/(x2 - x1)
 *
 *  In this case y is value in volts, x is raw value in bits,
 *  we want to compute the range limits y.rmax, y.rmin
 */
{
#define FMT_YY \
"[%2d] %s %6.3f V = %6.3f V + (%6.0f - %6.0f)*(%6.3f V - %6.3f V)/(%6.0f - %6.0f)"
	ChannelSelectionIterator it(ds1.getSelection());

	int noput = getenv("ACQ216_CAL_NOPUT") != 0;

	dbg(1, "01");

	for (int ch; (ch = it.getNext()) != 0; ){

		if (acq200_debug > 0){
			fprintf(stderr, "\n\n");
		}
		dbg(1, "ch:%02d ...", ch);

		double y1 = *v1;
		double y2 = *v2;
		double x1 = ds1.getMean(ch);
		double x2 = ds2.getMean(ch);
		Range x(Range::getCodeMin(), Range::getCodeMax());
		Range y;

		if (ds1.touchesRail(ch) || ds2.touchesRail(ch)){
			err("ch %02d TOUCHES RAIL: reduce input voltage", ch);
			continue;
		}
		checkEndstops(x1, x);
		checkEndstops(x2, x);


		y.rmax = y1 + (x.rmax - x1) * (y2 - y1) / (x2 - x1);
		y.rmin = y1 + (x.rmin - x1) * (y2 - y1) / (x2 - x1);

		if (acq200_debug){
			dbg(1, FMT_YY, ch, "max",
			    y.rmax, y1, x.rmax, x1, y2, y1, x2, x1);
			dbg(1, FMT_YY, ch, "min",
			    y.rmin, y1, x.rmin, x1, y2, y1, x2, x1);
			    
		}

		if (noput) continue;
		double vz = 0;

		if (acq200_zero){
			int zcode = 0x0000;
			if (acq200_zcode_volts != 0.0f){
				zcode = (int)straightLine(
					y.rmin, y.rmax, x.rmin, x.rmax,
					acq200_zcode_volts);
			}

			vz = straightLine(
				x.rmin, x.rmax,
				y.rmin, y.rmax,	zcode);

			dbg(1, "Offset at zcode 0x%04x %6.4f V %6.4f V, "
				"compensating ...",
			    zcode, acq200_zcode_volts, vz);

			y.rmin -= vz;
			y.rmax -= vz;
		}
		printf("ch:%02d Raw: %6.0f,%6.0f => "
		       "Volts %8.4f,%8.4f Vz %8.4f\n",
		       	ch, x.rmin, x.rmax, 
			y.rmin, y.rmax,
			vz);
		
		inputblock.putCalibratedRange(ch, y);
	}

	dbg(1, "99");
}

static void calibrate(
	InputBlock& inputBlock, 
	ChannelSelection& selection,
	struct Globs& GB
	)
{
	int channel_count = getChannelCount();
	if (channel_count){
		inputBlock.setChannelCount(channel_count);
	}
	DataSet ds1(selection, GB.nmean);
	DataSet ds2(selection, GB.nmean);
	Range nominal = inputBlock.getNominalRange(selection);

	GB.v1 = nominal.rmin + nominal.getSpan() * 0.9;
	GB.v2 = nominal.rmax - nominal.getSpan() * 0.9;

	dbg(3, "capture 01 set voltage %f.2V", GB.v1);
	capture(inputBlock, ds1, &GB.v1, "V1");
	dbg(3, "capture 02 set voltage %f.2V", GB.v2);
	capture(inputBlock, ds2, &GB.v2, "V2");
	dbg(3, "capture 03");

	compute(inputBlock, ds1, &GB.v1, ds2, &GB.v2);
	dbg(3, "99");
}

static void update(
	InputBlock& inputBlock,
	ChannelSelection& selection,
	const char* config_file)
{
	char backup[80];
	
	dbg(3, "01");
	snprintf(backup, 80, "%s.bak", config_file);
	rename(config_file, backup);
	
	inputBlock.saveFile(config_file);
	dbg(3, "99");
}


static void reportLine(
	int istep,
	double setv,
	double actv,
	DataSet& dataSet)
{
	fprintf(freport, "%5d,%9.5f,%9.5f", istep, setv, actv);

	ChannelSelectionIterator it(dataSet.getSelection());	
	int ch;

	while((ch = it.getNext()) != 0){
		fprintf(freport, ",%6d", (int)dataSet.getMean(ch));
	}
	fprintf(freport, "\n");
}

static int calibrateZero(
	InputBlock& inputBlock, 
	ChannelSelection& selection,
	struct Globs& GB
	)
{
	dbg(1, "01");
	int calibrated = 0;
	Range x(Range::getCodeMin(), Range::getCodeMax());
	DataSet ds(selection, GB.nmean);
	ds.load(inputBlock);
	ChannelSelectionIterator it(selection);

	for (int ch; (ch = it.getNext()) != 0; ){
		Range y = inputBlock.getRange(ch);
		double vz = straightLine(x.rmin, x.rmax,
					 y.rmin, y.rmax, ds.getMean(ch));
		dbg(3, "CH%02d mean:%.2f rmin %.2f %.2f V rmax:%.2f\n", 
		    ch, ds.getMean(ch), y.rmin, vz, y.rmax);

		if (ABS(vz) < ABS(y.rmax - y.rmin)/10){
			if (ABS(vz) > 0.0001){
				dbg(2, 
			    "CH%02d vz is %.2f subtract from rmin, rmax\n",
				    ch, vz);
				y.rmin -= vz;
				y.rmax -= vz;
				inputBlock.putCalibratedRange(ch, y);
				calibrated = 1;
			}
		}else{
			err("CH%02d Sanity check fail vz=.2fV\n", ch, vz);
		}

	}
	dbg(1, "99");
	return calibrated;
}

static void transferFunction(
	InputBlock& inputBlock, 
	ChannelSelection& selection,
	struct Globs& GB,
	int npoints)
{
	dbg(1, "01 npoints %d", npoints);

	Range nominal = inputBlock.getNominalRange(selection);
	double step = (nominal.rmax - nominal.rmin)/npoints;
	double setv;
	double actv;
	int istep;
	DataSet dataSet(selection, GB.nmean);
	char prompt[32];

	for (istep = 1, setv = nominal.rmin; istep != npoints;
	     ++istep, setv += step){
		actv = setv;
		sprintf(prompt, "%.3V", actv);

		dbg(1, "%d %9.5fV", istep, setv);

		capture(inputBlock, dataSet, &actv, prompt);
		reportLine(istep, setv, actv, dataSet);
	}
}


int main( int argc, const char* argv[] )
{
	info(VERID);
	InputBlock::setClientVersion(VERID);
	InputBlock::setClientCmd(argc, argv);

	static char *cs_def;
	static char *outfile;
	static struct Globs GB = {};
	static struct poptOption opt_table[] = {
		{ "site", 0, POPT_ARG_INT, &::site, 0 },
		{ "channels",    'c', POPT_ARG_STRING, &cs_def,      'c' },
		{ "nmean",      'm',  POPT_ARG_INT,    &GB.nmean,      0,
		  "number of samples to take into mean [256]"
		},
		{ "verbose",    'd', POPT_ARG_INT,    &acq200_debug,   0 },
		{ "obcoding",   'b', POPT_ARG_INT,     &GB.obcoding,   0,
		  "offset binary coding hack for old FPGAs "
		},
		{ "v1   ",      'V', POPT_ARG_DOUBLE, &GB.v1, 0 },
		{ "v2   ",      'V', POPT_ARG_DOUBLE, &GB.v2, 0 },
		{ "version",    'v', POPT_ARG_NONE,        0, 'v' },
		{ "output",     'o', POPT_ARG_STRING, &outfile, 'o' },
		{ "zero", 'z', POPT_ARG_NONE, 0, 'z',
		  "code 0x0000 is adjusted to be 0V" },
		{ "zerovolts", 'Z', POPT_ARG_DOUBLE, &acq200_zcode_volts, 'Z',
		  "code 0x0000 is adjusted to be X V"},
		POPT_AUTOHELP
		{ }
	};

	GB.nmean = getenv("CAL_NMEAN")? atoi(getenv("CAL_NMEAN")): 4096;	
	const char *config_file = 
		getenv("VIN_CONFIG")? getenv("VIN_CONFIG"): CONFIG_FILE;
	poptContext opt_context = 
		poptGetContext(argv[0], argc, argv, opt_table, 0 );
	int rc;


	while ((rc = poptGetNextOpt( opt_context )) > 0){
		switch(rc){
		case 'Z':
		case 'z':
			acq200_zero = 1;
			break;
		case 'o':
			if (outfile[0] != '-'){
				freport = fopen(outfile, "w");
				if (!freport){
					err("Failed to open \"%s\"", outfile);
					exit(errno);
				}
			}
			break;
		case 'v':
			return fprintf(stderr, VERID);
		}
	}

	dbg(2, "GB.nmean %d", GB.nmean);
	InputBlock& inputBlock = InputBlock::getInstance(config_file);
	inputBlock.setSite(::site);
	ChannelSelection* selection = 
		cs_def==0?
		new ChannelSelection(inputBlock.getNumChannels(), ALL):
		new ChannelSelection(inputBlock.getNumChannels(), cs_def);

	dbg(1, "Calibration for %s last cal was %s", hostname(), date());
	dbg(1, "inputBlock.getNumChannels() = %d\n", 
	    inputBlock.getNumChannels());

	const char *arg = poptGetArg(opt_context);

	if (arg){
		if (strcmp(arg, "transfer-function") == 0){
			int points = 100;
	
			arg = poptGetArg(opt_context);
			if (arg){
				points = atoi(arg);
			}
			transferFunction(inputBlock, *selection, GB, points);
		}else if (strcmp(arg, "set-zero") == 0){
			if (calibrateZero(inputBlock, *selection, GB)){
			        update(inputBlock, *selection, config_file);
				return 0;
			}else{
				return -1;
			}
		}
		goto cleanup;
	}

	calibrate(inputBlock, *selection, GB);	
	update(inputBlock, *selection, config_file);


cleanup:
	VoltageSource::create(0.0f, "zero");

	dbg(3, "99");
	return 0;
}
