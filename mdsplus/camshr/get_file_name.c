// get_file_name.c
//-------------------------------------------------------------------------
//	Stuart Sherman
//	MIT / PSFC
//	Cambridge, MA 02139  USA
//
//	This is a port of the MDSplus system software from VMS to Linux, 
//	specifically:
//			CAMAC subsystem, ie libCamShr.so and verbs.c for CTS.
//-------------------------------------------------------------------------
//	$Id: get_file_name.c,v 1.1 2002/09/10 16:06:59 twf Exp $
//-------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "prototypes.h"

//-------------------------------------------------------------------------
// get_file_name()
//-------------------------------------------------------------------------
// Prepends path information for files, specifically db files, using
// 	information passed in the ENVIRONMENT. User can append a '/' char,
// 	or not.
// 	Eg. use:  CTS_DB_DIR=/my_directory;  export CTS_DB_DIR
// 	     or:  CTS_DB_DIR=/my_directory/; export CTS_DB_DIR
// 	                   trailing '/' --^
//-------------------------------------------------------------------------
// Fri Apr 13 12:29:02 EDT 2001
// Tue Apr 17 11:42:55 EDT 2001	-- more concice processing of '/' character
//-------------------------------------------------------------------------
char *get_file_name( char *filename )
{
	char		*pEnv;					// pointer to environment var
	static char	db_dir[256], *pChar;	// needs to be persistant

	// get environment variable
	pEnv = getenv(DB_DIR);				// variable specified in ".../tunables.h"

	// adjust filename
	if( pEnv == NULL )
		sprintf(db_dir, "%s", filename);				// ENV variable is NULL -- no change to filename
	else
		sprintf(db_dir, "%s%s%s", 						// ENV variable non-NULL -- prepend to filename
			pEnv, 
			(*(pEnv+strlen(pEnv)-1) != '/') ? "/" : "",	// add '/' if necessary
			filename
			);

	pChar = &db_dir[0];

	if( MSGLVL(FUNCTION_NAME) )
		printf( "get_file_name('%s')\n", pChar );

	// send it back ...
	return pChar;
}
