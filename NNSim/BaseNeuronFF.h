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
// BaseNeuronFF.h: interface for the CBaseNeuronFF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__BASENEURONFF_H)
#define __BASENEURONFF_H


#include "BaseNeuron.h"
#include "BaseLinkFF.h"
class CGenom;

class CBaseNeuronFF : public CBaseNeuron  
{
	friend class CBaseNeuralNetFF;
	friend class CNeuralNetBProp;
	friend class CNeuralNetBPropM;
	friend class CGenom;
	friend void InitObjects(void);
public:
	virtual void Save(FILE *);
	virtual void Load(FILE *);
	CBaseNeuronFF();
	virtual ~CBaseNeuronFF();

	virtual CBaseLink *AddLinkFrom(long lW, CBaseWeight *pW,CBaseNeuron *);
	virtual CBaseLink *AddLinkTo(long lW, CBaseWeight *pW,CBaseNeuron *);

	virtual CBaseLink *NewLink(void) = 0;

	virtual void Calc(void) = 0;
	virtual void CalcDelta(void) = 0;
	virtual void InitWeights(netvar_t,netvar_t);
	void SetBias(netvar_t &);
protected:
	long lLToNum;
	long lLToMax;
	CBaseLink **ppLinksTo;
	long lLFromNum;
	long lLFromMax;
	CBaseLink **ppLinksFrom;

	netvar_t	m_dBias;		// bias
	netvar_t	m_dNet;			// net
	netvar_t	m_dAct;			// activation
	netvar_t	m_dOut;			// output
	netvar_t	m_dDelta;		// delta
	netvar_t	m_dTI;
	bool		m_bHidden;

	int m_iTypeAF;				// type of activation function
	netvar_t (*m_pfAct)	(const netvar_t &);		// activation function
	netvar_t (*m_pfActS)	(const netvar_t &);		// first derivative of activation function
};

#endif // !defined(__BASENEURONFF_H)
