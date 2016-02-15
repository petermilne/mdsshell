/*****************************************************************************
 *
 * File: usc.h
 *
 * $RCSfile: usc.h,v $
 * 
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: unix socket wrapper layer
 *
 * $Id: usc.h,v 1.3 2005/11/15 16:26:03 pgm Exp $
 * $Log: usc.h,v $
 * Revision 1.3  2005/11/15 16:26:03  pgm
 * out of band error check
 *
 * Revision 1.2  2005/05/21 14:55:40  pgm
 * split af_unix from mdsshell core
 *
 * Revision 1.1.1.1  2004/12/04 20:42:05  pgm
 *
 *
 * Revision 1.1.2.4  2004/02/15 16:08:07  pgm
 * local-slave help works
 *
 * Revision 1.1.2.3  2004/02/15 15:08:03  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.2  2004/02/15 13:23:47  pgm
 * localSlave socket in place
 *
 * Revision 1.1.2.1  2004/02/14 23:20:44  pgm
 * usc separated
 *
 */

#ifndef __USC_H__
#define __USC_H__


static inline void die(char *s)
{
	perror(s);
	_exit(1);
}


struct UnixSocket {
	const char *name;
	struct sockaddr_un unx_addr;
	struct sockaddr *address;
	int addr_len;
};

struct UnixSocketConnection {
	struct UnixSocket connector;
	int fd_connector;
	int fd_active;
};

int uscMake(struct UnixSocketConnection *cn);

int uscAccept(struct UnixSocketConnection *cn);
void uscClose(struct UnixSocketConnection *cn);


static inline int 
uscConnectionPending(struct UnixSocketConnection *cn, fd_set *set)
{
	return cn->fd_connector && FD_ISSET(cn->fd_connector, set);
}

static inline int 
uscDataPending(struct UnixSocketConnection *cn, fd_set *set)
{
	return cn->fd_active && FD_ISSET(cn->fd_active, set);
}

struct FdPool {
	unsigned fds;   /* we assume 32 is the max! */
};
static inline void addFd(struct FdPool *pool, int fd)
{
	unsigned mask = 1U << fd;

	assert(fd < 32);

	pool->fds |= mask;
}
static inline void removeFd(struct FdPool *pool, int fd)
{
	unsigned mask = 1U << fd;

	pool->fds &= ~mask;
}
static inline int maxFd(struct FdPool *pool)
{
	int imax;

	for (imax = 31; imax; --imax){
		if (pool->fds & (1U << imax)){
			return imax+1;
		}
	}
	return 0;
}


/*
 * class IoBuf {
 *
 * public iobCreate(), iobDestroy()
 */
struct IoBuf {
	struct Buf {
		char *buf;
		int maxlen;
		int len;
		int error;
	}
	in, out;
};


struct IoBuf* iobCreate(int in_max, int out_max);
void iobDestroy(struct IoBuf* iob);

int iobPrintf(struct Buf *buf, const char *fmt, ...);


int split(char *str, char *argv[], int maxargs);

void iobFlush(struct IoBuf *iob, struct UnixSocketConnection *connection);

#endif /* #define __USC_H__ */
