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

#include <iostream.h>

#include "extdll.h"
#include "util.h"
#include "cvardef.h"

#include "Commandfunc.h"

#include "bot_func.h"
#include "CBotBase.h"
#include "globalvars.h"
#include "WorldGnome.h"

#include "NeuralNet.h"
#include "som.h"

int g_menu_waypoint;
int g_menu_state = 0;

char *show_menu_1;			// pointer to texts
char *show_menu_2;
char *show_menu_2a;
char *show_menu_2am;
char *show_menu_2b;
char *show_menu_2c;
char *show_menu_3;
char *show_menu_4;
char *show_menu_5;

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
9. Pathwaypoint\n"};

char *E_show_menu_2a={"\\yWaypoint flags\n\n\
\\w1. Dont Avoid\n\
2. Ladder\n\
3. Visit\n\
4. Crouch\n\
5. Aiming\n\
6. Sniper spot\n\
7. Bomb target / hostages\n\
8. Team specific\n\
9. more ...\n"};

char *E_show_menu_2am={"\\yWaypoint flags - 2\n\n\
\\w1. Rescue zone\n\
2. Dont avoid fall\n\
3. Jump\n"};

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
8. full\n"};

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
9. Pfade\n"};

char *D_show_menu_2a={"\\yWegpunkt Eigenschaften\n\n\
\\w1. Vermeide nichts\n\
2. Leiter\n\
3. Besuchspunkt\n\
4. Ducken\n\
5. Zielen\n\
6. Sniper Spot\n\
7. Bombe / Geiseln\n\
8. Team spezifisch\n\
9. mehr ...\n"};

char *D_show_menu_2am={"\\yWaypoint Eigenschaften - 2\n\n\
\\w1. Rettungs-Zone\n\
2. Verhindere nicht zu fallen\n\
3. Spring!\n"};

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
8. voll\n"};

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

// // // /// // /// // /// /// // /// /// french joebot menu
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
9. Pathwaypoint\n"};

char *F_show_menu_2a={"\\yAttributs des waypoints\n\n\
\\w1. Ignorer obstacle\n\
2. Echelle\n\
3. Visite\n\
4. Accroupi\n\
5. Viser\n\
6. Point de Snipe\n\
7. Point de bombe / hostages\n\
8. Specifique a une equipe\n\
9. Plus ...\n"};

char *F_show_menu_2am={"\\yAttributs des waypoints - 2\n\n\
\\w1. Zone de sauvetage\n\
2. Empecher de tomber\n\
3. Sauter\n"};

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
8. tous\n"};

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

cvar_t *jb_cstrike15;
cvar_t *jb_mixnames;
cvar_t *jb_msgwelcome;
cvar_t *jb_prefixaggression;
cvar_t *jb_suffixskill;
cvar_t *jb_botsmin;
cvar_t *jb_botsmax;
cvar_t *jb_chat1337;
cvar_t *jb_chat;
cvar_t *jb_chati;
cvar_t *jb_chatfreq;
cvar_t *jb_msgradio;
cvar_t *jb_aimmomentum;
cvar_t *jb_aimspeed;
cvar_t *jb_nnupdaterate;
cvar_t *jb_campprobability;
cvar_t *jb_msgenemydown;
cvar_t *jb_shoot;
cvar_t *jb_tkpunish;
cvar_t *jb_language;
cvar_t *jb_wp;
cvar_t *jb_wpoffsetx;
cvar_t *jb_wpoffsety;
cvar_t *jb_wpoffsetz;
cvar_t *jb_wpfilename;
cvar_t *jb_wpsound;
cvar_t *jb_wppath;
cvar_t *jb_wpauto;
cvar_t *jb_wpautopath;
cvar_t *jb_wpautobots;
cvar_t *jb_wpautojump;
cvar_t *jb_wpautojumptest;
cvar_t *jb_wpautoadvanced;
cvar_t *jb_wpstats;
cvar_t *jb_wprecalc;
cvar_t *jb_prefixdefensive;
cvar_t *jb_prefixnormal;
cvar_t *jb_prefixaggressive;
cvar_t *jb_pistolonly;
cvar_t *jb_skillmin;
cvar_t *jb_skillmax;
cvar_t *jb_entergame;
cvar_t *jb_jointeam;
cvar_t *jb_spraypaint;
cvar_t *jb_showen;
cvar_t *jb_debugengine;

cvar_t init_jb_cstrike15        = {"jb_cstrike15", "0", 0, 0};
cvar_t init_jb_mixnames         = {"jb_mixnames", "1", 0, 1};
cvar_t init_jb_msgwelcome       = {"jb_msgwelcome", "1", 0, 1};
cvar_t init_jb_prefixaggression = {"jb_prefixaggression", "1", 0, 1};
cvar_t init_jb_suffixskill      = {"jb_suffixskill", "1", 0, 1};
cvar_t init_jb_botsmin          = {"jb_botsmin", "-1", 0, -1};
cvar_t init_jb_botsmax          = {"jb_botsmax", "-1", 0, -1};
cvar_t init_jb_chat1337         = {"jb_chat1337", "3", 0, 3};
cvar_t init_jb_chat             = {"jb_chat", "1", 0, 1};
cvar_t init_jb_chati            = {"jb_chati", "", 0, IC_ALL};
cvar_t init_jb_chatfreq         = {"jb_chatfreq", "45", 0, 45};
cvar_t init_jb_msgradio         = {"jb_msgradio", "1", 0, 1};
cvar_t init_jb_aimmomentum      = {"jb_aimmomentum", "", 0, _DEFAULTAMOMENTUM};
cvar_t init_jb_aimspeed         = {"jb_aimspeed", "", 0, _DEFAULTASPEED};
cvar_t init_jb_nnupdaterate     = {"jb_nnupdaterate", "", 0, _DEFAULTNNUPDATE};
cvar_t init_jb_campprobability  = {"jb_campprobability", "2", 0, 2};
cvar_t init_jb_msgenemydown     = {"jb_msgenemydown", "1", 0, 1};
cvar_t init_jb_shoot            = {"jb_shoot", "1", 0, 1};
cvar_t init_jb_tkpunish         = {"jb_tkpunish", "0", 0, 0};
cvar_t init_jb_language         = {"jb_language", "e", 0};
cvar_t init_jb_wp               = {"jb_wp", "0", 0, 0};
cvar_t init_jb_wpoffsetx        = {"jb_wpoffsetx", "0", 0, 0};
cvar_t init_jb_wpoffsety        = {"jb_wpoffsety", "0", 0, 0};
cvar_t init_jb_wpoffsetz        = {"jb_wpoffsetz", "0", 0, 0};
cvar_t init_jb_wpfilename       = {"jb_wpfilename", "", 0, 0};
cvar_t init_jb_wpsound          = {"jb_wpsound", "1", 0, 1};
cvar_t init_jb_wppath           = {"jb_wppath", "0", 0, 0};
cvar_t init_jb_wpauto           = {"jb_wpauto", "0", 0, 0};
cvar_t init_jb_wpautopath       = {"jb_wpautopath", "1", 0, 1};
cvar_t init_jb_wpautobots       = {"jb_wpautobots", "0", 0, 0};
cvar_t init_jb_wpautojump       = {"jb_wpautojump", "1", 0, 1};
cvar_t init_jb_wpautojumptest   = {"jb_wpautojumptest", "1", 0, 1};
cvar_t init_jb_wpautoadvanced   = {"jb_wpautoadvanced", "1", 0, 1};
cvar_t init_jb_wpstats          = {"jb_wpstats", "1", 0, 1};
cvar_t init_jb_wprecalc         = {"jb_wprecalc", "1", 0, 1};
cvar_t init_jb_prefixdefensive  = {"jb_prefixdefensive", "[J0E]", 0, 0};
cvar_t init_jb_prefixnormal     = {"jb_prefixnormal", "[JoE]", 0, 0};
cvar_t init_jb_prefixaggressive = {"jb_prefixaggressive", "[JOE]", 0, 0};
cvar_t init_jb_pistolonly       = {"jb_pistolonly", "0", 0, 0};
cvar_t init_jb_skillmin         = {"jb_skillmin", "", 0, _DEFAULTSKILL - 30};
cvar_t init_jb_skillmax         = {"jb_skillmax", "", 0, _DEFAULTSKILL};
cvar_t init_jb_entergame        = {"jb_entergame", "1", 0, 1};
cvar_t init_jb_jointeam         = {"jb_jointeam", "1", 0, 1};
cvar_t init_jb_spraypaint       = {"jb_spraypaint", "1", 0, 1};
cvar_t init_jb_showen           = {"jb_showen", "0", 0, 0};
cvar_t init_jb_debugengine      = {"jb_debugengine", "0", 0, 0};

