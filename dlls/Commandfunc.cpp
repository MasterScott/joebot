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

#include "CBotBase.h"
#include "Commandfunc.h"

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
		g_bMixNames = true;
	}
	else if (FStrEq(arg1, "off")){
		if (IS_DEDICATED_SERVER())printf("Note that this command has only effect before adding bots\n");
		g_bMixNames = false;
	}
	if ( g_bMixNames ){
		if (IS_DEDICATED_SERVER())printf("Bot names are random\n");
	}
	else{
		if (IS_DEDICATED_SERVER())printf("Bot names' order is like in bot_names.txt \n");
	}
	return true;
}

bool bc_welcome(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on")){
		bCheckWelcome = true;
	}
	else if (FStrEq(arg1, "off")){
		bCheckWelcome = false;
	}
	if(bCheckWelcome){
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
		b_addjoe = true;
	}
	else if (FStrEq(arg1, "off")){
		b_addjoe = false;
	}
	if(b_addjoe){
		sprintf(szTemp,"JoeBOT: Names are extended by : %s:%s,%s\n",szPrefixAgg,szPrefixNor,szPrefixDef);
	}
	else{
		sprintf(szTemp,"JoeBOT: Names are not extended by : %s:%s,%s\n",szPrefixAgg,szPrefixNor,szPrefixDef);
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	return true;
}

bool bc_extskill(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on")){
		b_addskill = true;
	}
	else if (FStrEq(arg1, "off")){
		b_addskill = false;
	}
	if(b_addskill){
		sprintf(szTemp,"JoeBOT: Names are extended by (skill)\n");
	}
	else{
		sprintf(szTemp,"JoeBOT: Names are not extended by (skill)\n");
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	return true;
}

bool bc_min_bots(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	min_bots = atoi( arg1 );
	
	if ((min_bots < 0) || (min_bots > 31))
		min_bots = 0;
	
	sprintf(szTemp, "JoeBOT: min_bots set to %d\n", min_bots);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	return true;
}

bool bc_max_bots(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	max_bots = atoi( arg1 );
	
	if ((max_bots < 0) || (max_bots > 31)) 
		max_bots = 0;
	
	sprintf(szTemp, "JoeBOT: max_bots set to %d\n", max_bots);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
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
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
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
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	}
	return true;
}

bool bc_leetposs(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	g_ileetposs = atoi(arg1);
	sprintf(szTemp,"JoeBOT: possibility for leet is @ %i\nJoeBOT: note that this has only an effect for bots which will be added later.\n",g_ileetposs);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	return true;
}

bool bc_botchat(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on")){
		g_bChat = true;
	}
	else if (FStrEq(arg1, "off")){
		g_bChat = false;
	}
	else{
		g_CHATFREQ = atof (arg1);
		if(g_CHATFREQ < 1)
			g_CHATFREQ = 10;
		g_bChat = true;
	}
	if(g_bChat){
		sprintf(szTemp,"JoeBOT: Bots are chatting @ %fs\n",g_CHATFREQ);
		/*if(listenserver_edict);
			(*g_engfuncs.pfnClientCommand)(listenserver_edict,"speak \"talk system is on\"\n");*/
	}
	else{
		sprintf(szTemp,"JoeBOT: The bots are quiet\n");
		/*if(listenserver_edict);
			(*g_engfuncs.pfnClientCommand)(listenserver_edict,"speak \"talk system is off\"\n");*/
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	return true;
}

bool bc_botichat(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "off")){
		g_iIChat = IC_NONE;
	}
	else if (FStrEq(arg1, "dead")){
		g_iIChat = IC_DEAD;
	}
	else if (FStrEq(arg1, "alive")){
		g_iIChat = IC_ALIVE;
	}
	else if (FStrEq(arg1, "all")){
		g_iIChat = IC_ALL;
	}
	else{
		sprintf(szTemp,"bot_ichat usage is : bot_ichat [off/dead/alive/all]\n");
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	}
	switch(g_iIChat){
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
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	return true;
}

