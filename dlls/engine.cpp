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
// engine.cpp
//

#include "extdll.h"
#include "util.h"
#include "engine.h"

#ifdef USE_METAMOD
#define SDK_UTIL_H  // util.h already included
#include "meta_api.h"
#endif /* USE_METAMOD */

#include "bot.h"
#include "bot_client.h"
#include "bot_modid.h"
#include "CBotCS.h"
#include "globalvars.h"

extern enginefuncs_t g_engfuncs;
//extern bot_t bots[32];
extern int mod_id;

int debug_engine = 0;

void (*botMsgFunction)(void *, int,int) = NULL;
int botMsgIndex;

#ifdef DEBUGENGINE
static FILE *fp;
#endif

int pfnPrecacheModel(char* s)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPrecacheModel: %s\n",s); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnPrecacheModel)(s);
#endif /* USE_METAMOD */
}
int pfnPrecacheSound(char* s)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPrecacheSound: %s\n",s); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnPrecacheSound)(s);
#endif /* USE_METAMOD */
}
void pfnSetModel(edict_t *e, const char *m)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetModel: edict=%x %s\n",e,m); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnSetModel)(e, m);
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
int pfnModelIndex(const char *m)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnModelIndex: %s\n",m); fclose(fp); }
	return (*g_engfuncs.pfnModelIndex)(m);
}
#endif /* not USE_METAMOD */
int pfnModelFrames(int modelIndex)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnModelFrames:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnModelFrames)(modelIndex);
#endif /* USE_METAMOD */
}
void pfnSetSize(edict_t *e, const float *rgflMin, const float *rgflMax)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetSize: %x\n",e); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnSetSize)(e, rgflMin, rgflMax);
#endif /* USE_METAMOD */
}
void pfnChangeLevel(char* s1, char* s2)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnChangeLevel:\n"); fclose(fp); }
#endif
	
	// kick any bot off of the server after time/frag limit...
	for (int index = 0; index < 32; index++)
	{
		if (bots[index])  // is this slot used?
		{
			char cmd[40];
			
			sprintf(cmd, "kick \"%s\"\n", bots[index]->name);
			
			SBInfo[index].respawn_state = RESPAWN_NEED_TO_RESPAWN;
			
			SERVER_COMMAND(cmd);  // kick the bot using (kick "name")
		}
	}
	gf_5th = 0;
	
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnChangeLevel)(s1, s2);
#endif /* USE_METAMOD */
}
void pfnGetSpawnParms(edict_t *ent)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetSpawnParms:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnGetSpawnParms)(ent);
#endif /* USE_METAMOD */
}
void pfnSaveSpawnParms(edict_t *ent)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSaveSpawnParms:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnSaveSpawnParms)(ent);
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
float pfnVecToYaw(const float *rgflVector)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnVecToYaw:\n"); fclose(fp); }
	return (*g_engfuncs.pfnVecToYaw)(rgflVector);
}
void pfnVecToAngles(const float *rgflVectorIn, float *rgflVectorOut)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnVecToAngles:\n"); fclose(fp); }
	(*g_engfuncs.pfnVecToAngles)(rgflVectorIn, rgflVectorOut);
}
#endif /* not USE_METAMOD */
void pfnMoveToOrigin(edict_t *ent, const float *pflGoal, float dist, int iMoveType)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnMoveToOrigin:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnMoveToOrigin)(ent, pflGoal, dist, iMoveType);
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
void pfnChangeYaw(edict_t* ent)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnChangeYaw:\n"); fclose(fp); }
	(*g_engfuncs.pfnChangeYaw)(ent);
}
void pfnChangePitch(edict_t* ent)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnChangePitch:\n"); fclose(fp); }
	(*g_engfuncs.pfnChangePitch)(ent);
}
#endif /* not USE_METAMOD */
edict_t* pfnFindEntityByString(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue)
{
	if (FStrEq(pszValue, "info_map_parameters") && (mod_id == CSTRIKE_DLL)){
		// set flag, that new round has started ....
		iGlobalRSCount = 0;

		//CalcDistances();
		
		for(int i=0;i<32;i++){
			AWP_ED[i].iLastWP = -1;
			bRec[i] = false;
			bStopRec[i] = false;
			if(bots[i]){
				((CBotCS*)bots[i])->bRSInit = true;
				((CBotCS*)bots[i])->bot_vip = false;
				
				iGlobalRSCount ++;
			}
		}
		g_pVIP=0;
		f_round_start = gpGlobals->time;
		g_bBombPlanted = false;
		g_bBombDropped = false;
		
		switch(g_iTypeoM){
		case MT_AS:
			fM[TE] = RANDOM_FLOAT(-1,g_fCampPoss/1.5);
			fM[CT] = RANDOM_FLOAT(-1,g_fCampPoss);
			break;
		case MT_DE:
			fM[TE] = RANDOM_FLOAT(-1,g_fCampPoss);
			fM[CT] = RANDOM_FLOAT(-1,g_fCampPoss/1.2);
			break;
		case MT_CS:
		default:
			fM[TE] = RANDOM_FLOAT(-1,g_fCampPoss/1.5);
			fM[CT] = RANDOM_FLOAT(-1,g_fCampPoss);
		};
	}
	
	//if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFindEntityByString: %s\n",pszValue); fclose(fp); }
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnFindEntityByString)(pEdictStartSearchAfter, pszField, pszValue);
#endif /* USE_METAMOD */
}
int pfnGetEntityIllum(edict_t* pEnt)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetEntityIllum:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnGetEntityIllum)(pEnt);
#endif /* USE_METAMOD */
}
edict_t* pfnFindEntityInSphere(edict_t *pEdictStartSearchAfter, const float *org, float rad)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFindEntityInSphere:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnFindEntityInSphere)(pEdictStartSearchAfter, org, rad);
#endif /* USE_METAMOD */
}
edict_t* pfnFindClientInPVS(edict_t *pEdict)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFindClientInPVS:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnFindClientInPVS)(pEdict);
#endif /* USE_METAMOD */
}
edict_t* pfnEntitiesInPVS(edict_t *pplayer)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnEntitiesInPVS:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnEntitiesInPVS)(pplayer);
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
void pfnMakeVectors(const float *rgflVector)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnMakeVectors:\n"); fclose(fp); }
	(*g_engfuncs.pfnMakeVectors)(rgflVector);
}
void pfnAngleVectors(const float *rgflVector, float *forward, float *right, float *up)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnAngleVectors:\n"); fclose(fp); }
	(*g_engfuncs.pfnAngleVectors)(rgflVector, forward, right, up);
}
#endif /* not USE_METAMOD */
#ifdef USE_METAMOD
edict_t* pfnCreateEntity_Post(void)
{
	edict_t *pent = META_RESULT_ORIG_RET(edict_t *);
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCreateEntity: %x\n",pent); fclose(fp); }
#endif
	RETURN_META_VALUE(MRES_HANDLED, NULL);
}
#else /* not USE_METAMOD */
edict_t* pfnCreateEntity(void)
{
	edict_t *pent = (*g_engfuncs.pfnCreateEntity)();
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCreateEntity: %x\n",pent); fclose(fp); }
#endif
	return pent;
}
#endif /* USE_METAMOD */
void pfnRemoveEntity(edict_t* e)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnRemoveEntity: %x\n",e); fclose(fp); }
#ifdef DEBUGENGINE
	if (debug_engine)
	{
		fp=fopen("bot.txt","a");
		fprintf(fp,"pfnRemoveEntity: %x\n",e);
		if (e->v.model != 0)
			fprintf(fp," model=%s\n", STRING(e->v.model));
		fclose(fp);
	}
