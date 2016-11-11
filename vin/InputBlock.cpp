extern "C" {
#include "local.h"
};
#include "InputBlock.h"
#include "Switch.h"

#include "tinyxml.h"

#include <errno.h>

#include <math.h>

#ifdef TIXML_USE_STL
	#include <iostream>
	#include <sstream>
	using namespace std;
#else
	#include <stdio.h>
#endif

#define DEPRECATED_WARNING err( \
    "WARNING: deprecated caldef file please modify to include"\
    " single root \"ACQ\"\n")

char* verid;
char* cmd;

void InputBlock::setClientVersion(const char* _verid) {
	verid = new char[strlen(_verid)+1];
	strcpy(verid, _verid);
	chomp(verid);
}

void InputBlock::setClientCmd(int argc, const char* argv[]) {
	int wc = 0;
	int ii;
	for (ii = 0; ii != argc; ++ii){
		wc += strlen(argv[ii]) + 1;
	}
	cmd = new char[wc];
	cmd [0] = '\0';

	for (ii = 0; ii < argc; ++ii ){
		if (ii){
			strcat(cmd, " ");
		}
		strcat(cmd, argv[ii]);
	}
}
void my_exit(int eno){
	exit(eno);                // @@stick yer breakpoint here
}

//
// This file demonstrates some basic functionality of TinyXml.
// Note that the example is very contrived. It presumes you know
// what is in the XML file. But it does test the basic operations,
// and show how to add and remove nodes.
//

void handleInfo(TiXmlNode* info)
{
	if (info){
		TiXmlNode* child = 0;
		while ((child = info->IterateChildren(child)) != 0){
			fprintf(stderr, "Info:");
//			child->Print(stdout);
		}
	}
}

void handleData(TiXmlNode* data)
{
	if (data){
		TiXmlNode* child = 0;
		while ((child = data->IterateChildren(child)) != 0){
			fprintf(stderr, "Data:");
//			child->Print(stdout);
		}		
	}
}

#define IMIN 0
#define IMAX 1

#define MAXCHAN 96 

void getVoltsRange(TiXmlElement* range, int aichan)
{
	TiXmlNode *nominal_node;
	double nominal[2];
	double calibrated[MAXCHAN][2];

	if ((nominal_node = range->FirstChild("Nominal")) != 0){

		nominal_node->ToElement()->Attribute("min", nominal+IMIN);
		nominal_node->ToElement()->Attribute("max", nominal+IMAX);

		printf("getVoltsRange=%6.4fV,%6.4fV\n", nominal[IMIN], nominal[IMAX]);

//		printf( "Nominal %6.4fV,%8.4fV\n",
//			nominal->ToElement()->FirstChild("min")->
	}

	for (int ic = 0; ic != aichan; ++ic){
		calibrated[ic][IMIN] = nominal[IMIN];
		calibrated[ic][IMAX] = nominal[IMAX];
	}


	
	for ( TiXmlNode *child = 0; (child = range->IterateChildren(child)) != 0; ){
		TiXmlElement* element = child->ToElement();
		int ic;

		if ((element->Attribute("ch", &ic)) != 0){

			fprintf(stderr, "Channel ch %d min %s max %s\n",
			       ic, element->Attribute("min"), element->Attribute("max"));

			element->Attribute("min", &calibrated[ic-1][IMIN]);
			element->Attribute("max", &calibrated[ic-1][IMAX]);			
		}
	}

	for (int ic = 0; ic != aichan; ++ic){
		printf( "%.4f,%6.4f%c", 
			calibrated[ic][IMIN], calibrated[ic][IMAX],
			ic+1 == aichan? '\n': ',');
	}	
}
void getVoltsRange(TiXmlNode* data, const char* _sw) {
	/* find the data element with attribute equals sw */

	int aichan = 0;
	data->ToElement()->Attribute("AICHAN", &aichan);

	printf("getVoltsRange()\n");
	if (data){
		TiXmlNode* child = 0;
		
		while((child = data->IterateChildren(child)) != 0){
			TiXmlElement* range = child->ToElement();
			if (range){
				if (range->Attribute("sw")){
					if (strcmp(range->Attribute("sw"), _sw) == 0){
						printf( "Range matches switch %s\n", _sw);
						getVoltsRange(range, aichan);
					}
				}
			}

		}
	}
	/* then print all the values in the correct format */
}


InputBlock::InputBlock(const char* _devroot, int _site) : site(_site)
{
	char* buf = new char[strlen(_devroot)+20];
	sprintf(buf, "/dev/%s", _devroot);
	devroot = buf;

	dbg(1, "%s devroot \"%s\" site %d\n", _PFN, devroot, site);
}

