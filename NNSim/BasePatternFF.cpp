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
// BasePatternFF.cpp: implementation of the CBasePatternFF class.
//
//////////////////////////////////////////////////////////////////////

#include "BasePatternFF.h"
#include "BasePatternElemFF.h"
#include <memory.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasePatternFF::CBasePatternFF()
{
	m_lInDim = 0;
	m_lOutDim = 0;
}

CBasePatternFF::~CBasePatternFF()
{

}

CBasePatternElem *CBasePatternFF :: AddPattern(netvar_t *pIn,netvar_t*pOut){
	CBasePatternElemFF *p= (CBasePatternElemFF*)CBasePattern::AddPattern();

	p->SetDim(m_lInDim,m_lOutDim);

	p->SetInput(pIn);
	p->SetOutput(pOut);
	return p;
}

CBasePatternElem *CBasePatternFF :: NewPattern(void){
	return new CBasePatternElemFF;
}

void CBasePatternFF :: SetNN(CBaseNeuralNetFF *pNN){
	m_lInDim = pNN->GetInputNum();
	m_lOutDim = pNN->GetOutputNum();

	m_pNN = pNN;
}

void CBasePatternFF :: Load(FILE *fhd){
	CBasePattern::Load(fhd);
}

void CBasePatternFF :: Save(FILE *fhd){
	CBasePattern::Save(fhd);
}
