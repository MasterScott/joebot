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

#include "extdll.h"
#include "util.h"

#ifdef USE_METAMOD
#define SDK_UTIL_H  // util.h already included
#include "meta_api.h"
#endif /* USE_METAMOD */

#include "bot.h"

#include "bot_names.h"
#include "CBotCS.h"
#include "CBotDOD.h"
#include "Commandfunc.h"
#include "CSkill.h"

// names ..
CBotNames Names;

//skill file
extern CSkill Skill;

// include NNSim
#include "NeuralNet.h"
#include "som.h"

// array for fnctions to buy weapons
void (*Buy[32])(CBotBase *);

int iGlobalRSCount;		// setting to number of bots on roundstart. every init of bot result in decrementing this variable. if it is 0 and it hasn't been called this rs, a global init function is called :D
float fLGlobalRSInit = -1000;

// include defs for NN
#include "BotNNDefs.h"
#include "NNWeapon.h"
NNWeapon WeaponDefs;
float gnn_lrate			= _DEFAULTNNLRATE;				// default lrate
edict_t *g_pVIP = 0;
float fLastVIPScan		= -10000;

float f_ES[32];											// enemy seen times
float fM[2];			// manner of teams

// input and output fields for NN
double dCollNNIn	[3];
double dCollNNOut	[1];

#include <sys/types.h>
#include <sys/stat.h>

extern bot_weapon_t weapon_defs[MAX_WEAPONS];
extern edict_t *clients[32];
extern int mod_id;
extern WAYPOINT waypoints[MAX_WAYPOINTS];
extern int num_waypoints;  // number of waypoints currently in use
edict_t *pEdictPlayer=0;

CBaseNeuralNetFF *NNCombat = 0;
CBaseNeuralNetFF *NNColl = 0;
SOMPattern SP(6);
bool bNNInit = false;
bool bNNInitError;

cvar_t *g_sv_maxspeed;		// sv_maxspeed cvar
cvar_t *g_mp_freezetime;	// mp_freezetime cvar

SInfo SBInfo[32];
CBotBase *bots[32];   // max of 32 bots in a game

bool   need_init = true;

bool g_bBombPlanted;		// bomb has been planted
float g_iBombExplode;
bool g_bBombDropped;

#ifdef _WIN32
extern HINSTANCE h_Library;
#else
extern void * h_Library;
#endif