InputBlock::~InputBlock()
{
	delete [] devroot;
}

void InputBlock::print(void)
{
	printf("InputBlock %p\n", this);
}

class DebugRangeSelector : public RangeSelector {
	const char* key;

 public:
	DebugRangeSelector(const char* _key) : key(_key) {}
	virtual const char* getCurrentRangeKey(int channel) {
		return key;
	}
	virtual void selectRange(const char* _key, int channel) {
		key = _key;
	}
};


const char* InputBlock::getDevRoot(void) 
{
	return devroot;
}

// @@todo
class ConcreteRangeSelector : public RangeSelector {
	const char* model;
 protected:
	Switch** switches;
	int nswitches;
	char rangeKey[16];

	void connectSwitches(const char* SW) {
		char *swbuf = new char [strlen(SW)+1];
		char *swbuf2 = new char [strlen(SW)+1];
		strcpy(swbuf, SW);
		char *tok;

		dbg(2, "01 model %s SW %s", model, SW);
		for (tok = swbuf, nswitches = 0; 
		     (tok = strtok_r(tok, ",", &swbuf2)) != 0; tok = 0){
			++nswitches;
		}

		strcpy(swbuf, SW);
		switches = new Switch* [nswitches];

		int is = 0;
		for (tok = swbuf;
		     (tok = strtok_r(tok, ",", &swbuf2)) != 0; ++is, tok = 0){
			switches[is] = SwitchFactory::create(
				model, input_block->getSite(), is, input_block->getDevRoot(), tok);
		}

		dbg(2, "99", SW);
	}

	
	void update(int isw) {
		switches[isw]->read();
	}
	void flush(int isw) {
		switches[isw]->write();
	}
 protected:
	void create(const char* SW) {		
		if (!SW){
			err("FAILED to find SW Attribute");
			my_exit(-1);
		}
		connectSwitches(SW);
	}
	InputBlock *input_block;
 public:
	ConcreteRangeSelector(
		InputBlock *_input_block,const char* _model, const char* SW) :
		model(_model),
		input_block(_input_block)
	{
		create(SW);
	}

	virtual const char* getCurrentRangeKey(int channel) {
		char *pk = rangeKey;

		for (int ir = 0; ir != nswitches; ++ir){
			if ((pk - rangeKey) != 0){
				*pk++ = ',';
			}
			*pk++ = '0' + switches[ir]->getState(channel);
		}
		*pk++ = '\0';

		dbg(1, "channel %d key %s\n", channel, rangeKey);

		return rangeKey;
	}
	virtual void selectRange(const char* key, int channel) {
		int ir = 0;

		for (const char* pk = key; *pk; ++pk){
			switch(*pk){
			case '1':
			case '0':
				switches[ir++]->setState(channel, *pk-'0');
				break;
			case ',':
				break;
			default:
				err("BAD key %s value not 0 or 1", key);
			}
		}
	}
	virtual void update(void) {
		for (int is = 0; is != nswitches; ++is){
			update(is);
		}
	}
	virtual void flush(void) {
		for (int is = 0; is != nswitches; ++is){
			flush(is);
		}
	}
};


class SingleChildElement : public TiXmlElement {
	TiXmlText content;
 protected:
	SingleChildElement(const char* name, const char* _content) :
		TiXmlElement(name),  
		content(_content) {
		InsertEndChild(content);
	}
};

class Knob {
	TiXmlText* content;
	TiXmlElement *element;

	void loadPipe(const char* cmd, char* buf, int maxbuf)
	{
		FILE *pp = popen(cmd, "r");
		if (!pp){
			strcpy(buf, "bad command");
		}else{
			fgets(buf, maxbuf, pp);
			pclose(pp);
		}
	}
	void loadFile(const char* _knob, char* buf, int maxbuf)
	{
		FILE *fp = fopen(_knob, "r");
		if (!fp){
			strcpy(buf, "not found");
		}else{
			fgets(buf, maxbuf, fp);
			fclose(fp);
		}
	}
	int isPipe(const char* s){
		return s[strlen(s)-1] == '|';
	}
	void loadKnob(const char* _knob, char* buf, int maxbuf)
	{
		if (isPipe(_knob)){
			char cmd[80];
			memset(cmd, 0, 80);
			strncpy(cmd, _knob, MIN(strlen(_knob)-1, 79));
			loadPipe(cmd, buf, maxbuf);			
		}else{
			loadFile(_knob, buf, maxbuf);
		}
		tr(buf, '\t', ' ');
		chomp(buf);
	}
public:
	Knob(const char* _name, const char* _knob)
	{
		char buf[128];
		loadKnob(_knob, buf, 128);

		char * _content = new char[strlen(buf)+1];
		strcpy(_content, buf);
		content = new TiXmlText(_content);
		element = new TiXmlElement(_name);
		element->InsertEndChild(*content);
	}
	~Knob() {
		delete content;
		delete element;
	}
	TiXmlElement& getElement() {
		return *element;
	}
	TiXmlElement* copyElement() {
		return new TiXmlElement(*element);
	}
};



