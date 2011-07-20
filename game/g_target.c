// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "g_local.h"
//#include <windows.h> //TiM : WTF?

//==========================================================

/*QUAKED target_give (1 0 0) (-8 -8 -8) (8 8 8)
Gives all the weapons specified here in the list.

"items" - separated by ' | ', specify the items
EG "WP_PHASER | WP_TOOLKIT" etc
(Don't forget the spaces!)
*/
void Use_Target_Give( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	int			i;
	playerState_t *ps = &activator->client->ps;

	if ( !activator || !activator->client ) {
		return;
	}

	for ( i=0; i < MAX_WEAPONS; i++ )
	{
		if ( (unsigned int)(ent->s.time) & (1 << i) )
		{
			ps->stats[STAT_WEAPONS] ^= ( 1 << i );
			
			if ( ps->stats[STAT_WEAPONS] & ( 1 << i ) )
				ps->ammo[i] = 1;
			else
				ps->ammo[i] = 0;
			continue;
		}
	}

	//Com_Printf( S_COLOR_RED "Final flags: %u\n", (unsigned int)(ent->s.time) );
}
//FIXME: Make the text parsed on load time. saves on resources!!
void SP_target_give( gentity_t *ent ) 
{
	char	*items;
	char	*textPtr;
	char	*token;
	int		weapon;

	G_SpawnString( "items", "", &items );

	if(!items[0] && ent->target) // spawnTEnt
		items = G_NewString(ent->target);

	textPtr = items;

	//Com_Printf( S_COLOR_RED "Using the Give! Message is %s\n", textPtr );

	COM_BeginParseSession();

	while ( 1 ) 
	{
		token = COM_Parse( &textPtr );
		if ( !token[0] )
			break;
		
		//Com_Printf( S_COLOR_RED "Token: %s\n", token );

		if ( !Q_stricmpn( token, "|", 1 ) )
			continue;

		if( !Q_stricmpn( token, "WP_", 3 ) ) 
		{
			weapon = GetIDForString( WeaponTable, token );

			if ( weapon >= 0 ) 
			{
				ent->s.time |= (1<<weapon);
			}
		}
	}

	//TiM - remove items per server discretion
	if ( rpg_mapGiveFlags.integer > 0 )
		ent->s.time &= rpg_mapGiveFlags.integer;

	//Com_Printf( S_COLOR_RED "Final flags: %u\n", (ent->s.time) );

	ent->use = Use_Target_Give;
}


//==========================================================

/*QUAKED target_remove_powerups (1 0 0) (-8 -8 -8) (8 8 8)
takes away all the activators powerups.
Used to drop flight powerups into death puts.
*/
void Use_target_remove_powerups( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	if ( !activator || !activator->client ) {
		return;
	}

	/*if ( activator->client->ps.powerups[PW_REDFLAG] ) {
		Team_ReturnFlag(TEAM_RED);
	} else if ( activator->client->ps.powerups[PW_BORG_ADAPT] ) {
		Team_ReturnFlag(TEAM_BLUE);
	}*/

	memset( activator->client->ps.powerups, 0, sizeof( activator->client->ps.powerups ) );
}

void SP_target_remove_powerups( gentity_t *ent ) {
	ent->use = Use_target_remove_powerups;
}


//==========================================================

/*QUAKED target_delay (1 0 0) (-8 -8 -8) (8 8 8) SELF
SELF   use the entity as activator instead of it's own activator when using it's targets (use this flag for targets that are target_boolean, targer_alert, and target_warp)

"wait" seconds to pause before firing targets.
"random" delay variance, total delay = delay +/- random seconds
*/
void Think_Target_Delay( gentity_t *ent ) {
	#ifdef G_LUA
	if(ent->luaTrigger)
	{
		if(ent->activator)
		{
			LuaHook_G_EntityTrigger(ent->luaTrigger, ent->s.number, ent->activator->s.number);
		}
		else
		{
			LuaHook_G_EntityTrigger(ent->luaTrigger, ent->s.number, ENTITYNUM_WORLD);
		}
	}
	#endif
	if(ent->spawnflags & 1)
		G_UseTargets(ent, ent);
	else
		G_UseTargets( ent, ent->activator );
}

void Use_Target_Delay( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	ent->nextthink = level.time + ( ent->wait + ent->random * crandom() ) * 1000;
	ent->think = Think_Target_Delay;
	ent->activator = activator;
}

void SP_target_delay( gentity_t *ent ) {
	if ( !ent->wait ) {
		G_SpawnFloat("delay", "0", &ent->wait);
		if(!ent->wait)
			ent->wait = 1;
	}
	ent->use = Use_Target_Delay;
}


//==========================================================

/*QUAKED target_score (1 0 0) (-8 -8 -8) (8 8 8) TEAMSCORE
TEAMSCORE - points are added to activator's team's score, not the individual

"count" number of points to add, default 1

The activator is given this many points.
*/
void Team_AddScore( int team, int points );
void Use_Target_Score (gentity_t *ent, gentity_t *other, gentity_t *activator) {
	if(!activator) return;
	if ( ent->spawnflags & 1 )
	{
		if ( activator->client )
		{
			Team_AddScore( activator->client->sess.sessionTeam, ent->count );
		}
	}
	else
	{
		AddScore( activator, ent->count );
	}
	CalculateRanks( qfalse );
}

void SP_target_score( gentity_t *ent ) {
	if ( !ent->count ) {
		ent->count = 1;
	}
	ent->use = Use_Target_Score;
}


//==========================================================

/*QUAKED target_print (1 0 0) (-8 -8 -8) (8 8 8) redteam blueteam private
"message"	text to print
If "private", only the activator gets the message.  If no checks, all clients get the message.
*/
void Use_Target_Print (gentity_t *ent, gentity_t *other, gentity_t *activator) {
	if ( activator && activator->client && ( ent->spawnflags & 4 ) ) {
		trap_SendServerCommand( activator-g_entities, va("servermsg %s", ent->message ));
		return;
	}

	if ( ent->spawnflags & 3 ) {
		if ( ent->spawnflags & 1 ) {
			G_TeamCommand( TEAM_RED, va("servermsg %s", ent->message) );
		}
		if ( ent->spawnflags & 2 ) {
			G_TeamCommand( TEAM_BLUE, va("servermsg %s", ent->message) );
		}
		return;
	}

	trap_SendServerCommand( -1, va("servermsg %s", ent->message ));
}

void SP_target_print( gentity_t *ent ) {
	ent->use = Use_Target_Print;
}


//==========================================================


/*QUAKED target_speaker (1 0 0) (-8 -8 -8) (8 8 8) looped-on looped-off global activator 
"noise"		wav file to play

A global sound will play full volume throughout the level.
Activator sounds will play on the player that activated the target.
Global and activator sounds can't be combined with looping.
Normal sounds play each time the target is used.
Looped sounds will be toggled by use functions.
Multiple identical looping sounds will just increase volume without any speed cost.
"wait" : Seconds between auto triggerings, 0 = don't auto trigger
"random"	wait variance, default is 0
*/
void Use_Target_Speaker (gentity_t *ent, gentity_t *other, gentity_t *activator) {
	if (ent->spawnflags & 3) {	// looping sound toggles
		if (ent->s.loopSound)
			ent->s.loopSound = 0;	// turn it off
		else
			ent->s.loopSound = ent->noise_index;	// start it
	}else {	// normal sound
		if ( activator && (ent->spawnflags & 8) ) {
			G_AddEvent( activator, EV_GENERAL_SOUND, ent->noise_index );
		} else if (ent->spawnflags & 4) {
			G_AddEvent( ent, EV_GLOBAL_SOUND, ent->noise_index );
		} else {
			G_AddEvent( ent, EV_GENERAL_SOUND, ent->noise_index );
		}
	}
}

void SP_target_speaker( gentity_t *ent ) {
	char	buffer[MAX_QPATH];
	char	*s;

	G_SpawnFloat( "wait", "0", &ent->wait );
	G_SpawnFloat( "random", "0", &ent->random );

	if ( !G_SpawnString( "noise", "NOSOUND", &s ) && !ent->count ) { // if ent->count then it is a spawned sound, either by spawnEnt or *.spawn
		G_Printf( "target_speaker without a noise key at %s", vtos( ent->s.origin ) );
		G_FreeEntity(ent);//let's not error out so that we can use SP maps with their funky speakers.
		return;
	}

	if(!ent->count) { // not by spawnTEnt/*.spawn
		// force all client reletive sounds to be "activator" speakers that
		// play on the entity that activates it
		if ( s[0] == '*' ) {
			ent->spawnflags |= 8;
		}

		memset(buffer, 0, sizeof(buffer));

		Q_strncpyz( buffer, s, sizeof(buffer) );
		COM_DefaultExtension( buffer, sizeof(buffer), ".wav");
		ent->noise_index = G_SoundIndex(buffer);
	} else { // by spawnTEnt or *.spawn file
		ent->noise_index = ent->count;
	}

	// a repeating speaker can be done completely client side
	ent->s.eType = ET_SPEAKER;
	ent->s.eventParm = ent->noise_index;
	ent->s.frame = ent->wait * 10;
	ent->s.clientNum = ent->random * 10;


	// check for prestarted looping sound
	if ( ent->spawnflags & 1 ) {
		ent->s.loopSound = ent->noise_index;
	}

	ent->use = Use_Target_Speaker;

	if (ent->spawnflags & 4) {
		ent->r.svFlags |= SVF_BROADCAST;
	}

	VectorCopy( ent->s.origin, ent->s.pos.trBase );

	// must link the entity so we get areas and clusters so
	// the server can determine who to send updates to
	trap_LinkEntity( ent );
}



//==========================================================

/*QUAKED target_laser (0 .5 .8) (-8 -8 -8) (8 8 8) START_ON
When triggered, fires a laser.  You can either set a target or a direction.
*/
void target_laser_think (gentity_t *self) {
	vec3_t	end;
	trace_t	tr;
	vec3_t	point;

	// if pointed at another entity, set movedir to point at it
	if ( self->enemy ) {
		VectorMA (self->enemy->s.origin, 0.5, self->enemy->r.mins, point);
		VectorMA (point, 0.5, self->enemy->r.maxs, point);
		VectorSubtract (point, self->s.origin, self->movedir);
		VectorNormalize (self->movedir);
	}

	// fire forward and see what we hit
	VectorMA (self->s.origin, 2048, self->movedir, end);

	trap_Trace( &tr, self->s.origin, NULL, NULL, end, self->s.number, CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE);

	if ( tr.entityNum ) {
		// hurt it if we can
		G_Damage ( &g_entities[tr.entityNum], self, self->activator, self->movedir, 
			tr.endpos, self->damage, DAMAGE_NO_KNOCKBACK, MOD_TARGET_LASER);
	}

	VectorCopy (tr.endpos, self->s.origin2);

	trap_LinkEntity( self );
	self->nextthink = level.time + FRAMETIME;
}

void target_laser_on (gentity_t *self)
{
	if (!self->activator)
		self->activator = self;
	target_laser_think (self);
}

void target_laser_off (gentity_t *self)
{
	trap_UnlinkEntity( self );
	self->nextthink = 0;
}

