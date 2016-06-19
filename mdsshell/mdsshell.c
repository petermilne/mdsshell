/*****************************************************************************
 *
 * File: mdsshell.c
 *
 * $RCSfile: mdsshell.c,v $
 * 
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: mdsshell.c,v 1.65 2011/11/13 17:11:48 pgm Exp $
 * $Log: mdsshell.c,v $
 * Revision 1.65  2011/11/13 17:11:48  pgm
 * ensure status gets passed back, avoid bogus error
 *
 * Revision 1.64  2011/11/12 19:01:27  pgm
 * B1051 fixes memory leak in build_timebase_descrip()
 *
 * Revision 1.63  2011/11/08 20:50:16  pgm
 * nsegs ..
 *
 * Revision 1.62  2011/11/08 18:59:34  pgm
 * doMdsValuePutSegment, first cut
 *
 * Revision 1.61  2011/11/08 17:54:12  pgm
 * doMdsValuePutSegment, first cut
 *
 * Revision 1.60  2011/11/08 16:57:00  pgm
 * B1049 fixes regression to mdsPut
 *
 * Revision 1.59  2011/11/05 21:24:12  pgm
 * B1048 mdsValuePut -T
 *
 * Revision 1.58  2011/11/05 19:51:47  pgm
 * -T timebase opt added to mdsValuePut
 *
 * Revision 1.57  2011/11/05 16:29:44  pgm
 * --window - subset the data to upload NOT MDS window
 *
 * Revision 1.56  2011/10/19 12:35:40  pgm
 * docs for mdsValuePut
 *
 * Revision 1.55  2011/10/19 10:45:31  pgm
 * doMdsValuePut with channel iterator works
 *
 * Revision 1.54  2011/10/19 09:28:29  pgm
 * change to integer channel descr
 *
 * Revision 1.53  2011/10/19 08:45:38  pgm
 * B1044: %llu WORKS
 *
 * Revision 1.52  2011/10/18 20:01:03  pgm
 * mdsValuePut implemented
 *
 * Revision 1.51  2011/09/20 19:41:02  pgm
 * -o - stdout hooked up
 *
 * Revision 1.50  2010/02/03 08:08:12  pgm
 * treats float dim[1] as scalar
 *
 * Revision 1.49  2009/02/04 22:56:57  pgm
 * @todo dumpOctaveBuf is default
 *
 * Revision 1.48  2008/04/30 14:34:08  pgm
 * --ull format, generalise scalars
 *
 * Revision 1.47  2007/10/01 20:44:59  pgm
 * split dump funs
 *
 * Revision 1.46  2007/09/17 16:48:29  pgm
 * handles multi dim arrays, thanks SJM
 *
 * Revision 1.45  2007/03/23 15:14:14  pgm
 * mdsshell timeout handled correctly
 *
 * Revision 1.44  2007/03/07 17:50:31  pgm
 * --batch, --timeout opts
 *
 * Revision 1.43  2007/03/07 10:42:54  pgm
 * mdsValue(waitev('evt')) DTYPE_UCHAR nil pointer - ignore
 *
 * Revision 1.42  2007/03/07 10:20:44  pgm
 * mdsValue supports DTYPE_UCHAR
 *
 * Revision 1.41  2007/01/31 09:34:40  pgm
 * set correct doMdsPutFloat expression -thanks ncroker
 *
 * Revision 1.40  2006/12/08 15:10:09  pgm
 * default format short
 *
 * Revision 1.39  2006/09/29 17:24:51  pgm
 * split stride as separate %STR key
 * try to use measured smaple rate
 *
 * Revision 1.38  2006/09/24 10:52:30  pgm
 * mdsDisconnect exits cleanly
 *
 * Revision 1.37  2006/09/20 16:06:17  pgm
 * fix bug blocking mdsValue --output
 *
 * Revision 1.36  2006/09/09 06:33:46  pgm
 * *** empty log message ***
 *
 * Revision 1.35  2005/12/15 10:27:26  pgm
 * CROSS YES and NO works
 *
 * Revision 1.34  2005/12/15 08:36:18  pgm
 * mdsDisconnect
 *
 * Revision 1.33  2005/12/01 12:37:19  pgm
 * sendfile interface and better errchk
 *
 * Revision 1.32  2005/11/17 21:18:44  pgm
 * *** empty log message ***
 *
 * Revision 1.31  2005/11/15 16:55:13  pgm
 * *** empty log message ***
 *
 * Revision 1.30  2005/11/15 15:59:20  pgm
 * mdsValue concats multiple args
 *
 * Revision 1.29  2005/11/15 10:02:57  pgm
 * mdsValue handles multiple types with -output [-o] file option
 *
 * Revision 1.28  2005/11/11 16:53:28  pgm
 * mdsPutCh prototype does something
 *
 * Revision 1.27  2005/06/13 18:40:40  pgm
 * B1032 fixes subarray bugs
 *
 * Revision 1.26  2005/06/12 20:38:45  pgm
 * B1029 fixes subtle no subarray bug
 *
 * Revision 1.25  2005/06/10 16:46:37  pgm
 * B1028 rases the MAXARGS limit to 20
 *
 * Revision 1.24  2005/06/10 16:33:53  pgm
 * subarray B1027 nfg
 *
 * Revision 1.23  2005/05/28 16:53:59  pgm
 * *** empty log message ***
 *
 * Revision 1.22  2005/05/28 16:45:32  pgm
 * *** empty log message ***
 *
 * Revision 1.21  2005/05/28 16:41:30  pgm
 * *** empty log message ***
 *
 * Revision 1.20  2005/05/25 23:12:49  pgm
 * *** empty log message ***
 *
 * Revision 1.19  2005/05/24 21:19:27  pgm
 * fixes immediate bug remembering last file
 *
 * Revision 1.18  2005/05/24 20:59:32  pgm
 * readline is good - Snader rocks
 *
 * Revision 1.17  2005/05/23 20:45:11  pgm
 * update for generic formats, immediate and multiple files
 *
 * Revision 1.16  2005/05/22 16:11:16  pgm
 * af_inet works
 *
 * Revision 1.15  2005/05/21 14:55:40  pgm
 * split af_unix from mdsshell core
 *
 * Revision 1.14  2005/05/21 13:46:54  pgm
 * acqcmd, epath configurable
 *
 * Revision 1.13  2005/04/14 21:16:03  pgm
 * remove repeated euid message
 *
 * Revision 1.12  2005/04/14 20:56:08  pgm
 * *** empty log message ***
 *
 * Revision 1.11  2005/04/14 19:21:38  pgm
 * fix warning
 *
 * Revision 1.10  2005/04/14 19:20:19  pgm
 * mdsPut shorts auto sizes on last dimension.
 *
 * Revision 1.9  2005/02/08 16:54:48  pgm
 * put to 0
 *
 * Revision 1.8  2005/01/11 16:56:40  pgm
 * acton_prep works
 *
 * Revision 1.7  2004/12/08 21:08:37  pgm
 * dim x,y,z opt
 *
 * Revision 1.6  2004/12/08 13:08:29  pgm
 * tidy iobPrintf
 *
 * Revision 1.5  2004/12/07 22:35:24  pgm
 * *** empty log message ***
 *
 * Revision 1.4  2004/12/07 12:00:58  pgm
 * tested string, shorts its good
 *
 * Revision 1.3  2004/12/05 21:51:35  pgm
 * Now with mdsOpen, mdsPut --format {float|cstring|shorts} --length l (experimental).
 *
 * Revision 1.2  2004/12/04 20:47:28  pgm
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2004/12/04 20:42:05  pgm
 *
 *
 */

