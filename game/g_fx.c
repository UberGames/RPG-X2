// This file contains game side effects that the designers can place throughout the maps

#include "g_local.h"

#define SPARK_STARTOFF		1

/*QUAKED fx_spark (0 0 1) (-8 -8 -8) (8 8 8) STARTOFF
Emits sparks at the specified point in the specified direction

  "target" - ( optional ) direction to aim the sparks in, otherwise, uses the angles set in the editor.
  "wait(2000)"	- interval between events (randomly twice as long) 
*/

//------------------------------------------
void spark_think( gentity_t *ent )
{
	G_AddEvent( ent, EV_FX_SPARK, 0 );
	ent->nextthink = level.time + 10000.0; // send a refresh message every 10 seconds
}

//T3h TiM-zor was here
void spark_use( gentity_t *self, gentity_t *other, gentity_t *activator ) {
	
	if ( self->count ) {
		self->think = 0;
		self->nextthink = -1;
	}
	else {
		self->think = spark_think;	
		self->nextthink = level.time + 10000.0;
	}

	self->count = !(self->count);
}

//------------------------------------------
void spark_link( gentity_t *ent )
{

	ent->s.time2 = ent->wait;
	if ( ent->target )
	{
		// try to use the target to orient me.
		gentity_t	*target = NULL;
		vec3_t		dir;

		target = G_Find (target, FOFS(targetname), ent->target);

		if (!target)
		{
			Com_Printf("spark_link: target specified but not found: %s\n", ent->target );

			ent->think = 0;
			ent->nextthink = -1;

			return;
		}
		
		VectorSubtract( target->s.origin, ent->s.origin, dir );
		VectorNormalize( dir );
		vectoangles( dir, ent->r.currentAngles );
		VectorCopy( ent->r.currentAngles, ent->s.angles2 );
		//SnapVector( ent->s.angles );
		VectorShort(ent->s.angles2);
		VectorCopy( ent->r.currentAngles, ent->s.apos.trBase );
		SnapVector(ent->s.apos.trBase);
	}

	//TiM : for optional length in other functions
	ent->s.time = 10000;

	if ( !( ent->spawnflags & 1 ) ) {
		G_AddEvent( ent, EV_FX_SPARK, 0 );
		ent->count = 1;

		ent->think = spark_think;	
		ent->nextthink = level.time + 10000.0;
	}
	else {
		ent->count = 0;

		ent->think = 0;	
		ent->nextthink = -1;
	}
}

//------------------------------------------
void SP_fx_spark( gentity_t	*ent )
{
	if (!ent->wait)
	{
		ent->wait = 2000;
	}

	SnapVector(ent->s.origin);
	VectorCopy( ent->s.origin, ent->s.pos.trBase );

	// The thing that this is targetting may not be spawned in yet, so wait a bit to try and link to it
	ent->think = spark_link; 
	ent->nextthink = level.time + 2000;
	ent->use = spark_use;

	trap_LinkEntity( ent );
}


/*QUAKED fx_steam (0 0 1) (-8 -8 -8) (8 8 8) STARTOFF 
Emits steam at the specified point in the specified direction. will point at a target if one is specified.

Use toggleable steam with caution as updates every second instead of every 10 seconds, 
which means it sends 10 times the information that an untoggleable steam will send.

STARTOFF steam is of at spawn  

"targetname" - toggles on/off whenever used
"damage" - damage to apply when caught in steam vent, default - zero damage (no damage). Don't add this unless you really have to.
*/

#define STEAM_STARTOFF		1
#define STEAM_BURSTS		2
#define STEAM_UNLINKED		999

//------------------------------------------
void steam_think( gentity_t *ent )
{
	G_AddEvent( ent, EV_FX_STEAM, 0 );
	if(ent->targetname) //toggleable steam needs to be updated more often
		ent->nextthink = level.time + 1000;
	else
		ent->nextthink = level.time + 10000.0; // send a refresh message every 10 seconds
/*	if ( ent->spawnflags & STEAM_BURSTS )
	{
		ent->nextthink = level.time + 1000 + random() * 500;
	}
	else
	{
		ent->nextthink = level.time + 50;
	}

	// FIXME: This may be a bit weird for steam bursts*/
	// If a fool gets in the bolt path, zap 'em
	if ( ent->damage ) 
	{
		vec3_t	start, temp;
		trace_t	trace;

		VectorSubtract( ent->s.origin2, ent->r.currentOrigin, temp );
		VectorNormalize( temp );
		VectorMA( ent->r.currentOrigin, 1, temp, start );

		trap_Trace( &trace, start, NULL, NULL, ent->s.origin2, -1, MASK_SHOT );//ignore

		if ( trace.fraction < 1.0 )
		{
			if ( trace.entityNum < ENTITYNUM_WORLD )
			{
				gentity_t *victim = &g_entities[trace.entityNum];
				if ( victim && victim->takedamage )
				{
					G_Damage( victim, ent, ent->activator, temp, trace.endpos, ent->damage, 0, MOD_LAVA );
				}
			}
		}
	}

}

//------------------------------------------
void steam_use( gentity_t *self, gentity_t *other, gentity_t *activator )
{
	if(self->count == STEAM_UNLINKED)
		return;
	if ( self->count )
	{
		self->think = 0;
		self->nextthink = -1;
	}
	else
	{
		self->think = steam_think;
		self->nextthink = level.time + 100;
	}
	
	self->count = !self->count;
}

//------------------------------------------
void steam_link( gentity_t *ent )
{
	gentity_t	*target = NULL;
	vec3_t		dir;
	float		len;
	//trace_t		*tr;

	if (ent->target)
	{
		target = G_Find (target, FOFS(targetname), ent->target);
	}

	if (!target)
	{
		Com_Printf("steam_link: unable to find target %s\n", ent->target );

		ent->think = 0;
		ent->nextthink = -1;

		return;
	}


	VectorSubtract( target->s.origin, ent->s.origin, dir );
	len = VectorNormalize(dir);
	VectorCopy(dir, ent->s.angles2);
	//vectoangles(dir, ent->s.angles2); //GSIO01: haha funny thing this made steam buggy
	VectorShort(ent->s.angles2);
	//SnapVector( ent->s.angles2 );
	
	VectorCopy( target->s.origin, ent->s.origin2 );
	SnapVector(ent->s.origin2);

	if(ent->targetname) // toggleable steam needs to be updated more often
		ent->s.time = 1000;
	else
		ent->s.time = 10000;
	
	ent->use = steam_use;

	trap_LinkEntity( ent );

	// this actually creates the continuously-spawning steam jet
	if(!ent->targetname || !(ent->spawnflags & STEAM_STARTOFF))
		G_AddEvent( ent, EV_FX_STEAM, 0 );
	ent->think = steam_think;
	if(ent->targetname && !(ent->spawnflags & STEAM_STARTOFF)) // toggleable steam needs to be updated more often
		ent->nextthink = level.time + 1000;
	else
		ent->nextthink = level.time + 10000;

	// This is used as the toggle switch
	//ent->count = !(ent->spawnflags & STEAM_STARTOFF);
	if(ent->targetname) {
		ent->count = !(ent->spawnflags & STEAM_STARTOFF);
	}
}

