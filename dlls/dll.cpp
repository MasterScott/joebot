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
// dll.cpp
//

#include "extdll.h"
#include "enginecallback.h"
#include "util.h"
#include "cbase.h"
#include "entity_state.h"

#include "bot.h"
#include "CBotCS.h"
#include "CBotDOD.h"
#include "bot_func.h"
#include "bot_wpstat.h"
#include "ChatHost.h"
#include "waypoint.h"

#include "CSkill.h"

#include "CCommand.h"

#include "som.h"

#include "Gnome.h"
#include "WorldGnome.h"

CWorldGnome CWG;

#define _MAXCFGLINESPERFRAME 5

extern GETENTITYAPI other_GetEntityAPI;
extern GETNEWDLLFUNCTIONS other_GetNewDLLFunctions;
extern enginefuncs_t g_engfuncs;
#ifdef DEBUGENGINE
extern int debug_engine;
#endif
extern globalvars_t  *gpGlobals;
extern char *g_argv;

static FILE *fp;

CSkill Skill;
CSDecals SDecals;
float TIMETOBEBORED = _TIMETOBEBORED;
float TIMEBEINGBORED = _TIMEBEINGBORED;
float g_fGameCommenced = -1;

// TheFatal - START
int g_msecnum;
float g_msecdel;
float g_msecval;
// TheFatal - END

GrenadeLog_t gSmoke[_MAXGRENADEREC];
GrenadeLog_t gFlash[_MAXGRENADEREC];
HostageLog_t gHossi[_MAXHOSTAGEREC];

long lGDCount = 4;

DLL_FUNCTIONS other_gFunctionTable;
DLL_GLOBAL const Vector g_vecZero = Vector(0,0,0);

CChatHost g_ChatHost;

bool g_bSpray = true;
int mod_id = -1;
int m_spriteTexture = 0;
//int default_bot_skill = 2;
int isFakeClientCommand = 0;
int fake_arg_count;
float bot_check_time = 2.0;
int min_bots = -1;
int max_bots = -1;
int num_bots = 0;
int prev_num_bots = 0;
bool g_GameRules = FALSE;
edict_t *clients[32];
edict_t *listenserver_edict = NULL;
bool bWelcome[32];
bool bCheckWelcome = true;
float welcome_time[32];
int g_menu_waypoint;
int g_menu_state = 0;
bool b_addjoe = true;
bool b_addskill = true;
char szPrefixAgg[32]="[JOE]";
char szPrefixNor[32]="[JoE]";
char szPrefixDef[32]="[J0E]";
bool g_bForceNOStat = false;
bool g_bJoinWHumanMAX = false;		// only use max_bots when a human already joined
bool g_bJoinWHumanRES = false;		// only respawn when a human already joined

CCommands Commands;

float gf_5th=0.0;
float gf_5thd = 0;
bool g_b5th;

float pfPlayerDistance[32][32];

float is_team_play = 0.0;
//char team_names[MAX_TEAMS][MAX_TEAMNAME_LENGTH];
int num_teams = 0;
bool checked_teamplay = FALSE;
edict_t *pent_info_tfdetect = NULL;
edict_t *pent_info_ctfdetect = NULL;
edict_t *pent_info_frontline = NULL;
edict_t *pent_item_tfgoal = NULL;
int max_team_players[4];  // for TFC
int team_class_limits[4];  // for TFC
int team_allies[4];  // TFC bit mapped allies BLUE, RED, YELLOW, and GREEN
int max_teams = 0;	// for TFC
//FLAG_S flags[MAX_FLAGS];  // for TFC
int num_flags = 0;  // for TFC

FILE *bot_cfg_fp = NULL;
bool need_to_open_cfg = TRUE;
float bot_cfg_pause_time = 0.0;
float respawn_time = 0.0;
bool spawn_time_reset = FALSE;

int flf_bug_fix;  // for FLF 1.1 capture point bug
int flf_bug_check;  // for FLF 1.1 capture point bug

edict_t *pEdictLastJoined;

cvar_t sv_bot = {"joebot",""};

char *show_menu_1,			// pointer to texts
*show_menu_2,
*show_menu_2a,
*show_menu_2am,
*show_menu_2b,
*show_menu_2c,
*show_menu_3,
*show_menu_4,
*show_menu_5;


// // // /// // /// // /// /// // /// /// english joebot menu
char *E_show_menu_1 ={"\\yJoeBOT Menu\n\n\
\\w1. Waypoint stuff\n\
2. Add bot\n\
3. Kick a bot\n\
4. End round\n\n\
5. CANCEL\n\n\
6. Language\n"};

char *E_show_menu_2={"\\yWaypoint menu\n\n\
\\w1. Waypoint on/off\n\
2. Autowaypoint on/off\n\
3. Autopath on/off\n\
4. Add\n\
5. Delete\n\
6. Set flags\n\
7. Load\n\
8. Save\n\
9. Pathwaypoint\n\
"};

char *E_show_menu_2a={"\\yWaypoint flags\n\n\
\\w1. Dont Avoid\n\
2. Ladder\n\
3. Visit\n\
4. Crouch\n\
5. Aiming\n\
6. Sniper spot\n\
7. Bomb target / hostages\n\
8. Team specific\n\
9. more ...\n\
"};

char *E_show_menu_2am={"\\yWaypoint flags - 2\n\n\
\\w1. Rescue zone\n\
2. Dont avoid fall\n\
3. Jump\n\
"};

char *E_show_menu_2b={"\\yPathwaypoint menu\n\n\
\\w1. create1\n\
2. create2\n\
3. remove1\n\
4. remove2\n"};

char *E_show_menu_2c={"\\ySelect a team :\n\n\
\\w1. Counterterrorists\n\
2. Terrorists\n\
3. Not team specific\n"};

char *E_show_menu_3={"\\yAdd JoeBOT(s)\n\n\
\\w1. Add a TE\n\
2. Add a CT\n\
3. Add a ?\n\
\nFill server ... \n\
4. half with TE\n\
5. half with CT\n\
6. full with TE\n\
7. full with CT\n\
8. full\n\
"};

char *E_show_menu_4={"\\yKick JoeBOT(s)\n\n\
\\w1. kick a te\n\
2. kick a ct\n\
3. kick all te\n\
4. kick all ct\n\
5. kick all bots\n"};

char *E_show_menu_5={"\\yChoose a language\n\n\
\\w1. English\n\
2. German / Deutsch\n\
3. French / Francais\n"};

// // // /// // /// // /// /// // /// /// deutsches joebot menu
char *D_show_menu_1 ={"\\yJoeBOT Menue\n\n\
\\w1. Menu fuer Wegpunkte\n\
2. Hinzufuegen von JoeBOTs\n\
3. Entfernen von JoeBOTs\n\
4. Sofortiges Ende der Runde\n\n\
5. Abbrechen\n\n\
6. Sprache"};

char *D_show_menu_2={"\\yWPmenue\n\n\
\\w1. Wegpunkte (WP) an/aus\n\
2. Autom. WP an/aus\n\
3. Autom. Pfade an/aus\n\
4. Fuege WP hinzu\n\
5. Loesche WP\n\
6. Setze WP Eigenschaften\n\
7. Lade WP\n\
8. Speichere WP\n\
9. Pfade\n\
"};

char *D_show_menu_2a={"\\yWegpunkt Eigenschaften\n\n\
\\w1. Vermeide nichts\n\
2. Leiter\n\
3. Besuchspunkt\n\
4. Ducken\n\
5. Zielen\n\
6. Sniper Spot\n\
7. Bombe / Geiseln\n\
8. Team spezifisch\n\
9. mehr ...\n\
"};

char *D_show_menu_2am={"\\yWaypoint Eigenschaften - 2\n\n\
\\w1. Rettungs-Zone\n\
2. Verhindere nicht zu fallen\n\
3. Spring!\n\
"};

char *D_show_menu_2b={"\\yPfad Menue\n\n\
\\w1. Erstelle Start\n\
2. Erstelle Ende\n\
3. Entferne Start\n\
4. Entferne Ende\n"};

char *D_show_menu_2c={"\\yWaehle ein Team aus :\n\n\
\\w1. Counterterrorists\n\
2. Terrorists\n\
3. Nicht Team spezifisch\n"};

char *D_show_menu_3={"\\yHinzufuegen von JoeBOT(s)\n\n\
\\w1. Fuege TE hinzu\n\
2. Fuege CT hinzu\n\
3. Fuege ? hinzu\n\
\n'Fülle' Server ... \n\
4. halb mit TE\n\
5. halb mit CT\n\
6. voll mit TE\n\
7. voll mit CT\n\
8. voll\n\
"};

char *D_show_menu_4={"\\yEntfernen von JoeBOT(s)\n\n\
\\w1. Entferne einen TE\n\
2. Entferne einen CT\n\
3. Entferne alle TEs\n\
4. Entferne alle CTs\n\
5. Entferne alle Bots\n"};

char *D_show_menu_5={"\\yWaehlen sie eine Sprache : \n\n\
\\w1. English / Englisch\n\
2. Deutsch\n\
3. Franzoesisch / Francais\n"};

// french botmenu
char *F_show_menu_1 ={"\\yJoeBOT Menu\n\n\
\\w1. Utilitaires waypoints\n\
2. Ajouter un/des bot(s)\n\
3. Kicker un bot\n\
4. Fin du round\n\n\
5. ANNULER\n\n\
6. Choisis une langue\n"};

char *F_show_menu_2={"\\yWaypoint menu\n\n\
\\w1. Waypoint on/off\n\
2. Autowaypoint on/off\n\
3. Autopath on/off\n\
4. Ajouter\n\
5. Effacer\n\
6. Ajuster les attributs\n\
7. Charger\n\
8. Sauvegarder\n\
9. Pathwaypoint\n\
"};

char *F_show_menu_2a={"\\yAttributs des waypoints\n\n\
\\w1. Ignorer obstacle\n\
2. Echelle\n\
3. Visite\n\
4. Accroupi\n\
5. Viser\n\
6. Point de Snipe\n\
7. Point de bombe / hostages\n\
8. Specifique a une equipe\n\
9. Plus ...\n\
"};

char *F_show_menu_2am={"\\yAttributs des waypoints - 2\n\n\
\\w1. Zone de sauvetage\n\
2. Empecher de tomber\n\
3. Sauter\n\
"};

char *F_show_menu_2b={"\\yPathwaypoint menu\n\n\
\\w1. Creer1\n\
2. Creer2\n\
3. Enlever1\n\
4. Enlever2\n"};

char *F_show_menu_2c={"\\yChoisir un team :\n\n\
\\w1. Counter-terrorists\n\
2. Terrorists\n\
3. Au hasard\n"};

char *F_show_menu_3={"\\yAjouter JoeBOT(s)\n\n\
\\w1. Ajouter un terro\n\
2. Ajouter un counter\n\
3. Ajouter au hasard\n\
\nCompleter le serveur avec \n\
4. 1/2 TE\n\
5. 1/2 CT\n\
6. tous terros\n\
7. tous counters\n\
8. tous\n\
"};

char *F_show_menu_4={"\\yKicker JoeBOT(s)\n\n\
\\w1. kicker un terro\n\
2. kicker un counter\n\
3. kicker tous les terros\n\
4. kicker tous les counters\n\
5. kicker tous les bots\n"};

char *F_show_menu_5={"\\yChosissez une langue : \n\n\
\\w1. Anglais / English\n\
2. Allemand / German\n\
3. Francais\n"};

CBotWPDir g_WPDir;

char welcome_msg[200];
char _JOEBOTVERSION[80];
char _JOEBOTVERSIONWOOS[80]= "1.6.2";
bool bDedicatedWelcome = false;
int g_iTypeoM;
int g_iLanguage;

float f_round_start;	// time of roundstart
float f_timesrs = 1000;		// time since round start	->	updated every frame by start frame

extern bool bNNInit;
extern bool bNNInitError;

bool g_bMyBirthday;
long g_lAge;

void BotNameInit(void);
void UpdateClientData(const struct edict_s *ent, int sendweapons, struct clientdata_s *cd);
void ProcessBotCfgFile(void);

