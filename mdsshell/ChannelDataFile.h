/* ------------------------------------------------------------------------- */
/* ChannelDataFile.h - models channel data file                              */
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

class ChannelDataFile {
protected:
	ChannelDataFile() 
	{}

public:
	virtual ~ChannelDataFile()
	{}
	virtual const char* process(int channel) = 0;
	/** process the data for channel (if any), return path to data */

	static ChannelDataFile& instance(const char* spec = 0);
	/** factory method */
};
