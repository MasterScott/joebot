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
// WeightBProp.cpp: implementation of the CWeightBProp class.
//
//////////////////////////////////////////////////////////////////////

#include "WeightBProp.h"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeightBProp::CWeightBProp()
{

}

CWeightBProp::~CWeightBProp()
{

}

const CWeightBProp &CWeightBProp ::  operator = (const CWeightBProp &p){
	m_dWeight = p.m_dWeight;

	return ((const CWeightBProp &)(*this));
}

void CWeightBProp :: Init(netvar_t l,netvar_t h){
	m_dWeight = (netvar_t)rand() / (netvar_t)RAND_MAX * (h-l) + l;
}

void CWeightBProp::Load(FILE *fhd){
	CBaseWeightFF::Load(fhd);

	fread(&m_dWeight,sizeof(netvar_t),1,fhd);
}

void CWeightBProp::Save(FILE *fhd){
	CBaseWeightFF::Save(fhd);

	fwrite(&m_dWeight,sizeof(netvar_t),1,fhd);
}
