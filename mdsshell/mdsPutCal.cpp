/* ------------------------------------------------------------------------- */
/* mdsPutCal.c -  writes voltage calibration to tree                         */
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

/** @file mdsPutCal.cpp writes voltage calibration to tree.
 * 
 * this is a custom client for mdsshell-af_unix server.
 *

- mdsPutCal --field FIELDFMT channels

 - CHANNELS: list | range
  - selects channels to read from memory
  - list : 1,2,3,6,9,12
  - range: 1-16 (or 1:16 or :)
  - channels outside channel mask are ignored.


Examples

mdsPutCal  --field "ACQ196_387.CH%02d" 1:96
*/

#define BUILD "$Id: mdsPutCal.cpp,v 1.1 2011/10/19 15:04:17 pgm Exp $ $Revision: 1.1 $ B1000"

#include <stdarg.h>
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

#define PROCLOGNAME "mdsPutCal"

extern "C" {
int acq200_debug = 0;
}
#include "local.h"
#include "acq-util.h"

//#include "usc.h"
#include "mdsshell.h"
#include <popt.h>



#include <list>

#include "parser.h"

#include "../vin/InputBlock.h"


#include "Timebase.h"

#define DEBUG_SOCK   "/tmp/mdsshell.debug"
#define _COMMAND_SOCK "/tmp/mdsshell.command"

const char* COMMAND_SOCK = _COMMAND_SOCK;


// canonical subfields
const char* GAIN_V = "GAIN_V";
const char* OFFSET_V = "OFFSET_V";

extern "C" int readline( int fd, char *bufptr, size_t len );

class UnixSocket {
	struct sockaddr_un address;
	int sock;
public:
	UnixSocket(int _sock) : sock(_sock) 
	{
	}

	UnixSocket(const char* _path) {
		if ((sock = socket(PF_UNIX, SOCK_STREAM, 0)) < 0){
			perror("socket");
			exit(-errno);
		}
		address.sun_family = AF_UNIX;
		strcpy(address.sun_path, _path);
		
		int addrLen = 
			sizeof(address.sun_family) + 
			strlen(address.sun_path);

		if (connect(sock, (struct sockaddr *)&address, addrLen)){
			perror( "connect failed SOCK" );
			exit(-errno);
		}
	}
	~UnixSocket() {		
	}
	int operator() () { return sock; }
};


class CBuf {
/* closure buffer */
public:
	char *buf;

	CBuf(int len) {
		buf = new char[len];
		buf[0] = '\0';
	}
	~CBuf() { delete [] buf; }
};

class MdsShellTransaction {
	UnixSocket& us;
public:
	int errors;

	MdsShellTransaction(UnixSocket& _us) : 
		us(_us), errors(0) {}
public:
	int put(const char* command) {
		return write(us(), command, strlen(command));
	}
	char *get(char *buf, int maxbuf) {
		CBuf mybuf(maxbuf);
		char *pbuf = buf;

		do {
			int rc = readline(us(), pbuf, maxbuf);
			if (rc < 0){
				err("readLine err %d", rc);
				exit(-errno);
			}
/** BUG what if field is called ERROR ? should be on EOF line ? */
			if (strstr(pbuf, "ERROR") || strstr(pbuf, "FAIL")){
				++errors;
				err("MDS ERROR: %s", pbuf);
			}
			pbuf = mybuf.buf;
		} while (!strstr(pbuf, "EOF"));

		return buf;
	}
};

static struct Globs {
	const char *field;
	UnixSocket* mds;
	ChannelSelection *channels;
}
	GL = {
		/* .field = */ "%02d",
	};




#define GCM	"acqcmd getChannelMask"
#define ACQPFX	"ACQ32:getChannelMask="

class ChannelMask {
	const char* the_mask;
	char* buf;
	int len;
public:
	ChannelMask(void){
		buf = new char[256];
		buf[0] = '\0';

		FILE *fp = popen(GCM, "r");
		if (fgets(buf, 256, fp)){
			if (strstr(buf, ACQPFX)){
				the_mask = buf+strlen(ACQPFX);
				len = strlen(the_mask);
			}
		}
	}
	virtual ~ChannelMask() {
		delete [] buf;
	}
	int contains(int ch) {
		int ich = ch - 1;
		if (ich >= 0 && ich < len){
			return the_mask[ich] != '0';
		}
		return 0;
	}
};

