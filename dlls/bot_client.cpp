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
// bot_client.cpp
//

#include "extdll.h"
#include "util.h"
#include "engine.h"
#include "cbase.h"

#ifdef USE_METAMOD
#define SDK_UTIL_H  // util.h already included
#include "meta_api.h"
#endif /* USE_METAMOD */

#include "bot_client.h"

#include "bot.h"
#include "bot_modid.h"
#include "CBotCS.h"
#include "CBotDOD.h"
#include "globalvars.h"

// types of damage to ignore...
#define IGNORE_DAMAGE (DMG_CRUSH | DMG_FREEZE | DMG_SHOCK | DMG_DROWN | \
	DMG_NERVEGAS | DMG_RADIATION | DMG_DROWNRECOVER | \
	DMG_ACID | DMG_SLOWBURN | DMG_SLOWFREEZE)

float f_LastRadio;
char szLastSayText[80];				// if somebody answered this, set szLastSayText[0] = 0;
int iLastSayClient;

char szDeadLastSayText[80];				// if somebody answered this, set szLastSayText[0] = 0;
int iDeadLastSayClient;

bot_weapon_t weapon_defs[MAX_WEAPONS]; // array of weapon definitions

void (*botmsgs[MAX_REG_MSGS])(void *, int, int);
#ifndef USE_METAMOD
usermsg_t usermsgs[MAX_REG_MSGS];
int usermsgs_count = 0;
#endif /* USE_METAMOD */
int msg_state = 0;

// This message is sent when the Counter-Strike VGUI menu is displayed.
void BotClient_CS_VGUIMenu(void *p, int bot_index,int iAdd){
	if(iAdd != _CLIENT_END){
		if ((*(int *)p) == 2)  // is it a team select menu?
			bots[bot_index]->start_action = MSG_CS_TEAM_SELECT;
		else if ((*(int *)p) == 26)  // is it a terrorist model select menu?
			bots[bot_index]->start_action = MSG_CS_T_SELECT;
		else if ((*(int *)p) == 27)  // is it a counter-terrorist model select menu?	
			bots[bot_index]->start_action = MSG_CS_CT_SELECT;
	}
}

void BotClient_DOD_VGUIMenu(void *p, int bot_index,int iAdd)
{
	if(iAdd != _CLIENT_END){
		if ((*(int *)p) == 2)  // is it a team select menu?
			bots[bot_index]->start_action = MSG_DOD_TEAM_SELECT;
		
		else if ((*(int *)p) == 10)  // is is a class selection axis menu?
			bots[bot_index]->start_action = MSG_DOD_CLASS_SELECT_AX;
		
		else if ((*(int *)p) == 9)  // is is a class selection allies menu?
			bots[bot_index]->start_action = MSG_DOD_CLASS_SELECT_AL;

		else if ((*(int *)p) == 21)  // is is a class selection allies menu?	// para
			bots[bot_index]->start_action = MSG_DOD_CLASS_SELECT_AL_PARA;	
		else if ((*(int *)p) == 22)  // is is a class selection axis menu?		// para
			bots[bot_index]->start_action = MSG_DOD_CLASS_SELECT_AX_PARA;
		
		else if ((*(int *)p) == 19)  // is is a class selection allies menu of the sergeant?
			bots[bot_index]->start_action = MSG_DOD_WPN_SELECT_SERGEANT;

		else if ((*(int *)p) == 20)  // is is a class selection axis menu for machinegunners ?
			bots[bot_index]->start_action = MSG_DOD_WPN_SELECT_MACHINE;

		else if ((*(int *)p) == 23)  // is is a class selection axis menu for fg42 ?
			bots[bot_index]->start_action = MSG_DOD_WPN_SELECT_FG42;

		else if ((*(int *)p) == 24)  // is is a class selection axis menu for riflemen ?
			bots[bot_index]->start_action = MSG_DOD_WPN_SELECT_RIFLEMAN;

		else if ((*(int *)p) == 25)  // is is a class selection axis menu for grenadiers ?
			bots[bot_index]->start_action = MSG_DOD_WPN_SELECT_GRENADIER;
	}
}

// This message is sent when a menu is being displayed in Counter-Strike.
void BotClient_CS_ShowMenu(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	if(iAdd != _CLIENT_END){
		if (state < 3){
			state++;  // ignore first 3 fields of message
			return;
		}
		
		if (FStrEq((char *)p, "#Team_Select")){  // team select menu?
			bots[bot_index]->start_action = MSG_CS_TEAM_SELECT;
		}
		else if (FStrEq((char *)p, "#Terrorist_Select")){  // T model select?
			bots[bot_index]->start_action = MSG_CS_T_SELECT;
		}
		else if (FStrEq((char *)p, "#CT_Select")){  // CT model select menu?
			bots[bot_index]->start_action = MSG_CS_CT_SELECT;
		}
		
		state = 0;  // reset state machine
	}
}


// This message is sent when a client joins the game.  All of the weapons
// are sent with the weapon ID and information about what ammo is used.
void BotClient_Valve_WeaponList(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static bot_weapon_t bot_weapon;
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			bot_weapon.iAmmoClip = -1;		// just init
			
			state++;
			strcpy(bot_weapon.szClassname, (char *)p);
		}
		else if (state == 1){
			state++;
			bot_weapon.iAmmo1 = *(int *)p;  // ammo index 1
		}
		else if (state == 2){
			state++;
			bot_weapon.iAmmo1Max = *(int *)p;  // max ammo1
		}
		else if (state == 3){
			state++;
			bot_weapon.iAmmo2 = *(int *)p;  // ammo index 2
		}
		else if (state == 4){
			state++;
			bot_weapon.iAmmo2Max = *(int *)p;  // max ammo2
		}
		else if (state == 5){
			state++;
			bot_weapon.iSlot = *(int *)p;  // slot for this weapon
		}
		else if (state == 6){
			state++;
			bot_weapon.iPosition = *(int *)p;  // position in slot
		}
		else if (state == 7){
			state++;
			bot_weapon.iId = *(int *)p;  // weapon ID
		}
		else if (state == 8){
			bot_weapon.iFlags = *(int *)p;  // flags for weapon (WTF???)
			
			// store away this weapon with it's ammo information...
			weapon_defs[bot_weapon.iId] = bot_weapon;
			
			state = 0;
		}
	}
}