/** @mainpage Mdsshell - MDSplus Thin Client. 
 *
 * $Id: mdsshell.c,v 1.65 2011/11/13 17:11:48 pgm Exp $ $Revision: 1.65 $
 *
 * The goal of the MDSplus Thin Client is to provide a simple interface
 * so that shell scripts running on D-TACQ ACQ216, ACQ196 series intelligent
 * digitizers are able to store capture data to a remote MDSplus Tree on the
 * network using the MDSTCPIP protocol. 
 * 
 * - "Send data to MDSplus, NO DRIVERS REQUIRED"
 *
 * Typically Mdsshell commands are embedded in the post shot script 
 * - /usr/local/bin/acq200.pp
 * The commands allow data to be uploaded to MDSplus immediately 
 * it is available.
 *
 * The scripting interface provides a similar capability for the Linux shell
 * that is provided for IDL, but without the need for any non-free software,
 * or for a heavyweight math environment on the embedded ARM processor.
 *
 * Mdsshell comprises a local client/server architecture, where the server acts
 * as a proxy for the remote MDSIP daemon, maintaining persisten connections.
 * Shell commands are clients of this local proxy. 
 *
 * Mdsshell is available on both ARM and x86 environments, and has found 
 * application on the x86 host side in cases where scriptable support is 
 * required, but there is no need for an expensive and heavyweight math
 * package.
 *
 * The MDSplus Thin Client comprises
 * - mdsshell - server
 * - mdsshell - low level client applications
 *  - mdsConnect <host>
 *  - mdsOpen <tree>
 *  - mdsPut [opts] value
 *  - mdsValue
 *  - mdsValuePut
 *  - mdsClose
 * - mdsPutCh - high level client 
 *  - efficient channel by channel data upload.
 *  - automated support of digital calibration.
 *  - data is stored as an MDSplus SIGNAL with automated timebase.
 * 
 */
/** @file mdsshell.c mds shell interface.
 *
 * executable may be run as server or client
 *
 * - server interfaces to libMdsIpShr and holds persistent sockets
 * - client submits a single command to the server
 *
 * uses a busybox-style named link system, argv[0] defines command
 * - mdsValuePut [options] EXPR
@verbatim
Usage: mdsValuePut [OPTION...]
  -d, --data=STRING      data from file [not implemented]
  -c, --ch1=STRING       channel spec subs expr arg $1 eg -c 1:96
  -k, --k234=STRING      constant spec subs expr $2,$3,$4
  --ch1_type=STRING      type of raw data [ushort (only)]
  --k234_type=STRING     type of constant [ull (only)]
  -T                     generate long long timebase in CH00
  -t, --timeout=INT      timeout in s [0 : off]
  --nsegs=INT			 split upload into nsegs segments [1]
  -h, --help
  -u, --usage
@endverbatim
 * - example:
@verbatim
mdsValuePut --ch1 1:96 --k234 1300000,1400000,1 \
'BeginSegment(ACQ196_387.CH%02d,$2,$3,make_dim(*,$2 : $3 : $4),$1)'
@endverbatim
 */

#define BUILD \
	"$Id: mdsshell.c,v 1.65 2011/11/13 17:11:48 pgm Exp $"\
	"$Revision: 1.65 $ B1051\n"

char *mdsshell_rev = BUILD;

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <setjmp.h>

#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
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

#define USE_GCC_CONST
#include "../mdsplus/mdstcpip/mdsip.h"

#include "mdsshell.h"

#include "acq-util.h"

#define DTYPE_NONE	-1

extern void set_timeout(int timeout_secs, jmp_buf* env);

int SendMdsMsg(SOCKET sock, Message *m, int oob);

//#include <mdslib.h>
/* mdslib, ipdesc prototypes clash - so lift this one and hope for the best */
int descr (int *dtype, void *data, int *dim1, ...);


#include "popt.h"

int acq200_debug = 0;

static long S_sock;           /* handle of the mdstcpip socket */
static const char* froot;     /* file root */


const char* fname(const char* fn) {
	if (froot == 0){
		return fn;
	}else{
		static char fname[256];

		if (strstr(froot, "%s") != 0){
			sprintf(fname, froot, fn);
		}else{
			strcpy(fname, froot);
			strcat(fname, "/");
			strcat(fname, fn);
		}
		return fname;
	}
}


#define MAXDIMS 4
#define IMMEDIATE_IDENT '['
#define LIST_SEPS ",;:"

#define MAXFILE 256

static int doMdsPutError(const char* field, const char* value, struct Buf* out);
static int doMdsPutFloat(const char* field, const char* value, struct Buf* out);
static int doMdsPutLong(const char* field, const char* value, struct Buf* out);
static int doMdsPutULL(const char* field, const char* value, struct Buf* out);
static int doMdsPutCString(const char* field, const char* value, struct Buf* out);
struct MdsPutDescriptor {
	char* key;
	int id;
	struct descrip desc;
	const char* scanfmt;
	int element_size;
	int dims[4];
	int ndims;
	int len;
	int max_nelems;
	struct Buf* out;
	int (*putScalar)(const char* field, const char* value, struct Buf* out);
};

static void makeDims(
		struct descrip* valss, int dtype, void* ptr,
		int ndims, int dims[], int nelems, int size);

static int getImmediateData(struct MdsPutDescriptor* pd, const char* filedef)
{
	const char* first = filedef+1;
	char fmt[64];
	void* dst = pd->desc.ptr;

	dbg(2, "01");
	pd->dims[0] = 0;

	/** example: "%ld%*[],; ]%n" */
	strcpy(fmt, pd->scanfmt);
	strcat(fmt, "%*[]" LIST_SEPS "]%n");

	int convert_chars = 0;

	while (sscanf(first, fmt, dst, &convert_chars) > 0){
		assert(convert_chars != 0);

		dst += pd->element_size;
		first += convert_chars;
		++pd->dims[0];
	}

	dbg(2, "dims[%d]", pd->dims[0]);

	return pd->dims[0];
}

static int _getFileData(
	struct MdsPutDescriptor* pd, const char* filedef, 
	void* dest, int nread)
{
	FILE* fp = fopen(filedef, "r");
	int rc;

	dbg(3, "process file %s", filedef);
	if (!fp){
		iobPrintf(pd->out, "ERROR: failed to open %s\n", filedef);
		return -1;
	}
	rc = fread(dest, pd->element_size, nread, fp);
	fclose(fp);

	dbg(2, "process file %s returns %d", filedef, rc);

	return rc;
}

static int getFileData(struct MdsPutDescriptor* pd, const char* filedef)
/** returns # elements */
{
	const char* end;

	if ((end = strpbrk(filedef, LIST_SEPS)) == 0){
		return _getFileData(pd, fname(filedef), 
				    pd->desc.ptr, pd->max_nelems);
	}else{
		/* foreach file in filedef read... */
		int nelems = 0;

		char thisfile[MAXFILE+1];
		const char* start = filedef;
		int rc;
		void* dest = pd->desc.ptr;

		
		do {
			if ((end = strpbrk(start, LIST_SEPS)) == 0){
				end = start + strlen(start);
			}
			if (end - start >= MAXFILE){
				iobPrintf(pd->out, "WARNING:fname too long\n");
				continue;
			}
			strncpy(thisfile, start, end - start);
			thisfile[end-start] = '\0';
			rc = _getFileData(pd, fname(thisfile), dest, 
					  pd->max_nelems - nelems);
			if (rc < 0){
				iobPrintf(pd->out, "ERROR: read failed %s\n",
					  fname(thisfile));
				return -1;
			}else{
				dest += rc * pd->element_size;
				nelems += rc;
			}
			if (*end){
				start = end + 1;
			}
		} while(*start && *end && nelems < pd->max_nelems);

		return nelems;
	}
}
static int getPipeData(struct MdsPutDescriptor* pd, const char* pipedef)
/** returns # elements */
{
	/** if it's an immediate array decode it, else read from pipe */
	if (pipedef[0] == IMMEDIATE_IDENT){
		return getImmediateData(pd, pipedef);
	}else{
		int rc;
		FILE* datapipe =  popen(pipedef, "r");

		if (!datapipe){
			iobPrintf(pd->out, "ERROR failed popen %s\n", pipedef);
			return -1;
                }
		rc = fread(pd->desc.ptr, pd->element_size, pd->len, datapipe);
		fclose(datapipe);
		return rc;
	}
}