void target_laser_use (gentity_t *self, gentity_t *other, gentity_t *activator)
{
	if(activator)
		self->activator = activator;
	if ( self->nextthink > 0 )
		target_laser_off (self);
	else
		target_laser_on (self);
}

void target_laser_start (gentity_t *self)
{
	gentity_t *ent;

	self->s.eType = ET_BEAM;

	if (self->target) {
		ent = G_Find (NULL, FOFS(targetname), self->target);
		if (!ent) {
			G_Printf ("%s at %s: %s is a bad target\n", self->classname, vtos(self->s.origin), self->target);
			G_FreeEntity(self);
			return;
		}
		self->enemy = ent;
	} else {
		G_SetMovedir (self->s.angles, self->movedir);
	}

	self->use = target_laser_use;
	self->think = target_laser_think;

	if ( !self->damage ) {
		self->damage = 1;
	}

	if (self->spawnflags & 1)
		target_laser_on (self);
	else
		target_laser_off (self);
}

void SP_target_laser (gentity_t *self)
{
	// let everything else get spawned before we start firing
	self->think = target_laser_start;
	self->nextthink = level.time + FRAMETIME;
}


//==========================================================

void target_teleporter_use( gentity_t *self, gentity_t *other, gentity_t *activator ) {
	gentity_t	*dest;
	vec3_t		destPoint;
	vec3_t		tracePoint;
	trace_t		tr;

	if(!Q_stricmp(self->swapname, activator->target)) {
		self->flags ^= FL_LOCKED;
		return;
	}

	if(self->flags & FL_LOCKED)
		return;

	if (!activator || !activator->client)
		return;
	dest = 	G_PickTarget( self->target );
	if (!dest) {
		G_Printf ("Couldn't find teleporter destination\n");
		G_FreeEntity(self);
		return;
	}

	VectorCopy(dest->s.origin, destPoint);

	if ( self->spawnflags & 2 )
	{
		destPoint[2] += dest->r.mins[2]; 
		destPoint[2] -= other->r.mins[2];
		destPoint[2] += 1;
	}
	else
	{
		VectorCopy( dest->s.origin, tracePoint );
		tracePoint[2] -= 4096;

		trap_Trace( &tr, dest->s.origin, dest->r.mins, dest->r.maxs, tracePoint, dest->s.number, MASK_PLAYERSOLID ); 
		VectorCopy( tr.endpos, destPoint );

		//offset the player's bounding box.
		destPoint[2] -= activator->r.mins[2]; //other->r.mins[2];

		//add 1 to ensure non-direct collision
		destPoint[2] += 1;
	}

	if ( self->spawnflags & 1 ) {
		if ( TransDat[activator->client->ps.clientNum].beamTime == 0 ) {
			G_InitTransport( activator->client->ps.clientNum, destPoint, dest->s.angles );				 
		}
	}
	else {
		TeleportPlayer( activator, destPoint, dest->s.angles, TP_NORMAL );
	}
}

/*QUAKED target_teleporter (1 0 0) (-8 -8 -8) (8 8 8) VISUAL_FX SUSPENDED DEACTIVATED
The activator will be instantly teleported away.
VISUAL_FX - Instead of instant teleportation with no FX, entity will play the Star Trek style
transporter effect and teleport over the course of an 8 second cycle.
SUSPENDED - Unless this is checked, the player will materialise on top of the first solid
surface underneath the entity

"targetname" - Any entities targeting this will activate it when used.
"target"     - Name of one or more notnull entities that the player teleport to.
"swapname"	 - Activate/Deactivate (Using entity needs SELF/NOACTIVATOR)

NB-If using the transporter VISUAL_FX, place the target entity so it's right on top of
the surface you want the player to appear on.  It's been hardcoded to take this offset into
account only when the VISUAL_FX flag is on.
*/
void SP_target_teleporter( gentity_t *self ) {
	if (!self->targetname)
		G_Printf("untargeted %s at %s\n", self->classname, vtos(self->s.origin));

	if(self->spawnflags & 4)
		self->flags ^= FL_LOCKED;

	self->use = target_teleporter_use;
}

//==========================================================

/*QUAKED target_relay (.5 .5 .5) (-8 -8 -8) (8 8 8) RED_ONLY BLUE_ONLY RANDOM SELF
This doesn't perform any actions except fire its targets.
The activator can be forced to be from a certain team.
if RANDOM is checked, only one of the targets will be fired, not all of them

SELF	use the entity as activator instead of it's own activator when using it's targets (use this flag for targets that are target_boolean, targer_alert, and target_warp)
*/

void target_relay_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	if ( ( self->spawnflags & 1 ) && activator && activator->client 
		&& activator->client->sess.sessionTeam != TEAM_RED ) {
		return;
	}
	if ( ( self->spawnflags & 2 ) && activator && activator->client 
		&& activator->client->sess.sessionTeam != TEAM_BLUE ) {
		return;
	}

	if(!activator) return;

	if ( self->spawnflags & 4 ) {
		gentity_t	*ent;

		ent = G_PickTarget( self->target );
		if ( ent && ent->use ) {
			if(self->spawnflags & 8) {
				ent->use(ent, self, self);
				#ifdef G_LUA
				if(ent->luaUse)
					LuaHook_G_EntityUse(self->luaUse, self->s.number, other->s.number, activator->s.number);
				#endif
			}
			else {
				ent->use( ent, self, activator );
				#ifdef G_LUA
				if(ent->luaUse)
					LuaHook_G_EntityUse(self->luaUse, self->s.number, other->s.number, self->s.number);
				#endif
			}
		}
		return;
	}
	if(self->spawnflags & 8)
		G_UseTargets(self, self);
	else
		G_UseTargets (self, activator);
}

void SP_target_relay (gentity_t *self) {
	self->use = target_relay_use;
}


//==========================================================

/*QUAKED target_kill (.5 .5 .5) (-8 -8 -8) (8 8 8)
Kills the activator.
*/
void target_kill_use( gentity_t *self, gentity_t *other, gentity_t *activator ) {
	if(activator)
		G_Damage ( activator, NULL, NULL, NULL, NULL, 100000, DAMAGE_NO_PROTECTION, MOD_TELEFRAG);
}

void SP_target_kill( gentity_t *self ) {
	self->use = target_kill_use;
}

/*QUAKED target_position (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for in-game calculation, like jumppad targets.
*/
void SP_target_position( gentity_t *self ){
	G_SetOrigin( self, self->s.origin );
}

static void target_location_linkup(gentity_t *ent)
{
	int i;
	int n;
	//gentity_t *tent;

	if (level.locationLinked) 
		return;

	level.locationLinked = qtrue;

	level.locationHead = NULL;

	trap_SetConfigstring( CS_LOCATIONS, "unknown" );

	for (i = 0, ent = g_entities, n = 1;
			i < level.num_entities;
			i++, ent++) {
		if (ent->classname && !Q_stricmp(ent->classname, "target_location")) {
			// lets overload some variables!
			ent->health = n; // use for location marking
			trap_SetConfigstring( CS_LOCATIONS + n, ent->message );
			n++;
			ent->nextTrain = level.locationHead;
			level.locationHead = ent;
		}
	}

	

	// All linked together now
}

/*QUAKED target_location (0 0.5 0) (-8 -8 -8) (8 8 8)
Set "message" to the name of this location.
Set "count" to 0-7 for color.
0:white 1:red 2:green 3:yellow 4:blue 5:cyan 6:magenta 7:white

Closest target_location in sight used for the location, if none
in site, closest in distance
*/
void SP_target_location( gentity_t *self ){
	self->think = target_location_linkup;
	self->nextthink = level.time + 200;  // Let them all spawn first
	
	//G_Printf( S_COLOR_RED "Location loaded! %s\n", self->message );

	G_SetOrigin( self, self->s.origin );
}

/*QUAKED target_counter (1.0 0 0) (-4 -4 -4) (4 4 4) x x x x x x x x
Acts as an intermediary for an action that takes multiple inputs.

After the counter has been triggered "count" times (default 2), it will fire all of it's targets and remove itself.
*/

void target_counter_use( gentity_t *self, gentity_t *other, gentity_t *activator )
{
	if ( self->count == 0 )
	{
		return;
	}
	
	self->count--;

	if ( self->count )
	{
		return;
	}
	
	if(activator)
		self->activator = activator;
	else 
		self->activator = self;
	G_UseTargets( self, activator );
}

void SP_target_counter (gentity_t *self)
{
	self->wait = -1;
	if (!self->count)
	{
		self->count = 2;
	}

	self->use = target_counter_use;
}

/*QUAKED target_objective (1.0 0 0) (-4 -4 -4) (4 4 4)
When used, the objective in the <mapname>.efo with this objective's "count" will be marked as completed

count - number of objective (as listed in the maps' <mapname>.efo)

NOTE: the objective with the lowest "count" will be considered the current objective
*/

void target_objective_use( gentity_t *self, gentity_t *other, gentity_t *activator )
{
	gentity_t *tent;

	tent = G_TempEntity( self->r.currentOrigin, EV_OBJECTIVE_COMPLETE );

	if(!tent) return; // uh ohhhh 

	//Be sure to send the event to everyone
	tent->r.svFlags |= SVF_BROADCAST;
	tent->s.eventParm = self->count;
}

void SP_target_objective (gentity_t *self)
{
	if ( self->count <= 0 )
	{
		//FIXME: error msg
		G_FreeEntity( self );
		return;
	}
	if ( self->targetname )
	{
		self->use = target_objective_use;
	}
	level.numObjectives++;
}

/*================
RPG-X Modification
Phenix
13/06/2004
================*/

/*QUAKED target_boolean (.5 .5 .5) (-8 -8 -8) (8 8 8) START_TRUE SWAP_FIRE SELF
Acts as an if statement. When fired normaly if true it fires one target, if false it fires another.
START_TRUE		the boolean starts true.
SWAP_FIRE		when the swap command is issued it will also fire the new target.
SELF   			use the entity as activator instead of it's own activator when using it's targets (use this flag for targets that are target_boolean, targer_alert, and target_warp)

"targetname"	this when fired will fire the target according to which state the boolean is in
"swapname"		this when fired will swap the boolean from one state to the opposite
"truename"		this when fired will swap the boolean's state to true
"falsename"		this when fired will sawp the boolean's state to false
"truetarget"	this will be fired if the boolean is true then the targetname is recieved
"falsetarget"	this will be fired if the boolean is false then the targetname is recieved
*/

