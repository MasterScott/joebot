/******************************************************************************

    JoeBOT - a bot for Counter-Strike
    Copyright (C) 2000-2002  Johannes Lampel

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

******************************************************************************/
#ifndef __SOMPATTERNELEM_H
#define __SOMPATTERNELEM_H

// SOMPatternElem.h by Johannes Lampel / LampelSoft
// (c) Johannes Lampel / LampelSoft
// written Sep 27th 2000

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#include "nVec.h"

class DllExport SOMPatternElem:protected nVec{
	friend class SOMPattern;
public:
	SOMPatternElem();			// ctor
	~SOMPatternElem();			// dtor
protected:
	double dLastError;			// just the last error, which has been calced last m::cat
private:
};

#endif  __SOMPATTERNELEM_H