void JBRegCvars(void)
{
	static bool bRegDone = false;

	if (bRegDone) return;

	CVAR_REGISTER(&init_jb_cstrike15);
	CVAR_REGISTER(&init_jb_mixnames);
	CVAR_REGISTER(&init_jb_msgwelcome);
	CVAR_REGISTER(&init_jb_prefixaggression);
	CVAR_REGISTER(&init_jb_suffixskill);
	CVAR_REGISTER(&init_jb_botsmin);
	CVAR_REGISTER(&init_jb_botsmax);
	CVAR_REGISTER(&init_jb_chat1337);
	CVAR_REGISTER(&init_jb_chat);
	CVAR_REGISTER(&init_jb_chati);
	CVAR_REGISTER(&init_jb_chatfreq);
	CVAR_REGISTER(&init_jb_msgradio);
	CVAR_REGISTER(&init_jb_aimmomentum);
	CVAR_REGISTER(&init_jb_aimspeed);
	CVAR_REGISTER(&init_jb_nnupdaterate);
	CVAR_REGISTER(&init_jb_campprobability);
	CVAR_REGISTER(&init_jb_msgenemydown);
	CVAR_REGISTER(&init_jb_shoot);
	CVAR_REGISTER(&init_jb_tkpunish);
	CVAR_REGISTER(&init_jb_language);
	CVAR_REGISTER(&init_jb_wp);
	CVAR_REGISTER(&init_jb_wpoffsetx);
	CVAR_REGISTER(&init_jb_wpoffsety);
	CVAR_REGISTER(&init_jb_wpoffsetz);
	CVAR_REGISTER(&init_jb_wpfilename);
	CVAR_REGISTER(&init_jb_wpsound);
	CVAR_REGISTER(&init_jb_wppath);
	CVAR_REGISTER(&init_jb_wpauto);
	CVAR_REGISTER(&init_jb_wpautopath);
	CVAR_REGISTER(&init_jb_wpautobots);
	CVAR_REGISTER(&init_jb_wpautojump);
	CVAR_REGISTER(&init_jb_wpautojumptest);
	CVAR_REGISTER(&init_jb_wpautoadvanced);
	CVAR_REGISTER(&init_jb_wpstats);
	CVAR_REGISTER(&init_jb_wprecalc);
	CVAR_REGISTER(&init_jb_prefixdefensive);
	CVAR_REGISTER(&init_jb_prefixnormal);
	CVAR_REGISTER(&init_jb_prefixaggressive);
	CVAR_REGISTER(&init_jb_pistolonly);
	CVAR_REGISTER(&init_jb_skillmin);
	CVAR_REGISTER(&init_jb_skillmax);
	CVAR_REGISTER(&init_jb_entergame);
	CVAR_REGISTER(&init_jb_jointeam);
	CVAR_REGISTER(&init_jb_spraypaint);
	CVAR_REGISTER(&init_jb_showen);
	CVAR_REGISTER(&init_jb_debugengine);

	jb_cstrike15        = CVAR_GET_POINTER("jb_cstrike15");
	jb_mixnames         = CVAR_GET_POINTER("jb_mixnames");
	jb_msgwelcome       = CVAR_GET_POINTER("jb_msgwelcome");
	jb_prefixaggression = CVAR_GET_POINTER("jb_prefixaggression");
	jb_suffixskill      = CVAR_GET_POINTER("jb_suffixskill");
	jb_botsmin          = CVAR_GET_POINTER("jb_botsmin");
	jb_botsmax          = CVAR_GET_POINTER("jb_botsmax");
	jb_chat1337         = CVAR_GET_POINTER("jb_chat1337");
	jb_chat             = CVAR_GET_POINTER("jb_chat");
	jb_chati            = CVAR_GET_POINTER("jb_chati");
	jb_chatfreq         = CVAR_GET_POINTER("jb_chatfreq");
	jb_msgradio         = CVAR_GET_POINTER("jb_msgradio");
	jb_aimmomentum      = CVAR_GET_POINTER("jb_aimmomentum");
	jb_aimspeed         = CVAR_GET_POINTER("jb_aimspeed");
	jb_nnupdaterate     = CVAR_GET_POINTER("jb_nnupdaterate");
	jb_campprobability  = CVAR_GET_POINTER("jb_campprobability");
	jb_msgenemydown     = CVAR_GET_POINTER("jb_msgenemydown");
	jb_shoot            = CVAR_GET_POINTER("jb_shoot");
	jb_tkpunish         = CVAR_GET_POINTER("jb_tkpunish");
	jb_language         = CVAR_GET_POINTER("jb_language");
	jb_wp               = CVAR_GET_POINTER("jb_wp");
	jb_wpoffsetx        = CVAR_GET_POINTER("jb_wpoffsetx");
	jb_wpoffsety        = CVAR_GET_POINTER("jb_wpoffsety");
	jb_wpoffsetz        = CVAR_GET_POINTER("jb_wpoffsetz");
	jb_wpfilename       = CVAR_GET_POINTER("jb_wpfilename");
	jb_wpsound          = CVAR_GET_POINTER("jb_wpsound");
	jb_wppath           = CVAR_GET_POINTER("jb_wppath");
	jb_wpauto           = CVAR_GET_POINTER("jb_wpauto");
	jb_wpautopath       = CVAR_GET_POINTER("jb_wpautopath");
	jb_wpautobots       = CVAR_GET_POINTER("jb_wpautobots");
	jb_wpautojump       = CVAR_GET_POINTER("jb_wpautojump");
	jb_wpautojumptest   = CVAR_GET_POINTER("jb_wpautojumptest");
	jb_wpautoadvanced   = CVAR_GET_POINTER("jb_wpautoadvanced");
	jb_wpstats          = CVAR_GET_POINTER("jb_wpstats");
	jb_wprecalc         = CVAR_GET_POINTER("jb_wprecalc");
	jb_prefixdefensive  = CVAR_GET_POINTER("jb_prefixdefensive");
	jb_prefixnormal     = CVAR_GET_POINTER("jb_prefixnormal");
	jb_prefixaggressive = CVAR_GET_POINTER("jb_prefixaggressive");
	jb_pistolonly       = CVAR_GET_POINTER("jb_pistolonly");
	jb_skillmin         = CVAR_GET_POINTER("jb_skillmin");
	jb_skillmax         = CVAR_GET_POINTER("jb_skillmax");
	jb_entergame        = CVAR_GET_POINTER("jb_entergame");
	jb_jointeam         = CVAR_GET_POINTER("jb_jointeam");
	jb_spraypaint       = CVAR_GET_POINTER("jb_spraypaint");
	jb_showen           = CVAR_GET_POINTER("jb_showen");
	jb_debugengine      = CVAR_GET_POINTER("jb_debugengine");

	bRegDone = true;
}

void UpdateLanguage(void)
{
	switch(int(jb_language->value))
	{
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

void ClientPrintEx( entvars_t *client, int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4 )
{
	if(IS_DEDICATED_SERVER() && client == 0){
		if(msg_name)	cout << msg_name;
		if(param1)		cout << param1;
		if(param2)		cout << param2;
		if(param3)		cout << param3;
		if(param4)		cout << param4;
		cout.flush();
	}
	else
		if(client)
			ClientPrint(client,msg_dest,msg_name,param1,param2,param3,param4);
		else if(listenserver_edict)
			ClientPrint(VARS(listenserver_edict),msg_dest,msg_name,param1,param2,param3,param4);
}

/*

  example :
  
	bool bc_whatever(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	}
	
*/

bool bc_addbot(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	BotCreate( pEntity, arg1, arg2, arg3, arg4);
	
	bot_check_time = gpGlobals->time + _PAUSE_TIME;
	
	return true;
}

bool bc_mix_names(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on")){
		if (IS_DEDICATED_SERVER())printf("Note that this command has only effect before adding bots\n");
		CVAR_SET_FLOAT("jb_mixnames", 1);
	}
	else if (FStrEq(arg1, "off")){
		if (IS_DEDICATED_SERVER())printf("Note that this command has only effect before adding bots\n");
		CVAR_SET_FLOAT("jb_mixnames", 0);
	}
	if ( bool(jb_mixnames->value) ){
		if (IS_DEDICATED_SERVER())printf("Bot names are random\n");
	}
	else{
		if (IS_DEDICATED_SERVER())printf("Bot names' order is like in bot_names.txt \n");
	}
	return true;
}

bool bc_welcome(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on")){
		CVAR_SET_FLOAT("jb_msgwelcome", 1);
	}
	else if (FStrEq(arg1, "off")){
		CVAR_SET_FLOAT("jb_msgwelcome", 0);
	}
	if(bool(jb_msgwelcome->value)){
		if(IS_DEDICATED_SERVER()){
			cout << "JoeBOT: Welcome messages are ON" << endl;
		}
	}
	else{
		if(IS_DEDICATED_SERVER()){
			cout << "JoeBOT: Welcome messages are OFF" << endl;
		}
	}
	return true;
}

bool bc_endround(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	cout<< "JoeBOT: Endround"<<endl;
	Endround();
	return true;
}