//------------------------------------------
void SP_fx_steam( gentity_t	*ent )
{
	SnapVector(ent->s.origin);
	VectorCopy( ent->s.origin, ent->s.pos.trBase );
	trap_LinkEntity( ent );

	// Try to apply defaults if nothing was set
	G_SpawnInt( "damage", "0", &ent->damage );

	ent->count = STEAM_UNLINKED; // so it can't be used before it's linked

	ent->think = steam_link;
	ent->nextthink = level.time + 2000;
}

/*QUAKED fx_bolt (0 0 1) (-8 -8 -8) (8 8 8) SPARKS BORG TAPER SMOOTH
Emits blue ( or borg green ) electric bolts from the specified point to the specified point

  SPARKS - create impact sparks, probably best used for time delayed bolts
  BORG - Make the bolts green

  "wait" - seconds between bolts (0 is always on, default is 2.0, -1 for random number between 0 and 5), bolts are always on for 0.2 seconds
  "damage" - damage per server frame (default 0)
  "random" - bolt chaos (0.1 = too calm, 0.5 = default, 1.0 or higher = pretty wicked)
*/


#define BOLT_SPARKS		(1<<0)
#define BOLT_BORG		(1<<1)

//------------------------------------------
void bolt_think( gentity_t *ent )
{
	vec3_t	start, temp;
	trace_t	trace;

	G_AddEvent( ent, EV_FX_BOLT, ent->spawnflags );
	ent->s.time2 = ent->wait;
	ent->nextthink = level.time + 10000;//(ent->wait + crandom() * ent->wait * 0.25) * 1000;

	// If a fool gets in the bolt path, zap 'em
	if ( ent->damage ) 
	{
		VectorSubtract( ent->s.origin2, ent->r.currentOrigin, temp );
		VectorNormalize( temp );
		VectorMA( ent->r.currentOrigin, 1, temp, start );

		trap_Trace( &trace, start, NULL, NULL, ent->s.origin2, -1, MASK_SHOT );//ignore

		if ( trace.fraction < 1.0 )
		{
			if ( trace.entityNum < ENTITYNUM_WORLD )
			{
				gentity_t *victim = &g_entities[trace.entityNum];
				if ( victim && victim->takedamage )
				{
					G_Damage( victim, ent, ent->activator, temp, trace.endpos, ent->damage, 0, MOD_PHASER_ALT );
				}
			}
		}
	}
	// net optimisations
	SnapVector(ent->s.origin2);
}

//------------------------------------------
void bolt_use( gentity_t *self, gentity_t *other, gentity_t *activator )
{
	if ( self->count )
	{
		self->think = 0;
		self->nextthink = -1;
	}
	else
	{
		self->think = bolt_think;
		self->nextthink = level.time + 200;
	}
	
	self->count = !self->count;
}

//------------------------------------------
void bolt_link( gentity_t *ent )
{
	gentity_t	*target = NULL;
	vec3_t		dir;
	float		len;

	if (ent->target)
	{
		target = G_Find (target, FOFS(targetname), ent->target);
	}

	if (!target)
	{
		Com_Printf("bolt_link: unable to find target %s\n", ent->target );

		ent->think = 0;
		ent->nextthink = -1;

		return;
	}

	VectorSubtract( target->s.origin, ent->s.origin, dir );
	len = VectorNormalize( dir );
	vectoangles( dir, ent->s.angles );
	
	VectorCopy( target->s.origin, ent->s.origin2 );
	SnapVector(ent->s.origin2);

	if ( ent->targetname )
	{
		ent->use = bolt_use;
	}

	G_AddEvent( ent, EV_FX_BOLT, ent->spawnflags );
	ent->s.time2 = ent->wait;
	ent->think = bolt_think;	
	ent->nextthink = level.time + 10000;
	trap_LinkEntity( ent );
}

//------------------------------------------
void SP_fx_bolt( gentity_t *ent )
{
	G_SpawnInt( "damage", "0", &ent->damage );
	G_SpawnFloat( "random", "0.5", &ent->random );
	G_SpawnFloat( "speed", "15.0", &ent->speed );

	// See if effect is supposed to be delayed
	G_SpawnFloat( "wait", "2.0", &ent->wait );

	SnapVector(ent->s.origin);
	VectorCopy( ent->s.origin, ent->s.pos.trBase );

	ent->s.angles2[0] = ent->speed;
	ent->s.angles2[1] = ent->random;

	if (ent->target)
	{
		ent->think = bolt_link;
		ent->nextthink = level.time + 100;
		return;
	}

	trap_LinkEntity( ent );
}


//--------------------------------------------------
/*QUAKED fx_transporter (0 0 1) (-8 -8 -8) (8 8 8)
Emits transporter pad effect at the specified point. just rest it flush on top of the pad. 

*/

void transporter_link( gentity_t *ent )
{
	G_AddEvent( ent, EV_FX_TRANSPORTER_PAD, 0 );
}

//------------------------------------------
void SP_fx_transporter(gentity_t *ent)
{
	SnapVector(ent->s.origin);
	VectorCopy( ent->s.origin, ent->s.pos.trBase );

	ent->think = transporter_link; 
	ent->nextthink = level.time + 2000;

	trap_LinkEntity( ent );
}



/*QUAKED fx_drip (0 0 1) (-8 -8 -8) (8 8 8) STARTOFF

  "damage" -- type of drips. 0 = water, 1 = oil, 2 = green
  "random" -- (0...1) degree of drippiness. 0 = one drip, 1 = Niagara Falls
*/

//------------------------------------------
void drip_think( gentity_t *ent )
{
	G_AddEvent( ent, EV_FX_DRIP, 0 );
	ent->nextthink = level.time + 10000; // send a refresh message every 10 seconds
}

//------------------------------------------
void SP_fx_drip( gentity_t	*ent )
{
	ent->s.time2 = ent->damage; 

	ent->s.angles2[0] = ent->random; 

	SnapVector(ent->s.origin);
	VectorCopy( ent->s.origin, ent->s.pos.trBase );

	ent->think = drip_think; 
	ent->nextthink = level.time + 1000;

	//TiM
	ent->s.time = 10000;

	trap_LinkEntity( ent );
}

//TiM - RPG-X FX Funcs
//Most of these were copied from EF SP, and then modified for compatibility with the EF MP engine
//***********************************************************************************

/*QUAKED fx_fountain (0 0 1) (-8 -8 -8) (8 8 8) STARTOFF

  STARTOFF - Effect spawns in an off state

"targetname" - name of entity when used turns this ent on/off
"target" - link to a notnull entity to position where the end point of this FX is
*/

void fountain_think( gentity_t *ent ) 
{
	G_AddEvent( ent, EV_FX_GARDEN_FOUNTAIN_SPURT, 0 );
	ent->nextthink = level.time + 100;	
}

void fountain_use( gentity_t *self, gentity_t *other, gentity_t *activator )
{
	if ( self->count )
	{
		self->think = 0;
		self->nextthink = -1;
	}
	else
	{
		self->think = fountain_think;
		self->nextthink = level.time + 100;
	}
	
	self->count = !self->count;
}

