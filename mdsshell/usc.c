/*****************************************************************************
 *
 * File: usc.c
 *
 * $RCSfile: usc.c,v $
 * 
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: impl of usc wrapper
 *
 * $Id: usc.c,v 1.6 2007/09/17 14:48:42 pgm Exp $
 * $Log: usc.c,v $
 * Revision 1.6  2007/09/17 14:48:42  pgm
 * check for buf overflow
 *
 * Revision 1.5  2005/11/15 16:26:03  pgm
 * out of band error check
 *
 * Revision 1.4  2005/11/11 16:53:28  pgm
 * mdsPutCh prototype does something
 *
 * Revision 1.3  2005/05/21 14:55:40  pgm
 * split af_unix from mdsshell core
 *
 * Revision 1.2  2004/12/08 13:08:29  pgm
 * tidy iobPrintf
 *
 * Revision 1.1.1.1  2004/12/04 20:42:05  pgm
 *
 *
 * Revision 1.1.2.2  2004/02/15 13:23:47  pgm
 * localSlave socket in place
 *
 * Revision 1.1.2.1  2004/02/14 23:20:44  pgm
 * usc separated
 *
 * Revision 1.1.2.3  2004/02/14 23:13:22  pgm
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

#include <assert.h>
#include <errno.h>

#include "usc.h"


int uscMake(struct UnixSocketConnection *cn)
{
	char cmd[80];

	if ((cn->fd_connector = socket(PF_UNIX, SOCK_STREAM, 0)) < 0){
		die("socket");
	}

	unlink(cn->connector.name);
	cn->connector.unx_addr.sun_family = AF_UNIX;
	strcpy(cn->connector.unx_addr.sun_path, cn->connector.name);
	cn->connector.addr_len = 
		sizeof(cn->connector.unx_addr.sun_family)+
		strlen(cn->connector.unx_addr.sun_path);
	cn->connector.address = (struct sockaddr *)&cn->connector.unx_addr;

	if (bind(cn->fd_connector, 
		 cn->connector.address,
		 cn->connector.addr_len)){
		die("bind");
	}

	if(listen(cn->fd_connector, 5)){
		die("listen");
	}
	sprintf(cmd, "chmod a+rw %s", cn->connector.name);	
	system(cmd);
	return cn->fd_connector;
}


int uscAccept(struct UnixSocketConnection *cn)
{
	if (cn->fd_active == 0){
		cn->fd_active = accept(cn->fd_connector,
				       cn->connector.address,
				       &cn->connector.addr_len);
		assert(cn->fd_active >= 0 );
		return cn->fd_active;
	}else{
		return -EBUSY;
	}
}

void uscClose(struct UnixSocketConnection *cn)
{
	close(cn->fd_active);
	cn->fd_active = 0;
}


struct IoBuf* iobCreate(int in_max, int out_max)
{
	struct IoBuf *iob = calloc(1, sizeof(struct IoBuf));

	iob->in.buf = calloc(iob->in.maxlen = in_max, 1);
	iob->out.buf = calloc(iob->out.maxlen = out_max, 1);
	
	assert(iob->in.buf);
	assert(iob->out.buf);

	return iob;
}

#include <stdarg.h>

int iobPrintf(struct Buf *buf, const char *fmt, ...)
{
        va_list ap;
        int rc;
	int headroom = buf->maxlen - buf->len;
        va_start(ap, fmt);
        rc = vsnprintf(buf->buf+buf->len, headroom, fmt, ap);
        va_end(ap);
	if (rc > headroom){
		rc = headroom;
	}
        buf->len += rc;
        return rc;
}

void iobDestroy(struct IoBuf* iob)
{
	free(iob->in.buf);
	free(iob->out.buf);
	free(iob);
}


#include <ctype.h>

int split(char *str, char *argv[], int maxargs)
{
#define INCR(argc) (argc<maxargs? argc++: argc)
#define DQUOTE '"'
#define ISDQUOTE(c) ((c) == DQUOTE)

	int argc = 0;
	char *ptok;
	char *pend;
	enum { IN_ARG, IN_QUOTED_ARG, IN_SPACE } state = IN_SPACE;

	for (ptok = pend = str; *pend; ++pend){
		switch(state){
		case IN_ARG:
			if (ISDQUOTE(*pend)){
			    state = IN_QUOTED_ARG;
			}else if (isspace(*pend)){
				*pend = '\0';
				argv[INCR(argc)] = ptok;
				ptok = pend + 1;
				state = IN_SPACE;
			}
			break;
		case IN_SPACE:
			if (ISDQUOTE(*pend)){
				state = IN_QUOTED_ARG;
				ptok = pend+1;
			}else if (!isspace(*pend)){
				state = IN_ARG;
				ptok = pend;
			}/* else do nothing */
			break;
		case IN_QUOTED_ARG:
			if (ISDQUOTE(*pend)){
				*pend = '\0';
				argv[INCR(argc)] = ptok;
				ptok = pend + 1;
				state = IN_SPACE;
			}
			break;
		default:
			assert(0);
		}
	}

	switch(state){
	case IN_ARG:
	case IN_QUOTED_ARG:
		argv[INCR(argc)] = ptok;
	default:
		;
	}
	return argc;
}

void iobFlush(
	struct IoBuf *iob, struct UnixSocketConnection *connection)
{
	if (iob->out.len){
		int rc;
		rc = write(connection->fd_active, iob->out.buf, iob->out.len);
		if (iob->out.error){
			write(connection->fd_active, "EOF ERR\n", 8);
		}else{
			write(connection->fd_active, "EOF\n", 4);
		}
		fsync(connection->fd_active);     /* willit work ?? */
		iob->out.len = 0;
	}
}

