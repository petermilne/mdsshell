/* ------------------------------------------------------------------------- */
/* readVolts.cpp - reads stored data output as volts                         */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2005 Peter Milne, D-TACQ Solutions Ltd
 *                      <Peter dot Milne at D hyphen TACQ dot com>
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of Version 2 of the GNU General Public License
 as published by the Free Software Foundation;
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, LSA.                */
/* ------------------------------------------------------------------------- */


#define BUILD "$File:$$Revision: 1.2 $ B1000"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/syslog.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <assert.h>
#include <errno.h>

#define PROCLOGNAME "readVolts"

extern "C" {
int acq200_debug = 0;
}
#include "local.h"

#include <popt.h>



#include <list>

#include "parser.h"

#include "../vin/InputBlock.h"


#include "Timebase.h"

static struct Globs {
	const char* format;
	const char* root;
	const char *timebase;
	ChannelSelection *channels;
	int norestore;
}
	GL = {
		/* .format = */ "%7.3fV ",
		/*. root   = */ "/dev/acq200/data/%02d",
		/* .timebase = */ "0,*,1",
	};


#define RAW1 -32768
#define RAW2 32767

static double raw2volts(int raw, Range range)
{
	float v1, v2;

	if (range.isReversed()){
		v1 = range.rmax;
		v2 = range.rmin;
	}else{
		v1 = range.rmin;
		v2 = range.rmax;
	}

	float volts = v1 + (raw - RAW1)*(v2 - v1)/(RAW2 - RAW1);
	return volts;
}
static int readVolts(InputBlock &inputBlock) {
	ChannelSelectionIterator it(*GL.channels);
	Timebase* timebase = Timebase::create(GL.timebase, !GL.norestore);

	dbg(1, "channels %s", GL.channels->toString());
	int nsamples = MIN(128, timebase->getSamples());
	short* data = new short[nsamples];

	for (int ch; (ch = it.getNext()) > 0; ){
		char datapath[80];
		snprintf(datapath, 80, GL.root, ch);

		FILE* fp = fopen(datapath, "r");
		
		if (!fp){
			err("failed to open %s", datapath);
			return -1;
		}

		int nsam = fread(data, 2, nsamples, fp);
		if (!nsam) {
			err("failed to read data");
			return -1;
		}
		fclose(fp);
	       
		int total = 0;
		for (int ix = 0; ix < nsam; ++ix){
			total += data[ix];
		}
		printf(GL.format, 
		       raw2volts(total/nsam, inputBlock.getRange(ch)));		       
	}
	if (!index(GL.format, '\n')){
		puts("\n");
	}
			
	delete timebase;
	return 0;
}

int main(int argc, const char* argv[])
{
	struct poptOption opt_table[] = {
		{ "format", 'F', POPT_ARG_STRING, &GL.format, 0 },
		{ "timebase", 'T', POPT_ARG_STRING, &GL.timebase, 0 },
		{ "verbose", 'v', POPT_ARG_INT, &acq200_debug, 0 },
		{ "norestore", 0, POPT_ARG_INT, &GL.norestore, 0 },
		{ "version",   0, POPT_ARG_NONE, 0, 'v' },
		POPT_AUTOHELP
		POPT_TABLEEND		
	};
	int rc;

	poptContext opt_context = 
		poptGetContext(argv[0], argc, argv, opt_table, 0);
	const char *config_file = CONFIG_FILE;

	dbg(1, "instantiate inputBlock...");

	InputBlock& inputBlock = InputBlock::getInstance(config_file);
	while ((rc = poptGetNextOpt(opt_context)) > 0){
		switch(rc){
		case 'v':
			fprintf(stderr, "readVolts " BUILD "\n");
			exit(1);
		default:
			;
		}
	}
	int argc2;
	const char ** argv2 = poptGetArgs(opt_context);

	for (argc2 = 0; argv2 && argv2[argc2] != 0; ++argc2){

	}

	
	if (argc2 == 0){
		static const char* all = "1-";
		argv2 = &all;
		argc2 = 1;
	}

	GL.channels = new ChannelSelection(
		inputBlock.getNumChannels(), argc2, argv2);

	return readVolts(inputBlock);
}