bool bc_extjoe(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on")){
		CVAR_SET_FLOAT("jb_prefixaggression", 1);
	}
	else if (FStrEq(arg1, "off")){
		CVAR_SET_FLOAT("jb_prefixaggression", 0);
	}
	if(bool(jb_prefixaggression->value)){
		sprintf(szTemp,"JoeBOT: Names are extended by : %s:%s,%s\n",jb_prefixaggressive->string,jb_prefixnormal->string,jb_prefixdefensive->string);
	}
	else{
		sprintf(szTemp,"JoeBOT: Names are not extended by : %s:%s,%s\n",jb_prefixaggressive->string,jb_prefixnormal->string,jb_prefixdefensive->string);
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_extskill(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on")){
		CVAR_SET_FLOAT("jb_suffixskill", 1);
	}
	else if (FStrEq(arg1, "off")){
		CVAR_SET_FLOAT("jb_suffixskill", 0);
	}
	if(bool(jb_suffixskill->value)){
		sprintf(szTemp,"JoeBOT: Names are extended by (skill)\n");
	}
	else{
		sprintf(szTemp,"JoeBOT: Names are not extended by (skill)\n");
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_min_bots(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	int min_bots = atoi( arg1 );
	
	if ((min_bots < 0) || (min_bots > 31))
		min_bots = 0;
	
	CVAR_SET_FLOAT("jb_botsmin", min_bots);
	sprintf(szTemp, "JoeBOT: min_bots set to %d\n", min_bots);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_max_bots(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	int max_bots = atoi( arg1 );
	
	if ((max_bots < 0) || (max_bots > 31)) 
		max_bots = 0;
	
	CVAR_SET_FLOAT("jb_botsmax", max_bots);
	sprintf(szTemp, "JoeBOT: max_bots set to %d\n", max_bots);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_kickbots(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "te")||FStrEq(arg1, "1")){
		KickBots(pEntity,TE,1);
	}
	else if (FStrEq(arg1, "ct")||FStrEq(arg1, "2")){
		KickBots(pEntity,CT,1);
	}
	else if (FStrEq(arg1, "all")||FStrEq(arg1, "5")){
		KickBots(pEntity,-1,1);
	}
	else{
		sprintf(szTemp,"Usage : kickbots [ct/te/all]\n");
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	}
	return true;
}
bool bc_kickbot(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (arg1&&(FStrEq(arg1, "te")||FStrEq(arg1, "1"))){
		KickBots(pEntity,TE,0);
	}
	else if (arg1&&(FStrEq(arg1, "ct")||FStrEq(arg1, "2"))){
		KickBots(pEntity,CT,0);
	}
	else{
		sprintf(szTemp,"Usage : kickbot [ct/te/all]\n");
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	}
	return true;
}

bool bc_leetposs(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	float leetposs = atof(arg1);
	CVAR_SET_FLOAT("jb_chat1337", leetposs);
	sprintf(szTemp,"JoeBOT: possibility for leet is @ %f\nJoeBOT: note that this has only an effect for bots which will be added later.\n",leetposs);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_botchat(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on")){
		CVAR_SET_FLOAT("jb_chat", 1);
	}
	else if (FStrEq(arg1, "off")){
		CVAR_SET_FLOAT("jb_chat", 0);
	}
	else{
		float chatfreq = atof(arg1);
		if (chatfreq < 1)
			chatfreq = 10;
		CVAR_SET_FLOAT("jb_chatfreq", chatfreq);
		CVAR_SET_FLOAT("jb_chat", 1);
	}
	if(bool(jb_chat->value)){
		sprintf(szTemp,"JoeBOT: Bots are chatting @ %fs\n",jb_chatfreq->value);
		/*if(listenserver_edict);
			(*g_engfuncs.pfnClientCommand)(listenserver_edict,"speak \"talk system is on\"\n");*/
	}
	else{
		sprintf(szTemp,"JoeBOT: The bots are quiet\n");
		/*if(listenserver_edict);
			(*g_engfuncs.pfnClientCommand)(listenserver_edict,"speak \"talk system is off\"\n");*/
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_botichat(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "off")){
		CVAR_SET_FLOAT("jb_chati", IC_NONE);
	}
	else if (FStrEq(arg1, "dead")){
		CVAR_SET_FLOAT("jb_chati", IC_DEAD);
	}
	else if (FStrEq(arg1, "alive")){
		CVAR_SET_FLOAT("jb_chati", IC_ALIVE);
	}
	else if (FStrEq(arg1, "all")){
		CVAR_SET_FLOAT("jb_chati", IC_ALL);
	}
	else{
		sprintf(szTemp,"bot_ichat usage is : bot_ichat [off/dead/alive/all]\n");
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	}
	switch(int(jb_chati->value)){
	case IC_NONE:
		sprintf(szTemp,"JoeBOT: The interactive chat won't be used.\n");
		break;
	case IC_DEAD:
		sprintf(szTemp,"JoeBOT: The interactive chat will only be used by dead bots.\n");
		break;
	case IC_ALIVE:
		sprintf(szTemp,"JoeBOT: The interactive chat will only be used by alive bots.\n");
		break;
	case IC_ALL:
		sprintf(szTemp,"JoeBOT: The interactive chat will be used by all bots.\n");
		break;
	default:
		sprintf(szTemp,"JoeBOT: Internal error\n");
		break;
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_botuseradio(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on")){
		CVAR_SET_FLOAT("jb_msgradio", 1);
	}
	else if (FStrEq(arg1, "off")){
		CVAR_SET_FLOAT("jb_msgradio", 0);
	}
	if(bool(jb_msgradio->value)){
		sprintf(szTemp,"Bots are using the radio\n");
	}
	else{
		sprintf(szTemp,"Bots are quiet - no radio commands\n");
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_botmomentum(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "default")){
		CVAR_SET_FLOAT("jb_aimmomentum", 1.0 - ( atof(arg2) / 100.0 ));
		sprintf(szTemp,"Default view momentum is set to %f - for bots already added, there has been no change !\n",100.0-jb_aimmomentum->value*100);
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	}
	if (FStrEq(arg1, "getdefault")||FStrEq(arg1, "get")){
		sprintf(szTemp,"Default view momentum is %f\n",100.0-jb_aimmomentum->value*100);
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	}
	else if (FStrEq(arg1, "all")){
		float f_AMomentum = atof (arg2);
		
		if(f_AMomentum >= 0 && f_AMomentum <=100){
			if(f_AMomentum< 1){
				f_AMomentum = 1;
			}
			if(f_AMomentum> 99){
				f_AMomentum = 99;
			}
			int i;
			for(i=0;i<32;i++){
				if(bots[i]){
					bots[i]->f_AMomentum = 1.0-(f_AMomentum/100.0);
				}
			}
			CVAR_SET_FLOAT("jb_aimmomentum", 1.0 - ( f_AMomentum / 100.0 ));
			sprintf(szTemp,"View Momentum is set to %f for all bots and the default.\n",f_AMomentum);
			ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
		}
		else{
			sprintf(szTemp,"There has been no changes in terms of accuracy ... the value has to be between 0 and 100\n");
			ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
		}
	}
	else{		// arg1 can be a name of a bot ... let's search
		if(strlen(arg1)){		// is somethin there ?
			int i;
			for(i=0;i<32;i++){
				if(bots[i]){
					if(strstr(arg1,STRING(bots[i]->pEdict->v.netname))){
						float f_AMomentum = atof (arg2);
						if(f_AMomentum >= 0 && f_AMomentum <= 100){
							if(f_AMomentum< 1){
								f_AMomentum = 1;
							}
							if(f_AMomentum> 99){
								f_AMomentum = 99;
							}
							bots[i]->f_AMomentum = 1.0-f_AMomentum/100.0;
							sprintf(szTemp,"View Momentum is set to %f for %s\n",f_AMomentum,STRING(bots[i]->pEdict->v.netname));
							ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
							break;
						}
					}
					else{
						sprintf(szTemp,"There has been no changes in terms of accuracy ... the value has to be between 0 and 100\n");
						ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
					}
				}
			}
			if(i==32){	// ... no bot found
				sprintf(szTemp,"%s wasn't found ... is it a bot ?!\n",arg1);
				ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
			}
		}
	}
	return true;
}

bool bc_botvspeed(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "default")){
		CVAR_SET_FLOAT("jb_aimspeed", atof(arg2) / 100.0);
		sprintf(szTemp,"Default aiming speed is set to %f - for bots already added, there has been no change !\n",jb_aimspeed->value*100);
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	}
	if (FStrEq(arg1, "getdefault")||FStrEq(arg1, "get")){
		sprintf(szTemp,"Default aiming speed is %f\n",jb_aimspeed->value*100.0);
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	}
	else if (FStrEq(arg1, "all")){
		float f_ASpeed = atof (arg2);
		
		if(f_ASpeed >= 0 && f_ASpeed <=200){
			if(f_ASpeed< 1){
				f_ASpeed = 1;
			}
			if(f_ASpeed> 200){
				f_ASpeed = 199;
			}
			int i;
			for(i=0;i<32;i++){
				if(bots[i]){
					bots[i]->f_ASpeed = f_ASpeed/100.0;
				}
			}
			CVAR_SET_FLOAT("jb_aimspeed", f_ASpeed / 100.0);
			sprintf(szTemp,"Aiming speed is set to %f for all bots and the default.\n",f_ASpeed);
			ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
		}
		else{
			sprintf(szTemp,"There has been no changes in terms of view speed change ... the value has to be between 0 and 200\n");
			ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
		}
	}
	else{		// arg1 can be a name of a bot ... let's search
		if(strlen(arg1)){		// is somethin there ?
			int i;
			for(i=0;i<32;i++){
				if(bots[i]){
					if(strstr(arg1,STRING(bots[i]->pEdict->v.netname))){
						float f_AMomentum = atof (arg2);
						if(f_AMomentum >= 0 && f_AMomentum <= 100){
							if(f_AMomentum< 1){
								f_AMomentum = 1;
							}
							if(f_AMomentum> 200){
								f_AMomentum = 199;
							}
							bots[i]->f_ASpeed = 1.0-f_AMomentum/100.0;
							sprintf(szTemp,"View change speed is set to %f for %s\n",f_AMomentum,STRING(bots[i]->pEdict->v.netname));
							ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
							break;
						}
					}
					else{
						sprintf(szTemp,"There has been no changes in terms of accuracy ... the value has to be between 0 and 200\n");
						ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
					}
				}
			}
			if(i==32){	// ... no bot found
				sprintf(szTemp,"%s wasn't found ... is it a bot ?!\n",arg1);
				ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
			}
		}
	}
	return true;
}

bool bc_botskill(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "default")){
		CVAR_SET_FLOAT("jb_skillmin", atof(arg2));
		sprintf(szTemp,"Default skill is set to %i - for bots already added, there has been no change !\n",jb_skillmin->value);
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	}
	if (FStrEq(arg1, "getdefault")||FStrEq(arg1, "get")){
		sprintf(szTemp,"Default skill is %i\n",jb_skillmin->value);
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	}
	else if (FStrEq(arg1, "all")){
		int i_Skill = atoi (arg2);
		
		if(i_Skill >= 0 && i_Skill <=100){
			int i;
			for(i=0;i<32;i++){
				if(bots[i]){
					bots[i]->bot_skill = i_Skill;
					bots[i]->UpdateSkill();
				}
			}
			CVAR_SET_FLOAT("jb_skillmin", i_Skill);
			sprintf(szTemp,"Skill is set to %i for all bots and the default.\n",i_Skill);
			ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
		}
		else{
			sprintf(szTemp,"There has been no changes in terms of accuracy ... the value has to be between 0 and 100\n");
			ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
		}
	}
	else{		// arg1 can be a name of a bot ... let's search
		if(strlen(arg1)){		// is somethin there ?
			int i;
			for(i=0;i<32;i++){
				if(bots[i]){
					if(strstr(arg1,STRING(bots[i]->pEdict->v.netname))){
						float f_AMomentum = atof (arg2);
						if(f_AMomentum >= 0 && f_AMomentum <= 100){
							bots[i]->bot_skill = int(f_AMomentum);
							bots[i]->UpdateSkill();
							sprintf(szTemp,"Skill set to %.0f for %s\n",f_AMomentum,STRING(bots[i]->pEdict->v.netname));
							ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
							break;
						}
					}
					else{
						printf(szTemp,"There has been no changes in terms of accuracy ... the value has to be between 0 and 100");
						ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
					}
				}
			}
			if(i==32){	// ... no bot found
				sprintf(szTemp,"%s wasn't found ... is it a bot ?!\n",arg1);
				ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
			}
		}
	}
	return true;
}