bool bc_botuseradio(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on")){
		g_bUseRadio = true;
	}
	else if (FStrEq(arg1, "off")){
		g_bUseRadio = false;
	}
	if(g_bUseRadio){
		sprintf(szTemp,"Bots are using the radio\n");
	}
	else{
		sprintf(szTemp,"Bots are quiet - no radio commands\n");
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	return true;
}

bool bc_botmomentum(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "default")){
		g_fBaseAMomentum = 1.0-(atof (arg2)/100.0);
		sprintf(szTemp,"Default view momentum is set to %f - for bots already added, there has been no change !\n",100.0-g_fBaseAMomentum*100);
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	}
	if (FStrEq(arg1, "getdefault")||FStrEq(arg1, "get")){
		sprintf(szTemp,"Default view momentum is %f\n",100.0-g_fBaseAMomentum*100);
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
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
			g_fBaseAMomentum = 1.0-(f_AMomentum/100.0);
			sprintf(szTemp,"View Momentum is set to %f for all bots and the default.\n",f_AMomentum);
			ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
		}
		else{
			sprintf(szTemp,"There has been no changes in terms of accuracy ... the value has to be between 0 and 100\n");
			ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
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
							ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
							break;
						}
					}
					else{
						sprintf(szTemp,"There has been no changes in terms of accuracy ... the value has to be between 0 and 100\n");
						ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
					}
				}
			}
			if(i==32){	// ... no bot found
				sprintf(szTemp,"%s wasn't found ... is it a bot ?!\n",arg1);
				ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
			}
		}
	}
	return true;
}

bool bc_botvspeed(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "default")){
		g_fBaseASpeed = atof (arg2)/100.0;
		sprintf(szTemp,"Default aiming speed is set to %f - for bots already added, there has been no change !\n",g_fBaseASpeed*100);
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	}
	if (FStrEq(arg1, "getdefault")||FStrEq(arg1, "get")){
		sprintf(szTemp,"Default aiming speed is %f\n",g_fBaseASpeed*100.0);
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
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
			g_fBaseASpeed = f_ASpeed/100.0;
			sprintf(szTemp,"Aiming speed is set to %f for all bots and the default.\n",f_ASpeed);
			ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
		}
		else{
			sprintf(szTemp,"There has been no changes in terms of view speed change ... the value has to be between 0 and 200\n");
			ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
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
							ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
							break;
						}
					}
					else{
						sprintf(szTemp,"There has been no changes in terms of accuracy ... the value has to be between 0 and 200\n");
						ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
					}
				}
			}
			if(i==32){	// ... no bot found
				sprintf(szTemp,"%s wasn't found ... is it a bot ?!\n",arg1);
				ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
			}
		}
	}
	return true;
}

bool bc_botskill(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "default")){
		g_iBaseSkillMin = atoi (arg2);
		sprintf(szTemp,"Default skill is set to %i - for bots already added, there has been no change !\n",g_iBaseSkillMin);
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	}
	if (FStrEq(arg1, "getdefault")||FStrEq(arg1, "get")){
		sprintf(szTemp,"Default skill is %i\n",g_iBaseSkillMin);
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
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
			g_iBaseSkillMin = i_Skill;
			sprintf(szTemp,"Skill is set to %i for all bots and the default.\n",i_Skill);
			ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
		}
		else{
			sprintf(szTemp,"There has been no changes in terms of accuracy ... the value has to be between 0 and 100\n");
			ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
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
							bots[i]->bot_skill = f_AMomentum;
							bots[i]->UpdateSkill();
							sprintf(szTemp,"Skill set to %.0f for %s\n",f_AMomentum,STRING(bots[i]->pEdict->v.netname));
							ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
							break;
						}
					}
					else{
						printf(szTemp,"There has been no changes in terms of accuracy ... the value has to be between 0 and 100");
						ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
					}
				}
			}
			if(i==32){	// ... no bot found
				sprintf(szTemp,"%s wasn't found ... is it a bot ?!\n",arg1);
				ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
			}
		}
	}
	return true;
}

bool bc_resetstat(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	WPStat.Init();
	sprintf(szTemp,"JoeBOT: Statistics have been resetted\n");
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	return true;
}