void target_boolean_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	
	if ((!self) || (!other) || (!activator))
	{
		return;
	}

	if (Q_stricmp(self->truetarget,"(NULL)") == 0)
	{
		G_SpawnString( "truetarget", "DEFAULTTARGET", &self->truetarget );
	}

	if (Q_stricmp(other->target, self->targetname) == 0) {	
		if (self->booleanstate == qtrue) {
			if(self->spawnflags & 4) {
				self->target = self->truetarget;
				G_UseTargets2( self, self, self->truetarget );
			} else {
				G_UseTargets2( self, activator, self->truetarget );
			}
			return;
		} else {
			if(self->spawnflags & 4) {
				self->target = self->falsetarget;
				G_UseTargets2( self, self, self->falsetarget );
			} else {
				G_UseTargets2( self, activator, self->falsetarget );
			}
			return;
		}

	} else if (Q_stricmp(other->target, self->truename) == 0) {
		self->booleanstate = qtrue;					//Make the boolean true
		return;
	} else if (Q_stricmp(other->target, self->falsename) == 0) {
		self->booleanstate = qfalse;					//Make the boolean false
		return;

	} else if (Q_stricmp(other->target, self->swapname) == 0) {
		if (self->booleanstate==qtrue) {			//If the boolean is true then swap to false
			self->booleanstate = qfalse;
			if (self->spawnflags & 2) {
				if(self->spawnflags & 4) {
					self->target = self->falsetarget;
					G_UseTargets2( self, self, self->falsetarget );
				} else {
					G_UseTargets2( self, activator, self->falsetarget );
				}
			}
		} else {
			self->booleanstate = qtrue;
			if (self->spawnflags & 2) {
				if(self->spawnflags & 4) {
					self->target = self->truetarget;
					G_UseTargets2( self, self, self->truetarget );
				} else {
					G_UseTargets2( self, activator, self->truetarget );
				}
			}
		}

		return;
	}
}

void SP_target_boolean (gentity_t *self) {
	if (!self->booleanstate && self->spawnflags & 1) {
		self->booleanstate = qtrue;
	} else if (!self->booleanstate) {
		self->booleanstate = qfalse;
	}
	
	self->use = target_boolean_use;
}

/*QUAKED target_gravity (.5 .5 .5) (-8 -8 -8) (8 8 8) PLAYER_ONLY MAP_GRAV
This changes the servers gravity to the ammount set.
PLAYER_ONLY		If select this will only change the gravity for teh actiator. TiM: an actiator eh?
MAP_GRAV		Will reset player to the current global gravity.

"gravity"	gravity value (default = g_gravity default = 800)
*/

void target_gravity_use (gentity_t *self, gentity_t *other, gentity_t *activator) 
{
	//CIf spawn flag 1 is set, change gravity to specific user
	if((self->spawnflags & 1) && activator && activator->client)
	{
		activator->client->ps.gravity = atoi(self->targetname2);
		activator->client->SpecialGrav = qtrue;
	}
	//resyncing players grav to map grav.
	else if((self->spawnflags & 2) && activator && activator->client)
	{
		activator->client->ps.gravity = g_gravity.integer;
		activator->client->SpecialGrav = qfalse;
	}
	//Else change gravity for all clients
	else
	{
		trap_Cvar_Set( "g_gravity", self->targetname2 );
	}
}

void SP_target_gravity (gentity_t *self) {
	char *temp;
	if(!self->tmpEntity) { // check for spawnTEnt
		G_SpawnString("gravity", "800", &temp);
		self->targetname2 = G_NewString(temp);
	}
	if(self->count) // support for SP
		self->targetname2 = G_NewString(va("%i", self->count));
	self->use = target_gravity_use;
}

/*QUAKED target_shake (.5 .5 .5) (-8 -8 -8) (8 8 8)
When fired every clients monitor will shake as if in an explition //TiM: explition eh?

"wait"	Time that the shaking lasts for in seconds
"intensity"	Strength of shake
*/

void target_shake_use (gentity_t *self, gentity_t *other, gentity_t *activator) 
{
	//trap_SendConsoleCommand( EXEC_APPEND, va("shake %f %2f HRkq1yF22o06Zng9FZXH5sle\n", self->intensity, self->wait) ); //Start Shaking
	//Com_Printf( "Intensity: %f, Duration %i ", self->intensity, ( (int)(level.time - level.startTime) + (int)( self->wait*1000 ) ) ) ;
	
	trap_SetConfigstring( CS_CAMERA_SHAKE, va( "%f %i", self->distance/*was self->intensity*/, ( (int)(level.time - level.startTime) + (int)( self->wait*1000 ) ) ) );
}

void SP_target_shake (gentity_t *self) {
	
	//TiM: Phenix, you're a n00b. You should always put default values in. ;P
	G_SpawnFloat( "intensity", "5", &self->distance /*was &self->intensity*/ );
	G_SpawnFloat( "wait", "5", &self->wait );

	self->use = target_shake_use;
}

/*QUAKED target_evosuit (.5 .5 .5) (-8 -8 -8) (8 8 8)
Used to put an evosuit on or off for each player
*/

void target_evosuit_use (gentity_t *self, gentity_t *other, gentity_t *activator) 
{

	if(!activator || !activator->client) return;

	activator->flags ^= FL_EVOSUIT;
	if (!(activator->flags & FL_EVOSUIT))
	{
		G_PrintfClient(activator, "%s\n", "You have taken an EVA Suit off\n");
		activator->client->ps.powerups[PW_EVOSUIT] = 0;
	}        
	else
	{
		G_PrintfClient(activator, "%s\n", "You have put an EVA Suit on\n");
		activator->client->ps.powerups[PW_EVOSUIT] = level.time + 1000000000;
	}
}

void SP_target_evosuit (gentity_t *self) {
	self->use = target_evosuit_use;
}

//==================================================================================
//
//TiM - Turbolift Ent
//Multiple phases are broken up into multiple think functions
//
//==================================================================================

static void target_turbolift_unlock ( gentity_t *ent )
{
	gentity_t* otherLift;

	//get target deck number lift entity
	otherLift = &g_entities[ent->count];

	//last phase - unlock turbolift doors
	{
		gentity_t *door=NULL;
	
		while ( ( door = G_Find( door, FOFS( targetname ), ent->target )) != NULL  )
		{
			if ( !Q_stricmp( door->classname, "func_door" ) )
			{
				door->flags &= ~FL_CLAMPED;
			}
		}

		door = NULL;
		if ( otherLift ) 
		{
			while ( ( door = G_Find( door, FOFS( targetname ), otherLift->target )) != NULL  )
			{
				if ( !Q_stricmp( door->classname, "func_door" ) )
				{
					door->flags &= ~FL_CLAMPED;
				}
			}
		}
	}

	//reset lifts
	if ( otherLift )
		otherLift->count = 0;
	
	ent->s.time2 = 0;
	if(otherLift)
		otherLift->s.time2 = 0;

	ent->count = 0;
	ent->nextthink = 0;
	ent->think = 0;
}


static void target_turbolift_endMove ( gentity_t *ent )
{
	gentity_t* lights=NULL;
	gentity_t* otherLift=NULL;
	//gentity_t* tent=NULL;

	#ifdef XTRA
	float f = 0;
	#endif

	otherLift = &g_entities[ent->count];
	if ( !otherLift )
	{
		target_turbolift_unlock( ent );
		return;
	}	

	//unplay move sound
	ent->r.svFlags |= SVF_NOCLIENT;
	otherLift->r.svFlags |= SVF_NOCLIENT;

	//play end sound
	G_Sound( ent, ent->s.otherEntityNum2 );
	G_Sound( otherLift, otherLift->s.otherEntityNum2 );

	//unshow flashy bits
	//find any usables parented to the lift ent, and use them
	{
		while ( ( lights = G_Find( lights, FOFS( targetname ), ent->target ) ) != NULL )
		{
			if ( !Q_stricmp( lights->classname, "func_usable" ) )
			{	
				if(!rpg_calcLiftTravelDuration.integer) {
					lights->use( lights, lights, ent );
					#ifdef G_LUA
					if(lights->luaUse)
						LuaHook_G_EntityUse(lights->luaUse, lights-g_entities, ent-g_entities, ent-g_entities);
					#endif
				}
				else {
					if(ent->s.eventParm < 0 && lights->targetname2) {
						if(!Q_stricmp(lights->targetname2, va("%s_dn", ent->target))) {
							lights->use(lights, lights, ent);
							#ifdef G_LUA
							if(lights->luaUse)
								LuaHook_G_EntityUse(lights->luaUse, lights-g_entities, ent-g_entities, ent-g_entities);
							#endif
						}
					} else if(ent->s.eventParm > 0 && lights->targetname2) {
						if(!Q_stricmp(lights->targetname2, va("%s_up", ent->target))) {
							lights->use(lights, lights, ent);
							#ifdef G_LUA
							if(lights->luaUse)
								LuaHook_G_EntityUse(lights->luaUse, lights-g_entities, ent-g_entities, ent-g_entities);
							#endif
						}
					} else {
						lights->use(lights, lights, ent);
						#ifdef G_LUA
						if(lights->luaUse)
							LuaHook_G_EntityUse(lights->luaUse, lights-g_entities, ent-g_entities, ent-g_entities);
						#endif
					}
				}
			}
		}

		lights = NULL;
		while ( ( lights = G_Find( lights, FOFS( targetname ), otherLift->target ) ) != NULL )
		{
			if ( !Q_stricmp( lights->classname, "func_usable" ) )
			{
				if(!rpg_calcLiftTravelDuration.integer) {
					lights->use( lights, lights, ent );
					#ifdef G_LUA
					if(lights->luaUse)
						LuaHook_G_EntityUse(lights->luaUse, lights-g_entities, ent-g_entities, ent-g_entities);
					#endif
				}
				else {
					if(ent->s.eventParm < 0 && lights->targetname2) {
						if(!Q_stricmp(lights->targetname2, va("%s_dn", otherLift->target))) {
							lights->use(lights, lights, ent);
							#ifdef G_LUA
							if(lights->luaUse)
								LuaHook_G_EntityUse(lights->luaUse, lights-g_entities, ent-g_entities, ent-g_entities);
							#endif
						}
					} else if(ent->s.eventParm && lights->targetname2) {
						if(!Q_stricmp(lights->targetname2, va("%s_up", otherLift->target))) {
							lights->use(lights, lights, ent);
							#ifdef G_LUA
							if(lights->luaUse)
								LuaHook_G_EntityUse(lights->luaUse, lights-g_entities, ent-g_entities, ent-g_entities);
							#endif
						}
					} else {
						lights->use(lights, lights, ent);
						#ifdef G_LUA
						if(lights->luaUse)
							LuaHook_G_EntityUse(lights->luaUse, lights-g_entities, ent-g_entities, ent-g_entities);
						#endif
					}
				}
			}
		}
	}

	#ifdef XTRA
	// check for shader remaps
	if(rpg_calcLiftTravelDuration.integer) {
		if((ent->truename && otherLift->truename) || (ent->falsename && otherLift->falsename)) {
			f = level.time * 0.001;
			AddRemap(ent->targetShaderName, ent->targetShaderName, f);
			AddRemap(otherLift->targetShaderName, otherLift->targetShaderName, f);
		}
		trap_SetConfigstring(CS_SHADERSTATE, BuildShaderStateConfig());
	}
	#endif

	//next phase, teleport player
	ent->nextthink = level.time + ent->sound1to2;
	ent->think = target_turbolift_unlock;
}

