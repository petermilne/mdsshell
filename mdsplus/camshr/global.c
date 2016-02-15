// global.c
//-------------------------------------------------------------------------
//	Stuart Sherman
//	MIT / PSFC
//	Cambridge, MA 02139  USA
//
//	This is a port of the MDSplus system software from VMS to Linux, 
//	specifically:
//			CAMAC subsystem, ie libCamShr.so and verbs.c for CTS.
//-------------------------------------------------------------------------
//	$Id: global.c,v 1.1 2002/09/10 16:06:59 twf Exp $
//-------------------------------------------------------------------------

#include "common.h"

int				CTSdbFileIsMapped   = FALSE;
int				CRATEdbFileIsMapped = FALSE;
struct MODULE	*CTSdb;
struct CRATE	*CRATEdb;