void BotClient_DOD_WeaponList(void *p, int bot_index, int iAdd)
{
	static int state = 0;   // current state machine state
	static bot_weapon_t bot_weapon;
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state++;
			strcpy(bot_weapon.szClassname, (char *)p);
		}
		else if (state == 1){
			state++;
			bot_weapon.iAmmo1 = *(int *)p;  // ammo index 1
		}
		else if (state == 2){
			state++;
			bot_weapon.iAmmo1Max = *(int *)p;  // max ammo1
		}
		else if (state == 3){
			state++;
			bot_weapon.iAmmo2 = *(int *)p;  // ammo index 2
		}
		else if (state == 4){
			state++;
			bot_weapon.iAmmo2Max = *(int *)p;  // max ammo2
		}
		else if (state == 5){
			state++;
			bot_weapon.iSlot = *(int *)p;  // slot for this weapon
		}
		else if (state == 6){
			state++;
			bot_weapon.iPosition = *(int *)p;  // position in slot
		}
		else if (state == 7){
			state++;
			bot_weapon.iId = *(int *)p;  // weapon ID
		}
		else if (state == 8){
			state ++;
			bot_weapon.iFlags = *(int *)p;  // flags for weapon (WTF???)
		}
		else if (state == 9){
			bot_weapon.iAmmoClip = *(int *)p;  // clip size
			
			// store away this weapon with it's ammo information...
			weapon_defs[bot_weapon.iId] = bot_weapon;

#ifdef DEBUGENGINE
			FILE *fhd;
			fhd = fopen("weapons.txt","a");
			fprintf(fhd,"%i %s\n",bot_weapon.iId,bot_weapon.szClassname);
			fclose(fhd);
#endif
			
			state = 0;
		}
	}
}

void BotClient_CS_WeaponList(void *p, int bot_index,int iAdd){
	// this is just like the Valve Weapon List message
	BotClient_Valve_WeaponList(p, bot_index,iAdd);
}

void BotClient_Gearbox_WeaponList(void *p, int bot_index,int iAdd){
	// this is just like the Valve Weapon List message
	BotClient_Valve_WeaponList(p, bot_index,iAdd);
}


// This message is sent when a weapon is selected (either by the bot chosing
// a weapon or by the server auto assigning the bot a weapon).
void BotClient_Valve_CurWeapon(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static int iState;
	static int iId;
	static int iClip;
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state++;
			iState = *(int *)p;  // state of the current weapon (WTF???)
		}
		else if (state == 1){
			state++;
			iId = *(int *)p;  // weapon ID of current weapon
		}
		else if (state == 2){
			if (iId <= 31 && iState){			// &&iState added by @$3.1415rin
				iClip = *(int *)p;  // ammo currently in the clip for this weapon
				
				bots[bot_index]->bot_weapons |= (1<<iId);  // set this weapon bit
				
				bots[bot_index]->current_weapon.iId = iId;
				bots[bot_index]->current_weapon.iClip = iClip;
				
				// update the ammo counts for this weapon...
				bots[bot_index]->current_weapon.iAmmo1 =
					bots[bot_index]->m_rgAmmo[weapon_defs[iId].iAmmo1];
				bots[bot_index]->current_weapon.iAmmo2 =
					bots[bot_index]->m_rgAmmo[weapon_defs[iId].iAmmo2];
			}
			
			state = 0;
		}
	}
}

void BotClient_CS_CurWeapon(void *p, int bot_index,int iAdd){
	// this is just like the Valve Current Weapon message
	BotClient_Valve_CurWeapon(p, bot_index,iAdd);
}

void BotClient_Gearbox_CurWeapon(void *p, int bot_index,int iAdd){
	// this is just like the Valve Current Weapon message
	BotClient_Valve_CurWeapon(p, bot_index,iAdd);
}

void BotClient_DOD_CurWeapon(void *p, int bot_index, int iAdd)
{
	// this is just like the Valve Current Weapon message
	BotClient_Valve_CurWeapon(p, bot_index,iAdd);
}

// This message is sent whenever ammo ammounts are adjusted (up or down).
void BotClient_Valve_AmmoX(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static int index;
	static int ammount;
	int ammo_index;
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state++;
			index = *(int *)p;  // ammo index (for type of ammo)
		}
		else if (state == 1){
			ammount = *(int *)p;  // the ammount of ammo currently available
			
			bots[bot_index]->m_rgAmmo[index] = ammount;  // store it away
			
			ammo_index = bots[bot_index]->current_weapon.iId;
			
			// update the ammo counts for this weapon...
			bots[bot_index]->current_weapon.iAmmo1 =
				bots[bot_index]->m_rgAmmo[weapon_defs[ammo_index].iAmmo1];
			bots[bot_index]->current_weapon.iAmmo2 =
				bots[bot_index]->m_rgAmmo[weapon_defs[ammo_index].iAmmo2];
			
			state = 0;
		}
	}
}

void BotClient_CS_AmmoX(void *p, int bot_index,int iAdd){
	// this is just like the Valve AmmoX message
	BotClient_Valve_AmmoX(p, bot_index,iAdd);
}

void BotClient_Gearbox_AmmoX(void *p, int bot_index,int iAdd){
	// this is just like the Valve AmmoX message
	BotClient_Valve_AmmoX(p, bot_index,iAdd);
}

void BotClient_DOD_AmmoX(void *p, int bot_index,int iAdd)
{
	// this is just like the Valve AmmoX message
	BotClient_Valve_AmmoX(p, bot_index,iAdd);
}

void BotClient_Valve_SayText(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static short int index;
	static char *szText;
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state++;
			index = *(short *)p;
		}
		else if (state == 1){
			state = 0;

			char *szDP;
			szText = (char *)p;

			char *szSayText;
			edict_t *pSayEdict;

			pSayEdict = INDEXENT(index);
			if(pSayEdict && IsAlive(pSayEdict)){
				szSayText = szLastSayText;
				iLastSayClient = index;
			}
			else{
				szSayText = szDeadLastSayText;
				iDeadLastSayClient = index;
			}

			if(strstr(szText,"(Counter-Terrorist)")		// just ignore this message if it's team specific
				||strstr(szText,"(Terrorist)")			// NOTE : This neednt be mod independant. This is just for cs/hldm
				||strstr(szText,"(TEAM)")){
				return;
			}

			strcpy(szSayText,szText);

			szDP = strchr(szSayText,':');

			if(szDP  < szSayText + sizeof(char) * 80 )
				memset(szSayText,' ',szDP - szSayText);

			// make it lowercase;
			UTIL_strlwr(szSayText);
			
			//cout << szSayText;
		}
	}
}

void BotClient_CS_SayText(void *p, int bot_index,int iAdd){
	// this is just like the Valve SayText message
	BotClient_Valve_SayText(p, bot_index,iAdd);
}

void BotClient_Gearbox_SayText(void *p, int bot_index,int iAdd){
	// this is just like the Valve SayText message
	BotClient_Valve_SayText(p, bot_index,iAdd);
}

void BotClient_DOD_SayText(void *p, int bot_index,int iAdd)
{
	// this is just like the Valve SayText message
	BotClient_Valve_SayText(p, bot_index,iAdd);
}

