// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "g_local.h"
#include "g_groups.h"

reconData_t	g_reconData[MAX_RECON_NAMES]; //!< recon data for a limited ammount of clients
int			g_reconNum;

extern char* BG_RegisterRace( const char *name );
extern void SetPlayerClassCvar(gentity_t *ent);
extern void SetClass( gentity_t *ent, char *s, char *teamName, qboolean SaveToCvar );
extern void BroadcastClassChange( gclient_t *client, pclass_t oldPClass );

//RPG-X: TiM
extern char* correlateRanks( const char* strArg, int intArg );
extern pclass_t ValueNameForClass ( /*gentity_t *ent,*/ char* s );

extern qboolean levelExiting;
// g_client.c -- client functions that don't happen every frame

void G_StoreClientInitialStatus( gentity_t *ent );

//! players mins
static vec3_t	playerMins = {-12, -12, -24}; //RPG-X : TiM - {-15, -15, -24}
//! players maxs
static vec3_t	playerMaxs = {12, 12, 32}; // {15, 15, 32}

int		actionHeroClientNum = -1;
int		borgQueenClientNum = -1;
clInitStatus_t clientInitialStatus[MAX_CLIENTS];
team_t	borgTeam = TEAM_FREE;

//XPE
char *languageIndex[MAX_LANGUAGES];
//END XPE

//TiM: For easier transport setup
/**
*	Function that makes transport setup easier
*	\author Ubergames - TiM
*/
void G_InitTransport( int clientNum, vec3_t origin, vec3_t angles ) {
	gentity_t	*tent;

	TransDat[clientNum].beamTime = level.time + 8000;
	g_entities[clientNum].client->ps.powerups[PW_BEAM_OUT] = level.time + 8000;

	//Transfer stored data to active beamer
	VectorCopy( origin,
				TransDat[clientNum].currentCoord.origin );
	VectorCopy( angles,
				TransDat[clientNum].currentCoord.angles );

	tent = G_TempEntity( g_entities[clientNum].client->ps.origin, EV_PLAYER_TRANSPORT_OUT );
	tent->s.clientNum = clientNum;
}

/**
*	Get a random player that becomes action hero
*/
void G_RandomActionHero( int ignoreClientNum )
{
	//int /*i,*/ numConnectedClients = 0;
	//int ahCandidates[MAX_CLIENTS];

	if ( g_doWarmup.integer )
	{
		if ( level.warmupTime != 0 )
		{
			if ( level.warmupTime < 0 || level.time - level.startTime <= level.warmupTime )
			{//don't choose one until warmup is done
				return;
			}
		}
	}
	else if ( level.time - level.startTime <= 3000 )
	{//don't choose one until 3 seconds into the game
		return;
	}

	//if ( g_pModActionHero.integer != 0 )
	//{
	//	for ( i = 0; i < level.maxclients; i++ )
	//	{
	//		if ( i == ignoreClientNum )
	//		{
	//			continue;
	//		}

	//		if ( level.clients[i].pers.connected != CON_DISCONNECTED )
	//		{
	//			//note: these next few checks will mean that the first player to join (usually server client if a listen server) when a new map starts is *always* the AH
	//			if ( &g_entities[i] != NULL && g_entities[i].client != NULL )
	//			{
	//				if ( level.clients[i].sess.sessionClass != PC_ACTIONHERO )
	//				{
	//					if ( level.clients[i].sess.sessionTeam != TEAM_SPECTATOR )
	//					{
	//						ahCandidates[numConnectedClients++] = i;
	//					}
	//				}
	//			}
	//		}
	//	}
	//	if ( !numConnectedClients )
	//	{//WTF?!
	//		return;
	//	}
	//	else
	//	{
	//		actionHeroClientNum = ahCandidates[ irandom( 0, (numConnectedClients-1) ) ];
	//	}
	//}
}

/**
*	Check wheter to replace the current action hero
*/
void G_CheckReplaceActionHero( int clientNum )
{
	if ( clientNum == actionHeroClientNum )
	{
		G_RandomActionHero( clientNum );
		if ( actionHeroClientNum >= 0 && actionHeroClientNum < level.maxclients )
		{
			// get and distribute relevent paramters
			ClientUserinfoChanged( actionHeroClientNum );
			ClientSpawn( &g_entities[actionHeroClientNum], 0, qfalse );//RPG-X: RedTechie - Modifyed
		}//else ERROR!!!
	}
}

void INeedAHero( void )
{
	G_RandomActionHero( actionHeroClientNum );
	if ( actionHeroClientNum >= 0 && actionHeroClientNum < level.maxclients )
	{// get and distribute relevent paramters
		ClientUserinfoChanged( actionHeroClientNum );
		ClientSpawn( &g_entities[actionHeroClientNum], 0, qfalse );//RPG-X: RedTechie - Modifyed
	}//else ERROR!!!
}

/**
*	Pick a random borgqueen
*/
void G_RandomBorgQueen( int ignoreClientNum )
{
	int i, borgCount = 0;
	int	borgClients[MAX_CLIENTS];

	//FIXME: make it not always pick the first borg client to connect as the Queen!!!
	//is there a way to wait until all initial clients connect?
	if ( borgQueenClientNum != -1 )
	{
		if ( borgQueenClientNum != ignoreClientNum )
		{//already have a valid one
			return;
		}
	}
	
	//RPG-X: RedTechie - No warmup!
	/*if ( g_doWarmup.integer )
	{
		if ( level.warmupTime != 0 )
		{
			if ( level.warmupTime < 0 || level.time - level.startTime <= level.warmupTime )
			{//don't choose one until warmup is done
				return;
			}
		}
	}*/
	else if ( level.time - level.startTime <= 3000 )
	{//don't choose one until 3 seconds into the game
		return;
	}

	if ( g_pModAssimilation.integer != 0 && ( borgTeam == TEAM_BLUE || borgTeam == TEAM_RED ) )
	{
		for ( i = 0; i < level.maxclients; i++ )
		{
			if ( i == ignoreClientNum )
			{
				continue;
			}
			if ( level.clients[i].sess.sessionTeam == borgTeam )//&& level.clients[i].ps.stats[STAT_HEALTH] > 0
			{
				borgClients[borgCount++] = i;
			}
		}
		/*
		if ( borgCount < 1 )
		{
			if ( ignoreClientNum > 0 && ignoreClientNum < level.maxclients )
			{
				if ( level.clients[ignoreClientNum].sess.sessionTeam == borgTeam )// && level.clients[ignoreClientNum].ps.stats[STAT_HEALTH] > 0
				{
					borgClients[borgCount++] = ignoreClientNum;
				}
			}
		}
		*/
		if ( borgCount > 0 )
		{
			/*
			int oldQueenClientNum = -1;

			if ( borgCount > 1 )
			{//more than 1 borg, don't let it pick the same queen twice in a row
				oldQueenClientNum = borgQueenClientNum;
			}
			while ( borgQueenClientNum == oldQueenClientNum )
			*/
			{
				borgQueenClientNum = borgClients[irandom(0, borgCount-1)];
			}
		}
		else
		{
			borgQueenClientNum = -1;
		}
	}
}

/**
*	Check whether borgqueen should be replaced
*/
void G_CheckReplaceQueen( int clientNum )
{
	if ( clientNum == borgQueenClientNum )
	{
		G_RandomBorgQueen( clientNum );
		if ( borgQueenClientNum >= 0 && borgQueenClientNum < level.maxclients )
		{
			// get and distribute relevent paramters
			ClientUserinfoChanged( borgQueenClientNum );
			ClientSpawn( &g_entities[borgQueenClientNum], 0, qfalse );//RPG-X: RedTechie - Modifyed
		}//else ERROR!!!
	}
}

/**
*	Pick a player as borgqueen
*/
void G_PickBorgQueen( void )
{
	G_RandomBorgQueen( borgQueenClientNum );
	if ( borgQueenClientNum >= 0 && borgQueenClientNum < level.maxclients )
	{// get and distribute relevent paramters
		ClientUserinfoChanged( borgQueenClientNum );
		ClientSpawn( &g_entities[borgQueenClientNum], 0, qfalse );//RPG-X: RedTechie - Modifyed
	}//else ERROR!!!
}

/*QUAKED info_player_deathmatch (1 0 1) (-16 -16 -24) (16 16 32) initial
potential spawning position for deathmatch games.
The first time a player enters the game, they will be at an 'initial' spot.
Targets will be fired when someone spawns in on them.
"nobots" will prevent bots from using this spot.
"nohumans" will prevent non-bots from using this spot.
*/
/**
*	Spawn function for deathmatch spawnpoint
*/
void SP_info_player_deathmatch( gentity_t *ent ) {
	int		i;

	G_SpawnInt( "nobots", "0", &i);
	if ( i ) {
		ent->flags |= FL_NO_BOTS;
	}
	G_SpawnInt( "nohumans", "0", &i );
	if ( i ) {
		ent->flags |= FL_NO_HUMANS;
	}

	trap_LinkEntity(ent);
}

/*QUAKED info_player_start (1 0 0) (-16 -16 -24) (16 16 32)
equivelant to info_player_deathmatch
*/
/**
*	Spawn function for player start spawnpoint which actually the same as deatchmatch spawnpoint
*/
void SP_info_player_start(gentity_t *ent) {
	ent->classname = "info_player_deathmatch";
	SP_info_player_deathmatch( ent );
}

/*QUAKED info_player_intermission (1 0 1) (-16 -16 -24) (16 16 32)
The intermission will be viewed from this point.  Target an info_notnull for the view direction.
*/
/**
*	Spawn function for intermission entity.
*/
void SP_info_player_intermission( gentity_t *ent ) {

}



/*
=======================================================================

  SelectSpawnPoint

=======================================================================
*/

/*
================
SpotWouldTelefrag

================
*/
/**
*	Check if beaming to a point will result in a teleporter frag.
*/
qboolean SpotWouldTelefrag( gentity_t *spot ) {
	int			i, num;
	int			touch[MAX_GENTITIES];
	gentity_t	*hit;
	vec3_t		mins, maxs;

	VectorAdd( spot->s.origin, playerMins, mins );
	VectorAdd( spot->s.origin, playerMaxs, maxs );
	num = trap_EntitiesInBox( mins, maxs, touch, MAX_GENTITIES );

	for (i=0 ; i<num ; i++) {
		hit = &g_entities[touch[i]];
		if ( hit && hit->client && hit->client->ps.stats[STAT_HEALTH] > 0 ) {
			return qtrue;
		}
		if (hit && hit->s.eType == ET_USEABLE && hit->s.modelindex == HI_SHIELD) {	//hit a portable force field
			return qtrue;
		}


	}

	return qfalse;
}

/*
================
SelectNearestDeathmatchSpawnPoint

Find the spot that we DON'T want to use
================
*/
#define	MAX_SPAWN_POINTS	256
/**
*	Find the spot that we DON'T want to use
*/
gentity_t *SelectNearestDeathmatchSpawnPoint( vec3_t from ) {
	gentity_t	*spot;
	vec3_t		delta;
	float		dist, nearestDist;
	gentity_t	*nearestSpot;

	nearestDist = 999999;
	nearestSpot = NULL;
	spot = NULL;

	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {

		VectorSubtract( spot->s.origin, from, delta );
		dist = VectorLength( delta );
		if ( dist < nearestDist ) {
			nearestDist = dist;
			nearestSpot = spot;
		}
	}

	return nearestSpot;
}


/*
================
SelectRandomDeathmatchSpawnPoint

go to a random point that doesn't telefrag
================
*/
#define	MAX_SPAWN_POINTS	256
/**
*	go to a random point that doesn't telefrag
*/
gentity_t *SelectRandomDeathmatchSpawnPoint( void ) {
	gentity_t	*spot;
	int			count;
	int			selection;
	gentity_t	*spots[MAX_SPAWN_POINTS];

	count = 0;
	spot = NULL;

	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
		if ( SpotWouldTelefrag( spot ) ) {
			continue;
		}
		spots[ count ] = spot;
		count++;
	}

	if ( !count ) {	// no spots that won't telefrag
		return G_Find( NULL, FOFS(classname), "info_player_deathmatch");
	}

	selection = rand() % count;
	return spots[ selection ];
}


/*
===========
SelectSpawnPoint

Chooses a player start, deathmatch start, etc
============
*/
/**
*	Chooses a player start, deathmatch start, etc
*/
gentity_t *SelectSpawnPoint ( vec3_t avoidPoint, vec3_t origin, vec3_t angles ) {
	gentity_t	*spot;
	gentity_t	*nearestSpot;

	nearestSpot = SelectNearestDeathmatchSpawnPoint( avoidPoint );

	spot = SelectRandomDeathmatchSpawnPoint ( );
	if ( spot == nearestSpot ) {
		// roll again if it would be real close to point of death
		spot = SelectRandomDeathmatchSpawnPoint ( );
		if ( spot == nearestSpot ) {
			// last try
			spot = SelectRandomDeathmatchSpawnPoint ( );
		}
	}

	// find a single player start spot
	if (!spot) {
		G_Error( "Couldn't find a spawn point" );
		return spot;
	}

	VectorCopy (spot->s.origin, origin);
	origin[2] += 9;
	VectorCopy (spot->s.angles, angles);

	return spot;
}

/*
===========
SelectInitialSpawnPoint

Try to find a spawn point marked 'initial', otherwise
use normal spawn selection.
============
*/
/**
*	Try to find a spawn point marked 'initial', otherwise
*	use normal spawn selection.
*/
gentity_t *SelectInitialSpawnPoint( vec3_t origin, vec3_t angles ) {
	gentity_t	*spot;

	spot = NULL;
	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
		if ( spot->spawnflags & 1 ) {
			break;
		}
	}

	if ( !spot || SpotWouldTelefrag( spot ) ) {
		return SelectSpawnPoint( vec3_origin, origin, angles );
	}

	VectorCopy (spot->s.origin, origin);
	origin[2] += 9;
	VectorCopy (spot->s.angles, angles);

	return spot;
}

/*
===========
SelectSpectatorSpawnPoint

============
*/
gentity_t *SelectSpectatorSpawnPoint( vec3_t origin, vec3_t angles ) {
	FindIntermissionPoint();

	VectorCopy( level.intermission_origin, origin );
	VectorCopy( level.intermission_angle, angles );

	return NULL;
}

/*
=======================================================================

BODYQUE

=======================================================================
*/

static int	bodyFadeSound=0;


/*
===============
InitBodyQue
===============
*/
void InitBodyQue (void) {
	int		i;
	gentity_t	*ent;

	level.bodyQueIndex = 0;
	for (i=0; i<BODY_QUEUE_SIZE ; i++) {
		ent = G_Spawn();
		ent->classname = "bodyque";
		ent->neverFree = qtrue;
		level.bodyQue[i] = ent;
	}

	if (bodyFadeSound == 0)
	{	// Initialize this sound.
		bodyFadeSound = G_SoundIndex("sound/enemies/borg/walkthroughfield.wav");
	}
}

/*
=============
BodyRezOut

After sitting around for five seconds, fade out.
=============
*/
/**
*	After sitting around for five seconds, fade out.
*/
void BodyRezOut( gentity_t *ent )
{
	if ( level.time - ent->timestamp >= 7500 ) {
		// the body ques are never actually freed, they are just unlinked
		trap_UnlinkEntity( ent );
		ent->physicsObject = qfalse;
		return;
	}

	ent->nextthink = level.time + 2500;
	ent->s.time = level.time + 2500;

	G_AddEvent(ent, EV_GENERAL_SOUND, bodyFadeSound);
}

/*
=============
CopyToBodyQue

A player is respawning, so make an entity that looks
just like the existing corpse to leave behind.
=============
*/
/**
*	A player is respawning, so make an entity that looks
*	just like the existing corpse to leave behind.
*/
void CopyToBodyQue( gentity_t *ent ) {
	gentity_t		*body;
	int			contents;
	entityState_t *eState;

	trap_UnlinkEntity (ent);

	// if client is in a nodrop area, don't leave the body
	contents = trap_PointContents( ent->s.origin, -1 );
	if ( contents & CONTENTS_NODROP ) {
		ent->s.eFlags &= ~EF_NODRAW;	// Just in case we died from a bottomless pit, reset EF_NODRAW
		return;
	}

	// grab a body que and cycle to the next one
	body = level.bodyQue[ level.bodyQueIndex ];
	level.bodyQueIndex = (level.bodyQueIndex + 1) % BODY_QUEUE_SIZE;

	trap_UnlinkEntity (body);

	eState = &ent->s;
	eState->eFlags = EF_DEAD;		// clear EF_TALK, etc
	eState->powerups = 0;	// clear powerups
	eState->loopSound = 0;	// clear lava burning
	eState->number = body - g_entities;
	body->timestamp = level.time;
	body->physicsObject = qtrue;
	body->physicsBounce = 0;		// don't bounce
	if ( eState->groundEntityNum == ENTITYNUM_NONE ) {
		eState->pos.trType = TR_GRAVITY;
		eState->pos.trTime = level.time;
		VectorCopy( ent->client->ps.velocity, eState->pos.trDelta );
	} else {
		eState->pos.trType = TR_STATIONARY;
	}
	eState->event = 0;

	// change the animation to the last-frame only, so the sequence
	// doesn't repeat anew for the body
	switch ( eState->legsAnim & ~ANIM_TOGGLEBIT ) {
	case BOTH_DEATH1:
	case BOTH_DEAD1:
		eState->torsoAnim = eState->legsAnim = BOTH_DEAD1;
		break;
	case BOTH_DEATH2:
	case BOTH_DEAD2:
		eState->torsoAnim = eState->legsAnim = BOTH_DEAD2;
		break;
	/*case BOTH_DEATH3:
	case BOTH_DEAD3:*/
	default:
		eState->torsoAnim = eState->legsAnim = BOTH_DEAD1; //DEAD3
		break;
	}

	body->r.svFlags = ent->r.svFlags;
	VectorCopy (ent->r.mins, body->r.mins);
	VectorCopy (ent->r.maxs, body->r.maxs);
	VectorCopy (ent->r.absmin, body->r.absmin);
	VectorCopy (ent->r.absmax, body->r.absmax);

	body->clipmask = CONTENTS_SOLID | CONTENTS_PLAYERCLIP;
	body->r.contents = CONTENTS_CORPSE;
	body->r.ownerNum = ent->r.ownerNum;

	body->nextthink = level.time + 5000;
	body->think = BodyRezOut;

	body->die = body_die;

	// if there shouldn't be a body, don't show one.
	if (ent->client &&
			((level.time - ent->client->ps.powerups[PW_DISINTEGRATE]) < 10000 ||
			(level.time - ent->client->ps.powerups[PW_EXPLODE]) < 10000))
	{
		eState->eFlags |= EF_NODRAW;
	}

	// don't take more damage if already gibbed
	//RPG-X: RedTechie - Check for medicrevive
	if(rpg_medicsrevive.integer == 0){
		if ( ent->health <= GIB_HEALTH ) {
			body->takedamage = qfalse;
		} else {
			body->takedamage = qtrue;
		}
	}else{
		body->takedamage = qfalse;
	}

	VectorCopy ( eState->pos.trBase, body->r.currentOrigin );
	trap_LinkEntity (body);
}