void SP_fx_fountain ( gentity_t *ent ) {
	gentity_t	*target = NULL;

	if ( ent->target[0] ) {
		target = G_Find (target, FOFS(targetname), ent->target);
	}

	if ( !target ) {
		Com_Printf( S_COLOR_RED "Unable to find target point: %s\n", ent->target );

		ent->think = 0;
		ent->nextthink = 0;
		return;
	}

	SnapVector( ent->s.origin );
	VectorCopy( ent->s.origin, ent->s.pos.trBase );

	//Set end point to be origin2
	VectorCopy( target->s.origin, ent->s.origin2 );

	ent->use = fountain_use;

	//on and/or off state
	ent->count = !( ent->spawnflags & 1 );

	if ( ent->count ) {
		ent->think = fountain_think;
		ent->nextthink = level.time + 100;
	}
	else {
		ent->think = 0;
		ent->nextthink= -1;
	}

	trap_LinkEntity( ent );
}

/*QUAKED fx_surface_explosion (0 0 1) (-8 -8 -8) (8 8 8) NO_SMOKE LOUDER NODAMAGE
Creates a triggerable explosion aimed at a specific point.  Always oriented towards viewer.

  LOUDER - Cheap hack to make the explosion sound louder.
  NODAMAGE - Does no damage

  "target" (optional) If no target is specified, the explosion is oriented up
  "damage" - Damage per blast, default is 50. Damage falls off based on proximity.
  "radius" - blast radius (default 20)
  "speed" - camera shake speed (default 12).  Set to zero to turn camera shakes off
  "targetname" - triggers explosion when used
*/

//------------------------------------------
void surface_explosion_use( gentity_t *self, gentity_t *other, gentity_t *activator)
{

	G_AddEvent( self, EV_FX_SURFACE_EXPLOSION, 0 );
	if ( self->splashDamage )
	{
		G_RadiusDamage( self->r.currentOrigin, self, self->splashDamage, self->splashRadius, self, DAMAGE_RADIUS|DAMAGE_ALL_TEAMS, MOD_EXPLOSION );
	}
}

//------------------------------------------
void surface_explosion_link( gentity_t *ent )
{
	gentity_t	*target = NULL;
	vec3_t		normal;

	target = G_Find (target, FOFS(targetname), ent->target);

	if ( target )
	{
		VectorSubtract( target->s.origin, ent->s.origin, normal );
		VectorNormalize( normal );
	}
	else
	{
		VectorSet(normal, 0, 0, 1);
	}

	VectorCopy( normal, ent->s.origin2 );

	ent->think = 0;
	ent->nextthink = -1;
	ent->use = surface_explosion_use;

	trap_LinkEntity( ent );
}

//------------------------------------------
void SP_fx_surface_explosion( gentity_t *ent )
{
	if ( !(ent->spawnflags&4) )
	{
		G_SpawnInt( "damage", "50", &ent->splashDamage );
		G_SpawnFloat( "radius", "20", &ent->distance ); // was: ent->radius
		ent->splashRadius = 160;
	}

	G_SpawnFloat( "speed", "12", &ent->speed );

	//TiM: Insert relevant params into the entityState struct so it gets passed to the client
	//Hacky this is... mmm yes

	ent->s.angles2[0] = ent->distance; // was: ent->radius
	ent->s.angles2[1] = ent->speed;
	ent->s.time2 = ent->spawnflags;

	// Precaching sounds
/*	if ( ent->spawnflags & 2 )
	{
		G_SoundIndex( "sound/weapons/explosions/explode2.wav" );
	}
	else
	{
		G_SoundIndex( "sound/weapons/explosions/cargoexplode.wav" );
	}
*/
	VectorCopy( ent->s.origin, ent->s.pos.trBase );

	//ent->e_UseFunc = useF_surface_explosion_use;

	//ent->e_ThinkFunc = thinkF_surface_explosion_link;
	ent->use = surface_explosion_use;
	ent->think = surface_explosion_link;
	ent->nextthink = 1000;

	trap_LinkEntity( ent );
}

/*QUAKED fx_blow_chunks (0 0 1) (-8 -8 -8) (8 8 8)
Creates a triggerable chunk spewer that can be aimed at a specific point.

  "target" - (required) Target to spew chunks at
  "targetname" - triggers chunks when used 
  "radius" - Average size of a chunk (default 65)

"material" - default is "metal" - choose from this list:
	None = 0,
	Metal = 1
	Glass = 2
	Glass Metal = 3
	Wood = 4
	Stone = 5
(there will be more eventually lol.. I hope)

*/

//"count" - Number of chunks to spew (default 5)
//"speed" - How fast a chunk will move when it get's spewed (default 175)
//------------------------------------------
void blow_chunks_use( gentity_t *self, gentity_t *other, gentity_t *activator)
{
	self->r.svFlags |= SVF_BROADCAST;
	G_AddEvent( self, EV_FX_CHUNKS, 0 );
}

//------------------------------------------
void blow_chunks_link( gentity_t *ent )
{
	gentity_t	*target = NULL;

	ent->think = 0;
	ent->nextthink = -1;

	target = G_Find (target, FOFS(targetname), ent->target);

	if ( !target )
	{
		Com_Printf("blow_chunks_link: unable to find target %s\n", ent->target );
		return;
	}

	//VectorCopy( target->s.origin, ent->s.origin2 );
	VectorCopy( target->s.origin, ent->s.angles2 );

	trap_LinkEntity( ent );
}

//------------------------------------------
void SP_fx_blow_chunks( gentity_t *ent )
{
	//G_SpawnInt( "count", "5", &ent->count );
	//G_SpawnFloat( "speed", "175", &ent->speed );
	if(!ent->distance) // check for spawnTEnt
		G_SpawnFloat( "radius", "65", &ent->distance ); // was:  ent->radius
	if(!ent->s.powerups) // check for spawnTEnt
		G_SpawnInt( "material", "0", &ent->s.powerups );

	VectorCopy( ent->s.origin, ent->s.pos.trBase );

	//TiM: Fill entityState
	ent->s.time2 = (int)ent->distance; //Hack. :P The client side chunkfunc wants radius to be an int >.< || was: ent->radius
	//ent->s.angles2[1] = ent->speed;

	ent->use = blow_chunks_use;

	ent->think = blow_chunks_link;
	ent->nextthink = 1000;

	trap_LinkEntity( ent );
}

/*QUAKED fx_smoke (0 0 1) (-8 -8 -8) (8 8 8) STARTOFF
Emits cloud of thick black smoke from specified point.

  "target" (option) If no target is specified, the smoke drifts up
  "targetname" - fires only when used
  "radius" - size of the smoke puffs (default 16.0)

*/

//------------------------------------------
void smoke_think( gentity_t *ent )
{
	ent->nextthink = level.time + 10000;
	G_AddEvent( ent, EV_FX_SMOKE, 0 );
}

//------------------------------------------
void smoke_use( gentity_t *self, gentity_t *other, gentity_t *activator)
{
	if ( self->count )
	{
		self->think = 0;
		self->nextthink = -1;
	}
	else
	{
		self->think = smoke_think;
		self->nextthink = level.time + 10000;
	}
	
	self->count = !self->count;
}

