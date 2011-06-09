#include "cg_local.h"
#include "fx_local.h"

void FX_StasisDischarge( vec3_t origin, vec3_t normal, int count, float dist_out, float dist_side );

#define FX_STASIS_ALT_RIGHT_OFS	0.10
#define FX_STASIS_ALT_UP_OFS	0.02
#define FX_STASIS_ALT_MUZZLE_OFS	1
#define FX_MAXRANGE_STASIS		4096

/*
-------------------------
FX_StasisShot

Alt-fire, beam that shrinks to its impact point
-------------------------
*/

/*void FX_SmallStasisBeam(centity_t *cent, vec3_t start, vec3_t dir)
{
	vec3_t end, org, vel = { 0,0,-4};
	trace_t tr;
	float r;
	int i, ct, t;

	VectorMA(start, FX_MAXRANGE_STASIS, dir, end);
	CG_Trace(&tr, start, NULL, NULL, end, cent->currentState.number, MASK_SHOT);

	// Beam
//	FX_AddLine( start, tr.endpos, 1.0f, 3.0f, 4.0f, 0.8f, 0.0f, 400.0f, cgs.media.stasisAltShader);

	// Do a quick LOD for number of decay particles
	ct = tr.fraction * (FX_MAXRANGE_STASIS * 0.02);
	if ( ct < 12 )
		ct = 12;
	else if ( ct > 24 )
		ct = 24;

	for ( i = 0; i < ct; i++ )
	{
		r = random() * tr.fraction * (FX_MAXRANGE_STASIS * 0.5);
		VectorMA( start, r, dir, org );

		for ( t = 0; t < 3; t++ )
			org[t] += crandom();

		if ( rand() & 1 )
			FX_AddSprite( org, vel, qfalse, random() + 1.5, -3, 1.0, 1.0, 0.0, 0.0, 500, cgs.media.blueParticleShader);
		else
			FX_AddSprite( org, vel, qfalse, random() + 1.5, -3, 1.0, 1.0, 0.0, 0.0, 500, cgs.media.purpleParticleShader);	
	}

	// Impact graphic if needed.
	if (cg_entities[tr.entityNum].currentState.eType == ET_PLAYER)
	{	// Hit an entity.
		// Expanding rings
//		FX_AddSprite( tr.endpos, NULL, qfalse, 1, 60, 0.8, 0.2, random() * 360, 0, 400, cgs.media.stasisRingShader );
		// Impact effect
//		FX_AddSprite( tr.endpos, NULL, qfalse, 7, 25, 1.0, 0.0, random() * 360, 0, 500, cgs.media.blueParticleShader );
		FX_AddSprite( tr.endpos, NULL, qfalse, 5, 18, 1.0, 0.0, random() * 360, 0, 420, cgs.media.ltblueParticleShader );
	}
	else if (!(tr.surfaceFlags & SURF_NOIMPACT) )
	{
		// Move me away from the wall a bit so that I don't z-buffer into it
		VectorMA( tr.endpos, 1.5, tr.plane.normal, end);

		// Expanding rings
//		FX_AddQuad( end, tr.plane.normal, 1, 12, 0.8, 0.2, random() * 360, 400, cgs.media.stasisRingShader );
//		FX_AddQuad( end, tr.plane.normal, 1, 30, 0.8, 0.2, random() * 360, 300, cgs.media.stasisRingShader );
		// Impact effect
		FX_AddQuad( end, tr.plane.normal, 4, 16, 1.0, 0.0, random() * 360, 500, cgs.media.blueParticleShader );
		FX_AddQuad( end, tr.plane.normal, 3, 12, 1.0, 0.0, random() * 360, 420, cgs.media.ltblueParticleShader );

		CG_ImpactMark( cgs.media.scavMarkShader, end, tr.plane.normal, random()*360, 1,1,1,0.6, qfalse, 
					5 + random() * 2, qfalse );
	}

	FX_AddSprite( tr.endpos, NULL, qfalse, flrandom(40,60), -50, 1.0, 0.0, random() * 360, 0, 500, cgs.media.blueParticleShader );

	// Pass the end position back to the calling function (yes, I know).
	VectorCopy(tr.endpos, dir);
}*/