//TiM - we'll have two sets of teleports, so let's re-use this
static void TeleportPlayers ( gentity_t* ent, gentity_t* targetLift, int numEnts, int *touch )
{
	int			i = 0;
	gentity_t	*player=NULL;
	float		dist;
	vec3_t		temp;
	vec3_t		angles;
	vec3_t		newOrigin;
	vec3_t		viewAng;

	if ( numEnts <= 0 )
		return;

	for ( i = 0; i < numEnts; i++ )
	{
		player = &g_entities[touch[i]];

		if ( !player->client )
			continue;

		//to teleport them, we need two things.  Their distance and angle from the origin
		VectorSubtract( player->client->ps.origin, ent->s.origin, temp );

		//distance + angles
		dist = VectorLength( temp );
		VectorNormalize( temp );
		vectoangles( temp, angles );

		angles[YAW] = AngleNormalize360( angles[YAW] - ent->s.angles[YAW] );

		//now... calc their new origin and view angles
		angles[YAW] = AngleNormalize360( angles[YAW] + targetLift->s.angles[YAW] );
		AngleVectors( angles, temp, NULL, NULL );

		VectorMA( targetLift->s.origin, dist, temp, newOrigin );

		VectorCopy( player->client->ps.viewangles, viewAng );
		viewAng[YAW] = AngleNormalize360( viewAng[YAW] + ( targetLift->s.angles[YAW] - ent->s.angles[YAW] ) );

		TeleportPlayer( player, newOrigin, viewAng, TP_TURBO );
	}
}

#ifdef Q3_VM
static void target_turbolift_TeleportPlayers ( gentity_t *ent )
{
	gentity_t	*targetLift;
	vec3_t		mins, maxs;
	float		time;

	//store both sets of data so they can be swapped at the same time
	int			liftTouch[MAX_GENTITIES];
	int			targetLiftTouch[MAX_GENTITIES];
	int			liftNumEnts;
	int			targetLiftNumEnts;

	//teleport the players
	targetLift = &g_entities[ent->count];

	if ( !targetLift ) {
		target_turbolift_unlock( ent );
		return;
	}

	//scan the turbo region for players
	//in the current lift
	{
		VectorCopy( ent->r.maxs, maxs );
		VectorCopy( ent->r.mins, mins );

		liftNumEnts = trap_EntitiesInBox( mins, maxs, liftTouch, MAX_GENTITIES );
	}

	//the target lift
	{
		VectorCopy( targetLift->r.maxs, maxs );
		VectorCopy( targetLift->r.mins, mins );

		targetLiftNumEnts = trap_EntitiesInBox( mins, maxs, targetLiftTouch, MAX_GENTITIES );
	}

	//TiM - Teleport the players from the other target to this one
	TeleportPlayers( targetLift, ent, targetLiftNumEnts, targetLiftTouch );

	//TiM - Teleport the main players
	TeleportPlayers( ent, targetLift, liftNumEnts, liftTouch );

	if(rpg_calcLiftTravelDuration.integer) {
		time = targetLift->health - ent->health;
		if(time < 0)
			time *= -1;
		time *= rpg_liftDurationModifier.value;
		time *= 1000;
		ent->think = target_turbolift_endMove;
		ent->nextthink = level.time + (time * 0.5f);
	} else {
		//first thing's first
		ent->think = target_turbolift_endMove;
		ent->nextthink = level.time + (ent->wait*0.5f);
	}
}
#else
static void target_turbolift_TeleportPlayers ( gentity_t *ent )
{
	gentity_t	*targetLift;
	vec3_t		mins, maxs;
	float		time;

	//store both sets of data so they can be swapped at the same time
	int			*liftTouch;
	int			*targetLiftTouch;
	int			liftNumEnts;
	int			targetLiftNumEnts;

	//teleport the players
	targetLift = &g_entities[ent->count];

	if ( !targetLift ) {
		target_turbolift_unlock( ent );
		return;
	}

	liftTouch = (int *)malloc(MAX_GENTITIES * sizeof(int));
	if(!liftTouch) {
		target_turbolift_unlock( ent );
		return;
	}

	//scan the turbo region for players
	//in the current lift
	{
		if(!ent->tmpEntity) {
			VectorCopy( ent->r.maxs, maxs );
			VectorCopy( ent->r.mins, mins );
		} else {
			VectorAdd(ent->r.maxs, ent->s.origin, maxs);
			VectorAdd(ent->r.mins, ent->s.origin, mins);
		}

		liftNumEnts = trap_EntitiesInBox( mins, maxs, liftTouch, MAX_GENTITIES );
	}

	targetLiftTouch = (int *)malloc(MAX_GENTITIES * sizeof(int));
	if(!targetLiftTouch) {
		target_turbolift_unlock( ent );
		free(liftTouch);
		return;
	}

	//the target lift
	{
		if(!targetLift->tmpEntity) {
			VectorCopy( targetLift->r.maxs, maxs );
			VectorCopy( targetLift->r.mins, mins );
		} else {
			VectorAdd(targetLift->r.maxs, targetLift->s.origin, maxs);
			VectorAdd(targetLift->r.mins, targetLift->s.origin, mins);
		}

		targetLiftNumEnts = trap_EntitiesInBox( mins, maxs, targetLiftTouch, MAX_GENTITIES );
	}

	//TiM - Teleport the players from the other target to this one
	TeleportPlayers( targetLift, ent, targetLiftNumEnts, targetLiftTouch );

	//TiM - Teleport the main players
	TeleportPlayers( ent, targetLift, liftNumEnts, liftTouch );

	if(rpg_calcLiftTravelDuration.integer) {
		time = targetLift->health - ent->health;
		if(time < 0)
			time *= -1;
		time *= rpg_liftDurationModifier.value;
		time *= 1000;
		ent->think = target_turbolift_endMove;
		ent->nextthink = level.time + (time * 0.5f);
	} else {
		//first thing's first
		ent->think = target_turbolift_endMove;
		ent->nextthink = level.time + (ent->wait*0.5f);
	}

	free(liftTouch);
	free(targetLiftTouch);
}
#endif


static void target_turbolift_startSoundEnd(gentity_t *ent) {
	ent->nextthink = -1;
	ent->parent->r.svFlags &= ~SVF_NOCLIENT;
	ent->touched->r.svFlags &= ~SVF_NOCLIENT;
}

static void target_turbolift_startMove ( gentity_t *ent )
{
	gentity_t*	lights=NULL;
	gentity_t*	otherLift=NULL;
	gentity_t*  tent=NULL;
	float		time = 0, time2 = 0;

	#ifdef XTRA
	float f = 0;
	#endif

	otherLift = &g_entities[ent->count];
	if ( !otherLift )
	{
		target_turbolift_unlock( ent );
		return;
	}	

	//play move sound
	if( rpg_calcLiftTravelDuration.integer ) {
		time = time2 = ent->health - otherLift->health;
		if(time < 0)
			time *= -1;
		if(ent->sound2to1) {
			if( rpg_liftDurationModifier.value * 1000 * time >= ent->distance * 1000 ) {
				tent = G_Spawn();
				tent->think = target_turbolift_startSoundEnd;
				tent->nextthink = level.time + (ent->distance * 1000);
				tent->parent = ent;
				tent->touched = otherLift;
				G_AddEvent(ent, EV_GENERAL_SOUND, ent->sound2to1);
			}
		} else {
			ent->r.svFlags &= ~SVF_NOCLIENT;
			otherLift->r.svFlags &= ~SVF_NOCLIENT;
		}
	} else {
		ent->r.svFlags					&= ~SVF_NOCLIENT;

		otherLift->r.svFlags			&= ~SVF_NOCLIENT;
	}
	//show flashy bits
	//find any usables parented to the lift ent, and use them
	{
		while ( ( lights = G_Find( lights, FOFS( targetname ), ent->target ) ) != NULL )
		{
			if ( !Q_stricmp( lights->classname, "func_usable" ) )
			{
				if(!rpg_calcLiftTravelDuration.integer) {
					lights->use( lights, lights, ent );
					#ifdef G_LUA
					if(lights->luaUse)
						LuaHook_G_EntityUse(lights->luaUse, lights-g_entities, ent-g_entities, ent-g_entities);
					#endif
				}
				else {
					if ( time2 < 0 && lights->targetname2 ) {
						if(!Q_stricmp(lights->targetname2, va("%s_dn", ent->target))) {
							lights->use(lights, lights, ent );
							#ifdef G_LUA
							if(lights->luaUse)
								LuaHook_G_EntityUse(lights->luaUse, lights-g_entities, ent-g_entities, ent-g_entities);
							#endif
						}
					} else if ( time2 > 0 && lights->targetname2 ) {
						if(!Q_stricmp(lights->targetname2, va("%s_up", ent->target))) {
							lights->use(lights, lights, ent );
							#ifdef G_LUA
							if(lights->luaUse)
								LuaHook_G_EntityUse(lights->luaUse, lights-g_entities, ent-g_entities, ent-g_entities);
							#endif
						}
					} else {
						lights->use( lights, lights, ent);
						#ifdef G_LUA
						if(lights->luaUse)
							LuaHook_G_EntityUse(lights->luaUse, lights-g_entities, ent-g_entities, ent-g_entities);
						#endif
					}
				}
			}
		}

		lights = NULL;
		while ( ( lights = G_Find( lights, FOFS( targetname ), otherLift->target ) ) != NULL )
		{
			if ( !Q_stricmp( lights->classname, "func_usable" ) )
			{
				if(!rpg_calcLiftTravelDuration.integer) {
					lights->use( lights, lights, ent );
					#ifdef G_LUA
					if(lights->luaUse)
						LuaHook_G_EntityUse(lights->luaUse, lights-g_entities, ent-g_entities, ent-g_entities);
					#endif
				}
				else {
					if(time2 < 0 && lights->targetname2) {
						if(!Q_stricmp(lights->targetname2, va("%s_dn", otherLift->target))) {
							lights->use(lights, lights, ent);
							#ifdef G_LUA
							if(lights->luaUse);
								LuaHook_G_EntityUse(lights->luaUse, lights-g_entities, ent-g_entities, ent-g_entities);
							#endif
						}
					} else if(time2 > 0 && lights->targetname2) {
						if(!Q_stricmp(lights->targetname2, va("%s_up", otherLift->target))) {
							lights->use(lights, lights, ent);
							#ifdef G_LUA
							if(lights->luaUse)
								LuaHook_G_EntityUse(lights->luaUse, lights-g_entities, ent-g_entities, ent-g_entities);			
							#endif
						}
					} else {
						lights->use(lights, lights, ent);
						#ifdef G_LUA
						if(lights->luaUse)
							LuaHook_G_EntityUse(lights->luaUse, lights-g_entities, ent-g_entities, ent-g_entities);
						#endif
					}
				}
			}
		}
	}

	#ifdef XTRA
	// check for shader remaps
	if(rpg_calcLiftTravelDuration.integer) {
		if(time2 < 0 && ent->truename && otherLift->truename) {
			f = level.time * 0.001;
			AddRemap(ent->targetShaderName, ent->truename, f);
			AddRemap(otherLift->targetShaderName, otherLift->truename, f);
		} else if(time2 >  0 && ent->falsename && otherLift->falsename) {
			f = level.time * 0.001;
			AddRemap(ent->targetShaderName, ent->falsename, f);
			AddRemap(otherLift->targetShaderName, otherLift->falsename, f);
		}
		trap_SetConfigstring(CS_SHADERSTATE, BuildShaderStateConfig());	
	}
	#endif

	if(rpg_calcLiftTravelDuration.integer) {
		/*time = ent->health - otherLift->health;
		if(time < 0)
			time *= -1;*/
		ent->s.eventParm = time2;
		time *= rpg_liftDurationModifier.value;
		time *= 1000;
		ent->s.time2 = level.time + time;
		otherLift->s.time2 = level.time + time;
		ent->nextthink = level.time + (time * 0.5f);
		ent->think = target_turbolift_TeleportPlayers;
	} else {
		//sent to the client for client-side rotation
		ent->s.time2 = level.time+ent->wait;
		otherLift->s.time2 = level.time+ent->wait;

		//next phase, teleport player
		ent->nextthink = level.time + (ent->wait*0.5f);
		ent->think = target_turbolift_TeleportPlayers;
	}
}

