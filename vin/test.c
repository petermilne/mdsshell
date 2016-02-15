/*
 * test
 */


#include <stdio.h>
#include "acq-util.h"

#define MAXCHAN 16

static int channels[MAXCHAN+1];

void test(const char* def)
{
	int nsel = acqMakeChannelRange(channels, MAXCHAN, def);

	printf( "%20s:%2d ", def, nsel);
	
	int ichan;

	for (ichan = 1; ichan <= MAXCHAN; ++ichan){
		printf("%d,", channels[ichan]);
	}
	printf("\n");
}
void main(int argc, const char* argv[])
{
	int iarg = 1;

	for (; iarg != argc; ++iarg){
		test(argv[iarg]);
	}
}