// kef -- fixme. the altfire stuff really wants to use some endcap stuff and some other flags
/*void FX_StasisShot( centity_t *cent, vec3_t end, vec3_t start )
{
	trace_t tr;
	vec3_t	fwd, newdir, org, vel = { 0,0,-4}, newstart, end2;
	int		i, t, ct;
	float	len, r;
	vec3_t	fwd2, right, up;
	int		bolt1, bolt2;
	vec3_t	bolt1vec, bolt2vec;
	centity_t		*traceEnt = NULL;
	int			clientNum = -1;

	// Choose which bolt will have the electricity accent.
	bolt1 = irandom(0,2);
	bolt2 = irandom(0,4);

	VectorSubtract( end, start, fwd );
	len = VectorNormalize( fwd );

	// Beam
//	FX_AddLine( end, start, 1.0f, 4.0f, 6.0f, 0.8f, 0.0f, 500.0f, cgs.media.stasisAltShader);

	// Do a quick LOD for number of decay particles
	ct = len * 0.03;
	if ( ct < 16 )
		ct = 16;
	else if ( ct > 32 )
		ct = 32;

	for ( i = 0; i < ct; i++ )
	{
		r = random() * len * 0.5;
		VectorMA( start, r, fwd, org );

		for ( t = 0; t < 3; t++ )
			org[t] += crandom();

		if ( rand() & 1 )
			FX_AddSprite( org, vel, qfalse, random() + 2, -4, 1.0, 1.0, 0.0, 0.0, 600, cgs.media.blueParticleShader);
		else
			FX_AddSprite( org, vel, qfalse, random() + 2, -4, 1.0, 1.0, 0.0, 0.0, 600, cgs.media.purpleParticleShader);	
	}
	VectorMA(start, FX_MAXRANGE_STASIS, fwd, end2);
	CG_Trace(&tr, start, NULL, NULL, end2, cent->currentState.number, MASK_SHOT);
	if (!( tr.surfaceFlags & SURF_NOIMPACT ))
	{
		traceEnt = &cg_entities[tr.entityNum];
		clientNum = traceEnt->currentState.clientNum;
		if ( (tr.entityNum != ENTITYNUM_WORLD) && (clientNum >= 0 || clientNum < MAX_CLIENTS) )
		{
			// hit a player
			FX_StasisShotImpact(tr.endpos, tr.plane.normal);
		}
		else
		{
			// hit the world
			FX_StasisShotMiss(tr.endpos, tr.plane.normal);
		}
	}
	// cap the impact end of the main beam to hide the nasty end of the line
	FX_AddSprite( tr.endpos, NULL, qfalse, flrandom(40,60), -50, 1.0, 0.0, random() * 360, 0, 500, cgs.media.blueParticleShader );

	if (bolt1==0)
	{
		VectorCopy(end, bolt1vec);
	}
	else if (bolt2==0)
	{
		VectorCopy(end, bolt2vec);
	}

	AngleVectors(cent->currentState.angles, fwd2, right, up);

//	CrossProduct(fwd, up, right);
//	VectorNormalize(right);			// "right" is scaled by the sin of the angle between fwd & up...  Ditch that.
//	CrossProduct(right, fwd, up);	// Change the "fake up" (0,0,1) to a "real up" (perpendicular to the forward vector).
	// VectorNormalize(up);			// If I cared about how the vertical variance looked when pointing up or down, I'd normalize this.

	// Fire a shot up and to the right.
	VectorMA(fwd, FX_STASIS_ALT_RIGHT_OFS, right, newdir);
	VectorMA(newdir, FX_STASIS_ALT_UP_OFS, up, newdir);
	VectorMA(start, FX_STASIS_ALT_MUZZLE_OFS, right, newstart);
	FX_SmallStasisBeam(cent, newstart, newdir);

	if (bolt1==1)
	{
		VectorCopy(newdir, bolt1vec);
	}
	else if (bolt2==1)
	{
		VectorCopy(newdir, bolt2vec);
	}

	// Fire a shot up and to the left.
	VectorMA(fwd, -FX_STASIS_ALT_RIGHT_OFS, right, newdir);
	VectorMA(newdir, FX_STASIS_ALT_UP_OFS, up, newdir);
	VectorMA(start, -FX_STASIS_ALT_MUZZLE_OFS, right, newstart);
	FX_SmallStasisBeam(cent, newstart, newdir);

	if (bolt1==2)
	{
		VectorCopy(newdir, bolt1vec);
	}
	else if (bolt2==2)
	{
		VectorCopy(newdir, bolt2vec);
	}

	// Fire a shot a bit down and to the right.
	VectorMA(fwd, 2.0*FX_STASIS_ALT_RIGHT_OFS, right, newdir);
	VectorMA(newdir, -0.5*FX_STASIS_ALT_UP_OFS, up, newdir);
	VectorMA(start, 2.0*FX_STASIS_ALT_MUZZLE_OFS, right, newstart);
	FX_SmallStasisBeam(cent, newstart, newdir);

	if (bolt1==3)
	{
		VectorCopy(newdir, bolt1vec);
	}
	else if (bolt2==3)
	{
		VectorCopy(newdir, bolt2vec);
	}

	// Fire a shot up and to the left.
	VectorMA(fwd, -2.0*FX_STASIS_ALT_RIGHT_OFS, right, newdir);
	VectorMA(newdir, -0.5*FX_STASIS_ALT_UP_OFS, up, newdir);
	VectorMA(start, -2.0*FX_STASIS_ALT_MUZZLE_OFS, right, newstart);
	FX_SmallStasisBeam(cent, newstart, newdir);

	if (bolt1==4)
	{
		VectorCopy(newdir, bolt1vec);
	}
	else if (bolt2==4)
	{
		VectorCopy(newdir, bolt2vec);
	}
		
	// Put a big gigant-mo sprite at the muzzle end so people can't see the crappy edges of the line
	FX_AddSprite( start, NULL, qfalse, random()*3 + 15, -20, 1.0, 0.5, 0.0, 0.0, 600, cgs.media.blueParticleShader);

	// Do an electrical arc to one of the impact points.
	FX_AddElectricity( start, bolt1vec, 0.2f, 15.0, -15.0, 1.0, 0.5, 100, cgs.media.dnBoltShader, 0.1 );

	if (bolt1!=bolt2)
	{
		// ALSO do an electrical arc to another point.
		FX_AddElectricity( bolt1vec, bolt2vec, 0.2f, 15.0, -15.0, 1.0, 0.5, flrandom(100,200), cgs.media.dnBoltShader, 0.5 );
	}
}*/

