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

// BasePatternElem.h: interface for the CBasePatternElem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__BASEPATTERNELEM_H)
#define __BASEPATTERNELEM_H


#include "NeuralNetDefs.h"
#include <stdio.h>

class CBasePatternElem  
{
public:
	CBasePatternElem();
	virtual ~CBasePatternElem();

	virtual void Save(FILE *fhd);
	virtual void Load(FILE *fhd);

	virtual void SaveText(FILE *fhd);
	virtual void LoadText(FILE *fhd);
};

#endif // !defined(__BASEPATTERNELEM_H)
