
#include "local.h"

#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "popt.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

extern int acq200_debug;

#include <errno.h>

#include "DvmReader.h"
#include "VoltageSource.h"	// ugly cross-coupling

#define VSANITY 0.1	/* max diff reading 1, reading 2 */

static void act_prompt(double* voltage, const char* txt)
{	
	printf("Enter %s [%.4fV]", txt, *voltage); fflush(stdout);

	char line[80];
	int matches = 0;

	fgets(line, 80, stdin) && (matches = sscanf(line, "%lf", voltage));
	dbg(2,"The line was %s matches %d", line, matches);
	printf("Capture %s %.4fV\n", txt, *voltage);
}


class HumanDvmReader : public DvmReader {
	public:
		HumanDvmReader(double *voltage, const char* txt) {
			dbg(2, "01 %p", this);			
			act_prompt(voltage, txt);		
		}		
		virtual ~HumanDvmReader() {
			dbg(2, "01 %p", this);
		}		
};

class OpenLoopDvm : public DvmReader {
public:
	OpenLoopDvm(double *voltage, const char* txt) {
		// assume VS is perfect!
		*voltage = VoltageSource::getSetpoint();
	}
};


#define READ_DVM "/usr/local/bin/read_dvm"

class AutoDvmReader : public DvmReader {
	double *client_volts;
	const char* client_txt;
	double v1, v2;
	
	int getVolts(double& v) {
		FILE *fp = popen(READ_DVM, "r");
		char buf[80];
		double vv;
		char* rc;
		
		dbg(2, "01 fp=%p", fp);
		if (fp == 0){
			snprintf(buf, 80, "failed to run %s", READ_DVM);
			perror(buf);
			exit(errno);				
		}	
				
		rc = fgets(buf, 80, fp);
		
		if (rc == 0){
			snprintf(buf, 80, "fgets() failed");
			perror(buf);
			exit(errno);	
		}
		
		fclose(fp);
		
		dbg(3, "Seeking %s read %s\n", client_txt, buf);
		
		if (sscanf(buf, "%lf", &vv) == 1){
			v = vv;
			dbg(3, "99 converted to %.4f", vv);
			return 1;	
		}else{
			dbg(3, "98");
			return 0;	
		}
	}

	void sanityCheck() {
		double delta = fabs(v1 - v2);
		if (delta > VSANITY){
			err("Input before/after voltages not within limits "
			    "v1:%.2fV v2 %.2fV delta:%.2fv LIMIT %.2fV\n",
			    v1, v2, delta, VSANITY);
			exit(-1);
		}
	}
	public:
		AutoDvmReader(double *voltage, const char* txt) :
			client_volts(voltage), client_txt(txt)
		{			
			if (!getVolts(v1)){
				err("failed to read volts, baling out");
				exit(-1);
			}			
		}
		virtual ~AutoDvmReader() {

			if (!getVolts(v2)){
				err("failed to read volts, baling out");
				exit(-1);	
			}
			*client_volts = (v1 + v2)/2;

			sanityCheck();
			dbg(2, "01 setting client to mean %.4f V", 
							*client_volts);
		}			
};


DvmReader* DvmReader::create(double *voltage, const char* txt) 
{
	if (getenv("OPEN_LOOP")){
		return new OpenLoopDvm(voltage, txt);
	}else if (getenv("AUTO_DVM")){
		return new AutoDvmReader(voltage, txt);				
	}else{
		return new HumanDvmReader(voltage, txt);	
	}	
}

DvmReader::~DvmReader() {
}
