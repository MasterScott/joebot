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
// NeuralNetBProp.h: interface for the CNeuralNetBProp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__NEURALNETBPROP_H)
#define __NEURALNETBPROP_H

#include "BaseNeuralNetFF.h"
#include "NeuronBProp.h"
#include "WeightBProp.h"

class CNeuralNetBProp : public CBaseNeuralNetFF  
{
public:
	virtual bool BackPropagate(void);
	CNeuralNetBProp();
	virtual ~CNeuralNetBProp();
	virtual bool Propagate(void);
	virtual CBaseNeuron* NewNeuron(void);
	virtual CBaseWeight* NewWeight(void);

	long m_lNumBProp;
protected:
	virtual void Save(FILE *fhd);
	virtual void Load(FILE *fhd);
};
#define NT_NEURALNETBPROP 3

#endif // !defined(__NEURALNETBPROP_H)
