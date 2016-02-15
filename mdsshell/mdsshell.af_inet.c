/* ------------------------------------------------------------------------- */
/* mdsshell.af_inet.c - mdsshell with inet socket comms                      */
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

#define VERID \
    "$Id: mdsshell.af_inet.c,v 1.8 2005/11/15 16:55:13 pgm Exp $" \
    "$Revision: 1.8 $ "\
    "BUILD 1005\n"


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


#include "usc.h"
#include "mdsshell.h"
#include <popt.h>


/*****************************************************************************
 *
 * File: tcpserver.c
 *
 * $RCSfile: mdsshell.af_inet.c,v $
 * 
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: dt100d network socket handling.
 *
 * Refs: Copied from Snader (Effective TCP/IP Programming by Jon Snader)
 *
 * $Id: mdsshell.af_inet.c,v 1.8 2005/11/15 16:55:13 pgm Exp $
 * $Log: mdsshell.af_inet.c,v $
 * Revision 1.8  2005/11/15 16:55:13  pgm
 * *** empty log message ***
 *
 * Revision 1.7  2005/05/28 16:53:59  pgm
 * *** empty log message ***
 *
 * Revision 1.6  2005/05/28 16:47:32  pgm
 * *** empty log message ***
 *
 * Revision 1.5  2005/05/28 16:46:34  pgm
 * *** empty log message ***
 *
 * Revision 1.4  2005/05/28 16:45:32  pgm
 * *** empty log message ***
 *
 * Revision 1.3  2005/05/25 23:12:49  pgm
 * *** empty log message ***
 *
 * Revision 1.2  2005/05/24 20:59:32  pgm
 * readline is good - Snader rocks
 *
 * Revision 1.1  2005/05/22 16:11:16  pgm
 * af_inet works
 *
 * Revision 1.6  2004/10/20 08:03:56  pgm
 * B1001 removes acqcmd -- marks and has close opt
 *
 * Revision 1.5  2004/05/30 15:41:28  pgm
 * accumulating read to smooth driver buffer variations
 *
 * Revision 1.4  2004/05/08 14:08:34  pgm
 * live streaming ship it quick
 *
 * Revision 1.3  2004/05/01 11:36:27  pgm
 * init test data version
 *
 * Revision 1.2  2004/03/06 22:08:56  pgm
 * remote shell its a runner
 *
 * Revision 1.1.1.1  2004/03/03 21:41:11  pgm
 *
 *
 * Revision 1.9  2004/02/28 20:36:54  pgm
 * data server with endpoint detect
 *
 * Revision 1.8  2004/02/24 15:55:13  pgm
 * no line mode for complete packets
 *
 * Revision 1.7  2004/02/24 14:52:32  pgm
 * remove line buffer - not wanted, use flush on lines
 *
 * Revision 1.6  2004/02/17 12:48:02  pgm
 * data upload multi thread good
 *
 * Revision 1.2.4.1  2003/09/08 21:12:06  pgm
 * *** empty log message ***
 *
 *
 *
\*****************************************************************************/




/* include includes */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "skel.h"
#include "popt.h"

#include "local.h"

#define NLISTEN 20   /* backlog: many data connections may Q */



unsigned S_BUFLEN = SKT_BUFLEN;


const char *program_name;

/* end includes */
/* error - print a diagnostic and optionally exit */
void error( int status, int err, char *fmt, ... )
{
	va_list ap;

	va_start( ap, fmt );
	fprintf( stderr, "%s: ", program_name );
	vfprintf( stderr, fmt, ap );
	va_end( ap );
	if ( err )
		fprintf( stderr, ": %s (%d)\n", strerror( err ), err );
	if ( status )
		EXIT( status );
}

/* set_address - fill in a sockaddr_in structure */
static void set_address( 
	const char *hname, const char *sname,
	struct sockaddr_in *sap, char *protocol )
{
	struct servent *sp;
	struct hostent *hp;
	char *endptr;
	short port;

	bzero( sap, sizeof( *sap ) );
	sap->sin_family = AF_INET;
	if ( hname != NULL )
	{
		if ( !inet_aton( hname, &sap->sin_addr ) )
		{
			hp = gethostbyname( hname );
			if ( hp == NULL )
				error( 1, 0, "unknown host: %s\n", hname );
			sap->sin_addr = *( struct in_addr * )hp->h_addr;
		}
	}
	else
		sap->sin_addr.s_addr = htonl( INADDR_ANY );
	port = strtol( sname, &endptr, 0 );

	if ( *endptr == '\0' )
		sap->sin_port = htons( port );
	else
	{
		sp = getservbyname( sname, protocol );
		if ( sp == NULL )
			error( 1, 0, "unknown service: %s\n", sname );
		sap->sin_port = sp->s_port;
	}
}

#define MAXBUF 4096


void prompt(int command, FILE* fd) {
	fprintf(fd, "EOF %04d\nmdsshell>", command);
	fflush(fd);
}

extern int readline( SOCKET fd, char *bufptr, size_t len );