class DateElement : public SingleChildElement {
	char the_date[80];

	const char* getDate(void) {
		FILE* fp = popen( "date +%Y%m%d:%H:%M", "r");


		fgets(the_date, 80, fp);
		the_date[strlen(the_date)-1] = '\0'; // chomp
		fclose(fp);
		return the_date;
	}
 public:
	DateElement(const char* name) :
		SingleChildElement(name, getDate()) {}
};
	

class SerialnumElement : public SingleChildElement {
	const char* serialnum(void) {
		const char* sn = getenv("serialnum");		
		return sn? sn: "d40000";
	}
 public:
	SerialnumElement(const char* name) :
		SingleChildElement(name, serialnum()) {}
};

class VersionElement : public SingleChildElement {
 public:
	VersionElement(const char* name) :
		SingleChildElement(name, verid) {}
};


class CommandElement : public SingleChildElement {
 public:
	CommandElement(const char *name) :
		SingleChildElement(name, cmd) {}
};



class ConcreteInputBlock : public InputBlock {

protected:
	struct RangeElement {
		TiXmlElement* element;
		Range* cal_tab;      // @@ {0..channelCount} [0] is nominal 
	};


	virtual void getVoltsRange(RangeElement& re);

	static void insertOrReplace(TiXmlNode* info, TiXmlElement* element) {
		TiXmlHandle infoH(info);
		TiXmlElement *orig = 
			infoH.FirstChild(element->Value()).Element();
		if (orig){
			info->ReplaceChild(orig, *element);
		}else{
			info->InsertEndChild(*element);
		}
		delete element;
	}

	const char* _getRangeSw(const char* name){
		for (int ir = 0; ir != (1<<rangeCount); ++ir){
			if (TiXmlElement* range = ranges[ir].element){
			
				const char* rn = range->Attribute("name");
				if (rn != 0 && strcmp(rn, name) == 0){
					return range->Attribute("sw");
				}
			}
		}
		return 0;
	}

	const char* _getLimitSw(int _max) {
		int imin = -1, imax = -1;
		float fmin = 0, fmax = 0;

		for (int ir = 0; ir != (1<<rangeCount); ++ir){
			if (TiXmlElement* range = ranges[ir].element){
			
				const char* rn = range->Attribute("name");
				float rv = atof(rn);

				if (ir == 0){
					imin = imax = 0;
					fmin = fmax = rv;	
				}else{
					if (rv < fmin){
						fmin = rv;
						imin = ir;
					}else if (rv > fmax){
						fmax = rv;
						imax = ir;
					}
				}
			}
		}
		if (imin == -1){
			return 0;
		}
		return ranges[_max? imax: imin].element->Attribute("sw");
	}


 protected:
	const char* fname;
	TiXmlDocument* doc;

	TiXmlElement* cal;
	TiXmlElement* data;
	const char* sw;
	int rangeCount;
	int channelCount;
	
	RangeSelector* rangeSelector;
	RangeElement* ranges;
	

	void updateInfo(TiXmlNode* info){
		insertOrReplace(info, new DateElement("Date"));
		insertOrReplace(info, new SerialnumElement("Serialnum"));
		insertOrReplace(info, new VersionElement("Version"));
		insertOrReplace(info, new CommandElement("Command"));

		Knob T0("T0", "/tmp/T0");
		Knob T1("T1", "/tmp/T1");
		Knob fw("Firmware", "get.release.versions|");
		Knob fpga("FPGA", "grep Date /tmp/fpga_header|");

		insertOrReplace(info, T0.copyElement());
		insertOrReplace(info, T1.copyElement());
		insertOrReplace(info, fw.copyElement());
		insertOrReplace(info, fpga.copyElement());
		
	        data->SetAttribute("AICHAN", channelCount);
	}


	void update(void) {
		dbg(2, "");
		TiXmlHandle calHandle(cal);
		TiXmlNode* info = calHandle.FirstChild("Info").Node();
		if (info){
			updateInfo(info);
		}
	}

