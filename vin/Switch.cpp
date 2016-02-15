/*
 * Switch.cpp - switch implementation 
 */

#include "local.h"

#define ALL_CHANNELS 0
#include "Switch.h"

#include <unistd.h>
#include <stdio.h>
#include <errno.h>


#include "FileClosure.h"

extern void my_exit(int eno);

extern int site;

class DefaultSwitch : public Switch {
 public:
	DefaultSwitch() {}
	virtual int getState(int channel) { return 0; }
	virtual int setState(int channel, int hi) { return 0; }
	virtual int read() { return 0; }
	virtual int write() { return 0; }
	virtual ~DefaultSwitch() {}
};


class SimpleSwitch : public Switch {
	const char* root;
	const char* switch_dev;
 protected:
	unsigned value;

	void modelSafetyCheck(FileAccessor& fileAccessor){
		if (!fileAccessor.fileExists()){
			err("ERROR:Config file not matched by hardware");
			my_exit(-1);
		}
	}

	virtual unsigned mask(int channel) {
		if (channel == ALL_CHANNELS){
			return 0xffff;
		}else{
			return 1U << (channel-1);
		}
	}
 public:
	void pokeValue(unsigned _value) { value = _value; }
	unsigned peekValue(void) { return value; }

	SimpleSwitch(const char* _root, const char* _switch) :
		root(_root), switch_dev(_switch)
		{}

	virtual int getState(int channel) {
		return (value & mask(channel)) != 0; 
	}
	virtual int setState(int channel, int hi) {
		if (hi){
			value |= mask(channel);
		}else{
			value &= ~mask(channel);
		}
		return 0;
	}
	virtual int read() {
		FileAccessor fileAccessor(root, switch_dev);

		modelSafetyCheck(fileAccessor);
		fileAccessor.getValue(&value);

		dbg(1, "%s : 0x%04x", switch_dev, value);
		return 0;
	}
	virtual int write() {
		FileAccessor fileAccessor(root, switch_dev);

		modelSafetyCheck(fileAccessor);
		fileAccessor.setValue(value);

		dbg(1, "%s : 0x%04x", switch_dev, value);
		return 0;
	}
};

class SimpleTwoPoleSwitch : public SimpleSwitch {
	virtual int position(int channel) {
		return 2 * (channel-1);
	}
 protected:
	virtual unsigned mask(int channel) {
		if (channel == ALL_CHANNELS){
			return 0xffff;
		}else{
			return 3U << position(channel);
		}
	}	
 public:
	SimpleTwoPoleSwitch(const char* _root, const char* _switch) :
		SimpleSwitch(_root, _switch)
		{}
	virtual int getState(int channel) {
		return (value & mask(channel)) >> position(channel); 
	}
	virtual int setState(int channel, int twobits) {
		value &= ~ mask(channel);
		if (twobits){
			value |= twobits << position(channel);
		}
		return 0;
	}
};

class LUTSwitch: public SimpleSwitch {
	const int* lut;   // ** indexes logical bit [0..15] to 
	                  //    phys bit [0..15];

 public:
	LUTSwitch(const int* _lut, const char* _root, const char* _switch) :
		SimpleSwitch(_root, _switch),
		lut(_lut) 
		{}

 protected:
	virtual unsigned mask(int channel) {
		if (channel == ALL_CHANNELS){
			return 0xffff;
		}else{
			return lut[channel];
		}
	}	
};


class Acq216_ADC_RANGE_SWITCH: public LUTSwitch {
	static int adc_range_lut[];
 public:
	Acq216_ADC_RANGE_SWITCH(const char* _root, const char* _switch) :
		LUTSwitch(adc_range_lut, _root, _switch)
		{}
};

int Acq216_ADC_RANGE_SWITCH::adc_range_lut[] = {
/* physical channel[logical_channel], indexed from 1 */
	0,
	0x0400, /* - sw11  ch01 */
	0x4000, /* - sw15  ch02 */
	0x0080, /* - sw08  ch03 */
	0x0008, /* - sw04  ch04 */
	0x0800, /* - sw12  ch05 */
	0x8000, /* - sw16  ch06 */
	0x0040, /* - sw07  ch07 */
	0x0004, /* - sw03  ch08 */
	0x0100, /* - sw09  ch09 */
	0x1000, /* - sw13  ch10 */
	0x0020, /* - sw06  ch11 */
	0x0002, /* - sw02  ch12 */
	0x0200, /* - sw10  ch13 */
	0x2000, /* - sw14  ch14 */
	0x0010, /* - sw05  ch15 */
	0x0001  /* - sw01  ch16 */
};


class SwitchEmulator : public SimpleSwitch {
/** proxy */
	unsigned short* cache;
	SimpleSwitch* the_switch;
public:
	SwitchEmulator(SimpleSwitch* _the_switch, unsigned short* _cache) :
		SimpleSwitch(0, 0),
		cache(_cache), the_switch(_the_switch)
	{
		dbg(3, "cache %p", _cache);
	}

	virtual int getState(int channel) {
		return the_switch->getState(channel);
	}
	virtual int setState(int channel, int hi){
		return the_switch->setState(channel, hi);
	}
	virtual int read() {
		the_switch->pokeValue(value = *cache);
		dbg(4, "value %04x cache %p", value, cache);
		return 0;
	}
	virtual int write() {
		value = *cache = the_switch->peekValue();
		dbg(4, "value %04x cache %p", value, cache);
		return 0;
	}
};