bool bc_resetstat(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	WPStat.Init();
	sprintf(szTemp,"JoeBOT: Statistics have been resetted\n");
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_nnupdate(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	float ftemp = atof (arg1);
	if(ftemp < 1.0 || ftemp > 1000){
		//CVAR_SET_FLOAT("jb_nnupdaterate", _DEFAULTNNUPDATE);
	}
	else
		CVAR_SET_FLOAT("jb_nnupdaterate", ftemp);
	
	sprintf(szTemp,"NNs are now running @ %f updates/s, if the framerate is high enough\n",jb_nnupdaterate->value);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_campposs(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	float ftemp = atof (arg1);
	if(ftemp < 0.0 || ftemp > 100){
		//CVAR_SET_FLOAT("jb_nnupdaterate", 2);
	}
	else
		CVAR_SET_FLOAT("jb_campprobability", ftemp);
	
	sprintf(szTemp,"camping possibility is set to %f ( 100 -> very low; 0 -> all the time )\n",jb_campprobability->value);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_wps(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	sprintf(szTemp,"map    : %s\n",WPStat.wpsHeader.szMapname);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,szTemp, NULL, NULL, NULL, NULL);
	sprintf(szTemp,"Kill   : %li\n",WPStat.d.lKill);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,szTemp, NULL, NULL, NULL, NULL);
	sprintf(szTemp,"Killed : %li\n",WPStat.d.lKilled);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_nnstat(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	// combat net
	sprintf(szTemp,"NNSim Version : %s\n",NNCombat->Version());
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,szTemp, NULL, NULL, NULL, NULL);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "------------ CombatNN :\n", NULL, NULL, NULL, NULL);
	sprintf(szTemp,"Prop       : %li\n",NNCombat->m_lNumProp);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,szTemp, NULL, NULL, NULL, NULL);
	sprintf(szTemp,"BProp      : %li\n",((CNeuralNetBProp *)NNCombat)->m_lNumBProp);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,szTemp, NULL, NULL, NULL, NULL);
	// collision net
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "------------ CollNN :\n", NULL, NULL, NULL, NULL);
	sprintf(szTemp,"Prop       : %li\n",NNColl->m_lNumProp);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,szTemp, NULL, NULL, NULL, NULL);
	sprintf(szTemp,"BProp      : %li\n",((CNeuralNetBProp *)NNColl)->m_lNumBProp);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,szTemp, NULL, NULL, NULL, NULL);
	
	return true;
}

bool bc_edown(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on")){
		CVAR_SET_FLOAT("jb_msgenemydown", 1);
	}
	else if (FStrEq(arg1, "off")){
		CVAR_SET_FLOAT("jb_msgenemydown", 0);
	}
	if(bool(jb_msgenemydown->value)){
		sprintf(szTemp,"Bots report \"Enemy down\"\n");
	}
	else{
		sprintf(szTemp,"Bots don't report \"Enemy down\"\n");
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_setme(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	pEdictPlayer = pEntity;
	return true;
}

bool bc_search(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	edict_t *pent = 0;
	char item_name[1000],szBuffer[1000];
	
	sprintf(item_name,"\0");
	sprintf(szBuffer,"------- searching in %s -------------------\n\n\0",STRING(gpGlobals->mapname));
	strcat(item_name,szBuffer);
	
	FILE *fhd;
	fhd=fopen("logent  -  search.txt","a");
	fprintf(fhd,"%s\n",item_name);
	fclose(fhd);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, item_name, NULL, NULL, NULL, NULL);
	while ((pent = UTIL_FindEntityInSphere( pent, pEntity->v.origin,300)) != NULL)
	{
		float fDistance = (pEntity->v.origin-pent->v.origin).Length();
		sprintf(item_name,"\0");
		
		sprintf(szBuffer,"%s\t\0",STRING(pent->v.classname));
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%s\t\0",STRING(pent->v.globalname));
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%s\t\0",STRING(pent->v.netname));
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%s\t\0",STRING(pent->v.target));
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%s\t\0",STRING(pent->v.targetname));
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%s\t\0",STRING(pent->v.model));
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%i\t\0",pent->v.skin);
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%i\t\0",pent->v.body);
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%i\t\0",pent->v.effects);
		strcat(item_name,szBuffer);
		sprintf(szBuffer,"%f\t\0",pent->v.velocity.Length());
		strcat(item_name,szBuffer);

		if(pent ->v.owner){
		sprintf(szBuffer," owner : %s\t\0",STRING(pent->v.owner->v.netname));
		strcat(item_name,szBuffer);
		}
		
		FILE *fhd;
		fhd=fopen("logent  -  search.txt","a");
		fprintf(fhd,"%s\n",item_name);
		fclose(fhd);
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, item_name, NULL, NULL, NULL, NULL);
	};
	return true;
}