	const char* getRangeSw(const char* name) {
		if (strcmp(name, "max") == 0){
			return _getLimitSw(1);
		}else if (strcmp(name, "min") == 0){
			return _getLimitSw(0);
		}else{
			return _getRangeSw(name);
		}
	}



	TiXmlHandle getCalibrationHandle(void) {
		return makeCalibrationHandle(doc);
	}

	static void set_code(
		TiXmlElement* data, const char* key, const char*& code_val) 
	{
		const char* value = data->Attribute(key);
		if (value){
			dbg(1, "Range::%s = %s", key, value);
			code_val = value;	
		}			
	}
	
	ConcreteInputBlock(		
		const char* _fname, TiXmlDocument* _doc,
		const char* _devroot, int _site = 0) :
		InputBlock(_devroot, _site),
		fname(_fname), doc(_doc) {

		TiXmlHandle calHandle = makeCalibrationHandle(doc);
		cal = calHandle.Element();
		assert(cal);
		data = calHandle.FirstChild("Data").Element();
		assert(data);
		data->Attribute("AICHAN", &channelCount);
		dbg(1, "channelCount set %d", channelCount);
		sw = data->Attribute("SW");
		dbg(1, "sw set %s", sw);
		const char* polarity = data->Attribute("polarity");
		if (polarity){
			if (strcmp(polarity, "REVERSED") == 0){
				dbg(1, "polarity %s", polarity);
				Range::setReversed();
			}else{
				err("polarity %s", polarity);
			}		
		}
		set_code(data, "code_min", Range::code_min);
		set_code(data, "code_max", Range::code_max);
	}

	void setRangeSelector(RangeSelector* _rangeSelector){
		rangeSelector = _rangeSelector;
		rangeSelector->update();
		getRanges();	
	}
	void getRanges(void) {
		// @@worktodo do a split() on sw
		rangeCount = 3;
		ranges = new RangeElement[1<<rangeCount];

		dbg(2, "ranges set %p", ranges);

		memset(ranges, 0, sizeof(RangeElement) * (1<<rangeCount));

		/** collect all the Ranges and Nominals 
                 *be lazy about the rest */
		TiXmlNode* child = 0;

		int ir = 0;

		while ((child = data->IterateChildren(child)) != 0){
			TiXmlElement* range;

			if ((range = child->ToElement()) != 0){
				dbg(2,"[%d] range %p %s added %s",
				    ir, 
				    range,
				    range->Attribute("name"),
				    range->Attribute("sw"));

				ranges[ir++].element = range;
			}				
		}
	}

	const char* listRangeKeys(char buf[], int maxbuf) {
		/** @@todo WARNING: does NOT test overflow */
		char *pbuf = buf;

		pbuf[0] = '\0';

		for (int ir = 0; ir < (1<<rangeCount); ++ir){
			TiXmlElement* elt = ranges[ir].element;

			if (elt != 0){
				pbuf += sprintf(pbuf, "%c\"%s\"",
						ir? ',':' ',
						elt->Attribute("name"));
			}else{
				break;
			}
		}
		return buf;
	}

	RangeElement* getCurrentRangeElement(int channel) {
		const char* key = rangeSelector->getCurrentRangeKey(channel);
		return getRangeElement(channel, key);
	}

	RangeElement * getRangeElement(int channel, const char *key){
		dbg(2, "key %s ranges %p", key, ranges);

		for (RangeElement* elt = ranges; elt->element != 0; ++elt){
			const char* lookup = elt->element->Attribute("sw");
			dbg(3, "elt:%p range %p %s %s", 
			    elt,
			    elt->element,
			    elt->element->Attribute("name"),
			    elt->element->Attribute("sw"));

			dbg(3, "elt:%p lookup:%s %s", 
			    elt, 
			    lookup==0? "lookup": lookup,
			    strcmp(lookup==0? "lookup": lookup, key)==0?
				"MATCH": "different");

			if ( lookup && strcmp(lookup, key) == 0){
				dbg(2, "we like this one %p", elt);

				if (!elt->cal_tab){
					elt->cal_tab = 
						new Range[channelCount+1];
					getVoltsRange(*elt);
				}
				
				return elt;
			}

		}
		my_exit(-1);
		return 0;
	}
	virtual int getNumChannels(void) { return channelCount; }
	virtual int getNumRanges(void) { return rangeCount; }