/*
-------------------------
FX_StasisShotImpact

Alt-fire, impact effect
-------------------------
*/
/*void FX_StasisShotImpact( vec3_t end, vec3_t dir )
{
	vec3_t	org;

	// Move me away from the wall a bit so that I don't z-buffer into it
	VectorMA( end, 1.5, dir, org );

	// Expanding rings
//	FX_AddQuad( org, dir, 1, 80, 0.8, 0.2, random() * 360, 400, cgs.media.stasisRingShader );
	// Impact effect
	FX_AddQuad( org, dir, 7, 35, 1.0, 0.0, random() * 360, 500, cgs.media.blueParticleShader );
	FX_AddQuad( org, dir, 5, 25, 1.0, 0.0, random() * 360, 420, cgs.media.ltblueParticleShader );

//	CG_ImpactMark( cgs.media.scavMarkShader, org, dir, random()*360, 1,1,1,0.6, qfalse, 
//				8 + random() * 2, qfalse );

//	FX_StasisDischarge( org, dir, irandom( 2,4 ), 24 + random() * 12, 64 + random() * 48 );
}*/

/*
-------------------------
FX_StasisShotMiss

Alt-fire, miss effect
-------------------------
*/
/*void FX_StasisShotMiss( vec3_t end, vec3_t dir )
{
	vec3_t	org;

	// Move me away from the wall a bit so that I don't z-buffer into it
	VectorMA( end, 0.5, dir, org );

	// Expanding rings
//	FX_AddQuad( org, dir, 1, 16, 0.8, 0.2, random() * 360, 400, cgs.media.stasisRingShader );
//	FX_AddQuad( org, dir, 1, 40, 0.8, 0.2, random() * 360, 300, cgs.media.stasisRingShader );
	// Impact effect
	FX_AddQuad( org, dir, 5, 25, 1.0, 0.0, random() * 360, 500, cgs.media.blueParticleShader );
	FX_AddQuad( org, dir, 4, 17, 1.0, 0.0, random() * 360, 420, cgs.media.ltblueParticleShader );

	CG_ImpactMark( cgs.media.scavMarkShader, org, dir, random()*360, 1,1,1,0.6, qfalse, 
				6 + random() * 2, qfalse );

	FX_AddSprite( end, NULL, qfalse, flrandom(40,60), -50, 1.0, 0.0, random() * 360, 0, 500, cgs.media.blueParticleShader );

//	FX_StasisDischarge( org, dir, irandom( 2,4 ), 24 + random() * 12, 64 + random() * 48 );
}*/