//------------------------------------------
void smoke_link( gentity_t *ent )
{
	// this link func is used because the target ent may not have spawned in yet, this
	//	will give it a bit of extra time for that to happen.
	//TiM: Ohhhh that's why.  I thought this was weird lol...
	gentity_t	*target = NULL;
	vec3_t		dir;

	target = G_Find (target, FOFS(targetname), ent->target);
	if (target)
	{
		//VectorCopy( target->s.origin, dir );
		VectorSubtract( target->s.origin, ent->s.origin, dir );
		VectorNormalize( dir );
		vectoangles( dir, ent->s.angles2 );
		VectorShort(ent->s.angles2);
	}
	else
	{
		//Hard code to be directly up
		VectorSet( dir, 0, 0, 10 );
		VectorNormalize( dir );
		vectoangles( dir, ent->s.angles2 );
		VectorShort(ent->s.angles2);
		//VectorCopy( ent->s.origin, dir );
		//dir[2] += 1;	// move up
	}

	if (ent->targetname)
	{
		ent->use = smoke_use;
	}

	if (!ent->targetname || !(ent->spawnflags & 1) )
	{
		ent->think = smoke_think;
		ent->nextthink = level.time + 200;
		ent->count = 1;
	}
	else
	{
		ent->think = 0;
		ent->nextthink = -1;
		ent->count = 0;
	}

	//VectorCopy( dir, ent->s.origin2 );

	ent->s.time2 = 10000;

	trap_LinkEntity( ent );
}

//------------------------------------------
void SP_fx_smoke( gentity_t *ent )
{
	G_SpawnFloat( "radius", "16.0", &ent->distance ); // was: ent->radius

	VectorCopy( ent->s.origin, ent->s.pos.trBase );
	ent->s.angles2[0] = ent->distance; // was: ent->radius

	ent->think = smoke_link;
	ent->nextthink = level.time + 1000;
	
	trap_LinkEntity( ent );
}

/*QUAKED fx_electrical_explosion (0 0 1) (-8 -8 -8) (8 8 8) x x NODAMAGE
Creates a triggerable explosion aimed at a specific point
NODAMAGE - does no damage

  "target" (optional) If no target is specified, the explosion is oriented up
  "damage" - Damage per blast, default is 20. Damage falls off based on proximity.
  "radius" - blast radius (default 50)
  "targetname" - explodes each time it's used

*/

//------------------------------------------
void electrical_explosion_use( gentity_t *self, gentity_t *other, gentity_t *activator)
{

	//self->nextthink = level.time + 100;
	//trap_LinkEntity( self );
	G_AddEvent( self, EV_FX_ELECTRICAL_EXPLOSION, 0 );

	if ( self->splashDamage )
	{
		G_RadiusDamage( self->s.origin, self, self->splashDamage, self->splashRadius, self, 0, MOD_EXPLOSION );
	}
}

//------------------------------------------
void electrical_explosion_link( gentity_t *ent )
{
	gentity_t	*target = NULL;
	vec3_t		normal;

	target = G_Find( target, FOFS(targetname), ent->target );

	if ( target )
	{
		VectorSubtract( target->s.pos.trBase, ent->s.origin, normal );
		VectorNormalize( normal );
	}
	else
	{
		// No target so just shoot up
		VectorSet( normal, 0, 0, 1 );
	}

	VectorCopy( normal, ent->s.origin2 );

	ent->think = 0;
	ent->nextthink = -1;

	trap_LinkEntity( ent );
}

//------------------------------------------
void SP_fx_electrical_explosion( gentity_t *ent )
{
	if ( !(ent->spawnflags&4) )
	{
		G_SpawnInt( "damage", "20", &ent->splashDamage );
		G_SpawnFloat( "radius", "50", &ent->distance ); // was: ent->radius
		ent->splashRadius = 80;
	}

	// Precaching sounds
//	G_SoundIndex( "sound/weapons/explosions/cargoexplode.wav" );

	VectorCopy( ent->s.origin, ent->s.pos.trBase );
	ent->s.angles2[0] = ent->distance; // was: ent->radius

	ent->think = electrical_explosion_link;
	ent->nextthink = level.time + 1000;

	ent->use = electrical_explosion_use;
	trap_LinkEntity( ent );
}

/*QUAKED fx_phaser (0 0 1) (-8 -8 -8) (8 8 8) NO_SOUND DISRUPTOR
A phaser effect.

"target"	endpoint
"wait"		how long the phaser fires
"scale"		adjust the effects scale, default: 20
"customSnd" use a custom sound
"delay"		delay the effect, but not the sound. Can be used to adhust the timing between effect and customSnd
"impact"	set to 1 if you want an impact to be drawn
*/
#define PHASER_FX_UNLINKED 999

void phaser_use(gentity_t *ent, gentity_t *other, gentity_t *activator) {
	if(ent->count == PHASER_FX_UNLINKED)
		return;
	if(ent->spawnflags & 2)
	{ 
		G_AddEvent(ent, EV_FX_DISRUPTOR, 0);
	}
	else 
	{
		G_AddEvent(ent, EV_FX_PHASER, 0);
	}

}

void phaser_link(gentity_t *ent) {
	gentity_t *target = NULL;
	target = G_Find(target, FOFS(targetname), ent->target);
	if(!target) {
		G_Printf("fx_phaser at %s with an unfound target %s!\n", vtos(ent->r.currentOrigin), ent->target);
		G_FreeEntity(ent);
		return;
	}
	VectorCopy(target->s.origin, ent->s.origin2);
	SnapVector(ent->s.origin2);
	ent->use = phaser_use;
	ent->count = 0;
	trap_LinkEntity(ent);
}

void SP_fx_phaser(gentity_t *ent) {
	float	scale;
	char	*sound;
	int		impact;
	ent->count = PHASER_FX_UNLINKED;
	if(!ent->target) {
		G_Printf("fx_phaser at %s without target!\n", vtos(ent->r.currentOrigin));
		return;
	}
	G_SpawnFloat("scale", "20", &scale);
	ent->s.angles[0] = scale;
	G_SpawnFloat("delay", "1", &scale);
	ent->s.angles[1] = scale * 1000;
	G_SpawnString("customSnd", "sound/pos_b/phaser.wav", &sound);
	if(!(ent->spawnflags & 1))
		ent->s.time = G_SoundIndex(sound);
	else
		ent->s.time = G_SoundIndex("NULL");
	if(ent->wait)
		ent->s.time2 = ent->wait * 1000;
	else
		ent->s.time2 = 3000;
	G_SpawnInt("impact", "0", & impact);
	ent->s.angles[2] = impact;
	ent->think = phaser_link;
	ent->nextthink = level.time + 1000;
	trap_LinkEntity(ent);
}

/*QUAKED fx_torpedo (0 0 1) (-8 -8 -8) (8 8 8) QUANTUM NO_SOUND
A torpedo effect.

QUANTUM		  set this flag if you whant an quantum fx instead of an photon fx

"target"      used for the calculation of the direction
"wait"		  time in seconds till fx can be used again
"noise"		  sound to play
"soundNoAmmo" sound to play if ammo is depleted
"count"		  ammount of torpedos that can be fired (defaults to -1 = infinite)
"speed"		  a speed modifier (default: 2.5)
*/
void fx_torpedo_use(gentity_t* ent, gentity_t*other, gentity_t *activator);

void fx_torpedo_think(gentity_t *ent) {
	ent->nextthink = -1;
	ent->use = fx_torpedo_use;
}