bool bc_savesom(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	SP.Save("combat.spt");
	sprintf(szTemp,"JoeBOT: som data saved\n");
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_trainnn(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	TrainNN(pEntity);
	return true;
}

bool bc_savenn(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	char filename[80];
	UTIL_BuildFileName(filename,"joebot","nn.br3");
	NNCombat->SaveFile(filename);
	return true;
}

bool bc_botshoot(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on"))
	{
		CVAR_SET_FLOAT("jb_shoot", 1);
		
		sprintf(szTemp,"Bots shoot\n");
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	}
	else if (FStrEq(arg1, "off"))
	{
		CVAR_SET_FLOAT("jb_shoot", 0);
		
		sprintf(szTemp,"Bots don't like violence any more\n");
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	}
	else{
		if ( !bool(jb_shoot->value) ){
			sprintf(szTemp,"Bots are friendly\n");
		}
		else{
			sprintf(szTemp,"Bots shoot\n");
		}
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	}
	
	return true;
}

bool bc_bottkpunish(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on"))
	{
		CVAR_SET_FLOAT("jb_tkpunish", 1);
	}
	else if (FStrEq(arg1, "off"))
	{
		CVAR_SET_FLOAT("jb_tkpunish", 0);
	}
	if (bool(jb_tkpunish->value)){
		sprintf(szTemp,"They'll sometimes shoot at a teamm8 after a tk\n");
	}
	else{
		sprintf(szTemp,"They won't shoot at teamm8 after tk\n");
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	
	return true;
}

bool bc_showen(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	sprintf(szTemp,"\"showen\" - This command is only to be used for debugging purposes by the author, %s !\n\0",STRING(pEntity->v.netname));
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	
	if (FStrEq(arg1, "on")){
		CVAR_SET_FLOAT("jb_showen", 1);
	}
	else if (FStrEq(arg1, "off")){
		CVAR_SET_FLOAT("jb_showen", 0);
	}
	
	return true;
}

bool bc_debug_engine(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	debug_engine = 1;
	
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "debug_engine enabled!\n", NULL, NULL, NULL, NULL);
	
	return true;
}

bool bc_getp(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	sprintf(szTemp,"%.0f,%.0f,%.0f\n",pEntity->v.origin.x,pEntity->v.origin.y,pEntity->v.origin.z);
	
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_setp(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	Vector p;
	p.x = atof(arg1);
	p.y = atof(arg2);
	p.z = atof(arg3);
	pEntity->v.origin = p;
	return true;
}

bool bc_botmenu(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if(UTIL_GetBotIndex(pEntity) == -1){
		//FakeClientCommand(pEntity,"menuselect 0");
		int index;
		
		index = WaypointFindNearest(pEntity, 100.0, -1);
		
		g_menu_waypoint = index;
		g_menu_state = MENU_1;
		
		UTIL_ShowMenu(pEntity, 0x3FF, -1, FALSE, show_menu_1);
	}
	return true;
}

bool bc_menuselect(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if(g_menu_state == MENU_NONE)
		return false;
	if(UTIL_GetBotIndex(pEntity) == -1){
		pEdictPlayer = pEntity;
		if (g_menu_state == MENU_1)  // main menu...
		{
			if (FStrEq(arg1, "1"))
			{
				g_menu_state = MENU_2;  // display wp stuff menu
				
				UTIL_ShowMenu(pEntity, 0x3FF, -1, FALSE, show_menu_2);
				
				return true;
			}
			else if (FStrEq(arg1, "2")){
				g_menu_state = MENU_3;  // display add menu
				
				UTIL_ShowMenu(pEntity, 0x1FF, -1, FALSE, show_menu_3);
				
				return true;
			}
			else if (FStrEq(arg1, "3")){
				g_menu_state = MENU_4;  // display kick menu
				
				UTIL_ShowMenu(pEntity, 0x1F, -1, FALSE, show_menu_4);
				
				return true;
			}
			else if (FStrEq(arg1, "4")){
				Endround();
				//return true;
			}
			else if (FStrEq(arg1, "5"))	// quit
			{
			}
			else if (FStrEq(arg1, "6")){
				g_menu_state = MENU_5;  // display language menu
				
				UTIL_ShowMenu(pEntity, 0x1F, -1, FALSE, show_menu_5);
				
				return true;
			}
		}
		else if (g_menu_state == MENU_2)  // wp - menu
		{
			if (FStrEq(arg1, "1")){			// toggle waypoints on/off
				CVAR_SET_FLOAT("jb_wp", bool(jb_wp->value) ? 0 : 1);
			}
			else if (FStrEq(arg1, "2")){	// toggle auto waypoint on/off
				CVAR_SET_FLOAT("jb_wpauto", bool(jb_wpauto->value) ? 0 : 1);
				if(bool(jb_wpauto->value)){
					if(listenserver_edict)
						(*g_engfuncs.pfnClientCommand)(listenserver_edict,"speak \"automatic observation system engaged\"\n");
				}
				else{
					if(listenserver_edict)
						(*g_engfuncs.pfnClientCommand)(listenserver_edict,"speak \"automatic observation system disengaged\"\n");
				}
				for(int i=0;i<32;i++){
					AWP_ED[i].iLastWP = -1;
				}
			}
			else if (FStrEq(arg1, "3")){	// toggle auto addpath on/off
				CVAR_SET_FLOAT("jb_wpautopath", jb_wpautopath->value ? 0 : 1);
			}
			else if (FStrEq(arg1, "4")){
				if (!bool(jb_wp->value))
					CVAR_SET_FLOAT("jb_wp", 1);  // turn waypoints on if off
				
				WaypointAdd(pEntity);
			}
			else if (FStrEq(arg1, "5")){
				if (!bool(jb_wp->value))
					CVAR_SET_FLOAT("jb_wp", 1);  // turn waypoints on if off
				
				WaypointDelete(pEntity);
			}
			else if (FStrEq(arg1, "6")){
				g_menu_state = MENU_2a;  // display flags menu
				
				UTIL_ShowMenu(pEntity, 0x3ff, -1, FALSE, show_menu_2a);
				return true;
			}
			else if (FStrEq(arg1, "7")){
				if(WaypointLoad(pEntity))
					ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "waypoints loaded\n", NULL, NULL, NULL, NULL);
			}
			else if (FStrEq(arg1, "8")){
				WaypointSave(pEntity);
				ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "waypoints saved\n", NULL, NULL, NULL, NULL);
			}
			else if (FStrEq(arg1, "9")){
				g_menu_state = MENU_2b;  // display pathwp menu
				
				UTIL_ShowMenu(pEntity, 0x1F, -1, FALSE, show_menu_2b);
				return true;
			}
		}
		else if (g_menu_state == MENU_2a)  // wp 2 - menu
		{
			if (FStrEq(arg1, "1") && g_menu_waypoint!=-1){
				waypoints[g_menu_waypoint].flags^=W_FL_DONTAVOID;
			}
			else if (FStrEq(arg1, "2") && g_menu_waypoint!=-1){
				waypoints[g_menu_waypoint].flags^=W_FL_LADDER;
			}
			else if (FStrEq(arg1, "3") && g_menu_waypoint!=-1){
				waypoints[g_menu_waypoint].flags^=W_FL_VISIT;
			}
			else if (FStrEq(arg1, "4") && g_menu_waypoint!=-1){
				waypoints[g_menu_waypoint].flags^=W_FL_CROUCH;
			}
			else if (FStrEq(arg1, "5") && g_menu_waypoint!=-1){
				waypoints[g_menu_waypoint].flags^=W_FL_AIMING;
			}
			else if (FStrEq(arg1, "6") && g_menu_waypoint!=-1){
				waypoints[g_menu_waypoint].flags^=W_FL_SNIPER;
			}
			else if (FStrEq(arg1, "7") && g_menu_waypoint!=-1){
				waypoints[g_menu_waypoint].flags^=W_FL_FLAG;
			}
			else if (FStrEq(arg1, "8")){
				g_menu_state = MENU_2c;  // display teamspec
				
				UTIL_ShowMenu(pEntity, 0xff, -1, FALSE, show_menu_2c);
				return true;
			}
			else if (FStrEq(arg1, "9")){
				g_menu_state = MENU_2am; // display teamspec
				
				UTIL_ShowMenu(pEntity, 0xff, -1, FALSE, show_menu_2am);
				return true;
			}
		}
		else if (g_menu_state == MENU_2am)  // wp 2 m - menu
		{
			if (FStrEq(arg1, "1") && g_menu_waypoint!=-1){
				waypoints[g_menu_waypoint].flags^=W_FL_FLAG_GOAL;
			}
			else if (FStrEq(arg1, "2") && g_menu_waypoint!=-1){
				waypoints[g_menu_waypoint].flags^=W_FL_DAF;
			}
			else if (FStrEq(arg1, "3") && g_menu_waypoint!=-1){
				waypoints[g_menu_waypoint].flags^=W_FL_JUMP;
			}
		}
		else if (g_menu_state == MENU_2c)  // teamspec - menu
		{
			if (FStrEq(arg1, "1") && g_menu_waypoint!=-1){
				waypoints[g_menu_waypoint].flags |= W_FL_TEAM_SPECIFIC;
				waypoints[g_menu_waypoint].flags &= 0xfffffffc;	// del bits 0 and 1
				waypoints[g_menu_waypoint].flags |= CT;
			}
			else if (FStrEq(arg1, "2") && g_menu_waypoint!=-1){
				waypoints[g_menu_waypoint].flags |= W_FL_TEAM_SPECIFIC;
				waypoints[g_menu_waypoint].flags &= 0xfffffffc;	// del bits 0 and 1
				waypoints[g_menu_waypoint].flags |= TE;
			}
			else if (FStrEq(arg1, "3") && g_menu_waypoint!=-1){
				waypoints[g_menu_waypoint].flags &= ~W_FL_TEAM_SPECIFIC;
			}
		}
		else if (g_menu_state == MENU_2b)  // pathwp - menu
		{
			if (FStrEq(arg1, "1") && g_menu_waypoint!=-1){
				WaypointCreatePath(pEntity, 1);
			}
			else if (FStrEq(arg1, "2") && g_menu_waypoint!=-1){
				WaypointCreatePath(pEntity, 2);
			}
			else if (FStrEq(arg1, "3") && g_menu_waypoint!=-1){
				WaypointRemovePath(pEntity, 1);
			}
			else if (FStrEq(arg1, "4") && g_menu_waypoint!=-1){
				WaypointRemovePath(pEntity, 2);
			}
		}
		else if (g_menu_state == MENU_3)  // add bot
		{
			if (FStrEq(arg1, "1")){
				BotCreate( NULL, "1", "", "", "");
			}
			else if (FStrEq(arg1, "2")){
				BotCreate( NULL, "2", "", "", "");
			}
			else if (FStrEq(arg1, "3")){
				BotCreate( NULL, "5", "", "", "");
			}
			// Fill Server
			else if (FStrEq(arg1, "4")){
				// fill server half with te
				bc_fillserver(pEntity,iType,"te","half","","");
			}
			else if (FStrEq(arg1, "5")){
				// fill server half with ct
				bc_fillserver(pEntity,iType,"ct","half","","");
			}
			else if (FStrEq(arg1, "6")){
				// fill server full with te
				bc_fillserver(pEntity,iType,"te","full","","");
			}
			else if (FStrEq(arg1, "7")){
				// fill server full with ct
				bc_fillserver(pEntity,iType,"ct","full","","");
			}
			else if (FStrEq(arg1, "8")){
				// fill server full
				bc_fillserver(pEntity,iType,"all","full","","");
			}
		}
		else if (g_menu_state == MENU_4){  // kick bot
			if (FStrEq(arg1, "1")){
				KickBots(pEntity,TE,0);
			}
			else if (FStrEq(arg1, "2")){
				KickBots(pEntity,CT,0);
			}
			else if (FStrEq(arg1, "3")){
				KickBots(pEntity,TE,1);
			}
			else if (FStrEq(arg1, "4")){
				KickBots(pEntity,CT,1);
			}
			else if (FStrEq(arg1, "5")){
				KickBots(pEntity,-1,1);
			}
		}
		else if (g_menu_state == MENU_5){  // language
			if (FStrEq(arg1, "1")){
				CVAR_SET_FLOAT("jb_language", LANG_E);
				UpdateLanguage();
			}
			else if (FStrEq(arg1, "2")){
				CVAR_SET_FLOAT("jb_language", LANG_DE);
				UpdateLanguage();
			}
			else if (FStrEq(arg1, "3")){
				CVAR_SET_FLOAT("jb_language", LANG_FR);
				UpdateLanguage();
			}
		}
		g_menu_state = MENU_NONE;
			}
			return true;
}