// This message is sent when the bot picks up some ammo (AmmoX messages are
// also sent so this message is probably not really necessary except it
// allows the HUD to draw pictures of ammo that have been picked up.  The
// bots don't really need pictures since they don't have any eyes anyway.
void BotClient_Valve_AmmoPickup(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static int index;
	static int ammount;
	int ammo_index;
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state++;
			index = *(int *)p;
		}
		else if (state == 1){
			ammount = *(int *)p;
			
			bots[bot_index]->m_rgAmmo[index] = ammount;
			
			ammo_index = bots[bot_index]->current_weapon.iId;
			
			// update the ammo counts for this weapon...
			bots[bot_index]->current_weapon.iAmmo1 =
				bots[bot_index]->m_rgAmmo[weapon_defs[ammo_index].iAmmo1];
			bots[bot_index]->current_weapon.iAmmo2 =
				bots[bot_index]->m_rgAmmo[weapon_defs[ammo_index].iAmmo2];
			
			state = 0;
		}
	}
}

void BotClient_CS_AmmoPickup(void *p, int bot_index,int iAdd){
	// this is just like the Valve Ammo Pickup message
	BotClient_Valve_AmmoPickup(p, bot_index,iAdd);
}

void BotClient_Gearbox_AmmoPickup(void *p, int bot_index,int iAdd){
	// this is just like the Valve Ammo Pickup message
	BotClient_Valve_AmmoPickup(p, bot_index,iAdd);
}

void BotClient_DOD_AmmoPickup(void *p, int bot_index,int iAdd)
{
	// this is just like the Valve Ammo Pickup message
	BotClient_Valve_AmmoPickup(p, bot_index,iAdd);
}

// This message gets sent when the bot picks up a weapon.
void BotClient_Valve_WeapPickup(void *p, int bot_index,int iAdd){
	int index;
	
	if(iAdd != _CLIENT_END){
		index = *(int *)p;
		
		// set this weapon bit to indicate that we are carrying this weapon
		bots[bot_index]->bot_weapons |= (1<<index);
	}
}

void BotClient_CS_WeapPickup(void *p, int bot_index,int iAdd){
	// this is just like the Valve Weapon Pickup message
	BotClient_Valve_WeapPickup(p, bot_index,iAdd);
}

void BotClient_Gearbox_WeapPickup(void *p, int bot_index,int iAdd){
	// this is just like the Valve Weapon Pickup message
	BotClient_Valve_WeapPickup(p, bot_index,iAdd);
}

void BotClient_DOD_WeapPickup(void *p, int bot_index,int iAdd)
{
	// this is just like the Valve Weapon Pickup message
	BotClient_Valve_WeapPickup(p, bot_index,iAdd);
}

// This message gets sent when the bot picks up an item (like a battery
// or a healthkit)
void BotClient_Valve_ItemPickup(void *p, int bot_index,int iAdd){
	if(iAdd != _CLIENT_END){
	}
}

void BotClient_CS_ItemPickup(void *p, int bot_index,int iAdd){
	// this is just like the Valve Item Pickup message
	BotClient_Valve_ItemPickup(p, bot_index,iAdd);
}

void BotClient_Gearbox_ItemPickup(void *p, int bot_index,int iAdd){
	// this is just like the Valve Item Pickup message
	BotClient_Valve_ItemPickup(p, bot_index,iAdd);
}

void BotClient_DOD_ItemPickup(void *p, int bot_index,int iAdd)
{
	// this is just like the Valve Item Pickup message
	BotClient_Valve_ItemPickup(p, bot_index,iAdd);
}

// This message gets sent when the bots health changes.
void BotClient_Valve_Health(void *p, int bot_index,int iAdd){
	if(iAdd != _CLIENT_END){
		bots[bot_index]->bot_health = *(int *)p;  // health ammount
	}
}

void BotClient_CS_Health(void *p, int bot_index,int iAdd){
	// this is just like the Valve Health message
	BotClient_Valve_Health(p, bot_index,iAdd);
}

void BotClient_Gearbox_Health(void *p, int bot_index,int iAdd){
	// this is just like the Valve Health message
	BotClient_Valve_Health(p, bot_index,iAdd);
}

void BotClient_DOD_Health(void *p, int bot_index,int iAdd)
{
	// this is just like the Valve Health message
	BotClient_Valve_Health(p, bot_index,iAdd);
}

// This message gets sent when the bots armor changes.
void BotClient_Valve_Battery(void *p, int bot_index,int iAdd){
	if(iAdd != _CLIENT_END){
		bots[bot_index]->bot_armor = *(int *)p;  // armor ammount
	}
}

void BotClient_CS_Battery(void *p, int bot_index,int iAdd){
	// this is just like the Valve Battery message
	BotClient_Valve_Battery(p, bot_index,iAdd);
}

void BotClient_Gearbox_Battery(void *p, int bot_index,int iAdd){
	// this is just like the Valve Battery message
	BotClient_Valve_Battery(p, bot_index,iAdd);
}

void BotClient_DOD_Battery(void *p, int bot_index,int iAdd)
{
	// this is just like the Valve Battery message
	BotClient_Valve_Battery(p, bot_index,iAdd);
}

// This message gets sent when the bots are getting damaged.
void BotClient_Valve_Damage(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	static int damage_armor;
	static int damage_taken;
	static int damage_bits;  // type of damage being done
	static Vector damage_origin;
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state++;
			damage_armor = *(int *)p;
		}
		else if (state == 1){
			state++;
			damage_taken = *(int *)p;
		}
		else if (state == 2){
			state++;
			damage_bits = *(int *)p;
		}
		else if (state == 3){
			state++;
			damage_origin.x = *(float *)p;
		}
		else if (state == 4){
			state++;
			damage_origin.y = *(float *)p;
		}
		else if (state == 5){
			damage_origin.z = *(float *)p;
			
			if ((damage_armor > 0) || (damage_taken > 0))
			{
				// ignore certain types of damage...
				if (damage_bits & IGNORE_DAMAGE)
					return;
				
				// if the bot doesn't have an enemy and someone is shooting at it then
				// turn in the attacker's direction...
				if (bots[bot_index]->pBotEnemy == NULL)
				{
					bots[bot_index]->VSuspEn = damage_origin;
					bots[bot_index]->Action.lAction|=BA_SUSPLOC;
					/*// face the attacker...
					Vector v_enemy = damage_origin - bots[bot_index]->pEdict->v.origin;
					Vector bot_angles = UTIL_VecToAngles( v_enemy );
					
					  bots[bot_index]->pEdict->v.ideal_yaw = bot_angles.y;
					  
					BotFixIdealYaw(bots[bot_index]->pEdict);*/
				}
			}
			
			state = 0;
		}
	}
}

void BotClient_CS_Damage(void *p, int bot_index,int iAdd)
{
	// this is just like the Valve Battery message
	BotClient_Valve_Damage(p, bot_index,iAdd);
}

void BotClient_Gearbox_Damage(void *p, int bot_index,int iAdd)
{
	// this is just like the Valve Battery message
	BotClient_Valve_Damage(p, bot_index,iAdd);
}

void BotClient_DOD_Damage(void *p, int bot_index,int iAdd)
{
	// this is just like the Valve Battery message
	BotClient_Valve_Damage(p, bot_index,iAdd);
}

