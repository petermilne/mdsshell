/* ------------------------------------------------------------------------- */
/* input_offsets.cpp - generic caldef xml file parser                           */
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
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                */
/* ------------------------------------------------------------------------- */


#include "local.h"

#include <assert.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "popt.h"
#include "tinyxml.h"

#define VERID "$RCSfile: input_offsets.cpp,v $ $Revision: 1.2 $ B1000\n"
#define CONFIG_FILE "/etc/cal/caldef.xml"

#include "libgen.h"

static int usage(void)
{
	printf("%s\n", VERID);
	printf(
"usage:\n"
"XML file parser\n"
"load.input_offsets\n"
"store.input_offsets frequency\n");
	return 1;
}

int G_verbose;
char* config_file = "/etc/cal/caldef.xml";
char* output_dir = "/dev/adc.offsets";
char* az_file = "/ffs/cal/autozero.0";


#define MAXCHAN 96
#define MAXLINE 81
#define MAXSTAT 16

struct ChanInfo {
        int ichan;
        short offset1;
        short offset2;
        float mean;
	char status[MAXSTAT];
};

struct ChanInfo info[MAXCHAN+1];
int iinfo;


static void writeDac(int ch, int value)
{
	char device[80];

	sprintf(device, "%s/%02d", output_dir, ch);
	FILE *fp = fopen(device, "w");
	if (fp == 0){
		err("failed to open %s", device);
		exit(errno);
	}

	fprintf(fp, "%d\n", value);
	fclose(fp);
}

static void commitDac()
{
	char device[80];
	sprintf(device, "%s/commit", output_dir);
	FILE *fp = fopen(device, "w");
	if (fp == 0){
		err("failed to open %s", device);
		exit(errno);
	}
	fprintf(fp, "1\n");
	fclose(fp);
}
static void loadAzData(void)
{
	FILE* fp = fopen(az_file, "r");
	if (!fp){
		err("failed to open autozero setting file \"%s\"", az_file);
		exit(errno);
	}
	char my_line[MAXLINE];

	while(fgets(my_line, MAXLINE, fp)){
		struct ChanInfo *ci = &info[iinfo];
		if (sscanf(my_line, "%d %d %d %f %s",
			   &ci->ichan, &ci->offset1, &ci->offset2, 
			   &ci->mean, ci->status) == 5){
			++iinfo;

			if (iinfo == MAXCHAN){
				break;
			}
		}
	}
	fclose(fp);

	dbg(1, "OK");
}

/** offsets structure *

<ACQ>
	<AcqZero>
		<Offsets sr="sample-rate">
		....

*/
TiXmlElement* getZeroRoot(TiXmlElement* root)
{
	if (root->FirstChildElement("AcqZero")){
		return root->FirstChildElement("AcqZero");	
	}else{
		TiXmlElement offsets("AcqZero");
		return root->InsertEndChild(offsets)->ToElement();
	}
}

TiXmlElement *getOffsets(TiXmlElement* parent, int sample_rate_hz)
/**< locate Offsets node for sample_rate_hz, inserting if necessary. */
{
	TiXmlNode *offsets = 0;
	while((offsets = parent->IterateChildren(offsets)) != 0){
		int current_rate;
	        if (strcmp(offsets->Value(), "Offsets") == 0 &&
		    offsets->ToElement()->Attribute("sr", &current_rate)){
			if (current_rate == sample_rate_hz){
				return offsets->ToElement();
			}
		}
	}

	TiXmlElement new_offsets("Offsets");
	new_offsets.SetAttribute("sr", sample_rate_hz);
	return parent->InsertEndChild(new_offsets)->ToElement();
}


static int loadOffsetGroup(TiXmlElement* offsets)
{
	TiXmlNode *offset = 0;
	while((offset = offsets->IterateChildren(offset)) != 0){
		int channel;
		int dac;
		offset->ToElement()->Attribute("ch", &channel);
		offset->ToElement()->Attribute("dac", &dac);
		dbg(1, "%02d %d", channel, dac);

		writeDac(channel, dac);
	}

	commitDac();
	return 0;
}