#endif
	
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnRemoveEntity)(e);
#endif /* USE_METAMOD */
}
#ifdef USE_METAMOD
edict_t* pfnCreateNamedEntity_Post(int className)
{
	edict_t *pent = META_RESULT_ORIG_RET(edict_t *);
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCreateNamedEntity: edict=%x name=%s\n",pent,STRING(className)); fclose(fp); }
#endif
	RETURN_META_VALUE(MRES_HANDLED, NULL);
}
#else /* not USE_METAMOD */
edict_t* pfnCreateNamedEntity(int className)
{
	edict_t *pent = (*g_engfuncs.pfnCreateNamedEntity)(className);
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCreateNamedEntity: edict=%x name=%s\n",pent,STRING(className)); fclose(fp); }
#endif
	return pent;
}
#endif /* USE_METAMOD */
void pfnMakeStatic(edict_t *ent)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnMakeStatic:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnMakeStatic)(ent);
#endif /* USE_METAMOD */
}
int pfnEntIsOnFloor(edict_t *e)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnEntIsOnFloor:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnEntIsOnFloor)(e);
#endif /* USE_METAMOD */
}
int pfnDropToFloor(edict_t* e)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDropToFloor:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnDropToFloor)(e);
#endif /* USE_METAMOD */
}
int pfnWalkMove(edict_t *ent, float yaw, float dist, int iMode)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWalkMove:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnWalkMove)(ent, yaw, dist, iMode);
#endif /* USE_METAMOD */
}
void pfnSetOrigin(edict_t *e, const float *rgflOrigin)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetOrigin:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnSetOrigin)(e, rgflOrigin);
#endif /* USE_METAMOD */
}
void pfnEmitSound(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnEmitSound:%i;%s\n",channel,sample); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnEmitSound)(entity, channel, sample, volume, attenuation, fFlags, pitch);
#endif /* USE_METAMOD */
}
void pfnEmitAmbientSound(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnEmitAmbientSound:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnEmitAmbientSound)(entity, pos, samp, vol, attenuation, fFlags, pitch);
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
void pfnTraceLine(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceLine:\n"); fclose(fp); }
	(*g_engfuncs.pfnTraceLine)(v1, v2, fNoMonsters, pentToSkip, ptr);
}
void pfnTraceToss(edict_t* pent, edict_t* pentToIgnore, TraceResult *ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceToss:\n"); fclose(fp); }
	(*g_engfuncs.pfnTraceToss)(pent, pentToIgnore, ptr);
}
int pfnTraceMonsterHull(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceMonsterHull:\n"); fclose(fp); }
	return (*g_engfuncs.pfnTraceMonsterHull)(pEdict, v1, v2, fNoMonsters, pentToSkip, ptr);
}
void pfnTraceHull(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceHull:\n"); fclose(fp); }
	(*g_engfuncs.pfnTraceHull)(v1, v2, fNoMonsters, hullNumber, pentToSkip, ptr);
}
void pfnTraceModel(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceModel:\n"); fclose(fp); }
	(*g_engfuncs.pfnTraceModel)(v1, v2, hullNumber, pent, ptr);
}
const char *pfnTraceTexture(edict_t *pTextureEntity, const float *v1, const float *v2 )
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceTexture:\n"); fclose(fp); }
	return (*g_engfuncs.pfnTraceTexture)(pTextureEntity, v1, v2);
}
void pfnTraceSphere(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceSphere:\n"); fclose(fp); }
	(*g_engfuncs.pfnTraceSphere)(v1, v2, fNoMonsters, radius, pentToSkip, ptr);
}
void pfnGetAimVector(edict_t* ent, float speed, float *rgflReturn)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetAimVector:\n"); fclose(fp); }
	(*g_engfuncs.pfnGetAimVector)(ent, speed, rgflReturn);
}
#endif /* not USE_METAMOD */
void pfnServerCommand(char* str)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnServerCommand: %s\n",str); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnServerCommand)(str);
#endif /* USE_METAMOD */
}
void pfnServerExecute(void)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnServerExecute:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnServerExecute)();
#endif /* USE_METAMOD */
}
void pfnClientCommand(edict_t* pEdict, char* szFmt, ...)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnClientCommand=%s\n",szFmt); fclose(fp); }
#endif
	
	if ( !(pEdict->v.flags & (FL_FAKECLIENT | FL_THIRDPARTYBOT)) )
	{
		char tempFmt[256];
		va_list argp;
		va_start(argp, szFmt);
		vsprintf(tempFmt, szFmt, argp);
		(*g_engfuncs.pfnClientCommand)(pEdict, tempFmt);
		va_end(argp);
	}
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	return;
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
void pfnParticleEffect(const float *org, const float *dir, float color, float count)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnParticleEffect:\n"); fclose(fp); }
	(*g_engfuncs.pfnParticleEffect)(org, dir, color, count);
}
void pfnLightStyle(int style, char* val)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnLightStyle:\n"); fclose(fp); }
	(*g_engfuncs.pfnLightStyle)(style, val);
}
int pfnDecalIndex(const char *name)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDecalIndex:\n"); fclose(fp); }
	return (*g_engfuncs.pfnDecalIndex)(name);
}
int pfnPointContents(const float *rgflVector)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPointContents:\n"); fclose(fp); }
	return (*g_engfuncs.pfnPointContents)(rgflVector);
}
#endif /* not USE_METAMOD */
void pfnMessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		//if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnMessageBegin: edict=%x dest=%d type=%d\n",ed,msg_dest,msg_type); fclose(fp); }
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnMessageBegin: edict=%x dest=%d type=%s\n",ed,msg_dest,GET_USER_MSG_NAME(PLID, msg_type, NULL)); fclose(fp); }
#endif
		BotMsgsInit();
		msg_state = 0; // reset state on message begin

		// is this message for a player?
		if (ed)
		{
			botMsgIndex = UTIL_GetBotIndex(ed);

         	// is this message for a bot?
			if (ed->v.flags & FL_THIRDPARTYBOT || botMsgIndex > -1)
				botMsgFunction = botmsgs[msg_type];
			//else printf("DEBUG: [%s] msg_dest = %d, msg_name = %s\n", ed->v.netname ? STRING(ed->v.netname) : "", msg_dest, GET_USER_MSG_NAME(PLID, msg_type, NULL));
		}
		else
		{
			botMsgIndex = -1; // index of bot receiving message (none)

			switch(msg_dest)
			{
				case MSG_INIT:
					if (msg_type == GET_USER_MSG_ID(PLID, "WeaponList", NULL))
						botMsgFunction = botmsgs[msg_type];
					break;
				case MSG_ALL:
					botMsgFunction = botmsgs[msg_type];
					break;
				default:
					;
			}
		}
	}