static void target_turbolift_shutDoors ( gentity_t *ent )
{
	gentity_t* door=NULL;
	gentity_t* otherLift=NULL;

	otherLift = &g_entities[ent->count];
	if ( !otherLift )
	{
		target_turbolift_unlock( ent );
		return;
	}

	while ( ( door = G_Find( door, FOFS( targetname ), ent->target )) != NULL  )
	{
		if ( !Q_stricmp( door->classname, "func_door" ) )
		{
			if ( door->moverState != MOVER_POS1 ) {
				ent->nextthink = level.time + 500;
				return;
			}
		}
	}

	door = NULL;
	while ( ( door = G_Find( door, FOFS( targetname ), otherLift->target )) != NULL  )
	{
		if ( !Q_stricmp( door->classname, "func_door" ) )
		{
			if ( door->moverState != MOVER_POS1 ) {
				ent->nextthink = level.time + 500;
				return;
			}
		}
	}

	//start phase 3
	ent->think = target_turbolift_startMove;
	ent->nextthink = level.time + FRAMETIME;
}

void target_turbolift_start ( gentity_t *self )
{
	gentity_t* otherLift;

	//get target deck number lift entity
	otherLift = &g_entities[self->count];

	if ( !otherLift )
	{
		target_turbolift_unlock( self );
		return;
	}

	//phase 1 - lock turbolift doors
	//lock the doors on both lifts
	{
		gentity_t *door=NULL;
	
		while ( ( door = G_Find( door, FOFS( targetname ), self->target )) != NULL  )
		{
			if ( !Q_stricmp( door->classname, "func_door" ) )
			{
				door->flags |= FL_CLAMPED;
				if ( door->moverState != MOVER_POS1 )
				{
					door->nextthink = level.time;
				}
			}
		}

		door = NULL;
		while ( ( door = G_Find( door, FOFS( targetname ), otherLift->target )) != NULL  )
		{
			if ( !Q_stricmp( door->classname, "func_door" ) )
			{
				door->flags |= FL_CLAMPED;
				if ( door->moverState != MOVER_POS1 )
				{
					door->nextthink = level.time;
				}
			}
		}
	}

	//phase 2 - wait until both doors are shut
	self->think = target_turbolift_shutDoors;
	self->nextthink = level.time + 500;
}

static void target_turbolift_use( gentity_t *self, gentity_t *other, gentity_t *activator)
{
	if(!Q_stricmp(self->swapname, activator->target)) {
		self->flags ^= FL_LOCKED;
	}

	if(self->flags & FL_LOCKED) return;

	if ( self->count > 0 )
	{
		trap_SendServerCommand( activator-g_entities, "print \"Unable to comply. The lift is currently in use.\n\" " );
		return;
	}

	//trap_SendServerCommand( activator-g_entities, va( "lift %i", (int)(self-g_entities) ) );
	trap_SendServerCommand( activator-g_entities, "lift" );
}

extern void BG_LanguageFilename(char *baseName,char *baseExtension,char *finalName);

/*
QUAKED target_turbolift (.5 .5 .5) ? x x x x x x x x OFFLINE
Turbolifts are delayed teleporters that send players between
each other, maintaining their view and position so the transition is seamless.
If you target this entity with a func_usable, upon activating that useable,
a menu will appear to select decks.  If you target any useables with this
entity, they'll be triggered when the sequence starts (ie scrolling light texture brushes).
If rpg_calcLiftTravelDuration is set to one it is possible to have two usables targeted, one for the
up and one for the down driection in order to use this set targetname2 of those to 
<targetname>_up and <targetname>_dn.
If you target any doors with this entity, they will shut and lock for this sequence.
For the angles, the entity's angle must be aimed at the main set of doors to the lift area.

OFFLINE				Turbolift is offline at start

"deck"				- which deck number this is (You can have multiple lifts of the same deck. Entity fails spawn if not specified)
"deckName"			- name of the main features on this deck (Appears in the deck menu, defaults to 'Unknown')
"wait"				- number of seconds to wait until teleporting the players (1000 = 1 second, default 3000)
"soundLoop"			- looping sound that plays in the wait period (Defaults to EF SP's sound. '*' for none)
"soundEnd"			- sound that plays as the wait period ends. (Defaults to EF SP's sound. '*' for none)
"soundStart			- sound that plays when the lift starts moving
"soundStartLength"	- how long the start sound is in seconds
"waitEnd"			- how long to wait from the lift stopping to the doors opening (default 1000 )
"swapname"			- toggles turbolift on/off
"targetShaderName"	- lights off shader
"falsename"			- lights up
"truename"			- lights down
*/

void SP_target_turbolift ( gentity_t *self )
{
	int				i;
	char*			loopSound;
	char*			endSound;
	char*			idleSound;
	char*			startSound;
	int				len;
	fileHandle_t	f;
	char			fileRoute[MAX_QPATH];
	char			mapRoute[MAX_QPATH];
	char			serverInfo[MAX_TOKEN_CHARS];

	//cache the moving sounds
	G_SpawnString( "soundLoop", "sound/movers/plats/turbomove.wav", &loopSound );
	G_SpawnString( "soundEnd", "sound/movers/plats/turbostop.wav", &endSound );
	G_SpawnString( "soundIdle", "100", &idleSound);
	G_SpawnString( "soundStart", "100", &startSound);
	G_SpawnFloat( "soundStartLength", "100", &self->distance);

	self->s.loopSound				= G_SoundIndex( loopSound ); //looping sound
	self->s.otherEntityNum2			= G_SoundIndex( endSound );	//End Phase sound
	/*self->soundLocked				= G_SoundIndex( idleSound );*/
	self->n00bCount					= G_SoundIndex( idleSound );
	self->sound2to1					= G_SoundIndex( startSound );

	if(self->spawnflags & 512)
		self->flags ^= FL_LOCKED;

	//get deck num
	G_SpawnInt( "deck", "0", &i );
	//kill the ent if it isn't valid
	if ( i <= 0 && !(self->tmpEntity))
	{
		G_Printf( S_COLOR_RED "ERROR: A turbolift entity does not have a valid deck number!\n" );
		G_FreeEntity( self );
		return;
	}

	if(!self->tmpEntity)
		self->health = i;
	self->count = 0; //target/targetted lift
	G_SpawnFloat( "wait", "3000", &self->wait );
	G_SpawnInt( "waitEnd", "1000", &self->sound1to2 );

	if(!self->tmpEntity)
		trap_SetBrushModel( self, self->model );
	self->r.contents = CONTENTS_TRIGGER;		// replaces the -1 from trap_SetBrushModel
	self->r.svFlags = SVF_NOCLIENT;
	self->s.eType = ET_TURBOLIFT;				//TiM - Client-side sound FX
		
	trap_LinkEntity( self );

	VectorCopy( self->r.mins, self->s.angles2 );
	VectorCopy( self->r.maxs, self->s.origin2 );

	VectorAverage( self->r.mins, self->r.maxs, self->s.origin );
	G_SetOrigin( self, self->s.origin );

	//insert code to worry about deck name later
	self->use = target_turbolift_use;

	if ( level.numDecks >= MAX_DECKS )
		return;

	//get the map name out of the server data
	trap_GetServerinfo( serverInfo, sizeof( serverInfo ) );

	//TiM - Configure the deck number and description into a config string
	Com_sprintf( mapRoute, sizeof( mapRoute ), "maps/%s", Info_ValueForKey( serverInfo, "mapname" ) );
	BG_LanguageFilename( mapRoute, "turbolift", fileRoute );

	//Check for a turbolift cfg
	len = trap_FS_FOpenFile( fileRoute, &f, FS_READ );
	trap_FS_FCloseFile( f );

	//if no file was found, resort to the string system.
	//BUT! we shouldn't rely on this system if we can 
	if ( len <= 0 )
	{
		char		infoString[MAX_TOKEN_CHARS];
		char*		deckNamePtr;
		char		deckName[57];
		gentity_t*	prevDeck=NULL;
		qboolean	deckFound=qfalse;

		while ( ( prevDeck = G_Find( prevDeck, FOFS( classname ), "target_turbolift" ) ) != NULL )
		{
			if ( prevDeck != self && prevDeck->health == self->health )
			{
				deckFound = qtrue;
				break;
			}
		}

		//this deck number hasn't been registered b4
		if ( !deckFound )
		{
			G_SpawnString( "deckName", "Unknown", &deckNamePtr );
			Q_strncpyz( deckName, deckNamePtr, sizeof( deckName ) );

			trap_GetConfigstring( CS_TURBOLIFT_DATA, infoString, sizeof( infoString ) );
			
			if ( !infoString[0] )
			{
				Com_sprintf( infoString, sizeof( infoString ), "d%i\\%i\\n%i\\%s\\", level.numDecks, self->health, level.numDecks, deckName );
			}
			else
			{
				Com_sprintf( infoString, sizeof( infoString ), "%sd%i\\%i\\n%i\\%s\\", infoString, level.numDecks, self->health, level.numDecks, deckName );
			}

			trap_SetConfigstring( CS_TURBOLIFT_DATA, infoString );
			level.numDecks++;
		}
	}

	level.numBrushEnts++;
}



/* ==============
END MODIFICATION
===============*/

//RPG-X | GSIO01 | 08/05/2009
/*QUAKED target_doorlock (1 0 0) (-8 -8 -8) (8 8 8) PRIVATE
Locks/Unlocks a door.

PRIVATE		if set, lockMsg/unlockMsg are only printed for activator

"target"    breakable to repair (either it's targetname or it's targetname2)
"lockMsg"   message printed if door gets locked
"unlockMsg" message printed if door gets unlocked
*/
void target_doorLock_use(gentity_t *ent, gentity_t *other, gentity_t* activator) {
	gentity_t	*target = NULL;
	target = G_Find(NULL, FOFS(targetname2), ent->target);
	if(!target) 
		return;
	if(!(target->flags & FL_LOCKED)) {
		if(ent->swapname) {
			if((ent->spawnflags & 1) && activator && activator->client)
				trap_SendServerCommand(activator-g_entities, va("servermsg %s", ent->swapname));
			else
				trap_SendServerCommand(-1, va("servermsg %s", ent->swapname));
		}
	}
	else 
	{
		if(ent->truename) {
			if((ent->spawnflags & 1) && activator && activator->client)
				trap_SendServerCommand(activator-g_entities, va("servermsg %s", ent->truename));
			else
				trap_SendServerCommand(-1, va("servermsg %s", ent->truename));
		}
	}
	if(!Q_stricmp(target->classname, "func_door") || !Q_stricmp(target->classname, "func_door_rotating")) {
		target->flags ^= FL_LOCKED;
	} else {
		G_Printf("Target %s of target_doorlock at %s is not a door!\n");
		return;
	}
}

