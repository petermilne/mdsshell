/** closure: configures timebase (global) and restores on exit. */

#include "local.h"
#include "stringtok.h"
#include <vector>
#include <string>
#ifdef PGMCOMOUT
extern "C" {
#include "acq200_root.h"      
};
#endif


#include "Timebase.h"

#define DROOT          "/dev/dtacq"
#define SR_START       "sample_read_start"
#define SR_STRIDE      "sample_read_stride"
#define MEASRATE       "measured_sample_rate"

/** EXTERNAL CLOCK: */

#define NSEC_PER_SEC	1000000000
#define STIME_EXT	NSEC_PER_SEC


class StartSampleStride {
	int start;
	int samples;
	int stride;

	/** absolute values in data set */
	int s0;         /** start sample */
	int s1;		/** mid sample, always zero */
	int s2;		/** end sample */


	/** same values, but possibly windowed - we export these ones */
	int ws0;
	int ws2;

	int stime;	/** sample time in nsecs */
	int _sysstart;
	int _sysstride;

	static int isDefault(const char* tok){
		return STREQ(tok, "*") || STREQ(tok, ":");
	}
	void init() {
		start = 0;
		stride = 1;
	}
public:
	StartSampleStride() {
		init();		
		update();
	}
	StartSampleStride(const char* timebase) {
		init();
		update();

		int rc = 0;
		const char* tok;


		// create from timebase 
		Tokenizer<vector<string> > args(timebase, ",");

		for (vector<string>::const_iterator i = args.list().begin();
		     i != args.list().end(); ++i){
			
			dbg(1, "element [%d] %s %s", 
			    rc, i->c_str(), args.list()[rc].c_str());

			++rc;
		}

		if (rc == 3 && !isDefault(tok = args.list()[2].c_str())){
			stride = atoi(tok);
			stride = MAX(1, stride);
		}else{
			stride = 1;
		}

		if (rc >= 1 && !isDefault(tok = args.list()[0].c_str())){
			start = MAX(0, atoi(tok));
			start = MIN(start, s2-s0);
		}else{
			start = 0;
		}
		start /= stride;
		start += s0;	/* s0 is negative */

		if (rc >= 2){
			if (isDefault(tok = args.list()[1].c_str())){
				samples = (s2 - s0)/stride;
			}else{
				int ss = atoi(tok);

				ss = MAX(1, ss);
				ss = MIN((s2 - s0)/stride, ss);
				samples = ss;
			}
		}else{
			samples = (s2 - s0)/stride;	
		}

		_sysstart = start*stride - s0;
		_sysstride = stride;

		ws0 = start;
       		ws2 = start + samples;				      

		dbg(1, "s0 %d $2 %d", s0, s2);
		dbg(1, "_sysstart %d _sysstride %d",
		    _sysstart, _sysstride);
		dbg(1, "start:%d ws0:%d samples:%d ws2:%d",
		    start, ws0, samples, ws2);
	}

	static void pipin(const char* cmd, char buf[], int maxbuf) {
		FILE* fp = popen(cmd, "r");		
		fgets(buf, maxbuf, fp);
		fclose(fp);
	}

	void getClockRate() {
		char buf[80];
#ifdef PGMCOMOUT
		float freq;
		char unit;
		int hz;

		
		pipin("acqcmd getInternalClock", buf, 80);

		if (sscanf(buf, "ACQ32:getInternalClock=%d", &hz) != 1){
			err("failed to getInternalClock");
			exit(1);
		}
		if (hz == 0){
			dbg(1, "getRoot %s/%s\n", DROOT, MEASRATE);
		
			_getRoot(DROOT, MEASRATE, buf, 80);

			dbg(1, "result: %s", buf);

			if (sscanf(buf, "%f %c", &freq, &unit) == 2){
				hz = (int)(freq * (unit=='M'? 1000000: 1000));
				if (hz > 0){
					dbg(1, "Measured: hz %d NSEC_PER_SEC %d", 
					    hz, NSEC_PER_SEC);
					stime = NSEC_PER_SEC/hz;
					dbg(1, "hz %d stime %d", hz, stime);
					return;
				}
			}

			stime = STIME_EXT;
		}else{
			stime = NSEC_PER_SEC/hz;
		}
#else
		pipin("get.site 0 SIG:CLK_S1:FREQ", buf, 80);
		float hz = 1000000;
		dbg(1, "freq:\"%s\"", buf);
		int nscan = sscanf(buf, "SIG:CLK_S1:FREQ %f", &hz);
		stime = NSEC_PER_SEC/hz;
		dbg(1, "hz %g stime %d %s", (double)hz, stime, nscan==0? "SCAN FAIL":"OK");
#endif


	}

