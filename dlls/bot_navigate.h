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
#ifndef __BOT_NAMES_H
#define __BOT_NAMES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "weaponinfo.h"

#include "CParser.h"

#define BN_MAXNAMES 50
#define BN_MAXNAMELENGTH 20

class CBotNames : private CParser{
public:
	CBotNames();
	~CBotNames();

	int Init(void);
	int Load(const char *);
	void MixIt(void);
	char *GetName(void);
protected:
private:
	char szNames[BN_MAXNAMES][BN_MAXNAMELENGTH];
	long lNum;
	long lLReturn;
	bool bInited;
};

#endif __BOT_NAMES_H