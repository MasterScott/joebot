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
// BasePatternElemFF.h: interface for the CBasePatternElemFF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__BASEPATTERNELEMFF_H)
#define __BASEPATTERNELEMFF_H

#include "BasePatternElem.h"

class CBasePatternElemFF : public CBasePatternElem  
{
public:
	CBasePatternElemFF();
	virtual ~CBasePatternElemFF();
	void SetDim(long,long);

	virtual void Save(FILE *fhd);
	virtual void Load(FILE *fhd);

	virtual void SaveText(FILE *fhd);
	virtual void LoadText(FILE *fhd);

	virtual void SetInput(netvar_t *p);
	virtual void SetOutput(netvar_t *p);

	netvar_t *m_pdIn;
	netvar_t *m_pdOut;

	long m_lInDim;
	long m_lOutDim;

	netvar_t m_dMaxError;
	netvar_t m_dMinError;
	netvar_t m_dAvError;
};

#endif // !defined(__BASEPATTERNELEMFF_H)
