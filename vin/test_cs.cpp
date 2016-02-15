#include "local.h"
#include "popt.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>               // getenv()
#include "InputBlock.h"



int main(int argc, const char* argv[])
{
	ChannelSelection cs(96, argc - 1, &argv[1]);
	puts(cs.toString());
	return 0;
}