/*
-------------------------
FX_StasisProjectileThink

Main fire, with crazy bits swirling around main projectile
Hehe used to :D -TiM
-------------------------
*/
//unused
/*void FX_StasisProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	int			size = 0;
	vec3_t  forward;
	//vec3_t right, up;
	//float	radius, temp;
	vec3_t	org;

	if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0 )
		forward[2] = 1;

	VectorCopy( cent->lerpOrigin, org );

//	org[0] -=32;

//FX_AddTrail(

	FX_AddTrail( org, forward, qfalse, 64, 0, 30.4f, 0.0f, 0.6f, 0.0f, 0, 1, cgs.media.disruptorBolt );

	FX_AddSprite( cent->lerpOrigin, NULL, qfalse, 25.0f, 0.0f, 0.7f, 0.0f, 1.0f, 0.0f, 1.0f, cgs.media.disruptorStreak );

	
}*/

/*
-------------------------
FX_OrientedBolt

Creates new bolts for a while
-------------------------
*/

void FX_OrientedBolt( vec3_t start, vec3_t end, vec3_t dir )
{
	vec3_t	mid;

	VectorSubtract( end, start, mid );
	VectorScale( mid, 0.1f + (random() * 0.8), mid );
	VectorAdd( start, mid, mid );
	VectorMA(mid, 3.0f + (random() * 10.0f), dir, mid );

	//FX_AddElectricity( mid, start, 0.5, 0.75 + random() * 0.75, 0.0, 1.0, 0.5, 300.0f + random() * 300, cgs.media.bolt2Shader, DEFAULT_DEVIATION);
	//FX_AddElectricity( mid, end, 0.5, 0.75 + random() * 0.75, 1.0, 1.0, 0.5, 300.0f + random() * 300, cgs.media.bolt2Shader, DEFAULT_DEVIATION);

	FX_AddElectricity( mid, start, 0.5, 0.75 + random() * 0.75, 0.0, 1.0, 0.5, 300.0f + random() * 300, cgs.media.borgLightningShaders[2], DEFAULT_DEVIATION);
	FX_AddElectricity( mid, end, 0.5, 0.75 + random() * 0.75, 1.0, 1.0, 0.5, 300.0f + random() * 300, cgs.media.borgLightningShaders[3], DEFAULT_DEVIATION);
}

/*
-------------------------
FX_StasisDischarge

Fun "crawling" electricity ( credit goes to Josh for this one )
-------------------------
*/

void FX_StasisDischarge( vec3_t origin, vec3_t normal, int count, float dist_out, float dist_side )
{
	trace_t	trace;
	vec3_t	org, dir, dest;
	vec3_t	vr;
	int		i;
	int		discharge = dist_side;

	vectoangles( normal, dir );
	dir[ROLL] += random() * 360;

	for (i = 0;	i < count; i++)
	{
		//Move out a set distance
		VectorMA( origin, dist_out, normal, org );
		
		//Even out the hits
		dir[ROLL] += (360 / count) + (rand() & 31);
		AngleVectors( dir, NULL, vr, NULL );

		//Move to the side in a random direction
		discharge += (int)( crandom() * 8.0f );
		VectorMA( org, discharge, vr, org );

		//Trace back to find a surface
		VectorMA( org, -dist_out * 3, normal, dest );

		CG_Trace( &trace, org, NULL, NULL, dest, 0, MASK_SHOT );
		
		//No surface found, start over
		if (trace.fraction == 1) 
			continue;

		//Connect the two points with bolts
		FX_OrientedBolt( origin, trace.endpos, normal );

		//TiM : Aww screw it.  Add a lens flare. ^_^
		CG_InitLensFlare( trace.endpos, 
					10, 10,
					colorTable[CT_GREEN], 1.2, 2.0, 1600, 500,
					colorTable[CT_GREEN], 1600, 500, 100, 5,  qtrue, 
					0, 0, qfalse, qtrue, 
					qfalse, 1.0, cg.time, 0, 0, 300.0f + random() * 300);
	}
}