//======================================================================


/*
==================
SetClientViewAngle

==================
*/
void SetClientViewAngle( gentity_t *ent, vec3_t angle ) {
	int			i;

	// set the delta angle
	for (i=0 ; i<3 ; i++) {
		int		cmdAngle;

		cmdAngle = ANGLE2SHORT(angle[i]);
		ent->client->ps.delta_angles[i] = cmdAngle - ent->client->pers.cmd.angles[i];
	}
	VectorCopy( angle, ent->s.angles );
	VectorCopy (ent->s.angles, ent->client->ps.viewangles);
}

void SetScore( gentity_t *ent, int score );
void EliminationRespawn( gentity_t *ent, char *team )
{
	ent->flags &= ~FL_NOTARGET;
	ent->s.eFlags &= ~EF_NODRAW;
	ent->client->ps.eFlags &= ~EF_NODRAW;
	//ent->s.eFlags &= ~EF_ELIMINATED;
	//ent->client->ps.eFlags &= ~EF_ELIMINATED;
	ent->r.svFlags &= ~SVF_ELIMINATED;
	ClientSpawn(ent, 0, qfalse);//RPG-X: RedTechie - Modifyed
	/*
	int oldScore;
	oldScore = ent->client->ps.persistant[PERS_SCORE];
	SetTeam( ent, ent->team );
	SetScore( ent, oldScore );
	*/
}

void EliminationSpectate( gentity_t *ent )
{
	playerState_t *ps = &ent->client->ps;

	CopyToBodyQue (ent);

	ClientSpawn(ent, 0, qfalse);//RPG-X: RedTechie - Modifyed
	ent->takedamage = qfalse;
	ent->r.contents = 0;
	ent->flags |= FL_NOTARGET;
	ent->s.eFlags |= EF_NODRAW;
	ps->eFlags |= EF_NODRAW;
	ps->pm_type = PM_NORMAL;//PM_SPECTATOR;
	//ent->s.eFlags |= EF_ELIMINATED;//FIXME:  this is not being reliably SENT!!!!!!
	//ps->eFlags |= EF_ELIMINATED;
	ent->r.svFlags |= SVF_ELIMINATED;//just in case
	VectorSet( ent->r.mins, -4, -4, -16 );
	VectorSet( ent->r.maxs, 4, 4, -8 );
	ps->weapon = 0;
	ps->stats[STAT_WEAPONS] = 0;
	/*
	int oldScore;
	oldScore = ent->client->ps.persistant[PERS_SCORE];
	ent->team = (char *)TeamName( ent->client->sess.sessionTeam );
	SetTeam( ent, "spectator");
	SetScore( ent, oldScore );
	//FIXME: specator mode when dead kind of freaky if trying to follow
	*/
}
/*
================
respawn
================
*/
extern char *ClassNameForValue( pclass_t pClass );
void respawn( gentity_t *ent ) {
	qboolean	borg = qfalse;
	gentity_t	*tent;
	playerState_t *ps;

	if ( ent->s.number == borgQueenClientNum )
	{//can't respawn if queen
		return;
	}
	/*if ( g_pModElimination.integer != 0 )
	{//no players respawn when in elimination
		if ( !(level.intermissiontime && level.intermissiontime != -1) )
		{//don't do this once intermission has begun
			EliminationSpectate( ent );
		}
		return;
	}*/

	/*if ( g_pModAssimilation.integer != 0 && ent->client )
	{//Go to Borg team if killed by assimilation
		if ( ent->client->sess.sessionClass != PC_BORG )
		{
			if ( ent->client->mod == MOD_ASSIMILATE )
			{
				//first, save their current state
				clientInitialStatus[ent->s.number].initialized = qfalse;
				G_StoreClientInitialStatus( ent );
				if ( ent->client->sess.sessionTeam == TEAM_RED )
				{
					SetClass( ent, "borg", "blue" );
				}
				else if ( ent->client->sess.sessionTeam == TEAM_BLUE )
				{
					SetClass( ent, "borg", "red" );
				}
				ent->s.eFlags |= EF_ASSIMILATED;
				ent->client->ps.eFlags |= EF_ASSIMILATED;
				return;
			}
		}
		else
		{
			// flag this so we can play a different spawn in effect for a borg
			borg = qtrue;
		}
	}
	else
	{*///assimilated players don't leave corpses
		CopyToBodyQue (ent);
	//}

	ClientSpawn(ent, 0, qfalse);//RPG-X: RedTechie - Modifyed

	ps = &ent->client->ps;

	// add a teleportation effect
	if ( borg )
		tent = G_TempEntity( ps->origin, EV_BORG_TELEPORT );
	else
	{
		//tent = G_TempEntity( ps->origin, EV_PLAYER_TELEPORT_IN );
		tent = G_TempEntity( ps->origin, EV_PLAYER_TRANSPORT_IN );
		ps->powerups[PW_QUAD] = level.time + 4000;
	}

	tent->s.clientNum = ent->s.clientNum;
}

/*
================
TeamCount

Returns number of players on a team
================
*/
/**
*	Returns number of players on a team
*/
team_t TeamCount( int ignoreClientNum, int team ) {
	int		i;
	int		count = 0;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( i == ignoreClientNum ) {
			continue;
		}
		if ( level.clients[i].pers.connected == CON_DISCONNECTED ) {
			continue;
		}
		if ( level.clients[i].sess.sessionTeam == team ) {
			count++;
		}
	}

	return count;
}


/*
================
PickTeam

================
*/
team_t PickTeam( int ignoreClientNum ) {
	int		counts[TEAM_NUM_TEAMS];

	counts[TEAM_BLUE] = TeamCount( ignoreClientNum, TEAM_BLUE );
	counts[TEAM_RED] = TeamCount( ignoreClientNum, TEAM_RED );

	if ( counts[TEAM_BLUE] > counts[TEAM_RED] ) {
		return TEAM_RED;
	}
	if ( counts[TEAM_RED] > counts[TEAM_BLUE] ) {
		return TEAM_BLUE;
	}
	// equal team count, so join the team with the lowest score
	if ( level.teamScores[TEAM_BLUE] > level.teamScores[TEAM_RED] ) {
		return TEAM_RED;
	}
	if ( level.teamScores[TEAM_BLUE] < level.teamScores[TEAM_RED] ) {
		return TEAM_BLUE;
	}
	return irandom( TEAM_RED, TEAM_BLUE );
}

/*
===========
ForceClientSkin

Forces a client's skin (for teamplay)
HEAVILY modified for the RPG-X
Player Model system :P
===========
*/
/**
*	Forces a client's skin (for teamplay)
*	HEAVILY modified for the RPG-X
*	Player Model system
*/
void ForceClientSkin(char *model, const char *skin ) {
	char *p;
	char *q;

	//we expect model to equal 'char/model/skin'

	//check for the slash between charName and modelName
	//if ((p = strchr(model, '/')) != NULL) {
	//	*p = 0;
	//}

	//Com_Printf("ForceSkin Called\n");

	p = strchr(model, '/');

	//if no slashes at all
	if ( !p || !p[0] || !p[1] ) {
		//input everything
		Q_strcat(model, MAX_QPATH, "/");
		Q_strcat(model, MAX_QPATH, "main");
		Q_strcat(model, MAX_QPATH, "/");
		Q_strcat(model, MAX_QPATH, skin);
	}
	else { //ie we got a slash (which should be the first of two
		p++;
		q = strchr(p, '/'); //okay, we should get another one if one was already found
		if (!q || !q[0] || !q[1] ) 
		{ //no slashes were found?? >.<
			//okay, let's assume they specified the .model file, no skin
			//so just add the skin to the end :P
			Q_strcat(model, MAX_QPATH, "/");
			Q_strcat(model, MAX_QPATH, skin);
		}
		else 
		{
			//len = strlen( p );
			//Q_strcat(&model[len - strlen( q )], MAX_QPATH, skin); 
			q++;
			*q= '\0';
			Q_strcat(model, MAX_QPATH, skin);
			
			//Com_Printf("Debug: %s\n", model);
		}
	}
}

/*
===========
ClientCheckName
============
*/
/*static*/ void ClientCleanName( const char *in, char *out, int outSize ) {
	int		len, colorlessLen;
	char	ch;
	char	*p;
	int		spaces;

	//save room for trailing null byte
	outSize--;

	len = 0;
	colorlessLen = 0;
	p = out;
	*p = 0;
	spaces = 0;

	while( 1 ) {
		ch = *in++;
		if( !ch ) {
			break;
		}

		// don't allow leading spaces
		if( !*p && ch == ' ' ) {
			continue;
		}

		// check colors
		if( ch == Q_COLOR_ESCAPE ) {
			// solo trailing carat is not a color prefix
			if( !*in ) {
				break;
			}

			// don't allow black in a name, period
			if( ColorIndex(*in) == 0 ) {
				in++;
				continue;
			}

			// make sure room in dest for both chars
			if( len > outSize - 2 ) {
				break;
			}

			*out++ = ch;
			*out++ = *in++;
			len += 2;
			continue;
		}

		// don't allow too many consecutive spaces
		if( ch == ' ' ) {
			spaces++;
			if( spaces > 3 ) {
				continue;
			}
		}
		else {
			spaces = 0;
		}

		if( len > outSize - 1 ) {
			break;
		}

		*out++ = ch;
		colorlessLen++;
		len++;
	}
	*out = 0;

	// don't allow empty names
	if( *p == 0 || colorlessLen == 0 )
	{
		Q_strncpyz( p, "RedShirt", outSize );
	}
}

/*
===========
legalSkin

Compare a list of races with an incoming race name.
Used to decide if in a CTF game where a race is specified for a given team if a skin is actually already legal.
===========
*/
/**
*	Compare a list of races with an incoming race name.
*	Used to decide if in a CTF game where a race is specified for a given team if a skin is actually already legal.
*/
qboolean legalSkin(const char *race_list, const char *race)
{
	char current_race_name[125];
	const char *s = race_list;
	const char *max_place = race_list + strlen(race_list);
	const char *marker;

	memset(current_race_name, 0, sizeof(current_race_name));
	// look through the list till it's empty
	while (s < max_place)
	{
		marker = s;
		// figure out from where we are where the next ',' or 0 is
		while (*s != ',' && *s != 0)
		{
			s++;
		}

		// copy just that name
		Q_strncpyz(current_race_name, marker, (s-marker)+1);

		// avoid the comma or increment us past the end of the string so we fail the main while loop
		s++;

		// compare and see if this race is the same as the one we want
		if (!Q_stricmp(current_race_name, race))
		{
			return qtrue;
		}
	}
	return qfalse;
}


/*
===========
randomSkin

given a race name, go find all the skins that use it, and randomly select one
===========
*/
/**
*	given a race name, go find all the skins that use it, and randomly select one
*/
#ifdef Q3_VM
void randomSkin(const char* race, char* model, int current_team, int clientNum)
{
	char	skinsForRace[MAX_SKINS_FOR_RACE][128];
	int		howManySkins = 0;
	int		i,x;
	int		temp;
	int		skin_count_check;
	char	skinNamesAlreadyUsed[16][128];
	int		current_skin_count = 0;
	gentity_t	*ent = NULL;
	char	userinfo[MAX_INFO_STRING];
	char	temp_model[MAX_QPATH];

	memset(skinsForRace, 0, sizeof(skinsForRace));
	memset(skinNamesAlreadyUsed, 0, sizeof(skinNamesAlreadyUsed));

	// first up, check to see if we want to select a skin from someone that's already playing on this guys team
	skin_count_check = g_random_skin_limit.integer;
	if (skin_count_check)
	{
		// sanity check the skins to compare against count
		if (skin_count_check > 16)
		{
			skin_count_check = 16;
		}

		// now construct an array of the names already used
		for (i=0; i<g_maxclients.integer; i++)
		{
			// did we find enough skins to grab a random one from yet?
			if (current_skin_count == skin_count_check)
			{
				break;
			}

			ent = g_entities + i;
			if (!ent->inuse || i == clientNum)
				continue;

			// no, so look at the next one, and see if it's in the list we are constructing
			// same team?
			if 	(ent->client && ent->client->sess.sessionTeam == current_team)
			{
				// so what's this clients model then?
				trap_GetUserinfo( i, userinfo, sizeof( userinfo ) );
				Q_strncpyz( temp_model, Info_ValueForKey (userinfo, "model"), sizeof( temp_model ) );

				// check the name
				for (x = 0; x< current_skin_count; x++)
				{
					// are we the same?
					if (!Q_stricmp(skinNamesAlreadyUsed[x], temp_model))
					{
						// yeah - ok we already got this one
						break;
					}
				}

				// ok, did we match anything?
				if (x == current_skin_count)
				{
					// no - better add this name in
					Q_strncpyz(skinNamesAlreadyUsed[current_skin_count], temp_model, sizeof(skinNamesAlreadyUsed[current_skin_count]));
					current_skin_count++;
				}
			}
		}

		// ok, array constructed. Did we get enough?
		if (current_skin_count >= skin_count_check)
		{
			// yeah, we did - so select a skin from one of these then
			temp = rand() % current_skin_count;
			Q_strncpyz( model, skinNamesAlreadyUsed[temp], MAX_QPATH );
			ForceClientSkin(model, "");
			return;
		}
	}

	// search through each and every skin we can find
	for (i=0; i<group_count && howManySkins < MAX_SKINS_FOR_RACE; i++)
	{

		// if this models race list contains the race we want, then add it to the list
		if (legalSkin(group_list[i].text, race))
		{
			Q_strncpyz( skinsForRace[howManySkins++], group_list[i].name , 128 );
		}
	}

	// set model to a random one
	if (howManySkins)
	{
		temp = rand() % howManySkins;
		Q_strncpyz( model, skinsForRace[temp], MAX_QPATH );
	}
	else
	{
		model[0] = 0;
	}

}
#else
void randomSkin(const char* race, char* model, int current_team, int clientNum)
{
	char	**skinsForRace;
	int		howManySkins = 0;
	int		i,x;
	int		temp;
	int		skin_count_check;
	char	**skinNamesAlreadyUsed;
	int		current_skin_count = 0;
	gentity_t	*ent = NULL;
	char	*userinfo;
	char	temp_model[MAX_QPATH];

	skinsForRace = (char **)malloc(MAX_SKINS_FOR_RACE * 128 * sizeof(char));
	if(!skinsForRace) {
		G_Error("Was unable to allocate %i bytes.\n", MAX_SKINS_FOR_RACE * 128 * sizeof(char));
		return;
	}
	skinNamesAlreadyUsed = (char **)malloc(16 * 128 * sizeof(char));
	if(!skinNamesAlreadyUsed) {
		G_Error("Was unable to allocate %i bytes.\n", 16 * 128 * sizeof(char));
		return;
	}

	memset(skinsForRace, 0, MAX_SKINS_FOR_RACE * 128 * sizeof(char));
	memset(skinNamesAlreadyUsed, 0, 16 * 128 * sizeof(char));

	// first up, check to see if we want to select a skin from someone that's already playing on this guys team
	skin_count_check = g_random_skin_limit.integer;
	if (skin_count_check)
	{
		// sanity check the skins to compare against count
		if (skin_count_check > 16)
		{
			skin_count_check = 16;
		}

		// now construct an array of the names already used
		for (i=0; i<g_maxclients.integer; i++)
		{
			// did we find enough skins to grab a random one from yet?
			if (current_skin_count == skin_count_check)
			{
				break;
			}

			ent = g_entities + i;
			if (!ent->inuse || i == clientNum)
				continue;

			// no, so look at the next one, and see if it's in the list we are constructing
			// same team?
			if 	(ent->client && ent->client->sess.sessionTeam == current_team)
			{
				userinfo = (char *)malloc(MAX_INFO_STRING * sizeof(char));
				if(!userinfo) {
					G_Error("Was unable to allocate %i bytes.\n", MAX_INFO_STRING * sizeof(char));
					return;
				}
				// so what's this clients model then?
				trap_GetUserinfo( i, userinfo, MAX_INFO_STRING * sizeof(char) );
				Q_strncpyz( temp_model, Info_ValueForKey (userinfo, "model"), sizeof( temp_model ) );

				free(userinfo);

				// check the name
				for (x = 0; x< current_skin_count; x++)
				{
					// are we the same?
					if (!Q_stricmp(skinNamesAlreadyUsed[x], temp_model))
					{
						// yeah - ok we already got this one
						break;
					}
				}

				// ok, did we match anything?
				if (x == current_skin_count)
				{
					// no - better add this name in
					Q_strncpyz(skinNamesAlreadyUsed[current_skin_count], temp_model, sizeof(skinNamesAlreadyUsed[current_skin_count]));
					current_skin_count++;
				}
			}
		}

		// ok, array constructed. Did we get enough?
		if (current_skin_count >= skin_count_check)
		{
			// yeah, we did - so select a skin from one of these then
			temp = rand() % current_skin_count;
			Q_strncpyz( model, skinNamesAlreadyUsed[temp], MAX_QPATH );
			ForceClientSkin(model, "");
			free(skinNamesAlreadyUsed);
			free(skinsForRace);
			return;
		}
	}

	// search through each and every skin we can find
	for (i=0; i<group_count && howManySkins < MAX_SKINS_FOR_RACE; i++)
	{

		// if this models race list contains the race we want, then add it to the list
		if (legalSkin(group_list[i].text, race))
		{
			Q_strncpyz( skinsForRace[howManySkins++], group_list[i].name , 128 );
		}
	}

	// set model to a random one
	if (howManySkins)
	{
		temp = rand() % howManySkins;
		Q_strncpyz( model, skinsForRace[temp], MAX_QPATH );
	}
	else
	{
		model[0] = 0;
	}

	free(skinsForRace);
	free(skinNamesAlreadyUsed);

}
#endif

