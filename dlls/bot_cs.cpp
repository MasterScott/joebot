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

//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// bot.cpp
//

#include "bot.h"

#include <sys/types.h>
#include <sys/stat.h>

void BotBuy_CS_WEAPON_P228_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","1",0);
	FakeClientCommand(pEdict,"menuselect","3",0);
}

void BotBuy_CS_WEAPON_SCOUT_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","4",0);
	FakeClientCommand(pEdict,"menuselect","3",0);
}

void BotBuy_CS_WEAPON_HEGRENADE_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buyequip",0,0);
	FakeClientCommand(pEdict,"menuselect","4",0);
}

void BotBuy_CS_WEAPON_XM1014_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","2",0);
	FakeClientCommand(pEdict,"menuselect","2",0);
}

void BotBuy_CS_WEAPON_MAC10_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","3",0);
	FakeClientCommand(pEdict,"menuselect","4",0);
}

void BotBuy_CS_WEAPON_AUG_T(CBotBase *pBot){
	return; // it's simply not possible to buy an aug as terrorist
}

void BotBuy_CS_WEAPON_SMOKEGRENADE_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buyequip",0,0);
	FakeClientCommand(pEdict,"menuselect","5",0);
}

void BotBuy_CS_WEAPON_ELITE_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","1",0);
	FakeClientCommand(pEdict,"menuselect","5",0);
}

void BotBuy_CS_WEAPON_FIVESEVEN_T(CBotBase *pBot){
	return; // it's simply not possible to buy a fiveseven as terrorist
}

void BotBuy_CS_WEAPON_UMP45_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","3",0);
	FakeClientCommand(pEdict,"menuselect","3",0);
}

void BotBuy_CS_WEAPON_SG550_T(CBotBase *pBot){
	return; // it's simply not possible to buy a 550 as terrorist
}

void BotBuy_CS_WEAPON_USP_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","1",0);
	FakeClientCommand(pEdict,"menuselect","2",0);
}

void BotBuy_CS_WEAPON_GLOCK18_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","1",0);
	FakeClientCommand(pEdict,"menuselect","1",0);
}

void BotBuy_CS_WEAPON_AWP_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","4",0);
	FakeClientCommand(pEdict,"menuselect","5",0);
}

void BotBuy_CS_WEAPON_MP5NAVY_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","3",0);
	FakeClientCommand(pEdict,"menuselect","2",0);
}

void BotBuy_CS_WEAPON_M249_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","5",0);
	FakeClientCommand(pEdict,"menuselect","1",0);
}

void BotBuy_CS_WEAPON_M3_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","2",0);
	FakeClientCommand(pEdict,"menuselect","1",0);
}

void BotBuy_CS_WEAPON_M4A1_T(CBotBase *pBot){
	return; // it's simply not possible to buy a m4a1 m4a1 as terrorist
}

void BotBuy_CS_WEAPON_TMP_T(CBotBase *pBot){
	return; // it's simply not possible to buy a tmp as terrorist
}

void BotBuy_CS_WEAPON_FAMAS_T(CBotBase *pBot){
	return; // it's not possible for a terrorist to buy a famas
}

void BotBuy_CS_WEAPON_G3SG1_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","4",0);
	FakeClientCommand(pEdict,"menuselect","7",0);
}

void BotBuy_CS_WEAPON_FLASHBANG_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buyequip",0,0);
	FakeClientCommand(pEdict,"menuselect","3",0);
}

void BotBuy_CS_WEAPON_DEAGLE_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","1",0);
	FakeClientCommand(pEdict,"menuselect","4",0);
}

void BotBuy_CS_WEAPON_SG552_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","4",0);
	FakeClientCommand(pEdict,"menuselect","4",0);
}

void BotBuy_CS_WEAPON_AK47_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","4",0);
	FakeClientCommand(pEdict,"menuselect","2",0);
}

void BotBuy_CS_WEAPON_P90_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","3",0);
	FakeClientCommand(pEdict,"menuselect","4",0);
}

void BotBuy_CS_WEAPON_GALIL_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","4",0);
	FakeClientCommand(pEdict,"menuselect","1",0);
}

void BotBuy_PAmmo(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","6",0);
}

void BotBuy_SAmmo(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","7",0);
}

/*void BotBuy_CS_WEAPON_HE_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buyequip",0,0);
	FakeClientCommand(pEdict,"menuselect","4",0);
}

void BotBuy_CS_WEAPON_FL_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buyequip",0,0);
	FakeClientCommand(pEdict,"menuselect","3",0);
}

void BotBuy_CS_WEAPON_SG_T(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buyequip",0,0);
	FakeClientCommand(pEdict,"menuselect","5",0);
}*/

///////////////////////////////////////////////////////////////////////////////////////////////
//
// counter terrorists buying
//
///////////////////////////////////////////////////////////////////////////////////////////////

void BotBuy_CS_WEAPON_P228_CT(CBotBase *pBot){
	BotBuy_CS_WEAPON_P228_T(pBot);
}

void BotBuy_CS_WEAPON_SCOUT_CT(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","4",0);
	FakeClientCommand(pEdict,"menuselect","2",0);
}

