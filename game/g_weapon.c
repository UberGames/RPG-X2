// Copyright (C) 1999-2000 Id Software, Inc.
//
// g_weapon.c 
// perform the server side effects of a weapon firing

#include "g_local.h"

static	float	s_quadFactor;
static	vec3_t	forward, right, up;
static	vec3_t	muzzle;

extern void G_MissileImpact( gentity_t *ent, trace_t *trace);

extern int	borgQueenClientNum;

#define MAX_BEAM_HITS	4

#define DMG_VAR			(flrandom(0.8,1.2))


// Weapon damages are located up here for easy access...
// Phaser
#define	PHASER_DAMAGE				55 //4 //RPG-X: TiM - Increased to a standard 0.5 second burst - Phenix GOING DOWN - TiM GOING UP we had complaints when this was put down :P //2
#define PHASER_ALT_RADIUS			80 //12 //RPG-X: TiM - Increased to a near instant kill

// Compression Rifle
#define	CRIFLE_DAMAGE				75		//10 // 20 //RPG-X: TiM - Increased to 2 hits kill (needs debate like TR-116 :P ) //20
#define CRIFLE_MAIN_SPLASH_RADIUS	64
#define CRIFLE_MAIN_SPLASH_DMG		0//16		// 20
#define CRIFLE_ALTDAMAGE			16 //85		// 100
#define CRIFLE_ALT_SPLASH_RADIUS	32
#define CRIFLE_ALT_SPLASH_DMG		0//10

// Imod
#define	IMOD_DAMAGE					10		// 32
#define	IMOD_ALT_DAMAGE				10

// Scavenger Rifle
#define SCAV_DAMAGE					8		// 16
#define SCAV_ALT_DAMAGE				60		// 60
#define SCAV_ALT_SPLASH_RAD			100
#define SCAV_ALT_SPLASH_DAM			60		// 60	

// Stasis Weapon
#define STASIS_DAMAGE				80		// 15 //7 //RPG-X: TiM - Increased to 2 hits kill (needs debate like TR-116 :P ) //15
#define STASIS_ALT_DAMAGE			7		// 40
#define STASIS_ALT_DAMAGE2			7		// 20

// Grenade Launcher
#define GRENADE_DAMAGE				75		// 100
#define GRENADE_SPLASH_RAD			190		//RPG-X: RedTechie - Before 160
#define GRENADE_SPLASH_DAM			100		//RPG-X: RedTechie - Before 75
#define GRENADE_ALT_DAMAGE			80		//RPG-X: RedTechie - Before 64

// Tetrion Disruptor
#define TETRION_DAMAGE				150		//RPG-X: J2J - Increased for one shot one kill (needs debate)
#define TETRION_ALT_DAMAGE			17		//RPG-X: J2J - Not used anymore for TR-116

// Quantum Burst
#define QUANTUM_DAMAGE				140		// 85		// 100 
#define QUANTUM_SPLASH_DAM			140		// 85		// 128
#define QUANTUM_SPLASH_RAD			160
#define QUANTUM_ALT_DAMAGE			140		// 75		// 100
#define QUANTUM_ALT_SPLASH_DAM		140		// 75		// 128
#define QUANTUM_ALT_SPLASH_RAD		80

// Dreadnought Weapon
#define DREADNOUGHT_DAMAGE			8		// 6
#define DREADNOUGHT_WIDTH			9		// 12
#define DREADNOUGHT_ALTDAMAGE		35		// 40

// Borg Weapon
#define BORG_PROJ_DAMAGE			20
#define BORG_TASER_DAMAGE			15

/*
======================
SnapVectorTowards

Round a vector to integers for more efficient network
transmission, but make sure that it rounds towards a given point
rather than blindly truncating.  This prevents it from truncating 
into a wall.
======================
*/
void SnapVectorTowards( vec3_t v, vec3_t to ) {
	int		i;

	for ( i = 0 ; i < 3 ; i++ ) {
		if ( to[i] <= v[i] ) {
			v[i] = (int)v[i];
		} else {
			v[i] = (int)v[i] + 1;
		}
	}
}




/*
----------------------------------------------
	PLAYER WEAPONS
----------------------------------------------


----------------------------------------------
	PHASER
----------------------------------------------
*/

#define MAXRANGE_PHASER			2048		// This is the same as the range MAX_BEAM_RANGE	2048
#define NUM_PHASER_TRACES 3

#define BEAM_VARIATION		6

#define PHASER_POINT_BLANK			96
#define PHASER_POINT_BLANK_FRAC		((float)PHASER_POINT_BLANK / (float)MAXRANGE_PHASER)


//RPG-X | GSIO01 | 09/05/2009 SOE
#define HYPERSPANNER_RATE		2
#define HYPERSPANNER_ALT_RATE	4
void WP_FireHyperspanner(gentity_t *ent, qboolean alt_fire) {
	//trace_t		tr;
	//vec3_t		end;
	//gentity_t	*traceEnt;
	float		modifier;
	//trap_Trace(&tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT);
	//traceEnt = &g_entities[tr.entityNum];
	//if(!traceEnt)
	//	return;
	if(rpg_repairModifier.value < 0)
		modifier = 1;
	else
		modifier = rpg_repairModifier.value;
	if(alt_fire)
		G_Repair(ent, HYPERSPANNER_ALT_RATE * modifier);
	else
		G_Repair(ent, HYPERSPANNER_RATE * modifier);
}
//RPG-X | GSIO01 | 09/05/2009 EOE

//---------------------------------------------------------
void WP_FirePhaser( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	trace_t		tr;
	vec3_t		end;
	gentity_t	*traceEnt;
	int			trEnts[NUM_PHASER_TRACES], i = 0;
	float		trEntFraction[NUM_PHASER_TRACES];
	int			damage = 0;

	VectorMA (muzzle, MAXRANGE_PHASER, forward, end);
	// Add a subtle variation to the beam weapon's endpoint
	for (i = 0; i < 3; i ++ )
	{
		end[i] += crandom() * BEAM_VARIATION;
	}

	for (i = 0; i < NUM_PHASER_TRACES; i++)
	{
		trEnts[i] = -1;
		trEntFraction[i] = 0.0;
	}
	// Find out who we've hit
//	gi.trace ( &tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT);
	trap_Trace (&tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT );
	if (tr.entityNum != (MAX_GENTITIES-1))
	{
		trEnts[0] = tr.entityNum;
		trEntFraction[0] = tr.fraction;
	}
	if ( alt_fire && ent->client->ps.ammo[WP_5])
	{	// Use the ending point of the thin trace to do two more traces, one on either side, for actual damaging effect.
		vec3_t	vUp = {0,0,1}, vRight, start2, end2;
		float	halfBeamWidth = PHASER_ALT_RADIUS;

		CrossProduct(forward, vUp, vRight);
		VectorNormalize(vRight);
		VectorMA(muzzle, halfBeamWidth, vRight, start2);
		VectorMA(end, halfBeamWidth, vRight, end2);
		VectorCopy(tr.endpos, end);
		trap_Trace (&tr, muzzle, NULL, NULL, end, ent->s.number, (CONTENTS_PLAYERCLIP|CONTENTS_BODY) );
		if (	(tr.entityNum != (MAX_GENTITIES-1)) &&
				(tr.entityNum != trEnts[0]) )
		{
			trEnts[1] = tr.entityNum;
			trEntFraction[1] = tr.fraction;
		}
		VectorMA(muzzle, -halfBeamWidth, vRight, start2);
		VectorMA(end, -halfBeamWidth, vRight, end2);
		trap_Trace (&tr, muzzle, NULL, NULL, end, ent->s.number, (CONTENTS_PLAYERCLIP|CONTENTS_BODY) );
		if (	(tr.entityNum != (MAX_GENTITIES-1)) &&
				(tr.entityNum != trEnts[0]) &&
				(tr.entityNum != trEnts[1]))
		{
			trEnts[2] = tr.entityNum;
			trEntFraction[2] = tr.fraction;
		}
	}

	for (i = 0; i < NUM_PHASER_TRACES; i++)
	{
		if (-1 == trEnts[i])
		{
			continue;
		}
		traceEnt = &g_entities[ trEnts[i] ];

		if ( traceEnt->takedamage && rpg_phaserdmg.integer != 0 ) 
		{
//			damage = (float)PHASER_DAMAGE*DMG_VAR*s_quadFactor;		// No variance on phaser
			damage = (float)PHASER_DAMAGE*s_quadFactor;

			if (trEntFraction[i] <= PHASER_POINT_BLANK_FRAC)
			{	// Point blank!  Do up to double damage.
				damage += damage * (1.0 - (trEntFraction[i]/PHASER_POINT_BLANK_FRAC));
			}
			else
			{	// Normal range
				damage -= (int)(trEntFraction[i]*5.0);
			}

			if (!ent->client->ps.ammo[WP_5])
			{
				damage *= .35; // weak out-of-ammo phaser
			}
			
			if (damage > 0)
			{
				if ( alt_fire /*|| ent->client->ps.ammo[WP_5]*/) //RPG-X: RedTechie - Im stupid i dident see this this is why it kept doing alt fire
				{
					G_Damage( traceEnt, ent, ent, forward, tr.endpos, damage, 
								DAMAGE_NO_KNOCKBACK | DAMAGE_NOT_ARMOR_PIERCING, MOD_PHASER_ALT );
				}
				else
				{
					G_Damage( traceEnt, ent, ent, forward, tr.endpos, damage, 
								DAMAGE_NO_KNOCKBACK | DAMAGE_ARMOR_PIERCING, MOD_PHASER );
				}
			}
		}
	}
}