#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnMessageBegin)(msg_dest, msg_type, pOrigin, ed);
#endif /* USE_METAMOD */
}

void pfnMessageEnd(void)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnMessageEnd:\n"); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
		{
			(*botMsgFunction)((void *)0, botMsgIndex,_CLIENT_END);
			botMsgFunction = NULL; // clear out the bot message function pointer...
		}

		msg_state = 0; // reset state on message end
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnMessageEnd)();
#endif /* USE_METAMOD */
}

void pfnWriteByte(int iValue)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWriteByte: %d\n",iValue); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&iValue, botMsgIndex,_CLIENT_BYTE);

		msg_state++; // increment message state
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnWriteByte)(iValue);
#endif /* USE_METAMOD */
}
void pfnWriteChar(int iValue)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWriteChar: %d\n",iValue); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&iValue, botMsgIndex,_CLIENT_CHAR);

		msg_state++; // increment message state
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnWriteChar)(iValue);
#endif /* USE_METAMOD */
}
void pfnWriteShort(int iValue)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"prnWriteShort: %d\n",iValue); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&iValue, botMsgIndex,_CLIENT_SHORT);

		msg_state++; // increment message state
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnWriteShort)(iValue);
#endif /* USE_METAMOD */
}
void pfnWriteLong(int iValue)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWriteLong: %d\n",iValue); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&iValue, botMsgIndex,_CLIENT_LONG);

		msg_state++; // increment message state
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnWriteLong)(iValue);
#endif /* USE_METAMOD */
}
void pfnWriteAngle(float flValue)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWriteAngle: %f\n",flValue); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&flValue, botMsgIndex,_CLIENT_ANGLE);

		msg_state++; // increment message state
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnWriteAngle)(flValue);
#endif /* USE_METAMOD */
}
void pfnWriteCoord(float flValue)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWriteCoord: %f\n",flValue); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&flValue, botMsgIndex,_CLIENT_COORD);

		msg_state++; // increment message state
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnWriteCoord)(flValue);
#endif /* USE_METAMOD */
}
void pfnWriteString(const char *sz)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWriteString: %s\n",sz); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)sz, botMsgIndex,_CLIENT_STRING);

		msg_state++; // increment message state
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnWriteString)(sz);
#endif /* USE_METAMOD */
}
void pfnWriteEntity(int iValue)
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWriteEntity: %d\n",iValue); fclose(fp); }
#endif
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&iValue, botMsgIndex,_CLIENT_ENTITY);

		msg_state++; // increment message state
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnWriteEntity)(iValue);
#endif /* USE_METAMOD */
}
void pfnCVarRegister(cvar_t *pCvar)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCVarRegister:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnCVarRegister)(pCvar);
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
float pfnCVarGetFloat(const char *szVarName)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCVarGetFloat: %s\n",szVarName); fclose(fp); }
	return (*g_engfuncs.pfnCVarGetFloat)(szVarName);
}
const char* pfnCVarGetString(const char *szVarName)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCVarGetString:\n"); fclose(fp); }
	return (*g_engfuncs.pfnCVarGetString)(szVarName);
}
void pfnCVarSetFloat(const char *szVarName, float flValue)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCVarSetFloat:\n"); fclose(fp); }
	(*g_engfuncs.pfnCVarSetFloat)(szVarName, flValue);
}
void pfnCVarSetString(const char *szVarName, const char *szValue)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCVarSetString:\n"); fclose(fp); }
	(*g_engfuncs.pfnCVarSetString)(szVarName, szValue);
}
#endif /* not USE_METAMOD */
void* pfnPvAllocEntPrivateData(edict_t *pEdict, long cb)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPvAllocEntPrivateData:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnPvAllocEntPrivateData)(pEdict, cb);
#endif /* USE_METAMOD */
}
void* pfnPvEntPrivateData(edict_t *pEdict)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPvEntPrivateData:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnPvEntPrivateData)(pEdict);
#endif /* USE_METAMOD */
}
void pfnFreeEntPrivateData(edict_t *pEdict)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFreeEntPrivateData:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnFreeEntPrivateData)(pEdict);
#endif /* USE_METAMOD */
}
const char* pfnSzFromIndex(int iString)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSzFromIndex:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnSzFromIndex)(iString);
#endif /* USE_METAMOD */
}
int pfnAllocString(const char *szValue)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnAllocString:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnAllocString)(szValue);
#endif /* USE_METAMOD */
}
entvars_t* pfnGetVarsOfEnt(edict_t *pEdict)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetVarsOfEnt:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnGetVarsOfEnt)(pEdict);
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
edict_t* pfnPEntityOfEntOffset(int iEntOffset)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPEntityOfEntOffset:\n"); fclose(fp); }
	return (*g_engfuncs.pfnPEntityOfEntOffset)(iEntOffset);
}
int pfnEntOffsetOfPEntity(const edict_t *pEdict)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnEntOffsetOfPEntity: %x\n",pEdict); fclose(fp); }
	return (*g_engfuncs.pfnEntOffsetOfPEntity)(pEdict);
}
int pfnIndexOfEdict(const edict_t *pEdict)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnIndexOfEdict: %x\n",pEdict); fclose(fp); }
	return (*g_engfuncs.pfnIndexOfEdict)(pEdict);
}
edict_t* pfnPEntityOfEntIndex(int iEntIndex)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPEntityOfEntIndex:\n"); fclose(fp); }
	return (*g_engfuncs.pfnPEntityOfEntIndex)(iEntIndex);
}
edict_t* pfnFindEntityByVars(entvars_t* pvars)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFindEntityByVars:\n"); fclose(fp); }
	return (*g_engfuncs.pfnFindEntityByVars)(pvars);
}
void* pfnGetModelPtr(edict_t* pEdict)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetModelPtr: %x\n",pEdict); fclose(fp); }
	return (*g_engfuncs.pfnGetModelPtr)(pEdict);
}
int pfnRegUserMsg(const char *pszName, int iSize)
{
	if (gpGlobals->deathmatch)
	{
		int msg = (*g_engfuncs.pfnRegUserMsg)(pszName, iSize);
#ifdef DEBUGMESSAGES
		fp=fopen("bot.txt","a");if(fp) fprintf(fp,"pfnRegUserMsg: pszName=%s msg=%d\n",pszName,msg); fclose(fp);
#endif
		AddUserMsg(pszName, msg, iSize);

		return msg;
	}
   
	return (*g_engfuncs.pfnRegUserMsg)(pszName, iSize);
}
void pfnAnimationAutomove(const edict_t* pEdict, float flTime)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnAnimationAutomove:\n"); fclose(fp); }
	(*g_engfuncs.pfnAnimationAutomove)(pEdict, flTime);
}
void pfnGetBonePosition(const edict_t* pEdict, int iBone, float *rgflOrigin, float *rgflAngles )
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetBonePosition:\n"); fclose(fp); }
	(*g_engfuncs.pfnGetBonePosition)(pEdict, iBone, rgflOrigin, rgflAngles);
}
#endif /* not USE_METAMOD */
unsigned long pfnFunctionFromName( const char *pName )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFunctionFromName:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnFunctionFromName)(pName);
#endif /* USE_METAMOD */
}
const char *pfnNameForFunction( unsigned long function )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnNameForFunction:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnNameForFunction)(function);
#endif /* USE_METAMOD */
}
void pfnClientPrintf( edict_t* pEdict, PRINT_TYPE ptype, const char *szMsg )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnClientPrintf:\n"); fclose(fp); }
#endif
	if ( pEdict->v.flags & (FL_FAKECLIENT | FL_THIRDPARTYBOT) )
