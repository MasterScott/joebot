
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

// BaseNeuron.h: interface for the CBaseNeuron class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__BASENEURON_H)
#define __BASENEURON_H


#include "NeuralNetDefs.h"
#include "BaseLink.h"
#include "Activations.h"
#include "stdio.h"

class CBaseNeuron  
{
	friend class CBaseNeuralNet;
public:
	virtual void Load(FILE *);
	virtual void Save(FILE *);
	CBaseNeuron();
	virtual ~CBaseNeuron();
	virtual void Calc(void) = 0;
	virtual void InitWeights(netvar_t,netvar_t) = 0;

	virtual CBaseLink *NewLink(void) = 0;

	float m_fX,m_fY;			// position on layer ( layers are on z )
};

#endif // !defined(__BASENEURON_H)
