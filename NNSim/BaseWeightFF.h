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
// BaseWeightFF.h: interface for the CBaseWeightFF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__BASEWEIGHTFF_H)
#define __BASEWEIGHTFF_H

#include "BaseWeight.h"

class CBaseWeightFF : public CBaseWeight
{
public:
	virtual void Save(FILE *fhd);
	virtual void Load(FILE *fhd);
	CBaseWeightFF();
	virtual ~CBaseWeightFF();

	virtual void Init(netvar_t,netvar_t) = 0;

	void SetWeight(netvar_t &d);
	netvar_t GetWeight(void);

	virtual const CBaseWeightFF & operator = (const CBaseWeightFF &);
protected:
	netvar_t m_dWeight;
};

#endif // !defined(__BASEWEIGHTFF_H)