static int total_dims(struct MdsPutDescriptor* pd)
{
	int id;
	int total = 1;
	for (id = 0; id != pd->ndims; ++id){
		if (pd->dims[id]){
			total *= pd->dims[id];
		}
	}
	return pd->max_nelems = total;
}
static int fillData(
	struct MdsPutDescriptor* pd,
	int (*getData)(struct MdsPutDescriptor* pd, const char* value),
	const char* value)
{
	int length = pd->element_size * total_dims(pd);
	void *ptr = malloc(length);
	if (!ptr){
		iobPrintf(pd->out, "ERROR: failed to malloc %d\n", length);
		return -1;
	}
	dbg(3, "buffer %p len %d", ptr, length);

	pd->desc.ptr = ptr;
	pd->len = length;
	
	int nelems = getData(pd, value);
	if (nelems <= 0){
		iobPrintf(pd->out, "ERROR: failed to read %s\n", value);
		return -1;
	}

	makeDims(&pd->desc, DTYPE_NONE, ptr,
			pd->ndims, pd->dims, nelems, pd->element_size);
	dbg(3, "returns %d", nelems);
	return nelems;
}



static void freeData(struct MdsPutDescriptor* pd)
{
	if (pd->desc.ptr){
		free(pd->desc.ptr);
	}
}


static int serverHelp(
	poptContext opt_context,
	struct Buf* out,
	void (*poptHelp)(poptContext con, FILE * f, int flags)
	)
{
	char fn[] = "/tmp/mdsshell_helpXXXXXX";
	int id = mkstemp(fn);
	FILE *fd = fdopen(id, "w");
	char *cp;

	poptHelp(opt_context, fd, 0);
	fclose(fd);
	close(id);
	
	fd = fopen(fn, "r");
	assert(fd);

	while((cp = fgets(out->buf+out->len, out->maxlen-out->len, fd)) != 0){
		out->len += strlen(cp);
	}
	fclose(fd);
	unlink(fn);

	return 0;
}


#define MAXARGS 40

#define DUMP_SHORTS "/tmp/shorts"



static void dumpData(FILE* fp, struct descrip* ans, int elt_size)
{
	int id;
	int len = 1;

	for (id = 0; ans->dims[id]; ++id){
		len *= ans->dims[id]; //calc # of elements in array
	}
	dbg(2, "fwrite(%p, %d, %d, %p)", 
	    ans->ptr, elt_size, len, fp);

	fwrite(ans->ptr, elt_size, len, fp);
}

static void dumpOctaveBuf(
	struct Buf* out, 
	struct descrip *ans, 
	int numcols, int numrows)
/* thanks SJM! */
{
#define FVAL(row, col) (((float *)ans->ptr)[(row)*numcols + (col)])

	if (ans->ndims <= 1 && numcols <= 1 && numrows <= 1){
		/* SCALAR */
		iobPrintf(out, "%g\n", FVAL(0, 0));	
	}else{
		/* ARRAY */
		unsigned row, col;  

		iobPrintf(out,"[");

		for (row = 0; row < numrows; row++){
			for (col = 0; col < numcols; col++){
				iobPrintf(out, "%g", FVAL(row, col));

				if (col <= numcols){    //signify new colum
					iobPrintf(out,", ");
				}
			}
			if (row <= numrows){    //signify new row
				iobPrintf(out,"; ");
			}
		}

		iobPrintf(out,"]\n");
	}
#undef FVAL
}



static void dumpOctaveFile(
	FILE *file, struct descrip* ans, 
	int numcols, int numrows)
{
	unsigned row, col; 
	float *fp = (float *)ans->ptr;
	int is_array = ans->ndims > 0;

	if (is_array){
		fprintf(file,"[");
	}

	for (row = 0; row < numrows; ++row){ 
		for (col = 0; col < numcols; ++col){ 
			fprintf(file, "%g", fp[row*numcols + col]); 
			if (col < numcols-1){    
				fprintf(file, ", ");
			}
		}

		if (row < numrows-1){
			fprintf(file, "; ");
		}
	}

	fprintf(file, is_array? "]\n": "\n");
}

static char* mds_put_expr = "$";

