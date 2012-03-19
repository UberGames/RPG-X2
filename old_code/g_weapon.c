/*
----------------------------------------------
	DREADNOUGHT
----------------------------------------------
*/

#define MAXRANGE_DREADNOUGHT	2048

//---------------------------------------------------------
void WP_FireDreadnoughtBeam( gentity_t *ent )
//---------------------------------------------------------
{
	trace_t		tr;
	vec3_t		end;
	gentity_t	*traceEnt;
	vec3_t		start;
	qboolean	bHit = qfalse;

	// Trace once to the right...
	VectorMA( muzzle, DREADNOUGHT_WIDTH, right, start);
	VectorMA( start, MAXRANGE_DREADNOUGHT, forward, end );

	// Find out who we've hit
	trap_Trace( &tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT );
	traceEnt = &g_entities[ tr.entityNum ];

	if ( traceEnt->takedamage)
	{
		G_Damage( traceEnt, ent, ent, forward, tr.endpos, DREADNOUGHT_DAMAGE*s_quadFactor, 
					DAMAGE_NOT_ARMOR_PIERCING, MOD_DREADNOUGHT);
		bHit = qtrue;
	}

	// Now trace once to the left...
	VectorMA( muzzle, -DREADNOUGHT_WIDTH, right, start);
	VectorMA( start, MAXRANGE_DREADNOUGHT, forward, end );

	// Find out who we've hit
	trap_Trace( &tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT );
	traceEnt = &g_entities[ tr.entityNum ];

	if ( traceEnt->takedamage)
	{
		G_Damage( traceEnt, ent, ent, forward, tr.endpos, DREADNOUGHT_DAMAGE*s_quadFactor, 
					DAMAGE_NOT_ARMOR_PIERCING, MOD_DREADNOUGHT);
		bHit = qtrue;
	}
	if (bHit)
	{
		// log hit
		/*if (ent->client)
		{
			ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
		}*/
	}
}


//#define DN_SEARCH_DIST	512
//#define DN_SIDE_DIST	64
//#define DN_RAND_DEV		8

