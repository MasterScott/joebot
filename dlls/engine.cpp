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
#include "Commandfunc.h"
#include "globalvars.h"

extern enginefuncs_t g_engfuncs;
//extern bot_t bots[32];
extern int mod_id;

int debug_engine = 0;

void (*botMsgFunction)(void *, int, int) = NULL;
int botMsgIndex;

#ifndef USE_METAMOD
int pfnPrecacheModel(char* s)
{
	BOT_LOG("pfnPrecacheModel", ("s=%s", s));
	return (*g_engfuncs.pfnPrecacheModel)(s);
}
int pfnPrecacheSound(char* s)
{
	BOT_LOG("pfnPrecacheSound", ("s=%s",s));
	return (*g_engfuncs.pfnPrecacheSound)(s);
}
void pfnSetModel(edict_t *e, const char *m)
{
	BOT_LOG("pfnSetModel", ("e=%x, m=%s", e, m));
	(*g_engfuncs.pfnSetModel)(e, m);
}
int pfnModelIndex(const char *m)
{
	//BOT_LOG("pfnModelIndex", ("m=%s", m));
	return (*g_engfuncs.pfnModelIndex)(m);
}
int pfnModelFrames(int modelIndex)
{
	BOT_LOG("pfnModelFrames", (""));
	return (*g_engfuncs.pfnModelFrames)(modelIndex);
}
void pfnSetSize(edict_t *e, const float *rgflMin, const float *rgflMax)
{
	BOT_LOG("pfnSetSize", ("e=%x", e));
	(*g_engfuncs.pfnSetSize)(e, rgflMin, rgflMax);
}
#endif /* not USE_METAMOD */
void pfnChangeLevel(char* s1, char* s2)
{
	BOT_LOG("pfnChangeLevel", (""));
	
	// kick any bot off of the server after time/frag limit...
	for (int index = 0; index < 32; index++)
	{
		if (bots[index])  // is this slot used?
		{
			SBInfo[index].respawn_state = RESPAWN_NEED_TO_RESPAWN;
			SERVER_COMMAND(UTIL_VarArgs("kick \"%s\"\n", bots[index]->name));  // kick the bot using (kick "name")
		}
	}
	gf_5th = 0;
	
#ifdef USE_METAMOD
	RETURN_META(MRES_IGNORED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnChangeLevel)(s1, s2);
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
void pfnGetSpawnParms(edict_t *ent)
{
	BOT_LOG("pfnGetSpawnParms", (""));
	(*g_engfuncs.pfnGetSpawnParms)(ent);
}
void pfnSaveSpawnParms(edict_t *ent)
{
	BOT_LOG("pfnSaveSpawnParms", (""));
	(*g_engfuncs.pfnSaveSpawnParms)(ent);
}
float pfnVecToYaw(const float *rgflVector)
{
	//BOT_LOG("pfnVecToYaw", (""));
	return (*g_engfuncs.pfnVecToYaw)(rgflVector);
}
void pfnVecToAngles(const float *rgflVectorIn, float *rgflVectorOut)
{
	//BOT_LOG("pfnVecToAngles", (""));
	(*g_engfuncs.pfnVecToAngles)(rgflVectorIn, rgflVectorOut);
}
void pfnMoveToOrigin(edict_t *ent, const float *pflGoal, float dist, int iMoveType)
{
	BOT_LOG("pfnMoveToOrigin", (""));
	(*g_engfuncs.pfnMoveToOrigin)(ent, pflGoal, dist, iMoveType);
}
void pfnChangeYaw(edict_t* ent)
{
	//BOT_LOG("pfnChangeYaw", (""));
	(*g_engfuncs.pfnChangeYaw)(ent);
}
void pfnChangePitch(edict_t* ent)
{
	//BOT_LOG("pfnChangePitch", (""));
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
			fM[CS_TEAM_TE] = RANDOM_FLOAT(-1,jb_campprobability->value/1.5);
			fM[CS_TEAM_CT] = RANDOM_FLOAT(-1,jb_campprobability->value);
			break;
		case MT_DE:
			fM[CS_TEAM_TE] = RANDOM_FLOAT(-1,jb_campprobability->value);
			fM[CS_TEAM_CT] = RANDOM_FLOAT(-1,jb_campprobability->value/1.2);
			break;
		case MT_CS:
		default:
			fM[CS_TEAM_TE] = RANDOM_FLOAT(-1,jb_campprobability->value/1.5);
			fM[CS_TEAM_CT] = RANDOM_FLOAT(-1,jb_campprobability->value);
		};
	}
	
	//BOT_LOG("pfnFindEntityByString", ("pszValue=%s", pszValue));