static int doMdsValue(int argc, const char *argv[], struct Buf* out)
/**
 *  we confine ourselves to floats for the moment
 */
{
	static jmp_buf env;
	char* outfile = "/tmp/mdsvalue.dat";
	char* format = "binary";
	int timeout = 0;


	struct poptOption opt_table[] = {
		{ "output", 'o', POPT_ARG_STRING, &outfile, 'o' },
		{ "format", 'f', POPT_ARG_STRING, &format, 'f' },
		{ "timeout", 't', POPT_ARG_INT, &timeout, 't' },
		{ "help",   'h', POPT_ARG_NONE,  0, 'h' },
		{ "usage",   'u', POPT_ARG_NONE,   0, 'u' },
		POPT_TABLEEND
	};
	

	char expr[256];
	const char* parg;
	unsigned guard1 = 0xdeadbeef;
	struct descrip ans = {DTYPE_FLOAT, 0};
	unsigned guard2 = 0xdeadbeef;  //Not a carnivore, Peter?  -SJM
	unsigned numrows, numcols;

	int status;

	poptContext opt_context = 
		poptGetContext(argv[0], argc, argv, opt_table, 0);
	FILE* outfp = 0;
	int rc;
	int id;

	while ((rc = poptGetNextOpt(opt_context)) > 0){
		switch(rc){
		case 'o':
			if (strcmp(outfile, "-") == 0){
				outfp = stdout;
				break;
			}
			outfp = fopen(outfile, "w");
			if (!outfp){
				iobPrintf(out, 
					  "ERROR unable to open file "
					  "\"%s\" errno:%d\n", outfile, errno);
				return -1;
			}
			break;
		case 'h':
			return serverHelp(opt_context, out, poptPrintHelp);
		case 'u':
			return serverHelp(opt_context, out, poptPrintUsage);
		default:
			;
		}
	}

	for (expr[0] = '\0'; (parg = poptGetArg(opt_context)) != 0; ){
		strcat(expr, parg);
		strcat(expr, " ");
		assert(strlen(expr) < sizeof(expr));
	}

	if (timeout){
		dbg(1, "set_timeout(%d)", timeout);
		set_timeout(timeout, &env);

		if (sigsetjmp(env, 1) != 0){
			err("timeout");
			/* timeout */
			DisconnectFromMds(S_sock);
			S_sock = 0;
			iobPrintf(out, "ERROR:TIMEOUT\n");
			return -1;
		}
	}
	status = MdsValue(S_sock, (char *)expr, &ans, 0);

	assert(guard1 == guard2 && guard2 == 0xdeadbeef);

	if (timeout){
		dbg(1, "set_timeout(0)");
		set_timeout(0, 0);
	}

	if (status&1){
		//begin SJM debugging / investigation:
		dbg(1, "Current length is %d\n",ans.length); //SJM
		dbg(1, "Current number of dimensions is %d\n",ans.ndims); //SJM
		if (ans.ndims > 0){   //SJM - print dimensions of array
			for (id = 0; id != ans.ndims; ++id){    //SJM
				dbg(1, "[%d] ", ans.dims[id]);  //SJM
			}   //SJM
			dbg(1,"\n"); //SJM
		}   //SJM
		//end SJM debugging / investigation
		switch(ans.ndims){
		case 0:
			numcols=1;
			numrows=1;
			break;
		case 1:
			numcols=ans.dims[0];
			numrows=1;
			break;
		case 2:
			numcols=ans.dims[0];
			numrows=ans.dims[1];
			break;
		default:
			numcols=ans.dims[0];
			numrows=ans.dims[1];
			printf("WARNING: Don\'t know how to format more than 2D data.\n");
			break;
		}
		switch(ans.dtype){
		case DTYPE_FLOAT:
			dbg(1,"Recognize DTYPE_FLOAT\n");
			if (strcmp(format, "binary") == 0){
				if (outfp){
					dumpData(outfp, &ans, sizeof(float)*numcols*numrows);
				}
			}else if (strcmp(format, "octave") == 0){
				if (outfp == 0){
					dumpOctaveBuf(out, &ans, numcols, numrows);
				}else{
					dumpOctaveFile(outfp, &ans, numcols, numrows);
				}
			}else{
				dumpOctaveBuf(out, &ans, numcols, numrows);	
			}
			if (acq200_debug){
				iobPrintf(out, "FLOAT: [%d,%d]\n", 
							numrows, numcols);
			}
			break;
		case DTYPE_CSTRING:
			dbg(1,"Recognize DTYPE_CSTRING\n");
			if (outfp){
				dumpData(outfp, &ans, strlen((char *)ans.ptr));
			}
			iobPrintf(out, "%s\n", (char*)ans.ptr);
			break;
		case DTYPE_SHORT: 
			dbg(1,"Recognize DTYPE_SHORT\n");
			if (outfp){
				dumpData(outfp, &ans, sizeof(short));
			}

			iobPrintf(out, "DTYPE_SHORT, dim:%d ", ans.ndims);

			for (id = 0; id != ans.ndims; ++id){
				iobPrintf(out, "[%d] ", ans.dims[id]);
			}
			iobPrintf(out, "length %d %s\n", 
				  ans.length, outfp? outfile: "");
			break;
		case DTYPE_LONG:
			dbg(1,"Recognize DTYPE_LONG\n");
			if (outfp){
				dumpData(outfp, &ans, sizeof(long));
			}
			iobPrintf(out, "%ld\n", *(long*)ans.ptr);
			break;
		case DTYPE_ULONG:
			dbg(1,"Recognize DTYPE_ULONG\n");
			if (outfp){
				dumpData(outfp, &ans, sizeof(unsigned long));
			}
			iobPrintf(out, "%uld\n", *(unsigned long*)ans.ptr);
			break;
		case DTYPE_UCHAR:
			dbg(1,"Recognize DTYPE_UCHAR\n");
			if (outfp){
				dumpData(outfp, &ans, sizeof(unsigned char));
			}
/** uchar usually from setEvent ... point NOT valid */
			iobPrintf(out, "DTYPE_UCHAR:%p\n", ans.ptr);
/*
  iobPrintf(out, "%c\n", *(unsigned char*)ans.ptr);
*/
			break;
			
		default:
			iobPrintf(out, "Unable to print this DTYPE %d\n", 
				  ans.dtype);
			break;
		}
	}else{
		iobPrintf(out, "ERROR MdsValue( %s ) FAILED\n", expr);
	}


	if (outfp){
		fclose(outfp);
	}
	dbg(2,"Leaving doMdsValue %s", outfp? "CLOSE":"");
	return (status&1) ? 0: -1;
}

static int doMdsPutError(const char* field, const char* value, struct Buf* out)
{
	iobPrintf(out, "ERROR scalar format not supported\n");
	return -1;
}

static int doMdsPutFloat(const char* field, const char* value, struct Buf* out)
{
	struct descrip vald = {.dtype = DTYPE_FLOAT, 0};
	int status;
	float fval = (float)atof(value);

	vald.ptr = (void *)&fval;

	dbg(2, "MdsPut(%ld \"%s\", \"$\" float %g\n",
	    S_sock, field, fval);

	status = MdsPut(S_sock, (char *)field, mds_put_expr, &vald,0); 
	if (status&1){
		iobPrintf(out, "mdsPut( %s ) = %g OK\n", 
			  field, *(float*)vald.ptr);	
		return 0;
	}else{
		iobPrintf(out, "mdsPut( %s ) = %g FAIL\n", 
			  field, *(float*)vald.ptr);
		return -1;
	}
}

static int doMdsPutLong(const char* field, const char* value, struct Buf* out)
{
	struct descrip vald = {.dtype = DTYPE_LONG, 0};
	int status;
	long lval = atol(value);
	vald.ptr = (void *)&lval;

	dbg(2, "MdsPut(%ld \"%s\", \"$\" long %ld\n",
	    S_sock, field, lval);

	status = MdsPut(S_sock, (char *)field, mds_put_expr,&vald,0); 
	if (status&1){
		iobPrintf(out, "mdsPut( %s ) = %ld OK\n", 
			  field, *(long*)vald.ptr);	
		return 0;
	}else{
		iobPrintf(out, "mdsPut( %s ) = %ld FAIL\n", 
			  field, *(long*)vald.ptr);
		return -1;
	}
}

static int doMdsPutULL(const char* field, const char* value, struct Buf* out)
{
	struct descrip vald = {0};
	int status;
	unsigned long long lval;

	if (sscanf(value, "%llu", &lval) != 1){
		return -2;
	}
	vald.ptr = (void *)&lval;

	makeDims(&vald, DTYPE_ULONGLONG, &lval, 1, 0, 1, sizeof(lval));

	dbg(2, "MdsPut(%ld \"%s\", \"$\" ull %llu\n",
	    S_sock, field, lval);

	status = MdsPut(S_sock, (char *)field, mds_put_expr,&vald,0); 
	if (status&1){
		iobPrintf(out, "mdsPut( %s ) = %llu OK\n",
			  field, *(unsigned long long*)vald.ptr);	
		return 0;
	}else{
		iobPrintf(out, "mdsPut( %s ) = %llu FAIL\n",
			  field, *(unsigned long long*)vald.ptr);
		return -1;
	}
}

static int doMdsPutCString(
	const char* field, const char* value, struct Buf* out)
{
	struct descrip vals = {.dtype = DTYPE_CSTRING, 0};
	int status;

	vals.length = strlen(value);
	vals.ptr = (void*)value;

	status = MdsPut(S_sock, (char *)field, mds_put_expr, &vals, 0);

	if (status&1){
		iobPrintf(out, "mdsPut( %s ) = %s OK\n", 
			  field, (char*)vals.ptr);	
		return 0;
	}else{
		iobPrintf(out, "mdsPut( %s ) = %s FAIL\n", 
			  field, (char*)vals.ptr);
		return -1;
	}
	return -1;
}