#define DN_SEARCH_DIST	256
#define DN_SIDE_DIST	128
#define DN_RAND_DEV		16
#define DN_ALT_THINK_TIME	100
#define DN_ALT_SIZE		12
void DreadnoughtBurstThink(gentity_t *ent)
{
	vec3_t startpos, endpos, perp;
	trace_t tr;
	gentity_t *traceEnt, *tent;
	int source;
	vec3_t dest, mins={-DN_ALT_SIZE,-DN_ALT_SIZE,-1}, maxs={DN_ALT_SIZE,DN_ALT_SIZE,1};
	float dot;
	static qboolean recursion=qfalse;

	VectorCopy(ent->s.origin, startpos);

	// Search in a 3-way arc in front of it.
	VectorMA(startpos, DN_SEARCH_DIST, ent->movedir, endpos);
	endpos[0] += flrandom(-DN_RAND_DEV, DN_RAND_DEV);
	endpos[1] += flrandom(-DN_RAND_DEV, DN_RAND_DEV);
	endpos[2] += flrandom(-DN_RAND_DEV*0.5, DN_RAND_DEV*0.5);

	// unlink this entity, so the next trace will go past it
	trap_UnlinkEntity(ent);

	// link back in any entities we unlinked
	if (ent->target_ent)
	{
		source = ent->target_ent->s.number;
	}
	else if (ent->r.ownerNum)
	{
		source = ent->r.ownerNum;
	}
	else
	{
		source = ent->s.number;
	}

	trap_Trace (&tr, startpos, mins, maxs, endpos, source, MASK_SHOT );
	traceEnt = &g_entities[ tr.entityNum ];
	// did we hit the holdable shield?
	if (traceEnt->classname && !Q_stricmp(traceEnt->classname, "holdable_shield"))
	{
		// Make sure you damage the surface first
		G_Damage( traceEnt, ent, &g_entities[ent->r.ownerNum], forward, tr.endpos, ent->damage, 
					DAMAGE_NOT_ARMOR_PIERCING, MOD_DREADNOUGHT_ALT);

		VectorCopy(ent->s.origin, ent->s.origin2);
		VectorCopy(endpos, ent->s.origin);
		SnapVector(ent->s.origin);
		SnapVector(ent->s.origin2);

		ent->nextthink = level.time + 100;

		// yes. We are done.
		ent->think = G_FreeEntity;

		tent = G_TempEntity( tr.endpos, EV_DREADNOUGHT_MISS );
		// Stash origins, etc. so that the effects can have access to them
		VectorCopy( startpos, tent->s.origin2 );
		SnapVector(tent->s.origin2);
		tent->s.eventParm = DirToByte( tr.plane.normal );

		return;
	}

	if (traceEnt->takedamage) 
	{
		ent->target_ent = traceEnt;
		VectorCopy(ent->s.origin, ent->s.origin2);
		SnapVector(ent->s.origin);
		SnapVector(ent->s.origin2);
		VectorCopy(traceEnt->r.currentOrigin, ent->s.origin);
		trap_LinkEntity(ent);
		VectorNormalize(ent->movedir);
		ent->nextthink = level.time + DN_ALT_THINK_TIME;
		G_Damage( traceEnt, ent, &g_entities[ent->r.ownerNum], forward, tr.endpos, ent->damage, 
					DAMAGE_NOT_ARMOR_PIERCING, MOD_DREADNOUGHT_ALT);
		// log hit
		/*if (ent->client)
		{
			ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
		}*/
		return;
	}
	else
	{
		if (tr.fraction < 0.02)
		{	// Hit a wall...

			dot = DotProduct(ent->movedir, tr.plane.normal);
			if (dot < -0.6 || dot == 0.0)
			{	// Stop.
				G_FreeEntity( ent );

				tent = G_TempEntity( tr.endpos, EV_DREADNOUGHT_MISS );

				// Stash origins, etc. so that the effects can have access to them
				VectorCopy( startpos, tent->s.origin2 );
				SnapVector(tent->s.origin2);
				tent->s.eventParm = DirToByte( tr.plane.normal );

				return;
			}
			else
			{

				// Bounce off the surface just a little
				VectorMA(ent->movedir, -1.25*dot, tr.plane.normal, ent->movedir);
				VectorNormalize(ent->movedir);

				// Make sure we store a next think time, else the effect could stick around forever...
				ent->nextthink = level.time + DN_ALT_THINK_TIME;

				if (!recursion)
				{	// NOTE RECURSION HERE.
					recursion=qtrue;
					DreadnoughtBurstThink(ent);
					recursion=qfalse;
				}

				return;
			}
				
		}

		VectorCopy(tr.endpos, dest);
	}

	// Didn't hit anything forward.  Try some side vectors.

	// Get the perp vector (okay, only in 2D) to find a right or left (random)-pointing perpendicular vector to the facing.
	if (irandom(0,1))
	{	// Right vector
		perp[0] = ent->movedir[1];
		perp[1] = -ent->movedir[0];
		perp[2] = ent->movedir[2];
	}
	else
	{	// Left vector
		perp[0] = -ent->movedir[1];
		perp[1] = ent->movedir[0];
		perp[2] = ent->movedir[2];
	}

	// Search a random interval from the side arc
	VectorMA(endpos, DN_SIDE_DIST, perp, endpos);
	endpos[0] += flrandom(-DN_RAND_DEV, DN_RAND_DEV);
	endpos[1] += flrandom(-DN_RAND_DEV, DN_RAND_DEV);
	endpos[2] += flrandom(-DN_RAND_DEV*0.5, DN_RAND_DEV*0.5);

	trap_Trace (&tr, startpos, mins, maxs, endpos, source, MASK_SHOT );
	traceEnt = &g_entities[ tr.entityNum ];
	// did we hit the holdable shield?
	if (traceEnt->classname && !Q_stricmp(traceEnt->classname, "holdable_shield"))
	{
		// yes. We are done.
		VectorCopy(ent->s.origin, ent->s.origin2);
		VectorCopy(endpos, ent->s.origin);
		SnapVector(ent->s.origin);
		SnapVector(ent->s.origin2);

		ent->nextthink = level.time + 100;

		ent->think = G_FreeEntity;
		tent = G_TempEntity( tr.endpos, EV_DREADNOUGHT_MISS );
		// Stash origins, etc. so that the effects can have access to them
		VectorCopy( startpos, tent->s.origin2 );
		SnapVector(tent->s.origin2);
		tent->s.eventParm = DirToByte( tr.plane.normal );

		return;
	}
	if (traceEnt->takedamage) 
	{
		ent->target_ent = traceEnt;
		VectorCopy(ent->s.origin, ent->s.origin2);
		SnapVector(ent->s.origin);
		SnapVector(ent->s.origin2);
		VectorCopy(traceEnt->r.currentOrigin, ent->s.origin);
		trap_LinkEntity(ent);
		VectorNormalize(ent->movedir);
		ent->nextthink = level.time + DN_ALT_THINK_TIME;
		G_Damage( traceEnt, ent, &g_entities[ent->r.ownerNum], forward, tr.endpos, ent->damage, 
					DAMAGE_NOT_ARMOR_PIERCING, MOD_DREADNOUGHT_ALT);

		// log hit
		/*if (ent->client)
		{
			ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
		}*/
		// NOTE Send this as a hit effect once we have one...
		G_Sound( traceEnt, G_SoundIndex( SOUND_DIR "dreadnought/dn_althit.wav"));
			
		return;
	}

	// Search a random interval in the opposite direction
	VectorMA(endpos, -2.0*DN_SIDE_DIST, perp, endpos);
	endpos[0] += flrandom(-DN_RAND_DEV, DN_RAND_DEV);
	endpos[1] += flrandom(-DN_RAND_DEV, DN_RAND_DEV);
	endpos[2] += flrandom(-DN_RAND_DEV*0.5, DN_RAND_DEV*0.5);

	trap_Trace (&tr, startpos, mins, maxs, endpos, source, MASK_SHOT );
	traceEnt = &g_entities[ tr.entityNum ];
	// did we hit the holdable shield?
	if (traceEnt->classname && !Q_stricmp(traceEnt->classname, "holdable_shield"))
	{
		VectorCopy(ent->s.origin, ent->s.origin2);
		VectorCopy(endpos, ent->s.origin);
		SnapVector(ent->s.origin);
		SnapVector(ent->s.origin2);

		ent->nextthink = level.time + 100;

		// yes. We are done.
		ent->think = G_FreeEntity;

		tent = G_TempEntity( tr.endpos, EV_DREADNOUGHT_MISS );
		// Stash origins, etc. so that the effects can have access to them
		VectorCopy( startpos, tent->s.origin2 );
		SnapVector(tent->s.origin2);
		tent->s.eventParm = DirToByte( tr.plane.normal );

		return;
	}
	if (traceEnt->takedamage) 
	{
		ent->target_ent = traceEnt;
		VectorCopy(ent->s.origin, ent->s.origin2);
		SnapVector(ent->s.origin);
		SnapVector(ent->s.origin2);
		VectorCopy(traceEnt->r.currentOrigin, ent->s.origin);
		trap_LinkEntity(ent);
		VectorNormalize(ent->movedir);
		ent->nextthink = level.time + DN_ALT_THINK_TIME;
		G_Damage( traceEnt, ent, &g_entities[ent->r.ownerNum], forward, tr.endpos, ent->damage, 
					DAMAGE_NOT_ARMOR_PIERCING, MOD_DREADNOUGHT_ALT);
		// log hit
		/*if (ent->client)
		{
			ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
		}*/
		return;
	}

	// We didn't find anything, so move the entity to the middle destination.
	ent->target_ent = NULL;
	VectorCopy(ent->s.origin, ent->s.origin2);
	VectorCopy(dest, ent->s.origin);
	VectorCopy(dest, ent->s.pos.trBase);
	SnapVector(ent->s.origin2);
	trap_LinkEntity(ent);
	ent->nextthink = level.time + DN_ALT_THINK_TIME;

	return;
}


