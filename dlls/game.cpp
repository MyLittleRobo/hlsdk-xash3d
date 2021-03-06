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
#include "extdll.h"
#include "eiface.h"
#include "util.h"
#include "game.h"
#include "mod_features.h"

BOOL		g_fIsXash3D;

cvar_t displaysoundlist = {"displaysoundlist","0"};

// multiplayer server rules
cvar_t fragsleft	= { "mp_fragsleft","0", FCVAR_SERVER | FCVAR_UNLOGGED };	  // Don't spam console/log files/users with this changing
cvar_t timeleft		= { "mp_timeleft","0" , FCVAR_SERVER | FCVAR_UNLOGGED };	  // "      "

// multiplayer server rules
cvar_t teamplay		= { "mp_teamplay","0", FCVAR_SERVER };
cvar_t fraglimit	= { "mp_fraglimit","0", FCVAR_SERVER };
cvar_t timelimit	= { "mp_timelimit","0", FCVAR_SERVER };
cvar_t friendlyfire	= { "mp_friendlyfire","0", FCVAR_SERVER };
cvar_t falldamage	= { "mp_falldamage","0", FCVAR_SERVER };
cvar_t weaponstay	= { "mp_weaponstay","0", FCVAR_SERVER };

cvar_t weapon_respawndelay = { "mp_weapon_respawndelay","-2",FCVAR_SERVER };
cvar_t ammo_respawndelay = { "mp_ammo_respawndelay","-2",FCVAR_SERVER };
cvar_t item_respawndelay = { "mp_item_respawndelay","-2",FCVAR_SERVER };
cvar_t healthcharger_rechargetime = { "mp_healthcharger_rechargetime","-2",FCVAR_SERVER };
cvar_t hevcharger_rechargetime = { "mp_hevcharger_rechargetime","-2",FCVAR_SERVER };

cvar_t selfgauss	= { "selfgauss", "1", FCVAR_SERVER };
cvar_t satchelfix	= { "satchelfix", "0", FCVAR_SERVER };
cvar_t monsteryawspeedfix	= { "monsteryawspeedfix", "1", FCVAR_SERVER };
cvar_t forcerespawn	= { "mp_forcerespawn","1", FCVAR_SERVER };
cvar_t respawndelay	= { "mp_respawndelay","0", FCVAR_SERVER };
cvar_t flashlight	= { "mp_flashlight","0", FCVAR_SERVER };
cvar_t aimcrosshair	= { "mp_autocrosshair","1", FCVAR_SERVER };
cvar_t decalfrequency	= { "decalfrequency","30", FCVAR_SERVER };
cvar_t teamlist		= { "mp_teamlist","hgrunt;scientist", FCVAR_SERVER };
cvar_t teamoverride	= { "mp_teamoverride","1" };
cvar_t defaultteam	= { "mp_defaultteam","0" };

cvar_t allowmonsters	= { "mp_allowmonsters","0", FCVAR_SERVER };
cvar_t allowmonsterinfo = { "mp_allowmonsterinfo","0", FCVAR_SERVER };
cvar_t npc_dropweapons = { "npc_dropweapons", "1", FCVAR_SERVER };
cvar_t dmgperscore = { "mp_dmgperscore", "0", FCVAR_SERVER };
cvar_t allydmgpenalty = { "mp_allydmgpenalty", "2", FCVAR_SERVER };
cvar_t npckill = { "mp_npckill", "1", FCVAR_SERVER };

cvar_t bhopcap		= { "mp_bhopcap", "1", FCVAR_SERVER };

cvar_t allow_spectators = { "allow_spectators", "0", FCVAR_SERVER };	// 0 prevents players from being spectators
cvar_t multibyte_only = { "mp_multibyte_only", "0", FCVAR_SERVER };

#if FEATURE_USE_THROUGH_WALLS_CVAR
cvar_t use_through_walls = { "use_through_walls", "1", FCVAR_SERVER };
#endif
#if FEATURE_TRIDEPTH_CVAR
cvar_t tridepth = { "tridepth", "1", FCVAR_SERVER };
#endif
#if FEATURE_NPC_NEAREST_CVAR
cvar_t npc_nearest = { "npc_nearest", "0", FCVAR_SERVER };
#endif
#if FEATURE_NPC_FORGET_ENEMY_CVAR
cvar_t npc_forget_enemy_time = { "npc_forget_enemy_time", "0", FCVAR_SERVER };
#endif
#if FEATURE_NPC_FIX_MELEE_DISTANCE_CVAR
cvar_t npc_fix_melee_distance = { "npc_fix_melee_distance", "0", FCVAR_SERVER };
#endif
#if FEATURE_NPC_ACTIVE_AFTER_COMBAT_CVAR
cvar_t npc_active_after_combat = { "npc_active_after_combat", "0", FCVAR_SERVER };
#endif
cvar_t npc_patrol = { "npc_patrol", "1", FCVAR_SERVER };

cvar_t mp_chattime	= { "mp_chattime","10", FCVAR_SERVER };

#if FEATURE_USE_TO_TAKE_CVAR
cvar_t use_to_take = { "use_to_take","0", FCVAR_SERVER };
#endif
#if FEATURE_GRENADE_JUMP_CVAR
cvar_t grenade_jump = { "grenade_jump","1", FCVAR_SERVER };
#endif

cvar_t keepinventory	= { "mp_keepinventory","0", FCVAR_SERVER }; // keep inventory across level transitions in multiplayer coop

// Engine Cvars
cvar_t *g_psv_gravity = NULL;
cvar_t *g_psv_aim = NULL;
cvar_t *g_footsteps = NULL;

cvar_t *g_psv_developer;

//CVARS FOR SKILL LEVEL SETTINGS
// Agrunt
cvar_t	sk_agrunt_health1 = {"sk_agrunt_health1","0"};
cvar_t	sk_agrunt_health2 = {"sk_agrunt_health2","0"};
cvar_t	sk_agrunt_health3 = {"sk_agrunt_health3","0"};

cvar_t	sk_agrunt_dmg_punch1 = {"sk_agrunt_dmg_punch1","0"};
cvar_t	sk_agrunt_dmg_punch2 = {"sk_agrunt_dmg_punch2","0"};
cvar_t	sk_agrunt_dmg_punch3 = {"sk_agrunt_dmg_punch3","0"};

// Apache
cvar_t	sk_apache_health1 = {"sk_apache_health1","0"};
cvar_t	sk_apache_health2 = {"sk_apache_health2","0"};
cvar_t	sk_apache_health3 = {"sk_apache_health3","0"};


#if FEATURE_BABYGARG
// Baby Gargantua
cvar_t	sk_babygargantua_health1 = {"sk_babygargantua_health1","0"};
cvar_t	sk_babygargantua_health2 = {"sk_babygargantua_health2","0"};
cvar_t	sk_babygargantua_health3 = {"sk_babygargantua_health3","0"};

cvar_t	sk_babygargantua_dmg_slash1	= {"sk_babygargantua_dmg_slash1","0"};
cvar_t	sk_babygargantua_dmg_slash2	= {"sk_babygargantua_dmg_slash2","0"};
cvar_t	sk_babygargantua_dmg_slash3	= {"sk_babygargantua_dmg_slash3","0"};

cvar_t	sk_babygargantua_dmg_fire1 = {"sk_babygargantua_dmg_fire1","0"};
cvar_t	sk_babygargantua_dmg_fire2 = {"sk_babygargantua_dmg_fire2","0"};
cvar_t	sk_babygargantua_dmg_fire3 = {"sk_babygargantua_dmg_fire3","0"};

cvar_t	sk_babygargantua_dmg_stomp1	= {"sk_babygargantua_dmg_stomp1","0"};
cvar_t	sk_babygargantua_dmg_stomp2	= {"sk_babygargantua_dmg_stomp2","0"};
cvar_t	sk_babygargantua_dmg_stomp3	= {"sk_babygargantua_dmg_stomp3","0"};
#endif

// Barnacle
cvar_t sk_barnacle_health1 = {"sk_barnacle_health1", "25"};
cvar_t sk_barnacle_health2 = {"sk_barnacle_health2", "25"};
cvar_t sk_barnacle_health3 = {"sk_barnacle_health3", "25"};

// Barney
cvar_t	sk_barney_health1 = {"sk_barney_health1","0"};
cvar_t	sk_barney_health2 = {"sk_barney_health2","0"};
cvar_t	sk_barney_health3 = {"sk_barney_health3","0"};

// Bullsquid
cvar_t	sk_bullsquid_health1 = {"sk_bullsquid_health1","0"};
cvar_t	sk_bullsquid_health2 = {"sk_bullsquid_health2","0"};
cvar_t	sk_bullsquid_health3 = {"sk_bullsquid_health3","0"};

cvar_t	sk_bullsquid_dmg_bite1 = {"sk_bullsquid_dmg_bite1","0"};
cvar_t	sk_bullsquid_dmg_bite2 = {"sk_bullsquid_dmg_bite2","0"};
cvar_t	sk_bullsquid_dmg_bite3 = {"sk_bullsquid_dmg_bite3","0"};

cvar_t	sk_bullsquid_dmg_whip1 = {"sk_bullsquid_dmg_whip1","0"};
cvar_t	sk_bullsquid_dmg_whip2 = {"sk_bullsquid_dmg_whip2","0"};
cvar_t	sk_bullsquid_dmg_whip3 = {"sk_bullsquid_dmg_whip3","0"};

cvar_t	sk_bullsquid_dmg_spit1 = {"sk_bullsquid_dmg_spit1","0"};
cvar_t	sk_bullsquid_dmg_spit2 = {"sk_bullsquid_dmg_spit2","0"};
cvar_t	sk_bullsquid_dmg_spit3 = {"sk_bullsquid_dmg_spit3","0"};


// Big Momma
cvar_t	sk_bigmomma_health_factor1 = {"sk_bigmomma_health_factor1","1.0"};
cvar_t	sk_bigmomma_health_factor2 = {"sk_bigmomma_health_factor2","1.0"};
cvar_t	sk_bigmomma_health_factor3 = {"sk_bigmomma_health_factor3","1.0"};

cvar_t	sk_bigmomma_dmg_slash1 = {"sk_bigmomma_dmg_slash1","50"};
cvar_t	sk_bigmomma_dmg_slash2 = {"sk_bigmomma_dmg_slash2","50"};
cvar_t	sk_bigmomma_dmg_slash3 = {"sk_bigmomma_dmg_slash3","50"};

cvar_t	sk_bigmomma_dmg_blast1 = {"sk_bigmomma_dmg_blast1","100"};
cvar_t	sk_bigmomma_dmg_blast2 = {"sk_bigmomma_dmg_blast2","100"};
cvar_t	sk_bigmomma_dmg_blast3 = {"sk_bigmomma_dmg_blast3","100"};

cvar_t	sk_bigmomma_radius_blast1 = {"sk_bigmomma_radius_blast1","250"};
cvar_t	sk_bigmomma_radius_blast2 = {"sk_bigmomma_radius_blast2","250"};
cvar_t	sk_bigmomma_radius_blast3 = {"sk_bigmomma_radius_blast3","250"};

#if FEATURE_CLEANSUIT_SCIENTIST
// Cleansuit Scientist
cvar_t	sk_cleansuit_scientist_health1 = {"sk_cleansuit_scientist_health1","0"};
cvar_t	sk_cleansuit_scientist_health2 = {"sk_cleansuit_scientist_health2","0"};
cvar_t	sk_cleansuit_scientist_health3 = {"sk_cleansuit_scientist_health3","0"};
#endif

// Gargantua
cvar_t	sk_gargantua_health1 = {"sk_gargantua_health1","0"};
cvar_t	sk_gargantua_health2 = {"sk_gargantua_health2","0"};
cvar_t	sk_gargantua_health3 = {"sk_gargantua_health3","0"};

cvar_t	sk_gargantua_dmg_slash1	= {"sk_gargantua_dmg_slash1","0"};
cvar_t	sk_gargantua_dmg_slash2	= {"sk_gargantua_dmg_slash2","0"};
cvar_t	sk_gargantua_dmg_slash3	= {"sk_gargantua_dmg_slash3","0"};

cvar_t	sk_gargantua_dmg_fire1 = {"sk_gargantua_dmg_fire1","0"};
cvar_t	sk_gargantua_dmg_fire2 = {"sk_gargantua_dmg_fire2","0"};
cvar_t	sk_gargantua_dmg_fire3 = {"sk_gargantua_dmg_fire3","0"};

cvar_t	sk_gargantua_dmg_stomp1	= {"sk_gargantua_dmg_stomp1","0"};
cvar_t	sk_gargantua_dmg_stomp2	= {"sk_gargantua_dmg_stomp2","0"};
cvar_t	sk_gargantua_dmg_stomp3	= {"sk_gargantua_dmg_stomp3","0"};


// Hassassin
cvar_t	sk_hassassin_health1 = {"sk_hassassin_health1","0"};
cvar_t	sk_hassassin_health2 = {"sk_hassassin_health2","0"};
cvar_t	sk_hassassin_health3 = {"sk_hassassin_health3","0"};


// Headcrab
cvar_t	sk_headcrab_health1 = {"sk_headcrab_health1","0"};
cvar_t	sk_headcrab_health2 = {"sk_headcrab_health2","0"};
cvar_t	sk_headcrab_health3 = {"sk_headcrab_health3","0"};

cvar_t	sk_headcrab_dmg_bite1 = {"sk_headcrab_dmg_bite1","0"};
cvar_t	sk_headcrab_dmg_bite2 = {"sk_headcrab_dmg_bite2","0"};
cvar_t	sk_headcrab_dmg_bite3 = {"sk_headcrab_dmg_bite3","0"};

#if FEATURE_OPFOR_GRUNT
// Opposing Force Hgrunt
cvar_t	sk_hgrunt_ally_health1 = {"sk_hgrunt_ally_health1","0"};
cvar_t	sk_hgrunt_ally_health2 = {"sk_hgrunt_ally_health2","0"};
cvar_t	sk_hgrunt_ally_health3 = {"sk_hgrunt_ally_health3","0"};

cvar_t	sk_hgrunt_ally_kick1 = {"sk_hgrunt_ally_kick1","0"};
cvar_t	sk_hgrunt_ally_kick2 = {"sk_hgrunt_ally_kick2","0"};
cvar_t	sk_hgrunt_ally_kick3 = {"sk_hgrunt_ally_kick3","0"};

