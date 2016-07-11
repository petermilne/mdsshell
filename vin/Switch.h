/*
 * Switch.h - abstracts multi bit switch
 */


#ifndef __SWITCH_H__
#define __SWITCH_H__

class Switch {
 protected:
	Switch() {}
 public:
	virtual int getState(int channel) = 0;
	virtual int setState(int channel, int hi) = 0;
	virtual int read() = 0;
	virtual int write() = 0;
	virtual ~Switch() {}
};

class SwitchFactory {
	static unsigned short* ebuf;   /** switch emulation */

 public:
	static Switch* create(
		const char* model, int site, int is,
		const char* root, const char*file);

	static void enableEmulation(unsigned short* _ebuf);
};



#endif /* __SWITCH_H__ */