	virtual int setRange(const char* key) {
		const char* sw = getRangeSw(key);

		if (sw == 0){
			err("FAILED to match RANGE %s", key);
			exit(-1);
		}

		dbg(2, "key \"%s\" sw %s", key, sw);

		for (int ic = 1; ic <= channelCount; ++ic){
			rangeSelector->selectRange(sw, ic);
		}
		return 0;
	}
	virtual int setRange(const char* key, ChannelSelection& cs) {
		const char* sw = getRangeSw(key);
		const int* selection = cs.getSelection();

		if (sw == 0){
			err("FAILED to match RANGE %s", key);
			exit(-1);
		}

		dbg(2, "key \"%s\" sw %s \"%s\"", key, sw, cs.toString());
		for (int ic = 1; ic <= channelCount; ++ic){
			if (selection[ic]){
				rangeSelector->selectRange(sw, ic);
			}
		}
		
		return 0;
	}
	virtual int setRange(const char* key, int channel) {

		dbg(2, "key:\"%s\", sw: \"%s\"", key, getRangeSw(key));

		rangeSelector->selectRange(getRangeSw(key), channel);
		return 0;
	}
	virtual int setCurrentRangeValue(ChannelSelection& cs, Range* range) {
		return 0;
	}

	virtual const char* getCurrentRangeName(int channel) {
		return getCurrentRangeElement(channel)->
			element->Attribute("name");
	}

	virtual Range getRange(int channel) {
		return getCurrentRangeElement(channel)->cal_tab[channel];
	}
	virtual Range getRange(int channel, const char* key) {
		return getRangeElement(channel, key)->cal_tab[channel];
	}
	virtual Range getNominalRange(int channel) {
		return getCurrentRangeElement(channel)->cal_tab[0];
	}

	virtual Range getNominalRange(ChannelSelection& selection) 
	/** return the range if all set same or ABORT. */
	{
		ChannelSelectionIterator ii(selection);

		int chan = ii.getNext();
		Range range = getNominalRange(chan);

		while((chan = ii.getNext()) != 0){
			if (range != getNominalRange(chan)){
				err("Range mismatch on channel %d", chan);
				exit(1);
			}
		}
		return range;
	}

	virtual void putCalibratedRange(int channel, Range range);

	virtual void print(void) {
		doc->Print(stderr);
	}

	virtual void validate(void) {
		rangeSelector->update();
	}
	virtual void finalize(void) {
		rangeSelector->flush();
	}

	virtual void saveFile(const char* fname) {
		update();
		dbg(2,"fname %s", fname);
		doc->SaveFile(fname);
	}

	virtual int polarityIsReversed(void) {
		return Range::isReversed();
	}

	virtual int setChannelCount(int channel_count) {
		return channelCount = channel_count;
	}

public:
	/** @todo do these two statics really belong here ? */
	static TiXmlHandle makeCalibrationHandle(TiXmlDocument* doc) {
		TiXmlHandle docHandle(doc);
		TiXmlHandle oldHand(docHandle.FirstChild("AcqCalibration"));
		TiXmlElement* calElement = oldHand.Element();

		if (calElement){
			DEPRECATED_WARNING;
			return oldHand;
		}else{
			TiXmlHandle newHand(docHandle.FirstChild("ACQ").
					     FirstChild("AcqCalibration"));
			calElement = newHand.Element();

			assert(calElement);
			return newHand;
		}
	}
	static const char* getModel(TiXmlDocument* doc) {
	
		TiXmlText* text = makeCalibrationHandle(doc).
			FirstChild("Info").FirstChild("Model").
			FirstChild().Text();

		if (text){
			return text->Value();
		}else{
			return 0;
		}
	}

	static const bool hasRangeSw(TiXmlDocument* doc) {
		TiXmlElement* data = makeCalibrationHandle(doc).
				FirstChild("Data").Element();
		assert(data);
		const char* sw = data->Attribute("SW");
		return sw && strcmp(sw, "default") != 0;
	}

	const char* getModel(void) {
		return getModel(doc);
	}

};


class CalibratedElement : public TiXmlElement {
 public:
	CalibratedElement(int ch, Range range) :
		TiXmlElement("Calibrated") {
		SetAttribute("ch", ch);
		SetDoubleAttribute("min", range.rmin);
		SetDoubleAttribute("max", range.rmax);
	}
};
/** What is the Name of a field (surely not an attribute)
 *  We hack around this by extracting the first N chars of Value
 */

#define CALIBRATED_NAME "Calibrated"

#define IS_CALIBRATED_NODE(node) (strcmp(node->Value(), CALIBRATED_NAME) == 0)

#define NOMINAL_NAME "Nominal"

#define IS_NOMINAL_NODE(node) (strcmp(node->Value(), NOMINAL_NAME) == 0)