cvar_t	sk_hgrunt_ally_pellets1 = {"sk_hgrunt_ally_pellets1","0"};
cvar_t	sk_hgrunt_ally_pellets2 = {"sk_hgrunt_ally_pellets2","0"};
cvar_t	sk_hgrunt_ally_pellets3 = {"sk_hgrunt_ally_pellets3","0"};

cvar_t	sk_hgrunt_ally_gspeed1 = {"sk_hgrunt_ally_gspeed1","0"};
cvar_t	sk_hgrunt_ally_gspeed2 = {"sk_hgrunt_ally_gspeed2","0"};
cvar_t	sk_hgrunt_ally_gspeed3 = {"sk_hgrunt_ally_gspeed3","0"};

// Medic
cvar_t	sk_medic_ally_health1 = {"sk_medic_ally_health1","0"};
cvar_t	sk_medic_ally_health2 = {"sk_medic_ally_health2","0"};
cvar_t	sk_medic_ally_health3 = {"sk_medic_ally_health3","0"};

cvar_t	sk_medic_ally_kick1 = {"sk_medic_ally_kick1","0"};
cvar_t	sk_medic_ally_kick2 = {"sk_medic_ally_kick2","0"};
cvar_t	sk_medic_ally_kick3 = {"sk_medic_ally_kick3","0"};

cvar_t	sk_medic_ally_gspeed1 = {"sk_medic_ally_gspeed1","0"};
cvar_t	sk_medic_ally_gspeed2 = {"sk_medic_ally_gspeed2","0"};
cvar_t	sk_medic_ally_gspeed3 = {"sk_medic_ally_gspeed3","0"};

cvar_t	sk_medic_ally_heal1 = {"sk_medic_ally_heal1","0"};
cvar_t	sk_medic_ally_heal2 = {"sk_medic_ally_heal2","0"};
cvar_t	sk_medic_ally_heal3 = {"sk_medic_ally_heal3","0"};

// Torch
cvar_t	sk_torch_ally_health1 = {"sk_torch_ally_health1","0"};
cvar_t	sk_torch_ally_health2 = {"sk_torch_ally_health2","0"};
cvar_t	sk_torch_ally_health3 = {"sk_torch_ally_health3","0"};

cvar_t	sk_torch_ally_kick1 = {"sk_torch_ally_kick1","0"};
cvar_t	sk_torch_ally_kick2 = {"sk_torch_ally_kick2","0"};
cvar_t	sk_torch_ally_kick3 = {"sk_torch_ally_kick3","0"};

cvar_t	sk_torch_ally_gspeed1 = {"sk_torch_ally_gspeed1","0"};
cvar_t	sk_torch_ally_gspeed2 = {"sk_torch_ally_gspeed2","0"};
cvar_t	sk_torch_ally_gspeed3 = {"sk_torch_ally_gspeed3","0"};
#endif

// Hgrunt 
cvar_t	sk_hgrunt_health1 = {"sk_hgrunt_health1","0"};
cvar_t	sk_hgrunt_health2 = {"sk_hgrunt_health2","0"};
cvar_t	sk_hgrunt_health3 = {"sk_hgrunt_health3","0"};

cvar_t	sk_hgrunt_kick1 = {"sk_hgrunt_kick1","0"};
cvar_t	sk_hgrunt_kick2 = {"sk_hgrunt_kick2","0"};
cvar_t	sk_hgrunt_kick3 = {"sk_hgrunt_kick3","0"};

cvar_t	sk_hgrunt_pellets1 = {"sk_hgrunt_pellets1","0"};
cvar_t	sk_hgrunt_pellets2 = {"sk_hgrunt_pellets2","0"};
cvar_t	sk_hgrunt_pellets3 = {"sk_hgrunt_pellets3","0"};

cvar_t	sk_hgrunt_gspeed1 = {"sk_hgrunt_gspeed1","0"};
cvar_t	sk_hgrunt_gspeed2 = {"sk_hgrunt_gspeed2","0"};
cvar_t	sk_hgrunt_gspeed3 = {"sk_hgrunt_gspeed3","0"};

#if FEATURE_HWGRUNT
cvar_t	sk_hwgrunt_health1 = {"sk_hwgrunt_health1","0"};
cvar_t	sk_hwgrunt_health2 = {"sk_hwgrunt_health2","0"};
cvar_t	sk_hwgrunt_health3 = {"sk_hwgrunt_health3","0"};
#endif

// Houndeye
cvar_t	sk_houndeye_health1 = {"sk_houndeye_health1","0"};
cvar_t	sk_houndeye_health2 = {"sk_houndeye_health2","0"};
cvar_t	sk_houndeye_health3 = {"sk_houndeye_health3","0"};

cvar_t	sk_houndeye_dmg_blast1 = {"sk_houndeye_dmg_blast1","0"};
cvar_t	sk_houndeye_dmg_blast2 = {"sk_houndeye_dmg_blast2","0"};
cvar_t	sk_houndeye_dmg_blast3 = {"sk_houndeye_dmg_blast3","0"};


// ISlave
cvar_t	sk_islave_health1 = {"sk_islave_health1","0"};
cvar_t	sk_islave_health2 = {"sk_islave_health2","0"};
cvar_t	sk_islave_health3 = {"sk_islave_health3","0"};

cvar_t	sk_islave_dmg_claw1 = {"sk_islave_dmg_claw1","0"};
cvar_t	sk_islave_dmg_claw2 = {"sk_islave_dmg_claw2","0"};
cvar_t	sk_islave_dmg_claw3 = {"sk_islave_dmg_claw3","0"};

cvar_t	sk_islave_dmg_clawrake1	= {"sk_islave_dmg_clawrake1","0"};
cvar_t	sk_islave_dmg_clawrake2	= {"sk_islave_dmg_clawrake2","0"};
cvar_t	sk_islave_dmg_clawrake3	= {"sk_islave_dmg_clawrake3","0"};
	
cvar_t	sk_islave_dmg_zap1 = {"sk_islave_dmg_zap1","0"};
cvar_t	sk_islave_dmg_zap2 = {"sk_islave_dmg_zap2","0"};
cvar_t	sk_islave_dmg_zap3 = {"sk_islave_dmg_zap3","0"};

cvar_t sk_islave_zap_rate1 = {"sk_islave_zap_rate1", "1"};
cvar_t sk_islave_zap_rate2 = {"sk_islave_zap_rate2", "1"};
cvar_t sk_islave_zap_rate3 = {"sk_islave_zap_rate3", "1.5"};

// Icthyosaur
cvar_t	sk_ichthyosaur_health1	= {"sk_ichthyosaur_health1","0"};
cvar_t	sk_ichthyosaur_health2	= {"sk_ichthyosaur_health2","0"};
cvar_t	sk_ichthyosaur_health3	= {"sk_ichthyosaur_health3","0"};

cvar_t	sk_ichthyosaur_shake1	= {"sk_ichthyosaur_shake1","0"};
cvar_t	sk_ichthyosaur_shake2	= {"sk_ichthyosaur_shake2","0"};
cvar_t	sk_ichthyosaur_shake3	= {"sk_ichthyosaur_shake3","0"};


// Leech
cvar_t	sk_leech_health1 = {"sk_leech_health1","0"};
cvar_t	sk_leech_health2 = {"sk_leech_health2","0"};
cvar_t	sk_leech_health3 = {"sk_leech_health3","0"};

cvar_t	sk_leech_dmg_bite1 = {"sk_leech_dmg_bite1","0"};
cvar_t	sk_leech_dmg_bite2 = {"sk_leech_dmg_bite2","0"};
cvar_t	sk_leech_dmg_bite3 = {"sk_leech_dmg_bite3","0"};

// Controller
cvar_t	sk_controller_health1 = {"sk_controller_health1","0"};
cvar_t	sk_controller_health2 = {"sk_controller_health2","0"};
cvar_t	sk_controller_health3 = {"sk_controller_health3","0"};

cvar_t	sk_controller_dmgzap1 = {"sk_controller_dmgzap1","0"};
cvar_t	sk_controller_dmgzap2 = {"sk_controller_dmgzap2","0"};
cvar_t	sk_controller_dmgzap3 = {"sk_controller_dmgzap3","0"};

cvar_t	sk_controller_speedball1 = {"sk_controller_speedball1","0"};
cvar_t	sk_controller_speedball2 = {"sk_controller_speedball2","0"};
cvar_t	sk_controller_speedball3 = {"sk_controller_speedball3","0"};

cvar_t	sk_controller_dmgball1 = {"sk_controller_dmgball1","0"};
cvar_t	sk_controller_dmgball2 = {"sk_controller_dmgball2","0"};
cvar_t	sk_controller_dmgball3 = {"sk_controller_dmgball3","0"};

#if FEATURE_MASSN
// Massassin
cvar_t	sk_massassin_health1 = {"sk_massassin_health1","0"};
cvar_t	sk_massassin_health2 = {"sk_massassin_health2","0"};
cvar_t	sk_massassin_health3 = {"sk_massassin_health3","0"};

cvar_t	sk_massassin_kick1 = {"sk_massassin_kick1","0"};
cvar_t	sk_massassin_kick2 = {"sk_massassin_kick2","0"};
cvar_t	sk_massassin_kick3 = {"sk_massassin_kick3","0"};

cvar_t	sk_massassin_gspeed1 = {"sk_massassin_gspeed1","0"};
cvar_t	sk_massassin_gspeed2 = {"sk_massassin_gspeed2","0"};
cvar_t	sk_massassin_gspeed3 = {"sk_massassin_gspeed3","0"};
#endif

// Nihilanth
cvar_t	sk_nihilanth_health1 = {"sk_nihilanth_health1","0"};
cvar_t	sk_nihilanth_health2 = {"sk_nihilanth_health2","0"};
cvar_t	sk_nihilanth_health3 = {"sk_nihilanth_health3","0"};

cvar_t	sk_nihilanth_zap1 = {"sk_nihilanth_zap1","0"};
cvar_t	sk_nihilanth_zap2 = {"sk_nihilanth_zap2","0"};
cvar_t	sk_nihilanth_zap3 = {"sk_nihilanth_zap3","0"};

// Osprey
cvar_t	sk_osprey1	= {"sk_osprey1","400"};
cvar_t	sk_osprey2	= {"sk_osprey2","400"};
cvar_t	sk_osprey3	= {"sk_osprey3","400"};

// Blackops Osprey
#if FEATURE_BLACK_OSPREY
cvar_t	sk_blkopsosprey1	= {"sk_blkopsosprey1"};
cvar_t	sk_blkopsosprey2	= {"sk_blkopsosprey2"};
cvar_t	sk_blkopsosprey3	= {"sk_blkopsosprey3"};
#endif

#if FEATURE_OTIS
// Otis
cvar_t	sk_otis_health1	= {"sk_otis_health1","0"};
cvar_t	sk_otis_health2	= {"sk_otis_health2","0"};
cvar_t	sk_otis_health3	= {"sk_otis_health3","0"};
#endif

#if FEATURE_PITDRONE
// Pitdrone
cvar_t	sk_pitdrone_health1 = { "sk_pitdrone_health1", "0" };
cvar_t	sk_pitdrone_health2 = { "sk_pitdrone_health2", "0" };
cvar_t	sk_pitdrone_health3 = { "sk_pitdrone_health3", "0" };

cvar_t	sk_pitdrone_dmg_bite1 = { "sk_pitdrone_dmg_bite1", "0" };
cvar_t	sk_pitdrone_dmg_bite2 = { "sk_pitdrone_dmg_bite2", "0" };
cvar_t	sk_pitdrone_dmg_bite3 = { "sk_pitdrone_dmg_bite3", "0" };

cvar_t	sk_pitdrone_dmg_whip1 = { "sk_pitdrone_dmg_whip1", "0" };
cvar_t	sk_pitdrone_dmg_whip2 = { "sk_pitdrone_dmg_whip2", "0" };
cvar_t	sk_pitdrone_dmg_whip3 = { "sk_pitdrone_dmg_whip3", "0" };

cvar_t	sk_pitdrone_dmg_spit1 = { "sk_pitdrone_dmg_spit1", "0" };
cvar_t	sk_pitdrone_dmg_spit2 = { "sk_pitdrone_dmg_spit2", "0" };
cvar_t	sk_pitdrone_dmg_spit3 = { "sk_pitdrone_dmg_spit3", "0" };
#endif

#if FEATURE_PITWORM
// Pitworm
cvar_t	sk_pitworm_health1 = { "sk_pitworm_health1", "0" };
cvar_t	sk_pitworm_health2 = { "sk_pitworm_health2", "0" };
cvar_t	sk_pitworm_health3 = { "sk_pitworm_health3", "0" };

cvar_t	sk_pitworm_dmg_swipe1 = { "sk_pitworm_dmg_swipe1", "0" };
cvar_t	sk_pitworm_dmg_swipe2 = { "sk_pitworm_dmg_swipe2", "0" };
cvar_t	sk_pitworm_dmg_swipe3 = { "sk_pitworm_dmg_swipe3", "0" };

cvar_t	sk_pitworm_dmg_beam1 = { "sk_pitworm_dmg_beam1", "0" };
cvar_t	sk_pitworm_dmg_beam2 = { "sk_pitworm_dmg_beam2", "0" };
cvar_t	sk_pitworm_dmg_beam3 = { "sk_pitworm_dmg_beam3", "0" };
#endif

#if FEATURE_GENEWORM
// Geneworm
cvar_t	sk_geneworm_health1 = { "sk_geneworm_health1", "0" };
cvar_t	sk_geneworm_health2 = { "sk_geneworm_health2", "0" };
cvar_t	sk_geneworm_health3 = { "sk_geneworm_health3", "0" };

cvar_t	sk_geneworm_dmg_spit1 = { "sk_geneworm_dmg_spit1", "0" };
cvar_t	sk_geneworm_dmg_spit2 = { "sk_geneworm_dmg_spit2", "0" };
cvar_t	sk_geneworm_dmg_spit3 = { "sk_geneworm_dmg_spit3", "0" };

cvar_t	sk_geneworm_dmg_hit1 = { "sk_geneworm_dmg_hit1", "0" };
cvar_t	sk_geneworm_dmg_hit2 = { "sk_geneworm_dmg_hit2", "0" };
cvar_t	sk_geneworm_dmg_hit3 = { "sk_geneworm_dmg_hit3", "0" };
#endif

