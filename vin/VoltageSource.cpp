
#include "local.h"

#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "popt.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

extern int acq200_debug;

#include <errno.h>

#include "VoltageSource.h"

static void act_prompt(double voltage, const char* txt)
{	
	printf("Set Voltage %s [%.4fV]", txt, voltage); fflush(stdout);

	char line[80];

	fgets(line, 80, stdin);
}


class HumanVoltageSource : public VoltageSource {
	public:
		HumanVoltageSource(double voltage, const char* txt) :
			VoltageSource(voltage, txt)
		{
			dbg(2, "01 %p", this);			
			act_prompt(voltage, txt);		
		}		
		virtual ~HumanVoltageSource() {
			dbg(2, "01 %p", this);
		}		
};

/* command: -a 20 -- CHX V A=0 */
#define SET_AO "echo -a 20 -- %c %.2f 0 | nc %s 53505"

#define CHX 'A'
/* some cards may require Evens E or Odds O */

#define AUTO_VS_IP getenv("AUTO_VS")

class AutoVoltageSource : public VoltageSource {
	int setVolts(double v) {
		
		char buf[128];

		sprintf(buf, SET_AO, CHX, v, AUTO_VS_IP);
		FILE *fp = popen(buf, "r");
		int rc = 1;

		dbg(2, ">%s", buf);

		while(fgets(buf, sizeof(buf), fp)){
			if (strstr(buf, "OK")){
				rc = 0;				
			}else if (strstr(buf, "ERR")){
				rc = -1;
			}
			dbg(2, "<%s", buf);
		}

		switch(rc){
		case 0:
			break;
		case 1:
			err("Failed to connect to AUTO_VS %s", AUTO_VS_IP);
			break;
		case -1:
			err("NAK from %s", AUTO_VS_IP);
			break;
		}
		return rc;
	}
	public:
		AutoVoltageSource(double voltage, const char* txt) :
			VoltageSource(voltage, txt)
		{			
			if (setVolts(voltage) != 0){
				err("failed to read volts, baling out");
				exit(-1);
			}else{
				dbg(1, "sleep(1) for VS");
				sleep(1);
			}
		}
		virtual ~AutoVoltageSource() {

		}			
};

#define SET_DC "echo %.4f 0 | nc %s 53506"

class DCAutoVoltageSource : public VoltageSource {
	const char* client_txt;
	
	int setVolts(double v) {
		
		char buf[128];

		sprintf(buf, SET_DC, v, AUTO_VS_IP);
		int rc = system(buf);

		switch(rc){
		case 0:
			break;
		default:
			err("Failed to connect to AUTO_VS %s", AUTO_VS_IP);
			break;
		}
		return rc;
	}
public:

	DCAutoVoltageSource(double voltage, const char* txt) :
		VoltageSource(voltage, txt)
	{			
		if (setVolts(voltage) != 0){
			err("failed to read volts, baling out");
			exit(-1);
		}else{
			dbg(1, "sleep(1) for VS");
			sleep(1);
		}
	}
	virtual ~DCAutoVoltageSource() {

	}			
};


void VoltageSource::create(double voltage, const char* txt) 
{
	if (getenv("AUTO_VS_FUNGENOK") && getenv("AUTO_VS")){
		AutoVoltageSource(voltage, txt);			
	}else if (getenv("AUTO_VS")){
		DCAutoVoltageSource(voltage, txt);	
	}else{
		HumanVoltageSource(voltage, txt);	
	}	
}

VoltageSource::~VoltageSource() {

}

VoltageSource::VoltageSource(double voltage, const char* txt) {
		setpoint = voltage;
		client_txt = txt;
		dbg(1, "setting voltage %.2f V", voltage);
}
double VoltageSource::setpoint;