bool bc_nnupdate(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	float ftemp = atof (arg1);
	if(ftemp < 1.0 || ftemp > 1000){
		//gnn_update = _DEFAULTNNUPDATE;
	}
	else
		gnn_update = ftemp;
	
	sprintf(szTemp,"NNs are now running @ %f updates/s, if the framerate is high enough\n",gnn_update);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	return true;
}

bool bc_campposs(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	float ftemp = atof (arg1);
	if(ftemp < 0.0 || ftemp > 100){
		//gnn_update = _DEFAULTNNUPDATE;
	}
	else
		g_fCampPoss = ftemp;
	
	sprintf(szTemp,"camping possibility is set to %f ( 100 -> very low; 0 -> all the time )\n",g_fCampPoss);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	return true;
}

bool bc_wps(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	sprintf(szTemp,"map    : %s\n",WPStat.wpsHeader.szMapname);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,szTemp);
	sprintf(szTemp,"Kill   : %li\n",WPStat.d.lKill);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,szTemp);
	sprintf(szTemp,"Killed : %li\n",WPStat.d.lKilled);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,szTemp);
	return true;
}

bool bc_nnstat(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	// combat net
	sprintf(szTemp,"NNSim Version : %s\n",NNCombat->Version());
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,szTemp);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "------------ CombatNN :\n");
	sprintf(szTemp,"Prop       : %li\n",NNCombat->m_lNumProp);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,szTemp);
	sprintf(szTemp,"BProp      : %li\n",((CNeuralNetBProp *)NNCombat)->m_lNumBProp);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,szTemp);
	// collision net
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "------------ CollNN :\n");
	sprintf(szTemp,"Prop       : %li\n",NNColl->m_lNumProp);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,szTemp);
	sprintf(szTemp,"BProp      : %li\n",((CNeuralNetBProp *)NNColl)->m_lNumBProp);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,szTemp);
	
	return true;
}

bool bc_edown(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on")){
		g_bEDown = true;
	}
	else if (FStrEq(arg1, "off")){
		g_bEDown = false;
	}
	if(g_bEDown){
		sprintf(szTemp,"Bots report \"Enemy down\"\n");
	}
	else{
		sprintf(szTemp,"Bots don't report \"Enemy down\"\n");
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
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
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, item_name);
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
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, item_name);
	};
	return true;
}

bool bc_savesom(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	SP.Save("combat.spt");
	sprintf(szTemp,"JoeBOT: som data saved\n");
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
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
		bBotsShoot = true;
		
		sprintf(szTemp,"Bots shoot\n");
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	}
	else if (FStrEq(arg1, "off"))
	{
		bBotsShoot = false;
		
		sprintf(szTemp,"Bots don't like violence any more\n");
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	}
	else{
		if ( !bBotsShoot ){
			sprintf(szTemp,"Bots are friendly\n");
		}
		else{
			sprintf(szTemp,"Bots shoot\n");
		}
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	}
	
	return true;
}

bool bc_bottkpunish(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on"))
	{
		g_bTKPunish = true;
	}
	else if (FStrEq(arg1, "off"))
	{
		g_bTKPunish = false;
	}
	if ( g_bTKPunish ){
		sprintf(szTemp,"They'll sometimes shoot at a teamm8 after a tk\n");
	}
	else{
		sprintf(szTemp,"They won't shoot at teamm8 after tk\n");
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	
	return true;
}

bool bc_showen(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	sprintf(szTemp,"\"showen\" - This command is only to be used for debugging purposes by the author, %s !\n\0",STRING(pEntity->v.netname));
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	
	if (FStrEq(arg1, "on")){
		g_bshowen = true;
	}
	else if (FStrEq(arg1, "off")){
		g_bshowen = false;
	}
	
	return true;
}

bool bc_debug_engine(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	debug_engine = 1;
	
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "debug_engine enabled!\n");
	
	return true;
}