void BotBuy_CS_WEAPON_HEGRENADE_CT(CBotBase *pBot){
	BotBuy_CS_WEAPON_HEGRENADE_T(pBot);
}

void BotBuy_CS_WEAPON_XM1014_CT(CBotBase *pBot){
	BotBuy_CS_WEAPON_XM1014_T(pBot);
}

void BotBuy_CS_WEAPON_MAC10_CT(CBotBase *pBot){
	return; // it's simply not possible to buy a mac10 as counter terrorist
}

void BotBuy_CS_WEAPON_AUG_CT(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","4",0);
	FakeClientCommand(pEdict,"menuselect","4",0);
}

void BotBuy_CS_WEAPON_SMOKEGRENADE_CT(CBotBase *pBot){
	BotBuy_CS_WEAPON_SMOKEGRENADE_T(pBot);
}

void BotBuy_CS_WEAPON_ELITE_CT(CBotBase *pBot){
	return; // it's simply not possible to buy the elites as counter terrorist
}

void BotBuy_CS_WEAPON_FIVESEVEN_CT(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","1",0);
	FakeClientCommand(pEdict,"menuselect","5",0);
}

void BotBuy_CS_WEAPON_UMP45_CT(CBotBase *pBot){
	BotBuy_CS_WEAPON_UMP45_T(pBot);
}

void BotBuy_CS_WEAPON_SG550_CT(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","4",0);
	FakeClientCommand(pEdict,"menuselect","5",0);
}

void BotBuy_CS_WEAPON_USP_CT(CBotBase *pBot){
	BotBuy_CS_WEAPON_USP_T(pBot);
}

void BotBuy_CS_WEAPON_GLOCK18_CT(CBotBase *pBot){
	BotBuy_CS_WEAPON_GLOCK18_T(pBot);
}

void BotBuy_CS_WEAPON_AWP_CT(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","4",0);
	FakeClientCommand(pEdict,"menuselect","6",0);
}

void BotBuy_CS_WEAPON_MP5NAVY_CT(CBotBase *pBot){
	BotBuy_CS_WEAPON_MP5NAVY_T(pBot);
}

void BotBuy_CS_WEAPON_M249_CT(CBotBase *pBot){
	BotBuy_CS_WEAPON_M249_T(pBot);
}

void BotBuy_CS_WEAPON_M3_CT(CBotBase *pBot){
	BotBuy_CS_WEAPON_M3_T(pBot);
}

void BotBuy_CS_WEAPON_M4A1_CT(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","4",0);
	FakeClientCommand(pEdict,"menuselect","3",0);
}

void BotBuy_CS_WEAPON_TMP_CT(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","3",0);
	FakeClientCommand(pEdict,"menuselect","1",0);
}

void BotBuy_CS_WEAPON_FAMAS_CT(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buy",0,0);
	FakeClientCommand(pEdict,"menuselect","4",0);
	FakeClientCommand(pEdict,"menuselect","1",0);
}

void BotBuy_CS_WEAPON_G3SG1_CT(CBotBase *pBot){
	return; // it's simply not possible to buy the g3sg1 as counter terrorist
}

void BotBuy_CS_WEAPON_FLASHBANG_CT(CBotBase *pBot){
	BotBuy_CS_WEAPON_FLASHBANG_T(pBot);
}

void BotBuy_CS_WEAPON_DEAGLE_CT(CBotBase *pBot){
	BotBuy_CS_WEAPON_DEAGLE_T(pBot);
}

void BotBuy_CS_WEAPON_SG552_CT(CBotBase *pBot){
	return; // it's simply not possible to buy a sg552 as counter terrorist
}

void BotBuy_CS_WEAPON_AK47_CT(CBotBase *pBot){
	return; // it's simply not possible to buy a ak47 as counter terrorist
}

void BotBuy_CS_WEAPON_GALIL_CT(CBotBase *pBot){
	return; // it's not possible for a ct to buy a galil
}

void BotBuy_CS_WEAPON_P90_CT(CBotBase *pBot){
	BotBuy_CS_WEAPON_P90_T(pBot);
}

void BotBuy_CS_WEAPON_SHIELD_CT(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buyequip",0,0);
	FakeClientCommand(pEdict,"menuselect","8",0);
}

/*
void BotBuy_CS_WEAPON_HE_CT(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buyequip",0,0);
	FakeClientCommand(pEdict,"menuselect","4",0);
}

void BotBuy_CS_WEAPON_FL_CT(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buyequip",0,0);
	FakeClientCommand(pEdict,"menuselect","3",0);
}

void BotBuy_CS_WEAPON_SG_CT(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buyequip",0,0);
	FakeClientCommand(pEdict,"menuselect","5",0);
}*/

// general stuff

void BotBuy_CS_Kevlar(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buyequip",0,0);
	FakeClientCommand(pEdict,"menuselect","1",0);
}

void BotBuy_CS_KevlarHelmet(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buyequip",0,0);
	FakeClientCommand(pEdict,"menuselect","2",0);
}

void BotBuy_CS_DefKit(CBotBase *pBot){
	edict_t *pEdict = pBot->pEdict;
	
	FakeClientCommand(pEdict,"buyequip",0,0);
	FakeClientCommand(pEdict,"menuselect","6",0);
}