#ifdef USE_METAMOD
		RETURN_META(MRES_SUPERCEDE);
#else
		return;
#endif

#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnClientPrintf)(pEdict, ptype, szMsg);
#endif /* USE_METAMOD */
}
void pfnServerPrint( const char *szMsg )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnServerPrint:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnServerPrint)(szMsg);
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
void pfnGetAttachment(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles )
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetAttachment:\n"); fclose(fp); }
	(*g_engfuncs.pfnGetAttachment)(pEdict, iAttachment, rgflOrigin, rgflAngles);
}
#endif /* not USE_METAMOD */

const char *pfnCmd_Args( void )
{
	if (isFakeClientCommand)
#ifdef USE_METAMOD
		RETURN_META_VALUE(MRES_SUPERCEDE, &g_argv[0]);
#else /* not USE_METAMOD */
		return &g_argv[0];
#endif /* USE_METAMOD */

#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_IGNORED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnCmd_Args)();
#endif /* USE_METAMOD */
}

const char *pfnCmd_Argv( int argc )
{
	if (isFakeClientCommand)
#ifdef USE_METAMOD
		RETURN_META_VALUE(MRES_SUPERCEDE, GetField (g_argv, argc));
#else /* not USE_METAMOD */
		return (GetField (g_argv, argc)); // returns the wanted argument
#endif /* USE_METAMOD */

#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_IGNORED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnCmd_Argv)(argc);
#endif /* USE_METAMOD */
}

int pfnCmd_Argc( void )
{
	if (isFakeClientCommand)
#ifdef USE_METAMOD
		RETURN_META_VALUE(MRES_SUPERCEDE, fake_arg_count);
#else /* not USE_METAMOD */
		return fake_arg_count;
#endif /* USE_METAMOD */

#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_IGNORED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnCmd_Argc)();
#endif /* USE_METAMOD */
}

void pfnCRC32_Init(CRC32_t *pulCRC)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCRC32_Init:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnCRC32_Init)(pulCRC);
#endif /* USE_METAMOD */
}
void pfnCRC32_ProcessBuffer(CRC32_t *pulCRC, void *p, int len)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCRC32_ProcessBuffer:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnCRC32_ProcessBuffer)(pulCRC, p, len);
#endif /* USE_METAMOD */
}
void pfnCRC32_ProcessByte(CRC32_t *pulCRC, unsigned char ch)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCRC32_ProcessByte:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnCRC32_ProcessByte)(pulCRC, ch);
#endif /* USE_METAMOD */
}
CRC32_t pfnCRC32_Final(CRC32_t pulCRC)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCRC32_Final:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnCRC32_Final)(pulCRC);
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
long pfnRandomLong(long lLow, long lHigh)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnRandomLong: lLow=%d lHigh=%d\n",lLow,lHigh); fclose(fp); }
	return (*g_engfuncs.pfnRandomLong)(lLow, lHigh);
}
float pfnRandomFloat(float flLow, float flHigh)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnRandomFloat:\n"); fclose(fp); }
	return (*g_engfuncs.pfnRandomFloat)(flLow, flHigh);
}
#endif /* not USE_METAMOD */
void pfnSetView(const edict_t *pClient, const edict_t *pViewent )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetView:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnSetView)(pClient, pViewent);
#endif /* USE_METAMOD */
}
float pfnTime( void )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTime:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0.0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnTime)();
#endif /* USE_METAMOD */
}
void pfnCrosshairAngle(const edict_t *pClient, float pitch, float yaw)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCrosshairAngle:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnCrosshairAngle)(pClient, pitch, yaw);
#endif /* USE_METAMOD */
}
byte *pfnLoadFileForMe(char *filename, int *pLength)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnLoadFileForMe: filename=%s\n",filename); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnLoadFileForMe)(filename, pLength);
#endif /* USE_METAMOD */
}
void pfnFreeFile(void *buffer)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFreeFile:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnFreeFile)(buffer);
#endif /* USE_METAMOD */
}
void pfnEndSection(const char *pszSectionName)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnEndSection:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnEndSection)(pszSectionName);
#endif /* USE_METAMOD */
}
int pfnCompareFileTime(char *filename1, char *filename2, int *iCompare)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCompareFileTime:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnCompareFileTime)(filename1, filename2, iCompare);
#endif /* USE_METAMOD */
}
void pfnGetGameDir(char *szGetGameDir)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetGameDir:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnGetGameDir)(szGetGameDir);
#endif /* USE_METAMOD */
}
void pfnCvar_RegisterVariable(cvar_t *variable)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCvar_RegisterVariable:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnCvar_RegisterVariable)(variable);
#endif /* USE_METAMOD */
}
void pfnFadeClientVolume(const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFadeClientVolume:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnFadeClientVolume)(pEdict, fadePercent, fadeOutSeconds, holdTime, fadeInSeconds);
#endif /* USE_METAMOD */
}
void pfnSetClientMaxspeed(const edict_t *pEdict, float fNewMaxspeed)
{
	int index = -1;
	index = UTIL_GetBotIndex(pEdict);
	if(index !=-1){
		bots[index]->f_max_speed = fNewMaxspeed;
		if(mod_id == DOD_DLL){
			bots[index]->f_max_speed /= 2;		// cause the other is sprinting speed
		}
	}
	
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetClientMaxspeed: edict=%x %f\n",pEdict,fNewMaxspeed); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnSetClientMaxspeed)(pEdict, fNewMaxspeed);
#endif /* USE_METAMOD */
}
edict_t * pfnCreateFakeClient(const char *netname)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCreateFakeClient:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnCreateFakeClient)(netname);
#endif /* USE_METAMOD */
}
void pfnRunPlayerMove(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnRunPlayerMove:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnRunPlayerMove)(fakeclient, viewangles, forwardmove, sidemove, upmove, buttons, impulse, msec);
#endif /* USE_METAMOD */
}
int pfnNumberOfEntities(void)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnNumberOfEntities:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnNumberOfEntities)();
#endif /* USE_METAMOD */
}
char* pfnGetInfoKeyBuffer(edict_t *e)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetInfoKeyBuffer:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnGetInfoKeyBuffer)(e);
#endif /* USE_METAMOD */
}
char* pfnInfoKeyValue(char *infobuffer, char *key)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnInfoKeyValue: %s %s\n",infobuffer,key); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnInfoKeyValue)(infobuffer, key);
#endif /* USE_METAMOD */
}
void pfnSetKeyValue(char *infobuffer, char *key, char *value)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetKeyValue: %s %s\n",key,value); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnSetKeyValue)(infobuffer, key, value);
#endif /* USE_METAMOD */
}
void pfnSetClientKeyValue(int clientIndex, char *infobuffer, char *key, char *value)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetClientKeyValue: %s %s\n",key,value); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnSetClientKeyValue)(clientIndex, infobuffer, key, value);
#endif /* USE_METAMOD */
}
int pfnIsMapValid(char *filename)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnIsMapValid:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnIsMapValid)(filename);
#endif /* USE_METAMOD */
}
void pfnStaticDecal( const float *origin, int decalIndex, int entityIndex, int modelIndex )
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnStaticDecal:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnStaticDecal)(origin, decalIndex, entityIndex, modelIndex);
#endif /* USE_METAMOD */
}
int pfnPrecacheGeneric(char* s)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPrecacheGeneric: %s\n",s); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnPrecacheGeneric)(s);
#endif /* USE_METAMOD */
}
int pfnGetPlayerUserId(edict_t *e )
{
	if (gpGlobals->deathmatch)
	{
#ifdef DEBUGENGINE
		if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetPlayerUserId: %x\n",e); fclose(fp); }
#endif
		
		if (mod_id == GEARBOX_DLL)
		{
			// is this edict a bot?
			if (UTIL_GetBotPointer( e ))
#ifdef USE_METAMOD
				RETURN_META_VALUE(MRES_SUPERCEDE, 0);
#else /* not USE_METAMOD */
				return 0;  // don't return a valid index (so bot won't get kicked)
#endif /* USE_METAMOD */
		}
	}
	
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnGetPlayerUserId)(e);
#endif /* USE_METAMOD */
}
void pfnBuildSoundMsg(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnBuildSoundMsg:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnBuildSoundMsg)(entity, channel, sample, volume, attenuation, fFlags, pitch, msg_dest, msg_type, pOrigin, ed);
#endif /* USE_METAMOD */
}
int pfnIsDedicatedServer(void)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnIsDedicatedServer:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnIsDedicatedServer)();
#endif /* USE_METAMOD */
}
cvar_t* pfnCVarGetPointer(const char *szVarName)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCVarGetPointer: %s\n",szVarName); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnCVarGetPointer)(szVarName);
#endif /* USE_METAMOD */
}
unsigned int pfnGetPlayerWONId(edict_t *e)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetPlayerWONId: %x\n",e); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnGetPlayerWONId)(e);
#endif /* USE_METAMOD */
}