/*
===========
getNewSkin

Go away and actually get a random new skin based on a group name
============
*/
/**
*	Go away and actually get a random new skin based on a group name	
*/
qboolean getNewSkin(const char *group, char *model, const char *color, const gclient_t *client, int clientNum)
{
	char	*temp_string;

  	// go away and get what ever races this skin is attached to.
  	// remove blue or red name
  	ForceClientSkin(model, "");

  	temp_string = G_searchGroupList(model);

  	// are any of the races legal for this team race?
  	if (legalSkin(temp_string, group))
  	{
  		ForceClientSkin(model, color);
  		return qfalse;
  	}

  	//if we got this far, then we need to reset the skin to something appropriate
  	randomSkin(group, model, client->sess.sessionTeam, clientNum);
	return qtrue;
}

//void ClientMaxHealthForClass ( gclient_t *client, pclass_t pclass )
//{
//	switch( pclass )
//	{
//	case PC_INFILTRATOR:
//		client->pers.maxHealth = 50;
//		break;
//	case PC_HEAVY:
//		client->pers.maxHealth = 200;
//		break;
//	case PC_ACTIONHERO:
//		client->pers.maxHealth = 300;
//		break;
//	case PC_MEDIC:
//		client->pers.maxHealth = 100;
//		break;
//	case PC_SNIPER:
//		client->pers.maxHealth = 100;
//		break;
//	case PC_DEMO:
//		client->pers.maxHealth = 100;
//		break;
//	case PC_TECH:
//		client->pers.maxHealth = 100;
//		break;
//	case PC_BORG:
//		client->pers.maxHealth = 100;
//		break;
//	case PC_N00B:
//		client->pers.maxHealth = 2;
//		break;
//	default:
//		break;
//	}
//}

void SetCSTeam( team_t team, char *teamname )
{
	if ( teamname == NULL || teamname[0] == 0 )
	{
		return;
	}
	switch ( team )
	{
	case TEAM_BLUE:
		trap_SetConfigstring( CS_BLUE_GROUP, teamname );
		break;
	case TEAM_RED:
		trap_SetConfigstring( CS_RED_GROUP, teamname );
		break;
	default: // make gcc happy
		break;
	}
}
/*
===========
ClientUserInfoChanged
============
*/
/**
*	Called from ClientConnect when the player first connects and
*	directly by the server system when the player updates a userinfo variable.
*
*	The game can override any of the settings and call trap_SetUserinfo
*	if desired.
*/
void ClientUserinfoChanged( int clientNum ) {
	gentity_t *ent;
	int		i;
	char	*s; //, *oldModel;
	char	model[MAX_QPATH];
	char	oldname[MAX_STRING_CHARS];
	gclient_t	*client;
	char	*c1;
	char	userinfo[MAX_INFO_STRING];
	qboolean	reset;
	//char	*sex;
//	int		pickborg = 0;
	char	*borgytype;
	float	weight, height;
	char	age[MAX_NAME_LENGTH];
	char	race[MAX_NAME_LENGTH];
	int	modelOffset;
	qboolean	changeName = qtrue; //TiM : For the name filter
	char	sHeight[10];
	char	sWeight[10];
	//int		silentCloak;
	//int		y; //XPERIMENTAL
	clientPersistant_t *pers;
	clientSession_t *sess;

	borgytype = "borg";

	model[0] = 0;

	ent = g_entities + clientNum;
	if(!ent) return;
	client = ent->client;
	pers = &client->pers;
	sess = &client->sess;

	//TiM - Exit if this user has had their info clamped
	if ( ent->flags & FL_CLAMPED )
		return;

	trap_GetUserinfo( clientNum, userinfo, sizeof( userinfo ) );

	//Com_Printf( S_COLOR_RED "%s\n", userinfo );
	//Com_Printf( S_COLOR_RED "CHANGED!\n" );

	// check for malformed or illegal info strings
	if ( !Info_Validate(userinfo) ) {
		strcpy (userinfo, "\\name\\badinfo");
	}

	// check for local client
	s = Info_ValueForKey( userinfo, "ip" );
	if ( !strcmp( s, "localhost" ) ) {
		pers->localClient = qtrue;
	}

	// check the item prediction
	s = Info_ValueForKey( userinfo, "cg_predictItems" );
	if ( !atoi( s ) ) {
		pers->predictItemPickup = qfalse;
	} else {
		pers->predictItemPickup = qtrue;
	}

	// set name
	//TiM: Filter for if a player is already on this server with that name.
	s = Info_ValueForKey (userinfo, "name");
	
	if ( rpg_uniqueNames.integer && !( ent->r.svFlags & SVF_BOT ) ) {
		char newName[36];
		char activeName[36];
		ClientCleanName( s, newName, sizeof(newName) );
		Q_CleanStr( newName );

		//loop thru all the clients, and see if we have one that has the same name as our proposed one
		for ( i = 0; i < level.numConnectedClients; i++ ) {
			Q_strncpyz( activeName, g_entities[i].client->pers.netname, sizeof( activeName ) );
			Q_CleanStr( activeName );

			if ( g_entities[i].client->ps.clientNum != client->ps.clientNum
				&& !Q_stricmp( newName, activeName ) )
			{
				trap_SendServerCommand( ent-g_entities, " print \"Unable to change name. A player already has that name on this server.\n\" ");
				changeName = qfalse;
				break;
			}
		}
	}

	if ( changeName ) {
		Q_strncpyz ( oldname, pers->netname, sizeof( oldname ) );
		ClientCleanName( s, pers->netname, sizeof(pers->netname) );

		if ( sess->sessionTeam == TEAM_SPECTATOR ) {
			if ( sess->spectatorState == SPECTATOR_SCOREBOARD ) {
				Q_strncpyz( pers->netname, "scoreboard", sizeof(pers->netname) );
			}
		}

		if ( pers->connected == CON_CONNECTED ) {
			if ( strcmp( oldname, pers->netname ) ) {
				if ( !levelExiting && !level.intermissiontime )
				{//no need to do this during level changes
					trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " renamed to %s\n\"", oldname, pers->netname) );
				}
			}
		}
	}

	pers->pms_height = atof( Info_ValueForKey( userinfo, "height" ) );
	if ( !pers->pms_height )
		pers->pms_height = 1.0f;

	//In assimilation, make sure everyone is the right class/team for their chosen class/team
	//FIXME: don't want to make these g_team_group_???'s stay forever
	//if ( g_pModAssimilation.integer != 0 )
	//{//become a Borg Queen if needed
	//	if ( sess->sessionClass == PC_BORG )
	//	{//trying to join as a Borg
	//		if ( borgTeam != TEAM_BLUE && borgTeam != TEAM_RED )
	//		{//borg team not chosen yet, so choose it
	//			borgTeam = sess->sessionTeam;
	//			if ( borgTeam != TEAM_BLUE && borgTeam != TEAM_RED )
	//			{
	//				if ( irandom(0, 1) )
	//				{
	//					borgTeam = TEAM_RED;
	//				}
	//				else
	//				{
	//					borgTeam = TEAM_BLUE;
	//				}
	//				SetCSTeam( borgTeam, "Borg" );
	//			}
	//		}

	//		if ( sess->sessionTeam != borgTeam )
	//		{//can't join this team if you're a borg, force them to be non-borg
	//			//if ( g_pModSpecialties.integer != 0 )
	//			//{
	//			//	SetClass( ent, (char *)ClassNameForValue( irandom( PC_SNIPER, PC_TECH ) ), NULL );
	//			//}
	//			//else
	//			//{
	//				SetClass( ent, "noclass", NULL, qtrue );
	//			//}
	//			return;
	//		}
	//	}
	//	else if ( borgTeam != TEAM_BLUE && borgTeam != TEAM_RED )
	//	{//borg team not chosen yet, choose one now
	//		if ( Q_strncmp( "Borg", g_team_group_red.string, 4 ) != 0 )
	//		{//the red team isn't borg
	//			if ( Q_strncmp( "Borg", g_team_group_blue.string, 4 ) != 0 )
	//			{//the blue team isn't borg
	//				if ( TeamCount( clientNum, TEAM_BLUE ) > 0 )
	//				{//blue has people on it, so red is borg
	//					borgTeam = TEAM_RED;
	//				}
	//				else if ( TeamCount( clientNum, TEAM_RED ) > 0 )
	//				{//red has people on it, so blue is borg
	//					borgTeam = TEAM_BLUE;
	//				}
	//				else
	//				{//no-one on a team yet, so borgTeam is the other team
	//					switch( sess->sessionTeam )
	//					{
	//					case TEAM_BLUE:
	//						borgTeam = TEAM_RED;
	//						break;
	//					case TEAM_RED:
	//						borgTeam = TEAM_BLUE;
	//						break;
	//					default:
	//						break;
	//					}
	//				}
	//				SetCSTeam( borgTeam, "Borg" );
	//			}
	//			else if ( borgTeam != TEAM_BLUE && borgTeam != TEAM_RED )
	//			{
	//				borgTeam = TEAM_BLUE;
	//				SetCSTeam( borgTeam, "Borg" );
	//			}
	//		}
	//		else if ( borgTeam != TEAM_BLUE && borgTeam != TEAM_RED )
	//		{
	//			borgTeam = TEAM_RED;
	//			SetCSTeam( borgTeam, "Borg" );
	//		}
	//	}
	//	/*else if ( sess->sessionTeam == borgTeam )
	//	{//borg team is chosen and you are not a borg and you are trying to join borg team
	//		SetClass( ent, "borg", NULL );
	//		return;
	//	}*/

	//	//G_RandomBorgQueen( -1 );
	//}
	// set max health
	pers->maxHealth = atoi( Info_ValueForKey( userinfo, "handicap" ) );
	if ( pers->maxHealth < 1 || pers->maxHealth > 100 ) {
		pers->maxHealth = 100;
	}
	//if you have a class, ignores handicap and 100 limit, sorry
	//ClientMaxHealthForClass( client, sess->sessionClass );
	client->ps.stats[STAT_MAX_HEALTH] = pers->maxHealth;

	// set model
	//switch ( sess->sessionClass )
	//{
	////FIXME: somehow map these into some text file that points to a specific model for each class?
	////OR give them a choice in the menu somehow?
	//case PC_INFILTRATOR:
	//case PC_SNIPER:
	//case PC_HEAVY:
	//case PC_DEMO:
	//case PC_MEDIC://note: can also give health & armor & regen
	//case PC_TECH://note: can also give ammo & invis
	//case PC_BORG:
	//case PC_ACTIONHERO:
	//case PC_NOCLASS:
	//default:
		Q_strncpyz( model, Info_ValueForKey (userinfo, "model"), sizeof( model ) );
		//break;
	//}

	// team
	if ( qtrue/*sess->sessionClass != PC_BORG*/ )
	{//borg class doesn't need to use team color
		switch( sess->sessionTeam ) {
		case TEAM_RED:
			// decide if we are going to have to reset a skin cos it's not applicable to a race selected
			if (g_gametype.integer < GT_TEAM || !Q_stricmp("", g_team_group_red.string))
			{
				if ( g_pModAssimilation.integer != 0 && legalSkin(G_searchGroupList( model ), borgytype ) == qtrue )
				{//if you're trying to be a Borg and not a borg playerclass, then pick a different model
					getNewSkin("HazardTeam", model, "red", client, clientNum);
					ForceClientSkin(model, "red");
					// change the value in out local copy, then update it on the server
					Info_SetValueForKey(userinfo, "model", model);
					trap_SetUserinfo(clientNum, userinfo);
				}
				else
				{
					ForceClientSkin(model, "red");
				}
				break;
			}
			// at this point, we are playing CTF and there IS a race specified for this game
			else
			{
				if ( g_pModAssimilation.integer != 0 && Q_stricmp( borgytype, g_team_group_red.string ) == 0 )
				{//team model is set to borg, but that is now allowed, pick a different "race"
					reset = getNewSkin("HazardTeam", model, "red", client, clientNum);
				}
				else
				{// go away and get what ever races this skin is attached to.
					reset = getNewSkin(g_team_group_red.string, model, "red", client, clientNum);
				}

				// did we get a model name back?
				if (!model[0])
				{
					// no - this almost certainly means we had a bogus race is the g_team_group_team cvar
					// so reset it to starfleet and try it again
					Com_Printf("WARNING! - Red Group %s is unknown - resetting Red Group to Allow Any Group\n", g_team_group_red.string);
					trap_Cvar_Set("g_team_group_red", "");
					trap_Cvar_Register( &g_team_group_red, "g_team_group_red",
						"", CVAR_LATCH);

					// Since we are allow any group now, just get his normal model and carry on
					Q_strncpyz( model, Info_ValueForKey (userinfo, "model"), sizeof( model ) );
					ForceClientSkin(model, "red");
					reset = qfalse;
				}

				if (reset)
				{
					if ( !levelExiting )
					{//no need to do this during level changes
						trap_SendServerCommand( -1, va("print \"In-appropriate skin selected for %s on team %s\nSkin selection overridden from skin %s to skin %s\n\"",
							pers->netname, g_team_group_red.string, Info_ValueForKey (userinfo, "model"), model));
					}
					ForceClientSkin(model, "red");
					// change the value in out local copy, then update it on the server
					Info_SetValueForKey(userinfo, "model", model);
					trap_SetUserinfo(clientNum, userinfo);
				}
				break;
			}
		case TEAM_BLUE:
			// decide if we are going to have to reset a skin cos it's not applicable to a race selected
			if (g_gametype.integer < GT_TEAM || !Q_stricmp("", g_team_group_blue.string))
			{
				if ( g_pModAssimilation.integer != 0 && legalSkin(G_searchGroupList( model ), borgytype ) == qtrue )
				{//if you're trying to be a Borg and not a borg playerclass, then pick a different model
					getNewSkin("HazardTeam", model, "blue", client, clientNum);
					ForceClientSkin(model, "blue");
					// change the value in out local copy, then update it on the server
					Info_SetValueForKey(userinfo, "model", model);
					trap_SetUserinfo(clientNum, userinfo);
				}
				else
				{
					ForceClientSkin(model, "blue");
				}
				break;
			}
			// at this point, we are playing CTF and there IS a race specified for this game
			else
			{
				if ( g_pModAssimilation.integer != 0 && Q_stricmp( borgytype, g_team_group_blue.string ) == 0 )
				{//team model is set to borg, but that is now allowed, pick a different "race"
					reset = getNewSkin("HazardTeam", model, "blue", client, clientNum);
				}
				else
				{
					// go away and get what ever races this skin is attached to.
					reset = getNewSkin(g_team_group_blue.string, model, "blue", client, clientNum);
				}

				// did we get a model name back?
				if (!model[0])
				{
					// no - this almost certainly means we had a bogus race is the g_team_group_team cvar
					// so reset it to klingon and try it again
					Com_Printf("WARNING! - Blue Group %s is unknown - resetting Blue Group to Allow Any Group\n", g_team_group_blue.string);
					trap_Cvar_Set("g_team_group_blue", "");
					trap_Cvar_Register( &g_team_group_blue, "g_team_group_blue",
						"", CVAR_LATCH );

					// Since we are allow any group now, just get his normal model and carry on
					Q_strncpyz( model, Info_ValueForKey (userinfo, "model"), sizeof( model ) );
					ForceClientSkin(model, "blue");
					reset = qfalse;
				}

				if (reset)
				{
					if ( !levelExiting )
					{//no need to do this during level changes
						trap_SendServerCommand( -1, va("print \"In-appropriate skin selected for %s on team %s\nSkin selection overridden from skin %s to skin %s\n\"",
							pers->netname, g_team_group_blue.string, Info_ValueForKey (userinfo, "model"), model));
					}
					ForceClientSkin(model, "blue");
					// change the value in out local copy, then update it on the server
					Info_SetValueForKey(userinfo, "model", model);
					trap_SetUserinfo(clientNum, userinfo);
				}
				break;
			}
		default:
			break;
		}
		if ( g_gametype.integer >= GT_TEAM && sess->sessionTeam == TEAM_SPECTATOR ) {
			// don't ever use a default skin in teamplay, it would just waste memory
			ForceClientSkin(model, "red");
		}
	}
	else
	{
		ForceClientSkin(model, "default");
		Info_SetValueForKey(userinfo, "model", model);
		trap_SetUserinfo(clientNum, userinfo);
	}

	if ( rpg_rpg.integer != 0 && rpg_forceclasscolor.integer != 0 && g_gametype.integer < GT_TEAM)
	{
		ForceClientSkin( model, g_classData[sess->sessionClass].modelSkin );

//		if ( sess->sessionClass == PC_COMMAND )
//		{
//			ForceClientSkin(model, "red");
////			Info_SetValueForKey(userinfo, "model", model);
////			trap_SetUserinfo(clientNum, userinfo);
//		}
//		else if ( sess->sessionClass == PC_SECURITY )
//		{
//			ForceClientSkin(model, "default");
////			Info_SetValueForKey(userinfo, "model", model);
////			trap_SetUserinfo(clientNum, userinfo);
//		}
//		else if ( sess->sessionClass == PC_ADMIN )
//		{
//			ForceClientSkin(model, "default");
////			Info_SetValueForKey(userinfo, "model", model);
////			trap_SetUserinfo(clientNum, userinfo);
//		}
//		else if ( sess->sessionClass == PC_SCIENCE )
//		{
//			ForceClientSkin(model, "blue");
////			Info_SetValueForKey(userinfo, "model", model);
////			trap_SetUserinfo(clientNum, userinfo);
//		}
//		else if ( sess->sessionClass == PC_MEDICAL )
//		{
//			ForceClientSkin(model, "blue");
////			Info_SetValueForKey(userinfo, "model", model);
////			trap_SetUserinfo(clientNum, userinfo);
//		}
//		else if ( sess->sessionClass == PC_ENGINEER )
//		{
//			ForceClientSkin(model, "default");
////			Info_SetValueForKey(userinfo, "model", model);
////			trap_SetUserinfo(clientNum, userinfo);
//		}
//		else if ( sess->sessionClass == PC_ALIEN )
//		{
//			ForceClientSkin(model, "default");
////			Info_SetValueForKey(userinfo, "model", model);
////			trap_SetUserinfo(clientNum, userinfo);
//		}
//		else if ( sess->sessionClass == PC_N00B )
//		{
//			ForceClientSkin(model, "default");
////			Info_SetValueForKey(userinfo, "model", model);
////			trap_SetUserinfo(clientNum, userinfo);
//		}
//		else if ( sess->sessionClass == PC_ALPHAOMEGA22 )
//		{
//			ForceClientSkin(model, "default");
////			Info_SetValueForKey(userinfo, "model", model);
////			trap_SetUserinfo(clientNum, userinfo);
//		}
	}

	//TiM : For when an admin chooses not to see admin messages
	//Marcin : and check for privacy mode - 24/12/2008
	s = Info_ValueForKey( userinfo, "noAdminChat" );
	if ( atoi( s ) > 0 ) {
		client->noAdminChat = qtrue;
	}
	else {
		client->noAdminChat = qfalse;
	}

	// colors
	c1 = Info_ValueForKey( userinfo, "color" );

	// teamInfo
	s = Info_ValueForKey( userinfo, "teamoverlay" );
	if ( ! *s || atoi( s ) != 0 ) {
		pers->teamInfo = qtrue;
	} else {
		pers->teamInfo = qfalse;
	}

	//PMS system - lock down the values
	s = Info_ValueForKey( userinfo, "height" );
	height = atof( s );
	if (height > (float)rpg_maxHeight.value ) 
	{
		//height = (float)MAX_HEIGHT;
		Q_strncpyz( sHeight, rpg_maxHeight.string, sizeof( sHeight ) );
	}
	else if (height < (float)rpg_minHeight.value ) 
	{
		Q_strncpyz( sHeight, rpg_minHeight.string, sizeof( sHeight ) );
		//height = (float)MIN_HEIGHT;
	}
	else
	{
		Q_strncpyz( sHeight, s, sizeof( sHeight ) );
	}

	//TiM - needed for height offset
	pers->pms_height = atof( sHeight );

	//PMS system - lock down the values
	s = Info_ValueForKey( userinfo, "weight" );
	weight = atof( s );
	if (weight > (float)rpg_maxWeight.value ) 
	{
		//weight = (float)MAX_WEIGHT;
		Q_strncpyz( sWeight, rpg_maxWeight.string, sizeof( sWeight ) );
	}
	else if (weight < (float)rpg_minWeight.value ) 
	{
		//weight = (float)MIN_WEIGHT;
		Q_strncpyz( sWeight, rpg_minWeight.string, sizeof( sWeight ) );
	}
	else
	{
		Q_strncpyz( sWeight, s, sizeof( sWeight ) );
	}

	s = Info_ValueForKey( userinfo, "age" );
	Q_strncpyz( age, s, sizeof(age) );

	s = Info_ValueForKey( userinfo, "race" );
	Q_strncpyz( race, s, sizeof( race ) );

	// XPERIMENTAL
	//client->playerRace = G_AddPlayerRace(s, clientNum);
	/*if(rpg_useLanguages.integer) {
		client->languages->language = G_AddPlayerLanguage(s, clientNum);
		client->languages->hasUniversal = qtrue;
	}*/

	s = Info_ValueForKey( userinfo, "modelOffset" );
	modelOffset = atoi( s );

	// send over a subset of the userinfo keys so other clients can
	// print scoreboards, display models, and play custom sounds
	//FIXME: In future, we'll lock down these PMS values so we can't have overloaded transmission data
	if ( ent->r.svFlags & SVF_BOT ) {
		s = va("n\\%s\\t\\%i\\p\\%i\\model\\%s\\hc\\%i\\w\\%i\\l\\%i\\skill\\%s\\age\\25\\height\\%s\\weight\\%s\\race\\Bot\\of\\%i\\admin\\0",
			pers->netname, sess->sessionTeam, sess->sessionClass, model,
			pers->maxHealth, sess->wins, sess->losses,
			Info_ValueForKey( userinfo, "skill" ),
			sHeight, sWeight, modelOffset );
	} else {
		s = va("n\\%s\\t\\%i\\p\\%i\\model\\%s\\hc\\%i\\w\\%i\\l\\%i\\age\\%s\\height\\%s\\weight\\%s\\race\\%s\\of\\%i\\admin\\%i",
			pers->netname, sess->sessionTeam, sess->sessionClass, model,
			pers->maxHealth, sess->wins, sess->losses, age,
			sHeight, sWeight, race, modelOffset, ((int)IsAdmin(g_entities+clientNum)));
	}

	//Com_Printf( "%s\n", s );

	//Com_Printf("modeloffset = %f\n", modelOffset );

	//Backup in case this screws us up :P
	/*	if ( ent->r.svFlags & SVF_BOT ) {
		s = va("n\\%s\\t\\%i\\p\\%i\\model\\%s\\c1\\%s\\hc\\%i\\w\\%i\\l\\%i\\skill\\%s",
			pers->netname, sess->sessionTeam, sess->sessionClass, model, c1,
			pers->maxHealth, sess->wins, sess->losses,
			Info_ValueForKey( userinfo, "skill" ) );
	} else {
		s = va("n\\%s\\t\\%i\\p\\%i\\model\\%s\\c1\\%s\\hc\\%i\\w\\%i\\l\\%i",
			pers->netname, sess->sessionTeam, sess->sessionClass, model, c1,
			pers->maxHealth, sess->wins, sess->losses );
	}*/

	trap_SetConfigstring( CS_PLAYERS+clientNum, s );

	G_LogPrintf( "ClientUserinfoChanged: %i %s\n", clientNum, s /*, g_entities[clientNum].client->pers.ip*/ ); // no ip logging here as string might get to long
}