// Scientist
cvar_t	sk_scientist_health1 = {"sk_scientist_health1","0"};
cvar_t	sk_scientist_health2 = {"sk_scientist_health2","0"};
cvar_t	sk_scientist_health3 = {"sk_scientist_health3","0"};

#if FEATURE_ROBOGRUNT
cvar_t	sk_rgrunt_explode1 = {"sk_rgrunt_explode1","0"};
cvar_t	sk_rgrunt_explode2 = {"sk_rgrunt_explode2","0"};
cvar_t	sk_rgrunt_explode3 = {"sk_rgrunt_explode3","0"};
#endif

#if FEATURE_SHOCKTROOPER
// Shock Roach
cvar_t	sk_shockroach_health1 = { "sk_shockroach_health1", "0" };
cvar_t	sk_shockroach_health2 = { "sk_shockroach_health2", "0" };
cvar_t	sk_shockroach_health3 = { "sk_shockroach_health3", "0" };

cvar_t	sk_shockroach_dmg_bite1 = { "sk_shockroach_dmg_bite1", "0" };
cvar_t	sk_shockroach_dmg_bite2 = { "sk_shockroach_dmg_bite2", "0" };
cvar_t	sk_shockroach_dmg_bite3 = { "sk_shockroach_dmg_bite3", "0" };

cvar_t	sk_shockroach_lifespan1 = { "sk_shockroach_lifespan1", "0" };
cvar_t	sk_shockroach_lifespan2 = { "sk_shockroach_lifespan2", "0" };
cvar_t	sk_shockroach_lifespan3 = { "sk_shockroach_lifespan3", "0" };

// ShockTrooper
cvar_t	sk_shocktrooper_health1 = { "sk_shocktrooper_health1", "0" };
cvar_t	sk_shocktrooper_health2 = { "sk_shocktrooper_health2", "0" };
cvar_t	sk_shocktrooper_health3 = { "sk_shocktrooper_health3", "0" };

cvar_t	sk_shocktrooper_kick1 = { "sk_shocktrooper_kick1", "0" };
cvar_t	sk_shocktrooper_kick2 = { "sk_shocktrooper_kick2", "0" };
cvar_t	sk_shocktrooper_kick3 = { "sk_shocktrooper_kick3", "0" };

cvar_t	sk_shocktrooper_gspeed1 = { "sk_shocktrooper_gspeed1", "0" };
cvar_t	sk_shocktrooper_gspeed2 = { "sk_shocktrooper_gspeed2", "0" };
cvar_t	sk_shocktrooper_gspeed3 = { "sk_shocktrooper_gspeed3", "0" };

cvar_t	sk_shocktrooper_maxcharge1 = { "sk_shocktrooper_maxcharge1", "0" };
cvar_t	sk_shocktrooper_maxcharge2 = { "sk_shocktrooper_maxcharge2", "0" };
cvar_t	sk_shocktrooper_maxcharge3 = { "sk_shocktrooper_maxcharge3", "0" };

cvar_t	sk_shocktrooper_rchgspeed1 = { "sk_shocktrooper_rchgspeed1", "0" };
cvar_t	sk_shocktrooper_rchgspeed2 = { "sk_shocktrooper_rchgspeed2", "0" };
cvar_t	sk_shocktrooper_rchgspeed3 = { "sk_shocktrooper_rchgspeed3", "0" };
#endif

// Snark
cvar_t	sk_snark_health1 = {"sk_snark_health1","0"};
cvar_t	sk_snark_health2 = {"sk_snark_health2","0"};
cvar_t	sk_snark_health3 = {"sk_snark_health3","0"};

cvar_t	sk_snark_dmg_bite1 = {"sk_snark_dmg_bite1","0"};
cvar_t	sk_snark_dmg_bite2 = {"sk_snark_dmg_bite2","0"};
cvar_t	sk_snark_dmg_bite3 = {"sk_snark_dmg_bite3","0"};

cvar_t	sk_snark_dmg_pop1 = {"sk_snark_dmg_pop1","0"};
cvar_t	sk_snark_dmg_pop2 = {"sk_snark_dmg_pop2","0"};
cvar_t	sk_snark_dmg_pop3 = {"sk_snark_dmg_pop3","0"};

#if FEATURE_VOLTIFORE
// Voltigore
cvar_t	sk_voltigore_health1 = { "sk_voltigore_health1", "0" };
cvar_t	sk_voltigore_health2 = { "sk_voltigore_health2", "0" };
cvar_t	sk_voltigore_health3 = { "sk_voltigore_health3", "0" };

cvar_t	sk_voltigore_dmg_punch1 = { "sk_voltigore_dmg_punch1", "0" };
cvar_t	sk_voltigore_dmg_punch2 = { "sk_voltigore_dmg_punch2", "0" };
cvar_t	sk_voltigore_dmg_punch3 = { "sk_voltigore_dmg_punch3", "0" };

cvar_t	sk_voltigore_dmg_beam1 = { "sk_voltigore_dmg_beam1", "0" };
cvar_t	sk_voltigore_dmg_beam2 = { "sk_voltigore_dmg_beam2", "0" };
cvar_t	sk_voltigore_dmg_beam3 = { "sk_voltigore_dmg_beam3", "0" };

cvar_t	sk_voltigore_dmg_explode1 = { "sk_voltigore_dmg_explode1", "0" };
cvar_t	sk_voltigore_dmg_explode2 = { "sk_voltigore_dmg_explode2", "0" };
cvar_t	sk_voltigore_dmg_explode3 = { "sk_voltigore_dmg_explode3", "0" };

// Baby Voltigore
cvar_t	sk_babyvoltigore_health1 = { "sk_babyvoltigore_health1", "0" };
cvar_t	sk_babyvoltigore_health2 = { "sk_babyvoltigore_health2", "0" };
cvar_t	sk_babyvoltigore_health3 = { "sk_babyvoltigore_health3", "0" };

cvar_t	sk_babyvoltigore_dmg_punch1 = { "sk_babyvoltigore_dmg_punch1", "0" };
cvar_t	sk_babyvoltigore_dmg_punch2 = { "sk_babyvoltigore_dmg_punch2", "0" };
cvar_t	sk_babyvoltigore_dmg_punch3 = { "sk_babyvoltigore_dmg_punch3", "0" };
#endif

// Zombie
cvar_t	sk_zombie_health1 = {"sk_zombie_health1","0"};
cvar_t	sk_zombie_health2 = {"sk_zombie_health2","0"};
cvar_t	sk_zombie_health3 = {"sk_zombie_health3","0"};

cvar_t	sk_zombie_dmg_one_slash1 = {"sk_zombie_dmg_one_slash1","0"};
cvar_t	sk_zombie_dmg_one_slash2 = {"sk_zombie_dmg_one_slash2","0"};
cvar_t	sk_zombie_dmg_one_slash3 = {"sk_zombie_dmg_one_slash3","0"};

cvar_t	sk_zombie_dmg_both_slash1 = {"sk_zombie_dmg_both_slash1","0"};
cvar_t	sk_zombie_dmg_both_slash2 = {"sk_zombie_dmg_both_slash2","0"};
cvar_t	sk_zombie_dmg_both_slash3 = {"sk_zombie_dmg_both_slash3","0"};

#if FEATURE_ZOMBIE_BARNEY
// Zombie Barney
cvar_t	sk_zombie_barney_health1 = {"sk_zombie_barney_health1","0"};
cvar_t	sk_zombie_barney_health2 = {"sk_zombie_barney_health2","0"};
cvar_t	sk_zombie_barney_health3 = {"sk_zombie_barney_health3","0"};

cvar_t	sk_zombie_barney_dmg_one_slash1 = {"sk_zombie_barney_dmg_one_slash1","0"};
cvar_t	sk_zombie_barney_dmg_one_slash2 = {"sk_zombie_barney_dmg_one_slash2","0"};
cvar_t	sk_zombie_barney_dmg_one_slash3 = {"sk_zombie_barney_dmg_one_slash3","0"};

cvar_t	sk_zombie_barney_dmg_both_slash1 = {"sk_zombie_barney_dmg_both_slash1","0"};
cvar_t	sk_zombie_barney_dmg_both_slash2 = {"sk_zombie_barney_dmg_both_slash2","0"};
cvar_t	sk_zombie_barney_dmg_both_slash3 = {"sk_zombie_barney_dmg_both_slash3","0"};
#endif

#if FEATURE_ZOMBIE_SOLDIER
// Zombie Soldier
cvar_t	sk_zombie_soldier_health1 = {"sk_zombie_soldier_health1","0"};
cvar_t	sk_zombie_soldier_health2 = {"sk_zombie_soldier_health2","0"};
cvar_t	sk_zombie_soldier_health3 = {"sk_zombie_soldier_health3","0"};

cvar_t	sk_zombie_soldier_dmg_one_slash1 = {"sk_zombie_soldier_dmg_one_slash1","0"};
cvar_t	sk_zombie_soldier_dmg_one_slash2 = {"sk_zombie_soldier_dmg_one_slash2","0"};
cvar_t	sk_zombie_soldier_dmg_one_slash3 = {"sk_zombie_soldier_dmg_one_slash3","0"};

cvar_t	sk_zombie_soldier_dmg_both_slash1 = {"sk_zombie_soldier_dmg_both_slash1","0"};
cvar_t	sk_zombie_soldier_dmg_both_slash2 = {"sk_zombie_soldier_dmg_both_slash2","0"};
cvar_t	sk_zombie_soldier_dmg_both_slash3 = {"sk_zombie_soldier_dmg_both_slash3","0"};
#endif

#if FEATURE_GONOME
// Gonome
cvar_t	sk_gonome_health1 = { "sk_gonome_health1", "0" };
cvar_t	sk_gonome_health2 = { "sk_gonome_health2", "0" };
cvar_t	sk_gonome_health3 = { "sk_gonome_health3", "0" };

cvar_t	sk_gonome_dmg_one_slash1 = { "sk_gonome_dmg_one_slash1", "0" };
cvar_t	sk_gonome_dmg_one_slash2 = { "sk_gonome_dmg_one_slash2", "0" };
cvar_t	sk_gonome_dmg_one_slash3 = { "sk_gonome_dmg_one_slash3", "0" };

cvar_t	sk_gonome_dmg_guts1 = { "sk_gonome_dmg_guts1", "0" };
cvar_t	sk_gonome_dmg_guts2 = { "sk_gonome_dmg_guts2", "0" };
cvar_t	sk_gonome_dmg_guts3 = { "sk_gonome_dmg_guts3", "0" };

cvar_t	sk_gonome_dmg_one_bite1 = { "sk_gonome_dmg_one_bite1", "0" };
cvar_t	sk_gonome_dmg_one_bite2 = { "sk_gonome_dmg_one_bite2", "0" };
cvar_t	sk_gonome_dmg_one_bite3 = { "sk_gonome_dmg_one_bite3", "0" };
#endif

//Turret
cvar_t	sk_turret_health1 = {"sk_turret_health1","0"};
cvar_t	sk_turret_health2 = {"sk_turret_health2","0"};
cvar_t	sk_turret_health3 = {"sk_turret_health3","0"};

// MiniTurret
cvar_t	sk_miniturret_health1 = {"sk_miniturret_health1","0"};
cvar_t	sk_miniturret_health2 = {"sk_miniturret_health2","0"};
cvar_t	sk_miniturret_health3 = {"sk_miniturret_health3","0"};

// Sentry Turret
cvar_t	sk_sentry_health1 = {"sk_sentry_health1","0"};
cvar_t	sk_sentry_health2 = {"sk_sentry_health2","0"};
cvar_t	sk_sentry_health3 = {"sk_sentry_health3","0"};

// PLAYER WEAPONS

// Crowbar whack
cvar_t	sk_plr_crowbar1 = {"sk_plr_crowbar1","0"};
cvar_t	sk_plr_crowbar2 = {"sk_plr_crowbar2","0"};
cvar_t	sk_plr_crowbar3 = {"sk_plr_crowbar3","0"};

// Glock Round
cvar_t	sk_plr_9mm_bullet1 = {"sk_plr_9mm_bullet1","0"};
cvar_t	sk_plr_9mm_bullet2 = {"sk_plr_9mm_bullet2","0"};
cvar_t	sk_plr_9mm_bullet3 = {"sk_plr_9mm_bullet3","0"};

// 357 Round
cvar_t	sk_plr_357_bullet1 = {"sk_plr_357_bullet1","0"};
cvar_t	sk_plr_357_bullet2 = {"sk_plr_357_bullet2","0"};
cvar_t	sk_plr_357_bullet3 = {"sk_plr_357_bullet3","0"};

// MP5 Round
cvar_t	sk_plr_9mmAR_bullet1 = {"sk_plr_9mmAR_bullet1","0"};
cvar_t	sk_plr_9mmAR_bullet2 = {"sk_plr_9mmAR_bullet2","0"};
cvar_t	sk_plr_9mmAR_bullet3 = {"sk_plr_9mmAR_bullet3","0"};


// M203 grenade
cvar_t	sk_plr_9mmAR_grenade1 = {"sk_plr_9mmAR_grenade1","0"};
cvar_t	sk_plr_9mmAR_grenade2 = {"sk_plr_9mmAR_grenade2","0"};
cvar_t	sk_plr_9mmAR_grenade3 = {"sk_plr_9mmAR_grenade3","0"};


// Shotgun buckshot
cvar_t	sk_plr_buckshot1 = {"sk_plr_buckshot1","0"};
cvar_t	sk_plr_buckshot2 = {"sk_plr_buckshot2","0"};
cvar_t	sk_plr_buckshot3 = {"sk_plr_buckshot3","0"};


// Crossbow
cvar_t	sk_plr_xbow_bolt_client1 = {"sk_plr_xbow_bolt_client1","0"};
cvar_t	sk_plr_xbow_bolt_client2 = {"sk_plr_xbow_bolt_client2","0"};
cvar_t	sk_plr_xbow_bolt_client3 = {"sk_plr_xbow_bolt_client3","0"};

cvar_t	sk_plr_xbow_bolt_monster1 = {"sk_plr_xbow_bolt_monster1","0"};
cvar_t	sk_plr_xbow_bolt_monster2 = {"sk_plr_xbow_bolt_monster2","0"};
cvar_t	sk_plr_xbow_bolt_monster3 = {"sk_plr_xbow_bolt_monster3","0"};


