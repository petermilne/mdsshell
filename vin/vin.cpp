/* ------------------------------------------------------------------------- */
/* vin.cpp - set.vin, get.vin multi channel implementation                   */
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
#include "InputBlock.h"
#include "RawLimits.h"

#define REVID "vin $Revision: 1.27 $ B1004"



#define CONFIG_FILE "/etc/cal/caldef.xml"

#if 1
#include "libgen.h"
#else
static const char* basename(const char* name) {
	const char* bn = rindex(name, '/');

	if (bn){
		return bn;
	}else{
		return name;
	}
}
#endif

static char* CSEP = ",";


int show_raw_limits = 0;

static void replace_escapes(char* csep) {
	char *p1 = csep;
	char *p2 = csep;

	for ( ; *p2; ++p2){
		if (p2[0] == '\\' && p2[1] != '\0'){
			switch(p2[1]){
			case 'n':
				*p1++ = '\n'; break;
			case 'r':
				*p1++ = '\r'; break;
			case 't':
				*p1++ = '\t'; break;
			default:
				*p1++ = p2[1]; break;
			}
			p2++;
		}else{
			*p1++ = p2[0];
		}
	}
	*p1 = '\0';
}
static void printRange(const Range& r) {
	r.print();
}

static void getVinSelection(InputBlock& inputBlock, ChannelSelection& cs)
{
	const int* selection = cs.getSelection();
	int first = 1;

	for (int ic = 1; ic <= inputBlock.getNumChannels(); ++ic){
		if (selection[ic]){
			if (!first) printf(CSEP);		
			printRange(inputBlock.getRange(ic));
			first = 0;
		}
	}
	printf("\n");
}

const Range getVinLargestRange(InputBlock& inputBlock) {
	Range max_range = inputBlock.getRange(1);

	for (int ic = 2; ic <= inputBlock.getNumChannels(); ++ic){
		Range a_range = inputBlock.getRange(ic);

		if (a_range.rmin < max_range.rmin){
			max_range.rmin = a_range.rmin;
		}
		if (a_range.rmax > max_range.rmax){
			max_range.rmax = a_range.rmax;
		}
	}
	
	return max_range;
}

static void printRangeNames(InputBlock& inputBlock, ChannelSelection& cs)
{
	const int* selection = cs.getSelection();
	int first = 1;

	for (int ic = 1; ic <= inputBlock.getNumChannels(); ++ic){
		if (selection[ic]){
			if (!first) printf(CSEP);
			printf("\"%s\"", inputBlock.getCurrentRangeName(ic));
			first = 0;
		}
	}
	printf("\n");
}

static void printNominalRanges(InputBlock& inputBlock, ChannelSelection& cs)
{
	const int* selection = cs.getSelection();
	int first = 1;

	for (int ic = 1; ic <= inputBlock.getNumChannels(); ++ic){
		if (selection[ic]){
			if (!first) printf(CSEP);
			printRange(inputBlock.getNominalRange(ic));
			first = 0;
		}
	}	
	printf("\n");
}


static void tr(char *str, char *from, char* to)
{
	char *psrc = str;
	char *pdst = str;
	char *pf, *pt;

	while(*psrc){
		for (pf = from, pt = to; *pf; ++pf){
			if (*psrc == *pf){
				if (*pt){
					*pdst++ = *pt;                /** translate */
					goto squish;
				}else{
					goto squish;                  /** squish */
				}
			}
			if (*pt) ++pt;
		}
		/* default: leave it alone in place */
		*pdst++ = *psrc;                                              
	squish:
		psrc++;
	}
	*pdst = '\0';
}


static void printConsts(InputBlock& inputBlock, const char *csel)
{
	char buf[80];
	char *running = buf;
	char* keys[10];
	char trkeys[10][32];
	const char* delimiters = ",";
	int ikey;

	ChannelSelection* cs = 
		new ChannelSelection(inputBlock.getNumChannels(), csel);
	const int* selection = cs->getSelection();

	inputBlock.listRangeKeys(buf, 80);

	for (int ik = 0; ik != 10; ++ik){
		keys[ik] = strsep (&running, delimiters);

		if (keys[ik] == NULL){
			break;
		}else{
			strcpy(trkeys[ik], keys[ik]);
			tr(keys[ik], "\" ", "");
			tr(trkeys[ik], ".\" ", "p");
		}
	}

	for (ikey = 0; keys[ikey] != NULL; ++ikey){
		for (int ic = 1; ic <= 16; ++ic){


			inputBlock.validate();

			if (selection[ic]){			
				ChannelSelection my_selection(
					inputBlock.getNumChannels(), ic);

				inputBlock.setRange(keys[ikey], my_selection);
				inputBlock.finalize();

				char symbol[80];
				char value[80];

				sprintf(symbol, 
					"VR_%s_%s_%02d", 
					inputBlock.getModel(),
					trkeys[ikey], ic);
				tr(symbol, "-", "_");
				printf("#define %-30s 0x", symbol);

				inputBlock.collateRegValues(value, 80);

				printf("%sULL\n", value);
				system("clear_state");
//				delete(handle);    @@todo
			}
		}
	}	
}