#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_IGNORED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnFindEntityByString)(pEdictStartSearchAfter, pszField, pszValue);
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
int pfnGetEntityIllum(edict_t* pEnt)
{
	BOT_LOG("pfnGetEntityIllum", (""));
	return (*g_engfuncs.pfnGetEntityIllum)(pEnt);
}
edict_t* pfnFindEntityInSphere(edict_t *pEdictStartSearchAfter, const float *org, float rad)
{
	BOT_LOG("pfnFindEntityInSphere", (""));
	return (*g_engfuncs.pfnFindEntityInSphere)(pEdictStartSearchAfter, org, rad);
}
edict_t* pfnFindClientInPVS(edict_t *pEdict)
{
	BOT_LOG("pfnFindClientInPVS", (""));
	return (*g_engfuncs.pfnFindClientInPVS)(pEdict);
}
edict_t* pfnEntitiesInPVS(edict_t *pplayer)
{
	BOT_LOG("pfnEntitiesInPVS", (""));
	return (*g_engfuncs.pfnEntitiesInPVS)(pplayer);
}
void pfnMakeVectors(const float *rgflVector)
{
	//BOT_LOG("pfnMakeVectors", (""));
	(*g_engfuncs.pfnMakeVectors)(rgflVector);
}
void pfnAngleVectors(const float *rgflVector, float *forward, float *right, float *up)
{
	//BOT_LOG("pfnAngleVectors", (""));
	(*g_engfuncs.pfnAngleVectors)(rgflVector, forward, right, up);
}
edict_t* pfnCreateEntity(void)
{
	edict_t *pent = (*g_engfuncs.pfnCreateEntity)();
	BOT_LOG("pfnCreateEntity", ("pent=%x", pent));
	return pent;
}
void pfnRemoveEntity(edict_t* e)
{
	//BOT_LOG("pfnRemoveEntity", ("e=%x",e));
	BOT_LOG("pfnRemoveEntity", ("e=%x, e->v.model=%s", e, e ? STRING(e->v.model) : ""));
	(*g_engfuncs.pfnRemoveEntity)(e);
}
edict_t* pfnCreateNamedEntity(int className)
{
	edict_t *pent = (*g_engfuncs.pfnCreateNamedEntity)(className);
	BOT_LOG("pfnCreateNamedEntity", ("pent=%x, name=%s", pent, STRING(className)));
	return pent;
}
void pfnMakeStatic(edict_t *ent)
{
	BOT_LOG("pfnMakeStatic", (""));
	(*g_engfuncs.pfnMakeStatic)(ent);
}
int pfnEntIsOnFloor(edict_t *e)
{
	BOT_LOG("pfnEntIsOnFloor", (""));
	return (*g_engfuncs.pfnEntIsOnFloor)(e);
}
int pfnDropToFloor(edict_t* e)
{
	BOT_LOG("pfnDropToFloor", (""));
	return (*g_engfuncs.pfnDropToFloor)(e);
}
int pfnWalkMove(edict_t *ent, float yaw, float dist, int iMode)
{
	BOT_LOG("pfnWalkMove", (""));
	return (*g_engfuncs.pfnWalkMove)(ent, yaw, dist, iMode);
}
void pfnSetOrigin(edict_t *e, const float *rgflOrigin)
{
	BOT_LOG("pfnSetOrigin", (""));
	(*g_engfuncs.pfnSetOrigin)(e, rgflOrigin);
}
void pfnEmitSound(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch)
{
	BOT_LOG("pfnEmitSound", ("channel=%d, sample=%s", channel, sample));
	(*g_engfuncs.pfnEmitSound)(entity, channel, sample, volume, attenuation, fFlags, pitch);
}
void pfnEmitAmbientSound(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch)
{
	BOT_LOG("pfnEmitAmbientSound", (""));
	(*g_engfuncs.pfnEmitAmbientSound)(entity, pos, samp, vol, attenuation, fFlags, pitch);
}
void pfnTraceLine(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr)
{
	//BOT_LOG("pfnTraceLine", (""));
	(*g_engfuncs.pfnTraceLine)(v1, v2, fNoMonsters, pentToSkip, ptr);
}
void pfnTraceToss(edict_t* pent, edict_t* pentToIgnore, TraceResult *ptr)
{
	//BOT_LOG("pfnTraceToss", (""));
	(*g_engfuncs.pfnTraceToss)(pent, pentToIgnore, ptr);
}
int pfnTraceMonsterHull(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr)
{
	//BOT_LOG("pfnTraceMonsterHull", (""));
	return (*g_engfuncs.pfnTraceMonsterHull)(pEdict, v1, v2, fNoMonsters, pentToSkip, ptr);
}
void pfnTraceHull(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr)
{
	//BOT_LOG("pfnTraceHull", (""));
	(*g_engfuncs.pfnTraceHull)(v1, v2, fNoMonsters, hullNumber, pentToSkip, ptr);
}
void pfnTraceModel(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr)
{
	//BOT_LOG("pfnTraceModel", (""));
	(*g_engfuncs.pfnTraceModel)(v1, v2, hullNumber, pent, ptr);
}
const char *pfnTraceTexture(edict_t *pTextureEntity, const float *v1, const float *v2 )
{
	//BOT_LOG("pfnTraceTexture", (""));
	return (*g_engfuncs.pfnTraceTexture)(pTextureEntity, v1, v2);
}
void pfnTraceSphere(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr)
{
	//BOT_LOG("pfnTraceSphere", (""));
	(*g_engfuncs.pfnTraceSphere)(v1, v2, fNoMonsters, radius, pentToSkip, ptr);
}
void pfnGetAimVector(edict_t* ent, float speed, float *rgflReturn)
{
	//BOT_LOG("pfnGetAimVector", (""));
	(*g_engfuncs.pfnGetAimVector)(ent, speed, rgflReturn);
}
void pfnServerCommand(char* str)
{
	BOT_LOG("pfnServerCommand", ("str=%s", str));
	(*g_engfuncs.pfnServerCommand)(str);
}
void pfnServerExecute(void)
{
	BOT_LOG("pfnServerExecute", (""));
	(*g_engfuncs.pfnServerExecute)();
}
#endif /* not USE_METAMOD */
void pfnClientCommand(edict_t* pEdict, char* szFmt, ...)
{
	BOT_LOG("pfnClientCommand", ("szFmt=%s", szFmt));
	
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
	RETURN_META(MRES_IGNORED);
#else /* not USE_METAMOD */
	return;
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
void pfnParticleEffect(const float *org, const float *dir, float color, float count)
{
	//BOT_LOG("pfnParticleEffect", (""));
	(*g_engfuncs.pfnParticleEffect)(org, dir, color, count);
}
void pfnLightStyle(int style, char* val)
{
	//BOT_LOG("pfnLightStyle", (""));
	(*g_engfuncs.pfnLightStyle)(style, val);
}
int pfnDecalIndex(const char *name)
{
	//BOT_LOG("pfnDecalIndex", (""));
	return (*g_engfuncs.pfnDecalIndex)(name);
}
int pfnPointContents(const float *rgflVector)
{
	//BOT_LOG("pfnPointContents", (""));
	return (*g_engfuncs.pfnPointContents)(rgflVector);
}
#endif /* not USE_METAMOD */
void pfnMessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
	if (gpGlobals->deathmatch)
	{
		//BOT_LOG("pfnMessageBegin", ("ed=%x, msg_dest=%d, msg_type=%d", ed, msg_dest, msg_type));
		BOT_LOG("pfnMessageBegin", ("ed=%x, msg_dest=%d, msg_type=%s(%d)", ed, msg_dest, GET_USER_MSG_NAME(PLID, msg_type, NULL), msg_type));

		JBRegMsgs();
		msg_state = 0; // reset state on message begin

		// is this message for a player?
		if (ed)
		{
			botMsgIndex = UTIL_GetBotIndex(ed);

         	// is this message for a bot?
			if (ed->v.flags & FL_THIRDPARTYBOT || botMsgIndex > -1)
				botMsgFunction = botmsgs[msg_type];
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
	RETURN_META(MRES_IGNORED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnMessageBegin)(msg_dest, msg_type, pOrigin, ed);
#endif /* USE_METAMOD */
}

void pfnMessageEnd(void)
{
	if (gpGlobals->deathmatch)
	{
		BOT_LOG("pfnMessageEnd", (""));
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
		{
			(*botMsgFunction)((void *)0, botMsgIndex,_CLIENT_END);
			botMsgFunction = NULL; // clear out the bot message function pointer...
		}

		msg_state = 0; // reset state on message end
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_IGNORED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnMessageEnd)();
#endif /* USE_METAMOD */
}

void pfnWriteByte(int iValue)
{
	if (gpGlobals->deathmatch)
	{
		BOT_LOG("pfnWriteByte", ("iValue=%d", iValue));
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&iValue, botMsgIndex,_CLIENT_BYTE);

		msg_state++; // increment message state
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_IGNORED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnWriteByte)(iValue);
#endif /* USE_METAMOD */
}
void pfnWriteChar(int iValue)
{
	if (gpGlobals->deathmatch)
	{
		BOT_LOG("pfnWriteChar", ("iValue=%d", iValue));
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&iValue, botMsgIndex,_CLIENT_CHAR);

		msg_state++; // increment message state
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_IGNORED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnWriteChar)(iValue);
#endif /* USE_METAMOD */
}
void pfnWriteShort(int iValue)
{
	if (gpGlobals->deathmatch)
	{
		BOT_LOG("prnWriteShort", ("iValue=%d", iValue));
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&iValue, botMsgIndex,_CLIENT_SHORT);

		msg_state++; // increment message state
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_IGNORED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnWriteShort)(iValue);
#endif /* USE_METAMOD */
}
void pfnWriteLong(int iValue)
{
	if (gpGlobals->deathmatch)
	{
		BOT_LOG("pfnWriteLong", ("iValue=%d", iValue));
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&iValue, botMsgIndex,_CLIENT_LONG);

		msg_state++; // increment message state
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_IGNORED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnWriteLong)(iValue);
#endif /* USE_METAMOD */
}
void pfnWriteAngle(float flValue)
{
	if (gpGlobals->deathmatch)
	{
		BOT_LOG("pfnWriteAngle", ("flValue=%f",flValue));
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&flValue, botMsgIndex,_CLIENT_ANGLE);

		msg_state++; // increment message state
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_IGNORED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnWriteAngle)(flValue);
#endif /* USE_METAMOD */
}
void pfnWriteCoord(float flValue)
{
	if (gpGlobals->deathmatch)
	{
		BOT_LOG("pfnWriteCoord", ("flValue=%f", flValue));
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&flValue, botMsgIndex,_CLIENT_COORD);

		msg_state++; // increment message state
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_IGNORED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnWriteCoord)(flValue);
#endif /* USE_METAMOD */
}
void pfnWriteString(const char *sz)
{
	if (gpGlobals->deathmatch)
	{
		BOT_LOG("pfnWriteString", ("sz=%s", sz));
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)sz, botMsgIndex,_CLIENT_STRING);

		msg_state++; // increment message state
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_IGNORED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnWriteString)(sz);
#endif /* USE_METAMOD */
}
void pfnWriteEntity(int iValue)
{
	if (gpGlobals->deathmatch)
	{
		BOT_LOG("pfnWriteEntity", ("iValue=%d", iValue));
		
		// if this message is for a bot, call the client message function...
		if (botMsgFunction)
			(*botMsgFunction)((void *)&iValue, botMsgIndex,_CLIENT_ENTITY);

		msg_state++; // increment message state
	}
	
#ifdef USE_METAMOD
	RETURN_META(MRES_IGNORED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnWriteEntity)(iValue);
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
void pfnCVarRegister(cvar_t *pCvar)
{
	BOT_LOG("pfnCVarRegister", (""));
	(*g_engfuncs.pfnCVarRegister)(pCvar);
}
float pfnCVarGetFloat(const char *szVarName)
{
	//BOT_LOG("pfnCVarGetFloat", ("szVarName=%s", szVarName));
	return (*g_engfuncs.pfnCVarGetFloat)(szVarName);
}
const char* pfnCVarGetString(const char *szVarName)
{
	//BOT_LOG("pfnCVarGetString", (""));
	return (*g_engfuncs.pfnCVarGetString)(szVarName);
}
void pfnCVarSetFloat(const char *szVarName, float flValue)
{
	//BOT_LOG("pfnCVarSetFloat", (""));
	(*g_engfuncs.pfnCVarSetFloat)(szVarName, flValue);
}
void pfnCVarSetString(const char *szVarName, const char *szValue)
{
	//BOT_LOG("pfnCVarSetString", (""));
	(*g_engfuncs.pfnCVarSetString)(szVarName, szValue);
}
void pfnAlertMessage(ALERT_TYPE atype, char *szFmt, ...)
{
	BOT_LOG("pfnAlertMessage", (""));
	va_list		argptr;
	static char		string[1024];

	va_start ( argptr, szFmt );
	vsnprintf ( string, sizeof(string), szFmt, argptr );
	va_end   ( argptr );

	(*g_engfuncs.pfnAlertMessage)(atype, string);
}
void pfnEngineFprintf(FILE *pfile, char *szFmt, ...)
{
	BOT_LOG("pfnEngineFprintf", (""));
	va_list		argptr;
	static char		string[1024];

	va_start ( argptr, szFmt );
	vsnprintf ( string, sizeof(string), szFmt, argptr );
	va_end   ( argptr );

	(*g_engfuncs.pfnEngineFprintf)(pfile, string);
}
void* pfnPvAllocEntPrivateData(edict_t *pEdict, int32 cb)
{
	BOT_LOG("pfnPvAllocEntPrivateData", (""));
	return (*g_engfuncs.pfnPvAllocEntPrivateData)(pEdict, cb);
}
void* pfnPvEntPrivateData(edict_t *pEdict)
{
	BOT_LOG("pfnPvEntPrivateData", (""));
	return (*g_engfuncs.pfnPvEntPrivateData)(pEdict);
}
void pfnFreeEntPrivateData(edict_t *pEdict)
{
	BOT_LOG("pfnFreeEntPrivateData", (""));
	(*g_engfuncs.pfnFreeEntPrivateData)(pEdict);
}
const char* pfnSzFromIndex(int iString)
{
	BOT_LOG("pfnSzFromIndex", (""));
	return (*g_engfuncs.pfnSzFromIndex)(iString);
}
int pfnAllocString(const char *szValue)
{
	BOT_LOG("pfnAllocString", (""));
	return (*g_engfuncs.pfnAllocString)(szValue);
}
entvars_t* pfnGetVarsOfEnt(edict_t *pEdict)
{
	BOT_LOG("pfnGetVarsOfEnt", (""));
	return (*g_engfuncs.pfnGetVarsOfEnt)(pEdict);
}
edict_t* pfnPEntityOfEntOffset(int iEntOffset)
{
	//BOT_LOG("pfnPEntityOfEntOffset", (""));
	return (*g_engfuncs.pfnPEntityOfEntOffset)(iEntOffset);
}
int pfnEntOffsetOfPEntity(const edict_t *pEdict)
{
	//BOT_LOG("pfnEntOffsetOfPEntity", ("pEdict=%x", pEdict));
	return (*g_engfuncs.pfnEntOffsetOfPEntity)(pEdict);
}
int pfnIndexOfEdict(const edict_t *pEdict)
{
	//BOT_LOG("pfnIndexOfEdict", ("pEdict=%x", pEdict));
	return (*g_engfuncs.pfnIndexOfEdict)(pEdict);
}
edict_t* pfnPEntityOfEntIndex(int iEntIndex)
{
	//BOT_LOG("pfnPEntityOfEntIndex", (""));
	return (*g_engfuncs.pfnPEntityOfEntIndex)(iEntIndex);
}
edict_t* pfnFindEntityByVars(entvars_t* pvars)
{
	//BOT_LOG("pfnFindEntityByVars", (""));
	return (*g_engfuncs.pfnFindEntityByVars)(pvars);
}
void* pfnGetModelPtr(edict_t* pEdict)
{
	//BOT_LOG("pfnGetModelPtr", ("pEdict=%x", pEdict));
	return (*g_engfuncs.pfnGetModelPtr)(pEdict);
}
int pfnRegUserMsg(const char *pszName, int iSize)
{
	if (gpGlobals->deathmatch)
	{
		int msg = REG_USER_MSG(pszName, iSize);
#ifdef DEBUGMESSAGES
		BOT_LOG("pfnRegUserMsg", ("pszName=%s, msg_id=%d", pszName, msg));
#endif
		AddUserMsg(pszName, msg, iSize);

		return msg;
	}
   
	return (*g_engfuncs.pfnRegUserMsg)(pszName, iSize);
}
void pfnAnimationAutomove(const edict_t* pEdict, float flTime)
{
	//BOT_LOG("pfnAnimationAutomove", (""));
	(*g_engfuncs.pfnAnimationAutomove)(pEdict, flTime);
}
void pfnGetBonePosition(const edict_t* pEdict, int iBone, float *rgflOrigin, float *rgflAngles )
{
	//BOT_LOG("pfnGetBonePosition", (""));
	(*g_engfuncs.pfnGetBonePosition)(pEdict, iBone, rgflOrigin, rgflAngles);
}
uint32 pfnFunctionFromName( const char *pName )
{
	BOT_LOG("pfnFunctionFromName", (""));
	return (*g_engfuncs.pfnFunctionFromName)(pName);
}
const char *pfnNameForFunction( uint32 function )
{
	BOT_LOG("pfnNameForFunction", (""));
	return (*g_engfuncs.pfnNameForFunction)(function);
}
#endif /* not USE_METAMOD */

void pfnClientPrintf( edict_t* pEdict, PRINT_TYPE ptype, const char *szMsg )
{
	BOT_LOG("pfnClientPrintf", (""));
	if ( pEdict->v.flags & (FL_FAKECLIENT | FL_THIRDPARTYBOT) )
#ifdef USE_METAMOD
		RETURN_META(MRES_SUPERCEDE);

	RETURN_META(MRES_IGNORED);
#else
		return;

	(*g_engfuncs.pfnClientPrintf)(pEdict, ptype, szMsg);
#endif
}

#ifndef USE_METAMOD
void pfnServerPrint( const char *szMsg )
{
	BOT_LOG("pfnServerPrint", (""));
	(*g_engfuncs.pfnServerPrint)(szMsg);
}
void pfnGetAttachment(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles )
{
	//BOT_LOG("pfnGetAttachment", (""));
	(*g_engfuncs.pfnGetAttachment)(pEdict, iAttachment, rgflOrigin, rgflAngles);
}
#endif /* not USE_METAMOD */

const char *pfnCmd_Args( void )
{
	if (isFakeClientCommand)
#ifdef USE_METAMOD
		RETURN_META_VALUE(MRES_SUPERCEDE, &g_argv[0]);

	RETURN_META_VALUE(MRES_IGNORED, NULL);
#else /* not USE_METAMOD */
		return &g_argv[0];

	return (*g_engfuncs.pfnCmd_Args)();
#endif /* USE_METAMOD */
}

const char *pfnCmd_Argv( int argc )
{
	if (isFakeClientCommand)
#ifdef USE_METAMOD
		RETURN_META_VALUE(MRES_SUPERCEDE, GetField (g_argv, argc));

	RETURN_META_VALUE(MRES_IGNORED, NULL);
#else /* not USE_METAMOD */
		return (GetField (g_argv, argc)); // returns the requested argument

	return (*g_engfuncs.pfnCmd_Argv)(argc);
#endif /* USE_METAMOD */
}

int pfnCmd_Argc( void )
{
	if (isFakeClientCommand)
#ifdef USE_METAMOD
		RETURN_META_VALUE(MRES_SUPERCEDE, fake_arg_count);

	RETURN_META_VALUE(MRES_IGNORED, 0);
#else /* not USE_METAMOD */
		return fake_arg_count;

	return (*g_engfuncs.pfnCmd_Argc)();
#endif /* USE_METAMOD */
}

#ifndef USE_METAMOD
void pfnCRC32_Init(CRC32_t *pulCRC)
{
	BOT_LOG("pfnCRC32_Init", (""));
	(*g_engfuncs.pfnCRC32_Init)(pulCRC);
}
void pfnCRC32_ProcessBuffer(CRC32_t *pulCRC, void *p, int len)
{
	BOT_LOG("pfnCRC32_ProcessBuffer", (""));
	(*g_engfuncs.pfnCRC32_ProcessBuffer)(pulCRC, p, len);
}
void pfnCRC32_ProcessByte(CRC32_t *pulCRC, unsigned char ch)
{
	BOT_LOG("pfnCRC32_ProcessByte", (""));
	(*g_engfuncs.pfnCRC32_ProcessByte)(pulCRC, ch);
}
CRC32_t pfnCRC32_Final(CRC32_t pulCRC)
{
	BOT_LOG("pfnCRC32_Final", (""));
	return (*g_engfuncs.pfnCRC32_Final)(pulCRC);
}
int32 pfnRandomLong(int32 lLow, int32 lHigh)
{
	//BOT_LOG("pfnRandomLong", ("lLow=%d, lHigh=%d", lLow, lHigh));
	return (*g_engfuncs.pfnRandomLong)(lLow, lHigh);
}
float pfnRandomFloat(float flLow, float flHigh)
{
	//BOT_LOG("pfnRandomFloat", (""));
	return (*g_engfuncs.pfnRandomFloat)(flLow, flHigh);
}
void pfnSetView(const edict_t *pClient, const edict_t *pViewent )
{
	BOT_LOG("pfnSetView", (""));
	(*g_engfuncs.pfnSetView)(pClient, pViewent);
}
float pfnTime( void )
{
	BOT_LOG("pfnTime", (""));
	return (*g_engfuncs.pfnTime)();
}
void pfnCrosshairAngle(const edict_t *pClient, float pitch, float yaw)
{
	BOT_LOG("pfnCrosshairAngle", (""));
	(*g_engfuncs.pfnCrosshairAngle)(pClient, pitch, yaw);
}
byte *pfnLoadFileForMe(char *filename, int *pLength)
{
	BOT_LOG("pfnLoadFileForMe", ("filename=%s", filename));
	return (*g_engfuncs.pfnLoadFileForMe)(filename, pLength);
}
void pfnFreeFile(void *buffer)
{
	BOT_LOG("pfnFreeFile", (""));
	(*g_engfuncs.pfnFreeFile)(buffer);
}
void pfnEndSection(const char *pszSectionName)
{
	BOT_LOG("pfnEndSection", (""));
	(*g_engfuncs.pfnEndSection)(pszSectionName);
}
int pfnCompareFileTime(char *filename1, char *filename2, int *iCompare)
{
	BOT_LOG("pfnCompareFileTime", (""));
	return (*g_engfuncs.pfnCompareFileTime)(filename1, filename2, iCompare);
}
void pfnGetGameDir(char *szGetGameDir)
{
	BOT_LOG("pfnGetGameDir", (""));
	(*g_engfuncs.pfnGetGameDir)(szGetGameDir);
}
void pfnCvar_RegisterVariable(cvar_t *variable)
{
	BOT_LOG("pfnCvar_RegisterVariable", (""));
	(*g_engfuncs.pfnCvar_RegisterVariable)(variable);
}
void pfnFadeClientVolume(const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds)
{
	BOT_LOG("pfnFadeClientVolume", (""));
	(*g_engfuncs.pfnFadeClientVolume)(pEdict, fadePercent, fadeOutSeconds, holdTime, fadeInSeconds);
}
#endif /* not USE_METAMOD */

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
	
	BOT_LOG("pfnSetClientMaxspeed", ("pEdict=%x, fNewMaxspeed=%f", pEdict, fNewMaxspeed));
#ifdef USE_METAMOD
	RETURN_META(MRES_IGNORED);
#else /* not USE_METAMOD */
	(*g_engfuncs.pfnSetClientMaxspeed)(pEdict, fNewMaxspeed);
#endif /* USE_METAMOD */
}

