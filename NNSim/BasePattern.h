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
// BasePattern.h: interface for the CBasePattern class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__BASEPATTERN_H)
#define __BASEPATTERN_H


#include "NeuralNetDefs.h"
#include "BasePatternElem.h"

#define _LODL_NORMAL	0
#define _LODL_FINE		1
#define _LODL_1			1

class CBasePattern  
{
public:
	void LogErrors(bool,int iLODL = 0);
	void Log2File(FILE *fhd);
	virtual long GetPatternNum(void);
	CBasePattern();
	virtual ~CBasePattern();
	virtual CBasePatternElem *AddPattern(void);
	virtual CBasePatternElem *NewPattern(void);

	virtual void Save(FILE *fhd);
	virtual void SaveText(FILE *fhd);
	virtual void Load(FILE *fhd);

	virtual bool SaveFile(const char *);
	virtual bool SaveFileText(const char *);
	virtual bool LoadFile(const char *);

	CBasePatternElem *operator [] (long);

	CBasePatternElem **ppPattern;
	long lNumPattern;
	long lMaxPattern;
	bool m_bLogErrors;

	long m_lPTrain;

	FILE *fhdLog;
	int m_iLODL;			// LOD of log
};

#endif // !defined(__BASEPATTERN_H)