// This message gets sent when the bots money ammount changes (for CS)
void BotClient_CS_Money(void *p, int bot_index,int iAdd)
{
	static int state = 0;   // current state machine state
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state ++;
			
			((CBotCS *)(bots[bot_index]))->bot_money = *(int *)p;  // amount of money
		}
		else{
			state = 0;  // ingore this field
		}
	}
	else{
		state = 0;  // ingore this field
	}
}

void BotClient_CS_StatusIcon(void *p, int bot_index,int iAdd)
{
	static int state = 0;   // current state machine state
	static bool btemp;
	
	if(iAdd != _CLIENT_END){
		if(state == 0){
			btemp = *(bool*)p;
			state ++;
		}
		else if (state == 1){
			state ++;
			
			if(FStrEq((char *)p,"buyzone")){
				btemp?bots[bot_index]->Action.lAction|=BA_BUYZONE:bots[bot_index]->Action.lAction&=~BA_BUYZONE;
			}
			if(FStrEq((char *)p,"defuser")){
				btemp?bots[bot_index]->Action.lAction|=BA_DEFKIT:bots[bot_index]->Action.lAction&=~BA_DEFKIT;
			}
		}
	}
	else
		state = 0;
}

void BotClient_CS_RoundTime(void *p, int bot_index,int iAdd)
{
	static int state = 0;   // current state machine state
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state ++;
			
			float f_round_time = *(short *)p;  // roundtime in seconds

			if (f_round_time <= g_mp_freezetime->value)
				f_end_freezetime = g_mp_freezetime->value + gpGlobals->time;  // set end of freezetime

			bots[bot_index]->f_round_time = f_round_time;
		}
		else{
			state = 0;  // ingore this field
		}
	}
	else{
		state = 0;
	}
}

void BotClient_CS_StatusValue(void *p, int bot_index,int iAdd)						// not yet impl
{
	static int state = 0;   // current state machine state
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state ++;
		}
		else if (state == 1){
			state ++;
		}
		else{
			state = 0;  // ingore this field
		}
	}
	else{
		state = 0;
	}
}

void BotClient_CS_SetFOV(void *p, int bot_index,int iAdd)
{
	static int state = 0;   // current state machine state
	
	if(iAdd != _CLIENT_END){
		if (state == 0){
			bots[bot_index]->i_FOV = *(int*)(p);
			state ++;
			//cout << bots[bot_index]->i_FOV << endl;
		}
		else{
			state = 0;  // ingore this field
		}
	}
	else{
		state = 0;
	}
}

