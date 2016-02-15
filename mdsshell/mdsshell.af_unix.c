/* ------------------------------------------------------------------------- */
/* mdsshell.af_unix.c - mdsshell with unix socket comms                      */
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

/** @file mdsshell.af_unix.c UNIX socket interface to mdsshell. */

#define BUILD "$Id: mdsshell.af_unix.c,v 1.24 2009/09/10 18:47:36 pgm Exp $$Revision: 1.24 $ B1032"

#include <libgen.h>                   /* basename */
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

#define PROCLOGNAME "mdsshell"
#include "local.h"


#include "etcp.h"
#include "usc.h"
#include "mdsshell.h"
#include <popt.h>

#define DEBUG_SOCK   "/tmp/mdsshell.debug"
#define COMMAND_SOCK "/tmp/mdsshell.command"

/** Environment KEYS */

#define MDSSHELL_LOCAL_SKT	"MDSSHELL_LOCAL_SKT"
#define MDSSHELL_SCRIPT		"MDSSHELL_SCRIPT"


enum TOP_STATE { TS_NOTHING, TS_HAVE_COMMAND, TS_HAVE_DEBUG };

int debugs = 0;
int commands = 0;
int euid;
char *debug_sock = DEBUG_SOCK;
char *command_sock = COMMAND_SOCK;

#define EPATH "/usr/local/bin"

const char* epath = EPATH;

#define JOB "/tmp/mdsshell.job"


static void ln_visitor(void *vobj, const char* command) 
{
	FILE* fp = (FILE*)vobj;

	fprintf(fp, "ln -s %s/%s %s/%s\n", epath, "mdsshell", epath, command);
}

static void makelinks(void)
{
	FILE* fp = fopen(JOB, "w");

	mdsshell_iterateCommands(ln_visitor, fp);
	fclose(fp);
	system("chmod u+x "JOB";"JOB);
}

static void debugPrompt(void)
{
	static int dbg_num;

	printf("debug %d>", ++dbg_num);
	fflush(stdout);
}

