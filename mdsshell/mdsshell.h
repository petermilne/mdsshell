/* ------------------------------------------------------------------------- */
/* mdsshell.h                                                                */
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
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                */
/* ------------------------------------------------------------------------- */

#ifndef __MDSSHELL_H__
#define __MDSSHELL_H__

#define MAXEXP	4096

int mdsshell_doServerCommand(struct IoBuf *cmd);
void mdsshell_iterateCommands(
	void (*visitor)(void *vobj, const char* command), 
	void* vobj);

void mdsshell_help(void);
#endif
