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
// BaseWeight.h: interface for the CBaseWeight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__BASEWEIGHT_H)
#define __BASEWEIGHT_H

#include "NeuralNetDefs.h"
#include <stdio.h>

class CBaseWeight  
{
public:
	virtual void Save(FILE *fhd);
	virtual void Load(FILE *fhd);
	CBaseWeight();
	virtual ~CBaseWeight();
	virtual void Init(netvar_t,netvar_t) = 0;

	virtual const CBaseWeight & operator = (const CBaseWeight &);
	long lUsed;
};

#endif // !defined(__BASEWEIGHT_H)