static enum TOP_STATE new_state(
        enum TOP_STATE state, 
        struct timeval *on_timeout,
	struct IoBuf *iob_deb, 
        struct IoBuf *iob_cmd
)
{
	static int first_time = 1;
	static struct FdPool fdpool;
	static struct UnixSocketConnection debug;
	static struct UnixSocketConnection command;
	static fd_set watchset;
	static sigset_t blocker;  /* block SIGPIPE */

	struct timeval timeout = {
		.tv_sec = 0,
		.tv_usec = 10000
	};
	struct timeval in_timeout;

	fd_set inset;
	
	int rc;

	if (first_time){
		debug.connector.name = debug_sock;
		command.connector.name = command_sock;

		sigemptyset(&blocker);
		sigaddset(&blocker, SIGPIPE);
		sigprocmask(SIG_BLOCK, &blocker, 0);

		addFd(&fdpool, uscMake(&debug));
		addFd(&fdpool, uscMake(&command));

		FD_ZERO(&watchset);
		FD_SET(debug.fd_connector, &watchset);
		FD_SET(command.fd_connector, &watchset);
	
		first_time = 0;
	}

	iobFlush(iob_cmd, &command);
	iobFlush(iob_deb, &debug);

	inset = watchset;
	memcpy(&in_timeout, &timeout, sizeof(struct timeval));

	rc = select(maxFd(&fdpool), &inset, 0, 0, &in_timeout);
	if ( rc < 0 ){
		if (errno == EINTR){
			dbg(1, "select was interrupted");
		}else{		
			err("select return error %d", errno);
			_exit(-errno);
		}
	}else if (rc == 0){
		return TS_NOTHING;
	}else{
		if (uscDataPending(&command, &inset)){
			rc = read(command.fd_active, 
				  iob_cmd->in.buf, iob_cmd->in.maxlen);
			if (rc < 0){
				die("read(command)");
			}else if (rc == 0){
				FD_CLR(command.fd_active, &watchset);
				removeFd(&fdpool, command.fd_active);
				uscClose(&command);
				return TS_NOTHING;
			}else{
				iob_cmd->in.buf[rc] = '\0';
				iob_cmd->in.len = rc;
				++commands;
				return TS_HAVE_COMMAND;
			}			
		}
		if (uscConnectionPending(&command, &inset)){
			if (uscAccept(&command) >= 0){
				FD_SET(command.fd_active, &watchset);
				addFd(&fdpool, command.fd_active);
#if 0  /* WORKTODO fdup ? */
				setvbuf(command.fd_active, NULL, _IOLBF, 0);
#endif
			}			
		}
		if (uscDataPending(&debug, &inset)){
			printf( "%d:server[%d] debug commands %d\n", 
				debugs, getpid(), commands);

			rc = read(debug.fd_active, 
				  iob_deb->in.buf, iob_deb->in.maxlen);
			if (rc < 0){
				die("read(debug_sock)");
			}else if (rc == 0){
				FD_CLR(debug.fd_active, &watchset);
				removeFd(&fdpool, debug.fd_active);
				uscClose(&debug);
				return TS_NOTHING;
			}else{
				iob_deb->in.buf[rc] = '\0';
				iob_deb->in.len = rc;
				++debugs;
				return TS_HAVE_DEBUG;
			}
		}
		if (uscConnectionPending(&debug, &inset)){
			if (uscAccept(&debug) >= 0 ){
				FD_SET(debug.fd_active, &watchset);
				addFd(&fdpool, debug.fd_active);

				/* redirect printf */
				close(1); dup(debug.fd_active);
				close(2); dup(debug.fd_active);
				/* lets hope stdout still assoc with fd=1 */
				/* force line buf */
				setvbuf(stdout, NULL, _IOLBF, 0);
				setvbuf(stderr, NULL, _IOLBF, 0);

				debugPrompt();
			}
		}
#if 0
		printf( "HERE with nowhere to go %08lx rc %d\n", inset, rc);
		printf( "%s: %d %s\n", "connector", debug.fd_connector, 
			FD_ISSET(debug.fd_connector, &inset)? "SET": "CLR" );
		printf( "%s: %d %s\n", "active", debug.fd_active, 
			FD_ISSET(debug.fd_active, &inset)? "SET": "CLR"  );
#endif
		return TS_NOTHING;
	}
	return state;
}

#ifdef __arm
extern void setSendFile(int enable);
#endif

static int doDebug(struct IoBuf *debug)
{
#define MAXARGS 10
#define IS_COMMENT(s) (s[0] == '#')
	char* argv[MAXARGS];
	int argc = split(debug->in.buf, argv, MAXARGS);

	if (argc == 0 || IS_COMMENT(argv[0])){        /* are you there ? */
		return 0;
	}
	if (strstr(argv[0], "debug")){
		acq200_debug = atoi(argv[1]);
#ifdef __arm
	}else if (strstr(argv[0], "sendfile")){
		setSendFile(atoi(argv[1]));
		printf("setSendFile %d\n", atoi(argv[1]));
#endif
	}

	debugPrompt();
	
	return 0;  	
}

static int server_work(void) 
{
	struct IoBuf *debug = iobCreate( 1024, 1024 );
	struct IoBuf *cmd   = iobCreate( MAXEXP, MAXEXP );
	struct timeval on_timeout = { .tv_sec = 10 };
	enum TOP_STATE state = TS_NOTHING;
	


	for (;;){
		switch(state = new_state(state, &on_timeout, debug, cmd)){
		case TS_NOTHING:
			break;
		case TS_HAVE_COMMAND:
			dbg(1, "TS_HAVE_COMMAND %s\n", cmd->in.buf);

			cmd->out.error = mdsshell_doServerCommand(cmd);
			break;
		case TS_HAVE_DEBUG:
			cmd->out.error = doDebug(debug);
			break;
		}
		if (euid){
			seteuid((gid_t)(euid));
			info("seteuid asked:%d got:%d", euid, (int)geteuid());
			euid = 0;
		}
	}
	iobDestroy(cmd);
	iobDestroy(debug);
	return 0;
}

