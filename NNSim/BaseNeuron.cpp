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
// BaseNeuron.cpp: implementation of the CBaseNeuron class.
//
//////////////////////////////////////////////////////////////////////

#include "BaseNeuron.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseNeuron::CBaseNeuron()
{
	m_fX = 0;
	m_fY = 0;
}

CBaseNeuron::~CBaseNeuron()
{
	
}

void CBaseNeuron::Save(FILE *fhd)
{
	fwrite(&m_fX,sizeof(float),1,fhd);
	fwrite(&m_fY,sizeof(float),1,fhd);
}

void CBaseNeuron::Load(FILE *fhd)
{
	fread(&m_fX,sizeof(float),1,fhd);
	fread(&m_fY,sizeof(float),1,fhd);
}