bool bc_getp(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	sprintf(szTemp,"%.0f,%.0f,%.0f\n",pEntity->v.origin.x,pEntity->v.origin.y,pEntity->v.origin.z);
	
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
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
		//FakeClientCommand(pEntity,"menuselect","0",0);
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
			if (FStrEq(arg1, "1")){			// waypoint on/off
				g_waypoint_on!=TRUE?g_waypoint_on=TRUE:g_waypoint_on=FALSE;
			}
			else if (FStrEq(arg1, "2")){	// autowaypoint on/off
				g_auto_waypoint!=TRUE?g_auto_waypoint=TRUE:g_auto_waypoint=FALSE;
				if(g_auto_waypoint){
					if(listenserver_edict)
						(*g_engfuncs.pfnClientCommand)(listenserver_edict,"speak \"automatic observation system engaged\"\n",0);
				}
				else{
					if(listenserver_edict)
						(*g_engfuncs.pfnClientCommand)(listenserver_edict,"speak \"automatic observation system disengaged\"\n",0);
				}
				for(int i=0;i<32;i++){
					AWP_ED[i].iLastWP = -1;
				}
			}
			else if (FStrEq(arg1, "3")){	// auto addpath
				g_auto_addpath!=TRUE?g_auto_addpath=FALSE:g_auto_addpath=TRUE;
			}
			else if (FStrEq(arg1, "4")){
				if (!g_waypoint_on)
					g_waypoint_on = TRUE;  // turn waypoints on if off
				
				WaypointAdd(pEntity);
			}
			else if (FStrEq(arg1, "5")){
				if (!g_waypoint_on)
					g_waypoint_on = TRUE;  // turn waypoints on if off
				
				WaypointDelete(pEntity);
			}
			else if (FStrEq(arg1, "6")){
				g_menu_state = MENU_2a;  // display flags menu
				
				UTIL_ShowMenu(pEntity, 0x3ff, -1, FALSE, show_menu_2a);
				return true;
			}
			else if (FStrEq(arg1, "7")){
				if(WaypointLoad(pEntity))
					ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "waypoints loaded\n");
			}
			else if (FStrEq(arg1, "8")){
				WaypointSave(pEntity);
				ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "waypoints saved\n");
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
				g_iLanguage = LANG_E;
				UpdateLanguage();
			}
			else if (FStrEq(arg1, "2")){
				g_iLanguage = LANG_DE;
				UpdateLanguage();
			}
			else if (FStrEq(arg1, "3")){
				g_iLanguage = LANG_FR;
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
		g_iLanguage = LANG_FR;
		
		sprintf(szTemp,"JoeBOT: The Language of the BotMenu is FRENCH\n");
	}
	
	else if (FStrEq(arg1, "de"))
	{
		g_iLanguage = LANG_DE;
		
		sprintf(szTemp,"JoeBOT: The Language of the BotMenu is GERMAN\n");
	}
	else if (FStrEq(arg1, "e"))
	{
		g_iLanguage = LANG_E;
		
		sprintf(szTemp,"JoeBOT: The Language of the BotMenu is ENGLISH\n");
	}
	else
	{
		g_iLanguage = LANG_E;
		
		sprintf(szTemp,"JoeBOT: Unrecognized parameter : The Language of the BotMenu is ENGLISH\n");
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
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
			  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "All bots got this WP as target to go to.\n");
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
			  g_waypoint_on = TRUE;
			  
			  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "waypoints are ON\n");
		  }
		  else if (FStrEq(arg1, "off"))
		  {
			  g_waypoint_on = FALSE;
			  
			  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "waypoints are OFF\n");
		  }
		  else if (FStrEq(arg1, "addstuff")){
			  WaypointAddStuff();
		  }
		  else if (FStrEq(arg1, "showstat")){
			  g_waypoint_stat = true;
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
			  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
		  }
		  else if (FStrEq(arg1, "add"))
		  {
			  if (!g_waypoint_on)
				  g_waypoint_on = TRUE;  // turn waypoints on if off
			  
			  WaypointAdd(pEntity);
		  }
		  else if (FStrEq(arg1, "delete"))
		  {
			  if (!g_waypoint_on)
				  g_waypoint_on = TRUE;  // turn waypoints on if off
			  
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
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "waypoints saved\n");
			  }
			  else{
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Error while processing this directive\n");
			  }
		  }
		  else if (FStrEq(arg1, "load")){
			  if (WaypointLoad(pEntity))
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "waypoints loaded\n");
		  }
		  else if (FStrEq(arg1, "info")){
			  WaypointPrintInfo(pEntity);
		  }
		  else if (FStrEq(arg1, "test")){
			  WaypointPrintTest(pEntity);
		  }
		  else if (FStrEq(arg1, "makepaths")){
			  //WaypointRouteInit();
			  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"sorry, this command cannot be used right now without saving the files.\n It'll save the files now and reload them. Then the paths will be remade\n\n\0");
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
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "no near wp found\n");
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
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "no near wp found\n");
		  }
		  else if ((FStrEq(arg1, "nt")) || (FStrEq(arg1, "noteam")))
		  {
			  int i = WaypointFindNearest(pEntity,100);
			  if(i!=-1){
				  waypoints[i].flags &= ~W_FL_TEAM_SPECIFIC;		// clear W_FL_TEAM_SPECIFIC flag
			  }
			  else
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "no near wp found\n");
		  }
		  else if (FStrEq(arg1, "addstdwp")){		// adding standard wp's
			  WaypointAddStdWP(pEntity);
		  }
		  else if (FStrEq(arg1,"show")){
			  int iWp = atoi(arg2);
			  if(iWp<0||iWp>num_waypoints){
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Out of range\n");
				  return true;
			  }
			  if(waypoints[iWp].flags & W_FL_DELETED){
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "WP is deleted\n");
				  return true;
			  }
			  WaypointDrawBeam(pEntity,pEntity->v.origin,waypoints[iWp].origin,100,10,255,255,255,255,100);
			  return true;
		  }
		  if (FStrEq(arg1, "delall")){
			  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "All waypoints are deleted in memory, but you can still get the old waypoints as long as you don't save other\n");
			  WaypointInit();
		  }
		  if (FStrEq(arg1, "forcestatrecalc")){
			  WPStat.InitWP(0);
			  g_bForceNOStat = FALSE;
		  }
		  if (FStrEq(arg1, "forcenostat")){
			  if (FStrEq(arg2, "on"))
			  {
				  g_bForceNOStat = TRUE;
				  
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Statistics are NOT recalculated on changes\n");
			  }
			  else if (FStrEq(arg2, "off"))
			  {
				  g_bForceNOStat = FALSE;
				  
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Statistics are recalculated on changes\n");
			  }
			  else{
				  if(g_bForceNOStat){
					  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Statistics are NOT recalculated on changes\n");
				  }
				  else{
					  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Statistics are recalculated on changes\n");
				  }
			  }
		  }
		  if (FStrEq(arg1, "sound")){
			  if (FStrEq(arg2, "on"))
			  {
				  g_waypointsound = TRUE;
				  
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Sound messages for waypointing are ON\n");
			  }
			  else if (FStrEq(arg2, "off"))
			  {
				  g_waypointsound = FALSE;
				  
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Sound messages for waypointing are OFF\n");
			  }
			  else{
				  if(g_waypointsound){
					  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Sound messages for waypointing are ON\n");
				  }
				  else{
					  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Sound messages for waypointing are OFF\n");
				  }
			  }
		  }
		  else
		  {
			  if(strlen(arg1)){   
				  sprintf(szTemp,"parameter unknown : %s",arg1);   
				  ClientPrintEx(VARS(pEntity),HUD_PRINTNOTIFY,szTemp);   
			  } 


			  if (g_waypoint_on)
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "waypoints are ON\n");
			  else
				  ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "waypoints are OFF\n");
		  }
		  
		  return true;
}