static int shuffle_up(
	void* ptr,
	int oldmajor, int offset, int newmajor,
	int old_len, int esize)
{
	void *dst = ptr;
	void *src = ptr + esize*offset;

	assert(sizeof(void) == 1);

	while (src - ptr < old_len){
		memcpy(dst, src, newmajor*esize);
		src += oldmajor * esize;
		dst += newmajor * esize;
	}

	return (src - ptr) * esize;
}
	
	
int elt_size(void) {
	return sizeof(short);            /** @@WORKTODO */
}

#define FULL_ARRAY "0,*"

static int MdsPutArray(int sock, 
		       /* const */ char* field,
		       /* const */ char* expr, 
		       struct descrip* descrip, 
		       const char* subdef
	)
{
	if (strcmp(subdef, FULL_ARRAY) == 0){	

		dbg(3, "MdsPut %d %s %s %p, ptr %p",
		    sock, field, expr, descrip, descrip->ptr);

		return MdsPut(sock, field, expr, descrip, 0);
	}else{
		int offset, newmajor;
		struct descrip newdescrip = *descrip;

		if (sscanf(subdef, "%d,%d", &offset, &newmajor) == 2){
			int oldmajor = descrip->dims[0];
			newdescrip.dims[0] = newmajor;

			dbg(2, "oldmajor %d newmajor %d offset %d", 
			    oldmajor, newmajor, offset);

			newdescrip.length = shuffle_up(
				descrip->ptr, 
				oldmajor, offset, newmajor, descrip->length,
				elt_size());

			dbg(2, "newdescrip length %d", newdescrip.length);

			return MdsPut(sock, field, expr, &newdescrip, 0);
		}else{
			err("failed to parse subdef %s", subdef);
			return 1;
		}
	}
}
static void makeDims(
		struct descrip* valss, int dtype, void* ptr,
		int ndims, int dims[], int nelems, int size)
{
	if (dtype != DTYPE_NONE) valss->dtype = dtype;
	valss->ptr = ptr;
	valss->ndims = ndims;
	valss->length = nelems * size;

	if (ndims == 1){
		valss->dims[0] = nelems;
	}else{
		int dims_len = 1;		
		int last_dim;
		int id;

		for (id = 0; id < ndims - 1; ++id){
			dims_len *= dims[id]; 
			valss->dims[id] = dims[id];
		}

		if (dims_len >= nelems){
			last_dim = 1;
		}else{
			last_dim = nelems/dims_len;
		}
				
		if (last_dim < dims[id]){
			info("Adjusting dim[%d] from %d to %d",
			     id, dims[id], last_dim );
		}

		valss->dims[id] = last_dim;


		for (id = 0; id < ndims; ++id){
			dbg(2, "dims[%d] = %d", id, valss->dims[id]);
		}
	}
}





static void setDim(int dims[], int id, int len) {
	if (len == 0){
		len = 0x10000;
	}
	dbg(3, "dims[%d] = %d", id, len);
	dims[id] = len;
}

static int getDims(const char* dimdef, int dims[], int maxdims)
/* dimdef: n,n,n,n, */
{
	char* buf = malloc(strlen(dimdef));
	int id;
	char* starts;
	char* ends;

	strcpy(buf, dimdef);

	for (id = 0, starts = buf; (ends = strchr(starts, ',')); ++id){
		*ends = '\0';
		setDim(dims, id, atoi(starts));
		starts = ends + 1;
	}
	setDim(dims, id, atoi(starts));

	free(buf);
	return id+1;
}


enum FORMAT_SEL { 
	FS_ERR, FS_FLOAT, FS_CSTRING, FS_SHORT, FS_LONG, FS_ULONG,
        FS_USHORT, FS_ULL};

#define PDI(_key, _fs, typ, siz, fmt, puts) { 	\
		.key = _key,			\
			.id = _fs,		\
			.desc.dtype = typ,	\
			.element_size = siz,	\
			.scanfmt = fmt,		\
			.putScalar = puts }


static const struct MdsPutDescriptor templates [] = {
	PDI("float", FS_FLOAT, DTYPE_FLOAT, 
		sizeof(float), "%f", doMdsPutFloat),
/** CSTRING looks bogus */
	PDI("string", FS_CSTRING, DTYPE_CSTRING, 
		sizeof(char), "%c", doMdsPutCString),	       
	PDI("short",  FS_SHORT,   DTYPE_SHORT, 
	    sizeof(short), "%hd", doMdsPutError),
/** backcompat ... */
	PDI("shorts",  FS_SHORT,   DTYPE_SHORT, 
	    sizeof(short), "%hd", doMdsPutError),
	PDI("long",   FS_LONG,    DTYPE_LONG,  
	    sizeof(long), "%ld", doMdsPutLong),
	PDI("ulong",  FS_ULONG,   DTYPE_ULONG, 
	    sizeof(unsigned long), "%lu", doMdsPutError),
	PDI("ushort", FS_USHORT,  DTYPE_USHORT, 
	    sizeof(short), "%hu", doMdsPutError),
	PDI("ull", FS_ULL, DTYPE_ULONGLONG, 
	    sizeof(unsigned long long), "%ull", doMdsPutULL)
};
#define NTEMPLATES (sizeof(templates)/sizeof(struct MdsPutDescriptor))

static const struct MdsPutDescriptor* lookupFormat(
	const char* key, struct MdsPutDescriptor* putdesc)
{
	int ikey;
	const struct MdsPutDescriptor* pd = templates;

	for (ikey = NTEMPLATES; ikey--; ++pd){
		int match = strcmp(pd->key, key) == 0;

		dbg((match? 2: 3), "strcmp %s %s %s", pd->key, key, 
		    match? "we have a match": "");

		if (strcmp(pd->key, key) == 0){
			if (putdesc){
				memcpy(putdesc, pd, 
				       sizeof(struct MdsPutDescriptor));

				return putdesc;
			}
			return pd;
		}
	}
	return 0;
}