/*
----------------------------------------------
	COMPRESSION RIFLE
----------------------------------------------
*/

#define COMPRESSION_SPREAD	100
#define MAXRANGE_CRIFLE		8192

#define CRIFLE_SIZE			1  //10  //RPG-X | Marcin | 04/12/2008
// not used:
#define CRIFLE_VELOCITY		2700 //2500

void FirePrifleBullet( gentity_t *ent, vec3_t start, vec3_t dir )
//---------------------------------------------------------
{
	gentity_t	*bolt;

	bolt = G_Spawn();
	
	bolt->classname = "prifle_proj";
	bolt->nextthink = level.time + 10000;
	bolt->think = G_FreeEntity;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_6;
	bolt->r.ownerNum = ent->s.number;
	bolt->parent = ent;

//fixme  - remove
	{
		// Flags effect as being the full beefy version for the player
		bolt->count = 0;
	}

	if( rpg_rifledmg.integer != 0 ) 
		bolt->damage = CRIFLE_DAMAGE*DMG_VAR*s_quadFactor;
	else
		bolt->damage = 0;

	bolt->splashDamage = 0;
	bolt->splashRadius = 0;
	bolt->methodOfDeath = MOD_CRIFLE;
	bolt->clipmask = MASK_SHOT;

	// Set the size of the missile up
	VectorSet(bolt->r.maxs, CRIFLE_SIZE>>1, CRIFLE_SIZE, CRIFLE_SIZE>>1);
	VectorSet(bolt->r.mins, -CRIFLE_SIZE>>1, -CRIFLE_SIZE, -CRIFLE_SIZE>>1);

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time - 10; //10		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	SnapVector( bolt->s.pos.trBase );			// save net bandwidth
	VectorScale( dir, rpg_rifleSpeed.integer, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
	VectorCopy( start, bolt->r.currentOrigin);
}

//---------------------------------------------------------
void WP_FireCompressionRifle ( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	if ( !alt_fire )
	{
		vec3_t		dir, angles, temp_ang, temp_org;
		vec3_t		start;
		float		offset;

		VectorCopy( forward, dir );
		VectorCopy( muzzle, start );


		vectoangles( dir, angles );
		VectorSet( temp_ang, angles[0] /*+ (crandom() * 1)*/, angles[1] /*+ (crandom() * 1)*/, angles[2] );
		AngleVectors( temp_ang, dir, NULL, NULL );

			// try to make the shot alternate between barrels
		offset = 0; //irandom(0, 1) * 2 + 1;

			// FIXME:  These offsets really don't work like they should 
		VectorMA( start, offset, right, temp_org );
		VectorMA( temp_org, offset, up, temp_org );
		FirePrifleBullet( ent, temp_org, dir ); //temp_org

		G_LogWeaponFire(ent->s.number, WP_6);
	}
	else
	{
		trace_t		tr;
		vec3_t		end;
		gentity_t	*traceEnt;
		//int			i = 0;
		int			damage = 0;

		VectorMA (muzzle, MAXRANGE_PHASER, forward, end);

		// Find out who we've hit
	//	gi.trace ( &tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT);
		
		trap_Trace (&tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT );
		
		if (tr.entityNum == (MAX_GENTITIES-1))
		{
			return;
		}

		traceEnt = &g_entities[ tr.entityNum ];

		if ( traceEnt->takedamage && rpg_phaserdmg.integer != 0 ) 
		{
//			damage = (float)PHASER_DAMAGE*DMG_VAR*s_quadFactor;		// No variance on phaser
			damage = (float)PHASER_DAMAGE*s_quadFactor;

			if (tr.fraction <= PHASER_POINT_BLANK_FRAC)
			{	// Point blank!  Do up to double damage.
				damage += damage * (1.0 - (tr.fraction/PHASER_POINT_BLANK_FRAC));
			}
			else
			{	// Normal range
				damage -= (int)(tr.fraction*5.0);
			}
			
			if (damage > 0)
			{
				G_Damage( traceEnt, ent, ent, forward, tr.endpos, damage, 
					DAMAGE_NO_KNOCKBACK | DAMAGE_ARMOR_PIERCING, MOD_CRIFLE_ALT ); //GSIO01: was MOD_PHASER
			}
		}
	}
}

/*
----------------------------------------------
	SCAVENGER
----------------------------------------------
*/
#define SCAV_SIZE			3
#define SCAV_ALT_SIZE		6

/*
----------------------------------------------
	STASIS
----------------------------------------------
*/

#define STASIS_VELOCITY		 2500 //2500 	//800	//650 //1100 //3000 // not used!
#define STASIS_VELOCITY2	1000	
// #define STASIS_SPREAD		5.0		//2.5	//1.8	// Keep the spread relatively small so that you can get multiple projectile impacts when a badie is close
#define STASIS_SPREAD		0.085f		// Roughly equivalent to sin(5 deg).

#define STASIS_MAIN_MISSILE_BIG		1 //4 //RPG-X | Marcin | 05/12/2008
#define STASIS_MAIN_MISSILE_SMALL	1 //2 //RPG-X | Marcin | 05/12/2008

#define STASIS_ALT_RIGHT_OFS	0.10
#define STASIS_ALT_UP_OFS		0.02
#define STASIS_ALT_MUZZLE_OFS	1

#define MAXRANGE_ALT_STASIS		4096

//---------------------------------------------------------
void FireDisruptorMissile( gentity_t *ent, vec3_t origin, vec3_t dir, int size )
//---------------------------------------------------------
{
	gentity_t	*bolt;
	int			boltsize;

	bolt = G_Spawn();
	bolt->classname = "disruptor_projectile";

	
	bolt->nextthink = level.time + 10000;
	bolt->think = G_FreeEntity;

	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_10;
	bolt->r.ownerNum = ent->s.number;
	bolt->parent = ent;
	if ( rpg_stasisdmg.integer != 0 )
	{
		bolt->damage = /*size */ STASIS_DAMAGE*DMG_VAR*s_quadFactor;
	}
	else
	{
		bolt->damage = 0;
	}
	bolt->splashDamage = 0;
	bolt->splashRadius = 0;
	bolt->methodOfDeath = MOD_STASIS_ALT; //GSIO01: was MOD_TETRION_ALT
	bolt->clipmask = MASK_SHOT;

	// Set the size of the missile up
	boltsize=3*size;
	VectorSet(bolt->r.maxs, boltsize>>1, boltsize, boltsize>>1);
	boltsize=-boltsize;
	VectorSet(bolt->r.mins, boltsize>>1, boltsize, boltsize>>1);

//	bolt->trigger_formation = qfalse;		// don't draw tail on first frame	

	// There are going to be a couple of different sized projectiles, so store 'em here
	bolt->count = size;
	// kef -- need to keep the size in something that'll reach the cgame side
	bolt->s.time2 = size;

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time;
	VectorCopy( origin, bolt->s.pos.trBase );
	SnapVector( bolt->s.pos.trBase );			// save net bandwidth
	
	VectorScale( dir, rpg_disruptorSpeed.integer + ( 50 * size ), bolt->s.pos.trDelta ); //RPG-X | Marcin | 05/12/2008
	
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
	VectorCopy (origin, bolt->r.currentOrigin);
	// Used by trails
	VectorCopy (origin, bolt->pos1 );
	VectorCopy (origin, bolt->pos2 );
	// kef -- need to keep the origin in something that'll reach the cgame side
	VectorCopy(origin, bolt->s.angles2);
	SnapVector( bolt->s.angles2 );			// save net bandwidth
}

//---------------------------------------------------------
void WP_FireDisruptorMain( gentity_t *ent )
//---------------------------------------------------------
{
	//vec3_t	dir;

	// Fire forward
	FireDisruptorMissile( ent, muzzle, forward, STASIS_MAIN_MISSILE_BIG );
	//FireStasisMissile( ent, muzzle, forward, STASIS_MAIN_MISSILE_BIG );

	// Fire slightly to the left
/*	VectorMA(forward, STASIS_SPREAD, right, dir);
	VectorNormalize(dir);
	FireStasisMissile( ent, muzzle, dir, STASIS_MAIN_MISSILE_SMALL );

	// Fire slightly to the right.
	VectorMA(forward, -STASIS_SPREAD, right, dir);
	VectorNormalize(dir);
	FireStasisMissile( ent, muzzle, dir, STASIS_MAIN_MISSILE_SMALL );*/
}


/*void DoSmallStasisBeam(gentity_t *ent, vec3_t start, vec3_t dir)
{
	vec3_t end;
	trace_t tr;
	gentity_t *traceEnt;

	VectorMA(start, MAXRANGE_ALT_STASIS, dir, end);
	trap_Trace(&tr, start, NULL, NULL, end, ent->s.number, MASK_SHOT);

	traceEnt = &g_entities[ tr.entityNum ];

	if ( traceEnt->takedamage && rpg_stasisdmg.integer != 0) 
	{
		//For knockback - send them up in air
		if ( dir[2] < 0.20f )
		{
			dir[2] = 0.20f;
		}

		VectorNormalize( dir );
	
		G_Damage(traceEnt, ent, ent, dir, tr.endpos, STASIS_ALT_DAMAGE2*DMG_VAR*s_quadFactor, DAMAGE_ARMOR_PIERCING, MOD_STASIS_ALT );
		// log hit
		if (ent->client)
		{
			ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
		}
	}
}*/


//---------------------------------------------------------
/*void WP_FireStasisAlt( gentity_t *ent )
//---------------------------------------------------------
{
	trace_t		tr;
	vec3_t		end, d_dir, d_right, d_up={0,0,1}, start;
	gentity_t	*tent;
	gentity_t	*traceEnt;

	// Find the main impact point
	VectorMA (muzzle, MAXRANGE_ALT_STASIS, forward, end);
	trap_Trace ( &tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT);
	
	traceEnt = &g_entities[ tr.entityNum ];

	// Why am I doing this when I've got a right and up already?  Well, because this is how it is calc'ed on the client side.
	CrossProduct(forward, d_up, d_right);
	VectorNormalize(d_right);				// "right" is scaled by the sin of the angle between fwd & up...  Ditch that.
	CrossProduct(d_right, forward, d_up);	// Change the "fake up" (0,0,1) to a "real up" (perpendicular to the forward vector).
	// VectorNormalize(d_up);				// If I cared about how the vertical variance looked when pointing up or down, I'd normalize this.

	// Fire a shot up and to the right.
	VectorMA(forward, STASIS_ALT_RIGHT_OFS, d_right, d_dir);
	VectorMA(d_dir, STASIS_ALT_UP_OFS, d_up, d_dir);
	VectorMA(muzzle, STASIS_ALT_MUZZLE_OFS, d_right, start);
	DoSmallStasisBeam(ent, start, d_dir);

	// Fire a shot up and to the left.
	VectorMA(forward, -STASIS_ALT_RIGHT_OFS, d_right, d_dir);
	VectorMA(d_dir, STASIS_ALT_UP_OFS, d_up, d_dir);
	VectorMA(muzzle, -STASIS_ALT_MUZZLE_OFS, d_right, start);
	DoSmallStasisBeam(ent, start, d_dir);

	// Fire a shot a bit down and to the right.
	VectorMA(forward, 2.0*STASIS_ALT_RIGHT_OFS, d_right, d_dir);
	VectorMA(d_dir, -0.5*STASIS_ALT_UP_OFS, d_up, d_dir);
	VectorMA(muzzle, 2.0*STASIS_ALT_MUZZLE_OFS, d_right, start);
	DoSmallStasisBeam(ent, start, d_dir);

	// Fire a shot up and to the left.
	VectorMA(forward, -2.0*STASIS_ALT_RIGHT_OFS, d_right, d_dir);
	VectorMA(d_dir, -0.5*STASIS_ALT_UP_OFS, d_up, d_dir);
	VectorMA(muzzle, -2.0*STASIS_ALT_MUZZLE_OFS, d_right, start);
	DoSmallStasisBeam(ent, start, d_dir);

	// Main beam
	tent = G_TempEntity( tr.endpos, EV_STASIS );
	tent->s.angles[YAW] = (int)(ent->client->ps.viewangles[YAW]);

	if ( traceEnt->takedamage && rpg_stasisdmg.integer != 0) 
	{
		//For knockback - send them up in air
		VectorCopy( forward, d_dir );
		if ( d_dir[2] < 0.30f )
		{
			d_dir[2] = 0.30f;
		}

		VectorNormalize( d_dir );
	
		G_Damage( traceEnt, ent, ent, d_dir, tr.endpos, STASIS_ALT_DAMAGE*DMG_VAR*s_quadFactor, 
					DAMAGE_ARMOR_PIERCING, MOD_STASIS_ALT );
		// log hit
		if (ent->client)
		{
			ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
		}
	}

	// Stash origins, etc. so that the effects can have access to them
	VectorCopy( muzzle, tent->s.origin2 );
	SnapVector(tent->s.origin2);
}*/

//TiM -void WP_FireStasis( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
void WP_FireDisruptor( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	// This was moved out of the FireWeapon switch statement below to keep things more consistent
	if ( !alt_fire )
	{
		trace_t		tr;
		vec3_t		end;
		gentity_t	*traceEnt;
		//int			i = 0;
		int			damage = 0;

		VectorMA (muzzle, MAXRANGE_PHASER, forward, end);

		// Find out who we've hit
	//	gi.trace ( &tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT);
		
		trap_Trace (&tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT );
		
		if (tr.entityNum == (MAX_GENTITIES-1))
		{
			return;
		}

		traceEnt = &g_entities[ tr.entityNum ];

		if ( traceEnt->takedamage && rpg_phaserdmg.integer != 0 ) 
		{
//			damage = (float)PHASER_DAMAGE*DMG_VAR*s_quadFactor;		// No variance on phaser
			damage = (float)PHASER_DAMAGE*s_quadFactor;

			if (tr.fraction <= PHASER_POINT_BLANK_FRAC)
			{	// Point blank!  Do up to double damage.
				damage += damage * (1.0 - (tr.fraction/PHASER_POINT_BLANK_FRAC));
			}
			else
			{	// Normal range
				damage -= (int)(tr.fraction*5.0);
			}
			
			if (damage > 0)
			{
				G_Damage( traceEnt, ent, ent, forward, tr.endpos, damage, 
					DAMAGE_NO_KNOCKBACK | DAMAGE_ARMOR_PIERCING, MOD_STASIS ); //GSIO01: was MOD_TETRION_ALT
			}
		}
	}
	else
	{
		//WP_FireStasisMain( ent );
		WP_FireDisruptorMain( ent );
	}

	G_LogWeaponFire(ent->s.number, WP_10);
}

/*
----------------------------------------------
	GRENADE LAUNCHER
----------------------------------------------
*/

#define GRENADE_VELOCITY		1000
#define GRENADE_TIME			2000
#define GRENADE_SIZE			4
#define GRENADE_ALT_VELOCITY	1200
#define GRENADE_ALT_TIME		2500

#define SHRAPNEL_DAMAGE			30
#define SHRAPNEL_DISTANCE		4096
#define SHRAPNEL_BITS			6
#define SHRAPNEL_RANDOM			3
#define SHRAPNEL_SPREAD			0.75

//---------------------------------------------------------
void grenadeExplode( gentity_t *ent )
//---------------------------------------------------------
{
	vec3_t		pos;
	gentity_t	*tent;

	VectorSet( pos, ent->r.currentOrigin[0], ent->r.currentOrigin[1], ent->r.currentOrigin[2] + 8 );

	tent = G_TempEntity( pos, EV_GRENADE_EXPLODE );

	// splash damage (doesn't apply to person directly hit)
	if ( ent->splashDamage ) {
		G_RadiusDamage( pos, ent->parent, ent->splashDamage, ent->splashRadius, 
			NULL, 0, ent->splashMethodOfDeath ); 
	}
	G_FreeEntity( ent );
}


//#include <windows.h>
//---------------------------------------------------------
void grenadeSpewShrapnel( gentity_t *ent )
//---------------------------------------------------------
{
	gentity_t	*tent = NULL;

	//MessageBox(NULL,"Debug10","",MB_OK);

	tent = G_TempEntity( ent->r.currentOrigin, EV_GRENADE_SHRAPNEL_EXPLODE );
	//MessageBox(NULL,"Debug11","",MB_OK);
	tent->s.eventParm = DirToByte(ent->pos1);
	//MessageBox(NULL,"Debug12","",MB_OK);

	// just do radius dmg for altfire
	if( G_RadiusDamage( ent->r.currentOrigin, ent->parent, ent->splashDamage, ent->splashRadius, 
		ent, 0, ent->splashMethodOfDeath ) )
	{
		//MessageBox(NULL,"Debug13","",MB_OK);
		// log hit
		/*if (ent->client)
		{
			//MessageBox(NULL,"Debug14","",MB_OK);
			ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
		}*/
	}

	//MessageBox(NULL,"Debug15","",MB_OK);

	G_FreeEntity(ent);
}


/*
===============
RPG-X Fire Effects gun
By: RedTechie
===============
*/
void WP_FxGun( gentity_t *ent, qboolean alt_fire ){
	gentity_t	*target;				//Target entity
	trace_t		trace;					//Used to trace target
	vec3_t		src, dest, vf;			//Used to find target
	
	if(!ent){
		return;
	}
	//if(ent->parent->client->sess.sessionClass == PC_ADMIN){
		if(alt_fire){
			return;
		}else{
			//////////////////////////////////////
			//All this code below finds the target entity

			VectorCopy( ent->r.currentOrigin, src );
			src[2] += ent->client->ps.viewheight;

			AngleVectors( ent->client->ps.viewangles, vf, NULL, NULL );

			//extend to find end of use trace
			VectorMA( src, -6, vf, src );//in case we're inside something?
			VectorMA( src, 1340, vf, dest );//128+6

			//Trace ahead to find a valid target
			trap_Trace( &trace, src, vec3_origin, vec3_origin, dest, ent->s.number, MASK_BRUSHES ); //RPG-X: RedTechie - Use to be MASK_ALL

			if ( trace.fraction == 1.0f || trace.entityNum < 0 )
			{
				trap_SendConsoleCommand( EXEC_APPEND, va("echo No target in range to kick.") );
				return;
			}

			target = &g_entities[trace.entityNum];

			////////////////////////////////
			
			//lets play the FX
			if(target){
				G_AddEvent( target, EV_FX_SPARK, 0 );
				//SP_fx_spark( target );
			}
		}
	//}
}


//---------------------------------------------------------
void WP_FireGrenade( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	gentity_t	*grenade;
	gentity_t	*tripwire = NULL;
	gentity_t	*tent = 0;
	vec3_t		dir, start;
	int			tripcount = 0;
	int			foundTripWires[MAX_GENTITIES] = {ENTITYNUM_NONE};
	int			tripcount_org;
	int			lowestTimeStamp;
	int			removeMe;
	int			i;
	trace_t		tr;
	vec3_t		end;

	VectorCopy( forward, dir );
	VectorCopy( muzzle, start );

	if(RPGEntityCount != ENTITYNUM_MAX_NORMAL-20){
		if ( alt_fire )
		{
			//RPG-X: RedTechie - Moved here to stop entities from being sucked up
			grenade = G_Spawn();
			
			// kef -- make sure count is 0 so it won't get its bounciness removed like the tetrion projectile
			grenade->count = 0;

			//RPG-X: RedTechie - Forced Tripwires
			if ( rpg_invisibletripmines.integer == 1 )
			{
				//limit to 10 placed at any one time
				//see how many there are now
				while ( (tripwire = G_Find( tripwire, FOFS(classname), "tripwire" )) != NULL )
				{
					if ( tripwire->parent != ent )
					{
						continue;
					}
					foundTripWires[tripcount++] = tripwire->s.number;
				}
				//now remove first ones we find until there are only 9 left
				tripwire = NULL;
				tripcount_org = tripcount;
				lowestTimeStamp = level.time;
				//RPG-X: RedTechie - Added 51 tripwires for each person
				while ( tripcount > 50 ) //9
				{
					removeMe = -1;
					for ( i = 0; i < tripcount_org; i++ )
					{
						if ( foundTripWires[i] == ENTITYNUM_NONE )
						{
							continue;
						}
						tripwire = &g_entities[foundTripWires[i]];
						if ( tripwire && tripwire->timestamp < lowestTimeStamp )
						{
							removeMe = i;
							lowestTimeStamp = tripwire->timestamp;
						}
					}
					if ( removeMe != -1 )
					{
						//remove it... or blow it?
						if ( &g_entities[foundTripWires[removeMe]] == NULL )
						{
							break;
						}
						else
						{
							G_FreeEntity( &g_entities[foundTripWires[removeMe]] );
						}
						foundTripWires[removeMe] = ENTITYNUM_NONE;
						tripcount--;
					}
					else
					{
						break;
					}
				}
				//now make the new one
				grenade->classname = "tripwire";
				grenade->splashDamage = GRENADE_SPLASH_DAM*2*s_quadFactor;
				grenade->splashRadius = GRENADE_SPLASH_RAD*2;
				grenade->s.pos.trType = TR_LINEAR;
				grenade->nextthink = level.time + 1000; // How long 'til she blows
				grenade->count = 1;//tell it it's a tripwire for when it sticks
				grenade->timestamp = level.time;//remember when we placed it
				grenade->s.otherEntityNum2 = ent->client->sess.sessionTeam;
			}
			else
			{
				grenade->classname = "grenade_alt_projectile";
				grenade->splashDamage = GRENADE_SPLASH_DAM*s_quadFactor;
				grenade->splashRadius = GRENADE_SPLASH_RAD;//*s_quadFactor;
				grenade->s.pos.trType = TR_GRAVITY;
				grenade->nextthink = level.time + GRENADE_ALT_TIME; // How long 'til she blows
			}
			grenade->think = grenadeSpewShrapnel;
			grenade->s.eFlags |= EF_MISSILE_STICK;
			VectorScale( dir, 1000/*GRENADE_ALT_VELOCITY*/, grenade->s.pos.trDelta );

			grenade->damage = GRENADE_ALT_DAMAGE*DMG_VAR*s_quadFactor;
			grenade->methodOfDeath = MOD_GRENADE_ALT;
			grenade->splashMethodOfDeath = MOD_GRENADE_ALT_SPLASH;
			grenade->s.eType = ET_ALT_MISSILE;



			//RPG-X: RedTechie - Moved here to stop entities from being sucked up
			grenade->r.svFlags = SVF_USE_CURRENT_ORIGIN;
			grenade->s.weapon = WP_8;
			grenade->r.ownerNum = ent->s.number;
			grenade->parent = ent;

			VectorSet(grenade->r.mins, -GRENADE_SIZE, -GRENADE_SIZE, -GRENADE_SIZE);
			VectorSet(grenade->r.maxs, GRENADE_SIZE, GRENADE_SIZE, GRENADE_SIZE);

			grenade->clipmask = MASK_SHOT;

			grenade->s.pos.trTime = level.time;		// move a bit on the very first frame
			VectorCopy( start, grenade->s.pos.trBase );
			SnapVector( grenade->s.pos.trBase );			// save net bandwidth
			
			SnapVector( grenade->s.pos.trDelta );			// save net bandwidth
			VectorCopy (start, grenade->r.currentOrigin);

			VectorCopy( start, grenade->pos2 );
		}
		else
		{
			//RPG-X: RedTechie - Check to see if there admin if so grant them effects gun
			if( IsAdmin(ent) && (rpg_effectsgun.integer == 1))
			{
				VectorMA (muzzle, MAXRANGE_CRIFLE, forward, end);
				trap_Trace (&tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT );

				//TiM : FX Gun additional effects.
				//Okay... screw the generic args. it's giving me a headache
				//Case in this case... harhar is teh solution
				if ( ent->client->fxGunData.eventNum > 0 ) 
				{
					fxGunData_t *fxGunData = &ent->client->fxGunData;

					//set the entity event
					tent = G_TempEntity( tr.endpos, fxGunData->eventNum );
					
					//based on the event, add additional args
					switch ( fxGunData->eventNum ) {
						//sparks
						case EV_FX_SPARK:
							//Direction vector based off of trace normal
							VectorCopy( tr.plane.normal, tent->s.angles2 );
							VectorShort( tent->s.angles2 );

							//spark interval
							tent->s.time2 = fxGunData->arg_float1;
							//spark time length
							tent->s.time = fxGunData->arg_int2;
							break;
						case EV_FX_STEAM:
							//Direction vector based off of trace normal
							VectorCopy( tr.plane.normal, tent->s.angles2 );
							VectorShort( tent->s.angles2 );
							//time length
							tent->s.time = fxGunData->arg_int2;
							break;
						case EV_FX_FIRE:
							VectorCopy( tr.plane.normal, tent->s.angles2 );
							VectorShort( tent->s.angles2 );
							tent->s.time = fxGunData->arg_int1;
							tent->s.time2 = fxGunData->arg_int2;
							break;
						case EV_FX_SHAKE:
							VectorCopy( tr.plane.normal, tent->s.angles2 );
							VectorShort( tent->s.angles2 );
							tent->s.time = fxGunData->arg_int1;
							tent->s.time2 = fxGunData->arg_int2;
							break;
						case EV_FX_CHUNKS:
							//normal direction
							VectorCopy( tr.plane.normal, tent->s.angles2 );
							VectorShort( tent->s.angles2 );	

							//scale/radius
							tent->s.time2 = fxGunData->arg_int1;
							//material type
							tent->s.powerups = fxGunData->arg_int2;
							break;
						case EV_FX_DRIP:
							//type of drip
							tent->s.time2 = fxGunData->arg_int1;
							//degree of drippiness
							tent->s.angles2[0] = fxGunData->arg_float1;
							//length of effect
							tent->s.powerups = fxGunData->arg_int2;
							break;
						case EV_FX_SMOKE:
							//Direction vector based off of trace normal
							VectorCopy( tr.plane.normal, tent->s.angles2 );
							VectorShort( tent->s.angles2 );
							//smoke radius
							tent->s.time = fxGunData->arg_int1;
							//killtime 
							tent->s.time2 = fxGunData->arg_int2;

							//set ent origin for dir calcs
							VectorCopy( tent->s.origin, tent->s.origin2 );
							//VectorMA( tent->s.origin2, 6, tr.plane.normal, tent->s.origin2 );
							tent->s.origin2[2] += 6;
							break;
						case EV_FX_SURFACE_EXPLOSION:
							//radius
							tent->s.angles2[0] = fxGunData->arg_float1;
							//camera shake
							tent->s.angles2[1] = fxGunData->arg_float2;
							//orient the dir to the plane we shot at
							VectorCopy( tr.plane.normal, tent->s.origin2 );
							//Meh... generic hardcoded data for the rest lol
							tent->s.time2 = 0;
							break;
						case EV_FX_ELECTRICAL_EXPLOSION:
							//Set direction
							VectorCopy( tr.plane.normal, tent->s.origin2 );
							//Set Radius
							tent->s.angles2[0] = fxGunData->arg_float1;
							break;
					}

					//Little hack to make the Detpack sound global
					if ( fxGunData->eventNum == EV_DETPACK ) {
						gentity_t	*te;
						te = G_TempEntity( tr.endpos, EV_GLOBAL_SOUND );
						te->s.eventParm = G_SoundIndex( "sound/weapons/explosions/detpakexplode.wav" );//cgs.media.detpackExplodeSound
						te->r.svFlags |= SVF_BROADCAST;
					}
				}
				else {
					tent = G_TempEntity( tr.endpos, EV_EFFECTGUN_SHOOT );//EV_EFFECTGUN_SHOOT muzzle //
				}
				
				tent->s.eFlags |= EF_FIRING;
				
				
				//G_FreeEntity(tent);
				//G_AddEvent( target, EV_FX_SPARK, 0 );
			}else{
				//RPG-X: RedTechie - Moved here to stop entities from being sucked up
				grenade = G_Spawn();
			
				// kef -- make sure count is 0 so it won't get its bounciness removed like the tetrion projectile
				grenade->count = 0;


				grenade->classname = "grenade_projectile";
				grenade->nextthink = level.time + GRENADE_TIME; // How long 'til she blows
				grenade->think = grenadeExplode;
				grenade->s.eFlags |= EF_BOUNCE_HALF;
				VectorScale( dir, GRENADE_VELOCITY, grenade->s.pos.trDelta );
				grenade->s.pos.trType = TR_GRAVITY;

				grenade->damage = GRENADE_DAMAGE*DMG_VAR*s_quadFactor;
				grenade->splashDamage = GRENADE_SPLASH_DAM*s_quadFactor;
				grenade->splashRadius = GRENADE_SPLASH_RAD;//*s_quadFactor;
				grenade->methodOfDeath = MOD_GRENADE;
				grenade->splashMethodOfDeath = MOD_GRENADE_SPLASH;
				grenade->s.eType = ET_MISSILE;


				//RPG-X: RedTechie - Moved here to stop entities from being sucked up
				grenade->r.svFlags = SVF_USE_CURRENT_ORIGIN;
				grenade->s.weapon = WP_8;
				grenade->r.ownerNum = ent->s.number;
				grenade->parent = ent;

				VectorSet(grenade->r.mins, -GRENADE_SIZE, -GRENADE_SIZE, -GRENADE_SIZE);
				VectorSet(grenade->r.maxs, GRENADE_SIZE, GRENADE_SIZE, GRENADE_SIZE);

				grenade->clipmask = MASK_SHOT;

				grenade->s.pos.trTime = level.time;		// move a bit on the very first frame
				VectorCopy( start, grenade->s.pos.trBase );
				SnapVector( grenade->s.pos.trBase );			// save net bandwidth
				
				SnapVector( grenade->s.pos.trDelta );			// save net bandwidth
				VectorCopy (start, grenade->r.currentOrigin);

				VectorCopy( start, grenade->pos2 );
			}
		}

		G_LogWeaponFire(ent->s.number, WP_8);
	}else{
		G_LogPrintf("RPG-X WARNING: Max entities about to be hit! Restart the server ASAP or suffer a server crash!\n");
		trap_SendServerCommand( -1, va("print \"^1RPG-X WARNING: Max entities about to be hit! Restart the server ASAP or suffer a server crash!\n\""));
	}
}

/*
----------------------------------------------
	TETRION
----------------------------------------------
*/

#define TETRION_ALT_SIZE		6

#define MAX_TR_116_DIST	8192
#define MAX_TRACES	24 //Number of traces thru walls we'll do before we give up lol

void WP_FireTR116Bullet( gentity_t *ent, vec3_t start, vec3_t dir ) {
	gentity_t	*traceEnt;
	vec3_t end; //end-point in trace
	vec3_t traceFrom;
	trace_t	tr;
	//int	i;

	VectorCopy( start, traceFrom );
	VectorMA( traceFrom, MAX_TR_116_DIST, dir, end ); //set trace end point

	trap_Trace( &tr, traceFrom, NULL, NULL, end, ent->s.number, CONTENTS_BODY ); //MASK_SHOT - TiM - Goes thru everything but players
	//ent->client->ps.clientNum
	/*for ( i = 0; i < MAX_TRACES; i++ ) {
		trap_Trace( &tr, traceFrom, NULL, NULL, end, ent->s.number, CONTENTS_BODY ); //MASK_SHOT - TiM - Goes thru everything but players
		
		//Com_Printf( "%i\n", tr.entityNum );

		if ( tr.entityNum >= ENTITYNUM_MAX_NORMAL ) {
			//cancel for skybox
			//if (tr.surfaceFlags & SURF_SKY)
			//	break;

			//Hit the end
			//if (tr.fraction == 1.0) {
			//	Com_Printf( "End reached\n");			
			//	break;
			//}

			// otherwise continue tracing thru walls
			VectorMA (tr.endpos,16,dir,traceFrom);
			VectorMA( traceFrom, MAX_TR_116_DIST, dir, end ); //set trace end point
			continue;
		}
		else {
			break;
		}
	}*/

	//Com_Printf( "%i\n", tr.entityNum );

	if ( tr.entityNum < ENTITYNUM_MAX_NORMAL ) {

		traceEnt = &g_entities[ tr.entityNum ];

		if ( traceEnt->takedamage ) {
			G_Damage( traceEnt, ent, ent, dir, tr.endpos, TETRION_DAMAGE * s_quadFactor, 0, MOD_TETRION_ALT );
		}
	}

}

//---------------------------------------------------------
void WP_FireTetrionDisruptor( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
//(RPG-X: J2J MOdified to make it look and feel like tr116
//RPG-X: TiM - Modified even furthur
{
	vec3_t	dir;
	vec3_t	start;

	VectorCopy( forward, dir );
	VectorCopy( muzzle, start );
	//VectorNormalize (dir);

	WP_FireTR116Bullet( ent, start, dir );

	G_LogWeaponFire(ent->s.number, WP_7);
	//ent->last_tr116_fire = level.time;								//RPG-X: J2J update last fire time
}


/*
----------------------------------------------
	QUANTUM BURST
----------------------------------------------
*/

#define QUANTUM_VELOCITY	1300 // not used
#define QUANTUM_SIZE		1 //3 //RPG-X | Marcin | 05/12/2008

#define QUANTUM_ALT_VELOCITY	650 // not used
#define QUANTUM_ALT_THINK_TIME	300
#define QUANTUM_ALT_SEARCH_TIME	100
#define QUANTUM_ALT_SEARCH_DIST	4096

//---------------------------------------------------------
void FireQuantumBurst( gentity_t *ent, vec3_t start, vec3_t dir )
//---------------------------------------------------------
{
	gentity_t	*bolt;

	bolt = G_Spawn();
	bolt->classname = "quantum_projectile";
	
	bolt->nextthink = level.time + 6000;
	bolt->think = G_FreeEntity;

	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_9;
	bolt->r.ownerNum = ent->s.number;
	bolt->parent = ent;

	bolt->damage = QUANTUM_DAMAGE*DMG_VAR*s_quadFactor;
	bolt->splashDamage = QUANTUM_SPLASH_DAM*s_quadFactor;
	bolt->splashRadius = QUANTUM_SPLASH_RAD;//*s_quadFactor;

	bolt->methodOfDeath = MOD_QUANTUM;
	bolt->splashMethodOfDeath = MOD_QUANTUM_SPLASH;
	bolt->clipmask = MASK_SHOT;

	VectorSet(bolt->r.mins, -QUANTUM_SIZE, -QUANTUM_SIZE, -QUANTUM_SIZE);
	VectorSet(bolt->r.maxs, QUANTUM_SIZE, QUANTUM_SIZE, QUANTUM_SIZE);

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	SnapVector( bolt->s.pos.trBase );			// save net bandwidth
	
	VectorScale( dir, rpg_photonSpeed.integer, bolt->s.pos.trDelta );
	
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
	VectorCopy (start, bolt->r.currentOrigin);
	VectorCopy (start, bolt->pos1);
}


qboolean SearchTarget(gentity_t *ent, vec3_t start, vec3_t end)
{
	trace_t tr;
	gentity_t *traceEnt;
	vec3_t fwd;

	trap_Trace (&tr, start, NULL, NULL, end, ent->s.number, MASK_SHOT );
	traceEnt = &g_entities[ tr.entityNum ];

	// Don't find teleporting borg in Assimilation mode
	/*if ( g_pModAssimilation.integer != 0 && traceEnt->client 
				&& traceEnt->client->sess.sessionClass == PC_BORG 
				&& traceEnt->s.eFlags == EF_NODRAW )
	{
		return qfalse;
	}*/

	if (traceEnt->takedamage && traceEnt->client && !OnSameTeam(traceEnt, &g_entities[ent->r.ownerNum])) 
	{
		ent->target_ent = traceEnt;
		VectorSubtract(ent->target_ent->r.currentOrigin, ent->r.currentOrigin, fwd);
		VectorNormalize(fwd);
		VectorScale(fwd, rpg_altPhotonSpeed.integer, ent->s.pos.trDelta);
		VectorCopy(fwd, ent->movedir);
		SnapVector(ent->s.pos.trDelta);			// save net bandwidth
		VectorCopy(ent->r.currentOrigin, ent->s.pos.trBase);
		ent->s.pos.trTime = level.time;
		ent->nextthink = level.time + QUANTUM_ALT_THINK_TIME;
		return qtrue;
	}
	return qfalse; 
}


void WP_QuantumAltThink(gentity_t *ent)
{
	vec3_t start, newdir, targetdir, lup={0,0,1}, lright, search; 
	float dot, dot2;

	ent->health--;
	if (ent->health<=0)
	{
		G_FreeEntity(ent);
		return;
	}

	if (ent->target_ent)
	{	// Already have a target, start homing.
		if (ent->health <= 0 || !ent->inuse /*|| 
			  (	g_pModAssimilation.integer != 0 && ent->target_ent->client 
				&& ent->target_ent->client->sess.sessionClass == PC_BORG 
				&& ent->target_ent->s.eFlags == EF_NODRAW )*/)
		{	// No longer target this
			ent->target_ent = NULL;
			ent->nextthink = level.time + 1000;
			ent->health -= 5;
			return;
		}
		VectorSubtract(ent->target_ent->r.currentOrigin, ent->r.currentOrigin, targetdir);
		VectorNormalize(targetdir);

		// Now the rocket can't do a 180 in space, so we'll limit the turn to about 45 degrees.
		dot = DotProduct(targetdir, ent->movedir);
		// a dot of 1.0 means right-on-target.
		if (dot < 0.0)
		{	// Go in the direction opposite, start a 180.
			CrossProduct(ent->movedir, lup, lright);
			dot2 = DotProduct(targetdir, lright);
			if (dot2 > 0)
			{	// Turn 45 degrees right.
				VectorAdd(ent->movedir, lright, newdir);
			}
			else
			{	// Turn 45 degrees left.
				VectorSubtract(ent->movedir, lright, newdir);
			}
			// Yeah we've adjusted horizontally, but let's split the difference vertically, so we kinda try to move towards it.
			newdir[2] = (targetdir[2] + ent->movedir[2]) * 0.5;
			VectorNormalize(newdir);
		}
		else if (dot < 0.7)
		{	// Need about one correcting turn.  Generate by meeting the target direction "halfway".
			// Note, this is less than a 45 degree turn, but it is sufficient.  We do this because the rocket may have to go UP.
			VectorAdd(ent->movedir, targetdir, newdir);
			VectorNormalize(newdir);
		}
		else
		{	// else adjust to right on target.
			VectorCopy(targetdir, newdir);
		}

		VectorScale(newdir, rpg_altPhotonSpeed.integer, ent->s.pos.trDelta);
		VectorCopy(newdir, ent->movedir);
		SnapVector(ent->s.pos.trDelta);			// save net bandwidth
		VectorCopy(ent->r.currentOrigin, ent->s.pos.trBase);
		SnapVector(ent->s.pos.trBase);
		ent->s.pos.trTime = level.time;

		// Home at a reduced frequency.
		ent->nextthink = level.time + QUANTUM_ALT_THINK_TIME;	// Nothing at all spectacular happened, continue.
	}
	else
	{	// Search in front of the missile for targets.
		VectorCopy(ent->r.currentOrigin, start);
		CrossProduct(ent->movedir, lup, lright);

		// Search straight ahead.
		VectorMA(start, QUANTUM_ALT_SEARCH_DIST, ent->movedir, search);

		// Add some small randomness to the search Z height, to give a bit of variation to where we are searching.
		search[2] += flrandom(-QUANTUM_ALT_SEARCH_DIST*0.075, QUANTUM_ALT_SEARCH_DIST*0.075);

		if (SearchTarget(ent, start, search))
			return;

		// Search to the right.
		VectorMA(search, QUANTUM_ALT_SEARCH_DIST*0.1, lright, search);
		if (SearchTarget(ent, start, search))
			return;
		
		// Search to the left.
		VectorMA(search, -QUANTUM_ALT_SEARCH_DIST*0.2, lright, search);
		if (SearchTarget(ent, start, search))
			return;

		// Search at a higher rate than correction.
		ent->nextthink = level.time + QUANTUM_ALT_SEARCH_TIME;	// Nothing at all spectacular happened, continue.

	}
	return;
}

//---------------------------------------------------------
void FireQuantumBurstAlt( gentity_t *ent, vec3_t start, vec3_t dir )
//---------------------------------------------------------
{
	gentity_t	*bolt;

	bolt = G_Spawn();
	bolt->classname = "quantum_alt_projectile";
	
	bolt->nextthink = level.time + 100;
	bolt->think = WP_QuantumAltThink;
	bolt->health = 25;		// 10 seconds.

	bolt->s.eType = ET_ALT_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_9;
	bolt->r.ownerNum = ent->s.number;
	bolt->parent = ent;
	bolt->s.eFlags |= EF_ALT_FIRING;

	bolt->damage = QUANTUM_ALT_DAMAGE*DMG_VAR*s_quadFactor;
	bolt->splashDamage = QUANTUM_ALT_SPLASH_DAM*s_quadFactor;
	bolt->splashRadius = QUANTUM_ALT_SPLASH_RAD;//*s_quadFactor;

	bolt->methodOfDeath = MOD_QUANTUM_ALT;
	bolt->splashMethodOfDeath = MOD_QUANTUM_ALT_SPLASH;
	bolt->clipmask = MASK_SHOT;

	VectorSet(bolt->r.mins, -QUANTUM_SIZE, -QUANTUM_SIZE, -QUANTUM_SIZE);
	VectorSet(bolt->r.maxs, QUANTUM_SIZE, QUANTUM_SIZE, QUANTUM_SIZE);

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	SnapVector(bolt->s.pos.trBase);
	
	VectorScale( dir, rpg_altPhotonSpeed.integer, bolt->s.pos.trDelta );
	VectorCopy(dir, bolt->movedir);
	
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
	VectorCopy (start, bolt->r.currentOrigin);
}

//---------------------------------------------------------
void WP_FireQuantumBurst( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	vec3_t	dir, start;

	VectorCopy( forward, dir );
	VectorCopy( muzzle, start );

	if ( alt_fire )
	{
		FireQuantumBurstAlt( ent, start, dir );
	}
	else
	{
		FireQuantumBurst( ent, start, dir );
	}

	G_LogWeaponFire(ent->s.number, WP_9);
}


/*
===============
LogAccuracyHit
===============
*/
qboolean LogAccuracyHit( gentity_t *target, gentity_t *attacker ) {
	if( !target->takedamage ) {
		return qfalse;
	}

	if ( target == attacker ) {
		return qfalse;
	}

	if( !target->client ) {
		return qfalse;
	}

	if( !attacker->client ) {
		return qfalse;
	}

	if( target->client->ps.stats[STAT_HEALTH] <= 0 ) {
		return qfalse;
	}

	if ( OnSameTeam( target, attacker ) ) {
		return qfalse;
	}

	return qtrue;
}

#define MAX_FORWARD_TRACE	8192

void CorrectForwardVector(gentity_t *ent, vec3_t fwd, vec3_t muzzlePoint, float projsize)
{
	trace_t		tr;
	vec3_t		end;
	vec3_t		eyepoint;
	vec3_t		mins, maxs;

	// Find the eyepoint.
	VectorCopy(ent->client->ps.origin, eyepoint);
	eyepoint[2] += ent->client->ps.viewheight;

	// First we must trace from the eyepoint to the muzzle point, to make sure that we have a legal muzzle point.
	if (projsize>0)
	{
		VectorSet(mins, -projsize, -projsize, -projsize);
		VectorSet(maxs, projsize, projsize, projsize);
		trap_Trace(&tr, eyepoint, mins, maxs, muzzlePoint, ent->s.number, MASK_SHOT);
	}
	else
	{
		trap_Trace(&tr, eyepoint, NULL, NULL, muzzlePoint, ent->s.number, MASK_SHOT);
	}

	if (tr.fraction < 1.0)
	{	// We hit something here...  Stomp the muzzlePoint back to the eye...
		VectorCopy(eyepoint, muzzlePoint);
		// Keep the forward vector where it is, 'cause straight forward from the eyeball is right where we want to be.
	}
	else
	{
		// figure out what our crosshairs are on...
		VectorMA(eyepoint, MAX_FORWARD_TRACE, forward, end);
		trap_Trace (&tr, eyepoint, NULL, NULL, end, ent->s.number, MASK_SHOT );

		// ...and have our new forward vector point at it
		VectorSubtract(tr.endpos, muzzlePoint, fwd);
		VectorNormalize(fwd);
	}
}

/*
===============
CalcMuzzlePoint

set muzzle location relative to pivoting eye
===============
*/

// Muzzle point table...
vec3_t WP_MuzzlePoint[WP_NUM_WEAPONS] = 
{//	Fwd,	right,	up.
	{0,		0,		0	},	// WP_0,
	{29,	2,		-4	},	// WP_5,			
	{25,	7,		-10	},	// WP_6,
	{25,	4,		-5	},	// WP_1,				
	{10,	14,		-8	},	// WP_4,	
	{25,	5,		-8	},	// WP_10,			
	{25,	5,		-10	},	// WP_8,	
	{0,		0,		0	},	// WP_7, //{22,	4.5,	-8	}, //TiM : Visual FX aren't necessary now, so just screw it
	{5,		6,		-6	},	// WP_9,	
	{29,	2,		-4	},	// WP_13,		
	{29,	2,		-4	},	// WP_12,	
	{29,	2,		-4	},	// WP_14
	{27,	8,		-10	},	// WP_11
	{29,	2,		-4	},	// WP_2,	
	{29,	2,		-4	},	// WP_3,	
	{29,	2,		-4	},	// WP_NEUTRINO_PROBE,
//	{25,	7,		-10	},	// WP_7
};


float WP_ShotSize[WP_NUM_WEAPONS] = 
{
	0,							// WP_0,
	0,							// WP_5,			
	0,							// WP_6,
	0,							// WP_1,				
	SCAV_SIZE,					// WP_4,	
	STASIS_MAIN_MISSILE_BIG*3,	// WP_10,			
	GRENADE_SIZE,				// WP_8,	
	6,							// WP_7,
	QUANTUM_SIZE,				// WP_9,	
	0,							// WP_13,		
	0,							// WP_12,
	0,							// WP_14
	0,		// WP_11
	0,							// WP_2,
	0,							// WP_3,
	0,							// WP_NEUTRINO_PROBE,
//	0,							// WP_7
};

float WP_ShotAltSize[WP_NUM_WEAPONS] = 
{
	0,							// WP_0,
	PHASER_ALT_RADIUS,			// WP_5,			
	0,							// WP_6,
	0,							// WP_1,				
	SCAV_ALT_SIZE,				// WP_4,	
	STASIS_MAIN_MISSILE_BIG*3,	// WP_10,			
	GRENADE_SIZE,				// WP_8,	
	TETRION_ALT_SIZE,			// WP_7,
	QUANTUM_SIZE,				// WP_9,	
	0,				// WP_13,		
	0,							// WP_12,		
	0,							// WP_14
	0,							// WP_11
	0,							// WP_2
	0,							// WP_3,
	0,							// WP_NEUTRINO_PROBE,
//	0,							// WP_7
};



//---------------------------------------------------------
void CalcMuzzlePoint ( gentity_t *ent, vec3_t fwd, vec3_t rt, vec3_t vup, vec3_t muzzlePoint, float projsize) 
//---------------------------------------------------------
{
	int weapontype;

	weapontype = ent->s.weapon;
	VectorCopy( ent->s.pos.trBase, muzzlePoint );

#if 1
	if (weapontype > WP_0 && weapontype < WP_NUM_WEAPONS)
	{	// Use the table to generate the muzzlepoint;
		{	// Crouching.  Use the add-to-Z method to adjust vertically.
			VectorMA(muzzlePoint, WP_MuzzlePoint[weapontype][0], fwd, muzzlePoint);
			VectorMA(muzzlePoint, WP_MuzzlePoint[weapontype][1], rt, muzzlePoint);
			if ( ent->client->ps.eFlags & EF_FULL_ROTATE && Q_fabs( ent->client->ps.viewangles[PITCH] > 89.0f ) ) {
				muzzlePoint[2] -= 20 + WP_MuzzlePoint[weapontype][2];
			}
			else
				muzzlePoint[2] += ent->client->ps.viewheight + WP_MuzzlePoint[weapontype][2];
			// VectorMA(muzzlePoint, ent->client->ps.viewheight + WP_MuzzlePoint[weapontype][2], vup, muzzlePoint);
		}
	}
#else	// Test code
	muzzlePoint[2] += ent->client->ps.viewheight;//By eyes
	muzzlePoint[2] += g_debugUp.value;
	VectorMA( muzzlePoint, g_debugForward.value, fwd, muzzlePoint);
	VectorMA( muzzlePoint, g_debugRight.value, rt, muzzlePoint);
#endif

	CorrectForwardVector(ent, fwd, muzzlePoint, projsize);
	SnapVector( muzzlePoint );
}


//---------------------------------------------------------

RPGX_SiteTOSiteData TransDat[MAX_CLIENTS];

//---------------------------------------------------------
void WP_TricorderScan (gentity_t *ent, qboolean alt_fire)
//---------------------------------------------------------
{
	gentity_t	*tr_ent;
	trace_t		tr;
	vec3_t		mins, maxs, end;
	int clientNum = ent->client->ps.clientNum;

	if ( rpg_rangetricorder.integer < 32 )
	{
		return;
	}

	// Fix - Changed || to && in the below if statement!
	if ( /*!g_classData[ent->client->sess.sessionClass].isMarine ent->client->sess.sessionClass != PC_ALPHAOMEGA22 &&*/ (IsAdmin( ent ) == qfalse))//ent->client->sess.sessionClass != PC_ADMIN )
	{
		return;
	}

	VectorMA( muzzle, rpg_rangetricorder.integer, forward, end );

	VectorSet( maxs, 6, 6, 6 );
	VectorScale( maxs, -1, mins );

	//TiM: I don't think performing a volume trace here is really needed.  It is after all based
	//on the player's current view.
	//TiM: No, I was wrong! They're better coz it means errant n00bs or bots can't dodge them as easily!
	trap_Trace ( &tr, muzzle, mins, maxs, end, ent->s.number, MASK_SHOT );
	//trap_Trace ( &tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT );
	tr_ent = &g_entities[tr.entityNum];

	//BOOKMARK J2J
	if ( alt_fire )
	{
		//RPG-X: J2J - New Transporter Tricorder Code (custom spawn points)
		//if( TransDat[clientNum].Used == qfalse )
		if ( VectorCompare( vec3_origin, TransDat[clientNum].storedCoord[TPT_TRICORDER].origin ) &&
			VectorCompare( vec3_origin, TransDat[clientNum].storedCoord[TPT_TRICORDER].angles ) )
		{
			//VectorCopy(ent->client->ps.origin, TransDat[clientNum].TransCoord);
			//VectorCopy(ent->client->ps.viewangles, TransDat[clientNum].TransCoordRot);
			VectorCopy( ent->client->ps.origin, TransDat[clientNum].storedCoord[TPT_TRICORDER].origin );
			VectorCopy( ent->client->ps.viewangles, TransDat[clientNum].storedCoord[TPT_TRICORDER].angles );
			//TransDat[clientNum].Used = qtrue;
		}

		if ( tr_ent && tr_ent->client && tr_ent->health > 0 )
		{
			//gentity_t	*tent;
			//TiM: If we're already in a transport sequence, don't try another one.
			//For starters, this screws up the visual FX, and secondly, I'm betting
			//if u actually tried this, you'd atomically disperse the transportee in a very painful way O_o
			if ( TransDat[tr_ent->client->ps.clientNum].beamTime > level.time /*&& level.time < tr_ent->client->ps.powerups[PW_QUAD]*/ ) {
				trap_SendServerCommand( ent-g_entities, va("chat \"Unable to comply. Subject is already within a transport cycle.\"", Q_COLOR_ESCAPE));
				return;
			}

			trap_SendServerCommand( ent-g_entities, va("chat \"Energizing.\"", Q_COLOR_ESCAPE));

			G_InitTransport( tr_ent->client->ps.clientNum, TransDat[clientNum].storedCoord[TPT_TRICORDER].origin,
							TransDat[clientNum].storedCoord[TPT_TRICORDER].angles );	
			//	TP_BORG,
			//  TP_NUM_TP
			//TeleportPlayer( tr_ent, TransDat[clientNum].TransCoord, TransDat[clientNum].TransCoordRot, TP_TRI_TP );
			//set beam time
			//TransDat[tr_ent->client->ps.clientNum].beamTime = level.time + 8000;

			//Add client-side visual FX
			//Make it last 8 seconds, so it won't end prematurely
			//tr_ent->client->ps.powerups[PW_BEAM_OUT] = level.time + 8000;
			//FIXME: Might need to add client side beam event here too, along with the powerup...

			//tent = G_TempEntity( tr_ent->client->ps.origin, EV_PLAYER_TRANSPORT_OUT );
			//tent->s.clientNum = tr_ent->client->ps.clientNum;

			//TransDat[tr_ent->client->ps.clientNum].beamer = clientNum;
			//VectorCopy( TransDat[clientNum].storedCoord[TPT_TRICORDER].origin, 
						//TransDat[tr_ent->client->ps.clientNum].currentCoord.origin );
			//VectorCopy( TransDat[clientNum].storedCoord[TPT_TRICORDER].angles,
						//TransDat[tr_ent->client->ps.clientNum].currentCoord.angles );
			return;
		}
		//If they clicked within 5 seconds ago
		if((level.time - TransDat[clientNum].LastClick) <= 5000)
		{
			VectorCopy( ent->client->ps.origin, TransDat[clientNum].storedCoord[TPT_TRICORDER].origin );
			VectorCopy( ent->client->ps.viewangles, TransDat[clientNum].storedCoord[TPT_TRICORDER].angles );

			//VectorCopy(ent->client->ps.origin, TransDat[clientNum].TransCoord);
			//VectorCopy(ent->client->ps.viewangles, TransDat[clientNum].TransCoordRot);
			TransDat[clientNum].LastClick = level.time-5000;
			trap_SendServerCommand( ent-g_entities, va("chat \"Location Confirmed.\"", Q_COLOR_ESCAPE));
			//trap_SendConsoleCommand( EXEC_APPEND, va("echo Location Confirmed.") );
		}
		else
		{
			trap_SendServerCommand( ent-g_entities, va("chat \"Click again to confirm Transporter Location.\"", Q_COLOR_ESCAPE));
			//trap_SendConsoleCommand( EXEC_APPEND, va("echo Click again to confirm Transporter Location.") );
			TransDat[clientNum].LastClick = level.time;
		}
	}
/* RPG-X: - J2J ==[TO DELETE]== (if prooven unneeded)
	else
	{
		//RPG-X J2J - This was above the alt fire if block, but hindered the transporter location setting from correctly working
		//            Moved here just incase it prevents game crashes.
		if ( tr.entityNum >= ENTITYNUM_WORLD )
		{
			return;
		}
		
		if ( tr_ent && tr_ent->client && tr_ent->health > 1 )
		{
			if ( tr_ent->health < tr_ent->client->ps.stats[STAT_MAX_HEALTH] )
			{
				tr_ent->health = tr_ent->health + 5;
			}
		}
	}
*/
}


void WP_SprayVoyagerHypo( gentity_t *ent, qboolean alt_fire )
{
	gentity_t	*tr_ent;
	trace_t		tr;
	vec3_t		mins, maxs, end;
	//vec3_t		vright;
	gentity_t	*t_ent;
	playerState_t *tr_entPs;

	if ( rpg_rangehypo.integer < 8 )//32
	{
		return;
	}

	VectorMA( muzzle, rpg_rangehypo.integer, forward, end );

	VectorSet( maxs, 6, 6, 6 ); //6, 6, 6 
	VectorScale( maxs, -1, mins );

	trap_Trace ( &tr, muzzle, mins, maxs, end, ent->s.number, MASK_OPAQUE|CONTENTS_BODY|CONTENTS_ITEM|CONTENTS_CORPSE );//MASK_SHOT
	
	if(rpg_effectsgun.integer == 1 && IsAdmin(ent) && alt_fire == qtrue && ent->s.weapon == WP_12){
		if(RPGEntityCount != ENTITYNUM_MAX_NORMAL-20){
			//if ( tr.entityNum >= ENTITYNUM_WORLD ) TiM - Meh.  NOTHING stops t3h l33tzor spray
			//{
				//VectorMA( muzzle, 20, forward, muzzle ); //TiM : Why??
				//VectorMA( muzzle, 4, right, muzzle );
				t_ent = G_TempEntity( muzzle, EV_HYPO_PUFF );
				t_ent->s.eventParm = qfalse; //TiM: Event parm is holding a qboolean value for color of spray
				VectorCopy( forward, t_ent->s.angles2 ); //TiM: Holds the directional vector.  This is passed to CG so it can be rendered right
				//ent->think = steam_think;
				//ent->nextthink = level.time + 10000.0;
				return;
			//}
		}else{
			G_LogPrintf("RPG-X WARNING: Max entities about to be hit! Restart the server ASAP or suffer a server crash!\n");
			trap_SendServerCommand( -1, va("print \"^1RPG-X WARNING: Max entities about to be hit! Restart the server ASAP or suffer a server crash!\n\""));
		}
	}

	tr_ent = &g_entities[tr.entityNum];
	//RPG-X: RedTechie - Medics can revive dead people
	if( (tr_ent && tr_ent->client) && (tr_ent->health == 1) && (tr_ent->client->ps.pm_type == PM_DEAD)){ //  && (tr_ent->r.contents == CONTENTS_CORPSE)
		tr_entPs = &tr_ent->client->ps;
		if(rpg_medicsrevive.integer == 1 /*&& (IsAdmin(ent) || g_classData[ent->client->sess.sessionClass].isMedical)*/ /*ent->client->sess.sessionClass == PC_MEDICAL*/){
				ClientSpawn(tr_ent, 1, qtrue);

				//TiM : Hard coded emote.  Makes the player play a 'get up' animation :)
				//G_MoveBox( tr_ent );
				tr_ent->r.contents = CONTENTS_NONE;
				tr_entPs->stats[LEGSANIM] = ((tr_entPs->stats[LEGSANIM] & ANIM_TOGGLEBIT) ^ ANIM_TOGGLEBIT ) | BOTH_GET_UP1;
				tr_entPs->stats[TORSOANIM] = ((tr_entPs->stats[LEGSANIM] & ANIM_TOGGLEBIT) ^ ANIM_TOGGLEBIT ) | BOTH_GET_UP1;
				tr_entPs->stats[EMOTES] |= EMOTE_BOTH | EMOTE_CLAMP_BODY | EMOTE_OVERRIDE_BOTH;
				tr_entPs->stats[TORSOTIMER] = 1700;
				tr_entPs->stats[LEGSTIMER] = 1700;
				tr_entPs->legsAnim = 0;
				tr_entPs->torsoAnim = 0;
				tr_entPs->torsoTimer = 0;
				tr_entPs->legsTimer = 0;

				//tr_entPs->stats[STAT_WEAPONS] = ( 1 << WP_0 );
				//tr_entPs->stats[STAT_HOLDABLE_ITEM] = HI_NONE;
		}
	//RPG-X: RedTechie - Regular functions still work
	}else if ( tr_ent && tr_ent->client && tr_ent->health > 0 )
	{
		tr_entPs = &tr_ent->client->ps;
		if ( rpg_rpg.integer > 0 && g_gametype.integer < GT_TEAM )
		{
			if ( tr_ent->health < tr_entPs->stats[STAT_MAX_HEALTH] )
			{
				tr_ent->health = tr_entPs->stats[STAT_MAX_HEALTH]; //+20
			}
		}
		else
		{
			if ( !OnSameTeam(tr_ent, ent) || g_gametype.integer < GT_TEAM )
			{
				tr_ent->health = 0;
				player_die( tr_ent, ent, ent, 100, MOD_KNOCKOUT );
				G_LogWeaponFire( ent->s.number, WP_12 );
			}
			else if ( OnSameTeam(tr_ent, ent) )
			{
				if ( tr_ent->health < tr_entPs->stats[STAT_MAX_HEALTH] )
				{
					tr_ent->health = tr_ent->health + 20;
				}
			}
		}
	}
	//TiM- else, use it on yourself
	else
	{
		ent->health = ent->client->ps.stats[STAT_MAX_HEALTH];
	}
}

/*
===============
FireWeapon
===============
*/

#define ACCURACY_TRACKING_DELAY			100		// in ms
#define NUM_FAST_WEAPONS				3

void FireWeapon( gentity_t *ent, qboolean alt_fire ) 
{
	float			projsize;

	s_quadFactor = 1;

	ent->client->pers.teamState.lastFireTime = level.time;

	// set aiming directions
	AngleVectors (ent->client->ps.viewangles, forward, right, up);

	if (alt_fire)
	{
		projsize = WP_ShotAltSize[ent->s.weapon];
	}
	else
	{
		projsize = WP_ShotSize[ent->s.weapon];
	}
	CalcMuzzlePoint ( ent, forward, right, up, muzzle, projsize);

	// fire the specific weapon
	switch( ent->s.weapon )
	{
	// Player weapons
	//-----------------
	case WP_5:
		WP_FirePhaser( ent, alt_fire );
		break;
	case WP_6:
		WP_FireCompressionRifle( ent, alt_fire );
		break;
	case WP_1:
		if ( IsAdmin( ent ) && alt_fire )
			WP_FireGrenade( ent, qfalse );
		break;
	case WP_4:
		break;
	case WP_10:
		WP_FireDisruptor( ent, alt_fire );
		break;
	case WP_8:
		WP_FireGrenade( ent, alt_fire );
		break;
	case WP_7:
		WP_FireTetrionDisruptor( ent, alt_fire );
		break;
	case WP_13:
		WP_SprayVoyagerHypo( ent, alt_fire );
		break;
	case WP_9:
		WP_FireQuantumBurst( ent, alt_fire );
		break;
	case WP_2:
		WP_TricorderScan( ent, alt_fire );
		break;
	case WP_3:
		break;
	case WP_15:
		WP_FireHyperspanner(ent, alt_fire);
		break;
	case WP_12:
		WP_SprayVoyagerHypo( ent, alt_fire );
		break;
	case WP_14:
		break;
	case WP_11:
		break;
	default:
		break;
	}
}

/*
============
Laser Sight Stuff

	Laser Sight / Flash Light Functions
============
*/
/*
void Laser_Gen( gentity_t *ent, int type )	{
	gentity_t	*las;
	int oldtype;


	//RPG-X: J2J - Makes sure only admin and marine class can access the laser, quits now if they arnt to avoid problems later.
	if(type == 1)
	{
		if ( (ent->client->sess.sessionClass != PC_ALPHAOMEGA22) && (IsAdmin( ent ) == qfalse) )
		{
				trap_SendServerCommand( ent-g_entities, va("print \"Laser only availible in Marine class!\n\""));
				return;
		}
	}

	//Get rid of you?
	if ( ent->client->lasersight) {
		  oldtype = ent->client->lasersight->s.eventParm;
		  G_FreeEntity( ent->client->lasersight );
		  ent->client->lasersight = NULL;
		  if (oldtype == type)
			  return;
	}

	las = G_Spawn();

	las->nextthink = level.time + 10;
	las->think = Laser_Think;
	las->r.ownerNum = ent->s.number;
	las->parent = ent;
	las->s.eType = ET_LASER;

	//Lets tell it if flashlight or laser
	if (type == 2)	
	{
		las->s.eventParm = 2; //tells CG that it is a flashlight
		las->classname = "flashlight";
	}
	else 
	{

		if ( ent->client->sess.sessionClass != PC_ALPHAOMEGA22 || ent->client->sess.sessionClass != PC_ADMIN)
		{
			trap_SendServerCommand( ent-g_entities, va("print \"Laser only availible in Marine class!\n\""));
			return;
		}

		las->s.eventParm = 1; //tells CG that it is a laser sight
		las->classname = "lasersight";
	}

	ent->client->lasersight = las;
}

void Laser_Think( gentity_t *self )	{
	vec3_t		end, start, forward, up;
	trace_t		tr;

	//If Player Dies, You Die -> now thanks to Camouflage!
	if (self->parent->client->ps.pm_type == PM_DEAD)  {
		G_FreeEntity(self);
		return;
	}

	//Set Aiming Directions
	AngleVectors(self->parent->client->ps.viewangles, forward, right, up);
	CalcMuzzlePoint(self->parent, forward, right, up, start, 12);
	VectorMA (start, 8192, forward, end);

	//Trace Position
	trap_Trace (&tr, start, NULL, NULL, end, self->parent->s.number, MASK_SHOT );

	//Did you not hit anything?
	if (tr.surfaceFlags & SURF_NOIMPACT || tr.surfaceFlags & SURF_SKY)	{
		self->nextthink = level.time + 10;
		trap_UnlinkEntity(self);
		return;
	}

	//Move you forward to keep you visible
	if (tr.fraction != 1)	VectorMA(tr.endpos,-4,forward,tr.endpos);

	//Set Your position
	VectorCopy( tr.endpos, self->r.currentOrigin );
	VectorCopy( tr.endpos, self->s.pos.trBase );

	vectoangles(tr.plane.normal, self->s.angles);

	trap_LinkEntity(self);

	//Prep next move
	self->nextthink = level.time + 10;
}*/