void fx_torpedo_use(gentity_t *ent, gentity_t *other, gentity_t *activator) {
	if(ent->count > 0) {
		ent->count--;
		trap_SendServerCommand(activator-g_entities, va("print \"Torpedos: %i of %i left.\n\"", ent->count, ent->damage));
		G_AddEvent(ent, EV_GENERAL_SOUND, ent->s.time);
	} else {
		trap_SendServerCommand(activator-g_entities, "print \"^1Out of Torpedos.\n\"");
		G_AddEvent(ent, EV_GENERAL_SOUND, ent->n00bCount /*ent->soundLocked*/);
		return;
	}
	G_AddEvent(ent, EV_FX_TORPEDO, ent->spawnflags);
	ent->use = 0;
	ent->think = fx_torpedo_think;
	ent->nextthink = level.time + ent->wait;
}

void fx_torpedo_link(gentity_t *ent) {
	vec3_t dir;
	gentity_t *target = NULL;
	target = G_Find(target, FOFS(targetname), ent->target);
	if(!target) {
		G_Printf("fx_torpedo at %s with unfound target: %s\n", vtos(ent->s.origin), ent->target);
		G_FreeEntity(ent);
		return;
	}
	VectorSubtract(target->s.origin, ent->s.origin, dir);
	VectorCopy(target->s.origin, ent->s.origin2);
	VectorNormalize(dir);
	VectorCopy(dir, ent->s.angles);
	trap_LinkEntity(ent);
	if(ent->wait)
		ent->wait *= 1000;
	if(!ent->count)
		ent->count = -1;
	else
		ent->damage = ent->count;
	if(ent->speed)
		ent->s.angles2[0] = ent->speed;
	else
		ent->s.angles2[0] = 2.5;
	ent->use = fx_torpedo_use;
	ent->nextthink = -1;
}

void SP_fx_torpedo(gentity_t *ent) {
	char	*sound;
	if(!ent->target) {
		G_Printf("fx_torpedo at %s without target\n", vtos(ent->s.origin));
		G_FreeEntity(ent);
		return;
	}
	G_SpawnString("noise", "sound/rpg_runabout/torp.wav", &sound);
	if(!(ent->spawnflags & 2))
		ent->s.time = G_SoundIndex(sound);
	else
		ent->s.time = G_SoundIndex("NULL");
	G_SpawnString("soundNoAmmo", "sound/movers/switches/voyneg.mp3", &sound);
	if(!(ent->spawnflags & 2))
		ent->n00bCount = G_SoundIndex(sound);
	else
		ent->n00bCount = G_SoundIndex("NULL");
	ent->think = fx_torpedo_link;
	ent->nextthink = level.time + 1000;
}

/*QUAKED fx_particle_fire (0 0 1) (-8 -8 -8) (8 8 8) 
A particle based fire effect. Use this sparingly as it is an fps killer.
If you want to use a bunch of fires use fx_fire.

"size"	how big the fire shoud be (default: 10)
*/
void particleFire_think(gentity_t *ent) {
	G_AddEvent(ent, EV_FX_PARTICLEFIRE, ent->count);
	ent->nextthink = level.time + 10000; //refresh every 10 seconds
}

void SP_fx_particleFire(gentity_t *ent) {
	int size;
	G_SpawnInt("size", "10", &size);
	if(!size)
		ent->count = 10;
	else
		ent->count = size;
	trap_LinkEntity(ent);
	ent->nextthink = level.time + 1000;
	ent->think = particleFire_think;

}

/*QUAKED fx_fire (0 0 1) (-8 -8 -8) (8 8 8) 
A fire affect based on the adminguns fire effect.

"size"	 how big the fire shoud be (default: 64)
"angles" fires angles (default: 0 0 0 = UP)
*/
void fire_think(gentity_t *ent) {
	G_AddEvent(ent, EV_FX_FIRE, 1);
	ent->nextthink = level.time + 10000;
}

void SP_fx_fire(gentity_t *ent) {
	int size;
	G_SpawnInt("size", "64", &size);
	if(!size)
		ent->s.time = 64;
	else
		ent->s.time = size;
	ent->s.angles2[2] = 1;
	ent->s.time2 = 10000;
	trap_LinkEntity(ent);
	ent->think = fire_think;
	ent->nextthink = level.time + 1000;
}

// Additional ports from SP by Harry Young

/*QUAKED fx_cooking_steam (0 0 1) (-8 -8 -8) (8 8 8) STARTOFF
Emits slowly moving steam puffs that rise up from the specified point

  "targetname" - toggles effect on/off whenver used
  "distance" - smoke puff size ( default 3.0 )
*/

//------------------------------------------
void cooking_steam_think( gentity_t *ent )
{
	G_AddEvent( ent, EV_FX_COOKING_STEAM, 0 );
	ent->nextthink = level.time + 100;
}

//------------------------------------------
void cooking_steam_use( gentity_t *self, gentity_t *other, gentity_t *activator)
{
	if ( self->count )
	{
		self->think = NULL;
		self->nextthink = -1;
	}
	else
	{
		self->think = cooking_steam_think;
		self->nextthink = level.time + 200;
	}
	
	self->count = !self->count;
}

//------------------------------------------
void SP_fx_cooking_steam( gentity_t	*ent )
{
	if (!ent->distance) ent->distance = 3.0;

	ent->s.angles[0] = ent->distance;

	trap_LinkEntity( ent );

	if (ent->targetname)
	{
		ent->use = cooking_steam_use;
	}

	ent->count = !(ent->spawnflags & 1);

	if (!ent->targetname || !(ent->spawnflags & 1) )
	{
		ent->think = cooking_steam_think;
		ent->nextthink = level.time + 2000;
	}
	else
	{
		ent->think = NULL;
		ent->nextthink = -1;
	}

	VectorCopy( ent->s.origin, ent->s.pos.trBase );
}

/*QUAKED fx_elecfire (0 0 1) (-8 -8 -8) (8 8 8)
Emits sparks at the specified point in the specified direction
Spawns smoke puffs.
*/

//------------------------------------------
void electric_fire_think( gentity_t *ent )
{
	G_AddEvent( ent, EV_FX_ELECFIRE, 0 );
	ent->nextthink = level.time + (750 + (random() * 300));
}

//------------------------------------------
void SP_fx_electricfire( gentity_t	*ent )
{
	ent->think = electric_fire_think;
	ent->nextthink = level.time + 500;

	VectorCopy( ent->s.origin, ent->s.pos.trBase );
	
	// Precaching sounds
	G_SoundIndex("sound/ambience/spark1.wav");
	G_SoundIndex("sound/ambience/spark2.wav");
	G_SoundIndex("sound/ambience/spark3.wav");
	G_SoundIndex("sound/ambience/spark4.wav");
	G_SoundIndex("sound/ambience/spark5.wav");
	G_SoundIndex("sound/ambience/spark6.wav");

	trap_LinkEntity( ent );
}

/*QUAKED fx_forge_bolt (0 0 1) (-8 -8 -8) (8 8 8) STARTOFF DELAYED SPARKS PULSE TAPER SMOOTH
Emits freaky orange bolts, sending pulses down the length of the beam if desired

  STARTOFF - effect is initially off
  DELAYED - bolts are time delayed, otherwise effect continuously fires
  SPARKS - create impact sparks, probably best used for time delayed bolts
  PULSE - sends a pulse down the length of the beam.
  TAPER - Bolt will taper on one end
  SMOOTH - Bolt texture stretches across whole length, makes short bolts look much better.

  "wait" - seconds between bolts, only valid when DELAYED is checked (default 2)
  "damage" - damage per server frame (default 0)
  "targetname" - toggles effect on/off each time it's used
  "random" - bolt chaos (0.1 = too calm, 0.4 = default, 1.0 or higher = pretty wicked)
  "radius" - radius of the bolt (3.0 = default) 
*/

