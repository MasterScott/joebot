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
// NeuronBProp.cpp: implementation of the CNeuronBProp class.
//
//////////////////////////////////////////////////////////////////////

#include "NeuronBProp.h"
#include "memory.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNeuronBProp::CNeuronBProp()
{
	m_dBias = 0;
	m_dDelta = 0;
}

CNeuronBProp::~CNeuronBProp()
{

}

void CNeuronBProp :: Calc(void){
	long lIN;
	CLinkBProp *p;

	m_dNet = 0;

	for(lIN = lLFromNum-1;lIN>-1;lIN--){
		p = (CLinkBProp*)(ppLinksFrom[lIN]);
		m_dNet += ((CWeightBProp*)(p->pW))->m_dWeight * ((CNeuronBProp *)(p->p))->m_dOut;
	}

	m_dAct = m_pfAct(m_dNet - m_dBias);
	m_dOut = m_dAct;				// identity as output function
}
#include <iostream.h>
void CNeuronBProp :: CalcDelta(void){
	if(m_bHidden){
		long lIN;
		CLinkBProp *p;

		m_dDelta = 0;

		for(lIN = lLToNum-1;lIN>-1;lIN--){
			p = (CLinkBProp*)(ppLinksTo[lIN]);
			m_dDelta += ((CWeightBProp*)(p->pW))->m_dWeight * ((CNeuronBProp *)(p->p))->m_dDelta;
		}

		m_dDelta *= m_pfActS(m_dNet);
	}
	else{		// isn't hidden -> is output neuron
		m_dDelta = m_pfActS(m_dNet) * (m_dTI - m_dOut);
	}
}

void CNeuronBProp::Save(FILE *fhd)
{
	CBaseNeuronFF::Save(fhd);
}

void CNeuronBProp::Load(FILE *fhd)
{
	CBaseNeuronFF::Load(fhd);
}

CBaseLink * CNeuronBProp :: NewLink(void){
	return new CLinkBProp;
}