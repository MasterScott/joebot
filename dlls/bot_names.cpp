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

#include "extdll.h"
#include "Util.h"

#include <iostream.h>

CBotNamesItem g_DefaultName;
extern bool g_bMixNames;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBotNames g_Names;

CBotNamesItem::CBotNamesItem(){
	*m_szName = 0;
}
CBotNamesItem::~CBotNamesItem(){
}


CBotNames::CBotNames(){
	m_ICName = m_LNames.begin();
	bInited = false;

	strcpy(g_DefaultName.m_szName,"JoeBOT");
}

CBotNames::~CBotNames(){
}

int CBotNames::init(void){
	char szfilename[80];
	UTIL_BuildFileName(szfilename, "joebot","bot_names.txt");
	return load(szfilename);
}

bool CBotNames::load(const char *szFileName){
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
					//strcpy(Names[lNum].szName,szName);
					//cout << szAdd << endl;

					CBotNamesItem tempName;

					strcpy(tempName.m_szName,szName);
					m_LNames.push_back(tempName);
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
	
	// if file wasn't filled up ... or file not found ...
	if(g_bMixNames) mixIt();
	
	return true;
}

void CBotNames::mixIt(void){
	int i1, i2,i;
	std::list<CBotNamesItem>::iterator iter1,iter2;
	CBotNamesItem temp;

	for(i=m_LNames.size()*5; i ; i--){
		i1 = RANDOM_LONG(0,m_LNames.size()-1);
		i2 = RANDOM_LONG(0,m_LNames.size()-1);

		if(i1 == i2)			// no need to swap
			continue;

		iter1 = iter2 = m_LNames.begin();

		for(;i1; i1 --){
			iter1++;
		}
		for(;i2; i2 --){
			iter2++;
		}
		/*if(iter1 == m_LNames.end()
			||iter2 == m_LNames.end()){
			cout << "---------" << endl;
			continue;
		}*/

		temp = *iter1;			// swap
		*iter1 = *iter2;
		*iter2 = temp;
	}
}

const CBotNamesItem *CBotNames::getName(void){
	//cout << m_LNames.size() << " names"<<endl;
	if(m_LNames.size()){
		m_ICName++;
		if(m_ICName == m_LNames.end()){
			m_ICName = m_LNames.begin();
		}
		return &(*m_ICName);
	}
	else
		return &g_DefaultName;
}