bool bc_autowaypoint(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on"))
	{
		g_auto_waypoint = TRUE;
		g_waypoint_on = TRUE;  // turn this on just in case
		
		for(int i=0;i<32;i++){
			AWP_ED[i].iLastWP = -1;
		}
	}
	else if (FStrEq(arg1, "off"))
	{
		g_auto_waypoint = FALSE;
	}
	else if (FStrEq(arg1, "all"))
	{
		g_bAutowpHuman = false;
	}
	else if (FStrEq(arg1, "human"))
	{
		g_bAutowpHuman = true;
	}
	else if(FStrEq(arg1,"testjump")){
		if (FStrEq(arg2, "on"))
		{
			g_bTestJump = TRUE;
			
			ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Testing Jump wp while autowaypoint is ON\n");
		}
		else if (FStrEq(arg2, "off"))
		{
			g_bTestJump = FALSE;
			
			ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Testing Jump wp while autowaypoint is OFF\n");
		}
		else{
			if(g_bTestJump){
				ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Testing Jump wp while autowaypoint is ON\n");
			}
			else{
				ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "Testing Jump wp while autowaypoint is OFF\n");
			}
		}
	}
	if(g_bAutowpHuman){
		if (g_auto_waypoint)
			sprintf(szTemp, "autowaypoint (by humans) is ON\n");
		else
			sprintf(szTemp, "autowaypoint (by humans) is OFF\n");
	}
	else{
		if (g_auto_waypoint)
			sprintf(szTemp, "autowaypoint (by players) is ON\n");
		else
			sprintf(szTemp, "autowaypoint (by players) is OFF\n");
	}
	
	if(g_auto_waypoint){
		if(listenserver_edict)
			if(g_bAutowpHuman){
				(*g_engfuncs.pfnClientCommand)(listenserver_edict,"speak \"automatic observation system engaged with one authorized inspector\"\n",0);
			}else{
				(*g_engfuncs.pfnClientCommand)(listenserver_edict,"speak \"automatic observation system engaged with all\"\n",0);
			}
	}
	else{
		if(listenserver_edict)
			(*g_engfuncs.pfnClientCommand)(listenserver_edict,"speak \"automatic disengaged\"\n",0);
	}
	
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	
	return true;
}