class M5_TopsideSwitch : public SimpleSwitch {
/* two logical switches ch1..16 map to two physical switches ch[8][2] */


	int is;

        static SimpleSwitch *physical[2];
	static SimpleSwitch *logical[2];
 protected:

	M5_TopsideSwitch(int _is, const char* _root, const char* _switch) :
		SimpleSwitch(_root,_switch), is(_is){}

	int write_copy() {
		for (int chan = 1; chan <= 8; ++chan){
			int two_bits = 
				logical[0]->getState(chan) |
				(logical[1]->getState(chan) << 1);
			physical[0]->setState(chan, two_bits);
		}
		for (int chan = 9; chan <= 16; ++chan){
			int two_bits = 
				logical[0]->getState(chan) |
				(logical[1]->getState(chan) << 1);
			physical[1]->setState(chan-8, two_bits);
		}
		physical[0]->write();
		physical[1]->write();
		return 0;
	}

	int read_copy() {
		physical[0]->read();
		physical[1]->read();

		for (int chan = 1; chan <= 8; ++chan){
			int two_bits = physical[0]->getState(chan);
			
			logical[0]->setState(chan, two_bits&1);
			logical[1]->setState(chan, two_bits>>1);
		}
		for (int chan = 9; chan <= 16; ++chan){
			int two_bits = physical[1]->getState(chan-8);
			
			logical[0]->setState(chan, two_bits&1);
			logical[1]->setState(chan, two_bits>>1);
		}
		return 0;
	}
 public:
	static SimpleSwitch* instance(
		int is, const char* root, const char*file) {
		
		physical[is] = new SimpleTwoPoleSwitch(root, file);
		logical[is] = new M5_TopsideSwitch(is, root, file);

		return logical[is];
	}

	virtual int read() {
		return is == 0? read_copy() : 0;
	}
	virtual int write() {
		return is == 0? write_copy(): 0;
	}

friend class SwitchFactory;
};

SimpleSwitch* M5_TopsideSwitch::physical[2];
SimpleSwitch* M5_TopsideSwitch::logical[2];


class Acq400Switch : public Switch {
	const char* knob;
	const char* knob_base;
	int nchan;
	char* switch_string;

 public:
	Acq400Switch(const char* _knob) : knob(_knob), switch_string(0)
 	{
		if (strcmp(knob, "gains") == 0){
			knob_base = "gain";
		}else{
			err("WARNING: guessing at knob_base:%s", knob);
			knob_base = knob;
		}
		/* lazy init nchan, switch_string from read() .. */
		read();
 	}

	virtual ~Acq400Switch() {
		delete [] switch_string;
	}

	virtual int getState(int channel) {
		return switch_string[channel-1] - '0';
	}
	virtual int setState(int channel, int value) {
		char cmd[128];
		switch_string[channel-1] = value + '0';
		sprintf(cmd, "set.site %d %s%d=%d", ::site, knob_base, channel, value);
		FILE *fp = popen(cmd, "w");
		pclose(fp);
		return 0;
	}
	virtual int read() {
		char query[128];
		sprintf(query, "get.site %d %s", ::site, knob);
		FILE* fp = popen(query, "r");
		fgets(query, 128, fp);
		chomp(query);
		dbg(1, "Acq400Switch::read returned %s [%d]", query, strlen(query) );
		if (!switch_string){
			nchan = strlen(query);
			assert (nchan >= 4 && nchan%4 == 0);
			switch_string = new char[nchan+1];
			memset(switch_string, '\0', nchan+1);
		}
		strncpy(switch_string, query, nchan);
		return 0;
	}
	virtual int write() {
		return 0;
	}
};
unsigned short* SwitchFactory::ebuf;

void SwitchFactory::enableEmulation(unsigned short *_ebuf) {
	ebuf = _ebuf;
}


/** @todo: for host side operation, create "in-memory switch option */
Switch* SwitchFactory::create(
	const char* model, int is, 
	const char* root, const char*file){


	SimpleSwitch* the_switch;
/* select on model, is, create appropriate switch */
	if (strstr(model, "acq420")){
		return new Acq400Switch(file);
	}else if (strstr(model, "M2")){

		if (is == 0){
			the_switch =  new Acq216_ADC_RANGE_SWITCH(
				root, file);
		}else{
			the_switch = new SimpleSwitch(root, file);
		}


	}else if (strstr(model, "M5")){
		if (is == 0){
			the_switch = new Acq216_ADC_RANGE_SWITCH(
				root, file);
		}else{
			the_switch = M5_TopsideSwitch::instance(
					is-1, root, file);
			if (ebuf != 0){
				SimpleSwitch** pps = 
					&M5_TopsideSwitch::physical[is-1];
				*pps = new SwitchEmulator(*pps, ebuf+is);
			}
			return the_switch;
		}
	}else if (strstr(model, "ACQ132")){
		return new SimpleSwitch(root, file);
	}else if (strstr(model, "ACQ196") || strstr(model, "ACQ164")){
		return new DefaultSwitch();
	}else{
		err("AcqCalibration.Info.Model \"%s\" NOT SUPPORTED", 
		    model);
		return 0;
	}

	if (ebuf){
		return new SwitchEmulator(the_switch, ebuf+is);
	}else{
		return the_switch;
	}
}