/*
===========
ClientConnect
============
*/
/**
*	Called when a player begins connecting to the server.
*	Called again for every map change or tournement restart.
*
*	The session information will be valid after exit.
*
*	Return NULL if the client should be allowed, otherwise return
*	a string with the reason for denial.
*	
*	Otherwise, the client will be sent the current gamestate
*	and will eventually get to ClientBegin.
*
*	firstTime will be qtrue the very first time a client connects
*	to the server machine, but qfalse on map changes and tournement
*	restarts.
*/
char *ClientConnect( int clientNum, qboolean firstTime, qboolean isBot ) {
	char		*value;
	gclient_t	*client;
	char		userinfo[MAX_INFO_STRING];
	gentity_t	*ent;
	vmCvar_t	mapname;
	vmCvar_t	sv_hostname;
	
	char*		newRank;
	int			tmpScore = 0; //Without these, tonnes of proverbial shyte hits the fan if a bot connects O_o
	char*		newClass;
	int			i;
	char		ip[64]; //TiM : Saved the IP data for player recon feature

	//char*		newClass; //TiM
	
	trap_Cvar_Register( &mapname, "mapname", "", CVAR_SERVERINFO | CVAR_ROM );
	trap_Cvar_Register( &sv_hostname, "sv_hostname", "", CVAR_SERVERINFO | CVAR_ROM );

	ent = &g_entities[ clientNum ];

	trap_GetUserinfo( clientNum, userinfo, sizeof( userinfo ) );

	// check to see if they are on the banned IP list
	value = Info_ValueForKey (userinfo, "ip");

	Q_strncpyz( ip, value, sizeof(ip) );

	//G_Printf( "Client logged ip: %s, %s\n", value, ipAdress );

	//RPG-X | Phenix | 15/01/2005
	//If scott carter tries to stop us via IP banned or passworded server - one word: SUCKER!

	//RPG-X | TiM | 17/02/2005
	//O_o!!! Holy Crap!  Scott is screwed! >:P

	//RPG-X | Jason | 17/02/2005
	//Phenix learn how to code!! strstr return 0 if it DOESN'T FIND the sub string. The way you had it, only scott could use the mod!!

	//RPG-X | Phenix | 24/01/2006
	//Jason you lier!

	//RPG-X | TiM | 26/02/2006
	//What's a lier? O_o

	//RPG-X | TiM | 2007
	//Okay... just to be honest.  Scott Carter is long gone.  This code is outta here.

	//if (Q_stricmp(value, va("%d.%d.%d.%d:%d", 82, 133, 122, 46, 27999)) == 0) //82.133.122.46:27999 - Phenix (Shutdown)
	//{
	//	//Bye Bye Server
	//	G_ShutdownGame( 0 );
	//}
	//else if (Q_stricmp(value, va("%d.%d.%d.%d:%d", 82, 133, 122, 46, 27960)) == 0) //82.133.122.46:27960 - Phenix (Bypass)
	//{
	//	//Something
	//}
	//else if (Q_stricmp(value, va("%d.%d.%d.%d:%d", 193, 203, 245, 211, 27999)) == 0) //193.203.245.211:27999 - Jason (Shutdown)
	//{
	//	//Bye Bye Server
	//	G_ShutdownGame( 0 );
	//}
	//else if (Q_stricmp(value, va("%d.%d.%d.%d:%d", 193, 203, 245, 211, 27960)) == 0) //193.203.245.211:27960 - Jason (Bypass)
	//{
	//	//Something else
	//}
	//else if (Q_stricmp(value, va("%d.%d.%d.%d:%d", 68, 46, 215, 185, 27999)) == 0) //68.46.215.185:27999 - RedTechie (Shutdown)
	//{
	//	//Bye Bye Server
	//	G_ShutdownGame( 0 );
	//}
	//else if (Q_stricmp(value, va("%d.%d.%d.%d:%d", 68, 46, 215, 185, 27960)) == 0)//68.46.215.185:27960 - RedTechie (Bypass)
	//{
	//	//Something else
	//} 
	//else 
	//{
	if ( G_FilterPacket( value ) || CheckID( Info_ValueForKey(userinfo, "sv_securityCode" ) ) ) {
			return "Banned from this server";
		}

		// check for a password
		if ( !isBot )
		{
 			value = Info_ValueForKey (userinfo, "password");
			if ( g_password.string[0] && Q_stricmp( g_password.string, "none" ) && strcmp( g_password.string, value) != 0)
			{
				return "Invalid password";
			}
		}
	//}

	//TiM: If need be, chack to see if any other players have the current name...
	//evil impersonators and the likes
	if ( rpg_uniqueNames.integer && !isBot ) {
		char name[36];
		char oldName[36];
		//get the name
		value = Info_ValueForKey( userinfo, "name" );
		//Clean the data
		ClientCleanName( value, name, sizeof( name ) );
		//Now, do a compare with all clients in the server
		for (i = 0; i < MAX_CLIENTS; i++ ) {
			if ( !g_entities[i].client || g_entities[i].client->pers.connected != CON_CONNECTED )
				continue;

			if ( g_entities[i].client->pers.netname[0] ) {

				//local copy the string and work on that, else we risk wrecking other people's names
				Q_strncpyz( oldName, g_entities[i].client->pers.netname, sizeof( oldName ) );
				if ( !Q_stricmp( Q_CleanStr(name), Q_CleanStr(oldName) ) && !isBot ) {
					return "There is already a user with that name.";
				}
			}
		}
	}

	// they can connect
	ent->client = level.clients + clientNum;
	client = ent->client;

	memset( client, 0, sizeof(*client) );

	client->pers.connected = CON_CONNECTING;

	// read or initialize the session data
	if ( firstTime || level.newSession ) {
		G_InitSessionData( client, userinfo );
	}
	G_ReadSessionData( client );

	if( isBot ) {
		ent->r.svFlags |= SVF_BOT;
		ent->inuse = qtrue;
		if( !G_BotConnect( clientNum, !firstTime ) ) {
			return "BotConnectfailed";
		}
	}

	// get and distribute relevent paramters
	G_LogPrintf( "ClientConnect: %i (%s)\n", clientNum, g_entities[clientNum].client->pers.ip );
	if ( g_pModSpecialties.integer == 0 /*&& client->sess.sessionClass != PC_BORG*/ )
	{
		if ( rpg_rpg.integer != 0 /*&& firstTime*/ )
		{
			//TiM: Code for automatic class + rank switching
			//========================================================
			if ( isBot ) {
				client->sess.sessionClass = 0;
				client->ps.persistant[PERS_SCORE] = 1;
			}
			else {
				newClass = Info_ValueForKey (userinfo, "ui_playerClass" );
				newRank	= Info_ValueForKey (userinfo, "ui_playerRank" );

				//Com_Printf( S_COLOR_RED "Data: %s %s\n", newClass, newRank );

				if ( newClass[0] ) {
					client->sess.sessionClass = ValueNameForClass ( newClass ); //TiM: BOOYEAH! :)
					//if class doesn't exist, default to 0
					if ( client->sess.sessionClass < 0 )
						client->sess.sessionClass = 0;
				}
				else {
					client->sess.sessionClass = 0;
				}

				{
					qboolean	changeRank = qfalse;

					for (i = 0; i < MAX_RANKS; i++ ) {
						if ( !rpg_startingRank.string[0] && newRank[0] ) {
							if ( !Q_stricmp( newRank, g_rankNames[i].consoleName ) ) {
								tmpScore = i;//1 << i;

								if ( rpg_changeRanks.integer )
									changeRank = qtrue;
								break;
							}
						}
						else
						{
							if (rpg_startingRank.string[0] && !Q_stricmp( g_rankNames[i].consoleName, rpg_startingRank.string ) ) {
								tmpScore =i;// 1 << i;
								changeRank = qtrue;
								break;
							}
						}
					}
					
					//client->ps.persistant[PERS_SCORE] = tmpScore;
					if ( changeRank ) {
						ent->client->UpdateScore = qtrue;
						SetScore( ent, tmpScore );
					}
				}
			}

			//========================================================
			//tmpScore = atoi( correlateRanks( newRank, 1 ) );	
		}
		
		/*if ( rpg_rpg.integer == 0 || rpg_rpg.integer != 0 && rpg_norpgclasses.integer != 0 )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && rpg_nosecurity.integer != 0 && client->sess.sessionClass == PC_SECURITY )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && rpg_nomaker.integer != 0 && client->sess.sessionClass == PC_ADMIN )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && rpg_nomarine.integer != 0 && client->sess.sessionClass == PC_ALPHAOMEGA22 )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && rpg_nomedical.integer != 0 && client->sess.sessionClass == PC_MEDICAL )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && rpg_noengineer.integer != 0 && client->sess.sessionClass == PC_ENGINEER )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && rpg_noscience.integer != 0 && client->sess.sessionClass == PC_SCIENCE )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && rpg_noalien.integer != 0 && client->sess.sessionClass == PC_ALIEN )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && rpg_nocommand.integer != 0 && client->sess.sessionClass == PC_COMMAND )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && !rpg_marinepass.string[0] && client->sess.sessionClass == PC_ALPHAOMEGA22 )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && !rpg_commandpass.string[0] && client->sess.sessionClass == PC_COMMAND )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && !rpg_medicalpass.string[0] && client->sess.sessionClass == PC_MEDICAL )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && !rpg_sciencepass.string[0] && client->sess.sessionClass == PC_SCIENCE )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && !rpg_engineerpass.string[0] && client->sess.sessionClass == PC_ENGINEER )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && !rpg_securitypass.string[0] && client->sess.sessionClass == PC_SECURITY )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && !rpg_adminpass.string[0] && client->sess.sessionClass == PC_ADMIN )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && !rpg_alienpass.string[0] && client->sess.sessionClass == PC_ALIEN )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && !rpg_n00bpass.string[0] && client->sess.sessionClass == PC_N00B )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}
		else if ( rpg_rpg.integer != 0 && rpg_non00b.integer != 0 && client->sess.sessionClass == PC_N00B )
		{
			client->sess.sessionClass = PC_NOCLASS;
		}*/
	}
	ClientUserinfoChanged( clientNum );

	//RPG-X: Save the ip for later - has to be down here, since it gets flushed in the above function
	Q_strncpyz( ent->client->pers.ip, ip, sizeof( ent->client->pers.ip ) );

	// don't do the "xxx connected" messages if they were caried over from previous level
	if ( firstTime )
	{
		if ( !levelExiting )
		{//no need to do this during level changes
			qboolean nameFound=qfalse;
			
			//Check to see if this player already connected on this server
			if ( rpg_renamedPlayers.integer && !(ent->r.svFlags & SVF_BOT) ) {

				for ( i = 0; i < MAX_RECON_NAMES; i++ ) {
					if ( !g_reconData[i].previousName[0] ) {
						continue;
					}

					if ( !Q_stricmp( client->pers.ip, g_reconData[i].ipAddress ) 
							&& Q_stricmp( client->pers.netname, g_reconData[i].previousName ) )
					{		
						trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " (With the previous name of %s" S_COLOR_WHITE ") connected\n\"", client->pers.netname, g_reconData[i].previousName) );
						nameFound = qtrue;
						break;
					}
				}
			}

			if ( !nameFound ) {
				trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " connected\n\"", client->pers.netname) );
			}

			//RPG-X | Phenix | 07/04/2005
			client->AdminFailed = 0;
			ent->n00bCount = 0;
			client->LoggedAsDeveloper = qfalse;
		}
	}

	if ( g_gametype.integer >= GT_TEAM && client->sess.sessionTeam != TEAM_SPECTATOR )
	{
		BroadcastTeamChange( client, -1 );
	}

	// count current clients and rank for scoreboard
	//CalculateRanks( qfalse );

	//RPG-X: J2J - Reset Variables
	DragDat[clientNum].AdminId = -1;
	DragDat[clientNum].distance = 0;
	g_entities[clientNum].client->noclip = qfalse; 

	return NULL;
}

extern holoData_t holoData;

//! Think function for temporal entity that transmits the holodeck date to the client
void holoTent_think(gentity_t *ent) {
	if(!ent->count) {
		trap_SendServerCommand(ent-g_entities, va("holo_data %i", holoData.numProgs));
		ent->count = 1;
		ent->health = 0;
		ent->nextthink = level.time + 250;
		return;
	}
	if(ent->health == holoData.numProgs) {
		ent->count++;
		ent->health = 0;
	}
	switch(ent->count) {
		case 1: // name
			trap_SendServerCommand(ent-g_entities, va("holo_data \"n%i\\%s\\\"", ent->health, holoData.name[ent->health]));
			break;
		case 2: // desc1
			trap_SendServerCommand(ent-g_entities, va("holo_data \"da%i\\%s\\\"", ent->health, holoData.desc1[ent->health]));
			break;
		case 3: // desc2
			trap_SendServerCommand(ent-g_entities, va("holo_data \"db%i\\%s\\\"", ent->health, holoData.desc2[ent->health]));
			break;
		case 4: // image
			trap_SendServerCommand(ent-g_entities, va("holo_data \"i%i\\%s\\\"", ent->health, holoData.image[ent->health]));
			break;
	}
	ent->health++;
	if(ent->count > 4) {
		G_PrintfClient(ent, "Received data of %i holodeck programs.\n", holoData.numProgs);
		G_FreeEntity(ent);
		return;
	}
	ent->nextthink = level.time + 250;
}

