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
// WeightBProp.h: interface for the CWeightBProp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__WEIGHTBPROP_H)
#define __WEIGHTBPROP_H

#include "BaseWeightFF.h"

class CWeightBProp : public CBaseWeightFF  
{
	friend class CNeuralNetBProp;
	friend class CNeuralNetBPropM;
	friend class CNeuronBProp;
	friend void InitObjects(void);
public:
	virtual void Save(FILE *fhd);
	virtual void Load(FILE *fhd);
	CWeightBProp();
	virtual ~CWeightBProp();

	virtual void Init(netvar_t,netvar_t);

	virtual const CWeightBProp & operator = (const CWeightBProp &);
protected :
};

#endif // !defined(__WEIGHTBPROP_H)
