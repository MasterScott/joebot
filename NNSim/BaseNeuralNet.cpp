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
// BaseNeuralNet.cpp: implementation of the CBaseNeuralNet class.
//
//////////////////////////////////////////////////////////////////////

#include "BaseNeuralNet.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseNeuralNet::CBaseNeuralNet()
{
	srand(time(0));					// random seed

	lMaxWeights = 0;
	lNumWeights = 0;
	ppWeights = 0;
	szVersion = 0;

	m_fNLDist = 4.0;

	char szTCVersion[] = "LampelSoft LSNNSI 3.0.20";

	szVersion = new char[strlen(szTCVersion)+1];
	strcpy(szVersion,szTCVersion);

	memset(m_szType,0,sizeof(char)*_LTYPEI);
	strcpy(m_szType,"CBaseNeuralNet");
	m_iType = NT_BASENEURALNET;
}

CBaseNeuralNet::~CBaseNeuralNet()
{
	if ( szVersion )
		delete [] szVersion;
	if(ppWeights){
		for(long lschl = 0;lschl < lMaxWeights;lschl ++){
			delete ppWeights[lschl];
		}
	}
}

char * CBaseNeuralNet::Version()
{
	return szVersion;
}

void CBaseNeuralNet::InitWeights(netvar_t l,netvar_t h){
	if(ppWeights){
		for(long lschl=0;lschl < lNumWeights;lschl++){
			ppWeights[lschl]->Init(l,h);
		}
	}
}

bool CBaseNeuralNet::SaveFile(const char *szFileName)
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

bool CBaseNeuralNet::LoadFile(const char *szFileName)
{
	FILE *fhd;

	if(!(fhd = fopen(szFileName,"rb"))){
		return false;
	}

	Load(fhd);

	fclose(fhd);
	return true;
}

void CBaseNeuralNet::Save(FILE *fhd){
	long lschl;

	fwrite(m_szType,sizeof(char),40,fhd);
	fwrite(&m_fNLDist,sizeof(float),1,fhd);
	fwrite(&lNumWeights,sizeof(long),1,fhd);
	for(lschl = 0;lschl < lNumWeights;lschl++){
		ppWeights[lschl]->Save(fhd);
	}
}

void CBaseNeuralNet::Load(FILE *fhd){
	long lLWeights,
		lschl,
		lAWeight;

	// delete all
	if(ppWeights){
		for(long lschl = 0;lschl < lMaxWeights;lschl ++){
			delete ppWeights[lschl];
		}
	}
	ppWeights = 0;
	lMaxWeights = lNumWeights = 0;

	// load data from file

	fread(m_szType,sizeof(char),40,fhd);
	fread(&m_fNLDist,sizeof(float),1,fhd);
	fread(&lLWeights,sizeof(long),1,fhd);

	for(lschl = 0;lschl < lLWeights;lschl ++){
		lAWeight = AddWeight();
		ppWeights[lAWeight]->Load(fhd);
	}
}

char *CBaseNeuralNet :: GetTONet(void){
	return m_szType;
}

void CBaseNeuralNet::SetLDist(const netvar_t &dParam)
{
	m_fNLDist = dParam;
}

netvar_t CBaseNeuralNet::GetLDist()
{
	return m_fNLDist;
}

CBaseWeight* CBaseNeuralNet :: GetWeight(long lW){
	return ppWeights[lW];
}

int CBaseNeuralNet :: GetITONet(void){
	return m_iType;
}