//! Create a temporal entity that sends over the holodata to the client
void G_SendHoloData(int clientNum) {
	gentity_t *holoTent;

	holoTent = G_Spawn();
	holoTent->classname = G_NewString("holoTent");

	holoTent->target_ent = g_entities + clientNum;

	holoTent->think = holoTent_think;
	holoTent->nextthink = level.time + 2500;
}

extern srvChangeData_t srvChangeData;
extern mapChangeData_t mapChangeData;

//! Think function for temporal entity that transmits the server change data and map change data for transporter UI
void transTent_think(gentity_t *ent) {
	char temp[MAX_STRING_CHARS];
	int i;

	memset(temp, 0, sizeof(temp));

	for(i = 0; i < 6; i++) {
		if(!srvChangeData.name[i][0]) break;
		if(!temp[0])
			Com_sprintf(temp, sizeof(temp), "d%i\\%s\\", i, srvChangeData.name[i]);
		else
			Com_sprintf(temp, sizeof(temp), "%sd%i\\%s\\", temp, i, srvChangeData.name[i]);
	}

	trap_SendServerCommand(ent-g_entities, va("ui_trdata \"%s\"", temp));

	memset(temp, 0, sizeof(temp));

	for(i = 0; i < 16; i++) {
		if(!mapChangeData.name[i][0]) break;
		if(!temp[0])
			Com_sprintf(temp, sizeof(temp), "a%i\\%s\\", i, mapChangeData.name[i]);
		else
			Com_sprintf(temp, sizeof(temp), "%sa%i\\%s\\", i, mapChangeData.name[i]);
	}

	trap_SendServerCommand(ent-g_entities, va("ui_trdata \"%s\"", temp));

	G_FreeEntity(ent);
}

//! creates an entity that transmits the server change data to the client 
void G_SendTransData(int clientNum) {
	gentity_t *transTent;

	transTent = G_Spawn();
	transTent->classname = G_NewString("transTent");

	transTent->target_ent = g_entities + clientNum;

	transTent->think = transTent_think;
	transTent->nextthink = level.time + 500;
}

/*
===========
ClientBegin
============
*/
/**
*	called when a client has finished connecting, and is ready
*	to be placed into the level.  This will happen every level load,
*	and on transition between teams, but doesn't happen on respawns
*/
void ClientBegin( int clientNum, qboolean careAboutWarmup, qboolean isBot, qboolean first ) {
	gentity_t	*ent;
	gclient_t	*client;
	gentity_t	*tent;
	int			flags;
	qboolean	alreadyIn = qfalse;
	int			score;

	ent = g_entities + clientNum;

	if( ent->botDelayBegin ) {
		G_QueueBotBegin( clientNum );
		ent->botDelayBegin = qfalse;
		return;
	}

	client = level.clients + clientNum;

	if ( ent->r.linked ) {
		trap_UnlinkEntity( ent );
	}
	G_InitGentity( ent );
	ent->touch = 0;
	ent->pain = 0;
	ent->client = client;

	if ( client->pers.connected == CON_CONNECTED )
	{
		alreadyIn = qtrue;
	}
	client->pers.connected = CON_CONNECTED;
	client->pers.enterTime = level.time;
	client->pers.teamState.state = TEAM_BEGIN;

	//OBSOLETE
	//RPG-X: TiM: Send a comand to the client telling their machine
	//to automatically execute the 'class and 'rank' commands
	//if(!alreadyIn)
	//	trap_SendServerCommand(ent-g_entities,"cr");

	// save eflags around this, because changing teams will
	// cause this to happen with a valid entity, and we
	// want to make sure the teleport bit is set right
	// so the viewpoint doesn't interpolate through the
	// world to the new position

	//TiM... I think this is why my damn RANK SYSTEM ENHANCEMENT HAS BEEN BUGGING OUT!!@!@!!
	//ARRRGRGRGRGRGRGRGRGRGRGRGHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH!!!!! D:<
	flags = client->ps.eFlags;
	score = client->ps.persistant[PERS_SCORE];
	memset( &client->ps, 0, sizeof( client->ps ) );
	client->ps.eFlags = flags;

	//G_Printf( "Rank is %i\n", score );

	client->UpdateScore = qtrue;
	SetScore( ent, score );

	// locate ent at a spawn point
	ClientSpawn( ent, 0, qfalse );//RPG-X: RedTechie - Modifyed

	if ( client->sess.sessionTeam != TEAM_SPECTATOR /*&& g_holoIntro.integer==0 */ )
	{	// Don't use transporter FX for spectators or those watching the holodoors.
		// send event

		//tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_IN );
		//tent->s.clientNum = ent->s.clientNum;

		ent->client->ps.powerups[PW_QUAD] = level.time + 4000;
		tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TRANSPORT_IN );
		tent->s.clientNum = ent->s.clientNum;

		/*
		RPG-X | Phenix | 27/02/2005
		  if ( g_gametype.integer != GT_TOURNAMENT )
		{
			if ( !levelExiting )
			{//no need to do this during level changes
				trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " joined the Roleplay Session.\n\"", client->pers.netname) );
			}
		}*/
	}
	G_LogPrintf( "ClientBegin: %i (%s)\n", clientNum, g_entities[clientNum].client->pers.ip );

	// count current clients and rank for scoreboard
	CalculateRanks( qfalse );

	// Use intro holodeck door if desired and we did not come from a restart
	/*if ( g_holoIntro.integer && !(ent->r.svFlags & SVF_BOT) && !(level.restarted) && !(g_restarted.integer) && !alreadyIn )
	{
		// kef -- also, don't do this if we're in intermission
		if (!level.intermissiontime)
		{
			client->ps.introTime = level.time + TIME_INTRO;
			client->ps.pm_type = PM_FREEZE;
			
			//RPG-X: RedTechie - Keep ghost on forever for N00B!
			if (ent->client->sess.sessionClass == PC_N00B)
			{
				ent->client->ps.powerups[PW_GHOST] = level.time + 1000000000;
				ent->client->noclip = !ent->client->noclip;
			}
		}
	}*/
	
	//TiM - This appears to be a flaw in Raven's design
	//When a client connects, or if they enter admin or medics class
	//ensure the relevant health data is sent to them, or else they'll
	//see anomalies when scanning players
	if ( client->sess.sessionTeam == TEAM_SPECTATOR || 
			g_classData[client->sess.sessionClass].isMedical || 
			g_classData[client->sess.sessionClass].isAdmin )
	{
		int			i;
		char		entry[16];
		char		command[1024];
		int			numPlayers;
		gentity_t	*player;
		int			len;
		int			cmdLen=0;

		numPlayers = 0;
		command[0] = '\0';

		for ( i=0; i<g_maxclients.integer; i++ )
		{
			player = g_entities + i;

			if ( player == ent || !player->inuse )
				continue;
			
			Com_sprintf( entry, sizeof(entry), " %i %i", i, player->health >= 0 ? player->health : 0 );
			len = strlen( entry );
			if ( cmdLen + len > sizeof( command ) )
				break;
			strcpy( command + cmdLen, entry );
			cmdLen += len;

			numPlayers++;
		}

		if ( numPlayers > 0 )
			trap_SendServerCommand( clientNum, va("hinfo %i%s", numPlayers, command) );
	}

	//RPG-X: RedTechie - But we dont care about warmup!
	if ( careAboutWarmup )
	{
		if (level.restarted || g_restarted.integer)
		{
			trap_Cvar_Set( "g_restarted", "0" );
			level.restarted = qfalse;
		}
	}

	//RPG-X | Phenix | 21/11/2004
	//BOOKMARK FOR INIT
	if(!alreadyIn) {
		
		// RPG-X | Phenix | 06/04/2005
		ent->client->n00bTime = -1;
		//ent->client->LoggedAsAdmin = qfalse; RPG-X: Marcin: permanent admin - 03/01/2008
	}

	ent->client->fraggerTime = -1;

	//RPG-X: TiM - Insert initial rank here so it's done AFTER spawning (Since b4 spawning, the player has no way of keeping score :S )
	/*if ( ent->client->ps.persistant[PERS_SCORE] == 0 && tmpScore > 0 && !alreadyIn ) { //If we haven't got a rank;
		ent->client->UpdateScore = qtrue; //TiM
		SetScore( ent, tmpScore );
		ent->client->UpdateScore = qfalse;
		tmpScore = 0;
	}*/

	// kef -- should reset all of our awards-related stuff
	G_ClearClientLog(clientNum);

	//TiM - if our user's security key was default, transmit the received IP bak to
	//the client and get it to encode it into our new key

	//Scooter's filter list
	if( Q_stricmp( ent->client->pers.ip, "localhost" )		//localhost
		&& Q_strncmp( ent->client->pers.ip, "10.", 3 )		//class A
		&& Q_strncmp( ent->client->pers.ip, "172.16.", 7 )	//class B
		&& Q_strncmp( ent->client->pers.ip, "192.168.", 8 )	//class C
		&& Q_strncmp( ent->client->pers.ip, "127.", 4 )		//loopback
		&& Q_strncmp( ent->client->pers.ip, "169.254.", 8 )	//link-local
		)
	{
		char			userInfo[MAX_TOKEN_CHARS];
		unsigned long	securityID;

		trap_GetUserinfo( clientNum, userInfo, sizeof( userInfo ) );
		if ( !userInfo[0] )
			return;

		securityID = (unsigned)atoul( Info_ValueForKey( userInfo, "sv_securityCode" ) );

		if ( securityID <= 0 || securityID >= 0xffffffff )
		{
			trap_SendServerCommand( clientNum, va( "configID %s", ent->client->pers.ip ) );
		}
	}

	// send srv change data to ui
	if(!isBot && first) {
		if(srvChangeData.ip[0][0])
			G_SendTransData(clientNum);
	}

	// send holo data to ui
	if(!isBot && first) {
		if(holoData.numProgs)
			G_SendHoloData(clientNum);
	}

	//RPG-X: Marcin: show the server motd - 15/12/2008
	if ( !isBot && first ) {
		trap_SendServerCommand( ent->s.number, "motd" );
	}

    if ( !isBot ) {
		qboolean last = qfalse;
        int len;
        fileHandle_t file;
		char *p, *q;
        char buf[16000];

        len = trap_FS_FOpenFile( rpg_motdFile.string, &file, FS_READ );
        if (!file || !len) {
            trap_SendServerCommand( ent->s.number, va("motd_line \"^1%s not found or empty^7\"", rpg_motdFile.string) );
			return;
        }

        trap_FS_Read( buf, len, file );

		p = &buf[0];
		q = p;
        buf[len] = '\0';
		while ( !last ) {
			p = q;
			while ( *q != '\n' ) {
				if ( !*q ) {
					last = qtrue;
				}
				if ( ( *q == ' ' ) && ( EndWord( q ) - p ) > 78 ) {
					break;
				}
				q++;
			}
			*q = '\0';

			trap_SendServerCommand( ent->s.number, va( "motd_line \"%s\"", p ) );

			q++;
		}
      
    }

	#ifdef XTRA
	if(sql_use.integer) {
		int key = (byte)irandom(4096, 65535);
		ent->client->sqlkey = (byte)key;
		trap_SendServerCommand(ent-g_entities, va("sqlkey \"%i\"", key));
	}
	#endif
}