#ifndef USE_METAMOD
edict_t * pfnCreateFakeClient(const char *netname)
{
	BOT_LOG("pfnCreateFakeClient", (""));
	return (*g_engfuncs.pfnCreateFakeClient)(netname);
}
void pfnRunPlayerMove(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec )
{
	BOT_LOG("pfnRunPlayerMove", (""));
	(*g_engfuncs.pfnRunPlayerMove)(fakeclient, viewangles, forwardmove, sidemove, upmove, buttons, impulse, msec);
}
int pfnNumberOfEntities(void)
{
	BOT_LOG("pfnNumberOfEntities", (""));
	return (*g_engfuncs.pfnNumberOfEntities)();
}
char* pfnGetInfoKeyBuffer(edict_t *e)
{
	BOT_LOG("pfnGetInfoKeyBuffer", (""));
	return (*g_engfuncs.pfnGetInfoKeyBuffer)(e);
}
char* pfnInfoKeyValue(char *infobuffer, char *key)
{
	BOT_LOG("pfnInfoKeyValue", ("infobuffer=%s, key=%s", infobuffer, key));
	return (*g_engfuncs.pfnInfoKeyValue)(infobuffer, key);
}
void pfnSetKeyValue(char *infobuffer, char *key, char *value)
{
	BOT_LOG("pfnSetKeyValue", ("infobuffer=%s, key=%s", key, value));
	(*g_engfuncs.pfnSetKeyValue)(infobuffer, key, value);
}
void pfnSetClientKeyValue(int clientIndex, char *infobuffer, char *key, char *value)
{
	BOT_LOG("pfnSetClientKeyValue", ("key=%s, value=%s", key, value));
	(*g_engfuncs.pfnSetClientKeyValue)(clientIndex, infobuffer, key, value);
}
int pfnIsMapValid(char *filename)
{
	BOT_LOG("pfnIsMapValid", (""));
	return (*g_engfuncs.pfnIsMapValid)(filename);
}
void pfnStaticDecal( const float *origin, int decalIndex, int entityIndex, int modelIndex )
{
	BOT_LOG("pfnStaticDecal", (""));
	(*g_engfuncs.pfnStaticDecal)(origin, decalIndex, entityIndex, modelIndex);
}
int pfnPrecacheGeneric(char* s)
{
	BOT_LOG("pfnPrecacheGeneric", ("s=%s", s));
	return (*g_engfuncs.pfnPrecacheGeneric)(s);
}
#endif /* not USE_METAMOD */

