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
#include "SOMPattern.h"

SOMPattern :: SOMPattern (const dim_c dcDim){
	pFPattern = 0;
	lPDimNum = dcDim;
	lActPPos = 0;
}

SOMPattern :: ~SOMPattern (){
	if(pFPattern) delete [] pFPattern;
}

int SOMPattern :: SetMaxPatternNum(const long lNum){
	long lschl;
	lPNum = lNum;

	if(pFPattern) delete [] pFPattern;

	pFPattern = new SOMPatternElem[lNum];
	for(lschl=0;lschl < lNum;lschl++){
		pFPattern[lschl].SetNumDim(lPDimNum);
	}
	return 1;
}

long SOMPattern :: AddPattern(const nVec &VParam){
	try{
		if(VParam.lCDim != lPDimNum)
			throw IncompatibleVectors();
		
		return AddPattern(VParam.ppdDim);
	}
	catch(IncompatibleVectors IV){
		cerr << "IncompatibleVectors thrown in SOMPattern :: AddPattern" << endl;
		throw IV;
	}
}

long SOMPattern :: AddPattern(const double *dpFParam){
	try{
		if(!pFPattern)
			throw SetMaxPatternwasntcalled();

		if(lActPPos >= lPNum)
			return -1;

		pFPattern[lActPPos].iType = nVT_copy;
		pFPattern[lActPPos].lCDim = lPDimNum;
		memcpy(pFPattern[lActPPos].ppdDim,dpFParam,sizeof(double) * lPDimNum);
		pFPattern[lActPPos].dLastError = 0.0;
		
		lActPPos ++;
		
		return lActPPos;
	}
	catch(SetMaxPatternwasntcalled SM){
		cout << "SetMaxPatternwasntcalled was thrown by SOMPattern :: AddPattern" << endl;
		throw SM;
	}
}

int SOMPattern :: SetSOM (qSOM2d *pSOM){
	SActSOM = pSOM;
	return 1;
}

int SOMPattern :: TeachEpoch(const double &dLearn,const double &dDist){
	long lschl;
	SActSOM -> SetDistP(dDist);
	SActSOM -> SetLRate(dLearn);
	for(lschl=0;lschl < lActPPos;lschl ++){
		pFPattern[lschl].dLastError = SActSOM->Categorize(pFPattern[lschl].ppdDim);
	}
	return 1;
}

int SOMPattern :: TeachEpochFast(const double &dLearn,const double &dDist){
	long lschl;
	SActSOM -> SetDistP(dDist);
	SActSOM -> SetLRate(dLearn);
	for(lschl=0;lschl < lActPPos;lschl ++){
		pFPattern[lschl].dLastError = SActSOM->CategorizeFast(pFPattern[lschl].ppdDim);
	}
	return 1;
}

int SOMPattern :: Load (const char *szName){
	FILE *fhd;
	if(fhd = fopen(szName,"rb")){
		Load(fhd);
		fclose(fhd);
		return true;
	}
	else{
		// failed
		cerr << "couldn't open file in SOMPattern::load" << endl;
		fclose(fhd);
		return false;
	}
}

int SOMPattern :: Save (const char *szName){
	FILE *fhd;
	if(fhd = fopen(szName,"wb")){
		Save(fhd);
		fclose(fhd);
		return true;
	}
	else{
		// failed
		cerr << "couldn't open file in SOMPattern::Save" << endl;
		fclose(fhd);
		return false;
	}
}

int SOMPattern :: Save(FILE *fhd){
	long lschl;

	fwrite(&lActPPos,sizeof(long),1,fhd);
	fwrite(&lPDimNum,sizeof(long),1,fhd);
	
	for(lschl=0;lschl < lActPPos;lschl++){
		pFPattern[lschl].Save(fhd);
	}
	return true;
}

int SOMPattern :: Load (FILE *fhd){
	long lschl;

	if(pFPattern) delete [] pFPattern;

	fread(&lPNum,sizeof(long),1,fhd);
	lActPPos = lPNum;
	pFPattern = new SOMPatternElem[lPNum];
	fread(&lPDimNum,sizeof(long),1,fhd);
	for(lschl=0;lschl < lPNum;lschl++){
		pFPattern[lschl].SetNumDim(lPDimNum);
		pFPattern[lschl].Load(fhd);
	}
	return true;
}