void ConcreteInputBlock::putCalibratedRange(int channel, Range range) {
	RangeElement& re = *getCurrentRangeElement(channel);
	CalibratedElement cal_elt(channel, range);
	TiXmlElement* child_elt;

	/** sanity check */
	Range& nominal = re.cal_tab[0];
	if (!Range::sanityCheck(nominal, range)){
		err("Refusing to update cal for ch%02d sanity check failed",
		    channel);
		return;
	}

         
	/** replace any existing cal element */
	/** or add a new one */
	/** we keep the ch list ordered, and assume others likewise */
	for (TiXmlNode *child = 0; 
	     (child = re.element->IterateChildren(child)) != 0; ){

		if ((child_elt = child->ToElement()) == 0){
			continue;
		}
		int ch2 = 0;

		dbg(3, "child %s child_elt %s", 
		    child->Value(), child_elt->Value());

		if (IS_CALIBRATED_NODE(child) &&
		    child_elt->Attribute("ch", &ch2) != 0 ){

			dbg(3,"channel %d ch2 %d", channel, ch2);

			if (ch2 == channel){
				re.element->ReplaceChild(child_elt, cal_elt);
				dbg(2, "ReplaceChild() ch %d", channel);
				return;
			}else if (ch2 > channel){
				re.element->InsertBeforeChild(
					child_elt, cal_elt);
				dbg(2, "InsertBeforeChild() ch %d", channel);
				return;
			}
		}
	}

/** OK, so we failed to insert or replace an element - try append */

	dbg(2, "InsertEndChild() ch %d", channel);
	re.element->InsertEndChild(cal_elt);
}

class Acq216M2InputBlock : public ConcreteInputBlock {
 public:
	Acq216M2InputBlock(const char* fname, TiXmlDocument* _doc) :
		ConcreteInputBlock(fname, _doc, "acq216") {		
		setRangeSelector(new ConcreteRangeSelector(this, "M2", sw));
		dbg(1, "Hello");
	}
};

class Acq216M5InputBlock : public ConcreteInputBlock {
 public:
	Acq216M5InputBlock(const char* fname, TiXmlDocument* _doc) :
	ConcreteInputBlock(fname, _doc, "acq216") {
		const char* swphys = data->Attribute("SWPHYS");
		setRangeSelector(new ConcreteRangeSelector(this, "M5", swphys));
		dbg(1, "Hello");
	}
};

class Acq196InputBlock : public ConcreteInputBlock {
 public:
	Acq196InputBlock(const char* fname, TiXmlDocument* _doc) :
		ConcreteInputBlock(fname, _doc, "") {
		setRangeSelector(new DebugRangeSelector("default"));
		}
};

class Acq164InputBlock : public ConcreteInputBlock {
 public:
	Acq164InputBlock(const char* fname, TiXmlDocument* _doc) :
		ConcreteInputBlock(fname, _doc, "") {
		setRangeSelector(new DebugRangeSelector("default"));
		}
};

class Acq132InputBlock : public ConcreteInputBlock {

public:
	Acq132InputBlock(const char* fname, TiXmlDocument* _doc) :
	ConcreteInputBlock(fname, _doc, "acq132") {
//			setRangeSelector(new DebugRangeSelector("default"));
// @@todo .. two ranges wanted (but it core dumps)
			setRangeSelector(
				new ConcreteRangeSelector(this, "ACQ132", sw));
		}	
};


class Acq42xInputBlock : public ConcreteInputBlock {
	char root[128];
	const char* makeRoot() {
		sprintf(root, "acq400.%d.knobs/", site);
		return root;
	}
 public:
	Acq42xInputBlock(const char* fname, TiXmlDocument* _doc, int _site) :
		ConcreteInputBlock(fname, _doc, makeRoot(), _site) {
		setRangeSelector(new ConcreteRangeSelector(this, "acq420", sw));
		dbg(1, "Hello");
	}
};

class Acq400InputBlock : public ConcreteInputBlock {
 public:
	Acq400InputBlock(const char* fname, TiXmlDocument* _doc, int _site) :
		ConcreteInputBlock(fname, _doc, "", _site) {
			setRangeSelector(new DebugRangeSelector("default"));
		}
};
class Acq480InputBlock : public Acq400InputBlock {
	const char* dg;
 public:
	Acq480InputBlock(const char* fname, TiXmlDocument* _doc, int _site) :
		Acq400InputBlock(fname, _doc, _site) {
		dg = data->Attribute("DG");
		if (dg != 0){
			dbg(1, "dg set %s", dg);
		}
	}

