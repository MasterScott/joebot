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
// BasePattern.cpp: implementation of the CBasePattern class.
//
//////////////////////////////////////////////////////////////////////

#include "BasePattern.h"
#include <memory.h>
#include <iostream.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasePattern::CBasePattern()
{
	lNumPattern = lMaxPattern = 0;
	ppPattern = 0;
	m_lPTrain = 0;
	fhdLog = 0;
}

CBasePattern::~CBasePattern()
{
	long lschl;
	if(ppPattern){
		for(lschl = 0;lschl < lNumPattern;lschl ++){
			delete ppPattern[lschl];
		}
		delete [] ppPattern;
		ppPattern = 0;
	}
}

CBasePatternElem *CBasePattern :: AddPattern(void){
	CBasePatternElem *p;
	if(lNumPattern >= lMaxPattern){
		// at end of field -> alloc new 
		lMaxPattern += _ARRAY_SEG_SIZE_PATTERN;
		CBasePatternElem **pNew = new CBasePatternElem*[lMaxPattern];
		if(ppPattern){
			//memset(pNew,0,lMaxPattern*sizeof(CBaseWeight*));
			memcpy(pNew,ppPattern,sizeof(CBasePattern*)*lNumPattern);// copy from old to new
			delete [] ppPattern;									// and delete it
		}

		for(long lschl = lNumPattern;lschl < lMaxPattern;lschl++){		// alloc new Pattern
			pNew[lschl] = NewPattern();
		}
		ppPattern = pNew;											// copy pointer to new data to var
	}

	p = ppPattern[lNumPattern];

	lNumPattern ++;
	return p;
}

CBasePatternElem *CBasePattern :: NewPattern(void){
	return new CBasePatternElem;
}

long CBasePattern::GetPatternNum(void)
{
	return lNumPattern;
}

bool CBasePattern::SaveFile(const char *szFileName)
{
	FILE *fhds;

	fhds = fopen(szFileName,"wb");

	if(!fhds){
		cout << "couldn't write file : "<<szFileName<<endl;
		return false;
	}

	Save(fhds);

	fclose(fhds);
	return true;
}

bool CBasePattern::SaveFileText(const char *szFileName)
{
	FILE *fhds;

	fhds = fopen(szFileName,"w");

	if(!fhds){
		cout << "couldn't write file : "<<szFileName<<endl;
		return false;
	}

	SaveText(fhds);

	fclose(fhds);
	return true;
}

bool CBasePattern::LoadFile(const char *szFileName)
{
	FILE *fhd;

	if(!(fhd = fopen(szFileName,"rb"))){
		return false;
	}

	Load(fhd);

	fclose(fhd);
	return true;
}

void CBasePattern :: Load(FILE *fhd){
	long lType,lschl,lFileNum;

	if(ppPattern){
		for(lschl = 0;lschl < lNumPattern;lschl ++){
			delete ppPattern[lschl];
		}
		delete [] ppPattern;
		ppPattern = 0;
	}
	lNumPattern = 0;
	lMaxPattern = 0;

	fread(&lType,sizeof(long),1,fhd);
	fread(&lFileNum,sizeof(long),1,fhd);

	//ppPattern = new CBasePatternElem*[lNumPattern];
	
	for(lschl = 0;lschl < lFileNum;lschl++){
		AddPattern()->Load(fhd);
	}
}

void CBasePattern :: Save(FILE *fhd){
	long lType = 0;

	fwrite(&lType,sizeof(long),1,fhd);
	fwrite(&lNumPattern,sizeof(long),1,fhd);
	//cout <<lNumPattern<<endl;
	
	for(long lschl = 0;lschl < lNumPattern;lschl++){
		ppPattern[lschl]->Save(fhd);
	}
}

void CBasePattern :: SaveText(FILE *fhd){
	long lType = 0;

	fprintf(fhd,"%li\n%li\n\n\n",lType,lNumPattern);
	//cout <<lNumPattern<<endl;
	
	for(long lschl = 0;lschl < lNumPattern;lschl++){
		ppPattern[lschl]->SaveText(fhd);
	}
}

void CBasePattern :: LogErrors(bool bP,int iLODL){
	m_bLogErrors = bP;
	m_iLODL = iLODL;
}

void CBasePattern :: Log2File(FILE *fhd){
	fhdLog = fhd;
	long ltime = time(0);
	if(fhd)fprintf(fhd,"<center>%s</center><table>",asctime(localtime(&ltime)));
}

CBasePatternElem *CBasePattern :: operator [] (long lP){
	return ppPattern[lP];
}