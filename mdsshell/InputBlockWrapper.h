/* ------------------------------------------------------------------------- */
/* InputBlockWrapper.h  D-TACQ ACQ400 FMC  DRIVER                                   
 * Project: MDSSHELL
 * Created: 11 Jul 2016  			/ User: pgm
 * ------------------------------------------------------------------------- *
 *   Copyright (C) 2016 Peter Milne, D-TACQ Solutions Ltd         *
 *                      <peter dot milne at D hyphen TACQ dot com>           *
 *                                                                           *
 *  This program is free software; you can redistribute it and/or modify     *
 *  it under the terms of Version 2 of the GNU General Public License        *
 *  as published by the Free Software Foundation;                            *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program; if not, write to the Free Software              *
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                *
 *
 * TODO 
 * TODO
\* ------------------------------------------------------------------------- */

#ifndef MDSSHELL_INPUTBLOCKWRAPPER_H_
#define MDSSHELL_INPUTBLOCKWRAPPER_H_

#include "../vin/InputBlock.h"

class InputBlockWrapper {

public:
	virtual ~InputBlockWrapper() {};
	static InputBlockWrapper& create(int site);

	virtual Range getRange(int channel = 1) = 0;
	virtual int getNumChannels(void) = 0;
};


#endif /* MDSSHELL_INPUTBLOCKWRAPPER_H_ */