static void fill(TiXmlNode* offset, struct ChanInfo *info)
{
	offset->ToElement()->Attribute("ch", &info->ichan);
	int o1;
	offset->ToElement()->Attribute("dac", &o1);
	info->offset1 = o1;
}

/*
 * the trad formula
 * (y - Y1)/(x - X1) = (Y2 - Y1)/(X2 - X1)
 *    
 * y = Y1 + (x - X1) * (Y2 - Y1)/(X2 - X1)
 */
static int interpolate(int X1, int Y1, int X2, int Y2, int x)
{
	return Y1 + (x - X1) * (Y2 - Y1) / (X2 - X1);
}

static int loadInterpolatedOffsetGroups(TiXmlElement* lr[2], int sample_rate)
{
	TiXmlNode* offset[2] = { 0, 0 };
	struct ChanInfo lr_info[2];
	int lr_rate[2];

	lr[0]->ToElement()->Attribute("sr", &lr_rate[0]);
	lr[1]->ToElement()->Attribute("sr", &lr_rate[1]);

	while ((offset[0] = lr[0]->IterateChildren(offset[0])) != 0 &&
	       (offset[1] = lr[1]->IterateChildren(offset[1])) != 0  ){
	
		fill(offset[0], &lr_info[0]);
		fill(offset[1], &lr_info[1]);

		if (lr_info[0].ichan != lr_info[1].ichan){
			err("channel mismatch %d != %d, dropping out",
			    lr_info[0].ichan, lr_info[1].ichan);
			exit(1);
		}
		

		int dac = interpolate(
				lr_rate[0], lr_info[0].offset1,
				lr_rate[1], lr_info[1].offset1, 
				sample_rate);

		dbg(1, "ch=%02d X1 %d Y1 %d X2 %d Y2 %d x %d y %d",
			lr_info[0].ichan, lr_rate[0], lr_info[0].offset1,
			lr_rate[1], lr_info[1].offset1, 
			sample_rate, dac);

		writeDac(lr_info[0].ichan, dac);
	}

	commitDac();
	return 0;
}


enum {
	O_EXACT = 0,
	O_LR_VALID = 2,
	O_LEFT_ONLY = 1,
	O_RIGHT_ONLY = 3,
	O_ERROR = -1
};

int straddleOffsets(
	TiXmlElement* parent, int sample_rate_hz, TiXmlElement* lr[2])
/** output the left, right Offsets nodes for sample_rate_hz.
 *  returns 2: lr valid, 1:exact, 0 : left only, -1: right only, else fail
 */
{
	int left = 0;
	int right = 0;
	TiXmlNode *offsets = 0;
	while((offsets = parent->IterateChildren(offsets)) != 0){
		int current_rate;
	        if (strcmp(offsets->Value(), "Offsets") == 0 &&
		    offsets->ToElement()->Attribute("sr", &current_rate)){
			if (current_rate == sample_rate_hz){
				lr[0] = offsets->ToElement();
				return O_EXACT;
			}else if (current_rate < sample_rate_hz){
				if (left == 0 || current_rate > left){
					lr[0] = offsets->ToElement();
					left = current_rate;
				}
			}else if (sample_rate_hz < current_rate){
				if (right == 0 || current_rate < right){
					lr[1] = offsets->ToElement();
					right = current_rate;
				}
			}
		}
	}

	if (left != 0 && right != 0){
		return O_LR_VALID;
	}else if (left != 0){
		return O_LEFT_ONLY;
	}else if (right != 0){
		return O_RIGHT_ONLY;
	}else{
		return O_ERROR;
	}
}