// new stuff for SDK 2.0

void pfnInfo_RemoveKey(char *s, const char *key)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnInfo_RemoveKey:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnInfo_RemoveKey)(s, key);
#endif /* USE_METAMOD */
}
const char *pfnGetPhysicsKeyValue(const edict_t *pClient, const char *key)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetPhysicsKeyValue:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnGetPhysicsKeyValue)(pClient, key);
#endif /* USE_METAMOD */
}
void pfnSetPhysicsKeyValue(const edict_t *pClient, const char *key, const char *value)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetPhysicsKeyValue:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnSetPhysicsKeyValue)(pClient, key, value);
#endif /* USE_METAMOD */
}
const char *pfnGetPhysicsInfoString(const edict_t *pClient)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetPhysicsInfoString:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnGetPhysicsInfoString)(pClient);
#endif /* USE_METAMOD */
}
unsigned short pfnPrecacheEvent(int type, const char *psz)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPrecacheEvent:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnPrecacheEvent)(type, psz);
#endif /* USE_METAMOD */
}
void pfnPlaybackEvent(int flags, const edict_t *pInvoker, unsigned short eventindex, float delay,
					  float *origin, float *angles, float fparam1,float fparam2, int iparam1, int iparam2, int bparam1, int bparam2)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPlaybackEvent:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnPlaybackEvent)(flags, pInvoker, eventindex, delay, origin, angles, fparam1, fparam2, iparam1, iparam2, bparam1, bparam2);
#endif /* USE_METAMOD */
}
unsigned char *pfnSetFatPVS(float *org)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetFatPVS:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnSetFatPVS)(org);
#endif /* USE_METAMOD */
}
unsigned char *pfnSetFatPAS(float *org)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetFatPAS:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnSetFatPAS)(org);
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
int pfnCheckVisibility(const edict_t *entity, unsigned char *pset)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCheckVisibility:\n"); fclose(fp); }
	return (*g_engfuncs.pfnCheckVisibility)(entity, pset);
}
void pfnDeltaSetField(struct delta_s *pFields, const char *fieldname)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaSetField:\n"); fclose(fp); }
	(*g_engfuncs.pfnDeltaSetField)(pFields, fieldname);
}
void pfnDeltaUnsetField(struct delta_s *pFields, const char *fieldname)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaUnsetField:\n"); fclose(fp); }
	(*g_engfuncs.pfnDeltaUnsetField)(pFields, fieldname);
}
void pfnDeltaAddEncoder(char *name, void (*conditionalencode)( struct delta_s *pFields, const unsigned char *from, const unsigned char *to))
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaAddEncoder:\n"); fclose(fp); }
	(*g_engfuncs.pfnDeltaAddEncoder)(name, conditionalencode);
}
#endif /* not USE_METAMOD */
int pfnGetCurrentPlayer(void)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetCurrentPlayer:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnGetCurrentPlayer)();
#endif /* USE_METAMOD */
}
int pfnCanSkipPlayer(const edict_t *player)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCanSkipPlayer:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnCanSkipPlayer)(player);
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
int pfnDeltaFindField(struct delta_s *pFields, const char *fieldname)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaFindField:\n"); fclose(fp); }
	return (*g_engfuncs.pfnDeltaFindField)(pFields, fieldname);
}
#endif /* not USE_METAMOD */
void pfnDeltaSetFieldByIndex(struct delta_s *pFields, int fieldNumber)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaSetFieldByIndex:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnDeltaSetFieldByIndex)(pFields, fieldNumber);
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
void pfnDeltaUnsetFieldByIndex(struct delta_s *pFields, int fieldNumber)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaUnsetFieldByIndex:\n"); fclose(fp); }
	(*g_engfuncs.pfnDeltaUnsetFieldByIndex)(pFields, fieldNumber);
}
#endif /* not USE_METAMOD */
void pfnSetGroupMask(int mask, int op)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetGroupMask:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnSetGroupMask)(mask, op);
#endif /* USE_METAMOD */
}
int pfnCreateInstancedBaseline(int classname, struct entity_state_s *baseline)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCreateInstancedBaseline:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_HANDLED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnCreateInstancedBaseline)(classname, baseline);
#endif /* USE_METAMOD */
}
void pfnCvar_DirectSet(struct cvar_s *var, char *value)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCvar_DirectSet:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnCvar_DirectSet)(var, value);
#endif /* USE_METAMOD */
}
void pfnForceUnmodified(FORCE_TYPE type, float *mins, float *maxs, const char *filename)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnForceUnmodified:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnForceUnmodified)(type, mins, maxs, filename);
#endif /* USE_METAMOD */
}
void pfnGetPlayerStats(const edict_t *pClient, int *ping, int *packet_loss)
{
#ifdef DEBUGENGINE
	if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetPlayerStats:\n"); fclose(fp); }
#endif
#ifdef USE_METAMOD
	RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnGetPlayerStats)(pClient, ping, packet_loss);