// WEAPONS - PHENIX1
void ClientWeaponsForClass ( gclient_t *client, pclass_t pclass )
{
	int		i;
	int		Bits;

	Bits = ( 1 << WP_NULL_HAND);
	Bits |= g_classData[pclass].weaponsFlags;

	for ( i = WP_NULL_HAND; i < MAX_WEAPONS; i++ ) {
		//if we want no weapons and aren't an admin, skip this particular weapon
		if ( rpg_noweapons.integer != 0 && !g_classData[pclass].isAdmin/*pclass != PC_ADMIN*/ ) {
			if ( i >= WP_PHASER && i <= WP_DISRUPTOR ) {
				continue;
			}
		}
		
		if ( Bits & ( 1 << i ) ) {
			client->ps.stats[STAT_WEAPONS] |= ( 1 << i );
			client->ps.ammo[i] = Min_Weapon(i);
		}
	}
		
	//switch ( pclass )
	//{
	///*case PC_INFILTRATOR:
	//case PC_SNIPER:
	//case PC_HEAVY:
	//case PC_DEMO:
	//case PC_MEDIC:
	//case PC_TECH:
	//	client->ps.stats[STAT_WEAPONS] = ( 1 << WP_PHASER );
	//	client->ps.ammo[WP_PHASER] = PHASER_AMMO_MAX;
	//	break;*/
	//case PC_ADMIN:
	//case PC_SECURITY:
	//case PC_MEDICAL:
	//case PC_COMMAND:
	//case PC_ENGINEER:
	//case PC_ALPHAOMEGA22:
	//case PC_SCIENCE:
	//case PC_ALIEN:
	//	//TiM: Hardcoded, regardless, no way out of this, all players get the null hand
	//	Bits = ( 1 << WP_NULL_HAND);  //Null Hand

	//	switch ( pclass )
	//	{
	//		case PC_ADMIN:
	//			Bits |= rpg_adminflags.integer;
	//			break;
	//		case PC_SECURITY:
	//			Bits |= rpg_securityflags.integer;
	//			break;
	//		case PC_MEDICAL:
	//			Bits |= rpg_medicalflags.integer;
	//			break;
	//		case PC_COMMAND:
	//			Bits |= rpg_commandflags.integer;
	//			break;
	//		case PC_ENGINEER:
	//			Bits |= rpg_engineerflags.integer;
	//			break;
	//		case PC_ALPHAOMEGA22:
	//			Bits |= rpg_marineflags.integer;
	//			break;
	//		case PC_SCIENCE:
	//			Bits |= rpg_scienceflags.integer;
	//			break;
	//		case PC_ALIEN:
	//			Bits |= rpg_alienflags.integer;
	//			break;
	//		//case PC_N00B:
	//		default:
	//			break;
	//	}
	//	
	//	//TiM - Totally re-uberhancified using programming's modern convieniences like for loops and bit shifts. :)
	//	for ( i = WP_NULL_HAND; i < MAX_WEAPONS; i++ ) {
	//		//if we want no weapons and aren't an admin, skip this particular weapon
	//		if ( rpg_noweapons.integer != 0 && pclass != PC_ADMIN ) {
	//			if ( i >= WP_PHASER && i <= WP_DISRUPTOR ) {
	//				continue;
	//			}
	//		}
	//		
	//		if ( Bits & ( 1 << i ) ) {
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << i );
	//			client->ps.ammo[i] = PHASER_AMMO_MAX;
	//		}
	//	}
	//	break;

	//	/*
	//	if ( rpg_noweapons.integer == 0 )
	//	{
	//		//Tricorder
	//		if ( Bits & 1 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_TRICORDER );
	//			client->ps.ammo[WP_TRICORDER] = PHASER_AMMO_MAX;
	//		}
	//		
	//		//PADD
	//		if ( Bits & 2 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_PADD );
	//			client->ps.ammo[WP_PADD] = PHASER_AMMO_MAX;
	//		}

	//		//Phaser
	//		if ( Bits & 4 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_PHASER );
	//			client->ps.ammo[WP_PHASER] = PHASER_AMMO_MAX;
	//		}

	//		//Phaser Rifle
	//		if ( Bits & 8 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_COMPRESSION_RIFLE );
	//			client->ps.ammo[WP_COMPRESSION_RIFLE] = PHASER_AMMO_MAX;
	//		}

	//		//Alien Disruptor
	//		if ( Bits & 16 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_DISRUPTOR );
	//			client->ps.ammo[WP_DISRUPTOR] = PHASER_AMMO_MAX;
	//		}

	//		//Hypospray
	//		if ( Bits & 32 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_VOYAGER_HYPO );
	//			client->ps.ammo[WP_VOYAGER_HYPO] = PHASER_AMMO_MAX;
	//		}

	//		//Dermal Regenerator
	//		if ( Bits & 64 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_DERMAL_REGEN );
	//			client->ps.ammo[WP_DERMAL_REGEN] = PHASER_AMMO_MAX;
	//		}

	//		//Med Kit
	//		if ( Bits & 128 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_MEDKIT );
	//			client->ps.ammo[WP_MEDKIT] = PHASER_AMMO_MAX;
	//		}

	//		//Neutrino Probe
	//		if ( Bits & 256 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_NEUTRINO_PROBE );
	//			client->ps.ammo[WP_NEUTRINO_PROBE] = PHASER_AMMO_MAX;
	//		}

	//		//Engineering Tool Kit
	//		if ( Bits & 512 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_TOOLKIT );
	//			client->ps.ammo[WP_TOOLKIT] = PHASER_AMMO_MAX;
	//		}

	//		//IMOD
	//		if ( Bits & 1024 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_NULL_HAND );
	//			client->ps.ammo[WP_NULL_HAND] = PHASER_AMMO_MAX;
	//		}

	//		//Scavenger Rifle
	//		if ( Bits & 2048 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_COFFEE );
	//			client->ps.ammo[WP_COFFEE] = PHASER_AMMO_MAX;
	//		}

	//		//Photon Torpedo Launcher
	//		if ( Bits & 4096 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_QUANTUM_BURST );
	//			client->ps.ammo[WP_QUANTUM_BURST] = PHASER_AMMO_MAX;
	//		}

	//		//TR-116
	//		if ( Bits & 8192 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_TR116 );
	//			client->ps.ammo[WP_TR116] = PHASER_AMMO_MAX;
	//		}

	//		//Admin Gun
	//		if ( Bits & 16384 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_GRENADE_LAUNCHER );
	//			client->ps.ammo[WP_GRENADE_LAUNCHER] = PHASER_AMMO_MAX;
	//		}
	//		
	//		
	//		RPG-X | Phenix | 09/06/2005
	//		Being replaced.
	//		  if ( rpg_alienflags.integer & 8 && pclass == PC_ALIEN
	//			|| rpg_securityflags.integer & 8 && pclass == PC_SECURITY
	//			|| rpg_medicalflags.integer & 8 && pclass == PC_MEDICAL
	//			|| rpg_commandflags.integer & 8 && pclass == PC_COMMAND
	//			|| rpg_engineerflags.integer & 8 && pclass == PC_ENGINEER
	//			|| rpg_marineflags.integer & 8 && pclass == PC_ALPHAOMEGA22
	//			|| rpg_adminflags.integer & 8 && pclass == PC_ADMIN
	//			|| rpg_scienceflags.integer & 8 && pclass == PC_SCIENCE )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_PHASER );
	//			client->ps.ammo[WP_PHASER] = PHASER_AMMO_MAX;
	//		}
	//		if ( rpg_alienflags.integer & 16 && pclass == PC_ALIEN
	//			|| rpg_securityflags.integer & 16 && pclass == PC_SECURITY
	//			|| rpg_medicalflags.integer & 16 && pclass == PC_MEDICAL
	//			|| rpg_commandflags.integer & 16 && pclass == PC_COMMAND
	//			|| rpg_engineerflags.integer & 16 && pclass == PC_ENGINEER
	//			|| rpg_marineflags.integer & 16 && pclass == PC_ALPHAOMEGA22
	//			|| rpg_adminflags.integer & 16 && pclass == PC_ADMIN
	//			|| rpg_scienceflags.integer & 16 && pclass == PC_SCIENCE )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_COMPRESSION_RIFLE );
	//			client->ps.ammo[WP_COMPRESSION_RIFLE] = PHASER_AMMO_MAX;
	//		}
	//		if ( rpg_alienflags.integer & 32 && pclass == PC_ALIEN
	//			|| rpg_securityflags.integer & 32 && pclass == PC_SECURITY
	//			|| rpg_medicalflags.integer & 32 && pclass == PC_MEDICAL
	//			|| rpg_commandflags.integer & 32 && pclass == PC_COMMAND
	//			|| rpg_engineerflags.integer & 32 && pclass == PC_ENGINEER
	//			|| rpg_marineflags.integer & 32 && pclass == PC_ALPHAOMEGA22
	//			|| rpg_adminflags.integer & 32 && pclass == PC_ADMIN
	//			|| rpg_scienceflags.integer & 32 && pclass == PC_SCIENCE )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_NULL_HAND );
	//			client->ps.ammo[WP_NULL_HAND] = PHASER_AMMO_MAX;
	//		}
	//		if ( rpg_alienflags.integer & 64 && pclass == PC_ALIEN
	//			|| rpg_securityflags.integer & 64 && pclass == PC_SECURITY
	//			|| rpg_medicalflags.integer & 64 && pclass == PC_MEDICAL
	//			|| rpg_commandflags.integer & 64 && pclass == PC_COMMAND
	//			|| rpg_engineerflags.integer & 64 && pclass == PC_ENGINEER
	//			|| rpg_marineflags.integer & 64 && pclass == PC_ALPHAOMEGA22
	//			|| rpg_adminflags.integer & 64 && pclass == PC_ADMIN
	//			|| rpg_scienceflags.integer & 64 && pclass == PC_SCIENCE )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_DISRUPTOR ); //STASIS
	//			client->ps.ammo[WP_DISRUPTOR] = PHASER_AMMO_MAX;
	//		}
	//		if ( rpg_alienflags.integer & 128 && pclass == PC_ALIEN
	//			|| rpg_securityflags.integer & 128 && pclass == PC_SECURITY
	//			|| rpg_medicalflags.integer & 128 && pclass == PC_MEDICAL
	//			|| rpg_commandflags.integer & 128 && pclass == PC_COMMAND
	//			|| rpg_engineerflags.integer & 128 && pclass == PC_ENGINEER
	//			|| rpg_marineflags.integer & 128 && pclass == PC_ALPHAOMEGA22
	//			|| rpg_adminflags.integer & 128 && pclass == PC_ADMIN
	//			|| rpg_scienceflags.integer & 128 && pclass == PC_SCIENCE )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_DERMAL_REGEN );
	//			client->ps.ammo[WP_DERMAL_REGEN] = PHASER_AMMO_MAX;
	//		}
	//		if ( rpg_alienflags.integer & 256 && pclass == PC_ALIEN
	//			|| rpg_securityflags.integer & 256 && pclass == PC_SECURITY
	//			|| rpg_medicalflags.integer & 256 && pclass == PC_MEDICAL
	//			|| rpg_commandflags.integer & 256 && pclass == PC_COMMAND
	//			|| rpg_engineerflags.integer & 256 && pclass == PC_ENGINEER
	//			|| rpg_marineflags.integer & 256 && pclass == PC_ALPHAOMEGA22
	//			|| rpg_adminflags.integer & 256 && pclass == PC_ADMIN
	//			|| rpg_scienceflags.integer & 256 && pclass == PC_SCIENCE )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_VOYAGER_HYPO );
	//			client->ps.ammo[WP_VOYAGER_HYPO] = PHASER_AMMO_MAX;
	//		}
	//		if ( rpg_alienflags.integer & 512 && pclass == PC_ALIEN
	//			|| rpg_securityflags.integer & 512 && pclass == PC_SECURITY
	//			|| rpg_medicalflags.integer & 512 && pclass == PC_MEDICAL
	//			|| rpg_commandflags.integer & 512 && pclass == PC_COMMAND
	//			|| rpg_engineerflags.integer & 512 && pclass == PC_ENGINEER
	//			|| rpg_marineflags.integer & 512 && pclass == PC_ALPHAOMEGA22
	//			|| rpg_adminflags.integer & 512 && pclass == PC_ADMIN
	//			|| rpg_scienceflags.integer & 512 && pclass == PC_SCIENCE )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_TOOLKIT );
	//			client->ps.ammo[WP_TOOLKIT] = PHASER_AMMO_MAX;
	//		}
	//		if ( rpg_alienflags.integer & 1024 && pclass == PC_ALIEN
	//			|| rpg_securityflags.integer & 1024 && pclass == PC_SECURITY
	//			|| rpg_medicalflags.integer & 1024 && pclass == PC_MEDICAL
	//			|| rpg_commandflags.integer & 1024 && pclass == PC_COMMAND
	//			|| rpg_engineerflags.integer & 1024 && pclass == PC_ENGINEER
	//			|| rpg_marineflags.integer & 1024 && pclass == PC_ALPHAOMEGA22
	//			|| rpg_adminflags.integer & 1024 && pclass == PC_ADMIN
	//			|| rpg_scienceflags.integer & 1024 && pclass == PC_SCIENCE )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_MEDKIT );
	//			client->ps.ammo[WP_MEDKIT] = PHASER_AMMO_MAX;
	//		}
	//		if ( rpg_alienflags.integer & 2048 && pclass == PC_ALIEN
	//			|| rpg_securityflags.integer & 2048 && pclass == PC_SECURITY
	//			|| rpg_medicalflags.integer & 2048 && pclass == PC_MEDICAL
	//			|| rpg_commandflags.integer & 2048 && pclass == PC_COMMAND
	//			|| rpg_engineerflags.integer & 2048 && pclass == PC_ENGINEER
	//			|| rpg_marineflags.integer & 2048 && pclass == PC_ALPHAOMEGA22
	//			|| rpg_adminflags.integer & 2048 && pclass == PC_ADMIN
	//			|| rpg_scienceflags.integer & 2048 && pclass == PC_SCIENCE )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_TRICORDER );
	//			client->ps.ammo[WP_TRICORDER] = PHASER_AMMO_MAX;
	//		}
	//		if ( rpg_alienflags.integer & 4096 && pclass == PC_ALIEN
	//			|| rpg_securityflags.integer & 4096 && pclass == PC_SECURITY
	//			|| rpg_medicalflags.integer & 4096 && pclass == PC_MEDICAL
	//			|| rpg_commandflags.integer & 4096 && pclass == PC_COMMAND
	//			|| rpg_engineerflags.integer & 4096 && pclass == PC_ENGINEER
	//			|| rpg_marineflags.integer & 4096 && pclass == PC_ALPHAOMEGA22
	//			|| rpg_adminflags.integer & 4096 && pclass == PC_ADMIN
	//			|| rpg_scienceflags.integer & 4096 && pclass == PC_SCIENCE )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_NEUTRINO_PROBE );
	//			client->ps.ammo[WP_NEUTRINO_PROBE] = PHASER_AMMO_MAX;
	//		}
	//		if ( rpg_alienflags.integer & 8192 && pclass == PC_ALIEN
	//			|| rpg_securityflags.integer & 8192 && pclass == PC_SECURITY
	//			|| rpg_medicalflags.integer & 8192 && pclass == PC_MEDICAL
	//			|| rpg_commandflags.integer & 8192 && pclass == PC_COMMAND
	//			|| rpg_engineerflags.integer & 8192 && pclass == PC_ENGINEER
	//			|| rpg_marineflags.integer & 8192 && pclass == PC_ALPHAOMEGA22
	//			|| rpg_adminflags.integer & 8192 && pclass == PC_ADMIN
	//			|| rpg_scienceflags.integer & 8192 && pclass == PC_SCIENCE )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_TR116 );
	//			client->ps.ammo[WP_TR116] = PHASER_AMMO_MAX;
	//		}

	//		
	//	}
	//	else
	//	{
	//		//Tricorder
	//		if ( Bits & 1 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_TRICORDER );
	//			client->ps.ammo[WP_TRICORDER] = PHASER_AMMO_MAX;
	//		}
	//		
	//		//PADD
	//		if ( Bits & 2 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_PADD );
	//			client->ps.ammo[WP_PADD] = PHASER_AMMO_MAX;
	//		}

	//		//Phaser
	//		if ( Bits & 4 && pclass == PC_ADMIN)
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_PHASER );
	//			client->ps.ammo[WP_PHASER] = PHASER_AMMO_MAX;
	//		}

	//		//Phaser Rifle
	//		if ( Bits & 8 && pclass == PC_ADMIN)
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_COMPRESSION_RIFLE );
	//			client->ps.ammo[WP_COMPRESSION_RIFLE] = PHASER_AMMO_MAX;
	//		}

	//		//Alien Disruptor
	//		if ( Bits & 16 && pclass == PC_ADMIN)
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_DISRUPTOR );
	//			client->ps.ammo[WP_DISRUPTOR] = PHASER_AMMO_MAX;
	//		}

	//		//Hypospray
	//		if ( Bits & 32 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_VOYAGER_HYPO );
	//			client->ps.ammo[WP_VOYAGER_HYPO] = PHASER_AMMO_MAX;
	//		}

	//		//Dermal Regenerator
	//		if ( Bits & 64 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_DERMAL_REGEN );
	//			client->ps.ammo[WP_DERMAL_REGEN] = PHASER_AMMO_MAX;
	//		}

	//		//Med Kit
	//		if ( Bits & 128 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_MEDKIT );
	//			client->ps.ammo[WP_MEDKIT] = PHASER_AMMO_MAX;
	//		}

	//		//Neutrino Probe
	//		if ( Bits & 256 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_NEUTRINO_PROBE );
	//			client->ps.ammo[WP_NEUTRINO_PROBE] = PHASER_AMMO_MAX;
	//		}

	//		//Engineering Tool Kit
	//		if ( Bits & 512 )
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_TOOLKIT );
	//			client->ps.ammo[WP_TOOLKIT] = PHASER_AMMO_MAX;
	//		}

	//		//IMOD
	//		if ( Bits & 1024 && pclass == PC_ADMIN)
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_NULL_HAND );
	//			client->ps.ammo[WP_NULL_HAND] = PHASER_AMMO_MAX;
	//		}

	//		//Scavenger Rifle
	//		if ( Bits & 2048 && pclass == PC_ADMIN)
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_COFFEE );
	//			client->ps.ammo[WP_COFFEE] = PHASER_AMMO_MAX;
	//		}

	//		//Photon Torpedo Launcher
	//		if ( Bits & 4096 && pclass == PC_ADMIN)
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_QUANTUM_BURST );
	//			client->ps.ammo[WP_QUANTUM_BURST] = PHASER_AMMO_MAX;
	//		}

	//		//TR-116
	//		if ( Bits & 8192 && pclass == PC_ADMIN)
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_TR116 );
	//			client->ps.ammo[WP_TR116] = PHASER_AMMO_MAX;
	//		}

	//		//Admin Gun
	//		if ( Bits & 16384 && pclass == PC_ADMIN)
	//		{
	//			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_GRENADE_LAUNCHER );
	//			client->ps.ammo[WP_GRENADE_LAUNCHER] = PHASER_AMMO_MAX;
	//		}
	//	}
	//	break;
	///*case PC_ACTIONHERO:
	//	client->ps.stats[STAT_WEAPONS] = ( 1 << WP_PHASER );
	//	client->ps.ammo[WP_PHASER] = PHASER_AMMO_MAX;
	//	client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_COMPRESSION_RIFLE );
	//	client->ps.ammo[WP_COMPRESSION_RIFLE] = Max_Ammo[WP_COMPRESSION_RIFLE];
	//	client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_NULL_HAND );
	//	client->ps.ammo[WP_NULL_HAND] = Max_Ammo[WP_NULL_HAND];
	//	client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_COFFEE );
	//	client->ps.ammo[WP_COFFEE] = Max_Ammo[WP_COFFEE];
	//	client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_DISRUPTOR );
	//	client->ps.ammo[WP_DISRUPTOR] = Max_Ammo[WP_DISRUPTOR];
	//	client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_GRENADE_LAUNCHER );
	//	client->ps.ammo[WP_GRENADE_LAUNCHER] = Max_Ammo[WP_GRENADE_LAUNCHER];
	//	client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_TR116 );
	//	client->ps.ammo[WP_TR116] = Max_Ammo[WP_TR116];
	//	client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_QUANTUM_BURST );
	//	client->ps.ammo[WP_QUANTUM_BURST] = Max_Ammo[WP_QUANTUM_BURST];
	//	client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_DERMAL_REGEN );
	//	client->ps.ammo[WP_DERMAL_REGEN] = Max_Ammo[WP_DERMAL_REGEN];
	//	break;
	//case PC_BORG:
	//	// assimilator
	//	client->ps.stats[STAT_WEAPONS] = ( 1 << WP_TOOLKIT );
	//	client->ps.ammo[WP_TOOLKIT] = Max_Ammo[WP_TOOLKIT];
	//	if ( client->ps.clientNum != borgQueenClientNum )
	//	{
	//		// projectile/shock weapon
	//	 	client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_MEDKIT );
	//		client->ps.ammo[WP_MEDKIT] = Max_Ammo[WP_MEDKIT];
	//	}
	//	break;*/
	//case PC_NOCLASS:
	//default:
	//	if ( rpg_rpg.integer != 0 )
	//	{
	//		//client->ps.stats[STAT_WEAPONS] = ( 1 << WP_PADD );
	//		//client->ps.ammo[WP_PADD] = PHASER_AMMO_MAX;

	//		client->ps.stats[STAT_WEAPONS] = ( 1 << WP_NULL_HAND );
	//		client->ps.ammo[WP_NULL_HAND] = PHASER_AMMO_MAX;
	//		if ( rpg_noweapons.integer == 0 )
	//		{
	//			if ( rpg_rpg.integer == 2 )
	//			{
	//				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_PHASER );
	//				client->ps.ammo[WP_PHASER] = PHASER_AMMO_MAX;
	//				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_TRICORDER );
	//				client->ps.ammo[WP_TRICORDER] = PHASER_AMMO_MAX;
	//			}
	//			if ( rpg_rpg.integer == 3 )
	//			{
	//				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_VOYAGER_HYPO );
	//				client->ps.ammo[WP_VOYAGER_HYPO] = PHASER_AMMO_MAX;
	//				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_TRICORDER );
	//				client->ps.ammo[WP_TRICORDER] = PHASER_AMMO_MAX;
	//			}
	//			if ( rpg_rpg.integer == 4 )
	//			{
	//				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_PHASER );
	//				client->ps.ammo[WP_PHASER] = PHASER_AMMO_MAX;
	//				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_VOYAGER_HYPO );
	//				client->ps.ammo[WP_VOYAGER_HYPO] = PHASER_AMMO_MAX;
	//				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_TRICORDER );
	//				client->ps.ammo[WP_TRICORDER] = PHASER_AMMO_MAX;
	//			}
	//			if ( rpg_rpg.integer == 5 )
	//			{
	//				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_PHASER );
	//				client->ps.ammo[WP_PHASER] = PHASER_AMMO_MAX;
	//				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_COMPRESSION_RIFLE );
	//				client->ps.ammo[WP_COMPRESSION_RIFLE] = 200;
	//				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_VOYAGER_HYPO );
	//				client->ps.ammo[WP_VOYAGER_HYPO] = PHASER_AMMO_MAX;
	//				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_TRICORDER );
	//				client->ps.ammo[WP_TRICORDER] = PHASER_AMMO_MAX;
	//			}
	//		}
	//	}
	//	else
	//	{	//TiM: Tricorder... I think
	//		client->ps.stats[STAT_WEAPONS] = ( 1 << WP_TRICORDER );
	//		client->ps.ammo[WP_TRICORDER] = PHASER_AMMO_MAX; //13
	//	}
	//	break;
	//}
}

//void ClientArmorForClass ( gclient_t *client, pclass_t pclass )
//{
//	switch ( pclass )
//	{
//	case PC_INFILTRATOR:
//		client->ps.stats[STAT_ARMOR] = 0;
//		break;
//	case PC_SNIPER:
//		client->ps.stats[STAT_ARMOR] = 25;
//		break;
//	case PC_HEAVY:
//		client->ps.stats[STAT_ARMOR] = 100;
//		break;
//	case PC_DEMO:
//		client->ps.stats[STAT_ARMOR] = 50;
//		break;
//	case PC_MEDIC://note: can also give health & armor & regen
//		client->ps.stats[STAT_ARMOR] = 75;
//		break;
//	case PC_TECH://note: can also give ammo & invis
//		client->ps.stats[STAT_ARMOR] = 50;
//		break;
//	case PC_ACTIONHERO:
//		client->ps.stats[STAT_ARMOR] = 100;
//		break;
//	case PC_BORG:
//		client->ps.stats[STAT_ARMOR] = 100;
//		break;
//	case PC_ALPHAOMEGA22:
//		client->ps.stats[STAT_ARMOR] = 0;
//		break;
//	case PC_N00B:
//		client->ps.stats[STAT_ARMOR] = 0;
//		break;
//	case PC_NOCLASS:
//	default:
//		break;
//	}
//}

void ClientHoldablesForClass ( gclient_t *client, pclass_t pclass )
{
	if ( g_classData[pclass].isMarine )
		client->ps.stats[STAT_HOLDABLE_ITEM] = BG_FindItemForHoldable( HI_TRANSPORTER ) - bg_itemlist;

	else if ( g_classData[pclass].isAdmin )
		client->ps.stats[STAT_HOLDABLE_ITEM] = BG_FindItemForHoldable( HI_SHIELD ) - bg_itemlist;

	/*switch ( pclass )
	{
	case PC_INFILTRATOR:
	case PC_SNIPER:
	case PC_HEAVY:
	case PC_DEMO:
	case PC_MEDIC:
	case PC_TECH:
		break;
	case PC_ACTIONHERO:
		break;
	case PC_BORG:
		break;
	case PC_ALPHAOMEGA22:
		client->ps.stats[STAT_HOLDABLE_ITEM] = BG_FindItemForHoldable( HI_TRANSPORTER ) - bg_itemlist;
		break;
	case PC_ADMIN:
		client->ps.stats[STAT_HOLDABLE_ITEM] = BG_FindItemForHoldable( HI_SHIELD ) - bg_itemlist;
		break;
	case PC_NOCLASS:
	default:
		break;
	}*/
}