int loadOffsets(TiXmlElement* root, int sample_rate_hz)
{
	TiXmlElement* lr[2];
	int rc = straddleOffsets(getZeroRoot(root), sample_rate_hz, lr);

	switch(rc){
	case O_EXACT:
		return loadOffsetGroup(lr[0]);
	case O_LR_VALID:
		return loadInterpolatedOffsetGroups(lr, sample_rate_hz);
	default:
		err("SORRY, can't handle this case at this time");
		exit(-2);
	}
}

static void appendChanInfo(TiXmlElement *offsets, struct ChanInfo *info)
{
	TiXmlElement ch("Offset");
	ch.SetAttribute("ch", info->ichan);
	ch.SetAttribute("dac", info->offset1);
	ch.SetAttribute("dac2", info->offset2);
	ch.SetDoubleAttribute("mean", info->mean);
	ch.SetAttribute("stat", info->status);
	offsets->InsertEndChild(ch);
}

int storeOffsets(TiXmlElement* root, int sample_rate_hz)
{
	loadAzData();
	TiXmlElement *offsets = getOffsets(getZeroRoot(root), sample_rate_hz);

	offsets->Clear();
	for (int ic = 0; ic < iinfo; ++ic){
		appendChanInfo(offsets, &info[ic]);
	}
	return 0;
}

static int computeSampleRateHz(void)
{
	err("WORKTODO");
	exit(-1);
}

int main(int argc, const char *argv[])
{
	const char *config_file = CONFIG_FILE;
	const char *verb = basename((char *)argv[0]);

        struct poptOption opt_table[] = {
		{ "verbose",    'v', POPT_ARG_INT,    &G_verbose,	0 },
		{ "debug",      'd', POPT_ARG_INT,    &acq200_debug,	0 },
		{ "config",     'c', POPT_ARG_STRING, &config_file,	0 },
		{ "output_dir", 'o', POPT_ARG_STRING, &output_dir,	0 },
		{ "azfile",     0,   POPT_ARG_STRING, &az_file,         0 },
		{ "help2",      '2', POPT_ARG_NONE,   0,		'2'},
		POPT_AUTOHELP
		{}
	};
	poptContext context = poptGetContext(
                argv[0], argc, argv, opt_table, 0 );

	int rc;
	while ((rc = poptGetNextOpt(context)) > 0){
		switch(rc){
		case '2':
			usage();
			exit(0);
		}
	}

	const char** args = poptGetArgs(context);
	int nargs;

	for (nargs = 0; args && args[nargs]; nargs++){
		;
	}

	if (G_verbose) printf("new TiXmlDocument(%s)\n", config_file);

	if (G_verbose > 2 )return 0;

	TiXmlDocument* doc = new TiXmlDocument(config_file);

	assert(doc);

	if (G_verbose) printf("doc %p config_file %s\n",
			      doc, config_file);

	if (!doc->LoadFile()){
		fprintf(stderr, "ERROR: file %s not valid\n", config_file);
		exit(-1);
	}
	/* handle modifed tree structure - later versions have
         * XML-conformant single top level root 
	 */
	TiXmlElement* root = doc->RootElement();
	assert(root);

	const char* ACQROOT;

	if (strcmp("ACQ", root->Value()) == 0){
		ACQROOT = "ACQ/";
	}else{
		ACQROOT = "";
		err("WARNING: deprecated caldef file please modify to include"
		    " single root \"ACQ\"\n");
	}

	int sample_rate_hz;
	const char *hz_spec;
	if ((hz_spec = poptGetArg(context)) != 0){
		sample_rate_hz = atoi(hz_spec);
	}else{
		sample_rate_hz = computeSampleRateHz();
	}

	if (strcmp(verb, "load.input_offsets") == 0){
		return loadOffsets(root, sample_rate_hz);
	}else if (strcmp(verb, "store.input_offsets") == 0){
		int rc = storeOffsets(root, sample_rate_hz);

		if (rc == 0){
			char backup[80];
			snprintf(backup, 80, "%s.bak", config_file);
			rename(config_file, backup);			
			doc->SaveFile(config_file);
		}else{
			return rc;
		}
	}else{
		return usage();
	}
}