void BotClient_CS_TextMsg(void *p, int bot_index,int iAdd){
	static int state = 0;   // current state machine state
	//static int sNumber = 0;
	static char szStrings[10][32]={"","","","","","","","","",""};
	// assuming 10 is max sentences
	if(iAdd != _CLIENT_END){
		if (state == 0){
			//sNumber = *(int*)p;
			//sNumber++;
			state ++;				// ignore this field
		}
		else{
			strcpy(szStrings[state-1],(char*)p);		// copy string
#ifdef __LOG
			FILE *fhd;fhd=fopen("gotaradiomess.txt","a");fprintf(fhd,"%s\n",szStrings[state -1]);fclose(fhd);
#endif
			state ++;
		}
	}
	else{
#ifdef __LOG
		FILE *fhd;fhd=fopen("gotaradiomess.txt","a");fprintf(fhd,"--------------------");fclose(fhd);
#endif
		if(FStrEq(szStrings[0],"#Switch_To_BurstFire")){
			((CBotCS*)(bots[bot_index]))->bGlockBurst = true;
		}
		if(FStrEq(szStrings[0],"#Switch_To_SemiAuto")){
			((CBotCS*)(bots[bot_index]))->bGlockBurst = false;
		}
		else if(FStrEq(szStrings[0],"#Bomb_Planted")){
			g_bBombPlanted = true;
			g_iBombExplode = int(gpGlobals->time + CVAR_GET_FLOAT("mp_c4timer"));
		}
		else if(FStrEq(szStrings[0],"#Game_bomb_drop")){
			g_bBombDropped = true;
		}
		else if(FStrEq(szStrings[0],"#Got_bomb")){
			g_bBombDropped = false;
		}
		else if(FStrEq(szStrings[0],"#Terrorists_Win")){
			g_bBombPlanted = false;
		}
		else if(FStrEq(szStrings[0],"#Bomb_Defused")){
			g_bBombPlanted = false;
		}
		else if(FStrEq(szStrings[0],"#CTs_Win")){
			g_bBombPlanted = false;
		}
		else if(FStrEq(szStrings[0],"#Game_Commencing")){
			g_fGameCommenced = gpGlobals->time;
		}
		else if(FStrEq(szStrings[0],"#Cant_buy")){
			((CBotCS*)(bots[bot_index]))->f_buytime = gpGlobals->time + 60.0;			// just to avoid too many buy trials
		}
		else if(FStrEq(szStrings[0],"#Game_teammate_attack")){
			// get edict of radio using player
			edict_t *pEnt = 0;
			CBotCS *pBot = ((CBotCS*)(bots[bot_index]));
			edict_t *pEdict = pBot->pEdict;
			int i;
			
			pBot->bot_IRadioM.pECalling = 0;		// resetting calling edict
			pBot->bot_IRadioM.lMessage = 0;
			
			for (i = 0; i < gpGlobals->maxClients; i++){
				pEnt = INDEXENT(i + 1);
				
				// skip invalid players and skip self (i.e. this bot)
				if ((pEnt) && (!pEnt->free) && (pEnt != pEdict))
				{
					if(!IsAlive(pEnt))
						continue;
					if(FStrEq(szStrings[1],STRING(pEnt->v.netname))){// if found ent with that name
						pBot->bot_IRadioM.pECalling = pEnt;					// copy pointer to ent
						
						pBot->bot_IRadioM.f_Time = gpGlobals->time + RANDOM_FLOAT(1.0,2.5);		// this random stuff for faking kind of reaction time to radio commands
						
						pBot->bot_IRadioM.lMessage = RC_TEAMATTACK;
					}
				}
			}
		}
		else if(FStrEq(szStrings[0],"#Game_radio")){
			// get edict of radio using player
			edict_t *pEnt = 0;
			CBotCS *pBot = ((CBotCS*)(bots[bot_index]));
			edict_t *pEdict = pBot->pEdict;
			int i;
			
			f_LastRadio = gpGlobals->time;			// set last time a radio command was used
			
			pBot->bot_IRadioM.pECalling = 0;		// resetting calling edict
			pBot->bot_IRadioM.lMessage = 0;
			
			for (i = 0; i < gpGlobals->maxClients; i++){
				pEnt = INDEXENT(i + 1);
				
				// skip invalid players and skip self (i.e. this bot)
				if ((pEnt) && (!pEnt->free) && (pEnt != pEdict))
				{
					if(!IsAlive(pEnt))
						continue;
					if(FStrEq(szStrings[1],STRING(pEnt->v.netname))){// if found ent with that name
						pBot->bot_IRadioM.pECalling = pEnt;					// copy pointer to ent
						
						pBot->bot_IRadioM.f_Time = gpGlobals->time + RANDOM_FLOAT(.75,2.0);		// this random stuff for faking kind of reaction time to radio commands
						
						if(FStrEq(szStrings[2],"#Cover_me")){
							pBot->bot_IRadioM.lMessage = RC_COVER_ME;
						}
						else if(FStrEq(szStrings[2],"#You_take_the_point")){
							pBot->bot_IRadioM.lMessage = RC_YOU_TAKE_THE_POINT;
						}
						else if(FStrEq(szStrings[2],"#Hold_this_position")){
							pBot->bot_IRadioM.lMessage = RC_HOLD_THIS_POSITION;
						}
						else if(FStrEq(szStrings[2],"#Regroup_team")){
							pBot->bot_IRadioM.lMessage = RC_REGROUP_TEAM;
						}
						else if(FStrEq(szStrings[2],"#Follow_me")){
							pBot->bot_IRadioM.lMessage = RC_FOLLOW_ME;
						}
						else if(FStrEq(szStrings[2],"#Taking_fire")){
							pBot->bot_IRadioM.lMessage = RC_TAKING_FIRE;
						}
						else if(FStrEq(szStrings[2],"#Go_go_go")){
							pBot->bot_IRadioM.lMessage = RC_GO_GO_GO;
						}
						else if(FStrEq(szStrings[2],"#Team_fall_back")){
							pBot->bot_IRadioM.lMessage = RC_TEAM_FALL_BACK;
						}
						else if(FStrEq(szStrings[2],"#Stick_together_team")){
							pBot->bot_IRadioM.lMessage = RC_STICK_TOGETHER_TEAM;
						}
						else if(FStrEq(szStrings[2],"#Get_in_position_and_wait")){
							pBot->bot_IRadioM.lMessage = RC_GET_IN_POSITION_AND_WAIT;
						}
						else if(FStrEq(szStrings[2],"#Storm_the_front")){
							pBot->bot_IRadioM.lMessage = RC_STORM_THE_FRONT;
						}
						else if(FStrEq(szStrings[2],"#Report_in_team")){
							pBot->bot_IRadioM.lMessage = RC_REPORT_IN_TEAM;
						}
						else if(FStrEq(szStrings[2],"#Affirmative")||FStrEq(szStrings[2],"#Roger_that")){
							pBot->bot_IRadioM.lMessage = RC_AFFIRMATIVE;
						}
						else if(FStrEq(szStrings[2],"#Enemy_spotted")){
							pBot->bot_IRadioM.lMessage = RC_ENEMY_SPOTTED;
						}
						else if(FStrEq(szStrings[2],"#Need_backup")){
							pBot->bot_IRadioM.lMessage = RC_NEED_BACKUP;
						}
						else if(FStrEq(szStrings[2],"#Sector_clear")){
							pBot->bot_IRadioM.lMessage = RC_SECTOR_CLEAR;
						}
						else if(FStrEq(szStrings[2],"#In_position")){
							pBot->bot_IRadioM.lMessage = RC_IN_POSITION;
						}
						else if(FStrEq(szStrings[2],"#Reporting_in")){
							pBot->bot_IRadioM.lMessage = RC_REPORTING_IN;
						}
						else if(FStrEq(szStrings[2],"#Get_out_of_there")){
							pBot->bot_IRadioM.lMessage = RC_GET_OUT_OF_THERE;
						}
						else if(FStrEq(szStrings[2],"#Negative")){
							pBot->bot_IRadioM.lMessage = RC_NEGATIVE;
						}
						else if(FStrEq(szStrings[2],"#Enemy_down")){
							pBot->bot_IRadioM.lMessage = RC_ENEMY_DOWN;
						}
						else if(FStrEq(szStrings[2],"#Fire_in_the_hole")){
							//pBot->bot_IRadioM.lMessage = RC_ENEMY_DOWN;
							// do nothing, cause this isnt important cause it's handled by pther funcs
						}
						else if(FStrEq(szStrings[2],"#Hostage_down")){
							pBot->bot_IRadioM.lMessage = RC_HOSTAGE_DOWN;
						}
						else{
							pBot->bot_IRadioM.lMessage = 0;
							FILE *fhd;fhd=fopen("gotaranunknowndiomess.txt","a");fprintf(fhd,"%s\n",szStrings[2]);fclose(fhd);
						}
						break;
					}
				}
			}
		}
		state = 0;			// reset state machine state
		memset(szStrings,0,sizeof(char) * 10 * 32);
		return;
	}
}

void BotClient_Valve_ScreenFade(void *p, int bot_index,int iAdd)
{
	static int state = 0;   // current state machine state
	static int duration;
	static int hold_time;
	static int fade_flags;
	int length;
	
	if(iAdd != _CLIENT_END){
		if (state == 0)
		{
			state++;
			duration = *(int *)p;
		}
		else if (state == 1)
		{
			state++;
			hold_time = *(int *)p;
		}
		else if (state == 2)
		{
			state++;
			fade_flags = *(int *)p;
		}
		else if (state == 6)
		{
			state = 0;
			
			length = (duration + hold_time) / 4096;
			bots[bot_index]->f_blinded_time = gpGlobals->time + length*2/3 - 1;
		}
		else
		{
			state++;
		}
	}
}

void BotClient_CS_ScreenFade(void *p, int bot_index,int iAdd)
{
	// this is just like the Valve ScreenFade message
	BotClient_Valve_ScreenFade(p, bot_index,iAdd);
}

void BotClient_DOD_ScreenFade(void *p, int bot_index,int iAdd)
{
	// this is just like the Valve ScreenFade message
	BotClient_Valve_ScreenFade(p, bot_index,iAdd);
}