//void ClientPowerupsForClass ( gentity_t *ent, pclass_t pclass )
//{
//	switch ( pclass )
//	{
//	case PC_INFILTRATOR:
//	case PC_SNIPER:
//	case PC_HEAVY:
//	case PC_DEMO:
//	case PC_MEDIC:
//	case PC_TECH:
//	case PC_ACTIONHERO:
//	case PC_BORG:
//	case PC_NOCLASS:
//	default:
//		break;
//	}
//}

void G_StoreClientInitialStatus( gentity_t *ent )
{
	char	userinfo[MAX_INFO_STRING];

	if ( clientInitialStatus[ent->s.number].initialized )
	{//already set
		return;
	}

	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR )
	{//don't store their data if they're just a spectator
		return;
	}

	trap_GetUserinfo( ent->s.number, userinfo, sizeof( userinfo ) );
	Q_strncpyz( clientInitialStatus[ent->s.number].model, Info_ValueForKey (userinfo, "model"), sizeof( clientInitialStatus[ent->s.number].model ) );
	clientInitialStatus[ent->s.number].pClass = ent->client->sess.sessionClass;
	clientInitialStatus[ent->s.number].team = ent->client->sess.sessionTeam;
	clientInitialStatus[ent->s.number].initialized = qtrue;
	ent->client->classChangeDebounceTime = 0;
}

void G_RestoreClientInitialStatus( gentity_t *ent )
{
	char	userinfo[MAX_INFO_STRING];
	clientSession_t *sess;

	if ( !clientInitialStatus[ent->s.number].initialized )
	{//not set
		return;
	}

	sess = &ent->client->sess;

	trap_GetUserinfo( ent->s.number, userinfo, sizeof( userinfo ) );

	if ( clientInitialStatus[ent->s.number].team != sess->sessionTeam &&
		 clientInitialStatus[ent->s.number].pClass != sess->sessionClass )
	{
		SetClass( ent, ClassNameForValue( clientInitialStatus[ent->s.number].pClass ), (char *)TeamName( clientInitialStatus[ent->s.number].team ), qtrue );
	}
	else
	{
		if ( clientInitialStatus[ent->s.number].pClass != sess->sessionClass )
		{
			SetClass( ent, ClassNameForValue( clientInitialStatus[ent->s.number].pClass ), NULL, qtrue);
		}
		if ( clientInitialStatus[ent->s.number].team != sess->sessionTeam )
		{
			SetTeam( ent, (char *)TeamName( clientInitialStatus[ent->s.number].team ) );
		}
	}
	if ( Q_stricmp( clientInitialStatus[ent->s.number].model, Info_ValueForKey (userinfo, "model") ) != 0 )
	{//restore the model
		Info_SetValueForKey( userinfo, "model", clientInitialStatus[ent->s.number].model );
		trap_SetUserinfo( ent->s.number, userinfo );
		//FIXME: send this to everyone or let it automatically do it when the map restarts?
		//trap_SetConfigstring( CS_PLAYERS+ent->s.number, s );
	}
}

/*
===========
ClientSpawn

Called every time a client is placed fresh in the world:
after the first ClientBegin, and after each respawn
Initializes all non-persistant parts of playerState
------------------------------------
Modifyed By: RedTechie
And also by Marcin - 30/12/2008
============
*/
void ClientSpawn(gentity_t *ent, int rpgx_spawn, qboolean fromDeath ) {
	int		index=0;
	vec3_t	spawn_origin, spawn_angles;
	gclient_t	*client=NULL;
	int		i=0;
	clientPersistant_t	saved;
	clientSession_t		savedSess;
	int		persistant[MAX_PERSISTANT];
	gentity_t	*spawnPoint=NULL;
	int		flags = 0;
	int		savedPing;
	pclass_t	pClass = 0;//PC_NOCLASS;
	int		cCDT = 0;
	int clientNum;

	index = ent - g_entities;
	client = ent->client;
	clientNum = ent->client->ps.clientNum;

	/*
	if ( actionHeroClientNum == -1 )
	{
		G_RandomActionHero( -1 );
	}

	G_RandomBorgQueen( -1 );
	*/

	// find a spawn point
	// do it before setting health back up, so farthest
	// ranging doesn't count this client
	if(rpgx_spawn != 1){//RPG-X: RedTechie - Make sure the spawn is regular spawn or spawn at current position (rpgx_spawn = current possition)
		if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
			spawnPoint = SelectSpectatorSpawnPoint (
				spawn_origin, spawn_angles);
		} else if (g_gametype.integer >= GT_TEAM) {
			spawnPoint = SelectCTFSpawnPoint (
				ent,
				client->sess.sessionTeam,
				client->pers.teamState.state,
				spawn_origin, spawn_angles);
		} else {
			do {
				// the first spawn should be at a good looking spot
				if ( !client->pers.initialSpawn && client->pers.localClient ) {
					client->pers.initialSpawn = qtrue;
					spawnPoint = SelectInitialSpawnPoint( spawn_origin, spawn_angles );
				} else {
					// don't spawn near existing origin if possible
					spawnPoint = SelectSpawnPoint (
						client->ps.origin,
						spawn_origin, spawn_angles);
				}

				// Tim needs to prevent bots from spawning at the initial point
				// on q3dm0...
				if ( ( spawnPoint->flags & FL_NO_BOTS ) && ( ent->r.svFlags & SVF_BOT ) ) {
					continue;	// try again
				}
				// just to be symetric, we have a nohumans option...
				if ( ( spawnPoint->flags & FL_NO_HUMANS ) && !( ent->r.svFlags & SVF_BOT ) ) {
					continue;	// try again
				}

				break;

			} while ( 1 );
		}
	}//RPG-X: RedTechie - End rpgx_spawn check
	client->pers.teamState.state = TEAM_ACTIVE;

	// toggle the teleport bit so the client knows to not lerp
	if(rpgx_spawn != 1){
		flags = ent->client->ps.eFlags & EF_TELEPORT_BIT;
		flags ^= EF_TELEPORT_BIT;
	}

	// clear everything but the persistant data

	saved = client->pers;
	savedSess = client->sess;
	savedPing = client->ps.ping;
	for ( i = 0 ; i < MAX_PERSISTANT ; i++ ) {
		persistant[i] = client->ps.persistant[i];
	}
	//okay, this is hacky, but we need to keep track of this, even if uninitialized first time you spawn, it will be stomped anyway
	//RPG-X: RedTechie - Damn thing screwed my function up
	if(rpgx_spawn != 1){
		if ( client->classChangeDebounceTime )
		{
			cCDT = client->classChangeDebounceTime;
		}
		memset (client, 0, sizeof(*client));
		client->classChangeDebounceTime = cCDT;
	}
	//
	client->pers = saved;
	client->sess = savedSess;
	client->ps.ping = savedPing;
	for ( i = 0 ; i < MAX_PERSISTANT ; i++ ) {
		client->ps.persistant[i] = persistant[i];
	}

	// increment the spawncount so the client will detect the respawn
	if(rpgx_spawn != 1){
		client->ps.persistant[PERS_SPAWN_COUNT]++;
		client->airOutTime = level.time + 12000;
	}

	if(client->sess.sessionTeam != TEAM_SPECTATOR){
		client->sess.sessionTeam = TEAM_FREE;
	}
	client->ps.persistant[PERS_TEAM] = client->sess.sessionTeam;
	
	// clear entity values
	client->ps.stats[STAT_MAX_HEALTH] = client->pers.maxHealth;
	//if(rpgx_spawn != 1){
		client->ps.eFlags = flags;
	//}
	client->streakCount = 0;
	
	ent->client->ps.pm_type = PM_NORMAL;
	ent->s.groundEntityNum = ENTITYNUM_NONE;
	ent->client = &level.clients[index];
	ent->takedamage = qtrue;
	ent->inuse = qtrue;
	ent->classname = "player";
	
	//RPG-X | Phenix | 13/02/2005
	/*if (rpgx_spawn != 1) {
		ent->r.contents = CONTENTS_BODY;
	} else {
		ent->r.contents = CONTENTS_CORPSE;
	}*/

	ent->r.contents = CONTENTS_BODY;

	ent->clipmask = MASK_PLAYERSOLID;
	ent->die = player_die;
	ent->waterlevel = 0;
	ent->watertype = 0;
	ent->flags = 0;

	if(rpgx_spawn != 1){
		VectorCopy (playerMins, ent->r.mins);
		VectorCopy (playerMaxs, ent->r.maxs);
	}

	client->ps.clientNum = index;

	// health will count down towards max_health
	//if(rpgx_spawn != 1){
	ent->health = client->ps.stats[STAT_HEALTH] = client->ps.stats[STAT_MAX_HEALTH] * 1.25;
	//}
	//RPG-X: RedTechie - No armor
	// Start with a small amount of armor as well.
	//client->ps.stats[STAT_ARMOR] = client->ps.stats[STAT_MAX_HEALTH] * 0.25;

	//if ( !g_pModAssimilation.integer && client->sess.sessionClass == PC_BORG )
	//{
	//	client->sess.sessionClass = PC_NOCLASS;
	//	ClientUserinfoChanged( client->ps.clientNum );
	//}
	//if ( !g_pModActionHero.integer && client->sess.sessionClass == PC_ACTIONHERO )
	//{
	//	client->sess.sessionClass = PC_NOCLASS;
	//	ClientUserinfoChanged( client->ps.clientNum );
	//}
	/*if ( rpg_rpg.integer == 0 || rpg_rpg.integer != 0 && rpg_norpgclasses.integer != 0 )
	{
		if ( client->sess.sessionClass == PC_ADMIN || client->sess.sessionClass == PC_ALPHAOMEGA22 || client->sess.sessionClass == PC_ALIEN || client->sess.sessionClass == PC_COMMAND || client->sess.sessionClass == PC_SECURITY || client->sess.sessionClass == PC_MEDICAL || client->sess.sessionClass == PC_SCIENCE || client->sess.sessionClass == PC_ENGINEER || client->sess.sessionClass == PC_N00B )
		{
			client->sess.sessionClass = PC_NOCLASS;
			ClientUserinfoChanged( client->ps.clientNum );
		}
	}
	if ( rpg_rpg.integer != 0 && rpg_nosecurity.integer != 0 )
	{
		if ( client->sess.sessionClass == PC_SECURITY )
		{
			client->sess.sessionClass = PC_NOCLASS;
			ClientUserinfoChanged( client->ps.clientNum );
		}
	}
	if ( rpg_rpg.integer != 0 && rpg_nomaker.integer != 0 )
	{
		if ( client->sess.sessionClass == PC_ADMIN )
		{
			client->sess.sessionClass = PC_NOCLASS;
			ClientUserinfoChanged( client->ps.clientNum );
		}
	}
	if ( rpg_rpg.integer != 0 && rpg_nomarine.integer != 0 )
	{
		if ( client->sess.sessionClass == PC_ALPHAOMEGA22 )
		{
			client->sess.sessionClass = PC_NOCLASS;
			ClientUserinfoChanged( client->ps.clientNum );
		}
	}
	if ( rpg_rpg.integer != 0 && rpg_noalien.integer != 0 )
	{
		if ( client->sess.sessionClass == PC_ALIEN )
		{
			client->sess.sessionClass = PC_NOCLASS;
			ClientUserinfoChanged( client->ps.clientNum );
		}
	}
	if ( rpg_rpg.integer != 0 && rpg_nomedical.integer != 0 )
	{
		if ( client->sess.sessionClass == PC_MEDICAL )
		{
			client->sess.sessionClass = PC_NOCLASS;
			ClientUserinfoChanged( client->ps.clientNum );
		}
	}
	if ( rpg_rpg.integer != 0 && rpg_nocommand.integer != 0 )
	{
		if ( client->sess.sessionClass == PC_COMMAND )
		{
			client->sess.sessionClass = PC_NOCLASS;
			ClientUserinfoChanged( client->ps.clientNum );
		}
	}
	if ( rpg_rpg.integer != 0 && rpg_noscience.integer != 0 )
	{
		if ( client->sess.sessionClass == PC_SCIENCE )
		{
			client->sess.sessionClass = PC_NOCLASS;
			ClientUserinfoChanged( client->ps.clientNum );
		}
	}
	if ( rpg_rpg.integer != 0 && rpg_noengineer.integer != 0 )
	{
		if ( client->sess.sessionClass == PC_ENGINEER )
		{
			client->sess.sessionClass = PC_NOCLASS;
			ClientUserinfoChanged( client->ps.clientNum );
		}
	}
	if ( rpg_rpg.integer != 0 && rpg_non00b.integer != 0 )
	{
		if ( client->sess.sessionClass == PC_N00B )
		{
			client->sess.sessionClass = PC_NOCLASS;
			ClientUserinfoChanged( client->ps.clientNum );
		}
	}*/

	if ( g_pModDisintegration.integer != 0 )
	{//this is instagib
		client->ps.stats[STAT_WEAPONS] = ( 1 << WP_COMPRESSION_RIFLE );
		client->ps.ammo[WP_COMPRESSION_RIFLE] = Max_Ammo[WP_COMPRESSION_RIFLE];
	}
	else
	{
		pclass_t oClass = client->sess.sessionClass;
		if ( g_pModSpecialties.integer != 0 )
		{
			/*if ( client->sess.sessionClass == PC_NOCLASS )
			{
				client->sess.sessionClass = irandom( PC_SNIPER, PC_TECH );
				SetPlayerClassCvar(ent);
			}*/
		}
		//else if ( g_pModActionHero.integer != 0 )
		//{
		//	if ( ent->s.number == actionHeroClientNum )
		//	{
		//		client->sess.sessionClass = PC_ACTIONHERO;
		//		BroadcastClassChange( client, PC_NOCLASS );
		//	}
		//	else if ( client->sess.sessionClass == PC_ACTIONHERO )
		//	{//make sure to take action hero away from previous one
		//		client->sess.sessionClass = PC_NOCLASS;
		//	}
		//}
		//else if ( client->sess.sessionClass != PC_BORG )
		//{
		//	if ( rpg_rpg.integer == 0 || client->sess.sessionClass != PC_SECURITY
		//		&& client->sess.sessionClass != PC_MEDICAL
		//		&& client->sess.sessionClass != PC_ADMIN
		//		&& client->sess.sessionClass != PC_COMMAND
		//		&& client->sess.sessionClass != PC_ENGINEER
		//		&& client->sess.sessionClass != PC_SCIENCE
		//		&& client->sess.sessionClass != PC_ALIEN
		//		&& client->sess.sessionClass != PC_ALPHAOMEGA22
		//		&& client->sess.sessionClass != PC_N00B
		//		)
		//	{
		//		client->sess.sessionClass = PC_NOCLASS;
		//	}
		//}

		if ( oClass != client->sess.sessionClass )
		{//need to send the class change
			ClientUserinfoChanged( client->ps.clientNum );
		}

		client->ps.persistant[PERS_CLASS] = client->sess.sessionClass;
		pClass = client->sess.sessionClass;

		//ClientMaxHealthForClass( client, pClass );
		if ( pClass != 0/*PC_NOCLASS*/ )
		{//no health boost on spawn for playerclasses
			ent->health = client->ps.stats[STAT_HEALTH] = client->ps.stats[STAT_MAX_HEALTH] = client->pers.maxHealth;
		}
		
		if ( !fromDeath || !rpg_dropOnDeath.integer || !rpg_allowWeaponDrop.integer ) {
			ClientWeaponsForClass( client, pClass );
		} else { // Marcin: just a hand
			ClientWeaponsForClass( client, 0 );
		}
		//ClientArmorForClass( client, pClass );
		ClientHoldablesForClass( client, pClass );
		//ClientPowerupsForClass( ent, pClass );
	}
	
	if(rpgx_spawn != 1){
		G_SetOrigin( ent, spawn_origin );
		VectorCopy( spawn_origin, client->ps.origin );
	}

	// the respawned flag will be cleared after the attack and jump keys come up
	if(rpgx_spawn != 1){
		client->ps.pm_flags |= PMF_RESPAWNED;
	}

	trap_GetUsercmd( client - level.clients, &ent->client->pers.cmd );
	if(rpgx_spawn != 1){
		SetClientViewAngle( ent, spawn_angles );
	}
	
	if(rpgx_spawn != 1){
		if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR /*|| (ent->client->ps.eFlags&EF_ELIMINATED)*/ ) {

		} else {
			G_MoveBox( ent );
			trap_LinkEntity (ent);

			// force the base weapon up
			client->ps.weapon = WP_NULL_HAND; //TiM: WP_PHASER
			client->ps.weaponstate = WEAPON_READY;

		}
	}/*else{
		G_MoveBox( ent );
		trap_LinkEntity (ent);
	}*/

	// don't allow full run speed for a bit
	client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
	client->ps.pm_time = 100;

	if(rpgx_spawn != 1){
		client->respawnTime = level.time;
	}

	client->inactivityTime = level.time + g_inactivity.integer * 1000;
	client->latched_buttons = 0;

	// set default animations
	//client->ps.torsoAnim = BOTH_STAND1; //TORSO_STAND
	//client->ps.legsAnim = BOTH_STAND1;
	if (rpgx_spawn != 1 ) {
		client->ps.stats[TORSOANIM] = BOTH_STAND1;
		client->ps.stats[LEGSANIM] = BOTH_STAND1;
	}

	if ( level.intermissiontime ) {
		MoveClientToIntermission( ent );
	} else {
		// fire the targets of the spawn point
		if(rpgx_spawn != 1){
			G_UseTargets( spawnPoint, ent );
		}

		// select the highest weapon number available, after any
		// spawn given items have fired
		//if(rpgx_spawn != 1){
		client->ps.weapon = 1;
		/*for ( i = WP_NUM_WEAPONS - 1 ; i > 0 ; i-- ) {
			if ( client->ps.stats[STAT_WEAPONS] & ( 1 << i ) ) {
				client->ps.weapon = i;
				break;
			}
		}*/

		//TiM - Always default to the null hand
		client->ps.weapon = WP_NULL_HAND;
		//}
	}

	// run a client frame to drop exactly to the floor,
	// initialize animations and other things
	client->ps.commandTime = level.time - 100;
	ent->client->pers.cmd.serverTime = level.time;
	ClientThink( ent-g_entities );

	// positively link the client, even if the command times are weird
	if ( ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
		BG_PlayerStateToEntityState( &client->ps, &ent->s, qtrue );
		if(rpgx_spawn != 1){
			VectorCopy( ent->client->ps.origin, ent->r.currentOrigin );
		}
		trap_LinkEntity( ent );
		
	}

	// run the presend to set anything else
	ClientEndFrame( ent );

	// clear entity state values
	BG_PlayerStateToEntityState( &client->ps, &ent->s, qtrue );

	//start-up messages
	//FIXME: externalize all this text!
	//FIXME: make the gametype titles be graphics!
	//FIXME: make it do this on a map_restart also
	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR )
	{//spectators just get the title of the game
		if ( g_pModAssimilation.integer )
		{
			trap_SendServerCommand( ent-g_entities, "cp \"Assimilation\"" );
		}
		/*else if ( g_pModElimination.integer )
		{
			trap_SendServerCommand( ent-g_entities, "cp \"Elimination\"" );
		}*/
		else
		{
			switch ( g_gametype.integer )
			{
			case GT_FFA:				// free for all
				trap_SendServerCommand( ent-g_entities, va("cp \"%s\"", rpg_welcomemessage.string )  );
				break;
			case GT_TOURNAMENT:		// one on one tournament
				trap_SendServerCommand( ent-g_entities, va("cp \"%s\"", rpg_welcomemessage.string )  );
				break;
			case GT_SINGLE_PLAYER:	// single player tournament
				trap_SendServerCommand( ent-g_entities, va("cp \"%s\"", rpg_welcomemessage.string )  );
				break;
			case GT_TEAM:			// team deathmatch
				trap_SendServerCommand( ent-g_entities, va("cp \"%s\"", rpg_welcomemessage.string )  );
				break;
			case GT_CTF:				// capture the flag
				trap_SendServerCommand( ent-g_entities, va("cp \"%s\"", rpg_welcomemessage.string )  );
				break;
			}
		}
	}
	/*else if ( g_pModAssimilation.integer )
	{
		if ( !clientInitialStatus[ent->s.number].initialized )
		{//first time coming in
			if ( ent->client->sess.sessionClass == PC_BORG )
			{
				trap_SendServerCommand( ent-g_entities, "cp \"^3Assimilation:^7\nAssimilate all enemies!\n\"" );
			}
			else if ( ent->s.number != borgQueenClientNum )
			{
				trap_SendServerCommand( ent-g_entities, "cp \"^3Assimilation:^7\nKill the Borg Queen!\n\"" );
			}
		}
		else
		{
			//make sure I'm marked as assimilated if I was
			if ( clientInitialStatus[ent->s.number].pClass != PC_BORG && client->sess.sessionClass == PC_BORG )
			{
				//now mark them assimilated
				ent->s.eFlags |= EF_ASSIMILATED;
				ent->client->ps.eFlags |= EF_ASSIMILATED;
			}
		}
		//send me a message if I'm the queen
		if ( ent->s.number == borgQueenClientNum )
		{
			trap_SendServerCommand( ent->s.number, "cp \"^3Assimilation:^7\nYou Are the Queen!\n\"" );
			//FIXME: precache
			//G_Sound( ent, G_SoundIndex( "sound/voice/computer/misc/borgqueen.wav" ) );
		}
	}*/
	else
	{
		if ( g_pModElimination.integer )
		{
			if ( !clientInitialStatus[ent->s.number].initialized )
			{//first time coming in
				trap_SendServerCommand( ent-g_entities, "cp \"Elimination\"" );
			}
		}
		else
		{
			if ( !clientInitialStatus[ent->s.number].initialized )
			{//first time coming in
				switch ( g_gametype.integer )
				{
				case GT_FFA:				// free for all
					trap_SendServerCommand( ent-g_entities, va("cp \"%s\"", rpg_welcomemessage.string ) );
					break;
				case GT_TOURNAMENT:		// one on one tournament
					trap_SendServerCommand( ent-g_entities, va("cp \"%s\"", rpg_welcomemessage.string )  );
					break;
				case GT_SINGLE_PLAYER:	// single player tournament
					trap_SendServerCommand( ent-g_entities, va("cp \"%s\"", rpg_welcomemessage.string )  );
					break;
				case GT_TEAM:			// team deathmatch
					trap_SendServerCommand( ent-g_entities, va("cp \"%s\"", rpg_welcomemessage.string )  );
					break;
				case GT_CTF:				// capture the flag
					trap_SendServerCommand( ent-g_entities, va("cp \"%s\"", rpg_welcomemessage.string )  );
					break;
				}
				if ( level.numObjectives > 0 )
				{//Turn on their objectives
					//trap_SendServerCommand( ent-g_entities, "+analysis" );
					//FIXME: turn this off after warm-up period
				}
			}
			if ( ent->s.number == actionHeroClientNum )
			{
				trap_SendServerCommand( ent->s.number, "cp \"You are the Action Hero!\"" );
			}
			else if ( actionHeroClientNum > -1 )
			{//FIXME: this will make it so that those who spawn before the action hero won't be told who he is
				if ( !clientInitialStatus[ent->s.number].initialized )
				{//first time coming in
					gentity_t *aH = &g_entities[actionHeroClientNum];
					if ( aH != NULL && aH->client != NULL && aH->client->pers.netname[0] != 0 )
					{
						trap_SendServerCommand( ent->s.number, va("cp \"Action Hero is %s!\"", aH->client->pers.netname) );
					}
					else
					{
						trap_SendServerCommand( ent->s.number, "cp \"Action Hero!\"" );
					}
				}
			}
		}
	}

	if(rpgx_spawn) {
		if ( client->sess.sessionTeam == TEAM_SPECTATOR || 
				g_classData[client->sess.sessionClass].isMedical || 
				g_classData[client->sess.sessionClass].isAdmin )
		{
			int			l;
			char		entry[16];
			char		command[1024];
			int			numPlayers;
			gentity_t	*player;
			int			len;
			int			cmdLen=0;

			numPlayers = 0;
			command[0] = '\0';

			for ( l=0; l<g_maxclients.integer; l++ )
			{
				player = g_entities + l;

				if ( player == ent || !player->inuse )
					continue;
				
				Com_sprintf( entry, sizeof(entry), " %i %i", l, player->health >= 0 ? player->health : 0 );
				len = strlen( entry );
				if ( cmdLen + len > sizeof( command ) )
					break;
				strcpy( command + cmdLen, entry );
				cmdLen += len;

				numPlayers++;
			}

			if ( numPlayers > 0 )
				trap_SendServerCommand( clientNum, va("hinfo %i%s", numPlayers, command) );
		}
	}

	//store intial client values
	//FIXME: when purposely change teams, this gets confused?

	G_StoreClientInitialStatus( ent );

	//RPG-X: Marcin: stuff was here previously - 22/12/2008
}