	virtual Range getRange(int channel = 1) {
		Range r = ConcreteInputBlock::getRange(channel);

		if (dg != 0){
			char knob[32];
			char cmd[128];

			sprintf(knob, dg, channel);
			sprintf(cmd, "get.site %d %s", site, knob);
			FILE *pp = popen(cmd, "r");
			if (!pp){
				perror(cmd);
			}else{
				float db = 0;
				char rsp[128]; rsp[0] = '\0';

				if (fgets(rsp, 128, pp) &&
					sscanf(rsp, "%*s %f dB", &db) != 1){
					fprintf(stderr, "WARNING: cmd \"%s\" rsp \"%s\"\n", cmd, rsp);
				}
				pclose(pp);
				// more gain, less range, mult faster than div, use logs
				float g = pow(10, -db/20);

				dbg(1, "dB %.0f apply gain. range *= %.3f", db, g);
				r.rmax *= g;
				r.rmin *= g;
			}
		}
		return r;

	}
};

void ConcreteInputBlock::getVoltsRange(RangeElement& re) {
	TiXmlNode *nominal_node;
	Range* nominal = &re.cal_tab[0];

	if ((nominal_node = re.element->FirstChild("Nominal")) != 0){

		nominal_node->ToElement()->Attribute("min", &nominal->rmin);
		nominal_node->ToElement()->Attribute("max", &nominal->rmax);

		dbg(3,"getVoltsRange=%6.4fV,%6.4fV\n", 
		    nominal->rmin, nominal->rmax);
	}


	dbg(2, "fmt %s", 
	    re.element->Attribute("fmt") != 0?
	    re.element->Attribute("fmt"): "default");

	if (re.element->Attribute("fmt") != 0){
		nominal->setFormat(re.element->Attribute("fmt"));
	}	
	
	for (Range* defaults = &re.cal_tab[channelCount]; 
	     defaults != nominal;
	     --defaults){
		*defaults = *nominal;
	}
	
	TiXmlElement* element = 0;

	for (TiXmlNode *child = 0; 
	     (child = re.element->IterateChildren(child)) != 0 &&
	     (element = child->ToElement()) != 0; 	){

		int ic;

		if ((element->Attribute("ch", &ic)) != 0){

			dbg(3, "Channel ch %d min %s max %s\n",
			    ic, 
			    element->Attribute("min"), 
			    element->Attribute("max"));

			element->Attribute("min", &re.cal_tab[ic].rmin);
			element->Attribute("max", &re.cal_tab[ic].rmax);
		}
	}
}

class BlockFactory {
 public:
	static InputBlock* create(const char* fname, TiXmlDocument* doc, int site) {
		TiXmlHandle docHandle(doc);
		
		const char* model = ConcreteInputBlock::getModel(doc);

		if (model){
			dbg(2, "model.Value() %s	 PGMWASHERE", model);

			if (strstr(model, "M2")){
				return new Acq216M2InputBlock(fname, doc);
			}else if (strstr(model, "M5")){
				return new Acq216M5InputBlock(fname, doc);
			}else if (strstr(model, "ACQ196")){
				return new Acq196InputBlock(fname, doc);
			}else if (strstr(model, "ACQ164")){
				return new Acq164InputBlock(fname, doc);
			}else if (strstr(model, "ACQ132")){
				return new Acq132InputBlock(fname, doc);
			}else if (strncmp(model, "ACQ4",  4) == 0) {
				if (ConcreteInputBlock::hasRangeSw(doc)){
					if (strncmp(model, "ACQ42", 5) == 0 ||
					    strncmp(model, "ACQ437", 6) == 0){
						return new Acq42xInputBlock(fname, doc, site);
					}else if (strncmp(model, "ACQ48", 5) == 0) {
						return new Acq480InputBlock(fname, doc, site);
					}else{
						err("AcqCalibration.Info.Model %s with ranges"
							"NOT SUPPORTED", model);
					}
				}else{
					return new Acq400InputBlock(fname, doc, site);
				}
			}
		}else{
			err("AcqCalibration.Info.Model field not found");
		}
		return 0;
	}
};


InputBlock& InputBlock::getInstance(const char* fname, int site)
/* singleton no more */
{
	InputBlock* instance = 0;

	dbg(1, "%s fname:%s site:%d\n", _PFN, fname, site);

	TiXmlDocument* doc = new TiXmlDocument(fname);
	if (doc->LoadFile()){
		instance = BlockFactory::create(fname, doc, site);

		if (!instance){
			my_exit(-1);
		}
	}else{
		err("Cal File \"%s\" not a cal file, "
		      "run model default\n", fname);
		my_exit(-1);
	}
	return *instance;
}