bool bc_language(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "fr"))
	{
		CVAR_SET_FLOAT("jb_language", LANG_FR);
		
		sprintf(szTemp,"JoeBOT: The Language of the BotMenu is FRENCH\n");
	}
	
	else if (FStrEq(arg1, "de"))
	{
		CVAR_SET_FLOAT("jb_language", LANG_DE);
		
		sprintf(szTemp,"JoeBOT: The Language of the BotMenu is GERMAN\n");
	}
	else if (FStrEq(arg1, "e"))
	{
		CVAR_SET_FLOAT("jb_language", LANG_E);
		
		sprintf(szTemp,"JoeBOT: The Language of the BotMenu is ENGLISH\n");
	}
	else
	{
		CVAR_SET_FLOAT("jb_language", LANG_E);
		
		sprintf(szTemp,"JoeBOT: Unrecognized parameter : The Language of the BotMenu is ENGLISH\n");
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	UpdateLanguage();
	return true;
}

bool bc_debuggoal(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	int iWP = -1;
		  if(strlen(arg1))
			  iWP = atoi(arg1);
		  if(iWP != -1){
			  for(int ischl = 0;ischl < 32; ischl ++){
				  if(bots[ischl]){
					  bots[ischl]->Task.AddTask(BT_GOTO,-1,iWP,(void*)atoi(arg2),0);
				  }
			  }
			  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "All bots got this WP as target to go to.\n", NULL, NULL, NULL, NULL);
		  }
		  return true;
}

bool bc_test(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	edict_t *pent=0;
	
	while(pent = UTIL_FindEntityByClassname(pent,"info_player_start")){
		if(pent){
			WaypointFlood(pent->v.origin,0);
			cout << "fuck" << endl;
			break;
		}
	}
	return true;
}

bool bc_waypoint(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	pEdictPlayer = pEntity;
		  
		  if (FStrEq(arg1, "on"))
		  {
			  CVAR_SET_FLOAT("jb_wp", 1);
			  
			  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "waypoints are ON\n", NULL, NULL, NULL, NULL);
		  }
		  else if (FStrEq(arg1, "off"))
		  {
			  CVAR_SET_FLOAT("jb_wp", 0);
			  
			  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "waypoints are OFF\n", NULL, NULL, NULL, NULL);
		  }
		  else if (FStrEq(arg1, "addstuff")){
			  WaypointAddStuff();
		  }
		  else if (FStrEq(arg1, "showstat")){
			  CVAR_SET_FLOAT("jb_wpstats", 1);
		  }
		  else if (FStrEq(arg1, "clean")){
			  WaypointClean();
		  }
		  else if (FStrEq(arg1, "flood"))
		  {
			  edict_t *pent=0;

			  WaypointFlood(pEntity->v.origin,atoi(arg2));

			  /*while(pent = UTIL_FindEntityByClassname(pent,"info_player_start")){
				  if(pent){
					  WaypointAuto(pent->v.origin,0);
					  break;
				  }
			  }
			  while(pent = UTIL_FindEntityByClassname(pent,"info_player_deathmatch")){
				  if(pent){
					  WaypointAuto(pent->v.origin,0);
					  break;
				  }
			  }
			  while(pent = UTIL_FindEntityByClassname(pent,"hostage")){
				  if(pent){
					  WaypointAuto(pent->v.origin,0);
					  break;
				  }
			  }*/
		  }
		  else if (FStrEq(arg1, "creator"))
		  {
			  strcpy(szWPCreator,arg2);
			  sprintf(szTemp,"The creator of the waypoints is named : %s\n",szWPCreator);
			  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
		  }
		  else if (FStrEq(arg1, "add"))
		  {
			  if (!bool(jb_wp->value))
				  CVAR_SET_FLOAT("jb_wp", 1);  // turn waypoints on if off
			  
			  WaypointAdd(pEntity);
		  }
		  else if (FStrEq(arg1, "delete"))
		  {
			  if (!bool(jb_wp->value))
				  CVAR_SET_FLOAT("jb_wp", 1);  // turn waypoints on if off
			  
			  WaypointDelete(pEntity);
		  }
		  else if (FStrEq(arg1, "jump"))
		  {
			  waypoints[WaypointFindNearest(pEntity,100)].flags^=W_FL_JUMP;
		  }
		  else if (FStrEq(arg1, "ladder"))
		  {
			  waypoints[WaypointFindNearest(pEntity,100)].flags^=W_FL_LADDER;
		  }
		  else if (FStrEq(arg1, "aim"))
		  {
			  waypoints[WaypointFindNearest(pEntity,100)].flags^=W_FL_AIMING;
		  }
		  else if (FStrEq(arg1, "crouch") || FStrEq(arg1, "duck"))
		  {
			  waypoints[WaypointFindNearest(pEntity,100)].flags^=W_FL_CROUCH;
		  }
		  else if (FStrEq(arg1, "hostage")
			  || FStrEq(arg1, "bomb")
			  || FStrEq(arg1, "viprescue"))
		  {
			  waypoints[WaypointFindNearest(pEntity,100)].flags^=W_FL_FLAG;
		  }
		  else if (FStrEq(arg1, "blockhostage")
			  ||FStrEq(arg1, "bh"))
		  {
			  waypoints[WaypointFindNearest(pEntity,100)].flags^=W_FL_BLOCKHOSTAGE;
		  }
		  else if (FStrEq(arg1, "rescue"))
		  {
			  waypoints[WaypointFindNearest(pEntity,100)].flags^=W_FL_FLAG_GOAL;
		  }
		  else if (FStrEq(arg1, "sniper"))
		  {
			  waypoints[WaypointFindNearest(pEntity,100)].flags^=W_FL_SNIPER;
		  }
		  else if (FStrEq(arg1, "visit"))
		  {
			  waypoints[WaypointFindNearest(pEntity,100)].flags^=W_FL_VISIT;
		  }
		  else if (FStrEq(arg1, "dontavoid")||FStrEq(arg1, "da"))
		  {
			  waypoints[WaypointFindNearest(pEntity,100)].flags^=W_FL_DONTAVOID;
		  }
		  else if (FStrEq(arg1, "reset"))
		  {
			  waypoints[WaypointFindNearest(pEntity,100)].flags^=W_FL_RESET;
		  }
		  else if (FStrEq(arg1, "daf")||FStrEq(arg1, "dontavoidfall"))
		  {
			  waypoints[WaypointFindNearest(pEntity,100)].flags^=W_FL_DAF;
		  }
		  else if (FStrEq(arg1, "save"))
		  {
			  if(WaypointSave(pEntity,arg2)){
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "waypoints saved\n", NULL, NULL, NULL, NULL);
			  }
			  else{
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Error while processing this directive\n", NULL, NULL, NULL, NULL);
			  }
		  }
		  else if (FStrEq(arg1, "load")){
			  if (WaypointLoad(pEntity))
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "waypoints loaded\n", NULL, NULL, NULL, NULL);
		  }
		  else if (FStrEq(arg1, "info")){
			  WaypointPrintInfo(pEntity);
		  }
		  else if (FStrEq(arg1, "test")){
			  WaypointPrintTest(pEntity);
		  }
		  else if (FStrEq(arg1, "makepaths")){
			  //WaypointRouteInit();
			  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"sorry, this command cannot be used right now without saving the files.\n It'll save the files now and reload them. Then the paths will be remade\n\n\0", NULL, NULL, NULL, NULL);
			  WaypointSave(pEntity,"tmp");
			  WaypointLoad(pEntity,"tmp");
		  }
		  else if ((FStrEq(arg1, "te")) || (FStrEq(arg1, "terror")))
		  {
			  int i = WaypointFindNearest(pEntity,100);
			  if(i!=-1){
				  waypoints[i].flags |= W_FL_TEAM_SPECIFIC;
				  waypoints[i].flags &= 0xfffffffc;	// del bits 0 and 1
				  waypoints[i].flags |= TE;
			  }
			  else
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "no near wp found\n", NULL, NULL, NULL, NULL);
		  }
		  else if ((FStrEq(arg1, "ct")) || (FStrEq(arg1, "counter")))
		  {
			  int i = WaypointFindNearest(pEntity,100);
			  if(i!=-1){
				  waypoints[i].flags |= W_FL_TEAM_SPECIFIC;
				  waypoints[i].flags &= 0xfffffffc;	// del bits 0 and 1
				  waypoints[i].flags |= CT;
			  }
			  else
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "no near wp found\n", NULL, NULL, NULL, NULL);
		  }
		  else if ((FStrEq(arg1, "nt")) || (FStrEq(arg1, "noteam")))
		  {
			  int i = WaypointFindNearest(pEntity,100);
			  if(i!=-1){
				  waypoints[i].flags &= ~W_FL_TEAM_SPECIFIC;		// clear W_FL_TEAM_SPECIFIC flag
			  }
			  else
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "no near wp found\n", NULL, NULL, NULL, NULL);
		  }
		  else if (FStrEq(arg1, "addstdwp")){		// adding standard wp's
			  WaypointAddStdWP(pEntity);
		  }
		  else if (FStrEq(arg1,"show")){
			  int iWp = atoi(arg2);
			  if(iWp<0||iWp>num_waypoints){
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Out of range\n", NULL, NULL, NULL, NULL);
				  return true;
			  }
			  if(waypoints[iWp].flags & W_FL_DELETED){
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "WP is deleted\n", NULL, NULL, NULL, NULL);
				  return true;
			  }
			  WaypointDrawBeam(pEntity,pEntity->v.origin,waypoints[iWp].origin,100,10,255,255,255,255,100);
			  return true;
		  }
		  else if (FStrEq(arg1, "delall")){
			  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "All waypoints are deleted in memory, but you can still get the old waypoints as long as you don't save other\n", NULL, NULL, NULL, NULL);
			  WaypointInit();
		  }
		  else if (FStrEq(arg1, "forcestatrecalc")){
			  WPStat.InitWP(0);
			  CVAR_SET_FLOAT("jb_wprecalc", 1);
		  }
		  else if (FStrEq(arg1, "forcenostat")){
			  if (FStrEq(arg2, "on"))
			  {
				  CVAR_SET_FLOAT("jb_wprecalc", 0);
				  
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Statistics are NOT recalculated on changes\n", NULL, NULL, NULL, NULL);
			  }
			  else if (FStrEq(arg2, "off"))
			  {
				  CVAR_SET_FLOAT("jb_wprecalc", 1);
				  
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Statistics are recalculated on changes\n", NULL, NULL, NULL, NULL);
			  }
			  else{
				  if(!bool(jb_wprecalc->value)){
					  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Statistics are NOT recalculated on changes\n", NULL, NULL, NULL, NULL);
				  }
				  else{
					  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Statistics are recalculated on changes\n", NULL, NULL, NULL, NULL);
				  }
			  }
		  }
		  else if (FStrEq(arg1, "sound")){
			  if (FStrEq(arg2, "on"))
			  {
				  CVAR_SET_FLOAT("jb_wpsound", 1);
				  
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Sound messages for waypointing are ON\n", NULL, NULL, NULL, NULL);
			  }
			  else if (FStrEq(arg2, "off"))
			  {
				  CVAR_SET_FLOAT("jb_wpsound", 0);
				  
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Sound messages for waypointing are OFF\n", NULL, NULL, NULL, NULL);
			  }
			  else{
				  if(bool(jb_wpsound->value)){
					  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Sound messages for waypointing are ON\n", NULL, NULL, NULL, NULL);
				  }
				  else{
					  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Sound messages for waypointing are OFF\n", NULL, NULL, NULL, NULL);
				  }
			  }
		  }
		  else
		  {
			  if(strlen(arg1)){   
				  sprintf(szTemp,"Unrecognized parameter %s\n",arg1);   
				  ClientPrintEx(VARS(pEntity),HUD_PRINTNOTIFY,szTemp, NULL, NULL, NULL, NULL);   
			  } 
		  }
		  return true;
}