// This message gets sent when the bots get killed
void BotClient_Valve_DeathMsg(void *p, int bot_index,int iAdd)
{
	static int state = 0;   // current state machine state
	static int killer_index;
	static int victim_index;
	static int iHeadShot;
	static edict_t *victim_edict=0,
		*killer_edict=0;
	static int killer_bot_index,victim_bot_index;
	
	if(iAdd != _CLIENT_END){
		if (state == 0)
		{
			state++;
			killer_index = *(int *)p;  // ENTINDEX() of killer
		}
		else if (state == 1)
		{
			state++;
			victim_index = *(int *)p;  // ENTINDEX() of victim
		}
		else if (state == 2)
		{
			state ++;
			iHeadShot = *(int*)p;		// true when headshot
		}
		else if( state == 3){		// (char*)p would be name of weapon
			//cout << (char*)p << endl;
			char szWeaponclass[32];
			long lKWeapon;
			strcpy(szWeaponclass,"weapon_");
			strcat(szWeaponclass,(char*)p);
			lKWeapon = CBotBase::WeaponClass2ID(szWeaponclass);

			state = 0;
			
			if(victim_index >= 0 && victim_index < 32)victim_edict = INDEXENT(victim_index);
			else victim_edict = 0;
			if(victim_index >= 0 && victim_index < 32)killer_edict = INDEXENT(killer_index);
			else killer_edict = 0;

			victim_bot_index = UTIL_GetBotIndex(victim_edict);
			killer_bot_index = UTIL_GetBotIndex(killer_edict);
			
			//index = UTIL_GetBotIndex(victim_edict);

			// is this message about a bot being killed?
			
			if ((killer_index == 0) || (killer_index == victim_index))
			{
				// bot killed by world (worldspawn) or bot killed self...
				if (victim_bot_index != -1){
					bots[victim_bot_index]->killer_edict = NULL;
				}
			}
			else
			{
				// notice bot that has been killed
				if (victim_bot_index != -1){
					bots[victim_bot_index]->KilledBy(killer_edict,lKWeapon);
				}
				// store information to killer
				
				//index = killer_bot_index;
				
				if(killer_bot_index != -1
					&&!FNullEnt(victim_index)){
					bots[killer_bot_index]->KilledSO(victim_edict,lKWeapon);
					//bots[killer_bot_index]->pEKilled = victim_edict;
				}
			}
			
			if(killer_edict != victim_edict){
				int i;
				for (i=0; i < 32; i++){
					if (bots[i]){
						bots[i]->pLastGlobalKill = killer_edict;
						bots[i]->pLastGlobalKilled = victim_edict;
					}
				}
				
				// do some stuff for statistics
				WPStat.AddKill(killer_edict,victim_edict);
			}
		}
	}
	else
		state = 0;	
}

void BotClient_CS_DeathMsg(void *p, int bot_index,int iAdd)
{
	// this is just like the Valve DeathMsg message
	BotClient_Valve_DeathMsg(p, bot_index,iAdd);
}

void BotClient_DOD_DeathMsg(void *p, int bot_index,int iAdd)
{
	//cout << "nak" << endl;
   	static int state = 0;   // current state machine state
	static int killer_index;
	static int victim_index;
	static edict_t *victim_edict,*killer_edict;
	static int killer_bot_index,victim_bot_index,killer_team,victim_team;
	
	if(iAdd != _CLIENT_END){
		if (state == 0)
		{
			state++;
			killer_index = *(int *)p;  // ENTINDEX() of killer
		}
		else if (state == 1)
		{
			state++;
			victim_index = *(int *)p;  // ENTINDEX() of victim
		}
		else if( state == 2){		// (char*)p would be name of weapon
			//cout << (char*)p << endl;
			char szWeaponclass[32];
			long lKWeapon;
			strcpy(szWeaponclass,"weapon_");
			strcat(szWeaponclass,(char*)p);
			lKWeapon = CBotBase::WeaponClass2ID(szWeaponclass);

			state = 0;
			
			victim_edict = INDEXENT(victim_index);
			killer_edict = INDEXENT(killer_index);
			victim_bot_index = UTIL_GetBotIndex(victim_edict);
			killer_bot_index = UTIL_GetBotIndex(killer_edict);
			
			//index = UTIL_GetBotIndex(victim_edict);
			
			// is this message about a bot being killed?
			
			if ((killer_index == 0) || (killer_index == victim_index))
			{
				// bot killed by world (worldspawn) or bot killed self...
				if (victim_bot_index != -1){
					bots[victim_bot_index]->killer_edict = NULL;
				}
			}
			else
			{
				// store edict of player that killed this bot...
				if (victim_bot_index != -1){
					bots[victim_bot_index]->killer_edict = killer_edict;

					// store information with which weapon the bot has been killed
					bots[victim_bot_index]->FLKW[bots[victim_bot_index]->iCLKW] = lKWeapon;
					bots[victim_bot_index]->iCLKW ++;
					if(bots[victim_bot_index]->iCLKW >= CBotBase::_MAXLKW){
						bots[victim_bot_index]->iCLKW = 0;
					}
				}
				// store information to killer
				
				//index = killer_bot_index;
				
				if(killer_bot_index != -1){
					bots[killer_bot_index]->pEKilled = victim_edict;
				}
			}
			
			if(killer_edict != victim_edict){
				if(victim_bot_index != -1 && killer_bot_index != -1){
					// update LTM of bot
					if(victim_edict&&killer_edict){
						victim_team = UTIL_GetTeam(victim_edict); 
						killer_team = UTIL_GetTeam(killer_edict);
						if(killer_team != victim_team){		// no tk
							if(victim_bot_index != -1)
								bots[victim_bot_index]->LTMem.Add(LTM_KILLED,victim_edict,killer_edict->v.origin,victim_edict->v.origin);
							
							if(killer_bot_index != -1)
								bots[killer_bot_index]->LTMem.Add(LTM_KILL,killer_edict,victim_edict->v.origin,killer_edict->v.origin);
						}
						else{
							if(victim_bot_index!=-1){
								Vector VTemp = Vector(0,0,0);
								bots[victim_bot_index]->LTMem.Add(LTM_TKILL,killer_edict,killer_edict->v.origin,VTemp);
							}
						}
					}
				}
				
				int i;
				for (i=0; i < 32; i++){
					if (bots[i]){
						bots[i]->pLastGlobalKill = killer_edict;
					}
				}
				
				// do some stuff for statistics
				//WPStat.AddKill(killer_edict,victim_edict);
			}
		}
	}
	else
		state = 0;
}

void BotClient_DOD_Stamina(void *p, int bot_index,int iAdd)
{
	static int state = 0;   // current state machine state
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state ++;
			
			((CBotDOD * )(bots[bot_index]))->iStamina = *(int *)p;
		}
		else{
			state = 0;  // ingore this field
		}
	}
}

void BotClient_DOD_Speed(void *p, int bot_index,int iAdd)
{
	static int state = 0;   // current state machine state
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state ++;
			
			//bots[bot_index]->f_max_speed = *(int *)p;
		}
		else{
			state = 0;  // ingore this field
		}
	}
	else{
		state = 0;
	}
}

void BotClient_DOD_Slowed(void *p, int bot_index,int iAdd)
{
	static int state = 0;   // current state machine state
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state ++;
			
			((CBotDOD * )(bots[bot_index]))->bSlowed = *(bool *)p;
		}
		else{
			state = 0;  // ingore this field
		}
	}
	else{
		state = 0;
	}
}

void BotClient_DOD_Bleeding(void *p, int bot_index,int iAdd)
{
	static int state = 0;   // current state machine state
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state ++;
			
			((CBotDOD * )(bots[bot_index]))->bBleeding = *(bool *)p;
		}
		else{
			state = 0;  // ingore this field
		}
	}
	else{
		state = 0;
	}
}

