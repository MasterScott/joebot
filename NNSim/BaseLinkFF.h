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


// BaseLinkFF.h: interface for the CBaseLinkFF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__BASELINKFF_H)
#define __BASELINKFF_H

#include "BaseLink.h"
#include "BaseNeuron.h"

class CBaseLinkFF : public CBaseLink  
{
public:
	virtual void Load(FILE *fhd);
	virtual void Save(FILE *fhd);
	CBaseLinkFF();
	virtual ~CBaseLinkFF();

	CBaseNeuron *p;
};

#endif // !defined(__BASELINKFF_H)