void UpdateLanguage(void){
	switch(g_iLanguage){
	case LANG_DE:
		show_menu_1		= D_show_menu_1;
		show_menu_2		= D_show_menu_2;
		show_menu_2a	= D_show_menu_2a;
		show_menu_2am	= D_show_menu_2am;
		show_menu_2b	= D_show_menu_2b;
		show_menu_2c	= D_show_menu_2c;
		show_menu_3		= D_show_menu_3;
		show_menu_4		= D_show_menu_4;
		show_menu_5		= D_show_menu_5;
		break;
	case LANG_FR:
		show_menu_1		= F_show_menu_1;
		show_menu_2		= F_show_menu_2;
		show_menu_2a	= F_show_menu_2a;
		show_menu_2am	= F_show_menu_2am;
		show_menu_2b	= F_show_menu_2b;
		show_menu_2c	= F_show_menu_2c;
		show_menu_3		= F_show_menu_3;
		show_menu_4		= F_show_menu_4;
		show_menu_5		= F_show_menu_5;
		break;
	case LANG_E:
	default:
		show_menu_1		= E_show_menu_1;
		show_menu_2		= E_show_menu_2;
		show_menu_2a	= E_show_menu_2a;
		show_menu_2am	= E_show_menu_2am;
		show_menu_2b	= E_show_menu_2b;
		show_menu_2c	= E_show_menu_2c;
		show_menu_3		= E_show_menu_3;
		show_menu_4		= E_show_menu_4;
		show_menu_5		= E_show_menu_5;
		break;
	}
}

bool bInitInfo = true;

bool MyBirthday(void){
	time_t now = time(NULL);
	tm *tm_now = localtime(&now);
	
	if(tm_now->tm_mon == 6
		&&tm_now->tm_mday==8){			// 8.7.
		g_lAge = tm_now->tm_year - 82;
		return true;
	}
	return false;
}

long CountLines(const char *szString){
	long lCount = 0;
	while(*szString){
		if(*szString=='\n')
			lCount++;
		szString ++;
	}
	return lCount;
}

void CalcDistances(void){
	edict_t *pP[32],*pPlayer;
	int i,i2;

	memset(pP,0,32*sizeof(edict_t *));

	for (i = 1; i<=gpGlobals->maxClients; i++){
		pPlayer = INDEXENT(i);
		
		// skip invalid players and skip self (i.e. this bot)
		if ((pPlayer) && (!pPlayer->free))
		{
			// skip this player if not alive (i.e. dead or dying)
			if (!IsAlive(pPlayer)){
				continue;
			}
			pP[i] = pPlayer;
			
			for(i2 = i; i2 > 0; i2--){
				if(pP[i2]){
					pfPlayerDistance[i][i2]
						= pfPlayerDistance[i2][i]
						= (pP[i]->v.origin - pP[i2]->v.origin).Length();
				}
			}
		}
	}
}

void GameDLLInit( void )
{
	long lschl;
	printf("JoeBOT: Launching DLL (CBB%liCBC%liCBD%li%li@%s)\n",sizeof(CBotBase),sizeof(CBotCS),sizeof(CBotDOD),time(NULL),"---");
	CVAR_REGISTER (&sv_bot);
	
	g_bMyBirthday = MyBirthday();
	
#ifdef __linux__
	sprintf(_JOEBOTVERSION,"%s (linux)",_JOEBOTVERSIONWOOS);
#else
	sprintf(_JOEBOTVERSION,"%s (win32)",_JOEBOTVERSIONWOOS);
#endif
#ifdef _DEBUG
	strcat(_JOEBOTVERSION," DBGV");
#endif

	sprintf(welcome_msg,"----- JoeBot %s by @$3.1415rin { http://www.joebot.net }; -----\n\0",_JOEBOTVERSION);

	for (int i=0; i<32; i++){
		clients[i] = NULL;
		for(int ii=0; ii<2; ii++){
			Buy[ii][i] = 0;
		}
		bWelcome[i] = false;
		welcome_time[i] = 5;
		bDedicatedWelcome = false;
	}
	if(bInitInfo){
		for (int i=0; i<32; i++){
			SBInfo[i].bot_class = 0;
			SBInfo[i].bot_skill = 90;
			SBInfo[i].bot_team = 5;
			SBInfo[i].kick_time = 0;
			*SBInfo[i].name = 0;
			SBInfo[i].respawn_state = RESPAWN_IDLE;
			*SBInfo[i].skin = 0;
		}
		bInitInfo = false;
	}

	memset(weapon_defs,0,sizeof(bot_weapon_t) * MAX_WEAPONS);
	
	for(lschl=0;lschl < _MAXGRENADEREC;lschl++){
		gFlash[lschl].p = 0;
		gSmoke[lschl].p = 0;
	}
	for(lschl=0;lschl < _MAXHOSTAGEREC;lschl++){
		gHossi[lschl].p = 0;
	}
	
	for(lschl = time(NULL)&255;lschl;lschl--){
		long l = RANDOM_LONG(0,100);
	}
	
	g_WPDir.Init();
	Skill.Load();
	Names.Init();
	UpdateLanguage();
	// precaching chat files - at least the standard file :)
	g_ChatHost.GetChat("texts.txt");
	
	// init func table for buying weapons
	Buy[TE][CS_WEAPON_P228]			= BotBuy_CS_WEAPON_P228_T;
	Buy[TE][CS_WEAPON_SCOUT]		= BotBuy_CS_WEAPON_SCOUT_T;
	Buy[TE][CS_WEAPON_HEGRENADE]	= BotBuy_CS_WEAPON_HEGRENADE_T;
	Buy[TE][CS_WEAPON_XM1014]		= BotBuy_CS_WEAPON_XM1014_T;
	Buy[TE][CS_WEAPON_MAC10]		= BotBuy_CS_WEAPON_MAC10_T;
	Buy[TE][CS_WEAPON_AUG]			= BotBuy_CS_WEAPON_AUG_T;
	Buy[TE][CS_WEAPON_SMOKEGRENADE] = BotBuy_CS_WEAPON_SMOKEGRENADE_T;
	Buy[TE][CS_WEAPON_ELITE]		= BotBuy_CS_WEAPON_ELITE_T;
	Buy[TE][CS_WEAPON_FIVESEVEN]	= BotBuy_CS_WEAPON_FIVESEVEN_T;
	Buy[TE][CS_WEAPON_UMP45]		= BotBuy_CS_WEAPON_UMP45_T;
	Buy[TE][CS_WEAPON_SG550]		= BotBuy_CS_WEAPON_SG550_T;
	Buy[TE][CS_WEAPON_USP]			= BotBuy_CS_WEAPON_USP_T;
	Buy[TE][CS_WEAPON_GLOCK18]		= BotBuy_CS_WEAPON_GLOCK18_T;
	Buy[TE][CS_WEAPON_AWP]			= BotBuy_CS_WEAPON_AWP_T;
	Buy[TE][CS_WEAPON_MP5NAVY]		= BotBuy_CS_WEAPON_MP5NAVY_T;
	Buy[TE][CS_WEAPON_M249]			= BotBuy_CS_WEAPON_M249_T;
	Buy[TE][CS_WEAPON_M3]			= BotBuy_CS_WEAPON_M3_T;
	Buy[TE][CS_WEAPON_M4A1]			= BotBuy_CS_WEAPON_M4A1_T;
	Buy[TE][CS_WEAPON_TMP]			= BotBuy_CS_WEAPON_TMP_T;
	Buy[TE][CS_WEAPON_G3SG1]		= BotBuy_CS_WEAPON_G3SG1_T;
	Buy[TE][CS_WEAPON_FLASHBANG]	= BotBuy_CS_WEAPON_FLASHBANG_T;
	Buy[TE][CS_WEAPON_DEAGLE]		= BotBuy_CS_WEAPON_DEAGLE_T;
	Buy[TE][CS_WEAPON_SG552]		= BotBuy_CS_WEAPON_SG552_T;
	Buy[TE][CS_WEAPON_AK47]			= BotBuy_CS_WEAPON_AK47_T;
	Buy[TE][CS_WEAPON_P90]			= BotBuy_CS_WEAPON_P90_T;
	/*Buy[TE][CS_WEAPON_HEGRENADE]	= BotBuy_CS_WEAPON_HE_T;
	Buy[TE][CS_WEAPON_FLASHBANG]	= BotBuy_CS_WEAPON_FL_T;
	Buy[TE][CS_WEAPON_SMOKEGRENADE]	= BotBuy_CS_WEAPON_SG_T;*/

	Buy[CT][CS_WEAPON_P228]			= BotBuy_CS_WEAPON_P228_CT;
	Buy[CT][CS_WEAPON_SCOUT]		= BotBuy_CS_WEAPON_SCOUT_CT;
	Buy[CT][CS_WEAPON_HEGRENADE]	= BotBuy_CS_WEAPON_HEGRENADE_CT;
	Buy[CT][CS_WEAPON_XM1014]		= BotBuy_CS_WEAPON_XM1014_CT;
	Buy[CT][CS_WEAPON_MAC10]		= BotBuy_CS_WEAPON_MAC10_CT;
	Buy[CT][CS_WEAPON_AUG]			= BotBuy_CS_WEAPON_AUG_CT;
	Buy[CT][CS_WEAPON_SMOKEGRENADE] = BotBuy_CS_WEAPON_SMOKEGRENADE_CT;
	Buy[CT][CS_WEAPON_ELITE]		= BotBuy_CS_WEAPON_ELITE_CT;
	Buy[CT][CS_WEAPON_FIVESEVEN]	= BotBuy_CS_WEAPON_FIVESEVEN_CT;
	Buy[CT][CS_WEAPON_UMP45]		= BotBuy_CS_WEAPON_UMP45_CT;
	Buy[CT][CS_WEAPON_SG550]		= BotBuy_CS_WEAPON_SG550_CT;
	Buy[CT][CS_WEAPON_USP]			= BotBuy_CS_WEAPON_USP_CT;
	Buy[CT][CS_WEAPON_GLOCK18]		= BotBuy_CS_WEAPON_GLOCK18_CT;
	Buy[CT][CS_WEAPON_AWP]			= BotBuy_CS_WEAPON_AWP_CT;
	Buy[CT][CS_WEAPON_MP5NAVY]		= BotBuy_CS_WEAPON_MP5NAVY_CT;
	Buy[CT][CS_WEAPON_M249]			= BotBuy_CS_WEAPON_M249_CT;
	Buy[CT][CS_WEAPON_M3]			= BotBuy_CS_WEAPON_M3_CT;
	Buy[CT][CS_WEAPON_M4A1]			= BotBuy_CS_WEAPON_M4A1_CT;
	Buy[CT][CS_WEAPON_TMP]			= BotBuy_CS_WEAPON_TMP_CT;
	Buy[CT][CS_WEAPON_G3SG1]		= BotBuy_CS_WEAPON_G3SG1_CT;
	Buy[CT][CS_WEAPON_FLASHBANG]	= BotBuy_CS_WEAPON_FLASHBANG_CT;
	Buy[CT][CS_WEAPON_DEAGLE]		= BotBuy_CS_WEAPON_DEAGLE_CT;
	Buy[CT][CS_WEAPON_SG552]		= BotBuy_CS_WEAPON_SG552_CT;
	Buy[CT][CS_WEAPON_AK47]			= BotBuy_CS_WEAPON_AK47_CT;
	Buy[CT][CS_WEAPON_P90]			= BotBuy_CS_WEAPON_P90_CT;
	/*Buy[CT][CS_WEAPON_HEGRENADE]	= BotBuy_CS_WEAPON_HE_CT;
	Buy[CT][CS_WEAPON_FLASHBANG]	= BotBuy_CS_WEAPON_FL_CT;
	Buy[CT][CS_WEAPON_SMOKEGRENADE]	= BotBuy_CS_WEAPON_SG_CT;*/
	
	if (!bNNInit){
		// init SOMPattern
		//SP.SetMaxPatternNum(1000000);
		char szFileNameNN[80];
		//try{
		UTIL_BuildFileName(szFileNameNN,"joebot","nn.br3");
		//NNCombat->LoadFile(szFileNameNN);
		CBaseNeuralNet *NNCombatP=0;
		LoadNet(NNCombatP,szFileNameNN);
		NNCombat = (CBaseNeuralNetFF *)NNCombatP;
		if (IS_DEDICATED_SERVER())
			if(NNCombat)
				printf("JoeBOT: loading neural network: %s\n", szFileNameNN);
			else
				printf("JoeBOT: error loading neural network: %s\n", szFileNameNN);
			if(!NNCombat)
				NNCombat = new CNeuralNetBProp;
			
			bNNInitError = false;
			/*}catch(...){		// just create a fuckin' NN to avoid any unforseen consequences
			if (IS_DEDICATED_SERVER())
			printf("JoeBOT : %s - havn't found NN - creating fake ones, to prevent crashing\n",szFileNameNN);
			NNCombat->SetLayerNum(4);
			NNCombat->SetNNeuronsOnLayer(0,IEND);
			NNCombat->SetNNeuronsOnLayer(1,7);
			NNCombat->SetNNeuronsOnLayer(2,7);
			NNCombat->SetNNeuronsOnLayer(3,OEND);
			NNCombat->AllocNeurons();
			NNCombat->ConnectLayer(0,1);
			NNCombat->ConnectLayer(1,2);
			NNCombat->ConnectLayer(2,3);
			bNNInitError = true;
	}*/
			try{
				UTIL_BuildFileName(szFileNameNN,"joebot","nnc.br3");
				//NNColl.LoadFile(szFileNameNN);
				CBaseNeuralNet *NNCollP=0;
				LoadNet(NNCollP,szFileNameNN);
				NNColl = (CBaseNeuralNetFF *)NNCollP;
				if (IS_DEDICATED_SERVER())
					if(NNColl)
						printf("JoeBOT: loading neural network: %s\n", szFileNameNN);
					else
						printf("JoeBOT: error loading neural network: %s\n", szFileNameNN);
					if(!NNColl)
						NNColl = new CNeuralNetBProp;
					
					bNNInitError = false;
			}catch(...){		// just create a fuckin' NN to avoid any unforseen consequences
				if (IS_DEDICATED_SERVER())
					printf("JoeBOT: %s - havn't found NN - creating fake ones, to prevent crashing\n",szFileNameNN);
				NNColl = new CNeuralNetBProp;
				NNCombat = new CNeuralNetBProp;
				bNNInitError = true;
			}
			bNNInit = true;
	}

	// prop nets  -  kind of precaching
	//cout << NNCombat << endl << NNColl << endl << endl;
	NNCombat->Propagate();
	NNColl->Propagate();

	if (IS_DEDICATED_SERVER())
		printf("JoeBOT: 'Precaching' nets\n");
	
	//other_gFunctionTable.pfnGameInit;
	(*other_gFunctionTable.pfnGameInit)();
}

