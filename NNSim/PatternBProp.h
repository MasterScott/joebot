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

// PatternBProp.h: interface for the CPatternBProp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__PATTERNBPROP_H)
#define __PATTERNBPROP_H

#include "BasePatternFF.h"

class CPatternBProp : public CBasePatternFF
{
public:
	CPatternBProp();
	virtual ~CPatternBProp();

	virtual CBasePatternElem *NewPattern(void);

	virtual void TeachEpoch(void);
	virtual void GetErrors(void);
	virtual void TeachPartoEpoch(long);

	virtual void Save(FILE *fhd);
	virtual void Load(FILE *fhd);
};

#endif // !defined(__PATTERNBPROP_H)