int pfnGetPlayerUserId(edict_t *e )
{
	if (gpGlobals->deathmatch)
	{
		BOT_LOG("pfnGetPlayerUserId", ("e=%x", e));
		
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
	RETURN_META_VALUE(MRES_IGNORED, 0);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnGetPlayerUserId)(e);
#endif /* USE_METAMOD */
}

#ifndef USE_METAMOD
void pfnBuildSoundMsg(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
	BOT_LOG("pfnBuildSoundMsg", (""));
	(*g_engfuncs.pfnBuildSoundMsg)(entity, channel, sample, volume, attenuation, fFlags, pitch, msg_dest, msg_type, pOrigin, ed);
}
int pfnIsDedicatedServer(void)
{
	BOT_LOG("pfnIsDedicatedServer", (""));
	return (*g_engfuncs.pfnIsDedicatedServer)();
}
cvar_t* pfnCVarGetPointer(const char *szVarName)
{
	BOT_LOG("pfnCVarGetPointer", ("szVarName=%s", szVarName));
	return (*g_engfuncs.pfnCVarGetPointer)(szVarName);
}
unsigned int pfnGetPlayerWONId(edict_t *e)
{
	BOT_LOG("pfnGetPlayerWONId", ("e=%x", e));
	return (*g_engfuncs.pfnGetPlayerWONId)(e);
}