#endif /* USE_METAMOD */
}
void pfnAddServerCommand(char *cmd_name, void (*function)(void))
{
#ifdef DEBUGENGINE
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnAddServerCommand: %s %x\n",cmd_name,function); fclose(fp); }
#endif
#ifdef USE_METAMOD
   RETURN_META(MRES_HANDLED);
#else /* not USE_METAMOD */
   (*g_engfuncs.pfnAddServerCommand)(cmd_name, function);
#endif /* USE_METAMOD */
}

// hl1108
#ifndef USE_METAMOD
qboolean pfnVoice_GetClientListening(int iReceiver, int iSender) { 
return (*g_engfuncs.pfnVoice_GetClientListening)(iReceiver, iSender); 
} 
qboolean pfnVoice_SetClientListening(int iReceiver, int iSender, qboolean bListen) { 
return (*g_engfuncs.pfnVoice_SetClientListening)(iReceiver, iSender, bListen); 
} 
// hl1109
const char *pfnGetPlayerAuthId(edict_t *e){
	return (*g_engfuncs.pfnGetPlayerAuthId)(e); 
}
#endif /* not USE_METAMOD */

#ifdef USE_METAMOD
enginefuncs_t meta_engfuncs = {
	pfnPrecacheModel,			// pfnPrecacheModel()
	pfnPrecacheSound,			// pfnPrecacheSound()
	pfnSetModel,				// pfnSetModel()
	NULL,				// pfnModelIndex()
	pfnModelFrames,			// pfnModelFrames()

	pfnSetSize,				// pfnSetSize()
	pfnChangeLevel,			// pfnChangeLevel()
	pfnGetSpawnParms,			// pfnGetSpawnParms()
	pfnSaveSpawnParms,			// pfnSaveSpawnParms()

	NULL,				// pfnVecToYaw()
	NULL,			// pfnVecToAngles()
	pfnMoveToOrigin,			// pfnMoveToOrigin()
	NULL,				// pfnChangeYaw()
	NULL,			// pfnChangePitch()

	pfnFindEntityByString,		// pfnFindEntityByString()
	pfnGetEntityIllum,			// pfnGetEntityIllum()
	pfnFindEntityInSphere,		// pfnFindEntityInSphere()
	pfnFindClientInPVS,		// pfnFindClientInPVS()
	pfnEntitiesInPVS,			// pfnEntitiesInPVS()

	NULL,			// pfnMakeVectors()
	NULL,			// pfnAngleVectors()

	NULL,			// pfnCreateEntity()
	pfnRemoveEntity,			// pfnRemoveEntity()
	NULL,		// pfnCreateNamedEntity()

	pfnMakeStatic,				// pfnMakeStatic()
	pfnEntIsOnFloor,			// pfnEntIsOnFloor()
	pfnDropToFloor,			// pfnDropToFloor()

	pfnWalkMove,				// pfnWalkMove()
	pfnSetOrigin,				// pfnSetOrigin()

	pfnEmitSound,				// pfnEmitSound()
	pfnEmitAmbientSound,		// pfnEmitAmbientSound()

	NULL,				// pfnTraceLine()
	NULL,				// pfnTraceToss()
	NULL,		// pfnTraceMonsterHull()
	NULL,				// pfnTraceHull()
	NULL,				// pfnTraceModel()
	NULL,			// pfnTraceTexture()
	NULL,			// pfnTraceSphere()
	NULL,			// pfnGetAimVector()

	pfnServerCommand,			// pfnServerCommand()
	pfnServerExecute,			// pfnServerExecute()
	pfnClientCommand,		// pfnClientCommand()

	NULL,			// pfnParticleEffect()
	NULL,				// pfnLightStyle()
	NULL,				// pfnDecalIndex()
	NULL,			// pfnPointContents()

	pfnMessageBegin,			// pfnMessageBegin()
	pfnMessageEnd,				// pfnMessageEnd()

	pfnWriteByte,				// pfnWriteByte()
	pfnWriteChar,				// pfnWriteChar()
	pfnWriteShort,				// pfnWriteShort()
	pfnWriteLong,				// pfnWriteLong()
	pfnWriteAngle,				// pfnWriteAngle()
	pfnWriteCoord,				// pfnWriteCoord()
	pfnWriteString,			// pfnWriteString()
	pfnWriteEntity,			// pfnWriteEntity()

	pfnCVarRegister,			// pfnCVarRegister()
	NULL,			// pfnCVarGetFloat()
	NULL,			// pfnCVarGetString()
	NULL,			// pfnCVarSetFloat()
	NULL,			// pfnCVarSetString()

	NULL,			// pfnAlertMessage()
	NULL,			// pfnEngineFprintf()

	pfnPvAllocEntPrivateData,	// pfnPvAllocEntPrivateData()
	pfnPvEntPrivateData,		// pfnPvEntPrivateData()
	pfnFreeEntPrivateData,		// pfnFreeEntPrivateData()

	pfnSzFromIndex,			// pfnSzFromIndex()
	pfnAllocString,			// pfnAllocString()

	pfnGetVarsOfEnt, 			// pfnGetVarsOfEnt()
	NULL,		// pfnPEntityOfEntOffset()
	NULL,		// pfnEntOffsetOfPEntity()
	NULL,			// pfnIndexOfEdict()
	NULL,		// pfnPEntityOfEntIndex()
	NULL,		// pfnFindEntityByVars()
	NULL,			// pfnGetModelPtr()

	NULL,				// pfnRegUserMsg()

	NULL,		// pfnAnimationAutomove()
	NULL,		// pfnGetBonePosition()

	pfnFunctionFromName,		// pfnFunctionFromName()
	pfnNameForFunction,		// pfnNameForFunction()

	pfnClientPrintf,			// pfnClientPrintf()			//! JOHN: engine callbacks so game DLL can print messages to individual clients
	pfnServerPrint,			// pfnServerPrint()

	pfnCmd_Args,			// pfnCmd_Args()	//! these 3 added 
	pfnCmd_Argv,			// pfnCmd_Argv()	//! so game DLL can easily 
	pfnCmd_Argc,			// pfnCmd_Argc()	//! access client 'cmd' strings

	NULL,			// pfnGetAttachment()

	pfnCRC32_Init,				// pfnCRC32_Init()
	pfnCRC32_ProcessBuffer,	// pfnCRC32_ProcessBuffer()
	pfnCRC32_ProcessByte,		// pfnCRC32_ProcessByte()
	pfnCRC32_Final,			// pfnCRC32_Final()

	NULL,				// pfnRandomLong()
	NULL,			// pfnRandomFloat()

	pfnSetView,				// pfnSetView()
	pfnTime,					// pfnTime()
	pfnCrosshairAngle,			// pfnCrosshairAngle()

	pfnLoadFileForMe,			// pfnLoadFileForMe()
	pfnFreeFile,				// pfnFreeFile()

	pfnEndSection,				// pfnEndSection()				//! trigger_endsection
	pfnCompareFileTime,		// pfnCompareFileTime()
	pfnGetGameDir,				// pfnGetGameDir()
	pfnCvar_RegisterVariable,	// pfnCvar_RegisterVariable()
	pfnFadeClientVolume,		// pfnFadeClientVolume()
	pfnSetClientMaxspeed,		// pfnSetClientMaxspeed()
	pfnCreateFakeClient,		// pfnCreateFakeClient() 		//! returns NULL if fake client can't be created
	pfnRunPlayerMove,			// pfnRunPlayerMove()
	pfnNumberOfEntities,		// pfnNumberOfEntities()

	pfnGetInfoKeyBuffer,		// pfnGetInfoKeyBuffer()		//! passing in NULL gets the serverinfo
	pfnInfoKeyValue,			// pfnInfoKeyValue()
	pfnSetKeyValue,			// pfnSetKeyValue()
	pfnSetClientKeyValue,		// pfnSetClientKeyValue()

	pfnIsMapValid,				// pfnIsMapValid()
	pfnStaticDecal,			// pfnStaticDecal()
	pfnPrecacheGeneric,		// pfnPrecacheGeneric()
	pfnGetPlayerUserId, 		// pfnGetPlayerUserId()			//! returns the server assigned userid for this player.
	pfnBuildSoundMsg,			// pfnBuildSoundMsg()
	pfnIsDedicatedServer,		// pfnIsDedicatedServer()		//! is this a dedicated server?
	pfnCVarGetPointer,			// pfnCVarGetPointer()
	pfnGetPlayerWONId,			// pfnGetPlayerWONId()			//! returns the server assigned WONid for this player.

	//! YWB 8/1/99 TFF Physics additions
	pfnInfo_RemoveKey,			// pfnInfo_RemoveKey()
	pfnGetPhysicsKeyValue,		// pfnGetPhysicsKeyValue()
	pfnSetPhysicsKeyValue,		// pfnSetPhysicsKeyValue()
	pfnGetPhysicsInfoString,	// pfnGetPhysicsInfoString()
	pfnPrecacheEvent,			// pfnPrecacheEvent()
	pfnPlaybackEvent,			// pfnPlaybackEvent()

	pfnSetFatPVS,				// pfnSetFatPVS()
	pfnSetFatPAS,				// pfnSetFatPAS()

	NULL,		// pfnCheckVisibility()

	NULL,			// pfnDeltaSetField()
	NULL,		// pfnDeltaUnsetField()
	NULL,		// pfnDeltaAddEncoder()
	pfnGetCurrentPlayer,		// pfnGetCurrentPlayer()
	pfnCanSkipPlayer,			// pfnCanSkipPlayer()
	NULL,			// pfnDeltaFindField()
	pfnDeltaSetFieldByIndex,	// pfnDeltaSetFieldByIndex()
	NULL,	// pfnDeltaUnsetFieldByIndex()

	pfnSetGroupMask,			// pfnSetGroupMask()

	pfnCreateInstancedBaseline, // pfnCreateInstancedBaseline()		// d'oh, CreateInstancedBaseline in dllapi too
	pfnCvar_DirectSet,			// pfnCvar_DirectSet()

	pfnForceUnmodified,		// pfnForceUnmodified()

	pfnGetPlayerStats,			// pfnGetPlayerStats()

	NULL,		// pfnAddServerCommand()
};