ChannelMask channel_mask;

static void mdsPutCommand(const char* fmt, ...)
{
	va_list args;
	char command[128];
	char tbuf[128];

	va_start(args, fmt);
	vsprintf(command, fmt, args);

	dbg(1, "send \"%s\"", command);

	MdsShellTransaction transaction(*GL.mds);
	transaction.put(command);
	if (transaction.get(tbuf, sizeof(tbuf)) == 0){
		err("COMMAND failed");
		exit(-errno);
	}

	dbg(1, "done: \"%s\"", tbuf);

	if (transaction.errors){
		err("COMMAND ERROR");
		exit(-1);
	}
}
// mdsPut ACQ196_387.CH01.GAIN_V 0.000305176

static int mdsPutCal(InputBlock& inputBlock) {
	ChannelSelectionIterator it(*GL.channels);
	dbg(1, "channels %s", GL.channels->toString());

	int rspan = Range::getCodeMax() - Range::getCodeMin();

	for (int ch; (ch = it.getNext()) > 0; ){

		if (!channel_mask.contains(ch)){
			continue;
		}
		
		Range range  = inputBlock.getRange(ch);

		dbg(1, "evaluate channel %d rmin %f rmax %f",
		    ch, range.rmin, range.rmax );

		double gain_v 	= range.getSpan()/rspan;
		double offset_v = (range.rmax + range.rmin)/2;

		dbg(1, "ch[%2d] gain:%f offset:%f", ch, gain_v, offset_v);

		char field_exp[128];

		sprintf(field_exp, GL.field, ch);
		mdsPutCommand("mdsPut -- %s.%s %f", field_exp, GAIN_V, gain_v);
		mdsPutCommand("mdsPut -- %s.%s %f", field_exp, OFFSET_V, offset_v);
	}

	dbg(1, "99");
	return 0;
}



static struct poptOption opt_table[] = {
{ "field", 'F', POPT_ARG_STRING, &GL.field, 0,
	"mdsplus field name %02d substitutes channel" },
{ "verbose", 'v', POPT_ARG_INT, &acq200_debug, 0 },
POPT_AUTOHELP
POPT_TABLEEND		
};



int repeat_mode = 0;


int get_args(int argc, const char* argv[], const char **&params)
{
	int rc;	
	poptContext opt_context = 
		poptGetContext(argv[0], argc, argv, opt_table, 0);
	while ((rc = poptGetNextOpt(opt_context)) > 0){
		switch(rc){
		default:
			;
		}
	}
	int argc2;
	const char ** argv2 = poptGetArgs(opt_context);

	for (argc2 = 0; argv2 && argv2[argc2] != 0; ++argc2){

	}
	if (argc2){
		params = argv2;
	}
	return argc2;	
}  

int process(int nparams, const char** params, InputBlock& inputBlock)
{
	dbg(1, "make ChannelSelection %d", nparams);

	GL.channels = new ChannelSelection(
		inputBlock.getNumChannels(), nparams, params);

	return mdsPutCal(inputBlock);
} 

#define MAXLINE 4096

static void init_env(void)
{
	if (getenv("GAIN_V")) GAIN_V = getenv("GAIN_V");
	if (getenv("OFFSET_V")) OFFSET_V = getenv("OFFSET_V");
}
int main(int argc, const char* argv[])
{
	const char *config_file = CONFIG_FILE;
	const char** params = 0;

	init_env();
	int nparams = get_args(argc, argv, params);

	GL.mds =  new UnixSocket(COMMAND_SOCK);

	dbg(1, "instantiate inputBlock...");
	InputBlock& inputBlock = InputBlock::getInstance(config_file);
	
	if (nparams == 0){
		/** command[s] on stdin - saves inputBlock overhead */
		char* buf = new char[MAXLINE];
		int rc = 0;
		
		while (fgets(buf, MAXLINE, stdin)){
			char* l_argv[20];			
			int l_argc = strsplit(buf, l_argv, 20, " ");
			int l_params =  get_args(
					l_argc, (const char**)l_argv, params);
			
			if (l_params){
				rc = process(l_params, params, inputBlock);				
				if (rc != 0){
					break;		
				}
			}
		}
				
		delete [] buf;
		return rc;
	}else{
		return process(nparams, params, inputBlock);	
	}
}
