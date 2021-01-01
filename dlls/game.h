/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#pragma once
#ifndef GAME_H
#define GAME_H

#include "cvardef.h"

extern void GameDLLInit( void );

extern cvar_t displaysoundlist;

// multiplayer server rules
extern cvar_t teamplay;
extern cvar_t fraglimit;
extern cvar_t timelimit;
extern cvar_t friendlyfire;
extern cvar_t smartfriendlyfire;
extern cvar_t falldamage;
extern cvar_t weaponstay;

extern cvar_t weapon_respawndelay;
extern cvar_t ammo_respawndelay;
extern cvar_t item_respawndelay;
extern cvar_t healthcharger_rechargetime;
extern cvar_t hevcharger_rechargetime;

extern cvar_t selfgauss;
extern cvar_t satchelfix;
extern cvar_t allowgaussjump;

extern cvar_t monsteryawspeedfix;
extern cvar_t forcerespawn;
extern cvar_t respawndelay;
extern cvar_t flashlight;
extern cvar_t aimcrosshair;
extern cvar_t decalfrequency;
extern cvar_t teamlist;
extern cvar_t teamoverride;
extern cvar_t defaultteam;

extern cvar_t allowmonsters;
extern cvar_t allowmonsterinfo;
extern cvar_t npc_dropweapons;
extern cvar_t dmgperscore;
extern cvar_t allydmgpenalty;
extern cvar_t npckill;

extern cvar_t keepinventory;

extern cvar_t survival;
extern cvar_t survival_warmup_time;
extern cvar_t survival_restart_number;

extern cvar_t char_phrases;

extern cvar_t minpanicdelay;
extern cvar_t defaultpanicdelay;

extern cvar_t l4m_weapon_system;
extern cvar_t speed_degradation;
extern cvar_t barnacle_paralyze;

extern cvar_t mp_semclip;

// Engine Cvars
extern cvar_t *g_psv_gravity;
extern cvar_t *g_psv_aim;
extern cvar_t *g_footsteps;

extern cvar_t *g_psv_developer;
#endif // GAME_H