	void update() {
		char buf[80];
#ifdef PGMCOMOUT
		// read from system;
		acq200_getRoot(DROOT, SR_START, 1, "%d", &_sysstart);
		acq200_getRoot(DROOT, SR_STRIDE, 1, "%d", &_sysstride);
#endif
		pipin("acqcmd getNumSamples", buf, 80);
		
		if (sscanf(buf, "ACQ32:getNumSamples=%d pre=%d post=%d", 
			   &samples, &s0, &s2) != 3){
			err("failed to read numsamples");
			exit(1);
		}
		s0 = -s0;

		getClockRate();
	}
	void flush() {
#ifdef PGMCOMOUT
		dbg(1, "acq200_setRoot %s/%s %d", DROOT, SR_START, _sysstart);
		dbg(1, "acq200_setRoot %s/%s %d", DROOT, SR_STRIDE,_sysstride);

		acq200_setRoot(DROOT, SR_START, "%d", _sysstart);
		acq200_setRoot(DROOT, SR_STRIDE, "%d", _sysstride);
#endif
		/** numsamples unchanged :-) */
	}
	friend class TimebaseImpl;
};

#define MAXDT_BUF 128

int Timebase::verbose;

Timebase::Timebase() {
	if (getenv("TimebaseVerbose")){
		verbose = atoi(getenv("TimebaseVerbose"));
		fprintf(stderr, "%s verbose=%d\n", _PFN, verbose);
	}
}
class TimebaseImpl: public Timebase {

protected:
	StartSampleStride old_setting;
	StartSampleStride new_setting;
	const int restore;
	static char *dt_buf;

	virtual int get_dt_multiplier() {
		if (verbose) {
			fprintf(stderr, "%s %d\n", _PFN, new_setting.stride);
		}
		return new_setting.stride;
	}
public:
	TimebaseImpl(const char* timebase, int _restore) : 
		old_setting(),
		new_setting(timebase),
		restore(_restore) 
	{
		new_setting.flush();		
	}

	~TimebaseImpl() {
		if (restore){
			old_setting.flush();
		}
	}

	virtual int getStart() { return new_setting.start; }
	virtual int getSamples() { return new_setting.samples; }
	virtual int getStride() { return new_setting.stride; }
	virtual int getStime() { return new_setting.stime; }

	virtual int getS0() { return new_setting.ws0; }
	virtual int getS2() { return new_setting.ws2; }

	virtual const char* getDt(){
		dbg(1, "dt_buf %p", dt_buf);

		if (!dt_buf){
			dt_buf = new char[MAXDT_BUF];	

			snprintf(dt_buf, MAXDT_BUF, 
				"(%d * %.3g)",
				 get_dt_multiplier(),	 
				 static_cast<double>(new_setting.stime)/NSEC_PER_SEC);
			if (verbose) fprintf(stderr, "%s %s\n", _PFN, dt_buf);

			dbg(1, "dt_buf %p value:%s", dt_buf, dt_buf);
		}
		if (verbose) fprintf(stderr, "%s %s\n", _PFN, dt_buf);
		return dt_buf;
	}
};

/* assume one size fits all .. */
/* WORKTODO: one day, Range will include ch id .. */
#define DECIMATE_KNOB	"/dev/dtacq/oversample_A_L"	

class  Acq132TimebaseImpl: public TimebaseImpl {

public:
	Acq132TimebaseImpl(const char* timebase, int _restore) :
		TimebaseImpl(timebase, _restore)
	{
		dbg(1, "DT includes decimation");
	}	

	int getDecimation(Range &range){
		FILE *fp = fopen(DECIMATE_KNOB, "r");
		char buf[80];
		int decimation = 1;
		if (!fp){
			err("failed to open %s", DECIMATE_KNOB);
			return decimation;
		}
		fgets(buf, 80, fp);
		sscanf(buf, "nacc=%d", &decimation);

		fclose(fp);
		return decimation;
	}
};

char *TimebaseImpl::dt_buf;

Timebase* Timebase::create(const char* timebase, int restore)
{
	const char* model = getenv("acq100");

	if (model && strncmp(model, "acq132", 6) == 0){
		return new Acq132TimebaseImpl(timebase, restore);
	}else{
		return new TimebaseImpl(timebase, restore);
	}
}
