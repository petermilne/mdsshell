/* ------------------------------------------------------------------------- */
/* timebase.h - control sample read stride readback                          */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2005 Peter Milne, D-TACQ Solutions Ltd
 *                      <Peter dot Milne at D hyphen TACQ dot com>
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of Version 2 of the GNU General Public License
 as published by the Free Software Foundation;
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, LSA.                */
/* ------------------------------------------------------------------------- */

class Range;

class Timebase {

protected:
	static int verbose;
	Timebase();
public:
	virtual ~Timebase() {} 

	virtual int getStart() = 0;	  /** start sample.         */
	virtual int getSamples() = 0;	  /** #samples in timebase  */
	virtual int getStride() = 0;	  /** stride in readout.    */
	virtual int getStime() = 0;       /** time between samples. nsec */

	virtual int getS0() = 0;       /** first sample relative to trigger */
	virtual int getS1() { return 0; }
	virtual int getS2() = 0;	/** last sample relative to trigger */

	virtual const char* getDt() = 0; /** delta time, inc stride, seconds*/

	virtual int getDecimation(Range& range) { return 1; }

	const char* toString() {
		static char buf[64];
		sprintf(buf, "%d,%d,%d", 
			getStart(), getSamples(), getStride());
		return buf;
	}

	/** factory method. */
	static Timebase* create(const char* timebase, int restore = 1);
};