//------------------------------------------
void forge_bolt_think( gentity_t *ent )
{
	G_AddEvent( ent, EV_FX_FORGE_BOLT, ent->spawnflags & 2 );
	ent->nextthink = level.time + (ent->wait + crandom() * ent->wait * 0.25) * 1000;

	// If a fool gets in the bolt path, zap 'em
	if ( ent->damage ) 
	{
		vec3_t	start, temp;
		trace_t	trace;

		VectorSubtract( ent->s.origin2, ent->r.currentOrigin, temp );
		VectorNormalize( temp );
		VectorMA( ent->r.currentOrigin, 1, temp, start );

		trap_Trace( &trace, start, NULL, NULL, ent->s.origin2, -1, MASK_SHOT );//ignore

		if ( trace.fraction < 1.0 )
		{
			if ( trace.entityNum < ENTITYNUM_WORLD )
			{
				gentity_t *victim = &g_entities[trace.entityNum];
				if ( victim && victim->takedamage )
				{
					G_Damage( victim, ent, ent->activator, temp, trace.endpos, ent->damage, 0, MOD_LAVA );
				}
			}
		}
	}
}

//------------------------------------------
void forge_bolt_use( gentity_t *self, gentity_t *other, gentity_t *activator )
{
	if ( self->count )
	{
		self->think = NULL;
		self->nextthink = -1;
	}
	else
	{
		self->think = forge_bolt_think;
		self->nextthink = level.time + 200;
	}
	
	self->count = !self->count;
}

//------------------------------------------
void forge_bolt_link( gentity_t *ent )
{
	gentity_t	*target = NULL;
	vec3_t		dir;
	float		len;

	target = G_Find (target, FOFS(targetname), ent->target);

	if (!target)
	{
		Com_Printf("forge_bolt_link: unable to find target %s\n", ent->target );

		ent->think = NULL;
		ent->nextthink = -1;

		return;
	}

	VectorSubtract( target->s.origin, ent->s.origin, dir );
	len = VectorNormalize( dir );
	vectoangles( dir, ent->s.angles );
	
	VectorCopy( target->s.origin, ent->s.origin2 );

	if ( ent->targetname )
	{
		ent->use = forge_bolt_use;
	}

	// This is used as the toggle switch
	ent->count = !(ent->spawnflags & 1);

	if (!ent->targetname || !(ent->spawnflags & 1) )
	{
		ent->think = forge_bolt_think;	
		ent->nextthink = level.time + 1000;
	}
	else
	{
		ent->think = NULL;
		ent->nextthink = -1;
	}

	trap_LinkEntity( ent );
}

//------------------------------------------
void SP_fx_forge_bolt( gentity_t *ent )
{
	G_SpawnInt( "damage", "0", &ent->damage );
	G_SpawnFloat( "random", "0.4", &ent->random );
	G_SpawnFloat( "radius", "3.0", &ent->distance );

	// See if effect is supposed to be delayed
	if ( ent->spawnflags & 2 )
	{
		G_SpawnFloat( "wait", "2.0", &ent->wait );
	}
	else
	{
		// Effect is continuous
		ent->wait = 0.1f;
	}

	VectorCopy( ent->s.origin, ent->s.pos.trBase );
	ent->wait = level.time + 1000;

	if (ent->target)
	{
		ent->think = forge_bolt_link;
		ent->nextthink = level.time + 100;
		return;
	}

	trap_LinkEntity( ent );
}

/*QUAKED fx_plasma (0 0 1) (-8 -8 -8) (8 8 8) START_OFF
Emits plasma jet directed from the specified point to the specified point. Jet size scales based on length.  

  "target" (required)
  "targetname" - fires only when used
  "startRGBA" - starting cone color, Red Green Blue Alpha 
	(default  100 180 255 255) Light-Blue
  "finalRGBA" - final cone color, Red Green Blue Alpha 
	(default  0 0 180 0) Blue
  "damage" - damage PER FRAME, default zero

*/

//------------------------------------------
void plasma_think( gentity_t *ent )
{
	G_AddEvent( ent, EV_FX_PLASMA, 0 );
	ent->nextthink = level.time + 100;

	// If a fool gets in the plasma cone, fry 'em
	if ( ent->damage ) 
	{
		vec3_t	start, temp;
		trace_t	trace;

		VectorSubtract( ent->s.origin2, ent->r.currentOrigin, temp );
		VectorNormalize( temp );
		VectorMA( ent->r.currentOrigin, 1, temp, start );

		trap_Trace( &trace, start, NULL, NULL, ent->s.origin2, -1, MASK_SHOT );//ignore

		if ( trace.fraction < 1.0 )
		{
			if ( trace.entityNum < ENTITYNUM_WORLD )
			{
				gentity_t *victim = &g_entities[trace.entityNum];
				if ( victim && victim->takedamage )
				{
					G_Damage( victim, ent, ent->activator, temp, trace.endpos, ent->damage, 0, MOD_LAVA );
				}
			}
		}
	}
}

//------------------------------------------
void plasma_use( gentity_t *self, gentity_t *other, gentity_t *activator)
{
	if ( self->count )
	{
		self->think = NULL;	
		self->nextthink = -1;
	}
	else
	{
		self->think = plasma_think;	
		self->nextthink = level.time + 200;
	}
	
	self->count = !self->count;
}

//------------------------------------------
void plasma_link( gentity_t *ent )
{
	gentity_t	*target = NULL;

	target = G_Find (target, FOFS(targetname), ent->target);
	if (!target)
	{
		Com_Printf("plasma_link: unable to find target %s\n", ent->target );

		ent->think = NULL;
		ent->nextthink = -1;

		return;
	}

	if (ent->targetname)
	{
		ent->use = plasma_use;
	}

	if (!ent->targetname || !(ent->spawnflags & 1) )
	{
		ent->think = plasma_think;	
		ent->nextthink = level.time + 200;
	}
	else
	{
		ent->think = NULL;
		ent->nextthink = -1;
	}

	trap_LinkEntity( ent );

	VectorCopy( target->s.origin, ent->s.origin2 );
}

//------------------------------------------
void SP_fx_plasma( gentity_t *ent )
{
	if (!ent->startRGBA)
	{
		ent->startRGBA[0] = 100;
		ent->startRGBA[1] = 180;
		ent->startRGBA[2] = 255;
		ent->startRGBA[3] = 255;
	}

	if (!ent->finalRGBA)
	{
		ent->finalRGBA[2] = 180;
	}

	G_SpawnInt( "damage", "0", &ent->damage );

	// Convert from range of 0-255 to 0-1
	int t;
	for (t=0; t < 4; t++)
	{
		ent->startRGBA[t] = ent->startRGBA[t] / 255;
		ent->finalRGBA[t] = ent->finalRGBA[t] / 255;
	}
	
	VectorCopy( ent->s.origin, ent->s.pos.trBase );

	// This is used as the toggle switch
	ent->count = !(ent->spawnflags & 1);
	
	trap_LinkEntity( ent );

	ent->think = plasma_link;
	ent->nextthink = level.time + 500;
}