void BotClient_DOD_Object(void *p, int bot_index,int iAdd)
{
	/*static int state = 0;   // current state machine state
	static char szString[80];

	if(iAdd != _CLIENT_END){
		if (state == 0){
			strcpy(szString,(char *)p);
			if(FStrEq(szString,"sprites/obj_tnt.spr")){
				((CBotDOD * )(bots[bot_index]))->bTNT = true;
			}
		}
	}
	else{
		state = 0;
	}*/
}

void BotClient_CS_Flashlight(void *p, int bot_index,int iAdd)
{
	static int state = 0;   // current state machine state
	static bool bFlashlight;
	static int iStrength;
	if(iAdd != _CLIENT_END){
		if (state == 0){
			state ++;
			
			bFlashlight = *(bool*)p;
		}
		if (state == 1){
			state ++;
			
			iStrength = *(int*)p;
			
			bots[bot_index]->bFlashlight = bFlashlight;
		}
	}
	else{
		state = 0;
	}
}

void JBRegMsgs(void)
{
	static bool bInitDone = FALSE;

	if (bInitDone) return;

	// initialize bot messages array
	for (int i = 0; i < MAX_REG_MSGS; i++)
		botmsgs[i] = NULL;

	switch(mod_id)
	{
		case VALVE_DLL:
			botmsgs[GET_USER_MSG_ID(PLID, "WeaponList", NULL)] = BotClient_Valve_WeaponList;
			botmsgs[GET_USER_MSG_ID(PLID, "CurWeapon", NULL)] = BotClient_Valve_CurWeapon;
			botmsgs[GET_USER_MSG_ID(PLID, "AmmoX", NULL)] = BotClient_Valve_AmmoX;
			botmsgs[GET_USER_MSG_ID(PLID, "AmmoPickup", NULL)] = BotClient_Valve_AmmoPickup;
			botmsgs[GET_USER_MSG_ID(PLID, "WeapPickup", NULL)] = BotClient_Valve_WeapPickup;
			botmsgs[GET_USER_MSG_ID(PLID, "ItemPickup", NULL)] = BotClient_Valve_ItemPickup;
			botmsgs[GET_USER_MSG_ID(PLID, "Health", NULL)] = BotClient_Valve_Health;
			botmsgs[GET_USER_MSG_ID(PLID, "Battery", NULL)] = BotClient_Valve_Battery;
			botmsgs[GET_USER_MSG_ID(PLID, "Damage", NULL)] = BotClient_Valve_Damage;
			break;

		/*case TFC_DLL:
			botmsgs[GET_USER_MSG_ID(PLID, "WeaponList", NULL)] = BotClient_TFC_WeaponList;
			botmsgs[GET_USER_MSG_ID(PLID, "VGUIMenu", NULL)] = BotClient_TFC_VGUIMenu;
			botmsgs[GET_USER_MSG_ID(PLID, "CurWeapon", NULL)] = BotClient_TFC_CurWeapon;
			botmsgs[GET_USER_MSG_ID(PLID, "AmmoX", NULL)] = BotClient_TFC_AmmoX;
			botmsgs[GET_USER_MSG_ID(PLID, "AmmoPickup", NULL)] = BotClient_TFC_AmmoPickup;
			botmsgs[GET_USER_MSG_ID(PLID, "WeapPickup", NULL)] = BotClient_TFC_WeapPickup;
			botmsgs[GET_USER_MSG_ID(PLID, "ItemPickup", NULL)] = BotClient_TFC_ItemPickup;
			botmsgs[GET_USER_MSG_ID(PLID, "Health", NULL)] = BotClient_TFC_Health;
			botmsgs[GET_USER_MSG_ID(PLID, "Battery", NULL)] = BotClient_TFC_Battery;
			botmsgs[GET_USER_MSG_ID(PLID, "Damage", NULL)] = BotClient_TFC_Damage;
			break;*/

		case CSTRIKE_DLL:
			botmsgs[GET_USER_MSG_ID(PLID, "WeaponList", NULL)] = BotClient_CS_WeaponList;
			botmsgs[GET_USER_MSG_ID(PLID, "VGUIMenu", NULL)] = BotClient_CS_VGUIMenu;
			botmsgs[GET_USER_MSG_ID(PLID, "CurWeapon", NULL)] = BotClient_CS_CurWeapon;
			botmsgs[GET_USER_MSG_ID(PLID, "AmmoX", NULL)] = BotClient_CS_AmmoX;
			botmsgs[GET_USER_MSG_ID(PLID, "AmmoPickup", NULL)] = BotClient_CS_AmmoPickup;
			botmsgs[GET_USER_MSG_ID(PLID, "WeapPickup", NULL)] = BotClient_CS_WeapPickup;
			botmsgs[GET_USER_MSG_ID(PLID, "ItemPickup", NULL)] = BotClient_CS_ItemPickup;
			botmsgs[GET_USER_MSG_ID(PLID, "Health", NULL)] = BotClient_CS_Health;
			botmsgs[GET_USER_MSG_ID(PLID, "Battery", NULL)] = BotClient_CS_Battery;
			botmsgs[GET_USER_MSG_ID(PLID, "Damage", NULL)] = BotClient_CS_Damage;
			botmsgs[GET_USER_MSG_ID(PLID, "ShowMenu", NULL)] = BotClient_CS_ShowMenu;
			botmsgs[GET_USER_MSG_ID(PLID, "Money", NULL)] = BotClient_CS_Money;
			botmsgs[GET_USER_MSG_ID(PLID, "StatusIcon", NULL)] = BotClient_CS_StatusIcon;
			botmsgs[GET_USER_MSG_ID(PLID, "RoundTime", NULL)] = BotClient_CS_RoundTime;
			botmsgs[GET_USER_MSG_ID(PLID, "SetFOV", NULL)] = BotClient_CS_SetFOV;
			botmsgs[GET_USER_MSG_ID(PLID, "ScreenFade", NULL)] = BotClient_CS_ScreenFade;
			botmsgs[GET_USER_MSG_ID(PLID, "TextMsg", NULL)] = BotClient_CS_TextMsg;
			botmsgs[GET_USER_MSG_ID(PLID, "Flashlight", NULL)] = BotClient_CS_Flashlight;
			botmsgs[GET_USER_MSG_ID(PLID, "SayText", NULL)] = BotClient_CS_SayText;
			/*botmsgs[GET_USER_MSG_ID(PLID, "StatusValue", NULL)] = BotClient_CS_StatusValue;*/
			break;

		case DOD_DLL:
			botmsgs[GET_USER_MSG_ID(PLID, "WeaponList", NULL)] = BotClient_DOD_WeaponList;
			botmsgs[GET_USER_MSG_ID(PLID, "VGUIMenu", NULL)] = BotClient_DOD_VGUIMenu;
			botmsgs[GET_USER_MSG_ID(PLID, "CurWeapon", NULL)] = BotClient_DOD_CurWeapon;
			botmsgs[GET_USER_MSG_ID(PLID, "AmmoX", NULL)] = BotClient_DOD_AmmoX;
			botmsgs[GET_USER_MSG_ID(PLID, "AmmoPickup", NULL)] = BotClient_DOD_AmmoPickup;
			botmsgs[GET_USER_MSG_ID(PLID, "WeapPickup", NULL)] = BotClient_DOD_WeapPickup;
			botmsgs[GET_USER_MSG_ID(PLID, "ItemPickup", NULL)] = BotClient_DOD_ItemPickup;
			botmsgs[GET_USER_MSG_ID(PLID, "Health", NULL)] = BotClient_DOD_Health;
			botmsgs[GET_USER_MSG_ID(PLID, "Battery", NULL)] = BotClient_DOD_Battery;
			botmsgs[GET_USER_MSG_ID(PLID, "Damage", NULL)] = BotClient_DOD_Damage;
			botmsgs[GET_USER_MSG_ID(PLID, "ScreenFade", NULL)] = BotClient_DOD_ScreenFade;
			botmsgs[GET_USER_MSG_ID(PLID, "Stamina", NULL)] = BotClient_DOD_Stamina;
			botmsgs[GET_USER_MSG_ID(PLID, "Slowed", NULL)] = BotClient_DOD_Slowed;
			botmsgs[GET_USER_MSG_ID(PLID, "Speed", NULL)] = BotClient_DOD_Speed;
			botmsgs[GET_USER_MSG_ID(PLID, "Bleeding", NULL)] = BotClient_DOD_Bleeding;
			botmsgs[GET_USER_MSG_ID(PLID, "Object", NULL)] = BotClient_DOD_Object;
			break;

		/*case GEARBOX_DLL:
			botmsgs[GET_USER_MSG_ID(PLID, "WeaponList", NULL)] = BotClient_Gearbox_WeaponList;
			botmsgs[GET_USER_MSG_ID(PLID, "CurWeapon", NULL)] = BotClient_Gearbox_CurWeapon;
			botmsgs[GET_USER_MSG_ID(PLID, "AmmoX", NULL)] = BotClient_Gearbox_AmmoX;
			botmsgs[GET_USER_MSG_ID(PLID, "AmmoPickup", NULL)] = BotClient_Gearbox_AmmoPickup;
			botmsgs[GET_USER_MSG_ID(PLID, "WeapPickup", NULL)] = BotClient_Gearbox_WeapPickup;
			botmsgs[GET_USER_MSG_ID(PLID, "ItemPickup", NULL)] = BotClient_Gearbox_ItemPickup;
			botmsgs[GET_USER_MSG_ID(PLID, "Health", NULL)] = BotClient_Gearbox_Health;
			botmsgs[GET_USER_MSG_ID(PLID, "Battery", NULL)] = BotClient_Gearbox_Battery;
			botmsgs[GET_USER_MSG_ID(PLID, "Damage", NULL)] = BotClient_Gearbox_Damage;
			break;*/
	}

	bInitDone = TRUE;
}