void InitGlobalRS(void){
	if(!iGlobalRSCount){
		//if(fabs(fLGlobalRSInit - gpGlobals->time) > 30){
		fLGlobalRSInit = gpGlobals->time;
		
		iGlobalRSCount = 1;		// do not execute this a second time this round
		
		int iRoamteamA[32];		// roamteam aischlm -> how many bots are already followischlng thischls player ?
		memset(iRoamteamA,0,sizeof(int)*32);
		
		edict_t *p,*pEBot,*pFE[32],*pEnt,*pNot;
		int piDist[32][32];
		int ischl,iBotschl,i;
		long lCount,lreturn;
		for(ischl=0;ischl<32;ischl++){
			for(iBotschl=0;iBotschl<32;iBotschl++){
				piDist[ischl][iBotschl] = 10000;
			}
		}
		for(ischl=gpGlobals->maxClients;ischl>=0;ischl--){
			for(iBotschl=gpGlobals->maxClients;iBotschl>=0;iBotschl--){
				if(iBotschl > ischl)
					continue;

				pEBot = INDEXENT(iBotschl);
				pEnt = INDEXENT(ischl);

				if(pEBot && pEnt && (!pEBot->free) && (!pEnt->free)){
					piDist[ischl][iBotschl] = int((pEBot->v.origin-pEnt->v.origin).Length());
					piDist[iBotschl][ischl] = piDist[ischl][iBotschl];
				}
			}
		}

		for(ischl=0;ischl<gpGlobals->maxClients;ischl++){
			pEBot = INDEXENT(ischl);
			iBotschl = UTIL_GetBotIndex(pEBot);
			if(iBotschl != -1){
				if(iRoamteamA[ischl] > 0)		// that means some bot is already following
					continue;
				if(bots[iBotschl]->bot_weapons&(1<<CS_WEAPON_C4))		// the bot with the c4 shouldn't follow others, they should follow him :)
					continue;
				// find a player without BT_ROAMTEAM or BT_COVER ( from getrandomplayerwo )
				pNot = pEBot;
				
				lCount = 0;
				for (i = gpGlobals->maxClients; i ; i--){
					if(piDist[i][ischl] > 400)
						continue;
					pEnt = INDEXENT(i);
					if ((pEnt) && (!pEnt->free) && (pEnt != pNot)){
						if(!IsAlive(pEnt))
							continue;
						
						if(iRoamteamA[i] > gpGlobals->maxClients/6)
							continue;
						
						if(UTIL_GetTeam(pEBot) != UTIL_GetTeam(pEnt))
							continue;
						
						int i = UTIL_GetBotIndex(pEnt);
						if(i != -1){
							if(((CBotCS*)bots[i])->Task.SearchP(pNot) != -1
								|| ((CBotCS*)bots[i])->Task.SearchT( BT_COVER|BT_ROAMTEAM ) != -1){
								continue;
							}
						}
						pFE[lCount] = pEnt;
						lCount++;
					}
				}
				
				if(lCount){
					lreturn=RANDOM_LONG(1,lCount)-1;
					p = pFE[lreturn];
				}
				else
					p=0;
				
				// end searchin entity
				
				if(p){
					int i2follow = UTIL_ClientIndex(p);
					
					iRoamteamA[i2follow]++;
					
					//cout << STRING(bots[iBotschl]->pEdict->v.netname) <<"("<<ischl<<")"<< " follows " << STRING(p->v.netname) <<"("<<i2follow<<")"<< endl;
					
					bots[iBotschl]->Task.AddTask(BT_ROAMTEAM,gpGlobals->time + 300.0,0,p,0);
				}
			}
		}
	}
}

//extern "C"
//{
// this is the LINK_ENTITY_TO_CLASS function that creates a player (bot)
//void player(entvars_t *pev);
//}

void player( entvars_t *pev )
{
	static LINK_ENTITY_FUNC otherClassName = NULL;
	if (otherClassName == NULL)
		otherClassName = (LINK_ENTITY_FUNC)GetProcAddress(h_Library, "player");
	if (otherClassName != NULL){
		(*otherClassName)(pev);
	}
}

double ConvertDistance(double dDistance){			// convert distance to [-1;1]
	double dReturn;
	
	const double dnear = 200;
	const double dmid = 1000;
	
	if(dDistance < dnear){
		dReturn = dDistance / dnear - 1.0;
	}
	if(dDistance > dnear && dDistance < dmid){
		dReturn = (dDistance-dnear) / (dmid-dnear);
	}
	if(dDistance > dmid){
		dReturn = 1;
	}
	return dReturn;
}