/*QUAKED fx_energy_stream (0 0 1) (-8 -8 -8) (8 8 8) STARTOFF
Creates streaming particles that travel between two points--for Stasis level. ONLY orients vertically.
	
 "damage" - amount of damage to player when standing in the stream (default 0)
 "target" (required) End point for particle stream.
 "targetname" - toggle effect on/off each time used.
*/

//------------------------------------------
void stream_think( gentity_t *ent )
{
	G_AddEvent( ent, EV_FX_STREAM, 0 );
	ent->nextthink = level.time + 150;

	// If a fool gets in the bolt path, zap 'em
	if ( ent->damage ) 
	{
		vec3_t	start, temp;
		trace_t	trace;

		VectorSubtract( ent->s.origin2, ent->r.currentOrigin, temp );
		VectorNormalize( temp );
		VectorMA( ent->r.currentOrigin, 1, temp, start );

		trap_Trace( &trace, start, NULL, NULL, ent->s.origin2, -1, MASK_SHOT );//ignore

		if ( trace.fraction < 1.0 )
		{
			if ( trace.entityNum < ENTITYNUM_WORLD )
			{
				gentity_t *victim = &g_entities[trace.entityNum];
				if ( victim && victim->takedamage )
				{
					G_Damage( victim, ent, ent->activator, temp, trace.endpos, ent->damage, 0, MOD_LAVA );
				}
			}
		}
	}
}

//------------------------------------------
void stream_use( gentity_t *self, gentity_t *other, gentity_t *activator)
{
	if ( self->count )
	{
		self->think = stream_think;
		self->nextthink = level.time + 200;
	}
	else
	{
		self->think = NULL;
		self->nextthink = -1;
	}
	
	self->count = !self->count;
}

//------------------------------------------
void stream_link( gentity_t *ent )
{
	gentity_t	*target = NULL;

	target = G_Find (target, FOFS(targetname), ent->target);
	if (!target)
	{
		Com_Printf("stream_link: unable to find target %s\n", ent->target );

		ent->think = NULL;
		ent->nextthink = -1;
		return;
	}

	VectorCopy( target->s.origin, ent->s.origin2 );
	
	if (!ent->targetname || !(ent->spawnflags & 1) )
	{
		ent->think = stream_think;
		ent->nextthink = level.time + 200;
	}
	else if ( ent->spawnflags & 1 )
	{
		ent->think = NULL;
		ent->nextthink = -1;
	}

	trap_LinkEntity( ent );
}

//------------------------------------------
void SP_fx_stream( gentity_t *ent )
{
	G_SpawnInt( "damage", "0", &ent->damage );

	if (ent->targetname)
	{
		ent->use = stream_use;
	}

	ent->count = !(ent->spawnflags & 1);

	VectorCopy( ent->s.origin, ent->s.pos.trBase );

	ent->think = stream_link;
	ent->nextthink = level.time + 1000;

	trap_LinkEntity( ent );
}

/*QUAKED fx_transporter_stream (0 0 1) (-8 -8 -8) (8 8 8) STARTOFF
Creates streaming particles that travel between two points--for forge level.
	
"target" (required) End point for particle stream.
"targetname" - fires only when used

*/

//------------------------------------------
void transporter_stream_think( gentity_t *ent )
{
	G_AddEvent( ent, EV_FX_TRANSPORTER_STREAM, 0 );
	ent->nextthink = level.time + 150;
}

//------------------------------------------
void transporter_stream_use( gentity_t *self, gentity_t *other, gentity_t *activator)
{
	if ( self->count )
	{
		self->think = transporter_stream_think;
		self->nextthink = level.time + 200;
	}
	else
	{
		self->think = NULL;
		self->nextthink = -1;
	}
	
	self->count = !self->count;
}

//------------------------------------------
void transporter_stream_link( gentity_t *ent )
{
	gentity_t	*target = NULL;

	target = G_Find (target, FOFS(targetname), ent->target);

	if (!target)
	{
		Com_Printf( "transporter_stream_link: unable to find target %s\n", ent->target );

		ent->think = NULL;
		ent->nextthink = -1;

		return;
	}

	if (ent->targetname)
	{
		ent->use = transporter_stream_use;
	}

	ent->count = !(ent->spawnflags & 1);

	if (!ent->targetname || !(ent->spawnflags & 1) )
	{
		ent->think = transporter_stream_think;
		ent->nextthink = level.time + 200;
	}
	else
	{
		ent->think = NULL;
		ent->nextthink = -1;
	}

	VectorCopy( target->s.origin, ent->s.origin2 );

	trap_LinkEntity( ent );

}

//------------------------------------------
void SP_fx_transporter_stream( gentity_t *ent )
{
	VectorCopy( ent->s.origin, ent->s.pos.trBase );

	ent->think = transporter_stream_link;
	ent->nextthink = 1000;

	trap_LinkEntity( ent );
}

/*QUAKED fx_explosion_trail (0 0 1) (-8 -8 -8) (8 8 8)
Creates a triggerable explosion aimed at a specific point.  Always oriented towards viewer.

  "target" (required) - end point for the explosion
  "damage" - Damage per blast, default is 150. Damage falls off based on proximity.
  "radius" - blast radius/explosion size (default 80)
  "targetname" - triggers explosion when used
*/

//------------------------------------------
void explosion_trail_use( gentity_t *self, gentity_t *other, gentity_t *activator)
{
	G_AddEvent( self, EV_FX_EXPLOSION_TRAIL, 0 );
}

//------------------------------------------
void explosion_trail_link( gentity_t *ent )
{
	gentity_t	*target = NULL;

	ent->think = NULL;
	ent->nextthink = -1;

	target = G_Find (target, FOFS(targetname), ent->target);

	if ( !target )
	{
		Com_Printf("explosion_trail_link: unable to find target %s\n", ent->target );
		return;
	}

	VectorCopy( target->s.origin, ent->s.origin2 );

	trap_LinkEntity( ent );
}

//------------------------------------------
void SP_fx_explosion_trail( gentity_t *ent )
{
	G_SpawnInt( "damage", "150", &ent->splashDamage );
	G_SpawnFloat( "radius", "80", &ent->distance );

	VectorCopy( ent->s.origin, ent->s.pos.trBase );

	ent->use = explosion_trail_use;

	ent->think = explosion_trail_link;
	ent->nextthink = 1000;

	ent->splashRadius = 160;

	//ent->svFlags |= SVF_BROADCAST;

	trap_LinkEntity( ent );
}

/*QUAKED fx_borg_energy_beam (0 0 1) (-8 -8 -8) (8 8 8) STARTOFF CONE
A borg tracing beam that either carves out a cone or swings like a pendulum, sweeping across an area. 
	
STARTOFF - The trace beam will start when used.
CONE - Beam traces a cone, default trace shape is a pendulum, sweeping across an area.

"radius" - Radius of the area to trace (default 30)
"speed" - How fast the tracer beam moves (default 100)
"startRGBA" - Effect color specified in RED GREEN BLUE ALPHA (default 0 255 0 128)
"target" (required) End point for trace beam, should be placed at the very center of the trace area.
"targetname" - fires only when used

*/