int DispatchSpawn( edict_t *pent )
{
	int index;
	
	if (gpGlobals->deathmatch)
	{
		char *pClassname = (char *)STRING(pent->v.classname);
		
#ifdef DEBUGENGINE
		if (debug_engine)
		{
			fp=fopen("bot.txt","a");
			fprintf(fp, "DispatchSpawn: %x %s\n",pent,pClassname);
			if (pent->v.model != 0)
				fprintf(fp, " model=%s\n",STRING(pent->v.model));
			fclose(fp);
		}
#endif
		
		if (!strcmp(pClassname, "worldspawn"))
		{
			g_bMyBirthday = MyBirthday();		// check if it is my birthday :D
			
			// do level initialization stuff here...
			WaypointInit();
			WaypointLoad(NULL);
			WPStat.Save();
			WPStat.Load();
			
			SDecals.Load();
			
			memset(AWP_ED,0,sizeof(AWP_EntLogItem)*32);
			
			for (int i=0; i<32; i++){
				bWelcome[i] = false;
				welcome_time[i] = 0;
			}
			bDedicatedWelcome = false;
			
			// determining g_iTypeoM
			if(!strncmp(STRING(gpGlobals->mapname),"cs",sizeof(char)*2)){
				g_iTypeoM = MT_CS;
			}
			else if(!strncmp(STRING(gpGlobals->mapname),"de",sizeof(char)*2)){
				g_iTypeoM = MT_DE;
			}
			else if(!strncmp(STRING(gpGlobals->mapname),"as",sizeof(char)*2)){
				g_iTypeoM = MT_AS;
			}
			else if(!strncmp(STRING(gpGlobals->mapname),"es",sizeof(char)*2)){
				g_iTypeoM = MT_ES;
			}
			else
				g_iTypeoM = MT_CS;
			
			pent_info_tfdetect = NULL;
			pent_info_ctfdetect = NULL;
			pent_info_frontline = NULL;
			pent_item_tfgoal = NULL;
			
			for (index=0; index < 4; index++)
			{
				max_team_players[index] = 0;  // no player limit
				team_class_limits[index] = 0;  // no class limits
				team_allies[index] = 0;
			}
			
			max_teams = 0;
			num_flags = 0;
			
			PRECACHE_SOUND("weapons/xbow_hit1.wav");      // waypoint add
			PRECACHE_SOUND("weapons/mine_activate.wav");  // waypoint delete
			PRECACHE_SOUND("common/wpn_hudoff.wav");      // path add/delete start
			PRECACHE_SOUND("common/wpn_hudon.wav");       // path add/delete done
			PRECACHE_SOUND("common/wpn_moveselect.wav");  // path add/delete cancel
			PRECACHE_SOUND("common/wpn_denyselect.wav");  // path add/delete error
			PRECACHE_SOUND("vox/loading.wav");
			PRECACHE_SOUND("vox/save.wav");
			PRECACHE_SOUND("player/sprayer.wav");
			
			m_spriteTexture = PRECACHE_MODEL( "sprites/lgtning.spr");
			
			g_GameRules = TRUE;
			
			is_team_play = 0.0;
			num_teams = 0;
			checked_teamplay = FALSE;
			
			bot_cfg_pause_time = 0.0;
			respawn_time = 0.0;
			spawn_time_reset = FALSE;
			
			prev_num_bots = num_bots;
			num_bots = 0;
			
			flf_bug_fix = 0;
			flf_bug_check = 0;
			
			bot_check_time = gpGlobals->time + _PAUSE_TIME;
			gf_5th = 0;
		}
	}
	
	return (*other_gFunctionTable.pfnSpawn)(pent);
}

void DispatchThink( edict_t *pent )
{
	(*other_gFunctionTable.pfnThink)(pent);
}

void DispatchUse( edict_t *pentUsed, edict_t *pentOther )
{
	(*other_gFunctionTable.pfnUse)(pentUsed, pentOther);
}

void DispatchTouch( edict_t *pentTouched, edict_t *pentOther )
{
	(*other_gFunctionTable.pfnTouch)(pentTouched, pentOther);
}

void DispatchBlocked( edict_t *pentBlocked, edict_t *pentOther )
{
	(*other_gFunctionTable.pfnBlocked)(pentBlocked, pentOther);
}

void DispatchKeyValue( edict_t *pentKeyvalue, KeyValueData *pkvd )
{
	static edict_t *temp_pent;
	static int flag_index;
	
	//   fp=fopen("bot.txt","a"); fprintf(fp, "DispatchKeyValue: %x %s=%s\n",pentKeyvalue,pkvd->szKeyName,pkvd->szValue); fclose(fp);
	
	(*other_gFunctionTable.pfnKeyValue)(pentKeyvalue, pkvd);
}

void DispatchSave( edict_t *pent, SAVERESTOREDATA *pSaveData )
{
	(*other_gFunctionTable.pfnSave)(pent, pSaveData);
}

int DispatchRestore( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity )
{
	return (*other_gFunctionTable.pfnRestore)(pent, pSaveData, globalEntity);
}

void DispatchObjectCollsionBox( edict_t *pent )
{
	(*other_gFunctionTable.pfnSetAbsBox)(pent);
}

void SaveWriteFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount )
{
	(*other_gFunctionTable.pfnSaveWriteFields)(pSaveData, pname, pBaseData, pFields, fieldCount);
}

void SaveReadFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount )
{
	(*other_gFunctionTable.pfnSaveReadFields)(pSaveData, pname, pBaseData, pFields, fieldCount);
}

void SaveGlobalState( SAVERESTOREDATA *pSaveData )
{
	(*other_gFunctionTable.pfnSaveGlobalState)(pSaveData);
}

void RestoreGlobalState( SAVERESTOREDATA *pSaveData )
{
	(*other_gFunctionTable.pfnRestoreGlobalState)(pSaveData);
}

void ResetGlobalState( void )
{
	(*other_gFunctionTable.pfnResetGlobalState)();
}

BOOL ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ]  )
{ 
	if (gpGlobals->deathmatch)
	{
		int i;
		int count = 0;
		
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp, "ClientConnect: pent=%x name=%s\n",pEntity,pszName); fclose(fp); }
#endif
		
		// check if this client is the listen server client
		if (!strcmp(pszAddress, "loopback"))
		{
			// save the edict of the listen server client...
			listenserver_edict = pEntity;
			
			//bot_cfg_pause_time = gpGlobals->time + .5f;
		}
		
		// check if this is NOT a bot joining the server...
		if (strcmp(pszAddress, "127.0.0.1"))
		{
			// don't try to add bots for 60 seconds, give client time to get added
			//bot_check_time = gpGlobals->time + 60.0;
			bot_check_time = gpGlobals->time + _PAUSE_TIME*2;
			/*
			for (i=0; i < 32; i++)
			{
				if (bots[i])  // count the number of bots in use
					count++;
			}
			
			// if there are currently more than the minimum number of bots running
			// then kick one of the bots off the server...
			if ((count > min_bots) && (min_bots != -1))
			{
				for (i=0; i < 32; i++){
					if (bots[i]){  // is this slot used?
						sprintf(szTemp, "kick \"%s\"\n", STRING(bots[i]->pEdict->v.netname));
						
						SERVER_COMMAND(szTemp);  // kick the bot using (kick "name")
						
						break;
					}
				}
			}*/
			int clientindex = UTIL_ClientIndex(pEntity);
			if(clientindex != -1){
				welcome_time[clientindex] = gpGlobals->time + 10.0f;		// set welcome time to some seconds in the future :D
				bWelcome[clientindex] = false;
			}
		}
		pEdictLastJoined = pEntity;
		long lProp = long(200.f / float(UTIL_ClientsInGame()));
		for (i=0; i < 32; i++){
			if (bots[i]){
				if(RANDOM_LONG(0,100) < lProp){
					bots[i]->Chat.l_ChatEvent |= E_WELCOME;
				}
			}
		}
	}
	
	return (*other_gFunctionTable.pfnClientConnect)(pEntity, pszName, pszAddress, szRejectReason);
}

void ClientDisconnect( edict_t *pEntity )
{
	if (gpGlobals->deathmatch)
	{
		int i;
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp, "ClientDisconnect: %x\n",pEntity); fclose(fp); }
#endif
		
		i = 0;
		while ((i < 32) && (clients[i] != pEntity))
			i++;
		
		if (i < 32)
			clients[i] = NULL;
		
		i = UTIL_GetBotIndex(pEntity);
		if(i!=-1){
			if ( FBitSet( VARS( pEntity )->flags, FL_FAKECLIENT ) )	// fix by Leon Hartwig
			{
				//FREE_PRIVATE( pEntity );
			}
			
			// someone kicked this bot off of the server...
			// copy stuff
			SBInfo[i].bot_class = bots[i]->bot_class;
			SBInfo[i].bot_skill = bots[i]->bot_skill;
			SBInfo[i].bot_team = bots[i]->bot_team;
			strcpy(SBInfo[i].name,bots[i]->name);
			strcpy(SBInfo[i].skin,bots[i]->skin);
			
			// save nn and stats
			char szFileName[80];
			WPStat.Save();
			
			UTIL_BuildFileName(szFileName,"joebot","nn_changed.br3");
			NNCombat->SaveFile(szFileName);
			
			SBInfo[i].kick_time = gpGlobals->time;  // save the kicked time
			
			delete bots[i];
			bots[i] = 0;
		}
	}
	
	(*other_gFunctionTable.pfnClientDisconnect)(pEntity);
}

