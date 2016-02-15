/* ------------------------------------------------------------------------- */
/* cbm.cpp - channel block mask ops                                          */
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "popt.h"
#include "tinyxml.h"

#define VERID "$RCSfile: cbm.cpp,v $ $Revision: 1.5 $ B1017\n"
#define CONFIG_FILE "/etc/cal/caldef.xml"

#include "libgen.h"

static void usage(void)
{
	printf("%s\n", VERID);
	printf(
"usage:\n"
"channelBlockMask ops\n"
"get.channelBlockMask\n"
"set.channelBlockMask cbm\n");
}

int acq200_debug;

class ChannelBlockMask {
	int block_width;
	char a_buf[256];
	char* max_block_set;
 public:
	ChannelBlockMask(TiXmlHandle docHandle) {
		TiXmlNode* cbm = docHandle.
			FirstChild("ModelSpec").
			FirstChild("ChannelBlockMask").Node();

		if (!cbm){
			fprintf(stderr, "ERROR:node %s not found\n",
				"ModelSpec.ChannelBlockMask");
			exit(-1);
		}
		TiXmlHandle nodeHandle = TiXmlHandle(cbm);
		TiXmlText* bwt  = nodeHandle.
			FirstChild("BlockWidth").FirstChild().Text();


		if (!bwt){
			fprintf(stderr, "ERROR: node %s not found\n",
				"BlockWidth");
			exit(-1);
		}
		block_width = atoi(bwt->Value());

		TiXmlText *bset = nodeHandle.
			FirstChild("BlockSet").FirstChild().Text();

		char block_sets[132];
		char *sep, *lead_quote, *trail_quote;
		char *max_block;	
		strncpy(block_sets, bset->Value(), 132);
		if ((sep = index(block_sets, ',')) != 0){
			*sep = '\0';
		}
		for (lead_quote = block_sets; *lead_quote == '"'; ++lead_quote)
			;
		max_block = lead_quote;
		if ((trail_quote = index(max_block, '"')) != 0){
			*trail_quote = '\0';
		}
		max_block_set = new char[strlen(max_block)+1];
		strcpy(max_block_set, max_block);

//		printf( "setting max_block_set \"%s\"\n", max_block_set);
		
	}
	void set(const char* block_mask) {

		if (!block_mask || strcmp(block_mask, "all") == 0){
			block_mask = max_block_set;
		}

		sprintf(a_buf, "acqcmd -- setChannelMask ");
		char* dest = &a_buf[strlen(a_buf)];
		const char* src = block_mask;
		char mask;

		while((mask = *src++) && 
		      (size_t)(src-block_mask) <= strlen(max_block_set)){
			for (int ic = 0; ic != block_width; ++ic){
				*dest++ = mask=='1'? '1': '0';
			}
		}
		*dest++ = '\n';
		*dest = '\0';

		FILE* fp = popen(a_buf, "r");
		
		do {
			puts(a_buf);
		} while(fgets(a_buf, 255, fp));

		fclose(fp);
	}

#define QPROMPT "ACQ32:getChannelMask"

	const char* get(void) {
		sprintf(a_buf, "acqcmd -- getChannelMask\n");

		FILE* fp = popen(a_buf, "r");

		while(fgets(a_buf, 255, fp)){
			char* split = strchr(a_buf, '=');
			if (split){
				*split = '\0';
				if (strcmp(a_buf, QPROMPT) == 0){
					char* src = split + 1;
					char* dst = a_buf;
					int ic = 0;

					while(char mask = *src++){
						if (ic == 0){
							*dst = mask;
						}else{
							assert(mask == *dst);
						}
						if (++ic == block_width){
							dst++;
							ic = 0;
						}
					}

					*dst = '\0';
					break;
				}else{
					;
				}
			}
		}

		fclose(fp);
		return a_buf;
	}
};

int main(int argc, const char *argv[])
{
	const char *config_file = CONFIG_FILE;
	const char *verb = basename((char *)argv[0]);

        struct poptOption opt_table[] = {
		{ "debug",      'd', POPT_ARG_INT,    &acq200_debug, 0 },
		{ "config",     'c', POPT_ARG_STRING, &config_file,  0 },
		{ "help2",      '2', POPT_ARG_NONE,   0,             '2'},
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

	TiXmlDocument* doc = new TiXmlDocument(config_file);
	if (!doc->LoadFile()){
		fprintf(stderr, "ERROR: file %s not valid\n", config_file);
		exit(-1);
	}

	/* handle modifed tree structure - later versions have
         * XML-conformant single top level root 
	 */
	TiXmlElement* root = doc->RootElement();
	assert(root);

	ChannelBlockMask* cbm;

	if (strcmp("ACQ", root->Value()) == 0){
		cbm = new ChannelBlockMask(TiXmlHandle(doc->RootElement()));
	}else{
		cbm = new ChannelBlockMask(TiXmlHandle(doc));
		err("WARNING: deprecated caldef file please modify to include"
		    " single root \"ACQ\"\n");
	}

	if (strcmp(verb, "set.channelBlockMask") == 0){
		cbm->set(poptGetArg(context));
	}else{
		printf("%s\n", cbm->get());
	}
}

