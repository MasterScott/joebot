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
#include "NNSimService.h"

#include "NeuralNet.h"
#include "string.h"

CBaseNeuralNet *LoadNet(CBaseNeuralNet *&pO,const char *szName){
	if( pO ){
		delete pO;
	}
	pO = LoadNet(szName);
	return pO;
}

CBaseNeuralNet *LoadNet(const char *szName){
	CBaseNeuralNet *p = 0;
	FILE *fhd;
	char szPreI[50];

	if(fhd = fopen(szName,"r")){
		fread(szPreI,sizeof(char),40,fhd);

		if(!strcmp(szPreI,"CNeuralNetBPropM")){
			p = new CNeuralNetBPropM;
		}
		else if(!strcmp(szPreI,"CNeuralNetBProp")){
			p = new CNeuralNetBProp;
		}

		if(p)
			p->LoadFile(szName);
	}

	return p;
}