void ClientKill( edict_t *pEntity )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp, "ClientKill: %x\n",pEntity); fclose(fp); }
#endif
	(*other_gFunctionTable.pfnClientKill)(pEntity);
}

void ClientPutInServer( edict_t *pEntity )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp, "ClientPutInServer: %x\n",pEntity); fclose(fp); }
#endif
	
	int i = 0;
	
	while ((i < 32) && (clients[i] != NULL))
		i++;
	
	if (i < 32)
		clients[i] = pEntity;  // store this clients edict in the clients array
	
	if (!FBitSet( pEntity->v.flags, FL_FAKECLIENT ))
	{
		int count = 0;
		for (i=0; i < 32; i++)
		{
			if (bots[i])  // count the number of bots in use
				count++;
		}
		
		// if there are currently more than the minimum number of bots running
		// then kick one of the bots off the server...
		if ((count > min_bots) && (min_bots != -1))
		{
			for (i=0; i < 32; i++){
				if (bots[i]){  // is this slot used?
					sprintf(szTemp, "kick \"%s\"\n", STRING(bots[i]->pEdict->v.netname));
					
					SERVER_COMMAND(szTemp);  // kick the bot using (kick "name")
					
					break;
				}
			}
		}
	}

	(*other_gFunctionTable.pfnClientPutInServer)(pEntity);
}

void FillServer(int iType, int iTypeAdd){
/*
iType	:	FILL_0
FILL_1
FILL_ALL
iTypeAdd:	FILL_HALF
FILL_FULL
	*/
	int iP0 = 0,		// which players are there already ?
		iP1 = 0,
		iPAll = 0;
	int iNeed2Fill,iFillTeam,ischl=0;
	
	int i;
	edict_t *pEnt;
	for (i = gpGlobals->maxClients; i; i--){
		pEnt = INDEXENT(i);
		if (!FNullEnt(pEnt) && (!pEnt->free)){
			if(!strncmp(STRING(pEnt->v.classname),"player",6)){
				char *infobuffer; 
				char cl_name[128]; 
				cl_name[0]=NULL; 
				infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)(pEnt); 
				strcpy(cl_name,g_engfuncs.pfnInfoKeyValue(infobuffer, "name")); 
				if(cl_name[0]!=NULL) 
				{ 
					iPAll ++;
					if(UTIL_GetTeam(pEnt) == 0){
						iP0++;
					}
					else
						iP1++;
				}
			}
		}
	}
	if(iTypeAdd == FILL_FULL){
		if(iType == FILL_ALL){
			iNeed2Fill = gpGlobals->maxClients - iPAll;
		}
		else if(iType == FILL_0){
			iNeed2Fill = gpGlobals->maxClients - iP0;
		}
		else if(iType == FILL_1){
			iNeed2Fill = gpGlobals->maxClients - iP1;
		}
		if(iNeed2Fill < 1)
			return;
	}
	else{
		if(iType == FILL_ALL){
			iNeed2Fill = gpGlobals->maxClients/2 - iPAll;
		}
		else if(iType == FILL_0){
			iNeed2Fill = gpGlobals->maxClients/2 - iP0;
		}
		else if(iType == FILL_1){
			iNeed2Fill = gpGlobals->maxClients/2 - iP1;
		}
		if(iNeed2Fill < 1)
			return;
	}
	if(iType == FILL_0)
		iFillTeam = 0;
	else if(iType == FILL_1)
		iFillTeam = 1;
	else 
		iFillTeam = 4;			// later 1 is added
	
	ischl = 0;
	//cout << iNeed2Fill<<"-"<<iPAll<<"-"<<gpGlobals->maxClients <<endl;
	while(iNeed2Fill && ischl < 32){
		if(!bots[ischl]){
			iNeed2Fill --;
			
			*SBInfo[ischl].name = 0;
			*SBInfo[ischl].skin = 0;
			SBInfo[ischl].bot_team = iFillTeam+1;
			SBInfo[ischl].bot_skill = -1;
			SBInfo[ischl].bot_class = -1;
			SBInfo[ischl].respawn_state = RESPAWN_NEED_TO_RESPAWN;
			SBInfo[ischl].kick_time = gpGlobals->time;
			
			//cout << "^";
		}
		ischl ++;
	}
	respawn_time = 0;
}

void KickBots(edict_t *pEntity,int iTeam,int iAll){
	int i;
	char szName[100];
	for(i=0;i<32;i++){
		if(bots[i]){
			sprintf(szName, "kick \"%s\"\n", STRING(bots[i]->pEdict->v.netname));
			if(iTeam != -1){
				if(UTIL_GetTeam(bots[i]->pEdict) == iTeam){
					SERVER_COMMAND(szName);
					if(!iAll)
						return;
				}
			}
			else{
				SERVER_COMMAND(szName);
				if(!iAll)
					return;
			}
		}
	}
}

#define MAX_TRIES 500
#define _MAXERROR .2
#define MAX_CTRIES 10

void TrainNN(edict_t *pEntity){
/*char szDir[80];
if(mod_id == CSTRIKE_DLL){
#ifdef __linux__
sprintf(szDir,"cstrike/joebot/");
#else
sprintf(szDir,"cstrike\\joebot\\");
#endif
}
else if(mod_id == DOD_DLL){
#ifdef __linux__
sprintf(szDir,"dod/joebot/");
#else
sprintf(szDir,"dod\\joebot\\");
#endif
}
char szLoadText[80];
char szSave[80];
char msg[200];
strcpy(szLoadText,szDir);
strcat(szLoadText,"nntrain.pta");
strcpy(szSave,szDir);
strcat(szSave,"nntrain.ptt");

	CPattern NNCPattern;
	try{
	NNCPattern.LoadText(szLoadText);
	}
	catch(...){
	sprintf(msg,"Error loading %s\n",szLoadText);
	ClientPrint( VARS(pEntity), HUD_PRINTNOTIFY, msg);
	return;
	}	
	sprintf(msg,"Sucessfully loading %s\n",szLoadText);
	ClientPrint( VARS(pEntity), HUD_PRINTNOTIFY, msg);
	NNCPattern.Save(szSave);
	NNCPattern.SetNN(NNCombat);
	
	 NNCombat->InitConnections(-.3,.3);
	 
	  bool bflag=true;
	  long lloop=0,lschl,lEpoch=0;
	  double dError;
	  while(bflag){
	  lloop++;
	  for(lschl=0;lschl < MAX_TRIES;lschl++){
	  NNCPattern.LearnEpochMM();
	  lEpoch++;
	  //cout << lEpoch << " ";cout.flush();
	  dError = NNCPattern.dMaxErrorMax;
	  if(dError < _MAXERROR)
	  break;
	  }
	  if(lschl >= MAX_TRIES){
	  lEpoch = 0;
	  NNCombat->InitConnections(-.3,.3);
	  sprintf(msg,"%i.after %i epochs the net could not be trained to a max error of %.2f, it's %.2f - resetting nn and trying again\n",lloop,MAX_TRIES, _MAXERROR,dError);
	  ClientPrint( VARS(pEntity), HUD_PRINTNOTIFY, msg);
	  }
	  else{
	  bflag = false;
	  }
	  if(lloop>10){
	  sprintf(msg,"%i. after %i epochs the net could be trained to a max error of %.2f - canceling training and reloading",lloop,MAX_TRIES, _MAXERROR);
	  ClientPrint( VARS(pEntity), HUD_PRINTNOTIFY, msg);
	  char filename[80];
	  UTIL_BuildFileName(filename,"joebot","nn.br3");
	  NNCombat->Save(filename);
	  return;
	  }
	  }
	  if(dError < _MAXERROR){
	  sprintf(msg,"after %i epochs the net could be trained to a max error of %.2f\nNow you can test the net and if u wish to save it, do 'savenn'\n",lloop, _MAXERROR);
	  ClientPrint( VARS(pEntity), HUD_PRINTNOTIFY, msg);
}*/
}

void Endround(void){
	int i;
	for(i=0;i<32;i++){
		if(bots[i]){
			if(IsAlive(bots[i]->pEdict)){
				bots[i]->pEdict->v.frags++;
				ClientKill(bots[i]->pEdict);
			}
		}
	}
}

void ClientCommand( edict_t *pEntity )
{
	//	int i;
	// only allow custom commands if deathmatch mode and NOT dedicated server and
	// client sending command is the listen server client...
	
	if ((gpGlobals->deathmatch) && (!IS_DEDICATED_SERVER()) &&
		(pEntity == listenserver_edict))
	{
		const char *pcmd = Cmd_Argv(0);
		const char *arg1 = Cmd_Argv(1);
		const char *arg2 = Cmd_Argv(2);
		const char *arg3 = Cmd_Argv(3);
		const char *arg4 = Cmd_Argv(4);
		const char *arg5 = Cmd_Argv(5);
		const char *arg6 = Cmd_Argv(6);
		const char *arg7 = Cmd_Argv(7);
		const char *arg8 = Cmd_Argv(8);
		//		char msg[80];
#ifdef DEBUGENGINE
		if (debug_engine)
		{
			fp=fopen("bot.txt","a"); fprintf(fp,"ClientCommand: %s",pcmd);
			if ((arg1 != NULL) && (*arg1 != 0))
				fprintf(fp," %s", arg1);
			if ((arg2 != NULL) && (*arg2 != 0))
				fprintf(fp," %s", arg2);
			if ((arg3 != NULL) && (*arg3 != 0))
				fprintf(fp," %s", arg3);
			if ((arg4 != NULL) && (*arg4 != 0))
				fprintf(fp," %s", arg4);
			fprintf(fp, "\n");
			fclose(fp);
		}
#endif
		
		if(Commands.Exec(pEntity,CM_CONSOLE,pcmd,arg1,arg2,arg3,arg4))
			return;
	}
	
	(*other_gFunctionTable.pfnClientCommand)(pEntity);
}

void ClientUserInfoChanged( edict_t *pEntity, char *infobuffer )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt", "a"); fprintf(fp, "ClientUserInfoChanged: pEntity=%x infobuffer=%s\n", pEntity, infobuffer); fclose(fp); }
#endif
	
	(*other_gFunctionTable.pfnClientUserInfoChanged)(pEntity, infobuffer);
}

void ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
	(*other_gFunctionTable.pfnServerActivate)(pEdictList, edictCount, clientMax);
}

void ServerDeactivate( void )
{
	(*other_gFunctionTable.pfnServerDeactivate)();
}

void PlayerPreThink( edict_t *pEntity )
{
	(*other_gFunctionTable.pfnPlayerPreThink)(pEntity);
}

void PlayerPostThink( edict_t *pEntity )
{
	(*other_gFunctionTable.pfnPlayerPostThink)(pEntity);
}

