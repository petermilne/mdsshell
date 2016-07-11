/*
 * InputBlock.h
 */

#ifndef __INPUTBLOCK_H__
#define __INPUTBLOCK_H__



#define CONFIG_FILE "/etc/cal/caldef.xml"

#define ALL_CHANNELS 0

class PObject {
 public:
	virtual void print(void) {
		printf("Hello PObject %p\n", this);
	}
	virtual ~PObject() {}
};

#define ALL -1
#define NONE 0
class ChannelSelection : public PObject {
	int *channels;
	int maxchan;
	char *def;

	void init(int _maxchan, int deflen);
 public:
	ChannelSelection(int achannel);
	ChannelSelection(int _maxchan, const char* def);
	ChannelSelection(int _maxchan, int all = NONE);     //** default NONE 
	ChannelSelection(int _maxchan, int argc, const char* argv[]);

	virtual ~ChannelSelection();

	const int* getSelection(void) { return channels; }

	const char* toString(void) const;
	const int getMaxChan(void) const { return maxchan; }
	const char* getDef(void) { return def; }

};


class ChannelSelectionIterator {
	ChannelSelection& cs;
	int cursor;
 public:
	ChannelSelectionIterator(ChannelSelection& _cs): cs(_cs) {
		cursor = 0;
	}
	int getNext(void) {
		while (++cursor <= cs.getMaxChan()){
			if (cs.getSelection()[cursor]){
				return cursor;
			}
		}

		return 0;
	}
};

class Range : public PObject {
	static int reversed;
	const char* fmt;
	
 public:
	double rmin, rmax;
	static const char* code_min;
	static const char* code_max;

	Range(int _rmin, int _rmax) : 
		fmt("%6.4f,%6.4f"),
		rmin(_rmin), rmax(_rmax)  
	{}

	Range() : 
		fmt("%6.4f,%6.4f"),
		rmin(0), rmax(0) 
	{}

	int print() const;

	double getSpan() {
		return rmax - rmin;
	}	

	static void setReversed() {
		reversed = 1;
	}
	static int isReversed(void) {
		return reversed != 0;
	}
	static int sanityCheck(Range& nominal, Range& test);

	void setFormat(const char* _fmt) {
		fmt = _fmt;
	}
	bool operator==(const Range& other) const {
		return other.rmin == rmin && other.rmax == rmax;
	}
	bool operator!=(const Range& other) const {
		return !(*this == other);
	}

	static int getCodeMin() {
		return strtol(code_min, 0, 0);	
	}
	static int getCodeMax() {
		return strtol(code_max, 0, 0);	
	}
};

class TiXmlHandle;

class RangeSelector {
 public:
	virtual const char* getCurrentRangeKey(int channel = ALL_CHANNELS) = 0;
	virtual void selectRange(
		const char* key, int channel = ALL_CHANNELS) = 0;

	virtual void update(void) {}
	virtual void flush(void) {}

	virtual ~RangeSelector() {}
};

class InputBlock : public PObject {
/** Factory Class */
 protected:
	const char* devroot;
	InputBlock(const char* _devroot);

	int site;

 public:

	virtual ~InputBlock();
	virtual TiXmlHandle getCalibrationHandle(void) = 0;
	virtual int getNumChannels(void) = 0;
	virtual int getNumRanges(void) = 0;

	virtual const char* listRangeKeys(char buf[], int maxbuf) = 0;
	
	
	virtual int setRange(const char* key) = 0;
	virtual int setRange(const char* key, ChannelSelection& cs) = 0;
	virtual int setRange(const char* key, int channel) = 0;

	virtual int setCurrentRangeValue(ChannelSelection& cs, Range* range)=0;
	virtual int setChannelCount(int channel_count) = 0;

//	virtual const char* getCurrentRangeKey(int channel) = 0;
	virtual const char* getCurrentRangeName(int channel) = 0;
	virtual Range getRange(int channel = 1) = 0;
	virtual Range getRange(int channel, const char *key) = 0;
	virtual Range getNominalRange(int channel = 1) = 0;
	virtual Range getNominalRange(ChannelSelection& selection) = 0;

	virtual void putCalibratedRange(int channel, Range range) = 0;

	virtual void print(void) = 0;

	static InputBlock& getInstance(
		const char* fname, const char* model = 0);

	virtual void finalize(void) {}
	virtual void validate(void) {}

	virtual void saveFile(const char* fname) = 0;

	static void setClientVersion(const char* _verid);
	static void setClientCmd(int argc, const char* argv[]);

	virtual int polarityIsReversed(void) = 0;

	int collateRegValues(char* ubuf, int ubuf_max);

	const char* getDevRoot(void);
	virtual const char* getModel(void) = 0;

	void setSite(int _site) {
		site = 1;
	}
	int getSite() const {
		return site;
	}
};


#endif // __INPUTBLOCK_H__