#ifndef USE_METAMOD
void AddUserMsg(const char *msgname, int msgid, int size)
{
	// Adapted from Pierre-Marie Baty's RACC 

	int i;

	// is this message NOT already registered ?
	for (i = 0; i < usermsgs_count; i++)
		if (FStrEq(usermsgs[i].name, msgname))
			break; // cycle through usermsgs types array and break when a similar record is found

	// now keep track (or update if necessary) this user message in our own array...
	usermsgs[i].name = msgname; // record this message's name
	usermsgs[i].id = msgid; // record this message's index
	usermsgs[i].size = size; // record this message's size

	// are we certain this message has not been registered in the past ?
	if (i == usermsgs_count)
		usermsgs_count++; // we know now one user message more
}

int GetUserMsgId(const char *msgname, int *size)
{
	// Adapted from Pierre-Marie Baty's RACC 

	// this function returns the user message id of the recorded message named msgname. Local
	// variables have been made static to speedup recurrent calls of this function.

	static int i;

	// is it a standard engine message (i.e, NOT a user message, already registered by engine) ?
	if (FStrEq("TempEntity", msgname)) {
		if (size) *size = strlen("TempEntity");
		return (SVC_TEMPENTITY); // return the correct message ID
	}
	else if (FStrEq("Intermission", msgname)) {
		if (size) *size = strlen("Intermission");
		return (SVC_INTERMISSION); // return the correct message ID
	}
	else if (FStrEq("CDTrack", msgname)) {
		if (size) *size = strlen("CDTrack");
		return (SVC_CDTRACK); // return the correct message ID
	}
	else if (FStrEq("WeaponAnim", msgname)) {
		if (size) *size = strlen("WeaponAnim");
		return (SVC_WEAPONANIM); // return the correct message ID
	}
	else if (FStrEq("RoomType", msgname)) {
		if (size) *size = strlen("RoomType");
		return (SVC_ROOMTYPE); // return the correct message ID
	}
	else if (FStrEq("Director", msgname)) {
		if (size) *size = strlen("Director");
		return (SVC_DIRECTOR); // return the correct message ID
	}

	// cycle through our known user message types array
	for (i = 0; i < usermsgs_count; i++)
		if (FStrEq(usermsgs[i].name, msgname)) {
			if (size) *size = usermsgs[i].size;
			return (usermsgs[i].id); // return the id of the user message named msgname
		}

	// unregistered user message, have the engine register it
	return (pfnRegUserMsg (msgname, -1)); // ask the engine to register this new message
}

const char *GetUserMsgName(int msgid, int *size)
{
	// Adapted from Pierre-Marie Baty's RACC 

	// this function returns the user message name of the recorded message index msgid. Local
	// variables have been made static to speedup recurrent calls of this function.

	static int i;

	// is it a standard engine message (i.e, NOT a user message, already registered by engine) ?
	switch(msgid) {
		case SVC_TEMPENTITY:
			if (size) *size = -1;
			return ("TempEntity"); // return the correct message name
		case SVC_INTERMISSION:
			if (size) *size = -1;
			return ("Intermission"); // return the correct message name
		case SVC_CDTRACK:
			if (size) *size = -1;
			return ("CDTrack"); // return the correct message name
		case SVC_WEAPONANIM:
			if (size) *size = -1;
			return ("WeaponAnim"); // return the correct message name
		case SVC_ROOMTYPE:
			if (size) *size = -1;
			return ("RoomType"); // return the correct message name
		case SVC_DIRECTOR:
			if (size) *size = -1;
			return ("Director"); // return the correct message name
		default:
			// cycle through our known user message types array
			for (i = 0; i < usermsgs_count; i++)
				if (usermsgs[i].id == msgid) {
					if (size) *size = usermsgs[i].size;
					return (usermsgs[i].name); // return the name of the user message having the msgid id
				}
	}

	return (NULL); // unregistered user message
}
#endif /* USE_METAMOD */
