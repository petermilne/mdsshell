
#include <stdio.h>
#include "RawLimits.h"


long RawLimits::readKnob(const char *fname, long _default)
{
	FILE *fp = fopen(fname, "r");
	long value = _default;

	if (fp){
		fscanf(fp, "%ld", &value);		
		fclose(fp);
	}
	
	return value;
}


int RawLimits::writeKnob(const char *knob, int value)
{
	FILE *fp = fopen(knob, "w");

	if (fp == 0){
		return -1;
	}

	fprintf(fp, "%d\n", value);
	fclose(fp);
	return 0;
}

#define MIN_CODE(size) ( -(1 << ((size)*8 - 1)))
#define MAX_CODE(size) (  (1 << ((size)*8 - 1)) - 1)


RawLimits::RawLimits() :
	word_size(readKnob("/dev/dtacq/word_size", 2)),
	code_min(readKnob("/dev/dtacq/code_min", MIN_CODE(word_size))),
	code_max(readKnob("/dev/dtacq/code_max", MAX_CODE(word_size)))
{

}

void RawLimits::print() {
	RawLimits rl;
	printf("code_min=%ld,code_max=%ld,", rl.code_min, rl.code_max);
}
