/* ------------------------------------------------------------------------- */
/* set_nacc.cpp - set.set_nacc, get.set_nacc multi channel implementation    */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2003 Peter Milne, D-TACQ Solutions Ltd
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
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                */
/* ------------------------------------------------------------------------- */


extern "C" {
#include "local.h"
int acq200_debug = 0;
};
#include "popt.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>               // getenv()

#include "tinyxml.h"

#include "InputBlock.h"
#include "RawLimits.h"

#define REVID "set_nacc $Revision: 1.6 $ B1005"



#define CONFIG_FILE "/etc/cal/caldef.xml"


const char *config_file = 
	getenv("SET_NACC_CONFIG")? getenv("SET_NACC_CONFIG"): CONFIG_FILE;






static int do_setNacc(InputBlock& inputBlock, int value)
{
	int rc = RawLimits::writeKnob("/dev/dtacq/nacc", value);

	if (rc == 0){
		int nacc = RawLimits::readKnob("/dev/dtacq/nacc", 1);
		RawLimits rawLimits;
		
		TiXmlHandle calHandle = inputBlock.getCalibrationHandle();
		TiXmlElement* data =  calHandle.FirstChild("Data").Element();

		assert(data);

		
		data->SetAttribute("code_min", rawLimits.code_min);
		data->SetAttribute("code_max", rawLimits.code_max);
		data->SetAttribute("nacc", nacc);

		inputBlock.saveFile(config_file);	
	}
	return rc;
}
static int setNacc(InputBlock& inputBlock, const char** args, int nargs)
{
	switch(nargs){
	case 1:
		return do_setNacc(inputBlock, atoi(args[0]));
	default:
		fprintf(stderr, "ERROR: set.nacc NACC\n");
		return -1;
	}
}


static int getNacc(void)
{
	int nacc = RawLimits::readKnob("/dev/dtacq/nacc", 1);

	printf("%d\n", nacc);
	return 0;
}

static int getState(void)
{
	return RawLimits::readKnob("/dev/dtacq/state", 1);
}


int main(int argc, const char *argv[])
{
	const char *verb = basename((char *)argv[0]);
        struct poptOption opt_table[] = {
		{ "debug",      'd', POPT_ARG_INT,    &acq200_debug, 0 },
		{ "config",     'c', POPT_ARG_STRING, &config_file,  0 },
		{ "version",    'v', POPT_ARG_NONE, 0, 'v' },
		POPT_AUTOHELP
		{}
	};
	poptContext context = poptGetContext(
                argv[0], argc, argv, opt_table, 0 );

	int rc;

	while ((rc = poptGetNextOpt(context)) > 0){
		switch(rc){
		case 'v':
			printf("%s\n", REVID);
			return 0;
		default:
			;
		}
	}

	const char** args = poptGetArgs(context);
	int nargs;

	for (nargs = 0; args && args[nargs]; nargs++){
		;
	}
	
	InputBlock& inputBlock = InputBlock::getInstance(config_file);

	dbg(1,"inputBlock().getNumChannels() %d", inputBlock.getNumChannels());

	if (strcmp(verb, "get_nacc") == 0){
		return getNacc();
	}else{
		if (getState() != 0){
			fprintf(stderr, "ERROR: state not ST_STOP\n");
			return 1;
		}else{
			return setNacc(inputBlock, args, nargs);
		}
	}
}