void SP_target_doorLock(gentity_t *ent) {
	char *temp;
	if(!ent->target) {
		G_Printf("target_doorlock at %s without target!\n");
		G_FreeEntity(ent);
		return;
	}
	G_SpawnString("lockMsg", "", &temp);
	ent->swapname = G_NewString(temp); // ent->swapnmae = temp or strcpy(...) screws everthing up
	G_SpawnString("unlockMsg", "", &temp);
	ent->truename = G_NewString(temp);
	ent->use = target_doorLock_use;
}

//RPG-X | GSIO01 | 11/05/2009 | MOD START
#ifndef XTRA
/*QUAKED target_alert (1 0 0) (-8 -8 -8) (8 8 8) SOUND_TOGGLE SOUND_OFF
This entity acts like 3-Alert-Conditions scripts.

Any of the func_usables that are used as buttons must have the NO_ACTIVATOR spawnflag.

SOUND_TOGGLE		if set the alert sound can be toggled on/off by using the alerts trigger again.
SOUND_OFF			if SOUND_TOGGLE is set, the alert will be silent at beginning

"greenname"			the trigger for green alert should target this				
"yellowname"		the trigger for yellow alert should target this				
"redname"			the trigger for red alert should target this				
"bluename"			the trigger for blue alert should target this				
"greentarget"		anything that should be toggled when activating green alert	
"yellowtarget"		anything that should be toggled when activating yellow alert
"redtarget"			anything that should be toggled when activating red alert	
"bluetarget"		anything that should be toggled when activating blue alert	
"greensnd"			targetname of target_speaker with sound for green alert		
"yellowsnd"			targetname of target_speaker with sound for yellow alert		
"redsnd"			targetname of target_speaker with sound for red alert			
"bluesnd"			targetname of target_speaker with sound for blue alert
*/
void target_alert_use(gentity_t *ent, gentity_t *other, gentity_t *activator) {
	if(!Q_stricmp(activator->target, ent->swapname)) {
		if(ent->damage == 0) {
			if(ent->spawnflags & 1) {
				ent->health = !ent->health;
				ent->target = ent->greensound;
				G_UseTargets(ent, ent);
			}
		} else {
			switch(ent->damage) {
				case 1: // yellow
					if(ent->health) {
						ent->target = ent->yellowsound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					}
					/*if(!ent->spawnflags) {
						ent->target = ent->yellowsound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->falsetarget;
					G_UseTargets(ent, ent);
					break;
				case 2: // red
					if(ent->health) {
						ent->target = ent->redsound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					}
					/*if(!ent->spawnflags) {
						ent->target = ent->redsound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->paintarget;
					G_UseTargets(ent, ent);
					break;
				case 3: // blue
					if(ent->health) {
						ent->target = ent->bluesound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					} 
					/*if(!ent->spawnflags) {
						ent->target = ent->bluesound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->targetname2;
					G_UseTargets(ent, ent);
					break;
			}
			if(!ent->spawnflags) {
				ent->target = ent->greensound;
				G_UseTargets(ent, ent);
			} else if(ent->spawnflags & 2) {
				ent->health = 0;
			} else {
				if(ent->spawnflags) {
					ent->target = ent->greensound;
					G_UseTargets(ent, ent);
					ent->health = 1;
				}
			}
			ent->target = ent->truetarget;
			G_UseTargets(ent, ent);
			ent->damage = 0;
		}
	} else if(!Q_stricmp(activator->target, ent->truename)) {
		if(ent->damage == 1) {
			if(ent->spawnflags & 1) {
				ent->health = !ent->health;
				ent->target = ent->yellowsound;
				G_UseTargets(ent, ent);
			}
		} else {
			switch(ent->damage) {
				case 0: // green
					if(ent->health) {
						ent->target = ent->greensound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					}
					/*if(!ent->spawnflags) {
						ent->target = ent->greensound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->truetarget;
					G_UseTargets(ent, ent);
					break;
				case 2: // red
					if(ent->health) {
						ent->target = ent->redsound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					}
					/*if(!ent->spawnflags) {
						ent->target = ent->redsound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->paintarget;
					G_UseTargets(ent, ent);
					break;
				case 3: // blue
					if(ent->health) {
						ent->target = ent->bluesound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					}
					/*if(!ent->spawnflags) {
						ent->target = ent->bluesound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->targetname2;
					G_UseTargets(ent, ent);
					break;
			}
			if(!ent->spawnflags) {
				ent->target = ent->yellowsound;
				G_UseTargets(ent, ent);
			} else if(ent->spawnflags & 2) {
				ent->health = 0;
			} else {
				if(ent->spawnflags) {
					ent->target = ent->yellowsound;
					G_UseTargets(ent, ent);
					ent->health = 1;
				}
			}
			ent->target = ent->falsetarget;
			G_UseTargets(ent, ent);
			ent->damage = 1;
		}
	} else if(!Q_stricmp(activator->target, ent->falsename)) {
		if(ent->damage == 2) {
			if(ent->spawnflags & 1) {
				ent->health = !ent->health;
				ent->target = ent->redsound;
				G_UseTargets(ent, ent);
			}
		} else {
			switch(ent->damage) {
				case 0: // green
					if(ent->health) {
						ent->target = ent->greensound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					}
					/*if(!ent->spawnflags) {
						ent->target = ent->greensound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->truetarget;
					G_UseTargets(ent, ent);
					break;
				case 1: // ryellow
					if(ent->health) {
						ent->target = ent->yellowsound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					}
					/*if(!ent->spawnflags) {
						ent->target = ent->yellowsound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->falsetarget;
					G_UseTargets(ent, ent);
					break;
				case 3: // blue
					if(ent->health) {
						ent->target = ent->bluesound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					}
					/*if(!ent->spawnflags) {
						ent->target = ent->bluesound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->targetname2;
					G_UseTargets(ent, ent);
					break;
			}
			if(!ent->spawnflags) {
				ent->target = ent->redsound;
				G_UseTargets(ent, ent);
			} else if(ent->spawnflags & 2) {
				ent->health = 0;
			} else {
				if(ent->spawnflags) {
					ent->target = ent->redsound;
					G_UseTargets(ent, ent);
					ent->health = 1;
				}
			}
			ent->target = ent->paintarget;
			G_UseTargets(ent, ent);
			ent->damage = 2;
		}
	} if(!Q_stricmp(activator->target, ent->bluename)) {
		if(ent->damage == 3) {
			if(ent->spawnflags & 1) {
				ent->health = !ent->health;
				ent->target = ent->bluesound;
				G_UseTargets(ent, ent);
			}
		} else {
			switch(ent->damage) {
				case 0: // green
					if(ent->health) {
						ent->target = ent->greensound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					} 
					/*if(!ent->spawnflags) {
						ent->target = ent->greensound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->truetarget;
					G_UseTargets(ent, ent);
					break;
				case 1: // yellow
					if(ent->health) {
						ent->target = ent->yellowsound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					} 
					/*if(!ent->spawnflags) {
						ent->target = ent->yellowsound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->falsetarget;
					G_UseTargets(ent, ent);
					break;
				case 2: // red
					if(ent->health) {
						ent->target = ent->redsound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					} 
					/*if(!ent->spawnflags) {
						ent->target = ent->redsound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->paintarget;
					G_UseTargets(ent, ent);
					break;
			}
			if(!ent->spawnflags) {
				ent->target = ent->bluesound;
				G_UseTargets(ent, ent);
			} else if(ent->spawnflags & 2) {
				ent->health = 0;
			} else {
				if(ent->spawnflags) {
					ent->target = ent->bluesound;
					G_UseTargets(ent, ent);
					ent->health = 1;
				}
			}
			ent->target = ent->targetname2;
			G_UseTargets(ent, ent);
			ent->damage = 3;
		}
	}
	// Free activator if no classname <-- alert command
	if(!activator->classname)
		G_FreeEntity(activator);
}
#else
/*QUAKED target_alert (1 0 0) (-8 -8 -8) (8 8 8) SOUND_TOGGLE SOUND_OFF
This entity acts like 3-Alert-Conditions scripts.

Any of the func_usables that are used as buttons must have the NO_ACTIVATOR spawnflag.

SOUND_TOGGLE		if set the alert sound can be toggled on/off by using the alerts trigger again.
SOUND_OFF			if SOUND_TOGGLE is set, the alert will be silent at beginning

"greenname"			the trigger for green alert should target this				
"yellowname"		the trigger for yellow alert should target this				
"redname"			the trigger for red alert should target this				
"bluename"			the trigger for blue alert should target this				
"greentarget"		anything that should be toggled when activating green alert	
"yellowtarget"		anything that should be toggled when activating yellow alert
"redtarget"			anything that should be toggled when activating red alert	
"bluetarget"		anything that should be toggled when activating blue alert	
"greensnd"			targetname of target_speaker with sound for green alert		
"yellowsnd"			targetname of target_speaker with sound for yellow alert		
"redsnd"			targetname of target_speaker with sound for red alert			
"bluesnd"			targetname of target_speaker with sound for blue alert

----------shader remapping----------
"greenshader"		shadername of condition green
"yellowshader"		shadername of condition yellow
"redshader"			shadername of condition red
"blueshader"		shadername of condition blue

You can remap multiple shaders by separating them with \n.
Example: "greenshader"	"textures/alert/green1\ntextures/alert/green2"
*/
typedef struct {
	char	*greenShaders[10];
	char	*redShaders[10];
	char	*yellowShaders[10];
	char	*blueShaders[10];
	int		numShaders;
} target_alert_Shaders_s;

static target_alert_Shaders_s alertShaders;

void target_alert_remapShaders(int target_condition) {
	float f = 0;
	int i;

	switch(target_condition) {
		case 1: // yellow
			for(i = 0; i < alertShaders.numShaders; i++) {
				f = level.time * 0.001;
				if(!alertShaders.greenShaders[i] || !alertShaders.yellowShaders[i]) break;
				AddRemap(alertShaders.greenShaders[i], alertShaders.yellowShaders[i], f);
			}
			trap_SetConfigstring(CS_SHADERSTATE, BuildShaderStateConfig());
			break;
		case 2: // red
			for(i = 0; i < alertShaders.numShaders; i++) {
				f = level.time * 0.001;
				if(!alertShaders.greenShaders[i] || !alertShaders.redShaders[i]) break;
				AddRemap(alertShaders.greenShaders[i], alertShaders.redShaders[i], f);
			}
			trap_SetConfigstring(CS_SHADERSTATE, BuildShaderStateConfig());
			break;
		case 3: // blue
			for(i = 0; i < alertShaders.numShaders; i++) {
				f = level.time * 0.001;
				if(!alertShaders.greenShaders[i] || !alertShaders.blueShaders[i]) break;
				AddRemap(alertShaders.greenShaders[i], alertShaders.blueShaders[i], f);
			}
			trap_SetConfigstring(CS_SHADERSTATE, BuildShaderStateConfig());
			break;
		case 0: // green
		default:
			for(i = 0; i < alertShaders.numShaders; i++) {
				f = level.time * 0.001;
				if(!alertShaders.greenShaders[i]) break;
				AddRemap(alertShaders.greenShaders[i], alertShaders.greenShaders[i], f);
			}
			trap_SetConfigstring(CS_SHADERSTATE, BuildShaderStateConfig());
			break;
	}

}

void target_alert_use(gentity_t *ent, gentity_t *other, gentity_t *activator) {
	if(!activator) {
		G_Printf(S_COLOR_RED "Error: target_alert_use called with NULL activator.\n");
		return;
	}
	if(!Q_stricmp(activator->target, ent->swapname)) {
		if(ent->damage == 0) {
			if(ent->spawnflags & 1) {
				ent->health = !ent->health;
				ent->target = ent->greensound;
				G_UseTargets(ent, ent);
			}
		} else {
			switch(ent->damage) {
				case 1: // yellow
					if(ent->health) {
						ent->target = ent->yellowsound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					}
					/*if(!ent->spawnflags) {
						ent->target = ent->yellowsound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->falsetarget;
					G_UseTargets(ent, ent);
					break;
				case 2: // red
					if(ent->health) {
						ent->target = ent->redsound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					}
					/*if(!ent->spawnflags) {
						ent->target = ent->redsound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->paintarget;
					G_UseTargets(ent, ent);
					break;
				case 3: // blue
					if(ent->health) {
						ent->target = ent->bluesound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					} 
					/*if(!ent->spawnflags) {
						ent->target = ent->bluesound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->targetname2;
					G_UseTargets(ent, ent);
					break;
			}
			if(!ent->spawnflags) {
				ent->target = ent->greensound;
				G_UseTargets(ent, ent);
			} else if(ent->spawnflags & 2) {
				ent->health = 0;
			} else {
				if(ent->spawnflags) {
					ent->target = ent->greensound;
					G_UseTargets(ent, ent);
					ent->health = 1;
				}
			}
			target_alert_remapShaders(0);
			ent->target = ent->truetarget;
			G_UseTargets(ent, ent);
			ent->damage = 0;
		}
	} else if(!Q_stricmp(activator->target, ent->truename)) {
		if(ent->damage == 1) {
			if(ent->spawnflags & 1) {
				ent->health = !ent->health;
				ent->target = ent->yellowsound;
				G_UseTargets(ent, ent);
			}
		} else {
			switch(ent->damage) {
				case 0: // green
					if(ent->health) {
						ent->target = ent->greensound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					}
					/*if(!ent->spawnflags) {
						ent->target = ent->greensound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->truetarget;
					G_UseTargets(ent, ent);
					break;
				case 2: // red
					if(ent->health) {
						ent->target = ent->redsound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					}
					/*if(!ent->spawnflags) {
						ent->target = ent->redsound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->paintarget;
					G_UseTargets(ent, ent);
					break;
				case 3: // blue
					if(ent->health) {
						ent->target = ent->bluesound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					}
					/*if(!ent->spawnflags) {
						ent->target = ent->bluesound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->targetname2;
					G_UseTargets(ent, ent);
					break;
			}
			if(!ent->spawnflags) {
				ent->target = ent->yellowsound;
				G_UseTargets(ent, ent);
			} else if(ent->spawnflags & 2) {
				ent->health = 0;
			} else {
				if(ent->spawnflags) {
					ent->target = ent->yellowsound;
					G_UseTargets(ent, ent);
					ent->health = 1;
				}
			}
			target_alert_remapShaders(1);
			ent->target = ent->falsetarget;
			G_UseTargets(ent, ent);
			ent->damage = 1;
		}
	} else if(!Q_stricmp(activator->target, ent->falsename)) {
		if(ent->damage == 2) {
			if(ent->spawnflags & 1) {
				ent->health = !ent->health;
				ent->target = ent->redsound;
				G_UseTargets(ent, ent);
			}
		} else {
			switch(ent->damage) {
				case 0: // green
					if(ent->health) {
						ent->target = ent->greensound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					}
					/*if(!ent->spawnflags) {
						ent->target = ent->greensound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->truetarget;
					G_UseTargets(ent, ent);
					break;
				case 1: // ryellow
					if(ent->health) {
						ent->target = ent->yellowsound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					}
					/*if(!ent->spawnflags) {
						ent->target = ent->yellowsound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->falsetarget;
					G_UseTargets(ent, ent);
					break;
				case 3: // blue
					if(ent->health) {
						ent->target = ent->bluesound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					}
					/*if(!ent->spawnflags) {
						ent->target = ent->bluesound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->targetname2;
					G_UseTargets(ent, ent);
					break;
			}
			if(!ent->spawnflags) {
				ent->target = ent->redsound;
				G_UseTargets(ent, ent);
			} else if(ent->spawnflags & 2) {
				ent->health = 0;
			} else {
				if(ent->spawnflags) {
					ent->target = ent->redsound;
					G_UseTargets(ent, ent);
					ent->health = 1;
				}
			}
			target_alert_remapShaders(2);
			ent->target = ent->paintarget;
			G_UseTargets(ent, ent);
			ent->damage = 2;
		}
	} if(!Q_stricmp(activator->target, ent->bluename)) {
		if(ent->damage == 3) {
			if(ent->spawnflags & 1) {
				ent->health = !ent->health;
				ent->target = ent->bluesound;
				G_UseTargets(ent, ent);
			}
		} else {
			switch(ent->damage) {
				case 0: // green
					if(ent->health) {
						ent->target = ent->greensound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					} 
					/*if(!ent->spawnflags) {
						ent->target = ent->greensound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->truetarget;
					G_UseTargets(ent, ent);
					break;
				case 1: // yellow
					if(ent->health) {
						ent->target = ent->yellowsound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					} 
					/*if(!ent->spawnflags) {
						ent->target = ent->yellowsound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->falsetarget;
					G_UseTargets(ent, ent);
					break;
				case 2: // red
					if(ent->health) {
						ent->target = ent->redsound;
						G_UseTargets(ent, ent);
						ent->health = !ent->health;
					} 
					/*if(!ent->spawnflags) {
						ent->target = ent->redsound;
						G_UseTargets(ent, ent);
					}*/
					ent->target = ent->paintarget;
					G_UseTargets(ent, ent);
					break;
			}
			if(!ent->spawnflags) {
				ent->target = ent->bluesound;
				G_UseTargets(ent, ent);
			} else if(ent->spawnflags & 2) {
				ent->health = 0;
			} else {
				if(ent->spawnflags) {
					ent->target = ent->bluesound;
					G_UseTargets(ent, ent);
					ent->health = 1;
				}
			}
			target_alert_remapShaders(3);
			ent->target = ent->targetname2;
			G_UseTargets(ent, ent);
			ent->damage = 3;
		}
	}
	// Free activator if no classname <-- alert command
	if(!activator->classname)
		G_FreeEntity(activator);
}
#endif

#ifdef XTRA
void target_alert_parseShaders(gentity_t *ent) {
	char	buffer[BIG_INFO_STRING];
	char	*txtPtr;
	char	*token;
	int		currentNum = 0;

	alertShaders.numShaders = 0;

	memset(buffer, 0, sizeof(buffer));

	// condition green shaders
	if(!ent->message) return;
	Q_strncpyz(buffer, ent->message, strlen(ent->message));
	txtPtr = buffer;
	token = COM_Parse(&txtPtr);
	while(1) {
		if(!token[0]) break;
		alertShaders.greenShaders[alertShaders.numShaders] = G_NewString(token);
		alertShaders.numShaders++;
		if(alertShaders.numShaders > 9) break;
		token = COM_Parse(&txtPtr);
	}

	// condition red shaders
	if(ent->model) {
		Q_strncpyz(buffer, ent->model, strlen(ent->model));
		txtPtr = buffer;
		token = COM_Parse(&txtPtr);
		while(1) {
			if(!token[0]) break;
			alertShaders.redShaders[currentNum] = G_NewString(token);
			currentNum++;
			if(currentNum > 9) break;
			token = COM_Parse(&txtPtr);
		}

		if(currentNum < alertShaders.numShaders || currentNum > alertShaders.numShaders) {
			G_Printf(S_COLOR_RED "ERROR - target_alert: number of red shaders(%i) does not equal number of green shaders(%i)!\n", currentNum, alertShaders.numShaders);
		}

		currentNum = 0;
	}

	// condition blue shaders
	if(ent->model2) {
		Q_strncpyz(buffer, ent->model2, strlen(ent->model2));
		txtPtr = buffer;
		token = COM_Parse(&txtPtr);
		while(1) {
			if(!token[0]) break;
			alertShaders.blueShaders[currentNum] = G_NewString(token);
			currentNum++;
			if(currentNum > 9) break;
			token = COM_Parse(&txtPtr);
		}

		if(currentNum < alertShaders.numShaders || currentNum > alertShaders.numShaders) {
			G_Printf(S_COLOR_RED "ERROR - target_alert: number of blue shaders(%i) does not equal number of green shaders(%i)!\n", currentNum, alertShaders.numShaders);
		}

		currentNum = 0;
	}

	// condition yellow shaders
	if(ent->team) {
		Q_strncpyz(buffer, ent->team, strlen(ent->team));
		txtPtr = buffer;
		token = COM_Parse(&txtPtr);
		while(1) {
			if(!token[0]) break;
			alertShaders.yellowShaders[currentNum] = G_NewString(token);
			currentNum++;
			if(currentNum > 9) break;
			token = COM_Parse(&txtPtr);
		}

		if(currentNum < alertShaders.numShaders || currentNum > alertShaders.numShaders) {
			G_Printf(S_COLOR_RED "ERROR - target_alert: number of yellow shaders(%i) does not equal number of green shaders(%i)!\n", currentNum, alertShaders.numShaders);
		}
	}
}
#endif

void SP_target_alert(gentity_t *ent) {
	//int			errorNum = 0;
	//qboolean	error = qfalse;
	char		*temp;
	//char		*origin = vtos(ent->s.origin);
	G_SpawnString("greenname", "", &temp);
	ent->swapname = G_NewString(temp);
	G_SpawnString("yellowname", "", &temp);
	ent->truename = G_NewString(temp);
	G_SpawnString("redname", "", &temp);
	ent->falsename = G_NewString(temp);
	G_SpawnString("greentarget", "", &temp);
	ent->truetarget = G_NewString(temp);
	G_SpawnString("yellowtarget", "", &temp);
	ent->falsetarget = G_NewString(temp);
	G_SpawnString("redtarget", "", &temp);
	ent->paintarget = G_NewString(temp);
	G_SpawnString("bluetarget", "", &temp);
	ent->targetname2 = G_NewString(temp);

	#ifdef XTRA
	if(G_SpawnString("greenshader", "", &temp))
		ent->message = G_NewString(temp);
	if(G_SpawnString("yellowshader", "", &temp))
		ent->team = G_NewString(temp);
	if(G_SpawnString("redshader", "", &temp))
		ent->model = G_NewString(temp);
	if(G_SpawnString("blueshader", "", &temp))
		ent->model2 = G_NewString(temp);

	target_alert_parseShaders(ent);
	#endif

	if(!ent->swapname || !ent->truename || !ent->falsename || !ent->bluename ||
		!ent->truetarget || !ent->falsetarget || !ent->paintarget || !ent->targetname2) {
			G_Printf(S_COLOR_RED "One or more needed keys for target_alert at %s where not set.\n", vtos(ent->s.origin));
		return;
	}

	if(!ent->wait)
		ent->wait = 1000;
	else
		ent->wait *= 1000;

	ent->use = target_alert_use;

	ent->damage = 0;

	ent->health = !(ent->spawnflags & 2);
}
//RPG-X | GSIO01 | 11/05/2009 | MOD END

//RPG-X | GSIO01 | 19/05/2009 | MOD START
/*QUAKED target_warp (1 0 0) (-8 -8 -8) (8 8 8) START_ON START_EJECTED START_WARP SELF
An entity that manages warp and warpcore.

Any func_usable using this must have NO_ACTIVATOR flag.
Any target_relay, target_delay, or target_boolean using this must have SELF flag.

START_ON - If set, warpcore is on at start
START_EJECTED - If set, core is ejected at start
START_WARP - ship is on warp at start

"swapWarp"		 targetname to toggle warp
"swapCoreState"	 targetname to toggle core on/off state
"swapCoreEject"	 targetname to toggle core ejected state
"warpTarget"	 target to fire when going to warp
"core"			 target core(func_train)
"coreSwap"		 target for visibility swap
"wait"			 time before warp can be toggled again after retrieving the core(seconds)
"greensnd"		 target_speaker with warp in sound
"yellowsnd"		 target_speaker with warp out sound
"redsnd"		 target_speaker with core off sound
"bluesnd"		 target_speaker with core on sound
*/
void target_warp_use(gentity_t *ent, gentity_t *other, gentity_t* activator);

void target_warp_reactivate(gentity_t *ent) {
	ent->use = target_warp_use;
	ent->nextthink = -1;
}

void target_warp_use(gentity_t *ent, gentity_t *other, gentity_t *activator) {
	int i;
	qboolean first = qtrue;
	gentity_t *target;

	if(!activator) {
		G_Printf(S_COLOR_RED "Error: target_warp_use called with NULL activator!\n");
		return;
	}

	// swapWarp
	if(!Q_stricmp(activator->target, ent->truename)) {
		if(ent->n00bCount) {
			ent->target = ent->truetarget;
			G_UseTargets(ent, activator);
			ent->n00bCount = 0;
			ent->target = ent->yellowsound;
			G_UseTargets(ent, activator);
		}
		/*ent->target = ent->bluename;
		G_UseTargets(ent, ent);*/
		for(i = 0; i < MAX_GENTITIES; i++) {
			if(!&g_entities[i]) continue;
			if(Q_stricmp(g_entities[i].classname, "func_train") && !Q_stricmp(g_entities[i].swapname, ent->bluename)) {
				target = &g_entities[i];
				if(!target) continue;
				if(ent->spawnflags & 4) {
					target->use(target, ent, ent);
					#ifdef G_LUA
					if(target->luaUse)
						LuaHook_G_EntityUse(target->luaUse, target-g_entities, ent-g_entities, ent-g_entities);
					#endif
				} else {
					target->use(target, ent, activator);
					#ifdef G_LUA
					if(target->luaUse)
						LuaHook_G_EntityUse(target->luaUse, target-g_entities, ent-g_entities, activator-g_entities);
					#endif
				}
			} else if(!Q_stricmp(g_entities[i].classname, "func_train") && !Q_stricmp(g_entities[i].swapname, ent->bluename)) {
				target = &g_entities[i];
				if(!target) continue;
				if(target->count == 1) {
					target->s.solid = 0;
					target->r.contents = 0;
					target->clipmask = 0;
					target->r.svFlags |= SVF_NOCLIENT;
					target->s.eFlags |= EF_NODRAW;
					target->count = 0;
					if(first){
						ent->target = ent->redsound;
						G_UseTargets(ent, activator);
						first = qfalse;
					}
				} else {
					target->clipmask = CONTENTS_BODY;
					trap_SetBrushModel( target, target->model );
					//VectorCopy( ent->s.origin, ent->s.pos.trBase );
					//VectorCopy( ent->s.origin, ent->r.currentOrigin );
					target->r.svFlags &= ~SVF_NOCLIENT;
					target->s.eFlags &= ~EF_NODRAW;
					target->clipmask = 0;
					target->count = 1;
					if(first) {
						ent->target = ent->bluesound;
						G_UseTargets(ent, activator);
						first = qfalse;
					}
				}
			}
		}
		ent->sound1to2 = !ent->sound1to2;
	} else if(!Q_stricmp(activator->target, ent->falsename)) { //eject
		if(ent->n00bCount) {
			ent->target = ent->truetarget;
			G_UseTargets(ent, activator);
			ent->n00bCount = 0;
			ent->target = ent->yellowsound;
			G_UseTargets(ent, activator);
		}
		if(ent->sound2to1) {
			ent->use = 0;
			ent->think = target_warp_reactivate;
			ent->nextthink =  level.time + (ent->wait * 1000);
		}
		ent->target = ent->falsetarget;
		G_UseTargets(ent, activator);
		ent->sound2to1 = !ent->sound2to1;
	} else if(!Q_stricmp(activator->target, ent->swapname)) { // toggle warp
		if(ent->sound1to2 && (ent->sound2to1 == 0)) {
			ent->target = ent->truetarget;
			G_UseTargets(ent, activator);
			if(ent->n00bCount)
				ent->target = ent->yellowsound;
			else
				ent->target = ent->greensound;
			G_UseTargets(ent, activator);
			ent->n00bCount = !ent->n00bCount;
		}
	}
}

void SP_target_warp(gentity_t *ent) {
	char *temp;

	G_SpawnString("swapWarp", "", &temp);
	ent->swapname = G_NewString(temp);
	G_SpawnString("swapCoreState", "", &temp);
	ent->truename = G_NewString(temp);
	G_SpawnString("swapCoreEject", "", &temp);
	ent->falsename = G_NewString(temp);
	G_SpawnString("warpTarget", "", &temp);
	ent->truetarget = G_NewString(temp);
	G_SpawnString("core", "", &temp);
	ent->falsetarget = G_NewString(temp);
	G_SpawnString("coreSwap", "", &temp);
	ent->bluename = G_NewString(temp);
	
	//set corestate
	ent->sound1to2 = (ent->spawnflags & 1);

	//set ejected state
	ent->sound2to1 = (ent->spawnflags & 2);

	//set warpstate
	ent->n00bCount = (ent->spawnflags & 4);

	ent->use = target_warp_use;
	
}

//RPG-X | GSIO01 | 19/05/2009 | MOD END
/*QUAKED target_deactivate (1 0 0) (-8 -8 -8) (8 8 8)
This entity can be used to de/activate all func_usables with "target" as targetname2.

"target"	func_usable to de/activate(targetname2).
*/
void target_deactivate_use(gentity_t *ent, gentity_t *other, gentity_t *activator) {
	gentity_t *target = NULL;
	while((target = G_Find(target, FOFS(targetname2), ent->target)) != NULL) {
		if(!Q_stricmp(target->classname, "func_usable")) {
			target->flags ^= FL_LOCKED;
		}
	}
}

void SP_target_deactivate(gentity_t *ent) {
	if(!ent->target) {
		G_Printf("target_deactivate at %s without target!\n", vtos(ent->r.currentOrigin));
		return;
	}

	ent->use = target_deactivate_use;
}

/*QUAKED target_serverchange (1 0 0) (-8 -8 -8) (8 8 8) START_ON
This will make any client inside it connect to a different server.

Can be toggled by an usable if the usable has NO_ACTIVATOR spawnflag.

"serverNum"	server to connect to (rpg_server<serverNum> cvar)
*/
void target_serverchange_think(gentity_t *ent) {
	if(!ent->touched || !ent->touched->client) return;
	trap_SendServerCommand(ent->touched->client->ps.clientNum, va("cg_connect \"%s\"\n", ent->targetname2));
}

void target_serverchange_use(gentity_t *ent, gentity_t *other, gentity_t *activator) {
	char *server;

	if(!activator || !activator->client) {
		ent->s.time2 = !ent->s.time2;
		return;
	}

	if(activator->flags & FL_LOCKED)
		return;

	activator->flags ^= FL_LOCKED;

	if(rpg_serverchange.integer && ent->s.time2) {
		switch(ent->count) {
			default:
			case 1:
				server = rpg_server1.string;
				break;
			case 2:
				server = rpg_server2.string;
				break;
			case 3:
				server = rpg_server3.string;
				break;
			case 4:
				server = rpg_server4.string;
				break;
			case 5:
				server = rpg_server5.string;
				break;
			case 6:
				server = rpg_server6.string;
				break;
		}
		ent->think = target_serverchange_think;
		ent->nextthink = level.time + 3000;
		TransDat[ent->client->ps.clientNum].beamTime = level.time + 8000;
		activator->client->ps.powerups[PW_BEAM_OUT] = level.time + 8000;
		ent->touched = activator;
		ent->targetname2 = server;
	}
}

void SP_target_serverchange(gentity_t *ent) {
	int serverNum;
	G_SpawnInt("serverNum", "1", &serverNum);
	ent->count = serverNum;
	if(!ent->count)
		ent->count = 1;
	if(ent->spawnflags & 1)
		ent->s.time2 = 1;

	ent->use = target_serverchange_use;
	trap_LinkEntity(ent);
}

/*QUAKED target_levelchange (1 0 0) (-8 -8 -8) (8 8 8) 
This will change the map if rpg_allowSPLevelChange is set to 1.

"target" map to load (for example: borg2)
*/
void target_levelchange_think(gentity_t *ent) {
	ent->count--;
	if(ent->count != 0)
		trap_SendServerCommand(-1, va("servercprint \"Mapchange in %i ...\"", ent->count)); 
	else {
		trap_SendConsoleCommand(EXEC_APPEND, va("devmap \"%s\"", ent->target));
		ent->nextthink = -1;
	}
	ent->nextthink = level.time + 1000;
}

void target_levelchange_use(gentity_t *ent, gentity_t *other, gentity_t *activator) {
	if(rpg_allowSPLevelChange.integer) {
		ent->think = target_levelchange_think;
		ent->nextthink = level.time + 1000;
		ent->count = 5;
		trap_SendServerCommand(-1, va("servercprint \"Mapchange in %i ...\"", ent->count)); 
	}
}

void SP_target_levelchange(gentity_t *ent) {
	if(!ent->target) {
		G_Printf(S_COLOR_RED "target_levelchange without target at %s!\n", vtos(ent->s.origin));
		G_FreeEntity(ent);
		return;
	}

	ent->use = target_levelchange_use;
}

/*QUAKED target_holodeck (1 0 0) (-8 -8 -8) (8 8 8)
	
*/

void SP_target_holodeck(gentity_t *ent) {

}