// new stuff for SDK 2.0

void pfnInfo_RemoveKey(char *s, const char *key)
{
	BOT_LOG("pfnInfo_RemoveKey", (""));
	(*g_engfuncs.pfnInfo_RemoveKey)(s, key);
}
const char *pfnGetPhysicsKeyValue(const edict_t *pClient, const char *key)
{
	BOT_LOG("pfnGetPhysicsKeyValue", (""));
	return (*g_engfuncs.pfnGetPhysicsKeyValue)(pClient, key);
}
void pfnSetPhysicsKeyValue(const edict_t *pClient, const char *key, const char *value)
{
	BOT_LOG("pfnSetPhysicsKeyValue", (""));
	(*g_engfuncs.pfnSetPhysicsKeyValue)(pClient, key, value);
}
const char *pfnGetPhysicsInfoString(const edict_t *pClient)
{
	BOT_LOG("pfnGetPhysicsInfoString", (""));
	return (*g_engfuncs.pfnGetPhysicsInfoString)(pClient);
}
unsigned short pfnPrecacheEvent(int type, const char *psz)
{
	BOT_LOG("pfnPrecacheEvent", (""));
	return (*g_engfuncs.pfnPrecacheEvent)(type, psz);
}
void pfnPlaybackEvent(int flags, const edict_t *pInvoker, unsigned short eventindex, float delay,
					  float *origin, float *angles, float fparam1,float fparam2, int iparam1, int iparam2, int bparam1, int bparam2)
{
	BOT_LOG("pfnPlaybackEvent", (""));
	(*g_engfuncs.pfnPlaybackEvent)(flags, pInvoker, eventindex, delay, origin, angles, fparam1, fparam2, iparam1, iparam2, bparam1, bparam2);
}
unsigned char *pfnSetFatPVS(float *org)
{
	BOT_LOG("pfnSetFatPVS", (""));
	return (*g_engfuncs.pfnSetFatPVS)(org);
}
unsigned char *pfnSetFatPAS(float *org)
{
	BOT_LOG("pfnSetFatPAS", (""));
	return (*g_engfuncs.pfnSetFatPAS)(org);
}
int pfnCheckVisibility(const edict_t *entity, unsigned char *pset)
{
	//BOT_LOG("pfnCheckVisibility", (""));
	return (*g_engfuncs.pfnCheckVisibility)(entity, pset);
}
void pfnDeltaSetField(struct delta_s *pFields, const char *fieldname)
{
	//BOT_LOG("pfnDeltaSetField", (""));
	(*g_engfuncs.pfnDeltaSetField)(pFields, fieldname);
}
void pfnDeltaUnsetField(struct delta_s *pFields, const char *fieldname)
{
	//BOT_LOG("pfnDeltaUnsetField", (""));
	(*g_engfuncs.pfnDeltaUnsetField)(pFields, fieldname);
}
void pfnDeltaAddEncoder(char *name, void (*conditionalencode)( struct delta_s *pFields, const unsigned char *from, const unsigned char *to))
{
	//BOT_LOG("pfnDeltaAddEncoder", (""));
	(*g_engfuncs.pfnDeltaAddEncoder)(name, conditionalencode);
}
int pfnGetCurrentPlayer(void)
{
	BOT_LOG("pfnGetCurrentPlayer", (""));
	return (*g_engfuncs.pfnGetCurrentPlayer)();
}
int pfnCanSkipPlayer(const edict_t *player)
{
	BOT_LOG("pfnCanSkipPlayer", (""));
	return (*g_engfuncs.pfnCanSkipPlayer)(player);
}
int pfnDeltaFindField(struct delta_s *pFields, const char *fieldname)
{
	//BOT_LOG("pfnDeltaFindField", (""));
	return (*g_engfuncs.pfnDeltaFindField)(pFields, fieldname);
}
void pfnDeltaSetFieldByIndex(struct delta_s *pFields, int fieldNumber)
{
	BOT_LOG("pfnDeltaSetFieldByIndex", (""));
	(*g_engfuncs.pfnDeltaSetFieldByIndex)(pFields, fieldNumber);
}
void pfnDeltaUnsetFieldByIndex(struct delta_s *pFields, int fieldNumber)
{
	//BOT_LOG("pfnDeltaUnsetFieldByIndex", (""));
	(*g_engfuncs.pfnDeltaUnsetFieldByIndex)(pFields, fieldNumber);
}
void pfnSetGroupMask(int mask, int op)
{
	BOT_LOG("pfnSetGroupMask", (""));
	(*g_engfuncs.pfnSetGroupMask)(mask, op);
}
int pfnCreateInstancedBaseline(int classname, struct entity_state_s *baseline)
{
	BOT_LOG("pfnCreateInstancedBaseline", (""));
	return (*g_engfuncs.pfnCreateInstancedBaseline)(classname, baseline);
}
void pfnCvar_DirectSet(struct cvar_s *var, char *value)
{
	BOT_LOG("pfnCvar_DirectSet", (""));
	(*g_engfuncs.pfnCvar_DirectSet)(var, value);
}
void pfnForceUnmodified(FORCE_TYPE type, float *mins, float *maxs, const char *filename)
{
	BOT_LOG("pfnForceUnmodified", (""));
	(*g_engfuncs.pfnForceUnmodified)(type, mins, maxs, filename);
}
void pfnGetPlayerStats(const edict_t *pClient, int *ping, int *packet_loss)
{
	BOT_LOG("pfnGetPlayerStats", (""));
	(*g_engfuncs.pfnGetPlayerStats)(pClient, ping, packet_loss);
}
void pfnAddServerCommand(char *cmd_name, void (*function)(void))
{
   BOT_LOG("pfnAddServerCommand", ("cmd_name=%s, function=%x", cmd_name, function));
   (*g_engfuncs.pfnAddServerCommand)(cmd_name, function);
}

