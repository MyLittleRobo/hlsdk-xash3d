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
/*

===== items.cpp ========================================================

  functions governing the selection/use of weapons for players

*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "weapons.h"
#include "player.h"
#include "skill.h"
#include "items.h"
#include "gamerules.h"

extern int gmsgItemPickup;

class CWorldItem : public CBaseEntity
{
public:
	void KeyValue( KeyValueData *pkvd ); 
	void Spawn( void );
	int m_iType;
};

LINK_ENTITY_TO_CLASS( world_items, CWorldItem )

void CWorldItem::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "type" ) )
	{
		m_iType = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else
		CBaseEntity::KeyValue( pkvd );
}

void CWorldItem::Spawn( void )
{
	CBaseEntity *pEntity = NULL;

	switch( m_iType ) 
	{
	case 44: // ITEM_BATTERY:
		pEntity = CBaseEntity::Create( "item_battery", pev->origin, pev->angles );
		break;
	case 42: // ITEM_ANTIDOTE:
		pEntity = CBaseEntity::Create( "item_antidote", pev->origin, pev->angles );
		break;
	case 43: // ITEM_SECURITY:
		pEntity = CBaseEntity::Create( "item_security", pev->origin, pev->angles );
		break;
	case 45: // ITEM_SUIT:
		pEntity = CBaseEntity::Create( "item_suit", pev->origin, pev->angles );
		break;
	}

	if( !pEntity )
	{
		ALERT( at_console, "unable to create world_item %d\n", m_iType );
	}
	else
	{
		pEntity->pev->target = pev->target;
		pEntity->pev->targetname = pev->targetname;
		pEntity->pev->spawnflags = pev->spawnflags;
	}

	REMOVE_ENTITY( edict() );
}

void CItem::Spawn( void )
{
	pev->movetype = MOVETYPE_TOSS;
	pev->solid = SOLID_TRIGGER;
	UTIL_SetOrigin( pev, pev->origin );
	UTIL_SetSize( pev, Vector( -16, -16, 0 ), Vector( 16, 16, 16 ) );
	SetTouch( &CItem::ItemTouch );

	if( DROP_TO_FLOOR(ENT( pev ) ) == 0 )
	{
		ALERT(at_error, "Item %s fell out of level at %f,%f,%f\n", STRING( pev->classname ), pev->origin.x, pev->origin.y, pev->origin.z);
		UTIL_Remove( this );
		return;
	}
}

extern int gEvilImpulse101;

void CItem::ItemTouch( CBaseEntity *pOther )
{
	// if it's not a player, ignore
	if( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	// ok, a player is touching this item, but can he have it?
	if( !g_pGameRules->CanHaveItem( pPlayer, this ) )
	{
		// no? Ignore the touch.
		return;
	}

	if( MyTouch( pPlayer ) )
	{
		SUB_UseTargets( pOther, USE_TOGGLE, 0 );
		SetTouch( NULL );
		
		// player grabbed the item. 
		g_pGameRules->PlayerGotItem( pPlayer, this );
		if( g_pGameRules->ItemShouldRespawn( this ) == GR_ITEM_RESPAWN_YES )
		{
			Respawn(); 
		}
		else
		{
			UTIL_Remove( this );
		}
	}
	else if( gEvilImpulse101 )
	{
		UTIL_Remove( this );
	}
}

CBaseEntity* CItem::Respawn( void )
{
	SetTouch( NULL );
	pev->effects |= EF_NODRAW;

	UTIL_SetOrigin( pev, g_pGameRules->VecItemRespawnSpot( this ) );// blip to whereever you should respawn.

	SetThink( &CItem::Materialize );
	pev->nextthink = g_pGameRules->FlItemRespawnTime( this ); 
	return this;
}

void CItem::Materialize( void )
{
	if( pev->effects & EF_NODRAW )
	{
		// changing from invisible state to visible.
		EMIT_SOUND_DYN( ENT( pev ), CHAN_WEAPON, "items/suitchargeok1.wav", 1, ATTN_NORM, 0, 150 );
		pev->effects &= ~EF_NODRAW;
		pev->effects |= EF_MUZZLEFLASH;
	}

	SetTouch( &CItem::ItemTouch );
	SetThink( NULL );
}

void CItem::SetMyModel(const char *model)
{
	if (FStringNull(pev->model)) {
		SET_MODEL( ENT( pev ), model );
	} else {
		SET_MODEL( ENT( pev ), STRING(pev->model) );
	}
}

void CItem::PrecacheMyModel(const char *model)
{
	if (FStringNull(pev->model)) {
		PRECACHE_MODEL( model );
	} else {
		PRECACHE_MODEL( STRING( pev->model ) );
	}
}

#define SF_SUIT_SHORTLOGON		0x0001
#define SF_SUIT_NOLOGON		0x0002

class CItemSuit : public CItem
{
	void Spawn( void )
	{
		Precache();
		SetMyModel( "models/w_suit.mdl" );
		CItem::Spawn();
	}
	void Precache( void )
	{
		PrecacheMyModel( "models/w_suit.mdl" );
	}
	BOOL MyTouch( CBasePlayer *pPlayer )
	{
		if( pPlayer->pev->weapons & ( 1<<WEAPON_SUIT ) )
			return FALSE;

		if ( pev->spawnflags & SF_SUIT_NOLOGON )
		{
			// pass
		}
		else if( pev->spawnflags & SF_SUIT_SHORTLOGON )
		{
			EMIT_SOUND_SUIT( pPlayer->edict(), "!HEV_A0" );		// short version of suit logon,
		}
		else
		{
			EMIT_SOUND_SUIT( pPlayer->edict(), "!HEV_AAx" );	// long version of suit logon
		}

		pPlayer->pev->weapons |= ( 1 << WEAPON_SUIT );
		return TRUE;
	}
};

LINK_ENTITY_TO_CLASS( item_suit, CItemSuit )

class CItemBattery : public CItem
{
	void Spawn( void )
	{
		Precache();
		SetMyModel( "models/w_battery.mdl" );
		CItem::Spawn();
	}
	void Precache( void )
	{
		PrecacheMyModel( "models/w_battery.mdl" );
		PRECACHE_SOUND( "items/gunpickup2.wav" );
	}
	BOOL MyTouch( CBasePlayer *pPlayer )
	{
		if( pPlayer->pev->deadflag != DEAD_NO )
		{
			return FALSE;
		}

		if( ( pPlayer->pev->armorvalue < MAX_NORMAL_BATTERY ) &&
			( pPlayer->pev->weapons & ( 1 << WEAPON_SUIT ) ) )
		{
			int pct;
			char szcharge[64];

			pPlayer->pev->armorvalue += gSkillData.batteryCapacity;
			pPlayer->pev->armorvalue = Q_min( pPlayer->pev->armorvalue, MAX_NORMAL_BATTERY );

			EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );

			MESSAGE_BEGIN( MSG_ONE, gmsgItemPickup, NULL, pPlayer->pev );
				WRITE_STRING( STRING( pev->classname ) );
			MESSAGE_END();

			// Suit reports new power level
			// For some reason this wasn't working in release build -- round it.
			pct = (int)( (float)( pPlayer->pev->armorvalue * 100.0 ) * ( 1.0 / MAX_NORMAL_BATTERY ) + 0.5 );
			pct = ( pct / 5 );
			if( pct > 0 )
				pct--;

			sprintf( szcharge,"!HEV_%1dP", pct );

			//EMIT_SOUND_SUIT( ENT( pev ), szcharge );
			pPlayer->SetSuitUpdate( szcharge, FALSE, SUIT_NEXT_IN_30SEC);
			return TRUE;
		}
		return FALSE;
	}
};

LINK_ENTITY_TO_CLASS( item_battery, CItemBattery )

class CItemAntidote : public CItem
{
	void Spawn( void )
	{
		Precache();
		SetMyModel( "models/w_antidote.mdl" );
		CItem::Spawn();
	}
	void Precache( void )
	{
		PrecacheMyModel( "models/w_antidote.mdl" );
	}
	BOOL MyTouch( CBasePlayer *pPlayer )
	{
		if( pPlayer->pev->deadflag != DEAD_NO )
		{
			return FALSE;
		}
		pPlayer->SetSuitUpdate( "!HEV_DET4", FALSE, SUIT_NEXT_IN_1MIN );

		pPlayer->m_rgItems[ITEM_ANTIDOTE] += 1;
		return TRUE;
	}
};

LINK_ENTITY_TO_CLASS( item_antidote, CItemAntidote )

class CItemSecurity : public CItem
{
	void Spawn( void )
	{
		Precache();
		SetMyModel( "models/w_security.mdl" );
		CItem::Spawn();
	}
	void Precache( void )
	{
		PrecacheMyModel( "models/w_security.mdl" );
		if (!FStringNull(pev->noise))
			PRECACHE_SOUND( STRING(pev->noise) );
	}
	BOOL MyTouch( CBasePlayer *pPlayer )
	{
		if( pPlayer->pev->deadflag != DEAD_NO )
		{
			return FALSE;
		}
		pPlayer->m_rgItems[ITEM_SECURITY] += 1;

		if (!FStringNull(pev->noise))
			EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, STRING(pev->noise), 1, ATTN_NORM );
		MESSAGE_BEGIN( MSG_ONE, gmsgItemPickup, NULL, pPlayer->pev );
			WRITE_STRING( STRING( pev->classname ) );
		MESSAGE_END();
		if (!FStringNull(pev->message))
			UTIL_ShowMessage( STRING( pev->message ), pPlayer );

		return TRUE;
	}
};

LINK_ENTITY_TO_CLASS( item_security, CItemSecurity )

class CItemLongJump : public CItem
{
	void Spawn( void )
	{ 
		Precache();
		SetMyModel( "models/w_longjump.mdl" );
		CItem::Spawn();
	}
	void Precache( void )
	{
		PrecacheMyModel( "models/w_longjump.mdl" );
	}
	BOOL MyTouch( CBasePlayer *pPlayer )
	{
		if( pPlayer->m_fLongJump )
		{
			return FALSE;
		}

		if( ( pPlayer->pev->weapons & ( 1 << WEAPON_SUIT ) ) )
		{
			pPlayer->m_fLongJump = TRUE;// player now has longjump module

			g_engfuncs.pfnSetPhysicsKeyValue( pPlayer->edict(), "slj", "1" );

			MESSAGE_BEGIN( MSG_ONE, gmsgItemPickup, NULL, pPlayer->pev );
				WRITE_STRING( STRING( pev->classname ) );
			MESSAGE_END();

			EMIT_SOUND_SUIT( pPlayer->edict(), "!HEV_A1" );	// Play the longjump sound UNDONE: Kelly? correct sound?
			return TRUE;		
		}
		return FALSE;
	}
};

LINK_ENTITY_TO_CLASS( item_longjump, CItemLongJump )

//=========================================================
// Generic item
//=========================================================
class CItemGeneric : public CBaseAnimating
{
public:
	int		Save(CSave &save);
	int		Restore(CRestore &restore);

	static	TYPEDESCRIPTION m_SaveData[];

	void Spawn(void);
	void Precache(void);
	void KeyValue(KeyValueData* pkvd);

	void EXPORT StartupThink(void);
	void EXPORT SequenceThink(void);

	string_t m_iszSequenceName;
};

LINK_ENTITY_TO_CLASS(item_generic, CItemGeneric)

TYPEDESCRIPTION CItemGeneric::m_SaveData[] =
{
	DEFINE_FIELD(CItemGeneric, m_iszSequenceName, FIELD_STRING),
};
IMPLEMENT_SAVERESTORE(CItemGeneric, CBaseAnimating)

void CItemGeneric::Spawn(void)
{
	Precache();
	if (FStringNull(pev->model))
	{
		ALERT(at_console, "Spawning item_generic without model!\n");
	}
	else
	{
		SET_MODEL(ENT(pev), STRING(pev->model));
	}

	UTIL_SetOrigin(pev, pev->origin);
	UTIL_SetSize(pev, Vector(-16, -16, 0), Vector(16, 16, 32));

	pev->takedamage	 = DAMAGE_NO;
	pev->solid		 = SOLID_NOT;
	pev->sequence	 = -1;

	// Call startup sequence to look for a sequence to play.
	SetThink(&CItemGeneric::StartupThink);
	pev->nextthink = gpGlobals->time + 0.1f;
}

void CItemGeneric::Precache(void)
{
	if (!FStringNull(pev->model))
		PRECACHE_MODEL(STRING(pev->model));
}

void CItemGeneric::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "sequencename"))
	{
		m_iszSequenceName = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
		CBaseAnimating::KeyValue(pkvd);
}

void CItemGeneric::StartupThink(void)
{
	// Try to look for a sequence to play.
	int iSequence = -1;
	iSequence = LookupSequence(STRING(m_iszSequenceName));

	// Validate sequence.
	if (iSequence != -1)
	{
		pev->sequence = iSequence;
		SetThink(&CItemGeneric::SequenceThink);
		pev->nextthink = gpGlobals->time + 0.01f;
	}
	else
	{
		// Cancel play sequence.
		SetThink(NULL);
	}
}

void CItemGeneric::SequenceThink(void)
{
	// Set next think time.
	pev->nextthink = gpGlobals->time + 0.1f;

	// Advance frames and dispatch events.
	StudioFrameAdvance();
	DispatchAnimEvents();

	// Restart sequence
	if (m_fSequenceFinished)
	{
		pev->frame = 0;
		ResetSequenceInfo();

		if (!m_fSequenceLoops)
		{
			// Prevent from calling ItemThink.
			SetThink(NULL);
			m_fSequenceFinished = TRUE;
			return;
		}
		else
		{
			pev->frame = 0;
			ResetSequenceInfo();
		}
	}
}

// Derive from CBaseMonster to use SetActivity
class CEyeScanner : public CBaseMonster
{
public:
	void KeyValue( KeyValueData *pkvd );
	void Spawn();
	void Precache(void);
	void EXPORT PlayBeep();
	void EXPORT WaitForSequenceEnd();
	int ObjectCaps( void ) { return CBaseMonster::ObjectCaps() | FCAP_IMPULSE_USE; }
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	int TakeDamage(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType);
	int Classify();

	virtual int Save( CSave &save );
	virtual int Restore( CRestore &restore );

	static TYPEDESCRIPTION m_SaveData[];

	string_t unlockedTarget;
	string_t lockedTarget;
	string_t unlockerName;
	string_t activatorName;
};

TYPEDESCRIPTION CEyeScanner::m_SaveData[] =
{
	DEFINE_FIELD( CEyeScanner, unlockedTarget, FIELD_STRING ),
	DEFINE_FIELD( CEyeScanner, lockedTarget, FIELD_STRING ),
	DEFINE_FIELD( CEyeScanner, unlockerName, FIELD_STRING ),
	DEFINE_FIELD( CEyeScanner, activatorName, FIELD_STRING ),
};

IMPLEMENT_SAVERESTORE( CEyeScanner, CBaseMonster )

LINK_ENTITY_TO_CLASS( item_eyescanner, CEyeScanner )

void CEyeScanner::KeyValue(KeyValueData *pkvd)
{
	if (FStrEq(pkvd->szKeyName, "unlocked_target"))
	{
		unlockedTarget = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "locked_target"))
	{
		lockedTarget = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "unlockersname"))
	{
		unlockerName = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "reset_delay")) // Dunno if it affects anything in PC version of Decay
	{
		m_flWait = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
		CBaseMonster::KeyValue( pkvd );
}

void CEyeScanner::Spawn()
{
	Precache();
	pev->solid = SOLID_SLIDEBOX;
	pev->movetype = MOVETYPE_FLY;
	pev->takedamage = DAMAGE_NO;
	pev->health = 1;
	pev->weapons = 0;

	SET_MODEL(ENT(pev), "models/EYE_SCANNER.mdl");
	UTIL_SetOrigin(pev, pev->origin);
	UTIL_SetSize(pev, Vector(-12, -16, 0), Vector(12, 16, 48));
	SetActivity(ACT_CROUCHIDLE);
	ResetSequenceInfo();
	SetThink(NULL);
}

void CEyeScanner::Precache()
{
	PRECACHE_MODEL("models/EYE_SCANNER.mdl");
	PRECACHE_SOUND("buttons/blip1.wav");
	PRECACHE_SOUND("buttons/blip2.wav");
	PRECACHE_SOUND("buttons/button11.wav");
}

void CEyeScanner::PlayBeep()
{
	pev->skin = pev->weapons % 3 + 1;
	pev->weapons++;
	if (pev->weapons < 10) {
		EMIT_SOUND( ENT(pev), CHAN_VOICE, "buttons/blip1.wav", 1, ATTN_NORM );
		pev->nextthink = gpGlobals->time + 0.125;
	} else {
		pev->skin = 0;
		pev->weapons = 0;
		if (FStringNull(unlockerName) || (!FStringNull(activatorName) && FStrEq(STRING(unlockerName), STRING(activatorName)))) {
			EMIT_SOUND( ENT(pev), CHAN_VOICE, "buttons/blip2.wav", 1, ATTN_NORM );
			FireTargets( STRING( unlockedTarget ), this, this, USE_TOGGLE, 0.0f );
		} else {
			EMIT_SOUND( ENT(pev), CHAN_VOICE, "buttons/button11.wav", 1, ATTN_NORM );
			FireTargets( STRING( lockedTarget ), this, this, USE_TOGGLE, 0.0f );
		}
		activatorName = iStringNull;
		SetActivity(ACT_CROUCH);
		ResetSequenceInfo();
		SetThink(&CEyeScanner::WaitForSequenceEnd);
		pev->nextthink = gpGlobals->time + 0.1;
	}
}

void CEyeScanner::WaitForSequenceEnd()
{
	if (m_fSequenceFinished) {
		if (m_Activity == ACT_STAND) {
			SetActivity(ACT_IDLE);
			SetThink(&CEyeScanner::PlayBeep);
			pev->nextthink = gpGlobals->time;
		} else if (m_Activity == ACT_CROUCH) {
			SetActivity(ACT_CROUCHIDLE);
			SetThink(NULL);
		}
		ResetSequenceInfo();
	} else {
		StudioFrameAdvance(0.1);
		pev->nextthink = gpGlobals->time + 0.1;
	}
}

void CEyeScanner::Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	if (m_Activity == ACT_CROUCHIDLE) {
		pActivator = pActivator ? pActivator : pCaller;
		activatorName = pActivator ? pActivator->pev->targetname : iStringNull;
		SetActivity( ACT_STAND );
		ResetSequenceInfo();
		SetThink(&CEyeScanner::WaitForSequenceEnd);
		pev->nextthink = gpGlobals->time + 0.1;
	}
}

int CEyeScanner::Classify()
{
	return CLASS_NONE;
}

int CEyeScanner::TakeDamage(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType)
{
	return 0;
}
