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

// PatternElemBProp.h: interface for the CPatternElemBProp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__PATTERNELEMBPROP_H)
#define __PATTERNELEMBPROP_H

#include "BasePatternElemFF.h"

class CPatternElemBProp : public CBasePatternElemFF  
{
public:
	CPatternElemBProp();
	virtual ~CPatternElemBProp();

	virtual void Save(FILE *fhd);
	virtual void Load(FILE *fhd);

	virtual void SaveText(FILE *fhd);
	virtual void LoadText(FILE *fhd);
};

#endif // !defined(__PATTERNELEMBPROP_H)
