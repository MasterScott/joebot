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
// WeightBPropM.cpp: implementation of the CWeightBPropM class.
//
//////////////////////////////////////////////////////////////////////

#include "WeightBPropM.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeightBPropM::CWeightBPropM()
{
	m_dMomentum = 0;
}

CWeightBPropM::~CWeightBPropM()
{

}

const CWeightBPropM &CWeightBPropM ::  operator = (const CWeightBPropM &p){
	m_dWeight = p.m_dWeight;
	m_dMomentum = p.m_dMomentum;

	return ((const CWeightBPropM &)(*this));
}

void CWeightBPropM :: Init(netvar_t l,netvar_t h){
	m_dMomentum = 0;
	CWeightBProp::Init(l,h);
}

void CWeightBPropM::Load(FILE *fhd){
	CWeightBProp::Load(fhd);

	fread(&m_dMomentum,sizeof(netvar_t),1,fhd);
}

void CWeightBPropM::Save(FILE *fhd){
	CWeightBProp::Save(fhd);

	fwrite(&m_dMomentum,sizeof(netvar_t),1,fhd);
}