void SearchEs_CSTRIKE(void){		// search entities
	long lNumS = 0,lNumF=0,lNumH=0;
	edict_t *pEnt;
	char szModel[80];
	
	pEnt = 0;
	
	while(pEnt = UTIL_FindEntityByClassname(pEnt,"hostage_entity")){
		gHossi[lNumH].p = pEnt;
		gHossi[lNumH].fVelocity = (gHossi[lNumH].VOrigin-pEnt->v.origin).Length();
		gHossi[lNumH].VOrigin = pEnt->v.origin;
		gHossi[lNumH].bUsed = true;
		lNumH ++;
		
		if(lNumH >= _MAXHOSTAGEREC)
			lNumH--;
	}
	while(pEnt = UTIL_FindEntityByClassname(pEnt,"grenade")){
		strcpy(szModel,STRING(pEnt->v.model));
		if(!strcmpi(szModel,"models/w_smokegrenade.mdl")){
			gSmoke[lNumS].p = pEnt;
			gSmoke[lNumS].VOrigin = pEnt->v.origin;
			gSmoke[lNumS].bUsed = true;
			lNumS ++;
			
			if(lNumS >= _MAXGRENADEREC)
				lNumS--;
		}
		else if(!strcmpi(szModel,"models/w_flashbang.mdl")){
			gFlash[lNumF].p = pEnt;
			gFlash[lNumF].VOrigin = pEnt->v.origin;
			gFlash[lNumF].bUsed = true;
			lNumF ++;
			
			if(lNumF >= _MAXGRENADEREC)
				lNumF--;
		}
	}
	
	for(;lNumS < _MAXGRENADEREC;lNumS++){
		gSmoke[lNumS].p = 0;
		gSmoke[lNumS].bUsed = false;
	}
	for(;lNumF < _MAXGRENADEREC;lNumF++){
		gFlash[lNumF].p = 0;
		gFlash[lNumF].bUsed = false;
	}
}
#ifdef _DEBUG
void ShowInfo(void){
	if(!IS_DEDICATED_SERVER()){
		if(listenserver_edict){
			edict_t *pInfo;
			char szText[1000];
			char szTemp[200];
			char szTemp2[200];
			float fMin = 10;
		
			szText[0] = 0;
			sprintf(szTemp,"time: %.0f/%.0f\n",gpGlobals->time,f_timesrs);
			strcat(szText,szTemp);
			pInfo = GetNearestPlayer(listenserver_edict->v.origin+(Vector(0,0,2)),-1,fMin,10,listenserver_edict);
			fMin = 100000;
			if(!pInfo){
				pInfo = GetNearestPlayer(listenserver_edict,-1,fMin,true,true);
			}
			if(pInfo){
				int iBI = UTIL_GetBotIndex(pInfo);
				if(iBI == -1){
				}
				else{
					sprintf(szTemp,"%s / %3.0f%% Health / %3.0f(%3.0f) ",STRING(pInfo->v.netname),pInfo->v.health,pInfo->v.velocity.Length(),bots[iBI]->f_max_speed);
					if(bots[iBI]->HasWeapon(1<<CS_WEAPON_FLASHBANG)){
						strcat(szTemp,"F");
					}
					if(bots[iBI]->HasWeapon(1<<CS_WEAPON_HEGRENADE)){
						strcat(szTemp,"H");
					}
					if(bots[iBI]->HasWeapon(1<<CS_WEAPON_SMOKEGRENADE)){
						strcat(szTemp,"S");
					}
					if(bots[iBI]->bReplay){
						strcat(szTemp," Replay");
					}
					strcat(szTemp,"\n");
					strcat(szText,szTemp);
					
					if(bots[iBI]->current_weapon.iId > -1 && bots[iBI]->current_weapon.iId <32){
						sprintf(szTemp,"%s %i/%0.0lf/%i",weapon_defs[bots[iBI]->current_weapon.iId].szClassname,bots[iBI]->current_weapon.iClip,WeaponDefs.ipClipSize[mod_id][bots[iBI]->current_weapon.iId],bots[iBI]->current_weapon.iAmmo1);
						strcat(szText,szTemp);
						sprintf(szTemp," - M:%2.2f A:%2.2f AH:%2.2f\n",bots[iBI]->d_Manner,bots[iBI]->f_Aggressivity,bots[iBI]->f_AimHead);
						strcat(szText,szTemp);
						sprintf(szTemp," - OV %.2f %.2f / VA %.0f %.0f --- fLT %.0f\n",bots[iBI]->v_Offset.y,bots[iBI]->v_Offset.x,bots[iBI]->pEdict->v.v_angle.y,bots[iBI]->pEdict->v.v_angle.x,bots[iBI]->f_LookTo - gpGlobals->time);
						strcat(szText,szTemp);
						sprintf(szTemp,"FG:%3.i/G:%3.i/C:%3.i\n",bots[iBI]->iGoal,bots[iBI]->iFarGoal,bots[iBI]->i_CurrWP);
						strcat(szText,szTemp);
					}
					
					if(bots[iBI]->f_ducktill > gpGlobals->time){
						sprintf(szTemp,"ducking: %3.f\n",bots[iBI]->f_ducktill-gpGlobals->time);
						strcat(szText,szTemp);
					}
					if(bots[iBI]->GOrder.lTypeoG){
						sprintf(szTemp,"GOrder: State(%i) Type(%s-%i)\n",bots[iBI]->GOrder.lState,weapon_defs[bots[iBI]->GOrder.lTypeoG].szClassname,bots[iBI]->GOrder.lTypeoG);
						WaypointDrawBeamDebug(listenserver_edict,bots[iBI]->pEdict->v.origin,bots[iBI]->GOrder.VAim,10,0,200,0,0,200,10);
						strcat(szText,szTemp);
					}
					sprintf(szTemp,"%i Task(s)\n",bots[iBI]->Task.lNOT);
					strcat(szText,szTemp);
					if(bots[iBI]->Task.current){
						CTaskItem *p;

						int i;
						for(i=bots[iBI]->Task.lNOT-1;i>=0&&i>bots[iBI]->Task.lNOT - 6;i--){
							p = bots[iBI]->Task.GetTask(i);
							sprintf(szTemp,"Task(%i): ",i);
							if(p->lType & BT_COVER){
								sprintf(szTemp2,"BT_COVER ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_HELP){
								sprintf(szTemp2,"BT_HELP ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_FOLLOW){
								sprintf(szTemp2,"BT_FOLLOW ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_ROAMTEAM){
								sprintf(szTemp2,"BT_ROAMTEAM ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_GOTO){
								sprintf(szTemp2,"BT_GOTO ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_GUARD){
								sprintf(szTemp2,"BT_GUARD ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_CAMPATGOAL){
								sprintf(szTemp2,"BT_CAMPATGOAL ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_WAIT4TM8){
								sprintf(szTemp2,"BT_WAIT4TM8 ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_HOLDPOS){
								sprintf(szTemp2,"BT_HOLDPOS ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_PICKUP){
								sprintf(szTemp2,"BT_PICKUP ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_HIDE){
								sprintf(szTemp2,"BT_HIDE ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_FLEE){
								sprintf(szTemp2,"BT_FLEE ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_RELOAD){
								sprintf(szTemp2,"BT_RELOAD ");
								strcat(szTemp,szTemp2);
							}
							/*if(p->lType & BT_PAUSE){
							sprintf(szTemp2,"BT_PAUSE ");
							strcat(szTemp,szTemp2);
						}*/
							if(p->lType & BT_GOBUTTON){
								sprintf(szTemp2,"BT_GOBUTTON ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_HUNT){
								sprintf(szTemp2,"BT_HUNT ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_CROUCH){
								sprintf(szTemp2,"BT_CROUCH ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_CAMP){
								sprintf(szTemp2,"BT_CAMP ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_BLINDED){
								sprintf(szTemp2,"BT_BLINDED ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_IGNOREENEMY){
								sprintf(szTemp2,"BT_IGNOREENEMY ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_TMP){
								sprintf(szTemp2,"BT_TMP ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_DEL){
								sprintf(szTemp2,"BT_DEL ");
								strcat(szTemp,szTemp2);
							}
							if(p->lType & BT_LOCKED){
								sprintf(szTemp2,"BT_LOCKED ");
								strcat(szTemp,szTemp2);
							}
							sprintf(szTemp2,"(%2.f)\n",(p->fLive2>0?p->fLive2:p->fLive2+gpGlobals->time - 1)-gpGlobals->time);
							strcat(szTemp,szTemp2);
							strcat(szText,szTemp);
							szText[512] = 0;
						}
					}
				}
			}
			else{
				strcat(szText,"\n\nno edict to display information about\n");
			}
			
			hudtextparms_t message_params;
			
			if(!pInfo)
				pInfo = listenserver_edict;
			
			message_params.x = 0;
			message_params.y = 1;
			message_params.effect = 0;
			message_params.r1 = !(UTIL_GetTeam(pInfo))*255;
			message_params.g1 = 128;
			message_params.b1 = (UTIL_GetTeam(pInfo))*255;
			message_params.a1 = 1;
			message_params.r2 = message_params.r1;
			message_params.g2 = message_params.g1;
			message_params.b2 = message_params.b1;
			message_params.a2 = 1;
			message_params.fadeinTime = 0.00;
			message_params.fadeoutTime = 0;
			message_params.holdTime = gf_5thd+0.02f;
			message_params.fxTime = 0;
			message_params.channel = 1;

			long lNL = CountLines(szText);
			lNL = 13 - lNL;
			for(;lNL;lNL--){
				strcat(szText,"\n");
			}
			
			UTIL_ShowText(listenserver_edict,message_params,szText);
			
			int iWPNear = WaypointFindNearest(listenserver_edict,100,-1,0,false,false,false);
			if(iWPNear != -1){
			/*int ischl;
			
			for(ischl = 0;ischl < num_waypoints;ischl ++){
				if ((waypoints[ischl].flags & W_FL_DELETED) == W_FL_DELETED)
					continue;  // skip any deleted waypoints
				
				if(WPStat.GetVisible(iWPNear,ischl)){
					WaypointDrawBeam(listenserver_edict,waypoints[ischl].origin,waypoints[iWPNear].origin,5,0,200,200,200,200,2);
				}
			}*/
			}
		}
	}
}
#endif

void StartFrame( void )
{
	//try{
	//cout << RANDOM_LONG(0,10) << endl;
	//cout << 1.f/gpGlobals->frametime<<endl;
	lbeam = 0;
	//cout << "-------------------------------------------------------------------------------"<<endl;
	if (gpGlobals->deathmatch)
	{
		edict_t *pPlayer;
		static float check_server_cmd = 0.0;
		static int i, index, player_index, bot_index;
		static float previous_time = -1.0;
		static float client_update_time = 0.0;
		clientdata_s cd;
		char msg[256];
		int count;
		//check_server_cmd = 0;
		
		// if a new map has started then (MUST BE FIRST IN StartFrame)...
		if ((gpGlobals->time + 0.1) < previous_time)
		{
			char filename[256];
			char mapname[64];
			
			check_server_cmd = 0.0;  // reset at start of map
			
			// check if mapname_bot.cfg file exists...
			
			//strcpy(mapname, STRING(gpGlobals->mapname));
			strcpy(mapname, "bot.cfg");
			UTIL_BuildFileName(filename,"joebot","bot.cfg");
			
			//UTIL_BuildFileName(filename, "maps", mapname);
			
			if ((bot_cfg_fp = fopen(filename, "r")) != NULL)
			{
				sprintf(msg, "JoeBOT: Executing %s\n", filename);
				ALERT( at_console, msg );
				printf(msg);
				
				/*for (index = 0; index < 32; index++)
				{
				bots[index].is_used = FALSE;
				bots[index].respawn_state = 0;
				bots[index].kick_time = 0.0;
			}*/
				if (IS_DEDICATED_SERVER())
					bot_cfg_pause_time = gpGlobals->time + _PAUSE_TIME;
				else
					bot_cfg_pause_time = gpGlobals->time + 20.0;
			}
			/*else
			{*/
			count = 0;
			
			// mark the bots as needing to be respawned...
			for (index = 0; index < 32; index++)
			{
				if (count >= prev_num_bots)
				{
					//bots[index].is_used = FALSE;
					SBInfo[index].respawn_state = 0;
					SBInfo[index].kick_time = 0.0;
				}
				
				if (bots[index])  // is this slot used?
				{
					SBInfo[index].respawn_state = RESPAWN_NEED_TO_RESPAWN;
					count++;
				}
				
				// check for any bots that were very recently kicked...
				if ((SBInfo[index].kick_time + 5.0) > previous_time)
				{
					SBInfo[index].respawn_state = RESPAWN_NEED_TO_RESPAWN;
					count++;
				}
				else
					SBInfo[index].kick_time = 0.0;  // reset to prevent false spawns later
			}
			
			// set the respawn time
			if (IS_DEDICATED_SERVER())
				respawn_time = gpGlobals->time + _PAUSE_TIME;
			else
				respawn_time = gpGlobals->time + _PAUSE_TIME;
			//}
			
			client_update_time = gpGlobals->time + 10.0;  // start updating client data again
			
			//bot_check_time = gpGlobals->time + 30.0;
			bot_check_time = gpGlobals->time + _PAUSE_TIME*2;
		}
		
		if(!g_bForceNOStat
			&&!g_waypoint_on
			&& num_waypoints)
			WPStat.CalcSlice();
		
		if (gf_5th <= gpGlobals->time){								// this is every .2 s the case
			g_b5th=true;
			if(mod_id == CSTRIKE_DLL||mod_id == CSCLASSIC_DLL)
				f_timesrs = gpGlobals->time - f_round_start;
			//cout << gf_5th << endl;
			gf_5thd = gpGlobals->time - gf_5th + .2f;
			gf_5th = gpGlobals->time + 0.2f;
			
			if(mod_id == CSTRIKE_DLL||mod_id == CSCLASSIC_DLL){
				lGDCount --;
				if(!lGDCount){
					lGDCount = 5;			// one time per second :D
					
					SearchEs_CSTRIKE();
					CalcDistances();
				}
				
				InitGlobalRS();
			}
			else if(mod_id == DOD_DLL){
			}
			
#ifdef _DEBUG
			ShowInfo();
#endif
			
			/* begin all .2s code */
			if(bCheckWelcome||g_bMyBirthday){
				if(!bDedicatedWelcome
					&& gpGlobals->time > _PAUSE_TIME){
					bDedicatedWelcome = true;
					cout << "*************************************************************" << endl;
					cout << "*************************************************************" << endl;
					cout << "                                                           **" << endl;
					cout << "              JoeBOT is running on this Server             **" << endl;
					cout << "                                                           **" << endl;
					cout << "*************************************************************" << endl;
					cout << "*************************************************************" << endl;
					cout << "**" << endl;
					cout << "** JoeBOT "<< _JOEBOTVERSION << endl;
					cout << "** (c) Johannes Lampel alias @$3.1415rin"<<endl;
					if(g_bMyBirthday){
						cout << "** who celebrates his "<<g_lAge<<". birthday today ! <<<<<<<<<<<"<<endl;
					}
					cout << "**"<<endl;
					cout << "** Please read the readme.html carefully before asking for"<< endl;
					cout << "** support via johannes@joebot.net - thx" << endl;
					cout << "**" << endl;
					cout << "*************************************************************" << endl;
					cout << "*************************************************************" << endl;
				}
				int i;
				edict_t *pEnt;
				if(g_bMyBirthday){
					for (i = gpGlobals->maxClients; i; i--){
						if(!bWelcome[i]){
							pEnt = INDEXENT(i);
							
							// skip invalid players and skip self
							if ((pEnt) && (!pEnt->free)){
								if(IS_DEDICATED_SERVER()
									||( g_fGameCommenced > 0 && g_fGameCommenced < gpGlobals->time - 4.0)){
									// are they out of observer mode yet?
									if (!IsAlive(pEnt)){
										welcome_time[i] = gpGlobals->time + 5;  // welcome in 5 seconds
										continue;
									}
									if(UTIL_GetBotIndex(pEnt) != -1){
										bWelcome[i] = true;
										continue;
									}
									if ((welcome_time[i] > 0.0)
										&& (welcome_time[i] < gpGlobals->time))
									{		
										bWelcome[i] = true;  // clear this so we only do it once
										
										char szOut[1000];
										szOut[0] = 0;
										strcat(szOut,"\n\n\n\nToday, it's July the 8th\n\n");
										strcat(szOut,"This means that this is\n\n");
										strcat(szOut,"The birthday of the creator\n\n");
										strcat(szOut,"Of the bots you are playing\n\n");
										strcat(szOut,"With, @$3.1415rin !\n\n\n");
										
										hudtextparms_t message_params;
										
										message_params.x = -1;
										message_params.y = -1;
										message_params.effect = 2;
										message_params.r1 = 255;
										message_params.g1 = 255;
										message_params.b1 = 255;
										message_params.a1 = 1;
										message_params.r2 = 255;
										message_params.g2 = 0;
										message_params.b2 = 0;
										message_params.a2 = 1;
										message_params.fadeinTime = 0.1;
										message_params.fadeoutTime = 0.9;
										message_params.holdTime = 8;
										message_params.fxTime = 4;
										message_params.channel = 1;
										
										UTIL_ShowText(pEnt,message_params,szOut);
										
										message_params.effect = 2;
										message_params.r1 = 255;
										message_params.g1 = 255;
										message_params.b1 = 255;
										message_params.a1 = 1;
										message_params.r2 = 0;
										message_params.g2 = 255;
										message_params.b2 = 0;
										message_params.a2 = 200;
										message_params.fadeinTime = 0.05;
										message_params.fadeoutTime = 0.9;
										message_params.holdTime = 8;
										message_params.fxTime = 4;
										message_params.channel = 2;
										
										UTIL_ShowText(pEnt,message_params,szOut);
										
										message_params.effect = 1;
										message_params.r1 = 255;
										message_params.g1 = 255;
										message_params.b1 = 255;
										message_params.a1 = 1;
										message_params.r2 = 0;
										message_params.g2 = 0;
										message_params.b2 = 255;
										message_params.a2 = 0;
										message_params.fadeinTime = 4;
										message_params.fadeoutTime = 4;
										message_params.holdTime = 0;
										message_params.fxTime = 4;
										message_params.channel = 3;
										
										UTIL_ShowText(pEnt,message_params,szOut);
									}
								}
							}
						}
					}
				}
				else{
					for (i = gpGlobals->maxClients; i; i--){
						if(!bWelcome[i]){
							pEnt = INDEXENT(i);
							
							// skip invalid players and skip self
							if ((pEnt) && (!pEnt->free)){
								if(IS_DEDICATED_SERVER()
									||( g_fGameCommenced > 0 && g_fGameCommenced < gpGlobals->time - 4.0)){
									// are they out of observer mode yet?
									if (!IsAlive(pEnt)){
										welcome_time[i] = gpGlobals->time + _PAUSE_TIME;  // welcome in 5 seconds
										continue;
									}
									if(pEnt->v.flags & FL_FAKECLIENT){
										bWelcome[i] = true;
										continue;
									}
									if ((welcome_time[i] > 0.0)
										&& (welcome_time[i] < gpGlobals->time))
									{
										cout << "welcoming " << STRING(pEnt->v.netname) << endl;
										// let's send a welcome message to this client...
										//UTIL_SayText(welcome_msg, listenserver_edict);
										
										//welcome_sent = TRUE;  // clear this so we only do it once
										// let's send a welcome message to this client...
										UTIL_SayText(welcome_msg, pEnt);
										
										bWelcome[i] = true;  // clear this so we only do it once
										
										char *szText,*szOut1,*szOut2;
										char szFileName[128];
										char mapname[128];
										
										szText = new char[1000];
										szOut1 = new char[1000];
										szOut2 = new char[1000];
										
										*szOut1 = *szOut2 = '\0';
										strcat (szOut2,"\n\n\n\n");
										
										strcpy(mapname, STRING(gpGlobals->mapname));
										strcat(mapname, ".wpj");
										
										WaypointGetDir(mapname,szFileName);
										
										sprintf(szOut1,"JoeBOT %s using waypoint file:\n%s%s\n",_JOEBOTVERSIONWOOS,szFileName,mapname);
										sprintf(szTemp,"( %li waypoints / stat:%li/%li )",num_waypoints,WPStat.d.lKill,WPStat.lKillMax);
										strcat(szOut1,szTemp);
										
										strcat(szFileName,STRING(gpGlobals->mapname));
										strcat(szFileName,".txt");
										
										long lSize = CParser :: GetFileSize(szFileName);
										FILE *fhd;
										fhd = fopen ( szFileName,"r");
										if(fhd && lSize<480){
											memset(szText,0,1000*sizeof(char));
											fread(szText,lSize,sizeof(char),fhd);
											strcat(szOut2,szText);
											long lNL = CountLines(szOut2);
											for(;lNL;lNL--){
												strcat(szOut1,"\n");
											}
											//UTIL_ShowMenu(clients[welcome_index], 0x1F, 6, FALSE, szText);
											fclose(fhd);
										}
										
										if(!num_waypoints){
											strcat(szOut2,"\n\nWaypoint File couldn't be loaded");
										}
										else{
											if(!fhd){
												strcat(szOut2,"\n\nNo Description File available");
											}
										}
										if(lSize>=480){
											strcat(szOut2,"\n\nThe text in the information file is too long, sorry I can't display it :(");
										}
										
										hudtextparms_t message_params;
										
										message_params.x = -1;
										message_params.y = -1;
										message_params.effect = 1;
										message_params.r1 = 255;
										message_params.g1 = 255;
										message_params.b1 = 255;
										message_params.a1 = 1;
										message_params.r2 = 0;
										message_params.g2 = 255;
										message_params.b2 = 0;
										message_params.a2 = 1;
										message_params.fadeinTime = 4;
										message_params.fadeoutTime = 4;
										message_params.holdTime = 0;
										message_params.fxTime = .1;
										message_params.channel = 2;
										
										UTIL_ShowText(pEnt,message_params,szOut1);
										
										message_params.x = -1;
										message_params.y = -1;
										message_params.effect = 2;
										message_params.r1 = 255;
										message_params.g1 = 255;
										message_params.b1 = 255;
										message_params.a1 = 1;
										message_params.r2 = !(UTIL_GetTeam(pEnt))*255;
										message_params.g2 = 0;
										message_params.b2 = (UTIL_GetTeam(pEnt))*255;
										message_params.a2 = 1;
										message_params.fadeinTime = 0.02;
										message_params.fadeoutTime = 0.6;
										message_params.holdTime = 4;
										message_params.fxTime = 3;
										message_params.channel = 3;
										
										UTIL_ShowText(pEnt,message_params,szOut2);
										delete [] szOut1,szOut2,szText;
									}
								}
							}
						}
					}
				}
			}
			
			if (client_update_time <= gpGlobals->time)
			{
				client_update_time = gpGlobals->time + 1.0;
				
				for (i=0; i < 32; i++)
				{
					if (bots[i])
					{
						memset(&cd, 0, sizeof(cd));
						
						UpdateClientData( bots[i]->pEdict, 1, &cd );
						
						// see if a weapon was dropped...
						if (bots[i]->bot_weapons != cd.weapons)
						{
							bots[i]->bot_weapons = cd.weapons;
						}
					}
				}
			}
			/* end all .2s code*/
		}
		else{
			g_b5th=false;
		}
		
		CWG.Think();		// let the gnomes be alive :D
		
		count = 0;
		
		// TheFatal - START from Advanced Bot Framework (Thanks Rich!)
		
		// adjust the millisecond delay based on the frame rate interval...
		if (g_msecdel <= gpGlobals->time)
		{
			g_msecdel = gpGlobals->time + 0.5;
			if (g_msecnum > 0)
				g_msecval = 450.0/g_msecnum;
			g_msecnum = 0;
		}
		else
			g_msecnum++;
		
		if (g_msecval < 5)    // don't allow msec to be less than 5...
			g_msecval = 5;
		
		if (g_msecval > 100)  // ...or greater than 100
			g_msecval = 100;
		
		// TheFatal - END
		
		for (bot_index = 0; bot_index < gpGlobals->maxClients; bot_index++)
		{
			if ((bots[bot_index]) &&  // is this slot used AND
				(SBInfo[bot_index].respawn_state == RESPAWN_IDLE))  // not respawning
			{
				//try{
				if (g_bJoinWHumanRES &&
					bots[bot_index]->bot_team != 6 &&
					bots[bot_index]->bot_team > 0 &&
					!UTIL_HumansInGame())
				{
					sprintf(szTemp, "kick \"%s\"\n", STRING(bots[bot_index]->pEdict->v.netname));
					SERVER_COMMAND(szTemp);
				}
				else
					bots[bot_index]->Think();
				
				count++;
				/*}
				catch(...){
				FILE *fhd = fopen("scheisse.txt","a");fprintf(fhd,"scheisse in think\n");fclose(fhd);
			}*/
			}
		}
		
		if (count > num_bots)
			num_bots = count;
		
		for (player_index = 1; player_index <= gpGlobals->maxClients; player_index++)
		{
			pPlayer = INDEXENT(player_index);
			
			if (pPlayer && !pPlayer->free)
			{
				if ((g_waypoint_on) && FBitSet(pPlayer->v.flags, FL_CLIENT))
				{
					WaypointThink(pPlayer);
				}
				
				/*if ((mod_id == FRONTLINE_DLL) && (flf_bug_check == 0))
				{
				edict_t *pent = NULL;
				int fix_flag = 0;
				
				 flf_bug_check = 1;
				 
				  while ((pent = UTIL_FindEntityByClassname( pent, "capture_point" )) != NULL)
				  {
				  if (pent->v.skin != 0)  // not blue skin?
				  {
				  flf_bug_fix = 1;  // need to use bug fix code
				  }
				  }
            }*/
			}
		}
		
		if (g_GameRules)
		{
			if (need_to_open_cfg)  // have we open bot.cfg file yet?
			{
				char filename[256];
				char mapname[64];
				
				need_to_open_cfg = FALSE;  // only do this once!!!
				
				// check if mapname_bot.cfg file exists...
				
				//strcpy(mapname, STRING(gpGlobals->mapname));
				strcpy(mapname, "bot.cfg");
				UTIL_BuildFileName(filename,"joebot","bot.cfg");
				
				//UTIL_BuildFileName(filename, "maps", mapname);
				
				if ((bot_cfg_fp = fopen(filename, "r")) != NULL)
				{
					sprintf(msg, "JoeBOT: Executing %s\n", filename);
					ALERT( at_console, msg );
					printf(msg);
				}
				else
				{
					UTIL_BuildFileName(filename, "bot.cfg", NULL);
					
					sprintf(msg, "JoeBOT: Executing %s\n", filename);
					ALERT( at_console, msg );
					
					bot_cfg_fp = fopen(filename, "r");
					
					if (bot_cfg_fp == NULL)
						ALERT( at_console, "bot.cfg file not found\n" );
				}
				
				if (IS_DEDICATED_SERVER())
					bot_cfg_pause_time = gpGlobals->time + 2.0;
				else
					bot_cfg_pause_time = gpGlobals->time + 20.0;
			}
			
			if (!IS_DEDICATED_SERVER() && !spawn_time_reset)
			{
				if (listenserver_edict != NULL)
				{
					if (IsAlive(listenserver_edict))
					{
						spawn_time_reset = TRUE;
						
						if (respawn_time >= 1.0)
							respawn_time = min(respawn_time, gpGlobals->time + (float)1.0);
						
						if (bot_cfg_pause_time >= 1.0)
							bot_cfg_pause_time = min(bot_cfg_pause_time, gpGlobals->time + (float)1.0);
					}
				}
			}
			
			if ((bot_cfg_fp) &&
				(bot_cfg_pause_time >= 1.0) && (bot_cfg_pause_time <= gpGlobals->time))
			{
				// process bot.cfg file options...
				ProcessBotCfgFile();
			}
			
		}    
		
		// are we currently respawning bots and is it time to spawn one yet?
		if(g_b5th){
			if (/*(respawn_time > 1.0) && */(respawn_time <= gpGlobals->time))
			{
				int index = 0;
				
				// find bot needing to be respawned...
				while ((index < 32) &&
					(SBInfo[index].respawn_state != RESPAWN_NEED_TO_RESPAWN))
					index++;
				
				if (index < 32)
				{
					if(!g_bJoinWHumanRES || (g_bJoinWHumanRES && UTIL_HumansInGame() != 0)){
						SBInfo[index].respawn_state = RESPAWN_IS_RESPAWNING;
						if(bots[index]) delete bots[index];      // free up this slot
						bots[index] = 0;
						
						// respawn 1 bot then wait a while (otherwise engine crashes)
						if ((mod_id == VALVE_DLL) ||
							((mod_id == GEARBOX_DLL) && (pent_info_ctfdetect == NULL)))
						{
							char c_skill[2],c_team[2];
							
							sprintf(c_skill, "%d", SBInfo[index].bot_skill);
							sprintf(c_team, "%d", SBInfo[index].bot_team);
							
							BotCreate(NULL, c_team,SBInfo[index].skin, SBInfo[index].name, c_skill);
						}
						else
						{
							char c_skill[5];
							char c_team[5];
							char c_class[5];
							
							//cout << " ------------------- respawning after map change - wanting to respawn" << endl;
							
							sprintf(c_skill, "%i", SBInfo[index].bot_skill);
							sprintf(c_team, "%i", SBInfo[index].bot_team);
							sprintf(c_class, "%i", SBInfo[index].bot_class);
							
							if ((mod_id == TFC_DLL) || (mod_id == GEARBOX_DLL))
								BotCreate(NULL, NULL, NULL, SBInfo[index].name, c_skill);
							else
								BotCreate(NULL, c_team, c_class, SBInfo[index].name, c_skill);
						}
						
						respawn_time = gpGlobals->time + .5;  // set next respawn time
						
						bot_check_time = gpGlobals->time + _PAUSE_TIME - .1;
					}
					else
						respawn_time = gpGlobals->time + .5;
				}
				else
				{
					respawn_time = 0.0;
				}
				
				// check if time to see if a bot needs to be created...
				if (bot_check_time < gpGlobals->time)
				{
					int count = 0;
					//int counthumans = UTIL_HumansInGame();
					
					bot_check_time = gpGlobals->time + 1.5;		//hackhack
					
					for (i = 0; i < 32; i++)
					{
						if (clients[i] != NULL)
							count++;
					}
					
					// if there are currently less than the maximum number of "players"
					// then add another bot using the default skill level...
					if ((count < max_bots) && (max_bots != -1))
					{
						//cout << " ------------------- creating bot due to max_bots" << endl;
						if(!g_bJoinWHumanMAX){
							BotCreate( NULL, NULL, NULL, NULL, NULL);
						}
						else{
							if ( UTIL_HumansInGame() != 0){
								BotCreate( NULL, NULL, NULL, NULL, NULL);
							}
						}
					}
					// if there are currently more than the minimum number of bots running
					// then kick one of the bots off the server...
					/*if ((count > min_bots) && (min_bots != -1))
					{
						for (i=0; i < 32; i++){
							if (bots[i]){  // is this slot used?
								sprintf(szTemp, "kick \"%s\"\n", STRING(bots[i]->pEdict->v.netname));
								
								SERVER_COMMAND(szTemp);  // kick the bot using (kick "name")
								
								break;
							}
						}
					}*/
				}
				
			}  
			
			// if time to check for server commands then do so...
			if ((check_server_cmd <= gpGlobals->time) && IS_DEDICATED_SERVER())
			{
				check_server_cmd = gpGlobals->time + 1.0;
				
				char *cvar_bot = (char *)CVAR_GET_STRING( "joebot" );
				
				if ( cvar_bot && cvar_bot[0] )
				{
					char cmd_line[80];
					char *cmd, *arg1, *arg2, *arg3, *arg4, *arg5, *arg6;
					
					strcpy(cmd_line, cvar_bot);
					
					index = 0;
					cmd = cmd_line;
					//cout << cmd << endl;
					arg1 = arg2 = arg3 = arg4 = arg5 = arg6 = NULL;
					
					// skip to blank or end of string...
					while ((cmd_line[index] != '|') && (cmd_line[index] != 0))
						index++;
					
					if (cmd_line[index] == '|')
					{
						cmd_line[index++] = 0;
						arg1 = &cmd_line[index];
						
						// skip to blank or end of string...
						while ((cmd_line[index] != '|') && (cmd_line[index] != 0))
							index++;
						
						if (cmd_line[index] == '|')
						{
							cmd_line[index++] = 0;
							arg2 = &cmd_line[index];
							
							// skip to blank or end of string...
							while ((cmd_line[index] != '|') && (cmd_line[index] != 0))
								index++;
							
							if (cmd_line[index] == '|')
							{
								cmd_line[index++] = 0;
								arg3 = &cmd_line[index];
								
								// skip to blank or end of string...
								while ((cmd_line[index] != '|') && (cmd_line[index] != 0))
									index++;
								
								if (cmd_line[index] == '|')
								{
									cmd_line[index++] = 0;
									arg4 = &cmd_line[index];
									
									// skip to blank or end of string...
									while ((cmd_line[index] != '|') && (cmd_line[index] != 0))
										index++;
									
									if (cmd_line[index] == '|')
									{
										cmd_line[index++] = 0;
										arg5 = &cmd_line[index];
										
										// skip to blank or end of string...
										while ((cmd_line[index] != '|') && (cmd_line[index] != 0))
											index++;
										
										if (cmd_line[index] == '|')
										{
											cmd_line[index++] = 0;
											arg6 = &cmd_line[index];
										}
									}
								}
							}
						}
					}
					Commands.Exec(0,CM_DEDICATED,cmd,arg1,arg2,arg3,arg4);
					
					CVAR_SET_STRING("joebot", "");
				}
			}
			
			/*// check if time to see if a bot needs to be created...
			if (bot_check_time < gpGlobals->time)
			{
			int count = 0;
			
			 bot_check_time = gpGlobals->time + 1.5;		//hackhack
			 
			  for (i = 0; i < 32; i++)
			  {
			  if (clients[i] != NULL)
			  count++;
			  }
			  
			   // if there are currently less than the maximum number of "players"
			   // then add another bot using the default skill level...
			   if ((count < max_bots) && (max_bots != -1))
			   {
			   cout << " ------------------- creating bot due to max_bots" << endl;
			   BotCreate( NULL, NULL, NULL, NULL, NULL);
			   }
      }*/
	  }
	  
      previous_time = gpGlobals->time;
   }
   /* }
   catch(...){
   FILE *fhd = fopen("scheisse.txt","a");fprintf(fhd,"scheisse in startframe\n");fclose(fhd);
   }*/
   
   (*other_gFunctionTable.pfnStartFrame)();
}

void ParmsNewLevel( void )
{
	(*other_gFunctionTable.pfnParmsNewLevel)();
}

void ParmsChangeLevel( void )
{
	(*other_gFunctionTable.pfnParmsChangeLevel)();
}

const char *GetGameDescription( void )
{
	return (*other_gFunctionTable.pfnGetGameDescription)();
}

void PlayerCustomization( edict_t *pEntity, customization_t *pCust )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt", "a"); fprintf(fp, "PlayerCustomization: %x\n",pEntity); fclose(fp); }
#endif
	
	(*other_gFunctionTable.pfnPlayerCustomization)(pEntity, pCust);
}

void SpectatorConnect( edict_t *pEntity )
{
	(*other_gFunctionTable.pfnSpectatorConnect)(pEntity);
}

void SpectatorDisconnect( edict_t *pEntity )
{
	(*other_gFunctionTable.pfnSpectatorDisconnect)(pEntity);
}

void SpectatorThink( edict_t *pEntity )
{
	(*other_gFunctionTable.pfnSpectatorThink)(pEntity);
}

void Sys_Error( const char *error_string )
{
	(*other_gFunctionTable.pfnSys_Error)(error_string);
}

void PM_Move ( struct playermove_s *ppmove, int server )
{
	(*other_gFunctionTable.pfnPM_Move)(ppmove, server);
}

void PM_Init ( struct playermove_s *ppmove )
{
	(*other_gFunctionTable.pfnPM_Init)(ppmove);
}

char PM_FindTextureType( char *name )
{
	return (*other_gFunctionTable.pfnPM_FindTextureType)(name);
}

void SetupVisibility( edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas )
{
	(*other_gFunctionTable.pfnSetupVisibility)(pViewEntity, pClient, pvs, pas);
}

void UpdateClientData ( const struct edict_s *ent, int sendweapons, struct clientdata_s *cd )
{
	(*other_gFunctionTable.pfnUpdateClientData)(ent, sendweapons, cd);
}

int AddToFullPack( struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet )
{
	return (*other_gFunctionTable.pfnAddToFullPack)(state, e, ent, host, hostflags, player, pSet);
}

void CreateBaseline( int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs )
{
	(*other_gFunctionTable.pfnCreateBaseline)(player, eindex, baseline, entity, playermodelindex, player_mins, player_maxs);
}

void RegisterEncoders( void )
{
	(*other_gFunctionTable.pfnRegisterEncoders)();
}

int GetWeaponData( struct edict_s *player, struct weapon_data_s *info )
{
	return (*other_gFunctionTable.pfnGetWeaponData)(player, info);
}

void CmdStart( const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed )
{
	(*other_gFunctionTable.pfnCmdStart)(player, cmd, random_seed);
}

void CmdEnd ( const edict_t *player )
{
	(*other_gFunctionTable.pfnCmdEnd)(player);
}

int ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size )
{
	return (*other_gFunctionTable.pfnConnectionlessPacket)(net_from, args, response_buffer, response_buffer_size);
}

int GetHullBounds( int hullnumber, float *mins, float *maxs )
{
	return (*other_gFunctionTable.pfnGetHullBounds)(hullnumber, mins, maxs);
}

void CreateInstancedBaselines( void )
{
	(*other_gFunctionTable.pfnCreateInstancedBaselines)();
}

int InconsistentFile( const edict_t *player, const char *filename, char *disconnect_message )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt", "a"); fprintf(fp, "InconsistentFile: %x filename=%s\n",player,filename); fclose(fp); }
#endif
	
	return (*other_gFunctionTable.pfnInconsistentFile)(player, filename, disconnect_message);
}

int AllowLagCompensation( void )
{
	return (*other_gFunctionTable.pfnAllowLagCompensation)();
}


DLL_FUNCTIONS gFunctionTable =
{
	GameDLLInit,               //pfnGameInit
		DispatchSpawn,             //pfnSpawn
		DispatchThink,             //pfnThink
		DispatchUse,               //pfnUse
		DispatchTouch,             //pfnTouch
		DispatchBlocked,           //pfnBlocked
		DispatchKeyValue,          //pfnKeyValue
		DispatchSave,              //pfnSave
		DispatchRestore,           //pfnRestore
		DispatchObjectCollsionBox, //pfnAbsBox
		
		SaveWriteFields,           //pfnSaveWriteFields
		SaveReadFields,            //pfnSaveReadFields
		
		SaveGlobalState,           //pfnSaveGlobalState
		RestoreGlobalState,        //pfnRestoreGlobalState
		ResetGlobalState,          //pfnResetGlobalState
		
		ClientConnect,             //pfnClientConnect
		ClientDisconnect,          //pfnClientDisconnect
		ClientKill,                //pfnClientKill
		ClientPutInServer,         //pfnClientPutInServer
		ClientCommand,             //pfnClientCommand
		ClientUserInfoChanged,     //pfnClientUserInfoChanged
		ServerActivate,            //pfnServerActivate
		ServerDeactivate,          //pfnServerDeactivate
		
		PlayerPreThink,            //pfnPlayerPreThink
		PlayerPostThink,           //pfnPlayerPostThink
		
		StartFrame,                //pfnStartFrame
		ParmsNewLevel,             //pfnParmsNewLevel
		ParmsChangeLevel,          //pfnParmsChangeLevel
		
		GetGameDescription,        //pfnGetGameDescription    Returns string describing current .dll game.
		PlayerCustomization,       //pfnPlayerCustomization   Notifies .dll of new customization for player.
		
		SpectatorConnect,          //pfnSpectatorConnect      Called when spectator joins server
		SpectatorDisconnect,       //pfnSpectatorDisconnect   Called when spectator leaves the server
		SpectatorThink,            //pfnSpectatorThink        Called when spectator sends a command packet (usercmd_t)
		
		Sys_Error,                 //pfnSys_Error          Called when engine has encountered an error
		
		PM_Move,                   //pfnPM_Move
		PM_Init,                   //pfnPM_Init            Server version of player movement initialization
		PM_FindTextureType,        //pfnPM_FindTextureType
		
		SetupVisibility,           //pfnSetupVisibility        Set up PVS and PAS for networking for this client
		UpdateClientData,          //pfnUpdateClientData       Set up data sent only to specific client
		AddToFullPack,             //pfnAddToFullPack
		CreateBaseline,            //pfnCreateBaseline        Tweak entity baseline for network encoding, allows setup of player baselines, too.
		RegisterEncoders,          //pfnRegisterEncoders      Callbacks for network encoding
		GetWeaponData,             //pfnGetWeaponData
		CmdStart,                  //pfnCmdStart
		CmdEnd,                    //pfnCmdEnd
		ConnectionlessPacket,      //pfnConnectionlessPacket
		GetHullBounds,             //pfnGetHullBounds
		CreateInstancedBaselines,  //pfnCreateInstancedBaselines
		InconsistentFile,          //pfnInconsistentFile
		AllowLagCompensation,      //pfnAllowLagCompensation
};

#ifdef __BORLANDC__
int EXPORT GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion )
#else
extern "C" EXPORT int GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion )
#endif
{
	// check if engine's pointer is valid and version is correct...
	
	if ( !pFunctionTable || interfaceVersion != INTERFACE_VERSION )
		return FALSE;
	
	// pass engine callback function table to engine...
	memcpy( pFunctionTable, &gFunctionTable, sizeof( DLL_FUNCTIONS ) );
	
	// pass other DLLs engine callbacks to function table...
	if (!(*other_GetEntityAPI)(&other_gFunctionTable, INTERFACE_VERSION))
	{
		return FALSE;  // error initializing function table!!!
	}
	
	return TRUE;
}


#ifdef __BORLANDC__
int EXPORT GetNewDLLFunctions( NEW_DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion )
#else
extern "C" EXPORT int GetNewDLLFunctions( NEW_DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion )
#endif
{
	if (other_GetNewDLLFunctions == NULL)
		return FALSE;
	
	// pass other DLLs engine callbacks to function table...
	if (!(*other_GetNewDLLFunctions)(pFunctionTable, interfaceVersion))
	{
		return FALSE;  // error initializing function table!!!
	}
	
	return TRUE;
}


void FakeClientCommand(edict_t *pBot, char *arg1, char *arg2, char *arg3)
{
	int length;
	
	memset(g_argv, 0, sizeof(g_argv));
	
	isFakeClientCommand = 1;
	
	if ((arg1 == NULL) || (*arg1 == 0))
		return;
	
	if ((arg2 == NULL) || (*arg2 == 0))
	{
		length = sprintf(&g_argv[0], "%s", arg1);
		fake_arg_count = 1;
	}
	else if ((arg3 == NULL) || (*arg3 == 0))
	{
		length = sprintf(&g_argv[0], "%s %s", arg1, arg2);
		fake_arg_count = 2;
	}
	else
	{
		length = sprintf(&g_argv[0], "%s %s %s", arg1, arg2, arg3);
		fake_arg_count = 3;
	}
	
	g_argv[length] = 0;  // null terminate just in case
	
	strcpy(&g_argv[64], arg1);
	
	if (arg2)
		strcpy(&g_argv[128], arg2);
	
	if (arg3)
		strcpy(&g_argv[192], arg3);
	
	// allow the MOD DLL to execute the ClientCommand...
	ClientCommand(pBot);
	
	isFakeClientCommand = 0;
}


const char *Cmd_Args( void )
{
	if (isFakeClientCommand)
	{
		return &g_argv[0];
	}
	else
	{
		return (*g_engfuncs.pfnCmd_Args)();
	}
}


const char *Cmd_Argv( int argc )
{
	if (isFakeClientCommand)
	{
		if (argc == 0)
		{
			return &g_argv[64];
		}
		else if (argc == 1)
		{
			return &g_argv[128];
		}
		else if (argc == 2)
		{
			return &g_argv[192];
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return (*g_engfuncs.pfnCmd_Argv)(argc);
	}
}


int Cmd_Argc( void )
{
	if (isFakeClientCommand)
	{
		return fake_arg_count;
	}
	else
	{
		return (*g_engfuncs.pfnCmd_Argc)();
	}
}


void ProcessBotCfgFile(void)
{
	int iLPF;
	int ch;
	char cmd_line[256];
	int cmd_index;
	static char server_cmd[256];
	char *cmd, *arg1, *arg2, *arg3, *arg4,*arg5, *arg6;
	//	char msg[80];
	
	if (bot_cfg_pause_time > gpGlobals->time)
		return;
	
	for(iLPF = 0;iLPF < _MAXCFGLINESPERFRAME;iLPF++){
		if (bot_cfg_fp == NULL)
			return;
		
		cmd_index = 0;
		cmd_line[cmd_index] = 0;
		
		ch = fgetc(bot_cfg_fp);
		
		// skip any leading blanks
		while (ch == ' ')
			ch = fgetc(bot_cfg_fp);
		
		while ((ch != EOF) && (ch != '\r') && (ch != '\n'))
		{
			if (ch == '\t')  // convert tabs to spaces
				ch = ' ';
			
			cmd_line[cmd_index] = ch;
			
			ch = fgetc(bot_cfg_fp);
			
			// skip multiple spaces in input file
			while ((cmd_line[cmd_index] == ' ') &&
				(ch == ' '))      
				ch = fgetc(bot_cfg_fp);
			
			cmd_index++;
		}
		
		if (ch == '\r')  // is it a carriage return?
		{
			ch = fgetc(bot_cfg_fp);  // skip the linefeed
		}
		
		// if reached end of file, then close it
		if (ch == EOF)
		{
			fclose(bot_cfg_fp);
			
			bot_cfg_fp = NULL;
			
			bot_cfg_pause_time = 0.0;
		}
		
		cmd_line[cmd_index] = 0;  // terminate the command line
		
		// copy the command line to a server command buffer...
		strcpy(server_cmd, cmd_line);
		strcat(server_cmd, "\n");
		
		cmd_index = 0;
		cmd = cmd_line;
		arg1 = arg2 = arg3 = arg4 = arg5 = arg6 = NULL;
		
		// skip to blank or end of string...
		while ((cmd_line[cmd_index] != ' ') && (cmd_line[cmd_index] != 0))
			cmd_index++;
		
		if (cmd_line[cmd_index] == ' ')
		{
			cmd_line[cmd_index++] = 0;
			arg1 = &cmd_line[cmd_index];
			
			// skip to blank or end of string...
			while ((cmd_line[cmd_index] != ' ') && (cmd_line[cmd_index] != 0))
				cmd_index++;
			
			if (cmd_line[cmd_index] == ' ')
			{
				cmd_line[cmd_index++] = 0;
				arg2 = &cmd_line[cmd_index];
				
				// skip to blank or end of string...
				while ((cmd_line[cmd_index] != ' ') && (cmd_line[cmd_index] != 0))
					cmd_index++;
				
				if (cmd_line[cmd_index] == ' ')
				{
					cmd_line[cmd_index++] = 0;
					arg3 = &cmd_line[cmd_index];
					
					// skip to blank or end of string...
					while ((cmd_line[cmd_index] != ' ') && (cmd_line[cmd_index] != 0))
						cmd_index++;
					
					if (cmd_line[cmd_index] == ' ')
					{
						cmd_line[cmd_index++] = 0;
						arg4 = &cmd_line[cmd_index];
						
						// skip to blank or end of string...
						while ((cmd_line[cmd_index] != ' ') && (cmd_line[cmd_index] != 0))
							cmd_index++;
						
						if (cmd_line[cmd_index] == ' ')
						{
							cmd_line[cmd_index++] = 0;
							arg5 = &cmd_line[cmd_index];
							
							// skip to blank or end of string...
							while ((cmd_line[cmd_index] != ' ') && (cmd_line[cmd_index] != 0))
								cmd_index++;
							
							if (cmd_line[cmd_index] == ' ')
							{
								cmd_line[cmd_index++] = 0;
								arg6 = &cmd_line[cmd_index];
							}
						}
					}
				}
			}
		}
		
		//printf("%s-%s-%s-%s-%s\n",cmd,arg1,arg2,arg3,arg4);
		if ((cmd_line[0] == '#') || (cmd_line[0] == 0))
			continue;  // return if comment or blank line
		
		if(Commands.Exec(0,CM_SCRIPT,cmd,arg1,arg2,arg3,arg4))
			continue;
		
		//sprintf(msg, "JoeBOT : executing server command: %s - is the syntax right ?\n", server_cmd);
		//ALERT( at_console, msg );
		
		/*if (IS_DEDICATED_SERVER())
		printf(msg);*/
		
		SERVER_COMMAND(server_cmd);
	}
}