void BotCreate( edict_t *pPlayer, const char *szTeam, const char *szClass,const char *szName, const char *szSkill)
{
	gf_5th = 0;
	//cout << "botcreate" << endl;
	edict_t *BotEnt = 0;
	CBotBase *pBot;
	int index; // index of pointer to bot ( bot[] )
	
	// initialize the bots array of structures if first time here...
	if (need_init)
	{
		need_init = false;
		//memset(bots, 0, sizeof(bots));
		for(long l=0;l<32;l++){
		}

		g_sv_maxspeed = CVAR_GET_POINTER("sv_maxspeed");
		g_mp_freezetime = CVAR_GET_POINTER("mp_freezetime");
	}
	
	const CBotNamesItem *pName = NULL;
	char szNameThis[32];
	char szTempName[32];
	int iSkill;
	
	////////////////////////////
	pName = Names.getName();					// get name from list
	strcpy(szNameThis,pName->m_szName);
	index = 0;
	while ((bots[index]) && (index < 32))
		index++;
	
	if (index == 32){
		UTIL_ConsoleMessage( pPlayer, "Can't create bot!\n");
		return;
	}
	
	if(mod_id == CSTRIKE_DLL||mod_id == CSCLASSIC_DLL){
		bots[index] = new CBotCS;
	}
	else if(mod_id == DOD_DLL){
		bots[index] = new CBotDOD;
	}
	
	pBot = bots[index];
	
	if(!szName || strlen(szName)<2 || FStrEq(szName,"default")|| FStrEq(szName,"unnamed")){}
	else{
		strcpy(szNameThis,szName);
	}
	
	strcpy(bots[index]->name,szNameThis);
	bots[index]->Personality.Load(bots[index]->name);
	
	pBot->d_Manner = pBot->Personality.fAggressiveness;
	
	////////////////////////////
	
	if(!szSkill || !strlen(szSkill)){
		iSkill = RANDOM_LONG(int(jb_skillmin->value),int(jb_skillmax->value));
	}
	else{
		sscanf(szSkill,"%i",&iSkill);
		if(!iSkill || iSkill == -1)
			iSkill = RANDOM_LONG(int(jb_skillmin->value),int(jb_skillmax->value));
	}
	
	if(pBot->Personality.iSkill != -1){
		pBot->bot_skill = pBot->Personality.iSkill;
	}
	else
		pBot->bot_skill = iSkill;
	
	if(!szName || strlen(szName)<2 || FStrEq(szName,"default")|| FStrEq(szName,"unnamed")){// name is default or no specified, get it from bot_names.txt
		CBotBase :: MakeName(szTempName,pName->m_szName,pBot->bot_skill,bots[index]->d_Manner);
	}
	else{
		CBotBase :: MakeName(szTempName,szName,pBot->bot_skill,bots[index]->d_Manner);
		strcpy(szNameThis,szName);
		strcpy(bots[index]->name,szNameThis);
	}
	
	BotEnt = CREATE_FAKE_CLIENT( szTempName );
		
	//strcpy(bots[index]->name,szTempName);
	
	if (FNullEnt( BotEnt )){
		if(index != -1 && index < 32){
			delete bots[index];
			bots[index] = 0;
		}
		if (pPlayer){
			UTIL_ConsoleMessage( pPlayer, "Max players reached; can't create bot!\n");
		}
	}
	else
	{
		char ptr[128];  // allocate space for message from ClientConnect
		char *infobuffer;
		int clientIndex;
		
		if (IS_DEDICATED_SERVER())
			LOG_MESSAGE(PLID, "Creating bot...");
		else if (pPlayer)
			UTIL_ConsoleMessage( pPlayer, "Creating bot...\n");

		//FREE_PRIVATE(BotEnt);
		//BotEnt->pvPrivateData = 0;
		
		// create the player entity by calling MOD's player function
		// (from LINK_ENTITY_TO_CLASS for player object)
		
		player( VARS(BotEnt) );
		
		infobuffer = GET_INFOKEYBUFFER( BotEnt );
		clientIndex = ENTINDEX( BotEnt );
		
		SET_CLIENT_KEYVALUE( clientIndex, infobuffer, "model", "gina" );
		
		if (mod_id == CSTRIKE_DLL||mod_id == CSCLASSIC_DLL){
			SET_CLIENT_KEYVALUE( clientIndex, infobuffer, "rate", "3500.000000");
			SET_CLIENT_KEYVALUE( clientIndex, infobuffer, "cl_updaterate", "20");
			SET_CLIENT_KEYVALUE( clientIndex, infobuffer, "cl_lw", "1");
			SET_CLIENT_KEYVALUE( clientIndex, infobuffer, "cl_lc", "1");
			SET_CLIENT_KEYVALUE( clientIndex, infobuffer, "tracker", "0");
			SET_CLIENT_KEYVALUE( clientIndex, infobuffer, "cl_dlmax", "128");
			SET_CLIENT_KEYVALUE( clientIndex, infobuffer, "lefthand", "1");
			SET_CLIENT_KEYVALUE( clientIndex, infobuffer, "friends", "0");
			SET_CLIENT_KEYVALUE( clientIndex, infobuffer, "dm", "0");
			SET_CLIENT_KEYVALUE( clientIndex, infobuffer, "ah", "1");
		}
		
#ifdef USE_METAMOD
		MDLL_ClientConnect( BotEnt, "joebot", "127.0.0.1", ptr );

		int i = 0;
		while ((i < 32) && (clients[i] != NULL))
			i++;

		if (i < 32)
			clients[i] = BotEnt; // store this clients edict in the clients array

		MDLL_ClientPutInServer( BotEnt );
#else
		ClientConnect( BotEnt, "joebot", "127.0.0.1", ptr );
		ClientPutInServer( BotEnt ); // Pieter van Dijk - use instead of DispatchSpawn() - Hip Hip Hurray!
#endif
		
		BotEnt->v.flags |= FL_THIRDPARTYBOT;

		// initialize all the variables for this bot...
		SBInfo[index].respawn_state = RESPAWN_IDLE;
		SBInfo[index].kick_time  = 0;
		//bots[index]->name[0] = 0;  // name not set by server yet

		pBot->pEdict = BotEnt;
		pBot->iEIndex = clientIndex;

		pBot->f_AMomentum = jb_aimmomentum->value;
		pBot->f_ASpeed = jb_aimspeed->value;
		pBot->Action.lAction = 0;
		
		if(pBot->Personality.iSpeakLeet != -1){
			if(mod_id == CSTRIKE_DLL||mod_id == CSCLASSIC_DLL){
				((CBotCS*)(pBot))->Chat.b1337chat = (pBot->Personality.iSpeakLeet==1);
			}
			else if(mod_id == DOD_DLL){
				((CBotDOD*)(pBot))->Chat.b1337chat = (pBot->Personality.iSpeakLeet==1);
			}
		}
		
		pBot->Init();
		
		strcpy(pBot->szPWeapon,pBot->Personality.szPrefWeapon);
		
		/* ------------------------------------------------------------------- */
		
		pBot->pEKilled = NULL;

		pBot->not_started = true;  // hasn't joined game yet
		
		if (mod_id == TFC_DLL)
			pBot->start_action = MSG_TFC_IDLE;
		else if (mod_id == CSTRIKE_DLL||mod_id == CSCLASSIC_DLL)
			pBot->start_action = MSG_CS_IDLE;
		else if (mod_id == DOD_DLL)
			pBot->start_action = MSG_DOD_IDLE;
		else
			pBot->start_action = 0;  // not needed for non-team MODs
		
		pBot->SpawnInit();
		
		BotEnt->v.idealpitch = BotEnt->v.v_angle.x;
		BotEnt->v.ideal_yaw = BotEnt->v.v_angle.y;
		BotEnt->v.pitch_speed = BOT_PITCH_SPEED;
		BotEnt->v.yaw_speed = BOT_YAW_SPEED;
		
		pBot->fViewSpeedYaw=0;		// speed of changing view yaw
		pBot->fViewSpeedPitch=0;		// speed of changing view pitch
		
		pBot->bot_team = -1;  // don't know what these are yet, server can change them
		pBot->bot_class = -1;
		
		if ((mod_id == TFC_DLL) || (mod_id == CSTRIKE_DLL||mod_id == CSCLASSIC_DLL)|| (mod_id == DOD_DLL)){
			if ((szTeam != NULL) && (szTeam[0] != 0)){
				pBot->bot_team = atoi(szTeam);
				
				if ((szClass != NULL) && (szClass[0] != 0)){
					pBot->bot_class = atoi(szClass);
				}
			}
			else{
				if(bots[index]->Personality.iPrefClass != -1){
					bots[index]->bot_class = bots[index]->Personality.iPrefClass;
				}
			}
		}
	}
}