static void helpFormat(struct Buf* out){
	int ikey;
	const struct MdsPutDescriptor* pd = templates;

	for (ikey = NTEMPLATES; ikey--; ++pd){
		iobPrintf(out, "%s [size:%d]\n", pd->key, pd->element_size);
	}
}
static int doMdsPut(int argc, const char *argv[], struct Buf* out)
{
	struct MdsPutDescriptor putdesc;

	struct SwitchArgs {
		char* format_select;
		int length;
		char* dimdef;
		char* file_def;
		char* subdef;      /* offset, # elements */
	};
	static struct SwitchArgs switch_defaults = {
		.format_select = "short",
		.length = 1,
		.dimdef = "",
		.file_def = 0,
		.subdef = FULL_ARRAY,
	};
	/* has to be static for opt_table to work */
	static struct SwitchArgs sw;

	static struct poptOption opt_table[] = {
		{ "format", 'F', POPT_ARG_STRING, &sw.format_select, 'F' },
		{ "length", 'l', POPT_ARG_INT,    &sw.length, 'l' },
		{ "dim",    'd', POPT_ARG_STRING, &sw.dimdef, 'd' },
		{ "subarray", 's', POPT_ARG_STRING, &sw.subdef, 's' },
		{ "expr",   'e', POPT_ARG_STRING, &mds_put_expr, 'e' },
		{ "file",   'f', POPT_ARG_STRING, &sw.file_def, 'f' },
		{ "root",   't', POPT_ARG_STRING, &froot, 't' },
		{ "help",   'h', POPT_ARG_NONE,   0, 'h' },
		{ "usage",   'u', POPT_ARG_NONE,   0, 'u' },
		POPT_TABLEEND
	};

	memcpy(&sw, &switch_defaults, sizeof(sw));
	poptContext opt_context = 
		poptGetContext(argv[0], argc, argv, opt_table, 0);
	int rc;
	const char* field;
	const char* value;
	int ndims = 0;
	int dims[MAXDIMS]; 
	const struct MdsPutDescriptor* template = 
		lookupFormat("float", &putdesc);

	dbg(3,"here with argc=%d argv[0] \"%s\", argv[1] \"%s\"",
	    argc, argv[0], argc>1? argv[1]: "");

	while ((rc = poptGetNextOpt(opt_context)) > 0){
		switch(rc){
		case 'F':
			if ((template = 
			     lookupFormat(sw.format_select, &putdesc)) == 0){
				err("format not found");
				helpFormat(out);
				return -1;
			}
			break;
		case 'l':
			ndims = 1;
			setDim(dims, 0, sw.length);
			break;
		case 'd':
			ndims = getDims(sw.dimdef, dims, MAXDIMS);
			break;
/*
 * we dont use POPT_ARG_AUTOHELP because it calls exit().
 * not good, because this is the server thread ...
 * this aint ideal 'cos the help prints to SERVER STDERR, but too bad
 */
		case 'h':
			return serverHelp(opt_context, out, poptPrintHelp);
		case 'u':
			return serverHelp(opt_context, out, poptPrintUsage);
		default: 
			;
		}
	}	



	field = poptGetArg(opt_context);
	if (!field){
		iobPrintf(out, "ERROR must supply field\n");
		return -1;
	}

	value = poptGetArg(opt_context);
	if (!value && sw.file_def == 0){
		iobPrintf(out, "ERROR must supply value\n");
		return -1;
	}

	dbg(2, "ndims %d field %s value %s", ndims, field, value);

	if (ndims == 0){
		return template->putScalar(field, value, out);
	}else if (ndims >= MAXDIMS){
		iobPrintf(out, "ERROR maxdims %d\n", MAXDIMS);
		return -1;
	}else{
		void (*cleanupDesc)(struct MdsPutDescriptor* pd) = freeData;
		int id;
		putdesc.ndims = ndims;
		for (id = 0; id != ndims; ++id){
			putdesc.dims[id] = dims[id];
		}
		putdesc.out = out;

		if(sw.file_def != 0){
			rc = fillData(&putdesc, getFileData, sw.file_def);
		}else{
			rc = fillData(&putdesc, getPipeData, value);
		}

		if (rc > 0){
			dbg(2, "MdsPut %s", field);

			rc = MdsPutArray(S_sock, (char *)field, 
					 mds_put_expr, &putdesc.desc, sw.subdef);
			if (rc&1){
				iobPrintf(out, "mdsPut( %s ) OK\n", field);
				rc = 0;
			}else{
				iobPrintf(out, "mdsPut( %s ) FAIL\n", field);
				rc = -1;
			}
		}else{
			iobPrintf(out, "mdsPut aborted");
			rc = -1;
		}

		cleanupDesc(&putdesc);
		return rc;
	}
}

#define MAXEXPR	4096
#define MAXK	3


struct DescripWrapper {
	struct descrip descrip;
	int nelems;
	union {
		unsigned long long ull;
		void *data;
	} payload;
};


FILE *getInputFp(const char *ch_str, int *flen)
{
	struct stat sb;
	FILE *fp;
	if (stat(ch_str, &sb) == 0){
		fp = fopen(ch_str, "r");
		*flen = sb.st_size;
		return fp;
	}else{
		return 0;
	}
}


struct Window {
	unsigned start_sample;
	unsigned length_samples;
	unsigned sample_size;
};

#define HAS_WINDOW(w) ((w)->length_samples!=0)


FILE *getDataFp(int chn, int *flen, struct Window* ww)
{
	char fname[128];
	struct stat sb;
	FILE *fp;

	sprintf(fname, "/dev/acq200/data/%02d", chn);

	dbg(2, "fname:%s", fname);

	if (stat(fname, &sb) == 0){
		fp = fopen(fname, "r");
		*flen = sb.st_size;

		if (HAS_WINDOW(ww)){
			unsigned start_byte = ww->start_sample*ww->sample_size;
			if (fseek(fp, start_byte, SEEK_SET) != 0){
				perror("seek failed");
			}else{
				unsigned rembytes = sb.st_size - start_byte;
				unsigned reqbytes = ww->sample_size*ww->length_samples;

				*flen = MIN(rembytes, reqbytes);
			}
			dbg(2, "windowed: start:%d len:%d", start_byte, *flen);
		}
		return fp;
	}else{
		return 0;
	}
}

/** @todo ch1_type not used yet : must be short */
static struct DescripWrapper* build_channel_descrip(
		int chn, const char *ch1_type, struct Window* ww)
{
	struct DescripWrapper *chx = calloc(sizeof(struct DescripWrapper), 1);
	int flen;
	FILE* fp_data = getDataFp(chn, &flen, ww);
	int nread;

	if (!fp_data){
		perror("failed to open channel");
		exit(errno);
	}
	chx->nelems = flen/sizeof(short);
	chx->payload.data = malloc(flen);
	nread = fread(chx->payload.data, sizeof(short), chx->nelems, fp_data);
	fclose(fp_data);

	dbg(2, "payload: %d items", nread);

	makeDims(&chx->descrip, DTYPE_SHORT, chx->payload.data,
			1, 0, nread, sizeof(short));
	return chx;
}

typedef unsigned long long tbtype;


static struct DescripWrapper *build_timebase_descrip(
		int nelems, struct DescripWrapper *kx)
{
	struct DescripWrapper* tbx = calloc(sizeof(struct DescripWrapper), 1);
	tbtype* time_cursor;
	tbtype tt = kx[0].payload.ull;
	unsigned dt = kx[2].payload.ull;

	tbx->nelems = nelems;
	tbx->payload.data = malloc(nelems*sizeof(tbtype));

	for (time_cursor = (tbtype *)tbx->payload.data; nelems--; tt += dt){
		*time_cursor++ = tt;
	}

	makeDims(&tbx->descrip, DTYPE_ULONGLONG, tbx->payload.data,
			1, 0, tbx->nelems, sizeof(unsigned long long));
	return tbx;
}
/** @todo k234_type not used yet : must be ulong */
static struct DescripWrapper* build_k234_descrip(
		const char* k234_str,
		const char* k234_type,
		int *nelems)
{
	struct DescripWrapper *kx = calloc(sizeof(struct DescripWrapper), MAXK);
	int ik;

	int nscan = sscanf(k234_str, "%llu,%llu,%llu",
			&kx[0].payload.ull,
			&kx[1].payload.ull,
			&kx[2].payload.ull);


	for (ik = 0; ik != MAXK; ++ik){
		makeDims(&kx[ik].descrip, DTYPE_ULONGLONG, &kx[ik].payload.ull,
				1, 0, 1, sizeof(unsigned long long));
	}

	if (nelems) *nelems = nscan;
	return kx;
}

static struct DescripWrapper* copy_k234_descrip(struct DescripWrapper* kfrom)
{
	struct DescripWrapper *kto = calloc(sizeof(struct DescripWrapper), MAXK);
	int ik;

	memcpy(kto, kfrom, sizeof(struct DescripWrapper) * MAXK);

	for (ik = 0; ik != MAXK; ++ik){
		makeDims(&kto[ik].descrip, DTYPE_ULONGLONG, &kto[ik].payload.ull,
				1, 0, 1, sizeof(unsigned long long));
	}

