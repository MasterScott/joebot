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
// BasePatternFF.h: interface for the CBasePatternFF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__BASEPATTERNFF_H)
#define __BASEPATTERNFF_H

#include "BasePattern.h"
#include "BasePatternElemFF.h"
#include "BaseNeuralNetFF.h"

class CBasePatternFF : public CBasePattern  
{
public:
	CBasePatternFF();
	virtual ~CBasePatternFF();

	virtual CBasePatternElem *AddPattern(netvar_t *,netvar_t*);
	virtual CBasePatternElem *NewPattern(void);

	virtual void SetNN(CBaseNeuralNetFF *);
	virtual void TeachEpoch(void) = 0;
	virtual void TeachPartoEpoch(long) = 0;

	virtual void Save(FILE *fhd);
	virtual void Load(FILE *fhd);

	long m_lInDim,
		m_lOutDim;

	CBaseNeuralNetFF *m_pNN;

	netvar_t m_dMinError;
	netvar_t m_dMaxError;
	netvar_t m_dAvError;
};

#endif // !defined(__BASEPATTERNFF_H)
