/* ------------------------------------------------------------------------- */
/* InputBlockWrapper.cpp  D-TACQ ACQ400 FMC  DRIVER                                   
 * Project: MDSSHELL
 * Created: 11 Jul 2016  			/ User: pgm
 * ------------------------------------------------------------------------- *
 *   Copyright (C) 2016 Peter Milne, D-TACQ Solutions Ltd         *
 *                      <peter dot milne at D hyphen TACQ dot com>           *
 *                                                                           *
 *  This program is free software; you can redistribute it and/or modify     *
 *  it under the terms of Version 2 of the GNU General Public License        *
 *  as published by the Free Software Foundation;                            *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program; if not, write to the Free Software              *
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                *
 *
 * TODO 
 * TODO
\* ------------------------------------------------------------------------- */

#include "local.h"
#include "acq-util.h"

#include <vector>
#include <string>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "InputBlockWrapper.h"


#define strtk_no_tr1_or_boost
#include "strtk/strtk.hpp"


class SingleInputBlockWrapper : public InputBlockWrapper {
	InputBlock& inputBlock;
	static const char* configFileName(int site) {
		char* _config_file = new char[80];
		sprintf(_config_file, "/dev/sites/%d/caldef.xml", site);
		return _config_file;
	}
public:
	static int debug;
	SingleInputBlockWrapper(const char* config_file, int site = -1) :
		inputBlock(InputBlock::getInstance(config_file, site))
	{
		if (site != -1){
			inputBlock.setSite(site);
		}
	}

	SingleInputBlockWrapper(int site) :
		inputBlock(InputBlock::getInstance(configFileName(site), site))
	{
		if (debug){
			fprintf(stderr,
				"SingleInputBlockWrapper %d \"%s\"\n",
				site, configFileName(site));
		}
		inputBlock.setSite(site);
	}
	virtual Range getRange(int channel) {
		return inputBlock.getRange(channel);
	}
	virtual int getNumChannels(void) {
		return inputBlock.getNumChannels();
	}
	virtual int getSite(int lchan) {
		return inputBlock.getSite();
	}
	virtual int pchan(int lchan) {
		if (debug) fprintf(stderr, "SingleInputBlockWrapper::pchan(%d)\n", lchan);
		return lchan;
	}
};

int SingleInputBlockWrapper::debug;

class AggregatorInputBlockWrapper : public InputBlockWrapper {
	std::vector<InputBlockWrapper*> inputBlocks;
	int nchan;

	void parse(char* sites) {
		char* site_args[8];
		int nsites = strsplit(sites, site_args, 8, ",");

		if (debug) fprintf(stderr, "%s nsites:%d\n", _PFN, nsites);

		for (int ii = 0; ii != nsites; ++ii){
			int site = atoi(site_args[ii]);
			if (debug){
				fprintf(stderr, "%s site:%d\n", _PFN, site);
			}
			if (site > 0){
				inputBlocks.push_back(new SingleInputBlockWrapper(site));
			}
		}

		for (unsigned ii = 0; ii < inputBlocks.size(); ++ii){
			nchan += inputBlocks[ii]->getNumChannels();
		}
		if (debug){
			fprintf(stderr, "AggregatorInputBlockWrapper nchan %d\n", nchan);
		}
	}
	int getMemberFromChannel(int channel, int* pchan = 0){
		int ic1 = 0;
		int ic2;
		assert(channel <= nchan);

		for (unsigned ii = 0; ii < inputBlocks.size(); ++ii, ic1 = ic2){

			ic2 = ic1 + inputBlocks[ii]->getNumChannels();
			if (debug){
				fprintf(stderr,
					"getMemberFromChannel() channel:%d ii:%d site:%d ic1:%d ic2:%d\n",
					channel, ii, inputBlocks[ii]->getSite(0), ic1, ic2);
			}
			if (channel <= ic2){
				if (pchan) *pchan = channel - ic1;
				return ii;
			}
		}
		return 0;
	}
public:
	static int debug;

	AggregatorInputBlockWrapper(): nchan(0)
	{
		FILE *pp = popen("sh -c 'eval $(get.site 0 aggregator);echo $sites'", "r");
		char sites[80];
		sites[0] = '\0';
		fgets(sites, 80, pp);
		pclose(pp);

		// chomp()
		for (int rr = strlen(sites)-1; rr > 0; --rr){
			if (strspn(sites+rr, "\n\r ") > 0){
				sites[rr] = '\0';
			}else{
				break;
			}
		}
		if (debug){
			fprintf(stderr, "AggregatorInputBlockWrapper() sites %s\n", sites);
		}
		parse(sites);
	}
	virtual Range getRange(int channel) {
		int pchan = 0;
		int ii = getMemberFromChannel(channel, &pchan);
		return inputBlocks[ii]->getRange(pchan);
	}
	virtual int getNumChannels(void) {
		return nchan;
	}
	virtual int getSite(int lchan) {
		return inputBlocks[getMemberFromChannel(lchan)]->getSite(0);
	}
	virtual int pchan(int lchan) {
		int _pchan = 0;
		getMemberFromChannel(lchan, &_pchan);
		return _pchan;
	}
};

int AggregatorInputBlockWrapper::debug;

InputBlockWrapper& InputBlockWrapper::create(int site)
{
	if (getenv("AggregatorInputBlockWrapperDebug")){
		AggregatorInputBlockWrapper::debug = atoi(getenv("AggregatorInputBlockWrapperDebug"));
	}
	if (getenv("SingleInputBlockWrapperDebug")){
		SingleInputBlockWrapper::debug = atoi(getenv("AggregatorInputBlockWrapperDebug"));
	}
	if (site == -1){
		return * new SingleInputBlockWrapper(CONFIG_FILE);
	}else if (site == 0){
		return * new AggregatorInputBlockWrapper();
	}else{
		return * new SingleInputBlockWrapper(site);
	}
}
