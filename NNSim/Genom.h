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
// Genom.h: interface for the CGenom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__GENOM_H)
#define __GENOM_H

#include "NeuralNetBProp.h"
#include "NeuralNetBPropM.h"
#include "PatternBProp.h"
#include "memory.h"
#include "string.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGenom  
{
public:
	CGenom();
	virtual ~CGenom();
	void Init(void);
	bool CreateNet(CBaseNeuralNet **p);			// create a net based on the genetic information stored
	bool SaveDescription(const char *szName);
	bool GetFFNet(CBaseNeuralNetFF *p);		// create genetic information from the net
	bool Save(const char *szFileName, bool bRemoveExt = false);
	bool SaveTopData(const char *szFileNameP);
	bool SaveBiases(const char *szFileNameP);
	bool SaveWeights(const char *szFileNameP);
	bool Load(const char *szFileName, bool bRemoveExt = false);
	bool LoadTopData(const char *szFileNameP);
	bool LoadBiases(const char *szFileNameP);
	bool LoadWeights(const char *szFileNameP);
	void WannaAddToken(int iSize=1);
	void WannaAddWeight(int iSize=1);
	void WannaAddBias(int iSize=1);
	void Mutation(float fPropability, float fRange);
	bool Crossing(float fCut, CGenom &GOtherGenom);
//protected:
	long lNumTopData;
	long lNumTopDataAlloc;
	short *psTopData;
	long lNumWeights;
	long lNumWeightsAlloc;
	netvar_t *pWeights;
	long lNumBiases;
	long lNumBiasesAlloc;
	netvar_t *pBiases;
};

struct GenomFileHeader_s{
	char szDescription[32];
	int iType;
	int iVersion;
};

#define __CGENOMVERSION 1
#define _FILEEXT_TOPDATA "tpd"
#define _TOP_DATA 1
#define _FILEEXT_BIASES "bia"
#define _BIAS_DATA 2
#define _FILEEXT_WEIGHTS "wgs"
#define _WEIGHT_DATA 3

#endif // !defined(__GENOM_H)
