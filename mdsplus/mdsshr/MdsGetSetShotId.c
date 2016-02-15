/*------------------------------------------------------------------------------

		Name:   MdsGetCurrentShotId

		Type:   C function

		Author:	Thomas W. Fredian

		Date:   11-OCT-1989

		Purpose: Get current shot number

------------------------------------------------------------------------------

	Call sequence:

int MdsGetCurrentShotId(experiment,shot)

------------------------------------------------------------------------------
   Copyright (c) 1989
   Property of Massachusetts Institute of Technology, Cambridge MA 02139.
   This program cannot be copied or distributed in any form for non-MIT
   use without specific written approval of MIT Plasma Fusion Center
   Management.
------------------------------------------------------------------------------*/
#include <stdio.h>
#include <STATICdef.h>

STATIC_CONSTANT char *cvsrev = "@(#)$RCSfile: MdsGetSetShotId.c,v $ $Revision: 1.9 $ $Date: 2003/11/24 17:50:18 $";

int       MdsGetCurrentShotId(char *experiment)
{
  printf("MdsGetCurrentShotId is obsolete, use TreeGetCurrentShotId in TreeShr\n");
  return 0;
}

int       MdsSetCurrentShotId(char *experiment, int shot)
{
  printf("MdsSetCurrentShotId is obsolete, use TreeSetCurrentShotId in TreeShr\n");
  return 0;
}