//---------------------------------------------------------
void WP_FireDreadnoughtBurst( gentity_t *ent )
//---------------------------------------------------------
{
	gentity_t	*bolt;
	vec3_t		dir, start;

	VectorCopy( forward, dir );
	VectorCopy( muzzle, start );

	bolt = G_Spawn();
	bolt->classname = "dn_projectile";
	
	bolt->nextthink = level.time + 200;
	bolt->think = DreadnoughtBurstThink;

	bolt->s.eType = ET_ALT_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_13;
	bolt->r.ownerNum = ent->s.number;
	bolt->parent = ent;
	bolt->damage = DREADNOUGHT_ALTDAMAGE*DMG_VAR*s_quadFactor;
	bolt->splashDamage = 0;
	bolt->splashRadius = 0;
	bolt->methodOfDeath = MOD_DREADNOUGHT_ALT;
	bolt->clipmask = MASK_SHOT;

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time;
	VectorCopy( start, bolt->s.pos.trBase );
	VectorCopy( start, bolt->s.origin);
	SnapVector(bolt->s.pos.trBase);
	SnapVector(bolt->s.origin);

	VectorCopy( forward, bolt->movedir);
	
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
	VectorCopy( start, bolt->r.currentOrigin );
	VectorCopy( start, bolt->pos1 );
	VectorCopy( start, bolt->pos2 );

	DreadnoughtBurstThink(bolt);
}

//---------------------------------------------------------
void WP_FireDreadnought( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	// This was moved out of the FireWeapon switch statement below to keep things more consistent
	if ( alt_fire )
	{
		WP_FireDreadnoughtBurst( ent );
	}
	else
	{
		WP_FireDreadnoughtBeam( ent );
	}

	G_LogWeaponFire(ent->s.number, WP_13);
}


//======================================================================

#define BORG_PROJECTILE_SIZE	8
#define BORG_PROJ_VELOCITY		1000

void WP_FireBorgTaser( gentity_t *ent )
{
	trace_t		tr;
	vec3_t		end, d_dir;
	gentity_t	*tent = 0;
	gentity_t	*traceEnt = NULL;

	VectorMA (muzzle, MAXRANGE_IMOD, forward, end);

	trap_Trace (&tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT );
	traceEnt = &g_entities[ tr.entityNum ];

	if ( traceEnt->takedamage )
	{
		LogAccuracyHit( traceEnt, ent );

		//For knockback - send them up in air
		VectorCopy( forward, d_dir );
		if ( d_dir[2] < 0.30f )
		{
			d_dir[2] = 0.30f;
		}
		VectorNormalize( d_dir );

		G_Damage( traceEnt, ent, ent, forward, tr.endpos, BORG_TASER_DAMAGE * s_quadFactor, 0, MOD_BORG_ALT );
	}

	VectorMA (muzzle, 2, forward, muzzle);
	tent = G_TempEntity( tr.endpos, EV_BORG_ALT_WEAPON );

	// Stash origins, etc. so the effect can have access to them
	VectorCopy( muzzle, tent->s.origin2 );
	SnapVector( tent->s.origin2 );			// save net bandwidth
	tent->s.eventParm = DirToByte( tr.plane.normal );
	tent->s.weapon = ent->s.weapon;
}