// hl1108
qboolean pfnVoice_GetClientListening(int iReceiver, int iSender)
{
	return (*g_engfuncs.pfnVoice_GetClientListening)(iReceiver, iSender); 
}
qboolean pfnVoice_SetClientListening(int iReceiver, int iSender, qboolean bListen)
{
	return (*g_engfuncs.pfnVoice_SetClientListening)(iReceiver, iSender, bListen); 
}
#endif /* not USE_METAMOD */
// hl1109
const char *pfnGetPlayerAuthId(edict_t *e)
{
	if (e->v.flags & (FL_FAKECLIENT | FL_THIRDPARTYBOT))
#ifdef USE_METAMOD
		RETURN_META_VALUE(MRES_SUPERCEDE, "0");
#else /* not USE_METAMOD */
		return "0";
#endif /* USE_METAMOD */

#ifdef USE_METAMOD
	RETURN_META_VALUE(MRES_IGNORED, NULL);
#else /* not USE_METAMOD */
	return (*g_engfuncs.pfnGetPlayerAuthId)(e); 
#endif /* USE_METAMOD */
}
#ifndef USE_METAMOD
// 2003/11/10
void *pfnSequenceGet(const char* fileName, const char* entryName)
{
	return (*g_engfuncs.pfnSequenceGet)(fileName, entryName);
}
void *pfnSequencePickSentence(const char* groupName, int pickMethod, int *picked)
{
	return (*g_engfuncs.pfnSequencePickSentence)(groupName, pickMethod, picked);
}
int pfnGetFileSize(char *filename)
{
	return (*g_engfuncs.pfnGetFileSize)(filename);
}
unsigned int pfnGetApproxWavePlayLen(const char *filepath)
{
	return (*g_engfuncs.pfnGetApproxWavePlayLen)(filepath);
}
int pfnIsCareerMatch(void)
{
	return (*g_engfuncs.pfnIsCareerMatch)();
}
int pfnGetLocalizedStringLength(const char *label)
{
	return (*g_engfuncs.pfnGetLocalizedStringLength)(label);
}
void pfnRegisterTutorMessageShown(int mid)
{
	(*g_engfuncs.pfnRegisterTutorMessageShown)(mid);
}
int pfnGetTimesTutorMessageShown(int mid)
{
	return (*g_engfuncs.pfnGetTimesTutorMessageShown)(mid);
}
void pfnProcessTutorMessageDecayBuffer(int *buffer, int bufferLength)
{
	(*g_engfuncs.pfnProcessTutorMessageDecayBuffer)(buffer, bufferLength);
}
void pfnConstructTutorMessageDecayBuffer(int *buffer, int bufferLength)
{
	(*g_engfuncs.pfnConstructTutorMessageDecayBuffer)(buffer, bufferLength);
}
void pfnResetTutorMessageDecayData(void)
{
	(*g_engfuncs.pfnResetTutorMessageDecayData)();
}
#endif /* not USE_METAMOD */