// RPG
cvar_t	sk_plr_rpg1 = {"sk_plr_rpg1","0"};
cvar_t	sk_plr_rpg2 = {"sk_plr_rpg2","0"};
cvar_t	sk_plr_rpg3 = {"sk_plr_rpg3","0"};


// Zero Point Generator
cvar_t	sk_plr_gauss1 = {"sk_plr_gauss1","0"};
cvar_t	sk_plr_gauss2 = {"sk_plr_gauss2","0"};
cvar_t	sk_plr_gauss3 = {"sk_plr_gauss3","0"};


// Tau Cannon
cvar_t	sk_plr_egon_narrow1 = {"sk_plr_egon_narrow1","0"};
cvar_t	sk_plr_egon_narrow2 = {"sk_plr_egon_narrow2","0"};
cvar_t	sk_plr_egon_narrow3 = {"sk_plr_egon_narrow3","0"};

cvar_t	sk_plr_egon_wide1 = {"sk_plr_egon_wide1","0"};
cvar_t	sk_plr_egon_wide2 = {"sk_plr_egon_wide2","0"};
cvar_t	sk_plr_egon_wide3 = {"sk_plr_egon_wide3","0"};


// Hand Grendade
cvar_t	sk_plr_hand_grenade1 = {"sk_plr_hand_grenade1","0"};
cvar_t	sk_plr_hand_grenade2 = {"sk_plr_hand_grenade2","0"};
cvar_t	sk_plr_hand_grenade3 = {"sk_plr_hand_grenade3","0"};


// Satchel Charge
cvar_t	sk_plr_satchel1	= {"sk_plr_satchel1","0"};
cvar_t	sk_plr_satchel2	= {"sk_plr_satchel2","0"};
cvar_t	sk_plr_satchel3	= {"sk_plr_satchel3","0"};


// Tripmine
cvar_t	sk_plr_tripmine1 = {"sk_plr_tripmine1","0"};
cvar_t	sk_plr_tripmine2 = {"sk_plr_tripmine2","0"};
cvar_t	sk_plr_tripmine3 = {"sk_plr_tripmine3","0"};

#if FEATURE_DESERT_EAGLE
// Desert Eagle
cvar_t	sk_plr_eagle1 = { "sk_plr_eagle1", "0" };
cvar_t	sk_plr_eagle2 = { "sk_plr_eagle2", "0" };
cvar_t	sk_plr_eagle3 = { "sk_plr_eagle3", "0" };
#endif

#if FEATURE_PIPEWRENCH
// Pipe wrench
cvar_t	sk_plr_pipewrench1 = { "sk_plr_pipewrench1", "0" };
cvar_t	sk_plr_pipewrench2 = { "sk_plr_pipewrench2", "0" };
cvar_t	sk_plr_pipewrench3 = { "sk_plr_pipewrench3", "0" };
#endif

#if FEATURE_KNIFE
// Knife
cvar_t	sk_plr_knife1 = { "sk_plr_knife1", "0" };
cvar_t	sk_plr_knife2 = { "sk_plr_knife2", "0" };
cvar_t	sk_plr_knife3 = { "sk_plr_knife3", "0" };
#endif

#if FEATURE_GRAPPLE
// Grapple
cvar_t	sk_plr_grapple1 = { "sk_plr_grapple1", "0" };
cvar_t	sk_plr_grapple2 = { "sk_plr_grapple2", "0" };
cvar_t	sk_plr_grapple3 = { "sk_plr_grapple3", "0" };
#endif

#if FEATURE_M249
// M249
cvar_t	sk_plr_556_bullet1 = { "sk_plr_556_bullet1", "0" };
cvar_t	sk_plr_556_bullet2 = { "sk_plr_556_bullet2", "0" };
cvar_t	sk_plr_556_bullet3 = { "sk_plr_556_bullet3", "0" };
#endif

#if FEATURE_SNIPERRIFLE
// Sniper rifle
cvar_t	sk_plr_762_bullet1 = { "sk_plr_762_bullet1", "0" };
cvar_t	sk_plr_762_bullet2 = { "sk_plr_762_bullet2", "0" };
cvar_t	sk_plr_762_bullet3 = { "sk_plr_762_bullet3", "0" };
#endif

#if FEATURE_MEDKIT
// Medkit
cvar_t sk_plr_medkitshot1 = {"sk_plr_medkitshot1","10"};
cvar_t sk_plr_medkitshot2 = {"sk_plr_medkitshot2","10"};
cvar_t sk_plr_medkitshot3 = {"sk_plr_medkitshot3","10"};

cvar_t sk_plr_medkittime1 = {"sk_plr_medkittime1","3"};
cvar_t sk_plr_medkittime2 = {"sk_plr_medkittime2","5"};
cvar_t sk_plr_medkittime3 = {"sk_plr_medkittime3","0"};
#endif

// WORLD WEAPONS
cvar_t	sk_12mm_bullet1 = {"sk_12mm_bullet1","0"};
cvar_t	sk_12mm_bullet2 = {"sk_12mm_bullet2","0"};
cvar_t	sk_12mm_bullet3 = {"sk_12mm_bullet3","0"};

cvar_t	sk_9mmAR_bullet1 = {"sk_9mmAR_bullet1","0"};
cvar_t	sk_9mmAR_bullet2 = {"sk_9mmAR_bullet2","0"};
cvar_t	sk_9mmAR_bullet3 = {"sk_9mmAR_bullet3","0"};

cvar_t	sk_9mm_bullet1 = {"sk_9mm_bullet1","0"};
cvar_t	sk_9mm_bullet2 = {"sk_9mm_bullet2","0"};
cvar_t	sk_9mm_bullet3 = {"sk_9mm_bullet3","0"};

cvar_t	sk_357_bullet1 = {"sk_357_bullet1","35"};
cvar_t	sk_357_bullet2 = {"sk_357_bullet2","35"};
cvar_t	sk_357_bullet3 = {"sk_357_bullet3","35"};

cvar_t	sk_556_bullet1 = { "sk_556_bullet1", "15" };
cvar_t	sk_556_bullet2 = { "sk_556_bullet2", "15" };
cvar_t	sk_556_bullet3 = { "sk_556_bullet3", "15" };

cvar_t	sk_762_bullet1 = {"sk_762_bullet1","35"};
cvar_t	sk_762_bullet2 = {"sk_762_bullet2","40"};
cvar_t	sk_762_bullet3 = {"sk_762_bullet3","40"};

#if FEATURE_SHOCKBEAM
cvar_t	sk_plr_shockroachs1 = { "sk_plr_shockroachs1", "0" };
cvar_t	sk_plr_shockroachs2 = { "sk_plr_shockroachs2", "0" };
cvar_t	sk_plr_shockroachs3 = { "sk_plr_shockroachs3", "0" };

cvar_t	sk_plr_shockroachm1 = { "sk_plr_shockroachm1", "0" };
cvar_t	sk_plr_shockroachm2 = { "sk_plr_shockroachm2", "0" };
cvar_t	sk_plr_shockroachm3 = { "sk_plr_shockroachm3", "0" };
#endif

#if FEATURE_SPOREGRENADE
cvar_t	sk_plr_spore1 = { "sk_plr_spore1", "0" };
cvar_t	sk_plr_spore2 = { "sk_plr_spore2", "0" };
cvar_t	sk_plr_spore3 = { "sk_plr_spore3", "0" };
#endif

#if FEATURE_DISPLACER
cvar_t	sk_plr_displacer1 = { "sk_plr_displacer_other1", "0" };
cvar_t	sk_plr_displacer2 = { "sk_plr_displacer_other2", "0" };
cvar_t	sk_plr_displacer3 = { "sk_plr_displacer_other3", "0" };

cvar_t	sk_plr_displacer_radius1 = { "sk_plr_displacer_radius1", "0" };
cvar_t	sk_plr_displacer_radius2 = { "sk_plr_displacer_radius2", "0" };
cvar_t	sk_plr_displacer_radius3 = { "sk_plr_displacer_radius3", "0" };
#endif

#if FEATURE_UZI
cvar_t	sk_plr_uzi1 = {"sk_plr_uzi1","6"};
cvar_t	sk_plr_uzi2 = {"sk_plr_uzi2","6"};
cvar_t	sk_plr_uzi3 = {"sk_plr_uzi3","6"};
#endif

// HORNET
cvar_t	sk_hornet_dmg1 = {"sk_hornet_dmg1","0"};
cvar_t	sk_hornet_dmg2 = {"sk_hornet_dmg2","0"};
cvar_t	sk_hornet_dmg3 = {"sk_hornet_dmg3","0"};

cvar_t	sk_plr_hornet_dmg1 = {"sk_plr_hornet_dmg1","7"};
cvar_t	sk_plr_hornet_dmg2 = {"sk_plr_hornet_dmg2","7"};
cvar_t	sk_plr_hornet_dmg3 = {"sk_plr_hornet_dmg3","7"};

// MORTAR
cvar_t	sk_mortar1 = {"sk_mortar1","200"};
cvar_t	sk_mortar2 = {"sk_mortar2","200"};
cvar_t	sk_mortar3 = {"sk_mortar3","200"};

// HEALTH/CHARGE
cvar_t	sk_suitcharger1	= { "sk_suitcharger1","0" };
cvar_t	sk_suitcharger2	= { "sk_suitcharger2","0" };		
cvar_t	sk_suitcharger3	= { "sk_suitcharger3","0" };		

cvar_t	sk_battery1	= { "sk_battery1","0" };			
cvar_t	sk_battery2	= { "sk_battery2","0" };			
cvar_t	sk_battery3	= { "sk_battery3","0" };			

cvar_t	sk_healthcharger1	= { "sk_healthcharger1","0" };		
cvar_t	sk_healthcharger2	= { "sk_healthcharger2","0" };		
cvar_t	sk_healthcharger3	= { "sk_healthcharger3","0" };		

cvar_t	sk_healthkit1	= { "sk_healthkit1","0" };		
cvar_t	sk_healthkit2	= { "sk_healthkit2","0" };		
cvar_t	sk_healthkit3	= { "sk_healthkit3","0" };		

cvar_t	sk_scientist_heal1	= { "sk_scientist_heal1","0" };	
cvar_t	sk_scientist_heal2	= { "sk_scientist_heal2","0" };	
cvar_t	sk_scientist_heal3	= { "sk_scientist_heal3","0" };	

cvar_t	sk_scientist_heal_time1	= { "sk_scientist_heal_time1", "60" };
cvar_t	sk_scientist_heal_time2	= { "sk_scientist_heal_time2", "60" };
cvar_t	sk_scientist_heal_time3	= { "sk_scientist_heal_time3", "60" };

cvar_t	sk_soda1	= { "sk_soda1","1" };		
cvar_t	sk_soda2	= { "sk_soda2","1" };		
cvar_t	sk_soda3	= { "sk_soda3","1" };		

// monster damage adjusters
cvar_t	sk_monster_head1	= { "sk_monster_head1","2" };
cvar_t	sk_monster_head2	= { "sk_monster_head2","2" };
cvar_t	sk_monster_head3	= { "sk_monster_head3","2" };

cvar_t	sk_monster_chest1	= { "sk_monster_chest1","1" };
cvar_t	sk_monster_chest2	= { "sk_monster_chest2","1" };
cvar_t	sk_monster_chest3	= { "sk_monster_chest3","1" };

cvar_t	sk_monster_stomach1	= { "sk_monster_stomach1","1" };
cvar_t	sk_monster_stomach2	= { "sk_monster_stomach2","1" };
cvar_t	sk_monster_stomach3	= { "sk_monster_stomach3","1" };

cvar_t	sk_monster_arm1	= { "sk_monster_arm1","1" };
cvar_t	sk_monster_arm2	= { "sk_monster_arm2","1" };
cvar_t	sk_monster_arm3	= { "sk_monster_arm3","1" };

cvar_t	sk_monster_leg1	= { "sk_monster_leg1","1" };
cvar_t	sk_monster_leg2	= { "sk_monster_leg2","1" };
cvar_t	sk_monster_leg3	= { "sk_monster_leg3","1" };

// player damage adjusters
cvar_t	sk_player_head1	= { "sk_player_head1","2" };
cvar_t	sk_player_head2	= { "sk_player_head2","2" };
cvar_t	sk_player_head3	= { "sk_player_head3","2" };

cvar_t	sk_player_chest1 = { "sk_player_chest1","1" };
cvar_t	sk_player_chest2 = { "sk_player_chest2","1" };
cvar_t	sk_player_chest3 = { "sk_player_chest3","1" };

cvar_t	sk_player_stomach1 = { "sk_player_stomach1","1" };
cvar_t	sk_player_stomach2 = { "sk_player_stomach2","1" };
cvar_t	sk_player_stomach3 = { "sk_player_stomach3","1" };

cvar_t	sk_player_arm1	= { "sk_player_arm1","1" };
cvar_t	sk_player_arm2	= { "sk_player_arm2","1" };
cvar_t	sk_player_arm3	= { "sk_player_arm3","1" };

cvar_t	sk_player_leg1	= { "sk_player_leg1","1" };
cvar_t	sk_player_leg2	= { "sk_player_leg2","1" };
cvar_t	sk_player_leg3	= { "sk_player_leg3","1" };

// flashlight settings

cvar_t	sk_flashlight_drain_time1	= { "sk_flashlight_drain_time1","120" };
cvar_t	sk_flashlight_drain_time2	= { "sk_flashlight_drain_time2","1020" };
cvar_t	sk_flashlight_drain_time3	= { "sk_flashlight_drain_time3","120" };

cvar_t	sk_flashlight_charge_time1	= { "sk_flashlight_charge_time1","20" };
cvar_t	sk_flashlight_charge_time2	= { "sk_flashlight_charge_time2","20" };
cvar_t	sk_flashlight_charge_time3	= { "sk_flashlight_charge_time3","20" };

// END Cvars for Skill Level settings