int GetEngineFunctions(enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion ) 
{
	if(!pengfuncsFromEngine) {
		LOG_ERROR(PLID, "GetEngineFunctions called with null pengfuncsFromEngine");
		return(FALSE);
	}
	else if(*interfaceVersion != ENGINE_INTERFACE_VERSION) {
		LOG_ERROR(PLID, "GetEngineFunctions version mismatch; requested=%d ours=%d", *interfaceVersion, ENGINE_INTERFACE_VERSION);
		// Tell metamod what version we had, so it can figure out who is out of date.
		*interfaceVersion = ENGINE_INTERFACE_VERSION;
		return(FALSE);
	}
	memcpy(pengfuncsFromEngine, &meta_engfuncs, sizeof(enginefuncs_t));
	return TRUE;
}


enginefuncs_t meta_engfuncs_post = {
	NULL,			// pfnPrecacheModel()
	NULL,			// pfnPrecacheSound()
	NULL,				// pfnSetModel()
	NULL,				// pfnModelIndex()
	NULL,			// pfnModelFrames()

	NULL,				// pfnSetSize()
	NULL,			// pfnChangeLevel()
	NULL,			// pfnGetSpawnParms()
	NULL,			// pfnSaveSpawnParms()

	NULL,				// pfnVecToYaw()
	NULL,			// pfnVecToAngles()
	NULL,			// pfnMoveToOrigin()
	NULL,				// pfnChangeYaw()
	NULL,			// pfnChangePitch()

	NULL,		// pfnFindEntityByString()
	NULL,			// pfnGetEntityIllum()
	NULL,		// pfnFindEntityInSphere()
	NULL,		// pfnFindClientInPVS()
	NULL,			// pfnEntitiesInPVS()

	NULL,			// pfnMakeVectors()
	NULL,			// pfnAngleVectors()

	pfnCreateEntity_Post,			// pfnCreateEntity()
	NULL,			// pfnRemoveEntity()
	pfnCreateNamedEntity_Post,		// pfnCreateNamedEntity()

	NULL,				// pfnMakeStatic()
	NULL,			// pfnEntIsOnFloor()
	NULL,			// pfnDropToFloor()

	NULL,				// pfnWalkMove()
	NULL,				// pfnSetOrigin()

	NULL,				// pfnEmitSound()
	NULL,		// pfnEmitAmbientSound()

	NULL,				// pfnTraceLine()
	NULL,				// pfnTraceToss()
	NULL,		// pfnTraceMonsterHull()
	NULL,				// pfnTraceHull()
	NULL,				// pfnTraceModel()
	NULL,			// pfnTraceTexture()
	NULL,			// pfnTraceSphere()
	NULL,			// pfnGetAimVector()

	NULL,			// pfnServerCommand()
	NULL,			// pfnServerExecute()
	NULL,		// pfnClientCommand()

	NULL,			// pfnParticleEffect()
	NULL,				// pfnLightStyle()
	NULL,				// pfnDecalIndex()
	NULL,			// pfnPointContents()

	NULL,			// pfnMessageBegin()
	NULL,				// pfnMessageEnd()

	NULL,				// pfnWriteByte()
	NULL,				// pfnWriteChar()
	NULL,				// pfnWriteShort()
	NULL,				// pfnWriteLong()
	NULL,				// pfnWriteAngle()
	NULL,				// pfnWriteCoord()
	NULL,			// pfnWriteString()
	NULL,			// pfnWriteEntity()

	NULL,			// pfnCVarRegister()
	NULL,			// pfnCVarGetFloat()
	NULL,			// pfnCVarGetString()
	NULL,			// pfnCVarSetFloat()
	NULL,			// pfnCVarSetString()

	NULL,			// pfnAlertMessage()
	NULL,			// pfnEngineFprintf()

	NULL,	// pfnPvAllocEntPrivateData()
	NULL,		// pfnPvEntPrivateData()
	NULL,		// pfnFreeEntPrivateData()

	NULL,			// pfnSzFromIndex()
	NULL,			// pfnAllocString()

	NULL, 			// pfnGetVarsOfEnt()
	NULL,		// pfnPEntityOfEntOffset()
	NULL,		// pfnEntOffsetOfPEntity()
	NULL,			// pfnIndexOfEdict()
	NULL,		// pfnPEntityOfEntIndex()
	NULL,		// pfnFindEntityByVars()
	NULL,			// pfnGetModelPtr()

	NULL,				// pfnRegUserMsg()

	NULL,		// pfnAnimationAutomove()
	NULL,		// pfnGetBonePosition()

	NULL,		// pfnFunctionFromName()
	NULL,		// pfnNameForFunction()

	NULL,			// pfnClientPrintf()			//! JOHN: engine callbacks so game DLL can print messages to individual clients
	NULL,			// pfnServerPrint()

	NULL,			// pfnCmd_Args()	//! these 3 added 
	NULL,			// pfnCmd_Argv()	//! so game DLL can easily 
	NULL,			// pfnCmd_Argc()	//! access client 'cmd' strings

	NULL,			// pfnGetAttachment()

	NULL,				// pfnCRC32_Init()
	NULL,	// pfnCRC32_ProcessBuffer()
	NULL,		// pfnCRC32_ProcessByte()
	NULL,			// pfnCRC32_Final()

	NULL,				// pfnRandomLong()
	NULL,			// pfnRandomFloat()

	NULL,				// pfnSetView()
	NULL,					// pfnTime()
	NULL,			// pfnCrosshairAngle()

	NULL,			// pfnLoadFileForMe()
	NULL,				// pfnFreeFile()

	NULL,				// pfnEndSection()				//! trigger_endsection
	NULL,		// pfnCompareFileTime()
	NULL,				// pfnGetGameDir()
	NULL,	// pfnCvar_RegisterVariable()
	NULL,		// pfnFadeClientVolume()
	NULL,		// pfnSetClientMaxspeed()
	NULL,		// pfnCreateFakeClient() 		//! returns NULL if fake client can't be created
	NULL,			// pfnRunPlayerMove()
	NULL,		// pfnNumberOfEntities()

	NULL,		// pfnGetInfoKeyBuffer()		//! passing in NULL gets the serverinfo
	NULL,			// pfnInfoKeyValue()
	NULL,			// pfnSetKeyValue()
	NULL,		// pfnSetClientKeyValue()

	NULL,				// pfnIsMapValid()
	NULL,			// pfnStaticDecal()
	NULL,		// pfnPrecacheGeneric()
	NULL, 		// pfnGetPlayerUserId()			//! returns the server assigned userid for this player.
	NULL,			// pfnBuildSoundMsg()
	NULL,		// pfnIsDedicatedServer()		//! is this a dedicated server?
	NULL,			// pfnCVarGetPointer()
	NULL,			// pfnGetPlayerWONId()			//! returns the server assigned WONid for this player.

	//! YWB 8/1/99 TFF Physics additions
	NULL,			// pfnInfo_RemoveKey()
	NULL,		// pfnGetPhysicsKeyValue()
	NULL,		// pfnSetPhysicsKeyValue()
	NULL,	// pfnGetPhysicsInfoString()
	NULL,			// pfnPrecacheEvent()
	NULL,			// pfnPlaybackEvent()

	NULL,				// pfnSetFatPVS()
	NULL,				// pfnSetFatPAS()

	NULL,		// pfnCheckVisibility()

	NULL,			// pfnDeltaSetField()
	NULL,		// pfnDeltaUnsetField()
	NULL,		// pfnDeltaAddEncoder()
	NULL,		// pfnGetCurrentPlayer()
	NULL,			// pfnCanSkipPlayer()
	NULL,			// pfnDeltaFindField()
	NULL,	// pfnDeltaSetFieldByIndex()
	NULL,	// pfnDeltaUnsetFieldByIndex()

	NULL,			// pfnSetGroupMask()

	NULL, // pfnCreateInstancedBaseline()		// d'oh, CreateInstancedBaseline in dllapi too
	NULL,			// pfnCvar_DirectSet()

	NULL,		// pfnForceUnmodified()

	NULL,			// pfnGetPlayerStats()

	NULL,		// pfnAddServerCommand()
};

int GetEngineFunctions_Post(enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion ) 
{
	if(!pengfuncsFromEngine) {
		LOG_ERROR(PLID, "GetEngineFunctions called with null pengfuncsFromEngine");
		return(FALSE);
	}
	else if(*interfaceVersion != ENGINE_INTERFACE_VERSION) {
		LOG_ERROR(PLID, "GetEngineFunctions version mismatch; requested=%d ours=%d", *interfaceVersion, ENGINE_INTERFACE_VERSION);
		// Tell metamod what version we had, so it can figure out who is out of date.
		*interfaceVersion = ENGINE_INTERFACE_VERSION;
		return(FALSE);
	}
	memcpy(pengfuncsFromEngine, &meta_engfuncs_post, sizeof(enginefuncs_t));
	return TRUE;
}
#endif /* USE_METAMOD */