/*
-------------------------
FX_StasisWeaponHitWall

Main fire impact
-------------------------
*/

#define NUM_DISCHARGES		6
#define	DISCHARGE_DIST		8
#define	DISCHARGE_SIDE_DIST	24

void FX_StasisWeaponHitWall( vec3_t origin, vec3_t dir, int size )
{
	vec3_t			vel, /*accel,*/ hitpos, direction, org;
	//int				i, t;
	weaponInfo_t	*weaponInfo = &cg_weapons[WP_DISRUPTOR];

	CG_InitLensFlare( origin, 
					375, 375,
					colorTable[CT_GREEN], 1.2, 2.0, 1600, 200,
					colorTable[CT_GREEN], 1600, 200, 800, 20,  qtrue, 
					0, 0, qfalse, qtrue, 
					qfalse, 1.0, cg.time, 0, 0, 200);

	// Generate "crawling" electricity		// eh, don't it doesn't look that great.
	FX_StasisDischarge( origin, dir, NUM_DISCHARGES, DISCHARGE_DIST, DISCHARGE_SIDE_DIST );

	VectorMA(origin, size, dir, hitpos);

	// Set an oriented residual glow effect
	FX_AddQuad( hitpos, dir, size * size * 15.0f, -150.0f, 
				1.0f, 0.0f, 0, 300, cgs.media.greenParticleShader );

	CG_ImpactMark( cgs.media.scavMarkShader, origin, dir, random()*360, 1,1,1,0.6, qfalse, 
					size * 12 + 1, qfalse );

	FX_AddSprite( hitpos, NULL, qfalse, size * size * 15.0f, -150.0f, 
				1.0f, 0.0f, 360*random(), 0, 400, cgs.media.greenParticleShader );

/*	FX_AddSprite( hitpos, NULL, qfalse, size * size * 15.0f, -150.0f, 
				1.0f, 0.0f, 360*random(), 0, 400, cgs.media.greenParticleStreakShader ); */

	FX_AddSprite( hitpos, NULL, qfalse, size * size * 25.0f, -150.0f, 
				1.0f, 0.0f, 0.0f, 0, 400, cgs.media.greenParticleStreakShader );

	VectorSubtract( cg.refdef.vieworg, origin, direction );
	VectorNormalize( direction );

	VectorMA( origin, 12, direction, org );
	VectorMA( org, 8, dir, direction );
	VectorSet(vel, 0, 0, 32 ); //8

	FX_AddSprite( origin, 
						vel, qfalse, 
						random() * 4 + 2, 12,
						0.6 + random() * 0.4, 0.0,
						random() * 180, 
						0.0, 
						random() * 200 + 1200, //300
						cgs.media.steamShader );

	//FX_AddSprite(

	// Only play the impact sound and throw off the purple particles when it's the main projectile
/*	if ( size < 3 )
		return;

	for ( i = 0; i < 4; i++ )
	{
		for ( t = 0; t < 3; t++ )
			vel[t] = ( dir[t] + crandom() * 0.9 ) * ( random() * 100 + 250 );

		VectorScale( vel, -2.2, accel );
		FX_AddSprite( hitpos, vel, qfalse, random() * 8 + 8, 0, 1.0, 0.0, 0.0, 0.0, 200, cgs.media.purpleParticleShader );

	}*/
	trap_S_StartSound( origin, ENTITYNUM_WORLD, CHAN_AUTO, weaponInfo->mainHitSound );
}