// Register your console variables here
// This gets called one time when the game is initialied
void GameDLLInit( void )
{
	// Register cvars here:
	if( CVAR_GET_POINTER( "build" ) )
		g_fIsXash3D = TRUE;

	g_psv_gravity = CVAR_GET_POINTER( "sv_gravity" );
	g_psv_aim = CVAR_GET_POINTER( "sv_aim" );
	g_footsteps = CVAR_GET_POINTER( "mp_footsteps" );

	g_psv_developer = CVAR_GET_POINTER( "developer" );

	CVAR_REGISTER( &displaysoundlist );
	CVAR_REGISTER( &allow_spectators );
#if FEATURE_USE_THROUGH_WALLS_CVAR
	CVAR_REGISTER( &use_through_walls );
#endif
#if FEATURE_TRIDEPTH_CVAR
	CVAR_REGISTER( &tridepth );
#endif
#if FEATURE_NPC_NEAREST_CVAR
	CVAR_REGISTER( &npc_nearest );
#endif
#if FEATURE_NPC_FORGET_ENEMY_CVAR
	CVAR_REGISTER( &npc_forget_enemy_time );
#endif
#if FEATURE_NPC_FIX_MELEE_DISTANCE_CVAR
	CVAR_REGISTER( &npc_fix_melee_distance );
#endif
#if FEATURE_NPC_ACTIVE_AFTER_COMBAT_CVAR
	CVAR_REGISTER( &npc_active_after_combat );
#endif
	CVAR_REGISTER( &npc_patrol );

	CVAR_REGISTER( &teamplay );
	CVAR_REGISTER( &fraglimit );
	CVAR_REGISTER( &timelimit );

	CVAR_REGISTER( &fragsleft );
	CVAR_REGISTER( &timeleft );

	CVAR_REGISTER( &friendlyfire );
	CVAR_REGISTER( &falldamage );
	CVAR_REGISTER( &weaponstay );

	CVAR_REGISTER( &weapon_respawndelay );
	CVAR_REGISTER( &ammo_respawndelay );
	CVAR_REGISTER( &item_respawndelay );
	CVAR_REGISTER( &healthcharger_rechargetime );
	CVAR_REGISTER( &hevcharger_rechargetime );

	CVAR_REGISTER( &selfgauss );
	CVAR_REGISTER( &satchelfix );

	CVAR_REGISTER( &monsteryawspeedfix );
	CVAR_REGISTER( &forcerespawn );
	CVAR_REGISTER( &respawndelay );
	CVAR_REGISTER( &flashlight );
	CVAR_REGISTER( &aimcrosshair );
	CVAR_REGISTER( &decalfrequency );
	CVAR_REGISTER( &teamlist );
	CVAR_REGISTER( &teamoverride );
	CVAR_REGISTER( &defaultteam );

	CVAR_REGISTER( &allowmonsters );
	CVAR_REGISTER( &allowmonsterinfo );
	CVAR_REGISTER( &npc_dropweapons );
	CVAR_REGISTER( &dmgperscore );
	CVAR_REGISTER( &allydmgpenalty );
	CVAR_REGISTER( &npckill );

	CVAR_REGISTER( &bhopcap );
	CVAR_REGISTER( &multibyte_only );

	CVAR_REGISTER( &mp_chattime );

#if FEATURE_USE_TO_TAKE_CVAR
	CVAR_REGISTER( &use_to_take );
#endif
#if FEATURE_GRENADE_JUMP_CVAR
	CVAR_REGISTER( &grenade_jump );
#endif

	CVAR_REGISTER( &keepinventory );

// REGISTER CVARS FOR SKILL LEVEL STUFF
	// Agrunt
	CVAR_REGISTER( &sk_agrunt_health1 );// {"sk_agrunt_health1","0"};
	CVAR_REGISTER( &sk_agrunt_health2 );// {"sk_agrunt_health2","0"};
	CVAR_REGISTER( &sk_agrunt_health3 );// {"sk_agrunt_health3","0"};

	CVAR_REGISTER( &sk_agrunt_dmg_punch1 );// {"sk_agrunt_dmg_punch1","0"};
	CVAR_REGISTER( &sk_agrunt_dmg_punch2 );// {"sk_agrunt_dmg_punch2","0"};
	CVAR_REGISTER( &sk_agrunt_dmg_punch3 );// {"sk_agrunt_dmg_punch3","0"};

	// Apache
	CVAR_REGISTER( &sk_apache_health1 );// {"sk_apache_health1","0"};
	CVAR_REGISTER( &sk_apache_health2 );// {"sk_apache_health2","0"};
	CVAR_REGISTER( &sk_apache_health3 );// {"sk_apache_health3","0"};
	
	// Barnacke
	CVAR_REGISTER( &sk_barnacle_health1 );
	CVAR_REGISTER( &sk_barnacle_health2 );
	CVAR_REGISTER( &sk_barnacle_health3 );

#if FEATURE_BABYGARG
	// Baby Gargantua
	CVAR_REGISTER( &sk_babygargantua_health1 );// {"sk_babygargantua_health1","0"};
	CVAR_REGISTER( &sk_babygargantua_health2 );// {"sk_babygargantua_health2","0"};
	CVAR_REGISTER( &sk_babygargantua_health3 );// {"sk_babygargantua_health3","0"};

	CVAR_REGISTER( &sk_babygargantua_dmg_slash1 );// {"sk_babygargantua_dmg_slash1","0"};
	CVAR_REGISTER( &sk_babygargantua_dmg_slash2 );// {"sk_babygargantua_dmg_slash2","0"};
	CVAR_REGISTER( &sk_babygargantua_dmg_slash3 );// {"sk_babygargantua_dmg_slash3","0"};

	CVAR_REGISTER( &sk_babygargantua_dmg_fire1 );// {"sk_babygargantua_dmg_fire1","0"};
	CVAR_REGISTER( &sk_babygargantua_dmg_fire2 );// {"sk_babygargantua_dmg_fire2","0"};
	CVAR_REGISTER( &sk_babygargantua_dmg_fire3 );// {"sk_babygargantua_dmg_fire3","0"};

	CVAR_REGISTER( &sk_babygargantua_dmg_stomp1 );// {"sk_babygargantua_dmg_stomp1","0"};
	CVAR_REGISTER( &sk_babygargantua_dmg_stomp2 );// {"sk_babygargantua_dmg_stomp2","0"};
	CVAR_REGISTER( &sk_babygargantua_dmg_stomp3	);// {"sk_babygargantua_dmg_stomp3","0"};
#endif

	// Barney
	CVAR_REGISTER( &sk_barney_health1 );// {"sk_barney_health1","0"};
	CVAR_REGISTER( &sk_barney_health2 );// {"sk_barney_health2","0"};
	CVAR_REGISTER( &sk_barney_health3 );// {"sk_barney_health3","0"};

	// Bullsquid
	CVAR_REGISTER( &sk_bullsquid_health1 );// {"sk_bullsquid_health1","0"};
	CVAR_REGISTER( &sk_bullsquid_health2 );// {"sk_bullsquid_health2","0"};
	CVAR_REGISTER( &sk_bullsquid_health3 );// {"sk_bullsquid_health3","0"};

	CVAR_REGISTER( &sk_bullsquid_dmg_bite1 );// {"sk_bullsquid_dmg_bite1","0"};
	CVAR_REGISTER( &sk_bullsquid_dmg_bite2 );// {"sk_bullsquid_dmg_bite2","0"};
	CVAR_REGISTER( &sk_bullsquid_dmg_bite3 );// {"sk_bullsquid_dmg_bite3","0"};

	CVAR_REGISTER( &sk_bullsquid_dmg_whip1 );// {"sk_bullsquid_dmg_whip1","0"};
	CVAR_REGISTER( &sk_bullsquid_dmg_whip2 );// {"sk_bullsquid_dmg_whip2","0"};
	CVAR_REGISTER( &sk_bullsquid_dmg_whip3 );// {"sk_bullsquid_dmg_whip3","0"};

	CVAR_REGISTER( &sk_bullsquid_dmg_spit1 );// {"sk_bullsquid_dmg_spit1","0"};
	CVAR_REGISTER( &sk_bullsquid_dmg_spit2 );// {"sk_bullsquid_dmg_spit2","0"};
	CVAR_REGISTER( &sk_bullsquid_dmg_spit3 );// {"sk_bullsquid_dmg_spit3","0"};

	CVAR_REGISTER( &sk_bigmomma_health_factor1 );// {"sk_bigmomma_health_factor1","1.0"};
	CVAR_REGISTER( &sk_bigmomma_health_factor2 );// {"sk_bigmomma_health_factor2","1.0"};
	CVAR_REGISTER( &sk_bigmomma_health_factor3 );// {"sk_bigmomma_health_factor3","1.0"};

	CVAR_REGISTER( &sk_bigmomma_dmg_slash1 );// {"sk_bigmomma_dmg_slash1","50"};
	CVAR_REGISTER( &sk_bigmomma_dmg_slash2 );// {"sk_bigmomma_dmg_slash2","50"};
	CVAR_REGISTER( &sk_bigmomma_dmg_slash3 );// {"sk_bigmomma_dmg_slash3","50"};

	CVAR_REGISTER( &sk_bigmomma_dmg_blast1 );// {"sk_bigmomma_dmg_blast1","100"};
	CVAR_REGISTER( &sk_bigmomma_dmg_blast2 );// {"sk_bigmomma_dmg_blast2","100"};
	CVAR_REGISTER( &sk_bigmomma_dmg_blast3 );// {"sk_bigmomma_dmg_blast3","100"};

	CVAR_REGISTER( &sk_bigmomma_radius_blast1 );// {"sk_bigmomma_radius_blast1","250"};
	CVAR_REGISTER( &sk_bigmomma_radius_blast2 );// {"sk_bigmomma_radius_blast2","250"};
	CVAR_REGISTER( &sk_bigmomma_radius_blast3 );// {"sk_bigmomma_radius_blast3","250"};

#if FEATURE_CLEANSUIT_SCIENTIST
	// Cleansuit Scientist
	CVAR_REGISTER ( &sk_cleansuit_scientist_health1 );// {"sk_scientist_health1","0"};
	CVAR_REGISTER ( &sk_cleansuit_scientist_health2 );// {"sk_scientist_health2","0"};
	CVAR_REGISTER ( &sk_cleansuit_scientist_health3 );// {"sk_scientist_health3","0"};
#endif

	// Gargantua
	CVAR_REGISTER( &sk_gargantua_health1 );// {"sk_gargantua_health1","0"};
	CVAR_REGISTER( &sk_gargantua_health2 );// {"sk_gargantua_health2","0"};
	CVAR_REGISTER( &sk_gargantua_health3 );// {"sk_gargantua_health3","0"};

	CVAR_REGISTER( &sk_gargantua_dmg_slash1 );// {"sk_gargantua_dmg_slash1","0"};
	CVAR_REGISTER( &sk_gargantua_dmg_slash2 );// {"sk_gargantua_dmg_slash2","0"};
	CVAR_REGISTER( &sk_gargantua_dmg_slash3 );// {"sk_gargantua_dmg_slash3","0"};

	CVAR_REGISTER( &sk_gargantua_dmg_fire1 );// {"sk_gargantua_dmg_fire1","0"};
	CVAR_REGISTER( &sk_gargantua_dmg_fire2 );// {"sk_gargantua_dmg_fire2","0"};
	CVAR_REGISTER( &sk_gargantua_dmg_fire3 );// {"sk_gargantua_dmg_fire3","0"};

	CVAR_REGISTER( &sk_gargantua_dmg_stomp1 );// {"sk_gargantua_dmg_stomp1","0"};
	CVAR_REGISTER( &sk_gargantua_dmg_stomp2 );// {"sk_gargantua_dmg_stomp2","0"};
	CVAR_REGISTER( &sk_gargantua_dmg_stomp3	);// {"sk_gargantua_dmg_stomp3","0"};

	// Hassassin
	CVAR_REGISTER( &sk_hassassin_health1 );// {"sk_hassassin_health1","0"};
	CVAR_REGISTER( &sk_hassassin_health2 );// {"sk_hassassin_health2","0"};
	CVAR_REGISTER( &sk_hassassin_health3 );// {"sk_hassassin_health3","0"};

	// Headcrab
	CVAR_REGISTER( &sk_headcrab_health1 );// {"sk_headcrab_health1","0"};
	CVAR_REGISTER( &sk_headcrab_health2 );// {"sk_headcrab_health2","0"};
	CVAR_REGISTER( &sk_headcrab_health3 );// {"sk_headcrab_health3","0"};

	CVAR_REGISTER( &sk_headcrab_dmg_bite1 );// {"sk_headcrab_dmg_bite1","0"};
	CVAR_REGISTER( &sk_headcrab_dmg_bite2 );// {"sk_headcrab_dmg_bite2","0"};
	CVAR_REGISTER( &sk_headcrab_dmg_bite3 );// {"sk_headcrab_dmg_bite3","0"};

#if FEATURE_OPFOR_GRUNT
	// Opposing Hgrunt
	CVAR_REGISTER( &sk_hgrunt_ally_health1 );
	CVAR_REGISTER( &sk_hgrunt_ally_health2 );
	CVAR_REGISTER( &sk_hgrunt_ally_health3 );

	CVAR_REGISTER( &sk_hgrunt_ally_kick1 );
	CVAR_REGISTER( &sk_hgrunt_ally_kick2 );
	CVAR_REGISTER( &sk_hgrunt_ally_kick3 );

	CVAR_REGISTER( &sk_hgrunt_ally_pellets1 );
	CVAR_REGISTER( &sk_hgrunt_ally_pellets2 );
	CVAR_REGISTER( &sk_hgrunt_ally_pellets3 );

	CVAR_REGISTER( &sk_hgrunt_ally_gspeed1 );
	CVAR_REGISTER( &sk_hgrunt_ally_gspeed2 );
	CVAR_REGISTER( &sk_hgrunt_ally_gspeed3 );

	// Medic
	CVAR_REGISTER( &sk_medic_ally_health1 );
	CVAR_REGISTER( &sk_medic_ally_health2 );
	CVAR_REGISTER( &sk_medic_ally_health3 );

	CVAR_REGISTER( &sk_medic_ally_kick1 );
	CVAR_REGISTER( &sk_medic_ally_kick2 );
	CVAR_REGISTER( &sk_medic_ally_kick3 );

	CVAR_REGISTER( &sk_medic_ally_gspeed1 );
	CVAR_REGISTER( &sk_medic_ally_gspeed2 );
	CVAR_REGISTER( &sk_medic_ally_gspeed3 );

	CVAR_REGISTER( &sk_medic_ally_heal1 );
	CVAR_REGISTER( &sk_medic_ally_heal2 );
	CVAR_REGISTER( &sk_medic_ally_heal3 );

	// Torch
	CVAR_REGISTER( &sk_torch_ally_health1 );
	CVAR_REGISTER( &sk_torch_ally_health2 );
	CVAR_REGISTER( &sk_torch_ally_health3 );

	CVAR_REGISTER( &sk_torch_ally_kick1 );
	CVAR_REGISTER( &sk_torch_ally_kick2 );
	CVAR_REGISTER( &sk_torch_ally_kick3 );

	CVAR_REGISTER( &sk_torch_ally_gspeed1 );
	CVAR_REGISTER( &sk_torch_ally_gspeed2 );
	CVAR_REGISTER( &sk_torch_ally_gspeed3 );
#endif

	// Hgrunt
	CVAR_REGISTER( &sk_hgrunt_health1 );// {"sk_hgrunt_health1","0"};
	CVAR_REGISTER( &sk_hgrunt_health2 );// {"sk_hgrunt_health2","0"};
	CVAR_REGISTER( &sk_hgrunt_health3 );// {"sk_hgrunt_health3","0"};

	CVAR_REGISTER( &sk_hgrunt_kick1 );// {"sk_hgrunt_kick1","0"};
	CVAR_REGISTER( &sk_hgrunt_kick2 );// {"sk_hgrunt_kick2","0"};
	CVAR_REGISTER( &sk_hgrunt_kick3 );// {"sk_hgrunt_kick3","0"};

	CVAR_REGISTER( &sk_hgrunt_pellets1 );
	CVAR_REGISTER( &sk_hgrunt_pellets2 );
	CVAR_REGISTER( &sk_hgrunt_pellets3 );

	CVAR_REGISTER( &sk_hgrunt_gspeed1 );
	CVAR_REGISTER( &sk_hgrunt_gspeed2 );
	CVAR_REGISTER( &sk_hgrunt_gspeed3 );

#if FEATURE_HWGRUNT
	// HWgrunt
	CVAR_REGISTER( &sk_hwgrunt_health1 );
	CVAR_REGISTER( &sk_hwgrunt_health2 );
	CVAR_REGISTER( &sk_hwgrunt_health3 );
#endif

	// Houndeye
	CVAR_REGISTER( &sk_houndeye_health1 );// {"sk_houndeye_health1","0"};
	CVAR_REGISTER( &sk_houndeye_health2 );// {"sk_houndeye_health2","0"};
	CVAR_REGISTER( &sk_houndeye_health3 );// {"sk_houndeye_health3","0"};

	CVAR_REGISTER( &sk_houndeye_dmg_blast1 );// {"sk_houndeye_dmg_blast1","0"};
	CVAR_REGISTER( &sk_houndeye_dmg_blast2 );// {"sk_houndeye_dmg_blast2","0"};
	CVAR_REGISTER( &sk_houndeye_dmg_blast3 );// {"sk_houndeye_dmg_blast3","0"};

	// ISlave
	CVAR_REGISTER( &sk_islave_health1 );// {"sk_islave_health1","0"};
	CVAR_REGISTER( &sk_islave_health2 );// {"sk_islave_health2","0"};
	CVAR_REGISTER( &sk_islave_health3 );// {"sk_islave_health3","0"};

	CVAR_REGISTER( &sk_islave_dmg_claw1 );// {"sk_islave_dmg_claw1","0"};
	CVAR_REGISTER( &sk_islave_dmg_claw2 );// {"sk_islave_dmg_claw2","0"};
	CVAR_REGISTER( &sk_islave_dmg_claw3 );// {"sk_islave_dmg_claw3","0"};

	CVAR_REGISTER( &sk_islave_dmg_clawrake1 );// {"sk_islave_dmg_clawrake1","0"};
	CVAR_REGISTER( &sk_islave_dmg_clawrake2 );// {"sk_islave_dmg_clawrake2","0"};
	CVAR_REGISTER( &sk_islave_dmg_clawrake3 );// {"sk_islave_dmg_clawrake3","0"};

	CVAR_REGISTER( &sk_islave_dmg_zap1 );// {"sk_islave_dmg_zap1","0"};
	CVAR_REGISTER( &sk_islave_dmg_zap2 );// {"sk_islave_dmg_zap2","0"};
	CVAR_REGISTER( &sk_islave_dmg_zap3 );// {"sk_islave_dmg_zap3","0"};

	CVAR_REGISTER( &sk_islave_zap_rate1 );
	CVAR_REGISTER( &sk_islave_zap_rate2 );
	CVAR_REGISTER( &sk_islave_zap_rate3 );

	// Icthyosaur
	CVAR_REGISTER( &sk_ichthyosaur_health1 );// {"sk_ichthyosaur_health1","0"};
	CVAR_REGISTER( &sk_ichthyosaur_health2 );// {"sk_ichthyosaur_health2","0"};
	CVAR_REGISTER( &sk_ichthyosaur_health3 );// {"sk_ichthyosaur_health3","0"};

	CVAR_REGISTER( &sk_ichthyosaur_shake1 );// {"sk_ichthyosaur_health3","0"};
	CVAR_REGISTER( &sk_ichthyosaur_shake2 );// {"sk_ichthyosaur_health3","0"};
	CVAR_REGISTER( &sk_ichthyosaur_shake3 );// {"sk_ichthyosaur_health3","0"};

	// Leech
	CVAR_REGISTER( &sk_leech_health1 );// {"sk_leech_health1","0"};
	CVAR_REGISTER( &sk_leech_health2 );// {"sk_leech_health2","0"};
	CVAR_REGISTER( &sk_leech_health3 );// {"sk_leech_health3","0"};

	CVAR_REGISTER( &sk_leech_dmg_bite1 );// {"sk_leech_dmg_bite1","0"};
	CVAR_REGISTER( &sk_leech_dmg_bite2 );// {"sk_leech_dmg_bite2","0"};
	CVAR_REGISTER( &sk_leech_dmg_bite3 );// {"sk_leech_dmg_bite3","0"};

	// Controller
	CVAR_REGISTER( &sk_controller_health1 );
	CVAR_REGISTER( &sk_controller_health2 );
	CVAR_REGISTER( &sk_controller_health3 );

	CVAR_REGISTER( &sk_controller_dmgzap1 );
	CVAR_REGISTER( &sk_controller_dmgzap2 );
	CVAR_REGISTER( &sk_controller_dmgzap3 );

	CVAR_REGISTER( &sk_controller_speedball1 );
	CVAR_REGISTER( &sk_controller_speedball2 );
	CVAR_REGISTER( &sk_controller_speedball3 );

	CVAR_REGISTER( &sk_controller_dmgball1 );
	CVAR_REGISTER( &sk_controller_dmgball2 );
	CVAR_REGISTER( &sk_controller_dmgball3 );

#if FEATURE_MASSN
	// Massassin
	CVAR_REGISTER( &sk_massassin_health1 );// {"sk_massassin_health1","0"};
	CVAR_REGISTER( &sk_massassin_health2 );// {"sk_massassin_health2","0"};
	CVAR_REGISTER( &sk_massassin_health3 );// {"sk_massassin_health3","0"};

	CVAR_REGISTER( &sk_massassin_kick1 );// {"sk_massassin_kick1","0"};
	CVAR_REGISTER( &sk_massassin_kick2 );// {"sk_massassin_kick2","0"};
	CVAR_REGISTER( &sk_massassin_kick3 );// {"sk_massassin_kick3","0"};

	CVAR_REGISTER( &sk_massassin_gspeed1 );
	CVAR_REGISTER( &sk_massassin_gspeed2 );
	CVAR_REGISTER( &sk_massassin_gspeed3 );
#endif

	// Nihilanth
	CVAR_REGISTER( &sk_nihilanth_health1 );// {"sk_nihilanth_health1","0"};
	CVAR_REGISTER( &sk_nihilanth_health2 );// {"sk_nihilanth_health2","0"};
	CVAR_REGISTER( &sk_nihilanth_health3 );// {"sk_nihilanth_health3","0"};

	CVAR_REGISTER( &sk_nihilanth_zap1 );
	CVAR_REGISTER( &sk_nihilanth_zap2 );
	CVAR_REGISTER( &sk_nihilanth_zap3 );

	CVAR_REGISTER ( &sk_osprey1 );
	CVAR_REGISTER ( &sk_osprey2 );
	CVAR_REGISTER ( &sk_osprey3 );

#if FEATURE_BLACK_OSPREY
	CVAR_REGISTER ( &sk_blkopsosprey1 );
	CVAR_REGISTER ( &sk_blkopsosprey2 );
	CVAR_REGISTER ( &sk_blkopsosprey3 );
#endif

#if FEATURE_OTIS
	// Otis
	CVAR_REGISTER ( &sk_otis_health1 );// {"sk_barney_health1","0"};
	CVAR_REGISTER ( &sk_otis_health2 );// {"sk_barney_health2","0"};
	CVAR_REGISTER ( &sk_otis_health3 );// {"sk_barney_health3","0"};
#endif

#if FEATURE_PITDRONE
	// Pitdrone
	CVAR_REGISTER( &sk_pitdrone_health1 );// { "sk_pitdrone_health1", "0" };
	CVAR_REGISTER( &sk_pitdrone_health2 );// { "sk_pitdrone_health2", "0" };
	CVAR_REGISTER( &sk_pitdrone_health3 );// { "sk_pitdrone_health3", "0" };

	CVAR_REGISTER( &sk_pitdrone_dmg_bite1 );// { "sk_pitdrone_dmg_bite1", "0" };
	CVAR_REGISTER( &sk_pitdrone_dmg_bite2 );// { "sk_pitdrone_dmg_bite2", "0" };
	CVAR_REGISTER( &sk_pitdrone_dmg_bite3 );// { "sk_pitdrone_dmg_bite3", "0" };

	CVAR_REGISTER( &sk_pitdrone_dmg_whip1 );// { "sk_pitdrone_dmg_whip1", "0" };
	CVAR_REGISTER( &sk_pitdrone_dmg_whip2 );// { "sk_pitdrone_dmg_whip2", "0" };
	CVAR_REGISTER( &sk_pitdrone_dmg_whip3 );// { "sk_pitdrone_dmg_whip3", "0" };

	CVAR_REGISTER( &sk_pitdrone_dmg_spit1 );// { "sk_pitdrone_dmg_spit1", "0" };
	CVAR_REGISTER( &sk_pitdrone_dmg_spit2 );// { "sk_pitdrone_dmg_spit2", "0" };
	CVAR_REGISTER( &sk_pitdrone_dmg_spit3 );// { "sk_pitdrone_dmg_spit3", "0" };
#endif

#if FEATURE_PITWORM
	CVAR_REGISTER( &sk_pitworm_health1 );// { "sk_pitworm_health1", "0" };
	CVAR_REGISTER( &sk_pitworm_health2 );// { "sk_pitworm_health2", "0" };
	CVAR_REGISTER( &sk_pitworm_health3 );// { "sk_pitworm_health3", "0" };

	CVAR_REGISTER( &sk_pitworm_dmg_swipe1 );// { "sk_pitworm_dmg_swipe1", "0" };
	CVAR_REGISTER( &sk_pitworm_dmg_swipe2 );// { "sk_pitworm_dmg_swipe2", "0" };
	CVAR_REGISTER( &sk_pitworm_dmg_swipe3 );// { "sk_pitworm_dmg_swipe3", "0" };

	CVAR_REGISTER( &sk_pitworm_dmg_beam1 );// { "sk_pitworm_dmg_beam1", "0" };
	CVAR_REGISTER( &sk_pitworm_dmg_beam2 );// { "sk_pitworm_dmg_beam2", "0" };
	CVAR_REGISTER( &sk_pitworm_dmg_beam3 );// { "sk_pitworm_dmg_beam3", "0" };
#endif

#if FEATURE_GENEWORM
	// Geneworm
	CVAR_REGISTER( &sk_geneworm_health1 );// { "sk_geneworm_health1", "0" };
	CVAR_REGISTER( &sk_geneworm_health2 );// { "sk_geneworm_health2", "0" };
	CVAR_REGISTER( &sk_geneworm_health3 );// { "sk_geneworm_health3", "0" };

	CVAR_REGISTER( &sk_geneworm_dmg_spit1 );// { "sk_geneworm_dmg_spit1", "0" };
	CVAR_REGISTER( &sk_geneworm_dmg_spit2 );// { "sk_geneworm_dmg_spit2", "0" };
	CVAR_REGISTER( &sk_geneworm_dmg_spit3 );// { "sk_geneworm_dmg_spit3", "0" };

	CVAR_REGISTER( &sk_geneworm_dmg_hit1);// { "sk_geneworm_dmg_hit1", "0" };
	CVAR_REGISTER( &sk_geneworm_dmg_hit2 );// { "sk_geneworm_dmg_hit2", "0" };
	CVAR_REGISTER( &sk_geneworm_dmg_hit3 );// { "sk_geneworm_dmg_hit3", "0" };
#endif

#if FEATURE_ROBOGRUNT
	CVAR_REGISTER( &sk_rgrunt_explode1 );
	CVAR_REGISTER( &sk_rgrunt_explode2 );
	CVAR_REGISTER( &sk_rgrunt_explode3 );
#endif


#if FEATURE_SHOCKTROOPER
	// ShockTrooper
	CVAR_REGISTER( &sk_shocktrooper_health1 );// { "sk_shocktrooper_health1", "0" };
	CVAR_REGISTER( &sk_shocktrooper_health2 );// { "sk_shocktrooper_health2", "0" };
	CVAR_REGISTER( &sk_shocktrooper_health3 );// { "sk_shocktrooper_health3", "0" };

	CVAR_REGISTER( &sk_shocktrooper_kick1 );// { "sk_shocktrooper_kick1", "0" };
	CVAR_REGISTER( &sk_shocktrooper_kick2 );// { "sk_shocktrooper_kick2", "0" };
	CVAR_REGISTER( &sk_shocktrooper_kick3 );// { "sk_shocktrooper_kick3", "0" };

	CVAR_REGISTER( &sk_shocktrooper_gspeed1 );// { "sk_shocktrooper_gspeed1", "0" };
	CVAR_REGISTER( &sk_shocktrooper_gspeed2 );// { "sk_shocktrooper_gspeed2", "0" };
	CVAR_REGISTER( &sk_shocktrooper_gspeed3 );// { "sk_shocktrooper_gspeed3", "0" };

	CVAR_REGISTER( &sk_shocktrooper_maxcharge1 );// { "sk_shocktrooper_maxcharge1", "0" };
	CVAR_REGISTER( &sk_shocktrooper_maxcharge2 );// { "sk_shocktrooper_maxcharge2", "0" };
	CVAR_REGISTER( &sk_shocktrooper_maxcharge3 );// { "sk_shocktrooper_maxcharge3", "0" };

	CVAR_REGISTER( &sk_shocktrooper_rchgspeed1 );// { "sk_shocktrooper_rchgspeed1", "0" };
	CVAR_REGISTER( &sk_shocktrooper_rchgspeed2 );// { "sk_shocktrooper_rchgspeed2", "0" };
	CVAR_REGISTER( &sk_shocktrooper_rchgspeed3 );// { "sk_shocktrooper_rchgspeed3", "0" };

	// Shock Roach
	CVAR_REGISTER( &sk_shockroach_health1 );// { "sk_shockroach_health1", "0" };
	CVAR_REGISTER( &sk_shockroach_health2 );// { "sk_shockroach_health2", "0" };
	CVAR_REGISTER( &sk_shockroach_health3 );// { "sk_shockroach_health3", "0" };

	CVAR_REGISTER( &sk_shockroach_dmg_bite1 );// { "sk_shockroach_dmg_bite1", "0" };
	CVAR_REGISTER( &sk_shockroach_dmg_bite2 );// { "sk_shockroach_dmg_bite2", "0" };
	CVAR_REGISTER( &sk_shockroach_dmg_bite3 );// { "sk_shockroach_dmg_bite3", "0" };

	CVAR_REGISTER( &sk_shockroach_lifespan1 );// { "sk_shockroach_lifespan1", "0" };
	CVAR_REGISTER( &sk_shockroach_lifespan2 );// { "sk_shockroach_lifespan2", "0" };
	CVAR_REGISTER( &sk_shockroach_lifespan3 );// { "sk_shockroach_lifespan3", "0" };
#endif

	// Scientist
	CVAR_REGISTER( &sk_scientist_health1 );// {"sk_scientist_health1","0"};
	CVAR_REGISTER( &sk_scientist_health2 );// {"sk_scientist_health2","0"};
	CVAR_REGISTER( &sk_scientist_health3 );// {"sk_scientist_health3","0"};

	// Snark
	CVAR_REGISTER( &sk_snark_health1 );// {"sk_snark_health1","0"};
	CVAR_REGISTER( &sk_snark_health2 );// {"sk_snark_health2","0"};
	CVAR_REGISTER( &sk_snark_health3 );// {"sk_snark_health3","0"};

	CVAR_REGISTER( &sk_snark_dmg_bite1 );// {"sk_snark_dmg_bite1","0"};
	CVAR_REGISTER( &sk_snark_dmg_bite2 );// {"sk_snark_dmg_bite2","0"};
	CVAR_REGISTER( &sk_snark_dmg_bite3 );// {"sk_snark_dmg_bite3","0"};

	CVAR_REGISTER( &sk_snark_dmg_pop1 );// {"sk_snark_dmg_pop1","0"};
	CVAR_REGISTER( &sk_snark_dmg_pop2 );// {"sk_snark_dmg_pop2","0"};
	CVAR_REGISTER( &sk_snark_dmg_pop3 );// {"sk_snark_dmg_pop3","0"};

#if FEATURE_VOLTIFORE
	// Voltigore
	CVAR_REGISTER( &sk_voltigore_health1 );// { "sk_voltigore_health1", "0" };
	CVAR_REGISTER( &sk_voltigore_health2 );// { "sk_voltigore_health2", "0" };
	CVAR_REGISTER( &sk_voltigore_health3 );// { "sk_voltigore_health3", "0" };

	CVAR_REGISTER( &sk_voltigore_dmg_punch1 );// { "sk_voltigore_dmg_punch1", "0" };
	CVAR_REGISTER( &sk_voltigore_dmg_punch2 );// { "sk_voltigore_dmg_punch2", "0" };
	CVAR_REGISTER( &sk_voltigore_dmg_punch3 );// { "sk_voltigore_dmg_punch3", "0" };

	CVAR_REGISTER( &sk_voltigore_dmg_beam1 );// { "sk_voltigore_dmg_beam1", "0" };
	CVAR_REGISTER( &sk_voltigore_dmg_beam2 );// { "sk_voltigore_dmg_beam2", "0" };
	CVAR_REGISTER( &sk_voltigore_dmg_beam3 );// { "sk_voltigore_dmg_beam3", "0" };

	CVAR_REGISTER( &sk_voltigore_dmg_explode1 );
	CVAR_REGISTER( &sk_voltigore_dmg_explode2 );
	CVAR_REGISTER( &sk_voltigore_dmg_explode3 );

	// Baby Voltigore
	CVAR_REGISTER( &sk_babyvoltigore_health1 );// { "sk_babyvoltigore_health1", "0" };
	CVAR_REGISTER( &sk_babyvoltigore_health2 );// { "sk_babyvoltigore_health2", "0" };
	CVAR_REGISTER( &sk_babyvoltigore_health3 );// { "sk_babyvoltigore_health3", "0" };

	CVAR_REGISTER( &sk_babyvoltigore_dmg_punch1 );// { "sk_babyvoltigore_dmg_punch1", "0" };
	CVAR_REGISTER( &sk_babyvoltigore_dmg_punch2 );// { "sk_babyvoltigore_dmg_punch2", "0" };
	CVAR_REGISTER( &sk_babyvoltigore_dmg_punch3 );// { "sk_babyvoltigore_dmg_punch3", "0" };
#endif

	// Zombie
	CVAR_REGISTER( &sk_zombie_health1 );// {"sk_zombie_health1","0"};
	CVAR_REGISTER( &sk_zombie_health2 );// {"sk_zombie_health3","0"};
	CVAR_REGISTER( &sk_zombie_health3 );// {"sk_zombie_health3","0"};

	CVAR_REGISTER( &sk_zombie_dmg_one_slash1 );// {"sk_zombie_dmg_one_slash1","0"};
	CVAR_REGISTER( &sk_zombie_dmg_one_slash2 );// {"sk_zombie_dmg_one_slash2","0"};
	CVAR_REGISTER( &sk_zombie_dmg_one_slash3 );// {"sk_zombie_dmg_one_slash3","0"};

	CVAR_REGISTER( &sk_zombie_dmg_both_slash1 );// {"sk_zombie_dmg_both_slash1","0"};
	CVAR_REGISTER( &sk_zombie_dmg_both_slash2 );// {"sk_zombie_dmg_both_slash2","0"};
	CVAR_REGISTER( &sk_zombie_dmg_both_slash3 );// {"sk_zombie_dmg_both_slash3","0"};

#if FEATURE_ZOMBIE_BARNEY
	// Zombie Barney
	CVAR_REGISTER ( &sk_zombie_barney_health1 );// {"sk_zombie_health1","0"};
	CVAR_REGISTER ( &sk_zombie_barney_health2 );// {"sk_zombie_health3","0"};
	CVAR_REGISTER ( &sk_zombie_barney_health3 );// {"sk_zombie_health3","0"};

	CVAR_REGISTER ( &sk_zombie_barney_dmg_one_slash1 );// {"sk_zombie_dmg_one_slash1","0"};
	CVAR_REGISTER ( &sk_zombie_barney_dmg_one_slash2 );// {"sk_zombie_dmg_one_slash2","0"};
	CVAR_REGISTER ( &sk_zombie_barney_dmg_one_slash3 );// {"sk_zombie_dmg_one_slash3","0"};

	CVAR_REGISTER ( &sk_zombie_barney_dmg_both_slash1 );// {"sk_zombie_dmg_both_slash1","0"};
	CVAR_REGISTER ( &sk_zombie_barney_dmg_both_slash2 );// {"sk_zombie_dmg_both_slash2","0"};
	CVAR_REGISTER ( &sk_zombie_barney_dmg_both_slash3 );// {"sk_zombie_dmg_both_slash3","0"};
#endif
#if FEATURE_ZOMBIE_SOLDIER
	// Zombie Soldier
	CVAR_REGISTER ( &sk_zombie_soldier_health1 );// {"sk_zombie_health1","0"};
	CVAR_REGISTER ( &sk_zombie_soldier_health2 );// {"sk_zombie_health3","0"};
	CVAR_REGISTER ( &sk_zombie_soldier_health3 );// {"sk_zombie_health3","0"};

	CVAR_REGISTER ( &sk_zombie_soldier_dmg_one_slash1 );// {"sk_zombie_dmg_one_slash1","0"};
	CVAR_REGISTER ( &sk_zombie_soldier_dmg_one_slash2 );// {"sk_zombie_dmg_one_slash2","0"};
	CVAR_REGISTER ( &sk_zombie_soldier_dmg_one_slash3 );// {"sk_zombie_dmg_one_slash3","0"};

	CVAR_REGISTER ( &sk_zombie_soldier_dmg_both_slash1 );// {"sk_zombie_dmg_both_slash1","0"};
	CVAR_REGISTER ( &sk_zombie_soldier_dmg_both_slash2 );// {"sk_zombie_dmg_both_slash2","0"};
	CVAR_REGISTER ( &sk_zombie_soldier_dmg_both_slash3 );// {"sk_zombie_dmg_both_slash3","0"};
#endif

#if FEATURE_GONOME
	// Gonome
	CVAR_REGISTER( &sk_gonome_health1 );// { "sk_gonome_health1", "0" };
	CVAR_REGISTER( &sk_gonome_health2 );// { "sk_gonome_health2", "0" };
	CVAR_REGISTER( &sk_gonome_health3 );// { "sk_gonome_health3", "0" };

	CVAR_REGISTER( &sk_gonome_dmg_one_slash1 );// { "sk_gonome_dmg_one_slash1", "0" };
	CVAR_REGISTER( &sk_gonome_dmg_one_slash2 );// { "sk_gonome_dmg_one_slash2", "0" };
	CVAR_REGISTER( &sk_gonome_dmg_one_slash3 );// { "sk_gonome_dmg_one_slash3", "0" };

	CVAR_REGISTER( &sk_gonome_dmg_guts1 );// { "sk_gonome_dmg_guts1", "0" };
	CVAR_REGISTER( &sk_gonome_dmg_guts2 );// { "sk_gonome_dmg_guts2", "0" };
	CVAR_REGISTER( &sk_gonome_dmg_guts3 );// { "sk_gonome_dmg_guts3", "0" };

	CVAR_REGISTER( &sk_gonome_dmg_one_bite1 );// { "sk_gonome_dmg_one_bite1", "0" };
	CVAR_REGISTER( &sk_gonome_dmg_one_bite2 );// { "sk_gonome_dmg_one_bite2", "0" };
	CVAR_REGISTER( &sk_gonome_dmg_one_bite3 );// { "sk_gonome_dmg_one_bite3", "0" };
#endif

	//Turret
	CVAR_REGISTER( &sk_turret_health1 );// {"sk_turret_health1","0"};
	CVAR_REGISTER( &sk_turret_health2 );// {"sk_turret_health2","0"};
	CVAR_REGISTER( &sk_turret_health3 );// {"sk_turret_health3","0"};

	// MiniTurret
	CVAR_REGISTER( &sk_miniturret_health1 );// {"sk_miniturret_health1","0"};
	CVAR_REGISTER( &sk_miniturret_health2 );// {"sk_miniturret_health2","0"};
	CVAR_REGISTER( &sk_miniturret_health3 );// {"sk_miniturret_health3","0"};

	// Sentry Turret
	CVAR_REGISTER( &sk_sentry_health1 );// {"sk_sentry_health1","0"};
	CVAR_REGISTER( &sk_sentry_health2 );// {"sk_sentry_health2","0"};
	CVAR_REGISTER( &sk_sentry_health3 );// {"sk_sentry_health3","0"};


	// PLAYER WEAPONS

	// Crowbar whack
	CVAR_REGISTER( &sk_plr_crowbar1 );// {"sk_plr_crowbar1","0"};
	CVAR_REGISTER( &sk_plr_crowbar2 );// {"sk_plr_crowbar2","0"};
	CVAR_REGISTER( &sk_plr_crowbar3 );// {"sk_plr_crowbar3","0"};

	// Glock Round
	CVAR_REGISTER( &sk_plr_9mm_bullet1 );// {"sk_plr_9mm_bullet1","0"};
	CVAR_REGISTER( &sk_plr_9mm_bullet2 );// {"sk_plr_9mm_bullet2","0"};
	CVAR_REGISTER( &sk_plr_9mm_bullet3 );// {"sk_plr_9mm_bullet3","0"};

	// 357 Round
	CVAR_REGISTER( &sk_plr_357_bullet1 );// {"sk_plr_357_bullet1","0"};
	CVAR_REGISTER( &sk_plr_357_bullet2 );// {"sk_plr_357_bullet2","0"};
	CVAR_REGISTER( &sk_plr_357_bullet3 );// {"sk_plr_357_bullet3","0"};

	// MP5 Round
	CVAR_REGISTER( &sk_plr_9mmAR_bullet1 );// {"sk_plr_9mmAR_bullet1","0"};
	CVAR_REGISTER( &sk_plr_9mmAR_bullet2 );// {"sk_plr_9mmAR_bullet2","0"};
	CVAR_REGISTER( &sk_plr_9mmAR_bullet3 );// {"sk_plr_9mmAR_bullet3","0"};

	// M203 grenade
	CVAR_REGISTER( &sk_plr_9mmAR_grenade1 );// {"sk_plr_9mmAR_grenade1","0"};
	CVAR_REGISTER( &sk_plr_9mmAR_grenade2 );// {"sk_plr_9mmAR_grenade2","0"};
	CVAR_REGISTER( &sk_plr_9mmAR_grenade3 );// {"sk_plr_9mmAR_grenade3","0"};

	// Shotgun buckshot
	CVAR_REGISTER( &sk_plr_buckshot1 );// {"sk_plr_buckshot1","0"};
	CVAR_REGISTER( &sk_plr_buckshot2 );// {"sk_plr_buckshot2","0"};
	CVAR_REGISTER( &sk_plr_buckshot3 );// {"sk_plr_buckshot3","0"};

	// Crossbow
	CVAR_REGISTER( &sk_plr_xbow_bolt_monster1 );// {"sk_plr_xbow_bolt1","0"};
	CVAR_REGISTER( &sk_plr_xbow_bolt_monster2 );// {"sk_plr_xbow_bolt2","0"};
	CVAR_REGISTER( &sk_plr_xbow_bolt_monster3 );// {"sk_plr_xbow_bolt3","0"};

	CVAR_REGISTER( &sk_plr_xbow_bolt_client1 );// {"sk_plr_xbow_bolt1","0"};
	CVAR_REGISTER( &sk_plr_xbow_bolt_client2 );// {"sk_plr_xbow_bolt2","0"};
	CVAR_REGISTER( &sk_plr_xbow_bolt_client3 );// {"sk_plr_xbow_bolt3","0"};

	// RPG
	CVAR_REGISTER( &sk_plr_rpg1 );// {"sk_plr_rpg1","0"};
	CVAR_REGISTER( &sk_plr_rpg2 );// {"sk_plr_rpg2","0"};
	CVAR_REGISTER( &sk_plr_rpg3 );// {"sk_plr_rpg3","0"};

	// Gauss Gun
	CVAR_REGISTER( &sk_plr_gauss1 );// {"sk_plr_gauss1","0"};
	CVAR_REGISTER( &sk_plr_gauss2 );// {"sk_plr_gauss2","0"};
	CVAR_REGISTER( &sk_plr_gauss3 );// {"sk_plr_gauss3","0"};

	// Egon Gun
	CVAR_REGISTER( &sk_plr_egon_narrow1 );// {"sk_plr_egon_narrow1","0"};
	CVAR_REGISTER( &sk_plr_egon_narrow2 );// {"sk_plr_egon_narrow2","0"};
	CVAR_REGISTER( &sk_plr_egon_narrow3 );// {"sk_plr_egon_narrow3","0"};

	CVAR_REGISTER( &sk_plr_egon_wide1 );// {"sk_plr_egon_wide1","0"};
	CVAR_REGISTER( &sk_plr_egon_wide2 );// {"sk_plr_egon_wide2","0"};
	CVAR_REGISTER( &sk_plr_egon_wide3 );// {"sk_plr_egon_wide3","0"};

	// Hand Grendade
	CVAR_REGISTER( &sk_plr_hand_grenade1 );// {"sk_plr_hand_grenade1","0"};
	CVAR_REGISTER( &sk_plr_hand_grenade2 );// {"sk_plr_hand_grenade2","0"};
	CVAR_REGISTER( &sk_plr_hand_grenade3 );// {"sk_plr_hand_grenade3","0"};

	// Satchel Charge
	CVAR_REGISTER( &sk_plr_satchel1 );// {"sk_plr_satchel1","0"};
	CVAR_REGISTER( &sk_plr_satchel2 );// {"sk_plr_satchel2","0"};
	CVAR_REGISTER( &sk_plr_satchel3 );// {"sk_plr_satchel3","0"};

	// Tripmine
	CVAR_REGISTER( &sk_plr_tripmine1 );// {"sk_plr_tripmine1","0"};
	CVAR_REGISTER( &sk_plr_tripmine2 );// {"sk_plr_tripmine2","0"};
	CVAR_REGISTER( &sk_plr_tripmine3 );// {"sk_plr_tripmine3","0"};

#if FEATURE_MEDKIT
	// Medkit
	CVAR_REGISTER( &sk_plr_medkitshot1 );
	CVAR_REGISTER( &sk_plr_medkitshot2 );
	CVAR_REGISTER( &sk_plr_medkitshot3 );
	
	CVAR_REGISTER( &sk_plr_medkittime1 );
	CVAR_REGISTER( &sk_plr_medkittime2 );
	CVAR_REGISTER( &sk_plr_medkittime3 );
#endif

#if FEATURE_DESERT_EAGLE
	// Desert Eagle
	CVAR_REGISTER( &sk_plr_eagle1 );// { "sk_plr_eagle1", "0" };
	CVAR_REGISTER( &sk_plr_eagle2 );// { "sk_plr_eagle2", "0" };
	CVAR_REGISTER( &sk_plr_eagle3 );// { "sk_plr_eagle3", "0" };
#endif

#if FEATURE_PIPEWRENCH
	// Pipe wrench
	CVAR_REGISTER( &sk_plr_pipewrench1 );// { "sk_plr_pipewrench1", "0" };
	CVAR_REGISTER( &sk_plr_pipewrench2 );// { "sk_plr_pipewrench2", "0" };
	CVAR_REGISTER( &sk_plr_pipewrench3 );// { "sk_plr_pipewrench3", "0" };
#endif

#if FEATURE_KNIFE
	// Knife
	CVAR_REGISTER( &sk_plr_knife1 );// { "sk_plr_knife1", "0" };
	CVAR_REGISTER( &sk_plr_knife2 );// { "sk_plr_knife2", "0" };
	CVAR_REGISTER( &sk_plr_knife3 );// { "sk_plr_knife3", "0" };
#endif

#if FEATURE_GRAPPLE
	// Grapple
	CVAR_REGISTER( &sk_plr_grapple1 );// { "sk_plr_grapple1", "0" };
	CVAR_REGISTER( &sk_plr_grapple2 );// { "sk_plr_grapple2", "0" };
	CVAR_REGISTER( &sk_plr_grapple3 );// { "sk_plr_grapple3", "0" };
#endif

#if FEATURE_M249
	// M249
	CVAR_REGISTER( &sk_plr_556_bullet1 );// { "sk_plr_556_bullet1", "0" };
	CVAR_REGISTER( &sk_plr_556_bullet2 );// { "sk_plr_556_bullet2", "0" };
	CVAR_REGISTER( &sk_plr_556_bullet3 );// { "sk_plr_556_bullet3", "0" };
#endif

#if FEATURE_SNIPERRIFLE
	// Sniper rifle
	CVAR_REGISTER( &sk_plr_762_bullet1 );// { "sk_plr_762_bullet1", "0" };
	CVAR_REGISTER( &sk_plr_762_bullet2 );// { "sk_plr_762_bullet2", "0" };
	CVAR_REGISTER( &sk_plr_762_bullet3 );// { "sk_plr_762_bullet3", "0" };
#endif

#if FEATURE_SHOCKBEAM
	// Shock rifle
	CVAR_REGISTER( &sk_plr_shockroachs1 );// { "sk_plr_shockroachs1", "0" };
	CVAR_REGISTER( &sk_plr_shockroachs2 );// { "sk_plr_shockroachs2", "0" };
	CVAR_REGISTER( &sk_plr_shockroachs3 );// { "sk_plr_shockroachs3", "0" };

	CVAR_REGISTER( &sk_plr_shockroachm1 );// { "sk_plr_shockroachm1", "0" };
	CVAR_REGISTER( &sk_plr_shockroachm2 );// { "sk_plr_shockroachm2", "0" };
	CVAR_REGISTER( &sk_plr_shockroachm3 );// { "sk_plr_shockroachm3", "0" };
#endif

#if FEATURE_SPOREGRENADE
	CVAR_REGISTER( &sk_plr_spore1 );// { "sk_plr_spore1", "0" };
	CVAR_REGISTER( &sk_plr_spore2 );// { "sk_plr_spore2", "0" };
	CVAR_REGISTER( &sk_plr_spore3 );// { "sk_plr_spore3", "0" };
#endif

#if FEATURE_DISPLACER
	CVAR_REGISTER( &sk_plr_displacer1 );
	CVAR_REGISTER( &sk_plr_displacer2 );
	CVAR_REGISTER( &sk_plr_displacer3 );

	CVAR_REGISTER( &sk_plr_displacer_radius1 );
	CVAR_REGISTER( &sk_plr_displacer_radius2 );
	CVAR_REGISTER( &sk_plr_displacer_radius3 );
#endif

#if FEATURE_UZI
	// Glock Round
	CVAR_REGISTER( &sk_plr_uzi1 );// {"sk_plr_uzi1","0"};
	CVAR_REGISTER( &sk_plr_uzi2 );// {"sk_plr_uzi2","0"};
	CVAR_REGISTER( &sk_plr_uzi3 );// {"sk_plr_uzi3","0"};
#endif

	// WORLD WEAPONS
	CVAR_REGISTER( &sk_12mm_bullet1 );// {"sk_12mm_bullet1","0"};
	CVAR_REGISTER( &sk_12mm_bullet2 );// {"sk_12mm_bullet2","0"};
	CVAR_REGISTER( &sk_12mm_bullet3 );// {"sk_12mm_bullet3","0"};

	CVAR_REGISTER( &sk_9mmAR_bullet1 );// {"sk_9mm_bullet1","0"};
	CVAR_REGISTER( &sk_9mmAR_bullet2 );// {"sk_9mm_bullet1","0"};
	CVAR_REGISTER( &sk_9mmAR_bullet3 );// {"sk_9mm_bullet1","0"};

	CVAR_REGISTER( &sk_9mm_bullet1 );// {"sk_9mm_bullet1","0"};
	CVAR_REGISTER( &sk_9mm_bullet2 );// {"sk_9mm_bullet2","0"};
	CVAR_REGISTER( &sk_9mm_bullet3 );// {"sk_9mm_bullet3","0"};

	CVAR_REGISTER( &sk_357_bullet1 );// {"sk_357_bullet1","0"};
	CVAR_REGISTER( &sk_357_bullet2 );// {"sk_357_bullet2","0"};
	CVAR_REGISTER( &sk_357_bullet3 );// {"sk_357_bullet3","0"};

	CVAR_REGISTER( &sk_556_bullet1 );// { "sk_556_bullet1", "0" };
	CVAR_REGISTER( &sk_556_bullet2 );// { "sk_556_bullet2", "0" };
	CVAR_REGISTER( &sk_556_bullet3 );// { "sk_556_bullet3", "0" };

	CVAR_REGISTER( &sk_762_bullet1 );// {"sk_762_bullet1","0"};
	CVAR_REGISTER( &sk_762_bullet2 );// {"sk_762_bullet2","0"};
	CVAR_REGISTER( &sk_762_bullet3 );// {"sk_762_bullet3","0"};

	// HORNET
	CVAR_REGISTER( &sk_hornet_dmg1 );// {"sk_hornet_dmg1","0"};
	CVAR_REGISTER( &sk_hornet_dmg2 );// {"sk_hornet_dmg2","0"};
	CVAR_REGISTER( &sk_hornet_dmg3 );// {"sk_hornet_dmg3","0"};

	CVAR_REGISTER( &sk_plr_hornet_dmg1 );
	CVAR_REGISTER( &sk_plr_hornet_dmg2 );
	CVAR_REGISTER( &sk_plr_hornet_dmg3 );

	// MORTAR
	CVAR_REGISTER( &sk_mortar1 );
	CVAR_REGISTER( &sk_mortar2 );
	CVAR_REGISTER( &sk_mortar3 );

	// HEALTH/SUIT CHARGE DISTRIBUTION
	CVAR_REGISTER( &sk_suitcharger1 );
	CVAR_REGISTER( &sk_suitcharger2 );
	CVAR_REGISTER( &sk_suitcharger3 );

	CVAR_REGISTER( &sk_battery1 );
	CVAR_REGISTER( &sk_battery2 );
	CVAR_REGISTER( &sk_battery3 );

	CVAR_REGISTER( &sk_healthcharger1 );
	CVAR_REGISTER( &sk_healthcharger2 );
	CVAR_REGISTER( &sk_healthcharger3 );

	CVAR_REGISTER( &sk_healthkit1 );
	CVAR_REGISTER( &sk_healthkit2 );
	CVAR_REGISTER( &sk_healthkit3 );

	CVAR_REGISTER( &sk_scientist_heal1 );
	CVAR_REGISTER( &sk_scientist_heal2 );
	CVAR_REGISTER( &sk_scientist_heal3 );

	CVAR_REGISTER( &sk_scientist_heal_time1 );
	CVAR_REGISTER( &sk_scientist_heal_time2 );
	CVAR_REGISTER( &sk_scientist_heal_time3 );
	
	CVAR_REGISTER( &sk_soda1 );
	CVAR_REGISTER( &sk_soda2 );
	CVAR_REGISTER( &sk_soda3 );

	// monster damage adjusters
	CVAR_REGISTER( &sk_monster_head1 );
	CVAR_REGISTER( &sk_monster_head2 );
	CVAR_REGISTER( &sk_monster_head3 );

	CVAR_REGISTER( &sk_monster_chest1 );
	CVAR_REGISTER( &sk_monster_chest2 );
	CVAR_REGISTER( &sk_monster_chest3 );

	CVAR_REGISTER( &sk_monster_stomach1 );
	CVAR_REGISTER( &sk_monster_stomach2 );
	CVAR_REGISTER( &sk_monster_stomach3 );

	CVAR_REGISTER( &sk_monster_arm1 );
	CVAR_REGISTER( &sk_monster_arm2 );
	CVAR_REGISTER( &sk_monster_arm3 );

	CVAR_REGISTER( &sk_monster_leg1 );
	CVAR_REGISTER( &sk_monster_leg2 );
	CVAR_REGISTER( &sk_monster_leg3 );

	// player damage adjusters
	CVAR_REGISTER( &sk_player_head1 );
	CVAR_REGISTER( &sk_player_head2 );
	CVAR_REGISTER( &sk_player_head3 );

	CVAR_REGISTER( &sk_player_chest1 );
	CVAR_REGISTER( &sk_player_chest2 );
	CVAR_REGISTER( &sk_player_chest3 );

	CVAR_REGISTER( &sk_player_stomach1 );
	CVAR_REGISTER( &sk_player_stomach2 );
	CVAR_REGISTER( &sk_player_stomach3 );

	CVAR_REGISTER( &sk_player_arm1 );
	CVAR_REGISTER( &sk_player_arm2 );
	CVAR_REGISTER( &sk_player_arm3 );

	CVAR_REGISTER( &sk_player_leg1 );
	CVAR_REGISTER( &sk_player_leg2 );
	CVAR_REGISTER( &sk_player_leg3 );

	CVAR_REGISTER( &sk_flashlight_drain_time1 );
	CVAR_REGISTER( &sk_flashlight_drain_time2 );
	CVAR_REGISTER( &sk_flashlight_drain_time3 );

	CVAR_REGISTER( &sk_flashlight_charge_time1 );
	CVAR_REGISTER( &sk_flashlight_charge_time2 );
	CVAR_REGISTER( &sk_flashlight_charge_time3 );
// END REGISTER CVARS FOR SKILL LEVEL STUFF

	SERVER_COMMAND( "exec skill.cfg\n" );
#if FEATURE_OPFOR_SKILL
	SERVER_COMMAND( "exec skillopfor.cfg\n" );
#endif
}