bool bc_autowaypoint(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on"))
	{
		CVAR_SET_FLOAT("jb_wpauto", 1);
		CVAR_SET_FLOAT("jb_wp", 1);
		
		for(int i=0;i<32;i++){
			AWP_ED[i].iLastWP = -1;
		}
	}
	else if (FStrEq(arg1, "off"))
	{
		CVAR_SET_FLOAT("jb_wpauto", 0);
	}
	else if (FStrEq(arg1, "all"))
	{
		CVAR_SET_FLOAT("jb_wpautobots", 1);
	}
	else if (FStrEq(arg1, "human"))
	{
		CVAR_SET_FLOAT("jb_wpautobots", 0);
	}
	else if(FStrEq(arg1,"testjump")){
		if (FStrEq(arg2, "on"))
		{
			CVAR_SET_FLOAT("jb_wpautojumptest", 1);
			
			ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Testing Jump wp while autowaypoint is ON\n", NULL, NULL, NULL, NULL);
		}
		else if (FStrEq(arg2, "off"))
		{
			CVAR_SET_FLOAT("jb_wpautojumptest", 0);
			
			ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Testing Jump wp while autowaypoint is OFF\n", NULL, NULL, NULL, NULL);
		}
		else{
			if(bool(jb_wpautojumptest->value)){
				ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Testing Jump wp while autowaypoint is ON\n", NULL, NULL, NULL, NULL);
			}
			else{
				ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Testing Jump wp while autowaypoint is OFF\n", NULL, NULL, NULL, NULL);
			}
		}
	}
	if(!bool(jb_wpautobots->value)){
		if (bool(jb_wpauto->value))
			sprintf(szTemp, "autowaypoint (by humans) is ON\n");
		else
			sprintf(szTemp, "autowaypoint (by humans) is OFF\n");
	}
	else{
		if (bool(jb_wpauto->value))
			sprintf(szTemp, "autowaypoint (by players) is ON\n");
		else
			sprintf(szTemp, "autowaypoint (by players) is OFF\n");
	}
	
	if (bool(jb_wpauto->value)){
		if(listenserver_edict)
			if(!bool(jb_wpautobots->value)){
				(*g_engfuncs.pfnClientCommand)(listenserver_edict,"speak \"automatic observation system engaged with one authorized inspector\"\n");
			}else{
				(*g_engfuncs.pfnClientCommand)(listenserver_edict,"speak \"automatic observation system engaged with all\"\n");
			}
	}
	else{
		if(listenserver_edict)
			(*g_engfuncs.pfnClientCommand)(listenserver_edict,"speak \"automatic disengaged\"\n");
	}
	
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	
	return true;
}

bool bc_autowaypointaddjump(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on"))
	{
		CVAR_SET_FLOAT("jb_wpautojump", 1);  // turn this on just in case
	}
	else if (FStrEq(arg1, "off"))
	{
		CVAR_SET_FLOAT("jb_wpautojump", 0);
	}
	
	if (bool(jb_wpautojump->value))
		sprintf(szTemp, "g_autowpjump is ON -> a jump waypoint is added while autowaypointing for human players\n");
	else
		sprintf(szTemp, "g_autowpjump is OFF  -> a jump waypoint is NOT added while autowaypointing for human players\n");
	
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	
	return true;
}

bool bc_advancedmovements(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on"))
	{
		CVAR_SET_FLOAT("jb_wpautoadvanced", 1);  // turn this on just in case
	}
	else if (FStrEq(arg1, "off"))
	{
		CVAR_SET_FLOAT("jb_wpautoadvanced", 0);
	}
	
	if (bool(jb_wpautoadvanced->value))
		sprintf(szTemp, "g_waypointadv is ON -> advanced movement system is used\n");
	else
		sprintf(szTemp, "g_waypointadv is OFF  -> advanced movement system is NOT used\n");
	
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	
	return true;
}

bool bc_autopath(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on"))
	{
		CVAR_SET_FLOAT("jb_wpautopath", 1);
	}
	else if (FStrEq(arg1, "off"))
	{
		CVAR_SET_FLOAT("jb_wpautopath", 0);
	}
	
	if (bool(jb_wpautopath->value))
		sprintf(szTemp, "g_auto_addpath is ON\n");
	else
		sprintf(szTemp, "g_auto_addpath is OFF\n");
	
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	
	return true;
}