#ifdef USE_METAMOD
enginefuncs_t meta_engfuncs = {
	NULL,			// pfnPrecacheModel()
	NULL,			// pfnPrecacheSound()
	NULL,			// pfnSetModel()
	NULL,			// pfnModelIndex()
	NULL,			// pfnModelFrames()

	NULL,			// pfnSetSize()
	pfnChangeLevel,	// pfnChangeLevel()
	NULL,			// pfnGetSpawnParms()
	NULL,			// pfnSaveSpawnParms()

	NULL,			// pfnVecToYaw()
	NULL,			// pfnVecToAngles()
	NULL,			// pfnMoveToOrigin()
	NULL,			// pfnChangeYaw()
	NULL,			// pfnChangePitch()

	pfnFindEntityByString,	// pfnFindEntityByString()
	NULL,			// pfnGetEntityIllum()
	NULL,			// pfnFindEntityInSphere()
	NULL,			// pfnFindClientInPVS()
	NULL,			// pfnEntitiesInPVS()

	NULL,			// pfnMakeVectors()
	NULL,			// pfnAngleVectors()

	NULL,			// pfnCreateEntity()
	NULL,			// pfnRemoveEntity()
	NULL,			// pfnCreateNamedEntity()

	NULL,			// pfnMakeStatic()
	NULL,			// pfnEntIsOnFloor()
	NULL,			// pfnDropToFloor()

	NULL,			// pfnWalkMove()
	NULL,			// pfnSetOrigin()

	NULL,			// pfnEmitSound()
	NULL,			// pfnEmitAmbientSound()

	NULL,			// pfnTraceLine()
	NULL,			// pfnTraceToss()
	NULL,			// pfnTraceMonsterHull()
	NULL,			// pfnTraceHull()
	NULL,			// pfnTraceModel()
	NULL,			// pfnTraceTexture()
	NULL,			// pfnTraceSphere()
	NULL,			// pfnGetAimVector()

	NULL,			// pfnServerCommand()
	NULL,			// pfnServerExecute()
	pfnClientCommand,	// pfnClientCommand()

	NULL,			// pfnParticleEffect()
	NULL,			// pfnLightStyle()
	NULL,			// pfnDecalIndex()
	NULL,			// pfnPointContents()

	pfnMessageBegin,	// pfnMessageBegin()
	pfnMessageEnd,	// pfnMessageEnd()

	pfnWriteByte,	// pfnWriteByte()
	pfnWriteChar,	// pfnWriteChar()
	pfnWriteShort,	// pfnWriteShort()
	pfnWriteLong,	// pfnWriteLong()
	pfnWriteAngle,	// pfnWriteAngle()
	pfnWriteCoord,	// pfnWriteCoord()
	pfnWriteString,	// pfnWriteString()
	pfnWriteEntity,	// pfnWriteEntity()

	NULL,			// pfnCVarRegister()
	NULL,			// pfnCVarGetFloat()
	NULL,			// pfnCVarGetString()
	NULL,			// pfnCVarSetFloat()
	NULL,			// pfnCVarSetString()

	NULL,			// pfnAlertMessage()
	NULL,			// pfnEngineFprintf()

	NULL,			// pfnPvAllocEntPrivateData()
	NULL,			// pfnPvEntPrivateData()
	NULL,			// pfnFreeEntPrivateData()

	NULL,			// pfnSzFromIndex()
	NULL,			// pfnAllocString()

	NULL,		 	// pfnGetVarsOfEnt()
	NULL,			// pfnPEntityOfEntOffset()
	NULL,			// pfnEntOffsetOfPEntity()
	NULL,			// pfnIndexOfEdict()
	NULL,			// pfnPEntityOfEntIndex()
	NULL,			// pfnFindEntityByVars()
	NULL,			// pfnGetModelPtr()

	NULL,			// pfnRegUserMsg()

	NULL,			// pfnAnimationAutomove()
	NULL,			// pfnGetBonePosition()

	NULL,			// pfnFunctionFromName()
	NULL,			// pfnNameForFunction()

	pfnClientPrintf,	// pfnClientPrintf()			//! JOHN: engine callbacks so game DLL can print messages to individual clients
	NULL,			// pfnServerPrint()

	pfnCmd_Args,	// pfnCmd_Args()	//! these 3 added 
	pfnCmd_Argv,	// pfnCmd_Argv()	//! so game DLL can easily 
	pfnCmd_Argc,	// pfnCmd_Argc()	//! access client 'cmd' strings

	NULL,			// pfnGetAttachment()

	NULL,			// pfnCRC32_Init()
	NULL,			// pfnCRC32_ProcessBuffer()
	NULL,			// pfnCRC32_ProcessByte()
	NULL,			// pfnCRC32_Final()

	NULL,			// pfnRandomLong()
	NULL,			// pfnRandomFloat()

	NULL,			// pfnSetView()
	NULL,			// pfnTime()
	NULL,			// pfnCrosshairAngle()

	NULL,			// pfnLoadFileForMe()
	NULL,			// pfnFreeFile()

	NULL,			// pfnEndSection()				//! trigger_endsection
	NULL,			// pfnCompareFileTime()
	NULL,			// pfnGetGameDir()
	NULL,			// pfnCvar_RegisterVariable()
	NULL,			// pfnFadeClientVolume()
	pfnSetClientMaxspeed,	// pfnSetClientMaxspeed()
	NULL,			// pfnCreateFakeClient() 		//! returns NULL if fake client can't be created
	NULL,			// pfnRunPlayerMove()
	NULL,			// pfnNumberOfEntities()

	NULL,			// pfnGetInfoKeyBuffer()		//! passing in NULL gets the serverinfo
	NULL,			// pfnInfoKeyValue()
	NULL,			// pfnSetKeyValue()
	NULL,			// pfnSetClientKeyValue()

	NULL,			// pfnIsMapValid()
	NULL,			// pfnStaticDecal()
	NULL,			// pfnPrecacheGeneric()
	pfnGetPlayerUserId,	// pfnGetPlayerUserId()			//! returns the server assigned userid for this player.
	NULL,			// pfnBuildSoundMsg()
	NULL,			// pfnIsDedicatedServer()		//! is this a dedicated server?
	NULL,			// pfnCVarGetPointer()
	NULL,			// pfnGetPlayerWONId()			//! returns the server assigned WONid for this player.

	//! YWB 8/1/99 TFF Physics additions
	NULL,			// pfnInfo_RemoveKey()
	NULL,			// pfnGetPhysicsKeyValue()
	NULL,			// pfnSetPhysicsKeyValue()
	NULL,			// pfnGetPhysicsInfoString()
	NULL,			// pfnPrecacheEvent()
	NULL,			// pfnPlaybackEvent()

	NULL,			// pfnSetFatPVS()
	NULL,			// pfnSetFatPAS()

	NULL,			// pfnCheckVisibility()

	NULL,			// pfnDeltaSetField()
	NULL,			// pfnDeltaUnsetField()
	NULL,			// pfnDeltaAddEncoder()
	NULL,			// pfnGetCurrentPlayer()
	NULL,			// pfnCanSkipPlayer()
	NULL,			// pfnDeltaFindField()
	NULL,			// pfnDeltaSetFieldByIndex()
	NULL,			// pfnDeltaUnsetFieldByIndex()

	NULL,			// pfnSetGroupMask()

	NULL,			// pfnCreateInstancedBaseline()		// d'oh, CreateInstancedBaseline in dllapi too
	NULL,			// pfnCvar_DirectSet()

	NULL,			// pfnForceUnmodified()

	NULL,			// pfnGetPlayerStats()

	NULL,			// pfnAddServerCommand()

	NULL,			// pfnVoice_GetClientListening()
	NULL,			// pfnVoice_SetClientListening()
	pfnGetPlayerAuthId,	// pfnGetPlayerAuthId()

	NULL,			// pfnSequenceGet()
	NULL,			// pfnSequencePickSentence()
	NULL,			// pfnGetFileSize()
	NULL,			// pfnGetApproxWavePlayLen()
	NULL,			// pfnIsCareerMatch()
	NULL,			// pfnGetLocalizedStringLength()
	NULL,			// pfnRegisterTutorMessageShown()
	NULL,			// pfnGetTimesTutorMessageShown()
	NULL,			// pfnProcessTutorMessageDecayBuffer()
	NULL,			// pfnConstructTutorMessageDecayBuffer()
	NULL,			// pfnResetTutorMessageDecayData()
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
#endif /* USE_METAMOD */