#include "acq-util.h"

void ChannelSelection::init(int _maxchan, int deflen) {
	maxchan = _maxchan;
	channels = new int[maxchan+1]; /** index from 1 */

	memset(channels, 0, (maxchan+1)*sizeof(int));	

	def = new char[deflen+1];
	def[0] = '\0';
}
ChannelSelection::ChannelSelection(int _maxchan, const char* _def)
{
	init(_maxchan, strlen(_def));
	strcpy(def, _def);
	acqMakeChannelRange(channels, maxchan, def);
}

ChannelSelection::ChannelSelection(int _maxchan, int argc, const char* argv[])
{
	int deflen = 0;
	for (int iarg = 0; iarg != argc; ++iarg){
		deflen += strlen(argv[iarg]) + 1;
	}
	init(_maxchan, deflen);

	for (int iarg = 0; iarg != argc; ++iarg){
		strcat(def, argv[iarg]);
		strcat(def, " ");
	}
	acqMakeChannelRange(channels, maxchan, def);
}

ChannelSelection::ChannelSelection(int _maxchan, int all)
{
	init(_maxchan, 10);

	switch(all){
	case ALL:
		sprintf(def, "%d-%d", 1, _maxchan);
		break;
	case 0:
		strcpy(def, "");
	default:
		sprintf(def, "%d", all);
	}

	acqMakeChannelRange(channels, maxchan, def);
}

ChannelSelection::~ChannelSelection()
{
	delete [] def;
	delete [] channels;
}

const char* ChannelSelection::toString(void) const
// poor man's java - non re-entrant       
{
	static char buf[256];
	char* pbuf = buf;

	for (int ic = 1; ic < maxchan; ++ic){
		pbuf += sprintf(pbuf, "%d,", channels[ic]);
	}
	sprintf(pbuf, "%d\n", channels[maxchan]);
	return buf;
}

int Range::print(void) const
{
	if (reversed){
		return printf(fmt, rmax, rmin);
	}else{
		return printf(fmt, rmin, rmax);
	}
}


//#define SIGN(x)  ((x)>=0? 1: -1)

static int sanityCheck(double v1, double v2) {
	int sane = 0;
	const char *errmsg = 0;

	if (!isfinite(v2)){
		errmsg = "v2 not finite";
	}else if (v2 == 0){
		errmsg = "v2 is zero";
	}else if (SIGN(v1) != SIGN(v2) && ABS(v1) > 0.5 ){
	        errmsg = "sign change";
	}else if (v1 != 0.0 && ABS(v2/v1) > 1.5 ){
		errmsg = "v2 greater than 50% of nominal";
	}else if (v1 != 0.0 && ABS(v2/v1) < 0.5 ){
		errmsg = "v2 less than 50% of nominal";
	}else{
		sane = 1;
	}
	if (!sane && errmsg != 0){
		err("v1:%5.2fV v2:%5.2fV : %s", v1, v2, errmsg);
	}
	return sane;
}
int Range::sanityCheck(Range& nominal, Range& test)
{
	return  ::sanityCheck(nominal.rmin, test.rmin) &&
		::sanityCheck(nominal.rmax, test.rmax);
}

int Range::reversed = 0;
// valid for 16 bit - others MUST get value from xml
const char* Range::code_min = "-32768";	  
const char* Range::code_max = "32767";

#include "FileClosure.h"

int InputBlock::collateRegValues(
       char* ubuf, int ubuf_max)
/** collect register actuals (reverse order for little endianmess). */
{
	char *ubufp = ubuf;
	const char* swphys = getCalibrationHandle().
		FirstChild("Data").Element()->Attribute("SWPHYS");

	if (!swphys){
		swphys = getCalibrationHandle().
			FirstChild("Data").Element()->Attribute("SW");
	}

	dbg(1,"SWPHYS: identified as: %s\n", swphys);

	char* my_str = new char[strlen(swphys)+1];
	char *argv[10];
	
	strcpy(my_str, swphys);

	int ndevs = strsplit(my_str, argv, 10, ",");

	/** iterate in reverse order to pander to little-endian-mess */
	for (int idev = ndevs; idev--; ){

		dbg(1, "%d/%d %s\n", idev, ndevs, argv[idev]);

		FpClosure file(getDevRoot(), argv[idev], "write", "r");
		
		fgets(ubufp, ubuf_max - (ubufp-ubuf), file.getFp());
		if (index(ubufp, '\n')){
			*index(ubufp, '\n') = '\0';
		}
		ubufp += strlen(ubufp);          
	}

	delete [] my_str;

	return ubufp - ubuf;
}