bool bc_pathwaypoint(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on"))
	{
		CVAR_SET_FLOAT("jb_wppath", 1);
		CVAR_SET_FLOAT("jb_wp", 1);
		
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "pathwaypoint is ON\n", NULL, NULL, NULL, NULL);
	}
	else if (FStrEq(arg1, "off"))
	{
		CVAR_SET_FLOAT("jb_wppath", 0);
		
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "pathwaypoint is OFF\n", NULL, NULL, NULL, NULL);
	}
	else if (FStrEq(arg1, "create1"))
	{
		WaypointCreatePath(pEntity, 1);
	}
	else if (FStrEq(arg1, "create2"))
	{
		WaypointCreatePath(pEntity, 2);
	}
	else if (FStrEq(arg1, "remove1"))
	{
		WaypointRemovePath(pEntity, 1);
	}
	else if (FStrEq(arg1, "remove2"))
	{
		WaypointRemovePath(pEntity, 2);
	}
	
	return true;
}

bool bc_prefix(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if(strlen(arg1)&&!FStrEq(arg1,"-")){
		CVAR_SET_STRING("jb_prefixaggressive", arg1);
	}
	else{
		CVAR_SET_STRING("jb_prefixaggressive", "[JOE]");
	}
	if(strlen(arg2)&&!FStrEq(arg2,"-")){
		CVAR_SET_STRING("jb_prefixnormal", arg2);
	}
	else{
		CVAR_SET_STRING("jb_prefixnormal", "[JoE]");
	}
	if(strlen(arg3)&&!FStrEq(arg3,"-")){
		CVAR_SET_STRING("jb_prefixdefensive", arg3);
	}
	else{
		CVAR_SET_STRING("jb_prefixdefensive", "[J0E]");
	}
	sprintf(szTemp,"JoeBOT: Prefixes are now : agg:%s nor:%s def:%s\n",jb_prefixaggressive->string,jb_prefixnormal->string,jb_prefixdefensive->string);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_pistolonly(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on")){
		CVAR_SET_FLOAT("jb_pistolonly", 1);
	}
	else if (FStrEq(arg1, "off")){
		CVAR_SET_FLOAT("jb_pistolonly", 0);
	}
	if(bool(jb_pistolonly->value)){
		sprintf(szTemp,"JoeBOT: bots only buy pistols\n");
		if(listenserver_edict)
			(*g_engfuncs.pfnClientCommand)(listenserver_edict,"speak \"some weapon are locked\"\n");
	}
	else{
		sprintf(szTemp,"bots buy everything\n");
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp, NULL, NULL, NULL, NULL);
	return true;
}

bool bc_botskill_min(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	int temp = atoi(arg1);
	
	if ((temp >= 0) && (temp <= 100))
		CVAR_SET_FLOAT("jb_skillmin", atoi(arg1));  // set default bot skill level
	
	cout << "JoeBOT: Minimum Skill is set to "<<jb_skillmin->value<<endl;
	
	return true;
}

bool bc_botskill_max(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	int temp = atoi(arg1);
	
	if ((temp >= 0) && (temp <= 100))
		CVAR_SET_FLOAT("jb_skillmax", atoi(arg1));  // set default bot skill level
	
	cout << "JoeBOT: Maximum Skill is set to "<<jb_skillmax->value<<endl;
	
	return true;
}

bool bc_pause(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	bot_cfg_pause_time = gpGlobals->time + atoi( arg1 );
	
	return true;
}
extern CWorldGnome CWG;

bool bc_startgnome(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	//WaypointInit();
	WaypointAddStdWP(pEntity);
	CGnome *p;
	int i;
	
	p = (CGnome*)CWG.Add1stGnome();
	if(p){
		//p->v_origin = waypoints[0].origin;
		p->v_origin = pEntity->v.origin;
		p->v_look = Vector(0,0,0);
	}
	
	for (i=0; i < num_waypoints; i++)
	{
		if ((waypoints[i].flags & W_FL_DELETED) == W_FL_DELETED)
			continue;  // skip any deleted waypoints
		
		if (waypoints[i].flags & W_FL_AIMING)
			continue;  // skip any aiming waypoints
		
		p = (CGnome*)CWG.AddGnome();
		if(p){
			p->v_origin = waypoints[i].origin;
			p->v_look = Vector(0,0,0);
		}
		p = (CGnome*)CWG.AddGnome();
		if(p){
			p->v_origin = waypoints[i].origin;
			p->v_look = Vector(0,90,0);
		}
		p = (CGnome*)CWG.AddGnome();
		if(p){
			p->v_origin = waypoints[i].origin;
			p->v_look = Vector(0,-90,0);
		}
		p = (CGnome*)CWG.AddGnome();
		if(p){
			p->v_origin = waypoints[i].origin;
			p->v_look = Vector(0,180,0);
		}
	}
	return true;
}

bool bc_stopgnome(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	CWG.next = 0;
	return true;
}

bool bc_loadbuyprob(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	strcpy(szLPBFile,arg1);
	cout << "JoeBOT: Setting weapon buy probability file : "<< arg1<<endl;
	bLoadedLPB = false;			// reload on creating next bot
	return true;
}

bool bc_fillserver(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if(!arg1||!*arg1||!arg2||!*arg2){
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: The fillserver command needs at least one more additional argument. The syntax is fillserver [all/ct/te/0/1] [full/half]\n", NULL, NULL, NULL, NULL);		
		return true;
	}
	int iTeam,iMode;
	if(FStrEq(arg1,"te")||FStrEq(arg1,"0")){
		iTeam = FILL_0;
	}
	else if(FStrEq(arg1,"ct")||FStrEq(arg1,"1")){
		iTeam = FILL_1;
	}
	else if(FStrEq(arg1,"all")){
		iTeam = FILL_ALL;
	}

	if(FStrEq(arg2,"full")){
		iMode = FILL_FULL;
	}
	else if(FStrEq(arg2,"half")){
		iMode = FILL_HALF;
	}

	FillServer(iTeam,iMode);

	return true;
}

bool bc_bot_spraying(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if(!*arg1){
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: The syntax is bot_spraying [on/off]\n", NULL, NULL, NULL, NULL);
		return true;
	}
	if(FStrEq(arg1,"on")){
		CVAR_SET_FLOAT("jb_spraypaint", 1);
	}
	else if(FStrEq(arg1,"off")){
		CVAR_SET_FLOAT("jb_spraypaint", 0);
	}
	if(bool(jb_spraypaint->value)){
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: The bots use spraypaints\n", NULL, NULL, NULL, NULL);
	}
	else{
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: The bots don't use spraypaints\n", NULL, NULL, NULL, NULL);
	}
	return true;
}

bool bc_joinwhumanmax(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if(!*arg1){
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: The syntax is joinwhuman_max [on/off]\n", NULL, NULL, NULL, NULL);
		return true;
	}
	if(FStrEq(arg1,"on")){
		CVAR_SET_FLOAT("jb_entergame", 0);
	}
	else if(FStrEq(arg1,"off")){
		CVAR_SET_FLOAT("jb_entergame", 1);
	}
	if(bool(jb_entergame->value)){
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: Bots will join even if there are no human on the server ( max_bots )\n", NULL, NULL, NULL, NULL);
	}
	else{
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: Bots will only join when a human is on the server ( max_bots )\n", NULL, NULL, NULL, NULL);
	}
	return true;
}

bool bc_joinwhumanres(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if(!*arg1){
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: The syntax is joinwhuman_res [on/off]\n", NULL, NULL, NULL, NULL);
		return true;
	}
	if(FStrEq(arg1,"on")){
		CVAR_SET_FLOAT("jb_jointeam", 0);
	}
	else if(FStrEq(arg1,"off")){
		CVAR_SET_FLOAT("jb_jointeam", 1);
	}
	if(bool(jb_jointeam->value)){
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: Bots will respawn even if there are no human players on the server\n", NULL, NULL, NULL, NULL);
	}
	else{
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: Bots will only respawn when a human player is on the server\n", NULL, NULL, NULL, NULL);
	}
	return true;
}

bool bc_randomwpload(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: Mixing WPDirs ... the order of dir.txt is now of no importance\n", NULL, NULL, NULL, NULL);
	WPStat.Save();
	g_WPDir.MixIt();
	WaypointLoad(pEntity);
	WPStat.Load();
	return true;
}

bool bc_botglow(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if(FStrEq(arg1,"on")){
		int i;
		for(i = 0;i < 32 ; i++){
			if(bots[i]){
				bots[i]->pEdict->v.rendermode = kRenderNormal;
				bots[i]->pEdict->v.renderfx = kRenderFxGlowShell;
				bots[i]->pEdict->v.rendercolor.x = UTIL_GetTeam(bots[i]->pEdict)==0?255:0;  // red       
				bots[i]->pEdict->v.rendercolor.y = 0;  // green 
				bots[i]->pEdict->v.rendercolor.z = UTIL_GetTeam(bots[i]->pEdict)!=0?255:0; // blue   
				bots[i]->pEdict->v.renderamt = 20;  // glow shell distance from entity
			}
		}
	}
	else if(FStrEq(arg1,"off")){
		int i;
		for(i = 0;i < 32 ; i++){
			if(bots[i]){
				bots[i]->pEdict->v.renderfx = kRenderFxNone;
			}
		}
	}
	return true;
}
/*
  bool bc_whatever(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
  }
*/
