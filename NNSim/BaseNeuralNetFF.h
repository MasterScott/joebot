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
// BaseNeuralNetFF.h: interface for the CBaseNeuralNetFF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__BASENEURALNETFF_H)
#define __BASENEURALNETFF_H

#include "BaseNeuralNet.h"
#include "BaseNeuron.h"
#include "BaseNeuronFF.h"
#include "BaseWeightFF.h"

class CBaseNeuralNetFF : public CBaseNeuralNet  
{
	friend void InitObjects(void);
public:
	virtual void GetOutputErrors(netvar_t &,netvar_t &,netvar_t &);
	void SetLRate(const netvar_t &);
	netvar_t GetLRate(void);
	virtual void InitWeights(netvar_t, netvar_t);
	virtual long AddWeight(void);
	virtual void CopyTI2N(void);
	virtual bool SetTInput(netvar_t *);
	virtual void GetOutput(netvar_t *);
	virtual void SetInput(netvar_t *);
	virtual long GetConnectionNum(void);
	virtual long GetWeightNum(void);
	virtual long GetOutputNum(void);
	virtual long GetInputNum(void);
	virtual bool ConnectNeurons(long,long,long,long,long);
	virtual bool ConnectLayer(long,long);
	virtual bool SetNNeuronsOnLayer(long, long);
	long lLayerNum;
	virtual bool SetLayerNum(long);
	virtual bool AllocNeurons(void);
	virtual bool Propagate(void) = 0;
	CBaseNeuralNetFF();
	virtual ~CBaseNeuralNetFF();
	
	void GetNPosition(CBaseNeuron *p, long &lL,long &lN);
	CBaseNeuronFF *GetNeuron(long,long);
	void RecalcPositions(void);

	virtual CBaseNeuron* NewNeuron(void) = 0;
	virtual CBaseWeight* NewWeight(void) = 0;

	long m_lNumProp;
	float *m_pfNDist;
	long *m_plNPR;
//protected :
	virtual void Save(FILE *fhd);
	virtual void Load(FILE *fhd);

	long *plLNNum;
	//long lMaxNpLayer;
	CBaseNeuron ***pppNeurons;
	netvar_t *m_pTI;			// teaching input
	netvar_t m_dLRate;
};
#define NT_BASENEURALNETFF 2

#endif // !defined(__BASENEURALNETFF_H)