void interpreter(SOCKET s)
{
	struct IoBuf* iob = iobCreate(MAXBUF, MAXBUF);
	FILE* fd_net = fdopen(s, "w");
	int command_id = 0;

	prompt(command_id, fd_net);

	errno = 0;

//	while(fgets(iob->in.buf, MAXBUF-1, fd_net)){
	while(readline(s, iob->in.buf, MAXBUF-1) > 0){
		++command_id;
		dbgnl(1, "cmd>%s", iob->in.buf); 
		if (strncmp("bye", iob->in.buf, 3) == 0){
			return;
		}
		mdsshell_doServerCommand(iob);
		dbgnl(1, "out>%s", iob->out.buf);
		/** this prompt used for transaction decode */
		fprintf(fd_net, "mdsshell>");
		fwrite(iob->out.buf, 1, iob->out.len, fd_net);
		if (iob->out.buf[iob->out.len-1] != '\n' ){
			fputs("\n", fd_net);
		}
		/** this prompt for interactive visual aesthetic */
		prompt(command_id, fd_net);
		iob->out.len = 0;
	}

	perror("fgets returned null");
	err("fgets() returned null errno %d", errno);
}

/* server - place holder for server */
static void server( SOCKET s, struct sockaddr_in *peerp )
{
	static int _id;
	int id;

	id = ++_id;

	if (fork()==0){
		dbg(1, "server %d %d HELLO\n", id, getpid());
		interpreter(s);
		shutdown(s, SHUT_RDWR);
		dbg(1,"server %d close %d\n", id, getpid());
		_exit(0);
	}else{
		CLOSE(s);  /* must shut our side */
	}
}


static void reaper(int sig)
{
	int ws, rc;

	while ((rc = waitpid(-1, &ws, WNOHANG)) > 0){
		dbg(1, "reaper pid:%d status:%d\n", rc, ws);
	}
}

extern const char* mdsshell_rev;

static const char* verid(void) 
{
	static char* _verid;

	if (!_verid){
		_verid = calloc(strlen(VERID) + strlen(mdsshell_rev)+1, 1);
		strcpy(_verid, VERID);
		strcat(_verid, mdsshell_rev);
	}
	return _verid;
}

static SOCKET build_socket(void)
{
	const int on = 1;
	SOCKET s = socket( AF_INET, SOCK_STREAM, 0 );
	if ( !isvalidsock( s ) )
		error( 1, errno, "socket call failed" );

	if ( setsockopt( s, SOL_SOCKET, SO_REUSEADDR, &on,
		 sizeof( on ) ) )
		error( 1, errno, "setsockopt failed" );

	if (setsockopt(s, SOL_SOCKET, SO_RCVBUF,
		       (char*)&S_BUFLEN, sizeof(S_BUFLEN)) ){
		error(1, errno, "setsockopt() SO_RCVBUF failed\n");
	}
	if (setsockopt(s, SOL_SOCKET, SO_SNDBUF,
		       (char*)&S_BUFLEN, sizeof(S_BUFLEN)) ){
		error(1, errno, "setsockopt() SO_SNDBUF failed\n");
	}
	return s;
}

extern int acq200_debug;

static int help(void)
{
	fprintf(stderr, "help\n" );
	return 1;
}


/* main - TCP setup, listen, and accept */
int main( int argc, const char **argv )
{
	const char *hname = "localhost";
	const char *sname = "53555";

        struct poptOption opt_table[] = {
                { "help",       'h', POPT_ARG_NONE,   0,           'h' },
		{ "debug",      'd', POPT_ARG_INT,    &acq200_debug, 'd' },
		{}
	};
	

	struct sockaddr_in local;

	SOCKET s;
	int rc;
	const char *arg;

	poptContext context = poptGetContext(
                argv[0], argc, argv, opt_table, 0 );


	info("%s", verid());
	printf(verid());
	INIT();

        while((rc =poptGetNextOpt(context)) > 0){
                switch(rc) {
		case 'd':
			dbg(acq200_debug, "debug level set %d", acq200_debug);
			break;
                case 'h':
                        return help();
                }
        }
                                                                                
	if ((arg = poptGetArg(context)) != 0){
		if (poptPeekArg(context)){
			hname = arg;
			sname = poptGetArg(context);
			if (strcmp(sname, "any") == 0){
				sname = NULL;
			}
		}else{
			sname = arg;
		}
	}

	signal(SIGCHLD, reaper);

	set_address( hname, sname, &local, "tcp" );
	s = build_socket();

	if (bind(s, (struct sockaddr *)&local, sizeof(local))){
		error(1, errno, "bind failed");
	}

	if (listen( s, NLISTEN )){
		error( 1, errno, "listen failed" );
	}

	do
	{
		struct sockaddr_in peer;
		unsigned peerlen = sizeof( peer );

		SOCKET s1 = accept(s, (struct sockaddr *)&peer, &peerlen);

		if (!isvalidsock( s1 )){
			error( 1, errno, "accept failed" );
		}
		server( s1, &peer );
	} while ( 1 );

	EXIT( 0 );
}