#ifdef __arm
extern void acq200_mdsip_setDebug(int level);
#endif

#define MAXCMD 4096

static int read_command(FILE *fp, struct Buf *cmd, int *line_num)
{
	int cursor = 0;
	int have_command = 0;

	for (; fgets(cmd->buf+cursor, cmd->maxlen-cursor, fp); ++*line_num){

		if (acq200_debug){
			printf(cmd->buf);
		}

		if (cmd->buf[0] == '#' || cmd->buf[0] == '\n'){
			continue;
		}else{
			have_command = 1;
			chomp(cmd->buf);
			if (cmd->buf[strlen(cmd->buf)-1] == '\\'){
				cursor = strlen(cmd->buf)-1;
			}else{
				break;
			}
		}
	}

	return have_command;
}

#define MAXARGSE 30



static void spawnCmd(struct IoBuf *cmd)
{
	char *argv[MAXARGSE];
	int argc = split(cmd->in.buf, argv, MAXARGSE);
	int sp[2];
	int rc;
	pid_t cpid;

	argv[argc] = (char*)NULL;
	rc = socketpair(PF_UNIX, SOCK_STREAM, 0, sp);

	assert(rc == 0);

	if ((cpid = fork()) == 0){
		char envb[32];
		sprintf(envb, "COMMAND_SOCK=%d", sp[1]);
		close(sp[0]);
		execvp(argv[0], argv);
	}else{
		int cursor = 0;
		close(sp[1]);
		while((rc = read(sp[0], cmd->in.buf+cursor,
					cmd->in.maxlen-cursor))>= 0){
			cursor += rc;
		}
		cmd->in.len = cursor;
		cmd->in.buf[cursor] = '\0';
		waitpid(cpid, 0, 0);
		close(sp[0]);
	}
}

//const char *MDSPUTCH = "mdsPutCh";


static int batch_work(FILE *fp)
{
#define MDSPUTCH	"mdsPutCh"
	struct IoBuf *cmd = iobCreate( MAXEXP, MAXEXP );
	struct UnixSocketConnection batch_out = {
		.fd_active = 1		/* stdout */
	};

	int line = 0;
	int rc;
	assert(cmd);

	while(read_command(fp, &cmd->in, &line) > 0){
		if (strncmp(cmd->in.buf,  MDSPUTCH, strlen(MDSPUTCH)) == 0){
			spawnCmd(cmd);
		}
		rc = mdsshell_doServerCommand(cmd);
		iobFlush(cmd, &batch_out);

		if (rc){
			fprintf(stderr, "ERROR at line %d\n", line);
			return -1;
		}
	}

	return 0;
}

static int process_script(const char* script)
{
	if (strcmp(script, "-") == 0){
		return batch_work(stdin);
	}else{
		FILE *fp = fopen(script, "r");
		if (fp != 0){
			return batch_work(fp);
		}else{
			err("FAILED to open file \"%s\"", 
				script);
			exit(1);
			return -1;
		}
	}
}