//------------------------------------------
void borg_energy_beam_think( gentity_t *ent )
{
	G_AddEvent( ent, EV_FX_BORG_ENERGY_BEAM, 0 );
	ent->nextthink = level.time + 100;
}

//------------------------------------------
void borg_energy_beam_use( gentity_t *self, gentity_t *other, gentity_t *activator)
{
	if ( self->count )
	{
		self->think = borg_energy_beam_think;
		self->nextthink = level.time + 200;
	}
	else
	{
		self->think = NULL;
		self->nextthink = -1;
	}
	
	self->count = !self->count;
}

//------------------------------------------
void borg_energy_beam_link( gentity_t *ent )
{
	gentity_t	*target = NULL;

	target = G_Find (target, FOFS(targetname), ent->target);

	if (!target)
	{
		Com_Printf( "borg_energy_beam_link: unable to find target %s\n", ent->target );

		ent->think = NULL;
		ent->nextthink = -1;

		return;
	}

	if (ent->targetname)
	{
		ent->use = borg_energy_beam_use;
	}

	ent->count = !(ent->spawnflags & 1);

	if (!ent->targetname || !(ent->spawnflags & 1) )
	{
		ent->think = borg_energy_beam_think;
		ent->nextthink = level.time + 200;
	}
	else
	{
		ent->think = NULL;
		ent->nextthink = -1;
	}

	VectorCopy( target->s.origin, ent->s.origin2 );
	VectorCopy( target->s.origin, ent->pos1 );


	trap_LinkEntity( ent );

}

//------------------------------------------
void SP_fx_borg_energy_beam( gentity_t *ent )
{
	G_SpawnFloat( "radius", "30", &ent->distance );
	G_SpawnFloat( "speed", "100", &ent->speed );
	if (!ent->startRGBA)
	{
		ent->startRGBA[1] = 255;
		ent->startRGBA[3] = 128;
	}

	// Convert from range of 0-255 to 0-1
	int t;
	for (t=0; t < 4; t++)
	{
		ent->startRGBA[t] = ent->startRGBA[t] / 255;
	}

//	ent->svFlags |= SVF_BROADCAST;

	VectorCopy( ent->s.origin, ent->s.pos.trBase );

	ent->think = borg_energy_beam_link;
	ent->nextthink = 1000;

	trap_LinkEntity( ent );
}

/*QUAKED fx_shimmery_thing (0 0 1) (-8 -8 -8) (8 8 8) STARTOFF TAPER 
Creates a shimmering cone or cylinder of colored light that stretches between two points.  Looks like a teleporter type thing. 

  STARTOFF - Effect turns on when used.
  TAPER - Cylinder tapers toward the top, creating a conical effect
  NO_AUTO_SHUTOFF - Tells the effect that it should never try to shut itself off.

  "radius" - radius of the cylinder or of the base of the cone. (default 10)
  "target" (required) End point for stream.
  "targetname" - fires only when used
  "wait" - how long to stay on before turning itself off ( default 2 seconds, -1 to disable auto shut off )

*/

//------------------------------------------
void shimmery_thing_think( gentity_t *ent )
{
	G_AddEvent( ent, EV_FX_SHIMMERY_THING, 0 );
	if ( ent->wait >= 0 )
		ent->nextthink = level.time + ent->wait * 1000;
	else
		ent->nextthink = -1;
}

//------------------------------------------
void shimmery_thing_use( gentity_t *self, gentity_t *other, gentity_t *activator)
{
	if ( self->count )
	{
		self->think = borg_energy_beam_think;
		self->nextthink = level.time + 200;
	}
	else
	{
		self->think = NULL;
		self->nextthink = -1;
	}
	
	self->count = !self->count;
}

//------------------------------------------
void shimmery_thing_link( gentity_t *ent )
{
	gentity_t	*target = NULL;

	target = G_Find (target, FOFS(targetname), ent->target);

	if (!target)
	{
		Com_Printf( "shimmery_thing_link: unable to find target %s\n", ent->target );

		ent->think = NULL;
		ent->nextthink = -1;

		return;
	}

	if (ent->targetname)
	{
		ent->use = shimmery_thing_use;
	}

	ent->count = !(ent->spawnflags & 1);

	if (!ent->targetname || !(ent->spawnflags & 1) )
	{
		ent->think = shimmery_thing_think;
		ent->nextthink = level.time + 200;
	}
	else
	{
		ent->think = NULL;
		ent->nextthink = -1;
	}

	VectorCopy( target->s.origin, ent->s.origin2 );

	trap_LinkEntity( ent );

}

//------------------------------------------
void SP_fx_shimmery_thing( gentity_t *ent )
{
	G_SpawnFloat( "radius", "10", &ent->distance );
	if ( !ent->wait )
		ent->wait = 2;

//	ent->svFlags |= SVF_BROADCAST;

	VectorCopy( ent->s.origin, ent->s.pos.trBase );

	ent->think = shimmery_thing_link;
	ent->nextthink = level.time + 1000;

	trap_LinkEntity( ent );
}

/*QUAKED fx_borg_bolt (0 0 1) (-8 -8 -8) (8 8 8) STARTOFF
Emits yellow electric bolts from the specified point to the specified point.
Emits showers of sparks if the endpoints are sufficiently close.

  STARTOFF - effect is initially off

  "target" (required) end point of the beam.  Can be a func_train, info_notnull, etc.
  "targetname" - toggles effect on/off each time it's used
*/

//------------------------------------------
void borg_bolt_think( gentity_t *ent )
{
	G_AddEvent( ent, EV_FX_BORG_BOLT, 0 );
	ent->nextthink = level.time + 100 + random() * 25;
}

//------------------------------------------
void borg_bolt_use( gentity_t *self, gentity_t *other, gentity_t *activator )
{

	if ( self->count )
	{
		self->think = NULL;
		self->nextthink = -1;
	}
	else
	{
		self->think = borg_bolt_think;
		self->nextthink = level.time + 200;
	}
	
	self->count = !self->count;
}

//------------------------------------------
void borg_bolt_link( gentity_t *ent )
{
	gentity_t	*target = NULL;

	target = G_Find (target, FOFS(targetname), ent->target);

	if (!target)
	{
		Com_Printf("borg_bolt_link: unable to find target %s\n", ent->target );

		ent->think = NULL;
		ent->nextthink = -1;

		return;
	}

//	ent->svFlags |= SVF_BROADCAST;// Broadcast to all clients

	VectorCopy( target->s.origin, ent->s.origin2 );

	if ( ent->targetname )
	{
		ent->use = borg_bolt_use;
	}

	// This is used as the toggle switch
	ent->count = !(ent->spawnflags & 1);

	if (!ent->targetname || !(ent->spawnflags & 1) )
	{
		ent->think = borg_bolt_think;	
		ent->nextthink = level.time + 1000;
	}
	else
	{
		ent->think = NULL;
		ent->nextthink = -1;
	}

	G_SoundIndex( "sound/enemies/borg/borgtaser.wav" );
	trap_LinkEntity( ent );
}

//------------------------------------------
void SP_fx_borg_bolt( gentity_t *ent )
{
	ent->think = borg_bolt_link;
	ent->nextthink = level.time + 1000;

	trap_LinkEntity( ent );
}