bool bc_autowaypointaddjump(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on"))
	{
		g_autowpjump = TRUE;  // turn this on just in case
	}
	else if (FStrEq(arg1, "off"))
	{
		g_autowpjump = FALSE;
	}
	
	if (g_autowpjump)
		sprintf(szTemp, "g_autowpjump is ON -> a jump waypoint is added while autowaypointing for human players\n");
	else
		sprintf(szTemp, "g_autowpjump is OFF  -> a jump waypoint is NOT added while autowaypointing for human players\n");
	
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	
	return true;
}

bool bc_advancedmovements(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on"))
	{
		g_waypointadv = TRUE;  // turn this on just in case
	}
	else if (FStrEq(arg1, "off"))
	{
		g_waypointadv = FALSE;
	}
	
	if (g_waypointadv)
		sprintf(szTemp, "g_waypointadv is ON -> advanced movement system is used\n");
	else
		sprintf(szTemp, "g_waypointadv is OFF  -> advanced movement system is NOT used\n");
	
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	
	return true;
}

bool bc_autopath(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on"))
	{
		g_auto_addpath = TRUE;
	}
	else if (FStrEq(arg1, "off"))
	{
		g_auto_addpath = FALSE;
	}
	
	if (g_auto_addpath)
		sprintf(szTemp, "g_auto_addpath is ON\n");
	else
		sprintf(szTemp, "g_auto_addpath is OFF\n");
	
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	
	return true;
}

bool bc_pathwaypoint(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on"))
	{
		g_path_waypoint = TRUE;
		g_waypoint_on = TRUE;  // turn this on just in case
		
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "pathwaypoint is ON\n");
	}
	else if (FStrEq(arg1, "off"))
	{
		g_path_waypoint = FALSE;
		
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, "pathwaypoint is OFF\n");
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
	if(strlen(arg1)&&strcmp(arg1,"-")){
		strcpy(szPrefixAgg,arg1);
	}
	else{
		strcpy(szPrefixAgg,"[JOE]");
	}
	if(strlen(arg2)&&strcmp(arg2,"-")){
		strcpy(szPrefixNor,arg2);
	}
	else{
		strcpy(szPrefixNor,"[JoE]");
	}
	if(strlen(arg3)&&strcmp(arg3,"-")){
		strcpy(szPrefixDef,arg3);
	}
	else{
		strcpy(szPrefixDef,"[J0E]");
	}
	sprintf(szTemp,"JoeBOT: Prefixes are now : agg:%s nor:%s def:%s\n",szPrefixAgg,szPrefixNor,szPrefixDef);
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	return true;
}