static int server(int argc, const char* argv[]) 
{
	static char* script;
	static struct poptOption opt_table[] = {
		{ "help2",      'h', POPT_ARG_NONE,   0,           'h' },
		{ "makelinks",  'm', POPT_ARG_NONE,   0,           'm' },
		{ "verbose",    'v', POPT_ARG_INT,    &acq200_debug, 'v' },
		{ "euid",       'e', POPT_ARG_INT,    &euid,        'e'},
		{ "path",       'p', POPT_ARG_STRING, &epath,      0 },
		{ "batch",      'b', POPT_ARG_NONE,   0, 'b' },
		{ "file",       'f', POPT_ARG_STRING, &script, 's' },
		{ "socket",     's', POPT_ARG_STRING, &command_sock, 0 },
		{ "dsocket",    'd', POPT_ARG_STRING, &debug_sock, 0 },
		POPT_AUTOHELP
		POPT_TABLEEND
	};
	int rc;
	poptContext opt_context =
		poptGetContext( argv[0], argc, argv, opt_table, 0 );

	while ((rc = poptGetNextOpt(opt_context)) > 0){
		switch(rc){
#ifdef __arm
		case 'v':
			acq200_mdsip_setDebug(acq200_debug);
			break;
#endif
		case 'h':
			mdsshell_help(); return 0;
		case 'm':
			makelinks(); return 0;
		case 's':
			return process_script(script);
		case 'b':
			return batch_work(stdin);
		default:
			;
		}
	}

	if ((script = getenv(MDSSHELL_SCRIPT)) != 0){
		return process_script(script);
	}else{
		return server_work();
	}
}



/**
 *   client ...
 */


static int getUnixSocket(const char* _path) 
{
	struct sockaddr_un address;
	int sock;	

	if ((sock = socket(PF_UNIX, SOCK_STREAM, 0)) < 0){
		perror("socket");
		exit(-errno);
	}
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, _path);
		
	int addrLen = sizeof(address.sun_family) + strlen(address.sun_path);

	if (connect(sock, (struct sockaddr *)&address, addrLen)){
		perror( "connect failed SOCK" );
		exit(-errno);
	}	
	return sock;
}

#define MAXBUF 4096

static int client_work(char* buf)
{
	int sock = getUnixSocket(command_sock);	
	char *pread = buf;
	int rc = 0;

	write(sock, buf, strlen(buf));

	do {
		pread += rc;

		rc = readline(sock, pread, MAXBUF - (pread-buf));
		if (rc <= 0){
			if (rc == 0){
				err("readline EOF");
				exit(1);
			}else{
				err("readline err %d", rc);
				exit(-errno);
			}
		}
	} while (strncmp(pread, "EOF", 3) != 0);

	rc = 0;
/** ideally the error should come "out of band" on the EOF line */
	if (strstr(pread, "ERR")){
		fprintf(stderr, "MDS ERROR \"%s\"\n", pread);
		rc = -1;
	}
	*pread = '\0';        /* stub the EOF line */
	fputs(buf, stdout);
	return rc;
}

static void processEnv(void)
{
	if (getenv("MDSSHELL_VERBOSE")){
		acq200_debug = atoi(getenv("MDSSHELL_VERBOSE"));
	}
	if (getenv("MDSSHELL_SOCKET")){
		command_sock = getenv("MDSSHELL_SOCKET");
	}
	if (getenv("MDSSHELL_DSOCKET")){
		debug_sock = getenv("MDSSHELL_DSOCKET");
	}
}

static int client(int argc, const char* argv[])
{
	char *command = malloc(MAXBUF);
	char *verb = malloc(strlen(argv[0])+1);
	int iarg;
	int rc;

	processEnv();
	assert(command);
	assert(verb);
	strcpy(verb, argv[0]);
	strcpy(command, basename(verb));
	free(verb);

	for (iarg = 1; iarg < argc; ++iarg){
		strcat(command, " ");
		strcat(command, argv[iarg]);
	}
	strcat(command, "\n");

	rc = client_work(command);

	free(command);
	return rc;
}



extern char* mdsshell_rev;

int main(int argc, const char* argv[])
{
	if (getenv(MDSSHELL_LOCAL_SKT) != 0){
		command_sock = getenv(MDSSHELL_LOCAL_SKT);
	}
	if (strstr(argv[0], "mdsshell")){
		printf("mdsshell %s %s\n%s\n", BUILD, argv[0], mdsshell_rev);
		return server(argc, argv);
	}else{
		return client(argc, argv);
	}	
}


