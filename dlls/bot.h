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
// HPB_bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// bot.h
//

#ifndef BOT_H
#define BOT_H

#include "CBotCS.h"
#include "CBotDOD.h"

//#define __LOG

#include "limits.h"
#include "waypoint.h"

// stuff for Win32 vs. Linux builds

// define some function prototypes...
BOOL ClientConnect( edict_t *pEntity, const char *pszName,
				   const char *pszAddress, char szRejectReason[ 128 ] );
void ClientPutInServer( edict_t *pEntity );
void ClientCommand( edict_t *pEntity );

void FakeClientCommand(edict_t *pBot, char *arg1, char *arg2, char *arg3);

void InitGlobalRS(void);

const char *Cmd_Args( void );
const char *Cmd_Argv( int argc );
int Cmd_Argc( void );

#include "bot_globaldefs.h"
#include "CSTMem.h"

// type of map
extern int g_iTypeoM;

// freq of nn updated ( def is 10 )
extern float gnn_update;

// is the bomb planted ?
extern bool		g_bBombPlanted;
extern bool		g_bBombDropped;
extern int		g_iBaseSkillMax;
extern int		g_iBaseSkillMin;
extern float	g_fBaseAMomentum;
extern float	g_fCampPoss;
extern float	g_fBaseASpeed;
extern bool		g_autowpjump;

extern AWP_EntLogItem AWP_ED[32];

extern int g_iLanguage;				// language of most stuff

#define LANG_E	0
#define LANG_DE	1
#define LANG_FR	2

// times for enemy seen
extern float f_ES[32];

// pistol - mode
extern bool g_bOnlySec;

// should the order of names should be random
extern bool g_bMixNames;

// last time a radio command was used
extern float f_LastRadio;

extern float f_timesrs;
extern float f_round_start;

extern float gf_5th;
extern bool g_b5th;

// autowaypointing ... recording movements
extern AWP_EntLogItem AWP_ED[32];
extern ADV_WPRecMove ADVL[32];
extern bool bRec[32];
extern bool bStopRec[32];

//extern char szTemp[100];

typedef struct{
	double dppNet[2][_MAXNNLOG][5];
	long lNum;
	long lAct;
} bot_nn_log;

#include "CRadio.h"

typedef struct{
	long lAction;
	float fTime;
	edict_t *pREdict;
	Vector VOrigin;
}sState;

#define DOD_FLAG_NONE	0
#define DOD_FLAG_ALLIES	1
#define DOD_FLAG_AXIS	2

typedef struct{
	int respawn_state;
	float kick_time;
	char name[BOT_NAME_LEN+1];
	char skin[BOT_SKIN_LEN+1];
	int bot_skill;
	int bot_team;
	int bot_class;
}SInfo;

extern SInfo SBInfo[32];

// new UTIL.CPP functions...
edict_t *UTIL_FindEntityInSphere( edict_t *pentStart, const Vector &vecCenter, float flRadius );
edict_t *UTIL_FindEntityByString( edict_t *pentStart, const char *szKeyword, const char *szValue );
edict_t *UTIL_FindEntityByClassname( edict_t *pentStart, const char *szName );
edict_t *UTIL_FindEntityByTargetname( edict_t *pentStart, const char *szName );
void UTIL_SayText( const char *pText, edict_t *pEdict );
int UTIL_GetTeam(edict_t *pEntity);
int UTIL_GetBotIndex(const edict_t *pEdict);
bool UTIL_IsVIP(edict_t *pEntity);
//bot_t *UTIL_GetBotPointer(edict_t *pEdict);
bool IsAlive(edict_t *pEdict);
bool FInViewCone(Vector *pOrigin, edict_t *pEdict);
bool FVisible( const Vector &vecOrigin, edict_t *pEdict );
bool TEq(float f1,float f2,float fD);

void BotVATurnTo(edict_t *pEdict,const Vector &vector);

//bool FVisibleEx(bot_t *pBot,const Vector &vecOrigin, edict_t *pEdict );
Vector Center(edict_t *pEdict);
Vector GetGunPosition(edict_t *pEdict);
void UTIL_SelectItem(edict_t *pEdict, char *item_name);
Vector VecBModelOrigin(edict_t *pEdict);
bool UpdateSounds(edict_t *pEdict, edict_t *pPlayer);
void UTIL_ShowMenu( edict_t *pEdict, int slots, int displaytime, bool needmore, char *pText );

extern bool g_bTKPunish;
extern edict_t *g_pVIP;
extern bool g_bChat;
extern bool g_bEDown;
extern float fM[2];

void CalcDistances(void);

#endif // BOT_H
