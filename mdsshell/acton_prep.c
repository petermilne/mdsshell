/* ------------------------------------------------------------------------- */
/* acton_prep - action on completion of PREP                                 */
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


#define VERSION "acton_prep $Revision: 1.4 $ B1000"

#define USAGE "\
acton_prep [opts] [command] [args]\n\
opts: --norecycle do not recycle data on completion.\n\
\n\
Command: any ARM side command to run, with <args>\n\
\n\
The command is run with the following environment variables set:\n\
PREP_ID=id\n\
PREP_PATH=path to prep data files\n\
PREP_FILE=parth to prep data file XXP\n\
PREP_START=start\n\
PREP_SAM=length in samples\n\
PREP_SHORTS=length in shots\n"

#include "local.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "popt.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include "acq200_root.h"

int norecycle;
int acq200_debug;


struct poptOption opt_table[] = {
	{ "norecycle", 0,  POPT_ARG_NONE, 0,            'r' },
        { "version", 'v',  POPT_ARG_NONE, 0,            'v' },
        { "usage",    0,   POPT_ARG_NONE, 0,            'u' }, 
        { "verbose", 'd',  POPT_ARG_INT,  &acq200_debug, 0, 0},
        POPT_AUTOHELP
        POPT_TABLEEND
};

#define DATA        "/dev/prep/data"
#define PREP_STATUS "/dev/prep/status"
#define MAXLINE 80

#define MAXENV_ITEM 128

/*
root@acq196_001 ~ #cat /dev/prep/status
005.4000000      2000   COMPLETE
006.6000000      3000   COMPLETE
*/

struct Action {
	char id[16];
	char start[32];
	char len[32];
	char status[32];
	char scc[32];
	char tv_sec[32];
	char tv_usec[32];
};


struct EnvMap {
	const char * key;
	void (*action)(char *value, struct Action *action);
	char * value;
	char * buf;
};


static int getSampleSize(void)
{
	int sample_size = 32;   /* set a default any default! */

	acq200_getRoot(DEV_ROOT, "sample_size", 1, "%d", &sample_size);

	return sample_size;
}
static struct Action *parse(const char* line)
{
	static struct Action action;

	if (sscanf(line, "%3s.%s %s %s %s %s %s", 
		   action.id, action.start,
		   action.len, action.status,
		   action.scc, action.tv_sec, action.tv_usec) == 7){
		return &action;
	}else{
		fprintf(stderr, "ERROR:parse FAILED\n");
		_exit(-1);
	}
}

static void actionID(char *value, struct Action *action) {
	strcpy(value, action->id);
}
static void actionPath(char *value, struct Action *action) {
	sprintf(value, "%s/%s.%s/", DATA, action->id, action->start);
}
static void actionFile(char *value, struct Action *action) {
	sprintf(value, "%s/%s.%s/XXP", DATA, action->id, action->start);
}
static void actionStart(char *value, struct Action *action) {
	strcpy(value, action->start);
}
static void actionSam(char *value, struct Action *action) {
	strcpy(value, action->len);
}
static void actionShorts(char *value, struct Action *action) {
	long len = strtoul(action->len, 0, 0);
	sprintf(value, "%ld", len*getSampleSize());
}


static void actionScc(char *value, struct Action *action) {
	strcpy(value, action->scc);
}

static void actionTv(char *value, struct Action *action) {
	sprintf(value, "\"tv_sec=%s tv_usec=%s\"", 
		action->tv_sec, action->tv_usec);
}

static void actionTod(char *value, struct Action *action) {
	time_t start_time = strtoul(action->tv_sec, 0, 0);
	ctime_r(&start_time, value);
}

struct EnvMap map[] = {
	{ .key = "PREP_ID=", .action = actionID },
	{ .key = "PREP_PATH=", .action = actionPath },
	{ .key = "PREP_FILE=", .action = actionFile },
	{ .key = "PREP_START=", .action = actionStart },
	{ .key = "PREP_SAM=",   .action = actionSam },
	{ .key = "PREP_SHORTS=", .action = actionShorts },
	{ .key = "PREP_SCC=",    .action = actionScc },
	{ .key = "PREP_TV=",     .action = actionTv },
	{ .key = "PREP_TIME=",   .action = actionTod }
};
#define MAPLEN  (sizeof(map)/sizeof(struct EnvMap))


static void do_putenv(struct EnvMap *map, struct Action *action)
{
	map->action(map->value, action);
	putenv(map->buf);
}

static void init_map(struct EnvMap *map, struct Action *def_action)
{
	map->buf = malloc(MAXENV_ITEM);
	strcpy(map->buf, map->key);
	map->value = map->buf + strlen(map->key);

	do_putenv(map, def_action);
}

static void init_env(void)
{
	struct Action def_action = {
		.id = "999", .start = "1234", .len = "6666666", 
		.status = "gash"
	};
	int imap;

	for (imap = 0; imap != MAPLEN; ++imap){
		init_map(&map[imap], &def_action);
	}
}

static void do_action(struct Action *action, char** child_def)
{
	int imap;

	for (imap = 0; imap != MAPLEN; ++imap){
		do_putenv(&map[imap], action);
	}

	pid_t child;

	if ((child = fork()) == 0){
		execvp(child_def[0], child_def);
	}else{
		int status;

		wait(&status);

		if (status == 0 && !norecycle){
			char cmd[80];
			char path[80];

			actionPath(path, action);

			sprintf(cmd, "rm -Rf %s", path);
			dbg(1, "recycle %s NOW\n", cmd);
			system(cmd);
		}else{
			fprintf(stderr, 
				"no recycle %d status %d", norecycle, status);
		}
	}
}


static void acton_prep(char** child_def)
{
	char next_line[MAXLINE];
	FILE* fp = fopen(PREP_STATUS, "r");
	if (!fp){
		fprintf(stderr, "ERROR: failed to open %s\n", PREP_STATUS);
		_exit(errno);
	}

	while(fgets(next_line, MAXLINE, fp)){
		do_action(parse(next_line), child_def);
	}
	fclose(fp);
}

int main( int argc, const char* argv[] )
{
       poptContext opt_context = 
		poptGetContext(argv[0], argc, argv, opt_table, 0 );
       int rc;

       while ((rc = poptGetNextOpt( opt_context )) > 0){
	       switch(rc){
	       case 'r':
		       norecycle = 1; break;
	       case 'v':
		       fprintf(stderr, "%s\n", VERSION); return 0;
	       case 'u':
		       fprintf(stderr, "%s\n", USAGE); return 0;
	       }
       }

       init_env();
       acton_prep(poptGetArgs(opt_context));
       return 0;
}