bool bc_pistolonly(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if (FStrEq(arg1, "on")){
		g_bOnlySec = true;
	}
	else if (FStrEq(arg1, "off")){
		g_bOnlySec = false;
	}
	if(g_bOnlySec){
		sprintf(szTemp,"JoeBOT: bots only buy pistols\n");
		if(listenserver_edict)
			(*g_engfuncs.pfnClientCommand)(listenserver_edict,"speak \"some weapon are locked\"\n");
	}
	else{
		sprintf(szTemp,"bots buy everything\n");
	}
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY, szTemp);
	return true;
}

bool bc_botskill_min(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	int temp = atoi(arg1);
	
	if ((temp >= 0) && (temp <= 100))
		g_iBaseSkillMin = atoi( arg1 );  // set default bot skill level
	
	cout << "JoeBOT: Minimum Skill is set to "<<g_iBaseSkillMin<<endl;
	
	return true;
}

bool bc_botskill_max(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	int temp = atoi(arg1);
	
	if ((temp >= 0) && (temp <= 100))
		g_iBaseSkillMax = atoi( arg1 );  // set default bot skill level
	
	cout << "JoeBOT: Maximum Skill is set to "<<g_iBaseSkillMax<<endl;
	
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
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: The fillserver command needs at least one more additional argument. The syntax is fillserver [all/ct/te/0/1] [full/half]\n");		
		return true;
	}
	int iTeam,iMode;
	if(!strcmp(arg1,"te")||!strcmp(arg1,"0")){
		iTeam = FILL_0;
	}
	else if(!strcmp(arg1,"ct")||!strcmp(arg1,"1")){
		iTeam = FILL_1;
	}
	else if(!strcmp(arg1,"all")){
		iTeam = FILL_ALL;
	}

	if(!strcmp(arg2,"full")){
		iMode = FILL_FULL;
	}
	else if(!strcmp(arg2,"half")){
		iMode = FILL_HALF;
	}

	FillServer(iTeam,iMode);

	return true;
}

bool bc_bot_spraying(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if(!*arg1){
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: The syntax is bot_spraying [on/off]\n");
		return true;
	}
	if(!strcmp(arg1,"on")){
		g_bSpray = true;
	}
	else if(!strcmp(arg1,"off")){
		g_bSpray = false;
	}
	if(g_bSpray){
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: The bots use spraypaints\n");
	}
	else{
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: The bots don't use spraypaints\n");
	}
	return true;
}

bool bc_joinwhumanmax(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if(!*arg1){
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: The syntax is joinwhuman_max [on/off]\n");
		return true;
	}
	if(!strcmp(arg1,"on")){
		g_bJoinWHumanMAX = true;
	}
	else if(!strcmp(arg1,"off")){
		g_bJoinWHumanMAX = false;
	}
	if(g_bJoinWHumanMAX){
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: Bots will only join when a human is on the server ( max_bots )\n");
	}
	else{
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: Bots will join even if there are no human on the server ( max_bots )\n");
	}
	return true;
}

bool bc_joinwhumanres(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if(!*arg1){
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: The syntax is joinwhuman_res [on/off]\n");
		return true;
	}
	if(!strcmp(arg1,"on")){
		g_bJoinWHumanRES = true;
	}
	else if(!strcmp(arg1,"off")){
		g_bJoinWHumanRES = false;
	}
	if(g_bJoinWHumanRES){
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: Bots will only respawn when a human player is on the server\n");
	}
	else{
		ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: Bots will respawn even if there are no human players on the server\n");
	}
	return true;
}

bool bc_randomwpload(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	ClientPrintEx( VARS(pEntity), HUD_PRINTNOTIFY,"JoeBOT: Mixing WPDirs ... the order of dir.txt is now of no importance\n");
	WPStat.Save();
	g_WPDir.MixIt();
	WaypointLoad(pEntity);
	WPStat.Load();
	return true;
}

bool bc_botglow(edict_t *pEntity,int iType,const char *arg1,const char *arg2,const char *arg3,const char *arg4){
	if(!strcmp(arg1,"on")){
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
	else if(!strcmp(arg1,"off")){
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