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

// WeightBPropM.h: interface for the CWeightBPropM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__WEIGHTBPROPM_H)
#define __WEIGHTBPROPM_H

#include "WeightBProp.h"

class CWeightBPropM : public CWeightBProp  
{
	friend class CNeuralNetBPropM;
public:
	CWeightBPropM();
	virtual ~CWeightBPropM();

	virtual void Save(FILE *fhd);
	virtual void Load(FILE *fhd);

	virtual void Init(netvar_t,netvar_t);

	virtual const CWeightBPropM & operator = (const CWeightBPropM &);
protected:
	netvar_t m_dMomentum;
};

#endif // !defined(__WEIGHTBPROPM_H)
