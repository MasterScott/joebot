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
// JoeBot
// by Johannes Lampel
// Johannes.Lampel@gmx.de
// http://joebot.counter-strike.de

#include "bot_names.h"

extern bool g_bMixNames;

CBotNames :: CBotNames(){
	lNum = 0;
	lLReturn = 0;
	bInited = false;
	for(long lschl=0;lschl < BN_MAXNAMES;lschl++){
		memset(Names[lschl].szName,0,sizeof(char) * BN_MAXNAMELENGTH);
	}
}

int CBotNames :: Init(void){
	char szfilename[80];
	UTIL_BuildFileName(szfilename, "joebot","bot_names.txt");
	return Load(szfilename);
/*#ifndef __linux__
	return Load("cstrike\\joebot\\bot_names.txt");
#else
	return Load("cstrike/joebot/bot_names.txt");
#endif*/
}

CBotNames :: ~CBotNames(){
}

Name *CBotNames :: GetName(void){
	// check if already inited ...
	if(!bInited){
		Init();
		bInited = true;
	}
	Name *Return = &(Names[lLReturn]);

	lLReturn ++;
	if(lLReturn >= lNum)
		lLReturn = 0;
	
	return Return;
}

void CBotNames :: MixIt(void){
	long lschl,lf,ls;
	char szpTemp[100];
	
	for(lschl=0;lschl < 100;lschl++){
		lf = RANDOM_LONG(0,lNum-1);
		ls = RANDOM_LONG(0,lNum-1);
		strcpy(szpTemp,Names[lf].szName);
		strcpy(Names[lf].szName,Names[ls].szName);
		strcpy(Names[ls].szName,szpTemp);
	}
}

int CBotNames :: Load(const char *szFileName){			// load names into ram ...
	char *szFileContent,*szAct,*szTmp,szName[200];
	long lFileSize;
	long lToReadLength;
	FILE *fhd;
	//char szType[200],szWeapon[200];
	
	if(lFileSize = GetFileSize(szFileName))
	{
		szFileContent = new char[lFileSize+2];
		memset(szFileContent,0,sizeof(char)*(lFileSize+2));
		
		if(fhd = fopen(szFileName,"r")){
			bInited = true;
			if (IS_DEDICATED_SERVER())
				printf("JoeBOT: Loading names from file : %s\n", szFileName);

			//fread(szFileContent,sizeof(char),lFileSize,fhd);

			char *p = szFileContent;
			while (!feof(fhd)){
				*p = fgetc(fhd);
				p ++;
			}

			fclose(fhd);
			
			Preprocess(szFileContent);		// remove uninteresting stuff
			
			szAct = szFileContent;
			
			while(*szAct){	// stop if 0
				while(!IsInstr(*szAct) && *szAct)	// goto next text ...
					szAct++;
				
				if (!(szTmp=strchr(szAct,'\n')))
					break;
				
				memset(szName,0,sizeof(char) * 200);
				lToReadLength = strchr(szAct,'\n') - szAct;
				if(!lToReadLength)
					break;
				strncpy(szName,szAct,sizeof(char) * lToReadLength);
#ifdef __linux__
				if (szName[strlen(szName) - 1] == '\r') szName[strlen(szName) - 1] = '\0';
#endif
				if(strlen(szName) < BN_MAXNAMELENGTH){
					strcpy(Names[lNum].szName,szName);
					//cout << szAdd << endl;
				}
				else{
					szAct = strchr(szAct,'\n');
					continue;
				}
				szAct = szAct + lToReadLength * sizeof(char);
				
				while(!IsInstr(*szAct) && *szAct)	// goto next text ...
					szAct++;
			
				lNum ++;
				if(lNum>BN_MAXNAMES)		// just ignore some names ...
					lNum--;
			}
		}
		delete [] szFileContent;
	}
	
	// if file wasn't filled up ... or file not found ...
	if(!lNum){
		strcpy(Names[0].szName,"JoeBOT");
		lNum ++;
	}
	else
		if(g_bMixNames) MixIt();
	
	return true;
}