	return kto;
}
static void destroyDescrip(struct DescripWrapper *descripWrapper)
{
	if (descripWrapper->nelems > 1){
		free(descripWrapper->payload.data);
	}
	free(descripWrapper);
}

#define MAXCHAN	96

int timeout = 0;
FILE* outfp = 0;

static int _doMdsValuePut(
		char* expr, int chn, struct DescripWrapper *chx, struct DescripWrapper *kx,
		struct Buf* out)
{
	static char *expr2;
	unsigned guard1 = 0xdeadbeef;
	struct descrip ans = {};
	unsigned guard2 = 0xdeadbeef;
	int status;

	if (!expr2) expr2 = malloc(MAXEXPR);	/** alloc and hold for duration */

	sprintf(expr2, expr, chn);

	dbg(2, "calling MdsValue");
	status = MdsValue(
			S_sock, expr2,
			&chx->descrip,
			&kx[0].descrip, &kx[1].descrip, &kx[2].descrip,		/* MAXK args */
			&ans, 0);

	dbg(2, "back from MdsValue status:%d", status);

	assert(guard1 == guard2 && guard2 == 0xdeadbeef);

	if (timeout){
		dbg(1, "set_timeout(0)");
		set_timeout(0, 0);
	}

	if (status&1){
		switch(ans.dtype){
		case DTYPE_LONG:
			dbg(1,"Recognize DTYPE_LONG\n");
			if (outfp){
				dumpData(outfp, &ans, sizeof(long));
			}
			iobPrintf(out, "%ld\n", *(long*)ans.ptr);
			break;
		default:
			iobPrintf(out, "Unable to print this DTYPE %d\n",
				  ans.dtype);
			break;
		}
	}else{
		iobPrintf(out, "ERROR MdsValue( %s ) FAILED\n", expr2);
	}

	return status;
}

static void makeWindow(
		const char* windef, struct Window* window, const char* c_type)
{
	if (strcmp(c_type, "short") == 0){
		window->sample_size = sizeof(short);
	}else{
		err("ctype not supported, chosing short");
		window->sample_size = sizeof(short);
	}
	sscanf(windef, "%u,%u", &window->start_sample, &window->length_samples);
}

struct MVP {
	char *ch1_type;
	int channels[MAXCHAN+1];
	int timebase_requested;
	char* expr;
};

static void init_mvp(struct MVP* mvp, char* expr) {
	mvp->ch1_type = "short";
	memset(&mvp->channels, 0, sizeof(mvp->channels));
	mvp->timebase_requested = 0;
	mvp->expr = expr;
}
int doMdsValuePutSegment(
		struct MVP* mvp,
		struct DescripWrapper *kx,
		struct Window* window,
		struct Buf* out)
{
	int nelems = 0;
	int status = 0;
	int ic;

	dbg(2, "01");
	for (ic = 1; ic < MAXCHAN; ++ic){
		if (mvp->channels[ic]){
			struct DescripWrapper *chx =
					build_channel_descrip(ic, mvp->ch1_type, window);
			if (nelems == 0){
				nelems = chx->nelems;
			}
			status = _doMdsValuePut(mvp->expr, ic, chx, kx, out);
			destroyDescrip(chx);
			if ((status&1) == 0){
				break;
			}
		}
	}

	dbg(2, "50");

	if (mvp->timebase_requested){
		struct DescripWrapper *tbx = build_timebase_descrip(nelems, kx);
		status = _doMdsValuePut(mvp->expr, 0, tbx, kx, out);
		destroyDescrip(tbx);
		if ((status&1) == 0){
			err("timebase put failed");
		}
	}
	dbg(2, "99");
	return status;
}
static int doMdsValuePut(int argc, const char *argv[], struct Buf* out)
/**
 *  we confine ourselves to floats for the moment
 */
{
	static jmp_buf env;
	static char *expr;

	char* infile = "/tmp/mdsvalue.dat";
	char* k234_type = "ulong";			/* MDSPLUS TYPE for const 		*/
	char *ch1 = "1";							/* $1 in EXPR 					*/
	char *k234 = "0,0,0";					/* $2,$3,$4 in EXPR 			*/
	struct Window window = {};
	char *windef;
	struct MVP mvp;
	int nsegs = 0;

#define DATA_DESCRIP	0
	struct DescripWrapper *kx;

	struct poptOption opt_table[] = {
		{ "data", 		'd', POPT_ARG_STRING, 	&infile, 	'd',
				"data from file [not implemented]"				},
		{ "ch1",  		'c', POPT_ARG_STRING, 	&ch1, 		'c',
				"channel spec subs expr arg $1 eg -c 1:96"		},
		{ "k234", 		'k', POPT_ARG_STRING, 	&k234, 		'k',
				"constant spec subs expr $2,$3,$4"				},
		{ "tb_ch00",    'T', POPT_ARG_NONE,     0,    		 'T',
				"timebase expression - specific timebase in u64"},
		{ "ch1_type",   0, POPT_ARG_STRING, 	&mvp.ch1_type,    0,
				"type of raw data [ushort (only)]"				},
		{ "k234_type",  0, POPT_ARG_STRING, 	&k234_type,   0,
				"type of constant [ull (only)]"					},
		{ "timeout", 	't', POPT_ARG_INT, 		&timeout, 	't',
				"timeout in s [0 : off]"						},
		{ "nsegs",        0, POPT_ARG_INT,     &nsegs,        0 },
		{ "window",     'w', POPT_ARG_STRING,  &windef,     'w' },
		{ "help",   	'h', POPT_ARG_NONE,  	0, 			'h' },
		{ "usage",   	'u', POPT_ARG_NONE,   	0, 			'u' },
		POPT_TABLEEND
	};

	const char* parg;
	int status = 0;
	FILE* infp = 0;
	int rc;
	int nchannels = 0;
	int make_window_requested = 0;
	int k234_nelems = 0;

	if (!expr) expr = malloc(MAXEXPR);		/* assign and hold for duration */
	init_mvp(&mvp, expr);

	dbg(1, "hello:argc=%d", argc);

	poptContext opt_context = poptGetContext(argv[0], argc, argv, opt_table, 0);

	while ((rc = poptGetNextOpt(opt_context)) > 0){
		switch(rc){
		case 'c':
			dbg(2, "call acqMakeChannelRange \"%s\"", ch1);
			nchannels = acqMakeChannelRange(mvp.channels, MAXCHAN, ch1);
			dbg(2, "nchannels=%d", nchannels);
			break;
		case 'T':
			mvp.timebase_requested = 1;
			break;
		case 'd':
			if (strcmp(infile, "-") == 0){
				infp = stdin;
				break;
			}
			infp = fopen(infile, "r");
			if (!infp){
				iobPrintf(out,
					  "ERROR unable to open file "
					  "\"%s\" errno:%d\n", infile, errno);
				return -1;
			}
			break;
		case 'w':
			make_window_requested = 1;

			break;
		case 'h':
			return serverHelp(opt_context, out, poptPrintHelp);
		case 'u':
			return serverHelp(opt_context, out, poptPrintUsage);
		default:
			;
		}
	}
	if (make_window_requested){
		makeWindow(windef, &window, mvp.ch1_type);
	}


	dbg(2, "build_k234_descrip");

	kx = build_k234_descrip(k234, k234_type, &k234_nelems);

	for (expr[0] = '\0'; (parg = poptGetArg(opt_context)) != 0; ){
		strcat(expr, parg);
		strcat(expr, " ");
		assert(strlen(expr) < MAXEXPR);
	}

	dbg(2, "expr aggregate:\"%s\"", expr);

	if (timeout){
		dbg(1, "set_timeout(%d)", timeout);
		set_timeout(timeout, &env);

		if (sigsetjmp(env, 1) != 0){
			err("timeout");
			/* timeout */
			DisconnectFromMds(S_sock);
			S_sock = 0;
			iobPrintf(out, "ERROR:TIMEOUT\n");
			status = 0;	/* error */
			goto cleanup;
		}
	}

	dbg(1, "nchannels:%d", nchannels);

	if (nchannels == 0){
		err("no channels defined");
		goto cleanup;
	}

	if (nsegs <= 1){
		status = doMdsValuePutSegment(&mvp, kx, &window, out);
	}else if (k234_nelems < 2){
		err("nsegs > 1 assumes k[0]=start, k[1]=end");
		iobPrintf(out, "ERROR:unsuitable k expression\n");
		goto cleanup;
	}else if (!HAS_WINDOW(&window)){
		err("nsegs > 1 required window to be set");
		iobPrintf(out, "ERROR segs > 1 required window to be set\n");
		goto cleanup;
	}else{
		unsigned seg_span = (kx[1].payload.ull - kx[0].payload.ull)/nsegs;
		struct Window w_seg = window;
		struct DescripWrapper* k_seg = copy_k234_descrip(kx);
		int iseg;
		k_seg[1].payload.ull = kx[0].payload.ull+seg_span;
		w_seg.length_samples = window.length_samples/nsegs;

		for (iseg = 0; iseg < nsegs; ++iseg){
			status = doMdsValuePutSegment(&mvp, k_seg, &w_seg, out);
			if ((status&1) == 0){
				break;
			}
			k_seg[0].payload.ull = k_seg[1].payload.ull + k_seg[2].payload.ull;
			k_seg[1].payload.ull += seg_span;
			w_seg.start_sample += w_seg.length_samples;
		}
		destroyDescrip(k_seg);
	}

cleanup:
	destroyDescrip(kx);

	if (outfp){
		fclose(outfp);
	}
	dbg(2,"Leaving doMdsValuePut %s", outfp? "CLOSE":"");
	return (status&1) ? 0: -1;
}
/* doMdsValuePut ends */