void FX_DisruptorBeamFire( vec3_t startpos, vec3_t endpos, vec3_t normal, qboolean spark, qboolean impact, qboolean empty )
{
	refEntity_t		beam;
	sfxHandle_t		sfx;
	float			size;
	vec3_t			velocity;
	int				sparks;
	vec3_t			rgb = { 1,0.9,0.6}, rgb2={1,0.3,0};

	//vec3_t			rgb3 = { 1.0, 1.0, 1.0 };

	sfx = 0;

	// Draw beam first.
	memset( &beam, 0, sizeof( beam ) );

	VectorCopy( startpos, beam.origin);
	VectorCopy( endpos, beam.oldorigin );
	beam.reType = RT_LINE;
	if (empty)
	{
		beam.customShader = cgs.media.phaserEmptyShader;
	}
	else
	{
		beam.customShader = cgs.media.disruptorBeam;
	}
	AxisClear( beam.axis );
	beam.shaderRGBA[0] = 0xff;
	beam.shaderRGBA[1] = 0xff;
	beam.shaderRGBA[2] = 0xff;
	beam.shaderRGBA[3] = 0xff;
	if (empty)
	{
		beam.data.line.width = 1.0f + ( crandom() * 0.6f );
	}
	else
	{
		beam.data.line.width = 1.5f + ( crandom() * 0.6f );
	}
	beam.data.line.stscale = 5.0;
	trap_R_AddRefEntityToScene( &beam );

	// Now draw the hit graphic
 
	// no explosion at LG impact, it is added with the beam

	if ( sfx )
	{
		Com_Printf("playing %s\n", "phaser sound");
		trap_S_StartSound( endpos, ENTITYNUM_WORLD, CHAN_AUTO, sfx );
	}

	//
	// impact mark
	//
	if (impact)
	{
		if (!empty)
		{	// normal.
			CG_ImpactMark( cgs.media.scavMarkShader, endpos, normal, random()*360, 1,1,1,0.2, qfalse, 
						random() + 1, qfalse );
			
			//VectorCopy( endpos, phaserFlare.worldCoord );

			/*CG_InitLensFlare( endpos,
									80,
									80,
									rgb,
									1.2,
									1.5,
									1600,
									200,
									colorTable[CT_BLACK],
									1600,
									200,
									80,
									5,
									qfalse,
									5,
									40,
									qfalse,
									qfalse,
									qfalse,
									1.0,
									1.0,
									200.0,
									200.0,
									200.0 );*/
			
			//CG_InitLensFlare( endpos, 
			//		30, 30,
			//		rgb, 1.2, 2.0, 1600, 200,
			//		colorTable[CT_BLACK], 1600, 200, 410, 15, qfalse, 
			//		0, 0, qfalse, qtrue, 
			//		qfalse, 1.0, cg.time, 0, 0, 50);

			//TiM : Add your basic cheesy 'seen-way-too-much-in-movies-these-days' anamorphic lens streak :)
			//CG_DrawLensFlare( &phaserFlare );
			//FX_AddSprite( endpos, NULL, qfalse, random() * 1.25 + 5.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 50.0, cgs.media.blueParticleStreakShader ); //1.5f
			
			//FX_AddQuad2(  endpos, normal, random() * 1.25 + 8.0f, 0.0f, 1.0f, 1.0f, rgb3, rgb3, 270, 50.0, cgs.media.blueParticleStreakShader );
			//eh... looked bad :P

			FX_AddQuad2( endpos, normal, random() * 1.25 + 1.5f, 0.0f, 1.0f, 0.0f, rgb, rgb2, rand() % 360, 500 + random() * 200, 
						cgs.media.sunnyFlareShader );
		}
		else
		{	// Wuss hit when empty.
			FX_AddQuad2( endpos, normal, random() * .75 + 1.0f, 0.0f, 0.5f, 0.0f, rgb, rgb2, rand() % 360, 300 + random() * 200, 
						cgs.media.sunnyFlareShader );
		}
	}

	// "Fun" sparks...  Not when empty.
	if ( spark && !empty)
	{
		sparks = (rand() & 1) + 1;
		for(;sparks>0;sparks--)
		{	
			size = 0.2f + (random() * 0.4);
			FXE_Spray( normal, 200, 75, 0.8f, velocity);
			if (rand() & LEF_USE_COLLISION)
			{	// This spark bounces.
				FX_AddTrail( endpos, velocity, qtrue, 5.0f, -15.0f,
										size, -size, 1.0f, 0.5f, 0.4f, 500.0f, cgs.media.sparkShader);
			}
			else
			{
				FX_AddTrail( endpos, velocity, qtrue, 5.0f, -15.0f,
										size, -size, 1.0f, 0.5f, 0.0, 500.0f, cgs.media.sparkShader);
			}
		}
	}
}
