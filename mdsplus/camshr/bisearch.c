// bisearch.c
//-------------------------------------------------------------------------
//	Stuart Sherman
//	MIT / PSFC
//	Cambridge, MA 02139  USA
//
//	This is a port of the MDSplus system software from VMS to Linux, 
//	specifically:
//			CAMAC subsystem, ie libCamShr.so and verbs.c for CTS.
//-------------------------------------------------------------------------
//	$Id: bisearch.c,v 1.1 2002/09/10 16:06:59 twf Exp $
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// include files
//-------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <errno.h>

#include "common.h"
#include "module.h"
#include "crate.h"
#include "prototypes.h"

//-------------------------------------------------------------------------
// bisearch -- 	binary search
// Wed Jan 10 16:08:48 EST 2001
// Tue May 14 17:03:57 EDT 2002	-- fixed comparision string length
// Wed May 15 11:23:04 EDT 2002	-- fixed 'addcrate' search
//-------------------------------------------------------------------------
// input:	see parameter list
// output:	-1 indicates entry not found; 0 or greater is index of found
// 			entry
//-------------------------------------------------------------------------
int bisearch( int dbType,
	  		  const void *target, 
	  		  int size, 
	  		  int (*compare)(const void *key1, const void *key2)
	  		  ) 
{
	void 					*dbptr;					// generic pointer
	char 					*candidate, fmt[9];
	int						left, middle, right;
	int 					entrySize;
	extern struct MODULE	*CTSdb;
	extern struct CRATE		*CRATEdb;

	char tmp[33];	// allows a full entry
	int length;

	if( MSGLVL(FUNCTION_NAME) )
		printf( "bisearch('%s') tablesize=%d\n", (char *)target,size );

	/*****************************************************************************
	*  Continue searching until the left and right indices cross.                *
	*****************************************************************************/
	left  = 0;
	right = size - 1;

	switch( dbType ) {
		case CTS_DB:
			(struct MODULE *)dbptr = CTSdb;
			entrySize              = MODULE_ENTRY;
			break;

		case CRATE_DB:
			(struct CRATE *)dbptr = CRATEdb;
			entrySize             = CRATE_ENTRY;
			break;
	}

	candidate = (char *)malloc(entrySize);	// allocate memory for item to compare with
	
	while( left <= right ) {
		middle = (left + right) / 2;
	if(MSGLVL(8)) 
		printf("middle=%d\n", middle);

		switch( dbType ) {		// [2002.05.15]
			case CTS_DB:
				sprintf(tmp, "%.32s", (char *)(dbptr + (middle * entrySize)));		// extract db entry [2002.05.14]
				length = strcspn(tmp, " ");											// get logical name [2002.05.14]
				break;

			case CRATE_DB:
//				length = 6;		// eg. 'GKabnn'
				length = (strlen(target) == 4) ? 4 : 6;		// eg. 'GKab' or 'GKabnn'
				break;
		}

		sprintf(fmt, "%%.%ds", length);										// build format string -- fixed [2002.05.14]
		sprintf(candidate, fmt, (char *)(dbptr + (middle * entrySize)));	// get an item to check
		if(MSGLVL(8)) 
			printf("calling  compare('%s', '%s')\n", candidate, (char *)target);

		switch( compare( candidate, target )) {
			case -1:
				/*****************************************************************
				*  Prepare to search to the right of the middle index.           *
				*****************************************************************/
				left = middle + 1;
				break;

			case 1:
				/*****************************************************************
				*  Prepare to search to the left of the middle index.            *
				*****************************************************************/
				right = middle - 1;
				break;

			case 0:
				/*****************************************************************
				*  Return the exact index where the data has been found.         *
				*****************************************************************/
				goto Bisearch_Exit;		// return curent middle value
		}
	}

	/*****************************************************************************
	*  Return that the data was not found.                                       *
	*****************************************************************************/
	middle = -1;

Bisearch_Exit:
	if( candidate )						// prevent memory leaks;
		free(candidate);

	return middle;
}