void WP_FireBorgProjectile( gentity_t *ent, vec3_t start )
//---------------------------------------------------------
{
	gentity_t	*bolt;

	bolt = G_Spawn();
	
	bolt->classname = "borg_proj";
	bolt->nextthink = level.time + 10000;
	bolt->think = G_FreeEntity;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_11;
	bolt->r.ownerNum = ent->s.number;
	bolt->parent = ent;

	bolt->damage = BORG_PROJ_DAMAGE * DMG_VAR * s_quadFactor;
	bolt->splashDamage = 0;
	bolt->splashRadius = 0;
	bolt->methodOfDeath = MOD_BORG;
	bolt->clipmask = MASK_SHOT;

	// Set the size of the missile up
	VectorSet(bolt->r.maxs, BORG_PROJECTILE_SIZE, BORG_PROJECTILE_SIZE, BORG_PROJECTILE_SIZE);
	VectorScale( bolt->r.maxs, -1, bolt->r.mins );

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	SnapVector( bolt->s.pos.trBase );			// save net bandwidth
	VectorScale( forward, BORG_PROJ_VELOCITY, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
	VectorCopy( start, bolt->r.currentOrigin);
}

void WP_Assimilate( gentity_t *ent, qboolean alt_fire )
{//MCG: hacked this in for now for testing the rules
	gentity_t	*tr_ent;
	trace_t		tr;
	vec3_t		end;
	float		range;

	if ( ent->s.number == borgQueenClientNum )
	{
		range = 32;
	}
	else
	{
		range = 64;
	}
	VectorMA( muzzle, range, forward, end );

	trap_Trace ( &tr, muzzle, vec3_origin, vec3_origin, end, ent->s.number, MASK_SHOT );

	if ( tr.entityNum >= ENTITYNUM_WORLD )
	{
		return;
	}

	tr_ent = &g_entities[tr.entityNum];

	if ( tr_ent && tr_ent->client && tr_ent->health > 0 && ent->client && ent->client->sess.sessionTeam != tr_ent->client->sess.sessionTeam )
	{
		if ( ent->s.number == borgQueenClientNum )
		{//Borg queen assimilates with one hit
			tr_ent->health = 0;
			player_die( tr_ent, ent, ent, 100, MOD_ASSIMILATE );
			//G_Damage( tr_ent, ent, ent, forward, tr.endpos, 50, DAMAGE_NO_ARMOR|DAMAGE_NO_INVULNERABILITY, MOD_ASSIMILATE );
		}
		else
		{ 
			G_Damage( tr_ent, ent, ent, forward, tr.endpos, 10, DAMAGE_NO_ARMOR|DAMAGE_NO_INVULNERABILITY, MOD_ASSIMILATE );
		}
	}
}

/*
===============
RPG-X Fire Engineer Tool
By: RedTechie
===============
*/
/*void WP_NEUTRINO_PROBE( gentity_t *ent, qboolean alt_fire ){
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
			trap_Trace( &trace, src, vec3_origin, vec3_origin, dest, ent->s.number, CONTENTS_TRIGGER );

			if ( trace.fraction == 1.0f || trace.entityNum < 0 )
			{
				trap_SendConsoleCommand( EXEC_APPEND, va("echo No target in range to kick.") );
				return;
			}

			target = &g_entities[trace.entityNum];

			////////////////////////////////
			
			//lets delete it
			if(target){
				G_FreeEntity( target );
			}
		}
	//}
}*/

/*
----------------------------------------------
	TETRION
----------------------------------------------
*/

#define TETRION_SPREAD			275
#define NUM_TETRION_SHOTS		3

#define TETRION_ALT_VELOCITY	500000		//RPG-X: J2J - increased velocity to 1/2 million units/second
#define TETRION_ALT_SIZE		6

#define MAXRANGE_TETRION		5000000		//RPG-X: J2J - increased range to 5 million units

typedef struct tetrionHit_s
{
	gentity_t	*ent;
	vec3_t		pos;
} tetrionHit_t;

// provide the center of the circle, a normal out from it (normalized, please), and the radius.
//out will then become a random position on the radius of the circle.
void fxRandCircumferencePos(vec3_t center, vec3_t normal, float radius, vec3_t out)
{
	float		rnd = flrandom(0, 2*M_PI);
	float		s = sin(rnd);
	float		c = cos(rnd);
	vec3_t		vTemp, radialX, radialY;

	vTemp[0]=0;
	vTemp[1]=0;
	vTemp[2]=-1;
	CrossProduct(normal, vTemp, radialX);
	CrossProduct(normal, radialX, radialY);
	VectorScale(radialX, radius, radialX);
	VectorScale(radialY, radius, radialY);
	VectorMA(center, s, radialX, out);
	VectorMA(out, c, radialY, out);
}
#define NUM_TETRION_BULLETS		3
//---------------------------------------------------------
void FireTetrionBullet( gentity_t *ent, vec3_t start, vec3_t dir )
//---------------------------------------------------------
{
	gentity_t	*tent = NULL;
	trace_t		tr;
	vec3_t		end, lup = {0,0,1}, lright;		// , zero = {0,0,0};
	int			i = 0, j = 0;
	qboolean		bHitAlready = qfalse;
	int				numHits = 0;
	tetrionHit_t	hitEnts[NUM_TETRION_BULLETS];


	vec3_t	new_start, radial, start2, spreadFwd;
	float	firingRadius = 6, minDeviation = 0.95, maxDeviation = 1.1;

	for (i = 0; i < NUM_TETRION_BULLETS; i++)
	{
		hitEnts[i].ent = NULL;
	}
	// create our message-entity with the firing position for an origin
	tent = G_TempEntity(muzzle, EV_TETRION);
	// stash our firing direction in the message also
	VectorCopy(forward, tent->s.angles2);
	VectorShort(tent->s.angles2);

	// now do the damage. we're going to fake three separate bullets here by doing three
	//traces and splitting two or three hits worth of dmg between whatever the traces hit.
	CrossProduct(forward, lup, lright);
	CrossProduct(lright, forward, up); // get a "real" up

	for (i = 0; i < NUM_TETRION_BULLETS; i++)
	{
		// determine new firing position 
		fxRandCircumferencePos(start, forward, firingRadius, new_start);
		VectorSubtract(new_start, start, radial);
		VectorMA(start, 10, forward, start2);
		VectorMA(start2, flrandom(minDeviation, maxDeviation), radial, start2);
		VectorSubtract(start2, new_start, spreadFwd);
		VectorNormalize(spreadFwd);
		// determine new end position for this bullet. give the endpoint some spread, too.
		VectorMA(new_start, MAXRANGE_TETRION, spreadFwd, end);
		trap_Trace ( &tr, new_start, NULL, NULL, end, ent->s.number, MASK_SHOT);
		if ((tr.entityNum < MAX_GENTITIES) && (tr.entityNum != ENTITYNUM_WORLD))
		{
			bHitAlready = qfalse;
			for (j = 0; j < numHits; j++)
			{
				if (hitEnts[j].ent == &g_entities[ tr.entityNum ])
				{
					// already hit this ent
					bHitAlready = qtrue;
					break;
				}
			}
			if (!bHitAlready)
			{
				hitEnts[numHits].ent = &g_entities[ tr.entityNum ];
				VectorCopy(tr.endpos, hitEnts[numHits].pos);
				numHits++;
			}
		}
	}
	if (numHits)
	{
		// determine damage (2 or 3 bullets)
		float totalDmg = (float)(irandom(2,3)*TETRION_DAMAGE*s_quadFactor), dmgPerHit = 0;

		dmgPerHit = (int)(totalDmg/(float)numHits);
		for (i = 0; i < numHits; i++)
		{
			if (hitEnts[i].ent->takedamage) 
			{
				G_Damage( hitEnts[i].ent, ent, ent, forward, hitEnts[i].pos, dmgPerHit, 
							DAMAGE_ARMOR_PIERCING|DAMAGE_NO_KNOCKBACK, MOD_TETRION );
			}
			// log hit
			/*if (ent->client)
			{
				ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
			}*/
		}
	}
}

//---------------------------------------------------------
void FireTetrionProjectile( gentity_t *ent, vec3_t start, vec3_t dir )
//---------------------------------------------------------
{// Projectile that /*bouncesoff surfaces but does not have gravity
	gentity_t	*bolt;

	bolt = G_Spawn();
	bolt->classname = "tetrion_projectile";
	bolt->nextthink = level.time + 2000;
	bolt->think = G_FreeEntity;

	bolt->s.eType = ET_ALT_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	//bolt->s.eFlags |= EF_BOUNCE;					//RPG-X: J2J - Dont bounce anymore
	//bolt->count = random() > 0.75f ? 3 : 4;		//RPG-X: J2J - Number of bounces

	//RPG-X: J2J - Not sure about alt size, may need tweaking
	VectorSet(bolt->r.mins, -TETRION_ALT_SIZE, -TETRION_ALT_SIZE, -TETRION_ALT_SIZE);
	VectorSet(bolt->r.maxs, TETRION_ALT_SIZE, TETRION_ALT_SIZE, TETRION_ALT_SIZE);

	bolt->s.weapon = WP_7;
	bolt->r.ownerNum = ent->s.number;
	bolt->parent = ent;
	bolt->damage = 300; //RPG-X: RedTechie - Use to be TETRION_ALT_DAMAGE*DMG_VAR*s_quadFactor;
	bolt->splashDamage = 0;
	bolt->splashRadius = 0;
	bolt->methodOfDeath = MOD_TETRION_ALT;
	bolt->clipmask = CONTENTS_BODY;	//MASK_SHOT;	MASK_ONLYPLAYER	//RPG-X: J2J - Goes through all objects except players and corpses

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	SnapVector( bolt->s.pos.trBase );			// save net bandwidth
	VectorScale( dir, TETRION_ALT_VELOCITY, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
	VectorCopy (start, bolt->r.currentOrigin);
}




void WP_FireBorgWeapon( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	if ( alt_fire )
	{
		WP_FireBorgTaser( ent );
	}
	else
	{
		WP_FireBorgProjectile( ent, muzzle );
	}

	G_LogWeaponFire( ent->s.number, WP_11 );
}

//TiM
// Alt-fire...
//---------------------------------------------------------
/*void FireScavengerGrenade( gentity_t *ent, vec3_t start, vec3_t dir )
//---------------------------------------------------------
{
	gentity_t	*grenade;

	grenade = G_Spawn();
	
	grenade->classname = "scav_grenade";
	grenade->nextthink = level.time + 9000;
	grenade->think = G_FreeEntity;
	grenade->s.eType = ET_ALT_MISSILE;
	grenade->s.weapon = WP_4;
	grenade->r.ownerNum = ent->s.number;
	grenade->parent = ent;
	grenade->damage = SCAV_ALT_DAMAGE*DMG_VAR*s_quadFactor; 
	grenade->splashDamage = SCAV_ALT_SPLASH_DAM*s_quadFactor;
	grenade->splashRadius = SCAV_ALT_SPLASH_RAD;// *s_quadFactor;
	grenade->methodOfDeath = MOD_SCAVENGER_ALT;
	grenade->splashMethodOfDeath = MOD_SCAVENGER_ALT_SPLASH;
	grenade->clipmask = MASK_SHOT;
	grenade->s.eFlags |= EF_ALT_FIRING;

	// Set the size of the missile up
	VectorSet(grenade->r.maxs, SCAV_ALT_SIZE, SCAV_ALT_SIZE, SCAV_ALT_SIZE);
	VectorSet(grenade->r.mins, -SCAV_ALT_SIZE, -SCAV_ALT_SIZE, -SCAV_ALT_SIZE);

	grenade->s.pos.trType = TR_GRAVITY;
	grenade->s.pos.trTime = level.time;		// move a bit on the very first frame
	
	VectorCopy( start, grenade->s.pos.trBase );
	SnapVector( grenade->s.pos.trBase );			// save net bandwidth
	VectorScale( dir, random() * 100 + SCAV_ALT_VELOCITY, grenade->s.pos.trDelta );

	// Add a tad of upwards velocity to the shot.
	grenade->s.pos.trDelta[2] += SCAV_ALT_UP_VELOCITY;

	// Add in half the player's velocity to the shot.
	VectorMA(grenade->s.pos.trDelta, 0.5, ent->s.pos.trDelta, grenade->s.pos.trDelta);

	SnapVector( grenade->s.pos.trDelta );			// save net bandwidth
	VectorCopy (start, grenade->r.currentOrigin);
	VectorCopy (start, grenade->pos1);
	VectorCopy (start, grenade->s.origin2);
	SnapVector( grenade->s.origin2 );			// save net bandwidth
}*/

//TiM
//---------------------------------------------------------
/*void WP_FireScavenger( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	vec3_t		dir, angles, temp_ang, temp_org;
	vec3_t		start;
	float		offset;

	VectorCopy( forward, dir );
	VectorCopy( muzzle, start );

	if ( alt_fire )
	{
		FireScavengerGrenade( ent, start, dir );
	}
	else
	{
		vectoangles( dir, angles );
		VectorSet( temp_ang, angles[0] + (crandom() * SCAV_SPREAD), angles[1] + (crandom() * SCAV_SPREAD), angles[2] );
		AngleVectors( temp_ang, dir, NULL, NULL );

		// try to make the shot alternate between barrels
		offset = irandom(0, 1) * 2 + 1;

		// FIXME:  These offsets really don't work like they should 
		VectorMA( start, offset, right, temp_org );
		VectorMA( temp_org, offset, up, temp_org );
		FireScavengerBullet( ent, temp_org, dir );
	}

	G_LogWeaponFire(ent->s.number, WP_4);
}*/

void FireSeeker( gentity_t *owner, gentity_t *target, vec3_t origin)
{
	vec3_t dir;
	
	VectorSubtract( target->r.currentOrigin, origin, dir);
	VectorNormalize(dir);

	// for now I'm just using the scavenger bullet.
	FireScavengerBullet( owner, origin, dir );
}

//TiM
//---------------------------------------------------------
void FireScavengerBullet( gentity_t *ent, vec3_t start, vec3_t dir )
//---------------------------------------------------------
{
	gentity_t	*bolt;

	bolt = G_Spawn();
	
	bolt->classname = "scav_proj";
	bolt->nextthink = level.time + 10000;
	bolt->think = G_FreeEntity;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	//bolt->s.weapon = WP_4;
	bolt->s.weapon = WP_10; //TiM
	bolt->r.ownerNum = ent->s.number;
	bolt->parent = ent;

//fixme  - remove
	{
		// Flags effect as being the full beefy version for the player
		bolt->count = 0;
	}

	bolt->damage = SCAV_DAMAGE*DMG_VAR*s_quadFactor;
	bolt->splashDamage = 0;
	bolt->splashRadius = 0;
	bolt->methodOfDeath = MOD_SCAVENGER;
	bolt->clipmask = MASK_SHOT;

	// Set the size of the missile up
	VectorSet(bolt->r.maxs, SCAV_SIZE, SCAV_SIZE, SCAV_SIZE);
	VectorSet(bolt->r.mins, -SCAV_SIZE, -SCAV_SIZE, -SCAV_SIZE);

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time - 10;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	SnapVector( bolt->s.pos.trBase );			// save net bandwidth
	VectorScale( dir, SCAV_VELOCITY, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
	VectorCopy( start, bolt->r.currentOrigin);
}

/*
----------------------------------------------
	IMOD
----------------------------------------------
*/

#define MAXRANGE_IMOD			4096
#define	MAX_RAIL_HITS	4

void IMODHit(qboolean alt_fire, gentity_t *traceEnt, gentity_t *ent, vec3_t d_dir, vec3_t endpos, vec3_t normal, 
			 qboolean render_impact)
{
	gentity_t	*tent = NULL;
	int			damage = IMOD_ALT_DAMAGE*DMG_VAR;

	if ( alt_fire )
	{
		// send beam impact
		if ( traceEnt && traceEnt->takedamage ) 
		{
			G_Damage( traceEnt, ent, ent, d_dir, endpos, damage, DAMAGE_NO_ARMOR | DAMAGE_NO_INVULNERABILITY, MOD_IMOD_ALT);
			if (render_impact)
			{
				tent = G_TempEntity( endpos, EV_IMOD_ALTFIRE_HIT );
			}
			// log hit
			/*if (ent->client)
			{
				ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
			}*/
		}
	}
	else
	{
		// send beam impact
		if ( traceEnt && traceEnt->takedamage && rpg_imoddmg.integer != 0) 
		{
			G_Damage( traceEnt, ent, ent, d_dir, endpos, damage, DAMAGE_NO_ARMOR | DAMAGE_NO_INVULNERABILITY, MOD_IMOD);
			if (render_impact)
			{
				tent = G_TempEntity( endpos, EV_IMOD_HIT );
			}
			// log hit
			/*if (ent->client)
			{
				ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
			}*/
		}
	}
	if (tent)
	{
		tent->s.eventParm = DirToByte( normal );
		tent->s.weapon = ent->s.weapon;
		VectorCopy( muzzle, tent->s.origin2 );
		SnapVector(tent->s.origin2);
	}
}

//---------------------------------------------------------
/*void WP_FireIMOD ( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	int			i = 0;
	int			hits = 0;
	int			unlinked = 0;
	gentity_t	*unlinkedEntities[MAX_RAIL_HITS];

	trace_t		tr;
	vec3_t		end, d_dir;
	gentity_t	*tent = 0;
	gentity_t	*traceEnt = NULL;
	qboolean	render_impact;

	VectorMA (muzzle, MAXRANGE_IMOD, forward, end);

	// trace only against the solids, so the railgun will go through people
	do
	{
		trap_Trace (&tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT );
		traceEnt = &g_entities[ tr.entityNum ];
		if ( tr.surfaceFlags & SURF_NOIMPACT ) 
		{	// If the beam hit the skybox, etc. it would look foolish to add in an explosion
			render_impact = qfalse;
		} else 
		{
			render_impact = qtrue;
		}
		if ( traceEnt->takedamage )
		{
			if( LogAccuracyHit( traceEnt, ent ) )
			{
				hits++;
			}
			//For knockback - send them up in air
			VectorCopy(forward, d_dir);
			if(d_dir[2] < 0.30f)
			{
				d_dir[2] = 0.30f;
			}
			VectorNormalize(d_dir);
			// do the damage and send an impact effect
			IMODHit(alt_fire, traceEnt, ent, d_dir, tr.endpos, tr.plane.normal, qtrue);

			// give the shooter a reward sound if they have made two sniper hits in a row
			// check for "impressive" reward sound
			// Note that hitting two people in a line warrants an "impressive" as well...
			if (traceEnt->client)
			{
				if (((g_gametype.integer >= GT_TEAM) && (ent->client->ps.persistant[PERS_TEAM] != traceEnt->client->ps.persistant[PERS_TEAM]))
					|| (g_gametype.integer < GT_TEAM))
				{
					if (alt_fire)
					{
						ent->client->accurateCount++;
						if ( ent->client->accurateCount >= 2 ) 
						{
							ent->client->accurateCount -= 2;
							ent->client->ps.persistant[PERS_REWARD_COUNT]++;
							ent->client->ps.persistant[PERS_REWARD] = REWARD_IMPRESSIVE;
							ent->client->ps.persistant[PERS_IMPRESSIVE_COUNT]++;
							// add the sprite over the player's head
							ent->client->ps.eFlags &= ~EF_AWARD_MASK;
							ent->client->ps.eFlags |= EF_AWARD_IMPRESSIVE;
							ent->client->rewardTime = level.time + REWARD_SPRITE_TIME;
						} // End of brace
					}
				}
			}
		}
		else
		{	// send an impact effect
			IMODHit(alt_fire, traceEnt, ent, d_dir, tr.endpos, tr.plane.normal, render_impact);
		}
		if ( tr.contents & CONTENTS_SOLID )
		{
			break;		// we hit something solid enough to stop the beam
		}
		// unlink this entity, so the next trace will go past it
		trap_UnlinkEntity( traceEnt );
		unlinkedEntities[unlinked] = traceEnt;
		unlinked++;
	} while ( unlinked < MAX_RAIL_HITS );

	// link back in any entities we unlinked
	for ( i = 0 ; i < unlinked ; i++ )
	{
		trap_LinkEntity( unlinkedEntities[i] );
	}

	// With endcapping on, the beam is actually longer than what the length parm in FX_AddLine for this
	//		effect specifies..so move it out so it doesn't clip into the gun so much
	if ( alt_fire )
		VectorMA (muzzle, 10, forward, muzzle);
	else
		VectorMA (muzzle, 2, forward, muzzle);

	// Create the events that will add in the necessary effects
	if ( alt_fire )
	{
		tent = G_TempEntity( tr.endpos, EV_IMOD_ALTFIRE );
	}
	else
	{
		tent = G_TempEntity( tr.endpos, EV_IMOD );
	}

	// Stash origins, etc. so the effect can have access to them
	VectorCopy( muzzle, tent->s.origin2 );
	SnapVector( tent->s.origin2 );			// save net bandwidth
	if ( render_impact )
	{
		tent->s.eventParm = DirToByte( tr.plane.normal );
		tent->s.weapon = ent->s.weapon;
	}

	G_LogWeaponFire(ent->s.number, WP_1);
	//G_LogWeaponFire(ent->s.number, WP_1);
}*/

#define		SEEKER_RADIUS	500
qboolean SeekerAcquiresTarget ( gentity_t *ent, vec3_t pos )
{
	vec3_t		seekerPos;
	float		angle;
	int			entityList[MAX_CLIENTS]; // targets within inital radius
	int			visibleTargets[MAX_CLIENTS]; // final filtered target list
	int			numListedEntities;
	int			i, e;
	gentity_t	*target;
	vec3_t		mins, maxs;

//	if (!irandom(0,2)) for now, it'll shoot every second it finds a target
	{
		angle = level.time/100.0f;
		seekerPos[0] = ent->r.currentOrigin[0] + 18 * cos(angle);
		seekerPos[1] = ent->r.currentOrigin[1] + 18 * sin(angle);
		seekerPos[2] = ent->r.currentOrigin[2] + ent->client->ps.viewheight + 8 + (3*cos(level.time/150.0f));

		for ( i = 0 ; i < 3 ; i++ )
		{
			mins[i] = seekerPos[i] - SEEKER_RADIUS;
			maxs[i] = seekerPos[i] + SEEKER_RADIUS;
		}

		//	get potential targets within radius
		numListedEntities = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );

		i = 0; // reset counter

		for ( e = 0 ; e < numListedEntities ; e++ )
		{
			target = &g_entities[entityList[ e ]];
			// seeker owner not a valid target
			if (target == ent)
			{
				continue;
			}
			
			// only players are valid targets
			if (!target->classname || strcmp(target->classname, "player"))
			{
				continue;
			}

			// teammates not valid targets
			if (OnSameTeam(ent, target))
			{
				continue;
			}

			// don't shoot at dead things
			if (target->health <= 0)
			{
				continue;
			}
			//RPG-X: RedTechie - Need to take this out causes bad crashes with entity shooter, shouldent be needed anyway with newer code
			//target->beingfiredby = ent;
			if( CanDamage (target, seekerPos) ) // visible target, so add it to the list
			{
				visibleTargets[i++] = entityList[e];
			}
		}

		if (i)
		{
			// ok, now we know there are i visible targets.  Pick one as the seeker's target
			ent->enemy = &g_entities[visibleTargets[irandom(0,i-1)]];
			VectorCopy(seekerPos, pos);
			return qtrue;
		}
	}

	return qfalse;
}