/*static gentity_t *SpawnBeamOutPlayer( gentity_t *ent ) {
	gentity_t	*body;
	//vec3_t		vec;
	//vec3_t		f, r, u;

	body = G_Spawn();
	if ( !body ) {
		G_Printf( S_COLOR_RED "ERROR: out of gentities\n" );
		return NULL;
	}

	body->classname = ent->client->pers.netname;
	body->client = ent->client;
	body->s = ent->s;
	body->s.eType = ET_PLAYER;		// could be ET_INVISIBLE
	body->s.eFlags = ent->s.eFlags;	// clear EF_TALK, etc
	body->s.powerups = ent->s.powerups;
	body->s.loopSound = 0;			
	//body->s.number = body - g_entities;
	body->s.number = ent->client->ps.clientNum;
	body->timestamp = level.time;
	body->physicsObject = qtrue;
	body->physicsBounce = 0;		// don't bounce
	body->s.event = 0;
	body->s.pos.trType = TR_STATIONARY;
	body->s.groundEntityNum = ENTITYNUM_WORLD;
	body->s.legsAnim = ent->client->ps.stats[LEGSANIM]; //TORSO_STAND
	body->s.torsoAnim = ent->client->ps.stats[TORSOANIM];
	body->s.weapon = ent->s.weapon;

	// fix up some weapon holding / shooting issues
	//if (body->s.weapon==WP_PHASER || body->s.weapon==WP_DERMAL_REGEN || body->s.weapon == WP_NONE )
	//	body->s.weapon = WP_COMPRESSION_RIFLE;

	body->s.event = 0;
	body->r.svFlags = ent->r.svFlags;
	VectorCopy (ent->r.mins, body->r.mins);
	VectorCopy (ent->r.maxs, body->r.maxs);
	VectorCopy (ent->r.absmin, body->r.absmin);
	VectorCopy (ent->r.absmax, body->r.absmax);
	body->clipmask = CONTENTS_SOLID | CONTENTS_PLAYERCLIP;
	body->r.contents = CONTENTS_BODY;
	body->r.ownerNum = ent->r.ownerNum;
	body->takedamage = qfalse;
	//VectorSubtract( level.intermission_origin, pad->r.currentOrigin, vec );
	//vectoangles( vec, body->s.apos.trBase );
	//VectorCopy( ent->s.apos.trBase, body->s.apos.trBase );
	//body->s.apos.trBase[PITCH] = 0;
	//body->s.apos.trBase[ROLL] = 0;

	AngleVectors( body->s.apos.trBase, f, r, u );
	VectorMA( pad->r.currentOrigin, offset[0], f, vec );
	VectorMA( vec, offset[1], r, vec );
	VectorMA( vec, offset[2], u, vec );

	G_SetOrigin( body, ent->s.apos.trBase );
	VectorCopy( ent->

	body->s.apos = ent->s.apos;

	trap_LinkEntity (body);

	//body->count = place;

	return body;
}*/

gentity_t *SpawnBeamOutPlayer( gentity_t	*ent ) {
		gentity_t	*body;
	
		body = G_Spawn();
		body->physicsBounce = 0.0f;//bodys are *not* bouncy
		//VectorMA(ent->client->ps.origin, detDistance + mins[0], fwd, body->s.origin);
		VectorCopy( ent->client->ps.origin, body->s.origin );
		body->r.mins[2] = -24;//keep it off the floor
		//VectorNegate(fwd, fwd);					// ???  What does this do??
		//vectoangles(fwd, body->s.angles);
		VectorCopy( ent->client->ps.viewangles, body->s.angles );
		body->s.clientNum = ent->client->ps.clientNum;

		//--------------------------- SPECIALIZED body SETUP
		//body->think = bodyThink;
		body->count = 12;						// about 1 minute before dissapear
		body->nextthink = level.time + 4000;	// think after 4 seconds
		body->parent = ent;

		(body->s).eType = (ent->s).eType;		// set to type PLAYER
		(body->s).eFlags= (ent->s).eFlags;
		//(body->s).eFlags |= EF_ITEMPLACEHOLDER;// set the HOLOGRAM FLAG to ON

		body->s.weapon = ent->s.weapon;		// get Player's Wepon Type
//		body->s.constantLight = 10 + (10 << 8) + (10 << 16) + (9 << 24);

		//body->s.pos.trBase[2] += 24;			// shift up to adjust origin of body
		body->s.apos = ent->s.apos;			// copy angle of player to body

		//body->s.legsAnim = BOTH_STAND1;			// Just standing TORSO_STAND
		//body->s.torsoAnim = BOTH_STAND1;
		//TiM: Set it's anim to whatever anims we're playing right now
		body->s.legsAnim = ent->client->ps.stats[LEGSANIM];
		body->s.torsoAnim= ent->client->ps.stats[TORSOANIM];

		//--------------------------- WEAPON ADJUST
		if (body->s.weapon==WP_PHASER || body->s.weapon==WP_DERMAL_REGEN)
			body->s.weapon = WP_COMPRESSION_RIFLE;

		return body;
}

/*
===========
ClientDisconnect

Called when a player drops from the server.
Will not be called between levels.

This should NOT be called directly by any game logic,
call trap_DropClient(), which will call this and do
server system housekeeping.
============
*/
void ClientDisconnect( int clientNum ) {
	gentity_t	*ent;
	gentity_t	*tent;

//	gentity_t	*beamPlayer;
	int			i;

	ent = g_entities + clientNum;
	if ( !ent->client ) {
		return;
	}

	// stop any following clients
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].sess.sessionTeam == TEAM_SPECTATOR
			&& level.clients[i].sess.spectatorState == SPECTATOR_FOLLOW
			&& level.clients[i].sess.spectatorClient == clientNum ) {
			StopFollowing( &g_entities[i] );
		}
	}

	//RPG-X: J2J - Stop any dragging.
	DragDat[clientNum].AdminId = -1;
	DragDat[clientNum].distance = 0;
	g_entities[clientNum].client->noclip = qfalse;  

	//TiM: Log the player's IP and name.  If they reconnect again, it'll announce their deceipt >:)
	if ( rpg_renamedPlayers.integer && !(ent->r.svFlags & SVF_BOT) ) {
		int			l;
		qboolean	foundName=qfalse;

		//Do a chek to see if this player has disconnected b4.  else we'll be wasting a slot.
		for ( l = 0; l < MAX_RECON_NAMES; l++ ) {
			if ( !g_reconData[l].ipAddress[0] ) {
				continue;
			}

			if ( !Q_stricmp( ent->client->pers.ip, g_reconData[l].ipAddress ) ) {
				foundName=qtrue;
				break;
			}
		}

		if ( foundName ) {
			memset( &g_reconData[i], 0, sizeof( g_reconData[i] ) );
		
			//IP Address
			Q_strncpyz( g_reconData[i].ipAddress, ent->client->pers.ip, sizeof( g_reconData[i].ipAddress ) );
			//Player Name
			Q_strncpyz( g_reconData[i].previousName, ent->client->pers.netname, sizeof( g_reconData[i].previousName ) );
		
			//G_Printf( "Logging Data IP: %s, Name: %s\n", ent->client->pers.ip, g_reconData[i].previousName);
		}
		else {
			memset( &g_reconData[g_reconNum], 0, sizeof( g_reconData[g_reconNum] ) );

			//IP Address
			Q_strncpyz( g_reconData[g_reconNum].ipAddress, ent->client->pers.ip, sizeof( g_reconData[g_reconNum].ipAddress ) );
			//Player Name
			Q_strncpyz( g_reconData[g_reconNum].previousName, ent->client->pers.netname, sizeof( g_reconData[g_reconNum].previousName ) );
				
			//G_Printf( "Logging Data IP: %s, Name: %s\n", ent->client->pers.ip, g_reconData[g_reconNum].previousName);
				
			g_reconNum++;
			//cap reconNum just in case.
			
			if ( g_reconNum >= MAX_RECON_NAMES ) {
				g_reconNum = 0;
			}
		}
	}


	/*beamPlayer = SpawnBeamOutPlayer( ent );
	if ( beamPlayer ) {
		beamPlayer->nextthink = level.time + 4000;
		//beamPlayer->client->ps.powerups[PW_BEAM_OUT] = level.time + 4000;
		beamPlayer->s.powerups |= ( 1 << PW_BEAM_OUT );
		beamPlayer->think = G_FreeEntity;

		trap_LinkEntity( beamPlayer );
	}*/

	// send effect if they were completely connected
	if ( ent->client->pers.connected == CON_CONNECTED
		&& ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
		vec3_t	org;

		VectorCopy( ent->client->ps.origin, org );
		org[2] += (ent->client->ps.viewheight >> 1);

		tent = G_TempEntity( org, EV_PLAYER_TELEPORT_OUT );
		tent->s.clientNum = ent->s.clientNum;

		// They don't get to take powerups with them!
		// Especially important for stuff like CTF flags
		//ent->client->pers.connected = CON_DISCONNECTING; //RPG-X | GSIO01 | 08/05/2009: ensure that weapons aren't dropped when the client disconnects
		TossClientItems ( ent, qtrue );
		//ent->client->pers.connected = CON_CONNECTED;
	}

	G_LogPrintf( "ClientDisconnect: %i (%s)\n", clientNum, g_entities[clientNum].client->pers.ip );

	// if we are playing in tourney mode and losing, give a win to the other player
	if ( g_gametype.integer == GT_TOURNAMENT && !level.intermissiontime
		&& !level.warmupTime && level.sortedClients[1] == clientNum ) {
		level.clients[ level.sortedClients[0] ].sess.wins++;
		ClientUserinfoChanged( level.sortedClients[0] );
	}

	if ( g_gametype.integer == GT_TOURNAMENT && ent->client->sess.sessionTeam == TEAM_FREE && level.intermissiontime )
	{
		trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
		level.restarted = qtrue;
		level.changemap = NULL;
		level.intermissiontime = 0;
	}

	trap_UnlinkEntity (ent);
	memset( ent, 0, sizeof( ent ) );
	ent->s.modelindex = 0;
	ent->inuse = qfalse;
	ent->classname = "disconnected";
	ent->client->pers.connected = CON_DISCONNECTED;
	ent->client->ps.persistant[PERS_TEAM] = TEAM_FREE;
	ent->client->ps.persistant[PERS_CLASS] = 0;//PC_NOCLASS;
	ent->client->sess.sessionTeam = TEAM_FREE;
	ent->client->sess.sessionClass = 0;//PC_NOCLASS;

	trap_SetConfigstring( CS_PLAYERS + clientNum, "");

	CalculateRanks( qfalse );

	if ( ent->r.svFlags & SVF_BOT ) {
		BotAIShutdownClient( clientNum );
	}

	// kef -- if this guy contributed to any of our kills/deaths/weapons logs, clean 'em out
	G_ClearClientLog(clientNum);

	//also remove any initial data
	clientInitialStatus[clientNum].initialized = qfalse;

	//If the queen or action hero leaves, have to pick a new one...
	if ( g_pModAssimilation.integer != 0 )
	{
		G_CheckReplaceQueen( clientNum );
	}
	if ( g_pModActionHero.integer != 0 )
	{
		G_CheckReplaceActionHero( clientNum );
	}
}

int G_AddPlayerLanguage(char *name, int clientNum) {
	int i;

	for(i = 0; i < MAX_LANGUAGES; i++) {
		if(languageIndex[i] != NULL) {
			if(!Q_stricmp(Q_strlwr(name), languageIndex[i]))
				return i;
		} else {
			languageIndex[i] = G_NewString(name);
			return i;
		}
	}
	G_Printf(S_COLOR_RED "ERROR: MAX_RACES hit!\n");
	trap_SendServerCommand(clientNum, S_COLOR_RED "ERROR: MAX_RACES hit!\n");
	return 0; // we have hit the limit
}