static void getVin(InputBlock& inputBlock, const char** args, int nargs)
{
	const char* action;
	ChannelSelection* selection = 
		new ChannelSelection(inputBlock.getNumChannels(), ALL);

	if (show_raw_limits){
		RawLimits::print();
	}
	switch(nargs){
	case 2:
		dbg(1, "selection [0] \"%s\" action \"%s\"",
		    args[0], args[1] );

		action = args[1];
		selection = new ChannelSelection(
			inputBlock.getNumChannels(), args[0]);

		assert(selection);
		break;
	case 1:
		if (!isdigit(args[0][0])){
			action = args[0];
			break;
		}else{
			selection = new ChannelSelection(
				inputBlock.getNumChannels(), args[0]);
			// fall thru
		}
	default:
		getVinSelection(inputBlock, *selection);
		return;
	}

	if (strcmp(action, "range-keys") == 0){
		char buf[80];
		printf("%s\n", inputBlock.listRangeKeys(buf, 80));
	}else if (strcmp(action, "ranges") == 0){
		printRangeNames(inputBlock, *selection);
	}else if (strcmp(action, "largest-range") == 0){
		printRange(getVinLargestRange(inputBlock));
		printf("\n");
	}else if (strcmp(action, "nominal-ranges") == 0){
		printNominalRanges(inputBlock, *selection);
	}else if (strcmp(action, "calibrated-ranges") == 0){
		getVinSelection(inputBlock, *selection);
	}else{
		err("command not understood \"%s\"", action);
	}
}

static void setVin(InputBlock& inputBlock, const char** args, int nargs)
{
	switch(nargs){
	case 2: {
		ChannelSelection *cs = new ChannelSelection(
			inputBlock.getNumChannels(), args[0]);

		assert(cs);

		inputBlock.setRange(
			args[1],       /* range */  *cs);

		for (int ic = 1; ic <= inputBlock.getNumChannels(); ++ic){
			if (ic != 1) printf(CSEP);
			printRange(inputBlock.getNominalRange(ic));
		}
		printf("\n");
		break;
	}
	case 1:
		inputBlock.setRange(args[0]);
		printRange(inputBlock.getNominalRange(1));
		printf("\n");
		break;
	default:
		fprintf(stderr, "ERROR: one or two args please\n");
		exit(-1);
	}
	inputBlock.finalize();
}


static void usage(void)
{
	printf("vin %s\n", REVID);
	printf(
"usage:\n"
"vin:  prints a copy of the config file\n"
"set.vin [channel-selection] range-name\n"
"get.vin [opts]\n"
"get.vin range-keys\n"
"get.vin [channel-selection] ranges\n"
"get.vin [channel-selection] largest-range\n"
"get.vin [channel-selection] calibrated-ranges\n"
"\n"
"channel-selection: 1,2,3,4   1-6 70-\n");
}

int site = 1;

int main(int argc, const char *argv[])
{
	const char *config_file = 
		getenv("VIN_CONFIG")? getenv("VIN_CONFIG"): CONFIG_FILE;
	const char *verb = basename((char *)argv[0]);

        struct poptOption opt_table[] = {
        	{ "site", 0, POPT_ARG_INT, &::site, 0 },
		{ "debug",      'd', POPT_ARG_INT,    &acq200_debug, 0 },
		{ "config",     'c', POPT_ARG_STRING, &config_file,  0 },
		{ "help2",      '2', POPT_ARG_NONE,   0,             '2'},
		{ "delim",      'D', POPT_ARG_STRING, &CSEP,         'D' },
		{ "show_raw_limits", 'r', POPT_ARG_NONE, 0, 'r' },
		POPT_AUTOHELP
		{}
	};
	poptContext context = poptGetContext(
                argv[0], argc, argv, opt_table, 0 );

	int rc;

	while ((rc = poptGetNextOpt(context)) > 0){
		switch(rc){
		case 'r':
			show_raw_limits = 1;
			break;		
		case '2':
			usage();
			exit(0);
		case 'D':
			replace_escapes(CSEP);
			break;
		}
	}

	const char** args = poptGetArgs(context);
	int nargs;

	for (nargs = 0; args && args[nargs]; nargs++){
		;
	}
	
	InputBlock& inputBlock = InputBlock::getInstance(config_file);

	dbg(1,"inputBlock().getNumChannels() %d", inputBlock.getNumChannels());

	if (strcmp(verb, "get.vin") == 0){
		getVin(inputBlock, args, nargs);
	}else if (strcmp(verb, "set.vin") == 0){
		setVin(inputBlock, args, nargs);
	}else if (nargs > 1 && strcmp(args[nargs-1], "print-consts") == 0){
		printConsts(inputBlock, nargs>1? args[0]: "1-16");
	}else{
		printf("%s\n", REVID);
		inputBlock.print();
	}
}


