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
#ifndef __CBOTCS_H
#define __CBOTCS_H

#include "CBotBase.h"
#include "Chat.h"

class CBotCS : public CBotBase{
public:
bool DistanceSight(void);

	CBotCS();
	~CBotCS();

	virtual void Think(void);
	virtual void Think5th(void);			// think 5th
	virtual void Think1(void);				// think every 1s
	virtual void Fight(void);

	virtual void HandleMenu(void);
	virtual bool HandleOrders(void);
	virtual void SpawnInit(void);
	virtual void Init(void);
	virtual bool BuyWeapon(void);

	virtual void PreprocessTasks(void);

	virtual bool ReactOnSound(void);
	virtual void ReactOnRadio(void);
	virtual bool CheckGrenadeThrowing(void);

	virtual long IsPrimaryWeapon(long lbit);
	virtual long IsSecondaryWeapon(long lbit);
	virtual long IsShotgunWeapon(long lbit);
	virtual long IsSubMWeapon(long lbit);
	virtual long IsRifleWeapon(long lbit);
	virtual long IsSniperWeapon(long lbit);
	virtual long IsKnifeWeapon(long lbit);
	virtual long IsMachineGunWeapon(long lbit);
	virtual long IsGrenadeWeapon(long lbit);
	virtual long WeaponModel2ID(const char *);

	virtual bool IsWeaponBetterCurrent(long);

	virtual Vector BodyTarget( edict_t *pBotEnemy);

	virtual edict_t *FindEnemy(void);
	bool IsBehindSG(edict_t *pPlayer);
	virtual bool FireWeapon( Vector &v_enemy );

	virtual bool HasAlreadyHostage(const edict_t *pHostage);

	virtual bool GoToSpEnt( void );
	virtual bool HeadTowardSpEnt(void);
	virtual void Goto(Vector &VGoto);
	virtual bool Bored(void);
	virtual bool HeadTowardWaypoint( void );
	virtual int GoNextWPOnPath(void);
	virtual int GetNextWPOnPath(void);
	virtual bool HandleNearWP(int iNearest, bool &bReturn);
	virtual void FindRoute(int iNearWP);
	virtual bool DecideOnWay(void);
	void CheckTasksOnReachability(int);

	virtual void SendRadioCommand(int);

	virtual void GrenadeThrown(void);

	// data
	float f_buytime;			// last time of buying
	int bot_money;
	bool bot_vip;				// is bot VIP ?
	bool bGlockBurst;
	
	edict_t *pHostages[_MAXHOSTAGES];		// pointer to entities of used hostages
	int i_UsedHostages;			// number of touched hostages
protected:
private:
};

void BotBuy_CS_WEAPON_P228_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_FAMAS_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_GALIL_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_SCOUT_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_HEGRENADE_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_XM1014_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_MAC10_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_AUG_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_SMOKEGRENADE_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_ELITE_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_FIVESEVEN_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_UMP45_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_SG550_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_USP_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_GLOCK18_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_AWP_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_MP5NAVY_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_M249_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_M3_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_M4A1_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_TMP_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_G3SG1_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_FLASHBANG_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_DEAGLE_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_SG552_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_AK47_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_P90_T(CBotBase *pBot);
/*void BotBuy_CS_WEAPON_HE_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_FL_T(CBotBase *pBot);
void BotBuy_CS_WEAPON_SG_T(CBotBase *pBot);*/

void BotBuy_CS_WEAPON_P228_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_FAMAS_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_GALIL_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_SCOUT_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_HEGRENADE_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_XM1014_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_MAC10_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_AUG_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_SMOKEGRENADE_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_ELITE_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_FIVESEVEN_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_UMP45_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_SG550_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_USP_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_GLOCK18_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_AWP_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_MP5NAVY_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_M249_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_M3_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_M4A1_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_TMP_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_G3SG1_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_FLASHBANG_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_DEAGLE_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_SG552_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_AK47_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_P90_CT(CBotBase *pBot);
/*void BotBuy_CS_WEAPON_HE_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_FL_CT(CBotBase *pBot);
void BotBuy_CS_WEAPON_SG_CT(CBotBase *pBot);*/


void BotBuy_PAmmo(CBotBase *pBot);
void BotBuy_SAmmo(CBotBase *pBot);

void BotBuy_CS_Kevlar(CBotBase *pBot);
void BotBuy_CS_KevlarHelmet(CBotBase *pBot);
void BotBuy_CS_DefKit(CBotBase *pBot);


#endif  __CBOTCS_H
