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
#ifndef __GLOBALVARS_H
#define __GLOBALVARS_H

//#define _ROAMBEAM

class CBotBase;
class CChatHost;

extern CChatHost g_ChatHost;

extern int mod_id;

extern bool g_b5th;
extern float gf_5thd;
class CBaseNeuralNetFF;
extern CBaseNeuralNetFF *NNCombat;
extern CBaseNeuralNetFF *NNColl;
class NNWeapon;
extern NNWeapon WeaponDefs;
extern void (*Buy[32])(CBotBase *);

/*extern edict_t *pSmokeG[_MAXGRENADEREC];
extern bool bSmokeG[_MAXGRENADEREC];
extern Vector VSmokeG[_MAXGRENADEREC];
extern edict_t *pFlashG[_MAXGRENADEREC];
extern bool bFlashG[_MAXGRENADEREC];
extern Vector VFlashG[_MAXGRENADEREC];*/

// TheFatal - START
extern int g_msecnum;
extern float g_msecdel;
extern float g_msecval;
// TheFatal - END

extern float pfPlayerDistance[32][32];

extern edict_t *clients[32];

extern char szLastSayText[80];
extern int iLastSayClient;

extern char szDeadLastSayText[80];
extern int iDeadLastSayClient;

extern bool g_bTKPunish;
extern edict_t *g_pVIP;
extern bool g_bChat;
extern float _SCHATTIME;
enum ICHATMODES{IC_ALL=0,IC_DEAD,IC_ALIVE,IC_NONE};
extern int g_iIChat;
extern bool g_bEDown;
extern float fM[2];
extern bool bCheckWelcome;
extern int min_bots;
extern int max_bots;
extern edict_t *pEdictLastJoined;
extern int g_ileetposs;
extern float g_CHATFREQ;
extern bool g_bshowen;
extern int debug_engine;
extern int g_menu_waypoint;
extern bool g_waypointsound;
extern int g_menu_state;
extern bool g_waypoint_on;
extern bool g_auto_waypoint;
extern bool g_auto_addpath;
extern bool g_path_waypoint;
extern float bot_cfg_pause_time;
extern float g_fGameCommenced;
extern bool g_waypoint_stat;
extern bool g_bForceNOStat;
extern bool g_bSpray;

extern float g_GRAVITYADJ;

extern char *show_menu_1,			// pointer to texts
*show_menu_2,
*show_menu_2a,
*show_menu_2am,
*show_menu_2b,
*show_menu_2c,
*show_menu_3,
*show_menu_4,
*show_menu_5;

#define MENU_NONE  0
#define MENU_1     1
#define MENU_2     2
#define MENU_2a    6
#define MENU_2am   9
#define MENU_2b    7
#define MENU_2c    8
#define MENU_3     3
#define MENU_4     4
#define MENU_5     5

class SOMPattern;
extern SOMPattern SP;

// type of map
extern int g_iTypeoM;

extern edict_t *pEdictPlayer;

// freq of nn updated ( def is 10 )
extern float gnn_update;

//dll.cpp stuff
extern float bot_check_time;

// is the bomb planted ?
extern bool		g_bBombPlanted;
extern float	g_iBombExplode;
extern bool		g_bBombDropped;
extern int		g_iBaseSkillMax;
extern int		g_iBaseSkillMin;
extern float	g_fBaseAMomentum;
extern float	g_fCampPoss;
extern float	g_fBaseASpeed;
extern bool		g_autowpjump;
extern bool		g_waypointadv;
extern bool		b_addjoe;
extern bool		b_addskill;
extern bool		g_bUseRadio;
extern bool		bNNInitError;
extern float	TIMETOBEBORED;
extern float	TIMEBEINGBORED;
extern float	fLastVIPScan;
extern bool		g_bTestJump;
extern bool		g_bAutowpHuman;
extern long		lbeam;
extern bool		g_bJoinWHumanMAX;
extern bool		g_bJoinWHumanRES;

extern char szTemp[200];

extern CSkill Skill;

extern CSDecals SDecals;

extern CBotNames Names;

extern CBotWPDir g_WPDir;

/*class CChat;
extern CChat Chat;*/

extern float f_timesrs;

extern bool bBotsShoot;

#include "bot_wpstat.h"
extern CWPStat WPStat;

extern AWP_EntLogItem AWP_ED[32];
class CBotBase;
extern CBotBase *bots[32];

extern int g_iLanguage;				// language of most stuff

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

extern char szPrefixAgg[32];
extern char szPrefixNor[32];
extern char szPrefixDef[32];

extern char szWPCreator[80];

extern int iGlobalRSCount;		// setting to number of bots on roundstart. every init of bot result in decrementing this variable. if it is 0 and it hasn't been called this rs, a global init function is called :D
extern float fLGlobalRSInit;

extern char szLPBFile[80];
extern bool bLoadedLPB;

extern edict_t *listenserver_edict;

#endif  __GLOBALVARS_H