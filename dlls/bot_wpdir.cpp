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

#include "bot_wpdir.h"

CBotWPDir :: CBotWPDir(){
	memset(szPDir,0,sizeof(WPDir)*MAX_WP_DIR);
	//Init();
}

int CBotWPDir :: Init(void){
	char filename[80];
#ifndef __linux__
	UTIL_BuildFileName(filename,"joebot\\wpjs","dir.txt");
	return Load(filename);
#else
	UTIL_BuildFileName(filename,"joebot/wpjs","dir.txt");
	return Load(filename);
#endif
}

CBotWPDir :: ~CBotWPDir(){
}

int CBotWPDir :: Load(const char *szFileName){			// load WPDir into ram ...
	char *szFileContent,*szAct,*szTmp,szName[200];
	long lFileSize;
	long lToReadLength;
	FILE *fhd;
	
	if(lFileSize = GetFileSize(szFileName))
	{
		szFileContent = new char[lFileSize+2];
		memset(szFileContent,0,sizeof(char)*(lFileSize+2));
		
		if(fhd = fopen(szFileName,"r")){
			
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
				strncpy(szName,szAct,sizeof(char) * lToReadLength);
#ifdef __linux__
				if (szName[strlen(szName) - 1] == '\r') szName[strlen(szName) - 1] = '\0';
#endif
				if(strlen(szName) < 20){
					strcpy(szPDir[lNum].szDir,szName);
#ifndef __linux__
					cout << "JoeBOT: adding wp-directory : cstrike\\joebot\\wpjs\\"<<szName << endl;
#else
					cout << "JoeBOT: adding wp-directory : cstrike/joebot/wpjs/"<<szName << endl;
#endif
					lNum++;
				}
				else{
					szAct = strchr(szAct,'\n');
					continue;
				}
				szAct = szAct + lToReadLength * sizeof(char);
				
				while(!IsInstr(*szAct) && *szAct)	// goto next text ...
					szAct++;
			}
		}
		delete [] szFileContent;
	}

	if(!lNum){
		strcpy(szPDir[lNum].szDir,"std");
		lNum++;
	}
	
	return true;
}

void CBotWPDir :: MixIt(void){
	long lschl,lf,ls;
	char szpTemp[100];
	
	for(lschl=0;lschl < 100;lschl++){
		lf = RANDOM_LONG(0,lNum-1);
		ls = RANDOM_LONG(0,lNum-1);
		strcpy(szpTemp,szPDir[lf].szDir);
		strcpy(szPDir[lf].szDir,szPDir[ls].szDir);
		strcpy(szPDir[ls].szDir,szpTemp);
	}
}