static int doMdsSetEvent(int argc, const char *argv[], struct Buf* out)
{
	const char* evname = argc > 1? argv[1]: "acq2xx_event";

	Message *m = malloc(sizeof(MsgHdr) + strlen(evname));
	m->h.ndims = 0;
	m->h.client_type = IEEE_CLIENT;
	m->h.msglen = sizeof(MsgHdr) + strlen(evname);
	m->h.dtype = DTYPE_EVENT_NOTIFY;
	strncpy(m->bytes, evname, 12);

	SendMdsMsg(S_sock, m, MSG_OOB);
	free(m);
	return 0;
}
static int doMdsOpen(int argc, const char *argv[], struct Buf* out)
{
	const char* tree = argc > 1 ? argv[1] : "main";
	int shot = argc > 2 ? atoi(argv[2]) : 0;

	int ok = MdsOpen(S_sock, (char *)tree, shot) & 1;

	if (!ok){
		iobPrintf(out, "ERROR:MDS_OPEN FAILED\n");
		return -1;
	}else{
		iobPrintf(out, "OK\n");
		return 0;
	}
}
static int doMdsConnect(int argc, const char *argv[], struct Buf* out)
{
	if (S_sock){
		DisconnectFromMds(S_sock);
	}
	S_sock = ConnectToMds((char *)((argc > 1) ? argv[1] : 
				       "lost.pfc.mit.edu:9000"));

	if (S_sock >= 0){
		iobPrintf(out, "MDS_SOCK=%ld\n", S_sock);
		return 0;
	}else{
		iobPrintf(out, "ERROR: mdsConnect failed\n");
		return -1;
	}
}
static int doMdsDisconnect(int argc, const char *argv[], struct Buf* out)
{
	DisconnectFromMds(S_sock);
	iobPrintf(out, "OK\n");
	return 0;
}


static int doMdsClose(int argc, const char *argv[], struct Buf* out)
{
	int status = MdsClose(S_sock);

	if (status&1){
		iobPrintf(out, "OK\n");
		return 0;
	}else{
		iobPrintf(out, "ERROR: mdsClose failed\n");
		return -1;
	}
}

static int doMdsSetDef(int argc, const char *argv[], struct Buf* out)
{
	int status = MdsSetDefault(S_sock, (char *)argv[1]);

	if (status&1){
		iobPrintf(out, "OK\n");
		return 0;
	}else{
		iobPrintf(out, "ERROR: mdsSetDef %s failed\n", argv[1]);
		return -1;
	}
}


static int doUnrecognizedCommand(int argc, const char *argv[], struct Buf* out)
{
	iobPrintf(out, "ERROR command not recognized %s ", argv[0]);
	return 0;
}


static void help_visitor(void *vobj, const char* command)
{
	struct Buf* out = (struct Buf*)vobj;

	iobPrintf(out, "%s\n", command);
}
static int doMdsHelp(int argc, const char *argv[], struct Buf* out)
{
	mdsshell_iterateCommands(help_visitor, out);
	return 0;
}



static struct CommandLut {
	const char* command;
	int (* action)(int argc, const char* argv[], struct Buf* out);
} serverCommands[] = {
	{ "mdsConnect", doMdsConnect },
	{ "mdsOpen",    doMdsOpen },
	{ "mdsPut",     doMdsPut },
	{ "mdsValue",   doMdsValue },
	{ "mdsValuePut", doMdsValuePut },
	{ "mdsSegPut",   doMdsValuePut },
	{ "mdsSetEvent", doMdsSetEvent },
	{ "mdsClose",   doMdsClose },
	{ "mdsSetDef",  doMdsSetDef },
	{ "mdsDisconnect", doMdsDisconnect },
	{ "help",       doMdsHelp },
	{ "",           doUnrecognizedCommand }
};

#define NCMDS (sizeof(serverCommands)/sizeof(struct CommandLut))

void mdsshell_help(void) {
	int ic;

	fprintf(stderr, "mdsshell: multicall binary and server " BUILD "\n");
	fprintf(stderr, "server:\nmdsshell 2>&1 >/tmp/mdsshell.log &\n");
	
	for (ic = 0; ic != NCMDS; ++ic){
		fprintf(stderr, "%s\n", serverCommands[ic].command);
	}
} 


void mdsshell_iterateCommands(
	void (*visitor)(void *vobj, const char* command), 
	void* vobj)
{
	int ic;

	for (ic = 0; ic != NCMDS; ++ic){
		visitor(vobj, serverCommands[ic].command);
	}	
}

int mdsshell_doServerCommand(struct IoBuf *cmd) 
{
#define IS_COMMENT(s) (s[0] == '#')
	char* argv[MAXARGS];
	int argc = split(cmd->in.buf, argv, MAXARGS);
	int command;

	dbg(2,"split: argc:%d", argc);
	if (acq200_debug){
		int ii;
		for (ii = 0; ii != argc; ++ii){
			dbg(3, "%d : \"%s\"", ii, argv[ii]);
		}
	}
	if (argc == 0 || IS_COMMENT(argv[0])){        /* are you there ? */
		return 0;
	}
	for (command = 0; command != NCMDS; ++command){
		struct CommandLut* sc = &serverCommands[command];
		if (strcmp(argv[0], sc->command) == 0){
			return sc->action(argc, (const char **)argv, &cmd->out);
		}
	}
	
	assert(0);
	return -1;  
}



