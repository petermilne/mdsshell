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
	SingleInputBlockWrapper(const char* config_file, int site = -1) :
		inputBlock(InputBlock::getInstance(config_file))
	{
		if (site != -1){
			inputBlock.setSite(site);
		}
	}

	SingleInputBlockWrapper(int site) :
		inputBlock(InputBlock::getInstance(configFileName(site)))
	{
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
		return lchan;
	}
};

class AggregatorInputBlockWrapper : public InputBlockWrapper {
	std::vector<InputBlockWrapper*> inputBlocks;
	int nchan;

	void parse(std::string sites) {
		strtk::std_string::token_list_type site_list;
		strtk::split(",",sites,std::back_inserter(site_list));
		strtk::std_string::token_list_type::iterator itr = site_list.begin();
		while (site_list.end() != itr){
			int site = atoi(itr->first);
			if (site > 0){
				inputBlocks.push_back(new SingleInputBlockWrapper(site));
			}
		}
		for (unsigned ii = 0; ii < inputBlocks.size(); ++ii){
			nchan += inputBlocks[ii]->getNumChannels();
		}
	}
	int getMemberFromChannel(int channel, int* pchan = 0){
		int ic1 = 0;
		int ic2;
		assert(channel <= nchan);

		for (unsigned ii = 0; ii < inputBlocks.size(); ++ii, ic1 = ic2){
			ic2 = ic1 + inputBlocks[ii]->getNumChannels();
			if (channel <= ic2){
				if (pchan) *pchan = channel - ic1;
				return ii;
			}
		}
		return 0;
	}
public:
	AggregatorInputBlockWrapper(): nchan(0)
	{
		FILE *pp = popen("sh -c 'eval $(get.site 0 aggregator);echo $sites'", "r");
		char sites[80];
		sites[0] = '\0';
		fgets(sites, 80, pp);
		pclose(pp);
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
InputBlockWrapper& InputBlockWrapper::create(int site)
{
	if (site == -1){
		return * new SingleInputBlockWrapper(CONFIG_FILE);
	}else if (site == 0){
		return * new AggregatorInputBlockWrapper();
	}else{
		return * new SingleInputBlockWrapper(site);
	}
}
