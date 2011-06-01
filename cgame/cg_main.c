// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_main.c -- initialization and primary entry point for cgame

//TiM : 22/12/2005 - Commented out any assets that we no longer use.
//TiM : 24/12/2005 - Added a ranks parsing function.

#include "cg_local.h"
#include "cg_text.h"

void CG_Init( int serverMessageNum, int serverCommandSequence );
void CG_Shutdown( void );
void CG_LoadIngameText(void);
void CG_LoadObjectivesForMap(void);
void BG_LoadItemNames(void);
qboolean CG_LoadUsablesStrings( void );

//TiM - Placed the func @ the bottom of the page for easier access
//extern static qboolean CG_LoadRanks( void );

extern void FX_InitSinTable(void);

//extern lensReflec_s lensReflec[10];

int cg_liftEnts[MAX_CLIENTS];
int	cg_numAnims;
int cg_numSndAnims;

animsSndList_t		cg_animsSndList[MAX_CLIENTS];

animsList_t			cg_animsList[MAX_CLIENTS];

/*
================
vmMain

This is the only way control passes into the module.
This must be the very first function compiled into the .q3vm file
================
*/
int vmMain( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6 ) {
	switch ( command ) {
	case CG_INIT:
		CG_Init( arg0, arg1 );
		return 0;
	case CG_SHUTDOWN:
		CG_Shutdown();
		return 0;
	case CG_CONSOLE_COMMAND:
		return CG_ConsoleCommand();
	case CG_DRAW_ACTIVE_FRAME:
		CG_DrawActiveFrame( arg0, arg1, arg2 );
		return 0;
	case CG_CROSSHAIR_PLAYER:
		return CG_CrosshairPlayer();
	case CG_LAST_ATTACKER:
		return CG_LastAttacker();
	default:
		CG_Error( "vmMain: unknown command %i", command );
		break;
	}
	return -1;
}


cg_t				cg;
cgs_t				cgs;
centity_t			cg_entities[MAX_GENTITIES];
weaponInfo_t		cg_weapons[MAX_WEAPONS];
itemInfo_t			cg_items[MAX_ITEMS];

vmCvar_t	cg_centertime;
vmCvar_t	cg_runpitch;
vmCvar_t	cg_runroll;
vmCvar_t	cg_bobup;
vmCvar_t	cg_bobpitch;
vmCvar_t	cg_bobroll;
vmCvar_t	cg_swingSpeed;
vmCvar_t	cg_shadows;
vmCvar_t	cg_gibs;
vmCvar_t	cg_drawTimer;
vmCvar_t	cg_drawFPS;
vmCvar_t	cg_drawSnapshot;
vmCvar_t	cg_draw3dIcons;
vmCvar_t	cg_drawIcons;
vmCvar_t	cg_drawAmmoWarning;
vmCvar_t	cg_drawCrosshair;
vmCvar_t	cg_drawCrosshairNames;
vmCvar_t	cg_dynamicCrosshairNames;
vmCvar_t	cg_drawRewards;
vmCvar_t	cg_crosshairSize;
vmCvar_t	cg_crosshairX;
vmCvar_t	cg_crosshairY;
vmCvar_t	cg_crosshairHealth;
vmCvar_t	cg_draw2D;
vmCvar_t	cg_drawStatus;
vmCvar_t	cg_animSpeed;
vmCvar_t	cg_debugAnim;
vmCvar_t	cg_debugPosition;
vmCvar_t	cg_debugEvents;
vmCvar_t	cg_errorDecay;
vmCvar_t	cg_nopredict;
vmCvar_t	cg_noPlayerAnims;
vmCvar_t	cg_showmiss;
vmCvar_t	cg_footsteps;
vmCvar_t	cg_addMarks;
vmCvar_t	cg_viewsize;
vmCvar_t	cg_drawGun;
vmCvar_t	cg_gun_frame;
vmCvar_t	cg_gun_x;
vmCvar_t	cg_gun_y;
vmCvar_t	cg_gun_z;
vmCvar_t	cg_autoswitch;
vmCvar_t	cg_ignore;
vmCvar_t	cg_simpleItems;
vmCvar_t	cg_fov;
vmCvar_t	cg_zoomFov;
vmCvar_t	cg_thirdPerson;
vmCvar_t	cg_thirdPersonRange;
vmCvar_t	cg_thirdPersonAngle;
//RPG-X: TiM - Cool JKA CVARs
vmCvar_t	cg_thirdPersonVertOffset;
vmCvar_t	cg_thirdPersonHorzOffset;
vmCvar_t	cg_thirdPersonAlpha;
vmCvar_t	cg_thirdPersonCameraDamp;
vmCvar_t	cg_thirdPersonTargetDamp;
vmCvar_t	cg_thirdPersonPitchOffset;
vmCvar_t	cg_stereoSeparation;
vmCvar_t	cg_lagometer;
vmCvar_t	cg_drawAttacker;
vmCvar_t	cg_synchronousClients;
vmCvar_t 	cg_teamChatTime;
vmCvar_t 	cg_teamChatHeight;
vmCvar_t 	cg_stats;
vmCvar_t 	cg_reportDamage;
vmCvar_t 	cg_buildScript;
vmCvar_t 	cg_forceModel;
vmCvar_t	cg_paused;
vmCvar_t	cg_blood;

vmCvar_t	cg_predictItems;
vmCvar_t	cg_deferPlayers;
vmCvar_t	cg_drawTeamOverlay;
vmCvar_t	cg_teamOverlayUserinfo;
vmCvar_t	ui_playerClass;
vmCvar_t	ui_playerRank;
vmCvar_t	cg_disablekillmsgs;
vmCvar_t	cg_drawradar;
vmCvar_t	rpg_ctribgrenade;	//RPG-X: - RedTechie i think J2J added this to control invisible tripmines on clients end - TOFIX:THIS ALSO COULD BE HACKED!
vmCvar_t	cg_dynamicCrosshair;	//RPG-X | Phenix | 09/06/2005
vmCvar_t	doomHead;	//RPG-X | Phenix | 09/06/2005
vmCvar_t	cg_dynamiclensflares;
vmCvar_t	cg_noTalkingHeads;
vmCvar_t	cg_noDynamicRanks;

vmCvar_t	noAdminChat; //TiM

//RPG-X: TiM - Player Model Parameters
vmCvar_t	pms_age;
vmCvar_t	pms_height;
vmCvar_t	pms_weight;
vmCvar_t	pms_race;

vmCvar_t	cg_defaultChar;

//RPG-X: TiM - Emote system model offset
vmCvar_t	emote_Offset;

vmCvar_t	cg_thirdPersonZoomRate;
vmCvar_t	cg_noFrowningHeads;
vmCvar_t	cg_noBlinkingHeads;

//RPG-X: TiM - First Person Body
vmCvar_t	cg_firstPersonBody;

vmCvar_t	cg_defaultModel;

vmCvar_t	cg_showEntityNums;

//TiM - SecurityCode
vmCvar_t	sv_securityHash;
vmCvar_t	sv_securityCode;

//TiM - widescreen
vmCvar_t	cg_handleWidescreen;
vmCvar_t	ui_handleWidescreen;

//vmCvar_t	cg_chatBGColor;

vmCvar_t	cg_chatColor;

//RPG-X | GSIO01 | 11/05/2009
vmCvar_t	rpg_forceFieldSet;

// grp cvars
vmCvar_t	grp_berp;


//RPG-X | Phenix | 05/02/2006
//Ban System (and it's backup cvars)
//vmCvar_t	cg_playerID;
//vmCvar_t	s_mhz; //Part A + 562
//vmCvar_t	cg_fow; //Part B + 333
//vmCvar_t	cl_avgPacket; //Part C + 99
//vmCvar_t	cg_rewardsSize;//Part D + 120

typedef struct {
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;
} cvarTable_t;

static cvarTable_t	cvarTable[] = {
	{ &cg_ignore, "cg_ignore", "0", 0 },	// used for debugging
	//{ &s_mhz, "s_mhz", "0", CVAR_ARCHIVE | CVAR_ROM | CVAR_NORESTART }, //RPG-X | Phenix | 05/02/2006
	{ &cg_autoswitch, "cg_autoswitch", "1", CVAR_ARCHIVE },
	{ &cg_drawGun, "cg_drawGun", "1", CVAR_ARCHIVE },
	{ &cg_zoomFov, "cg_zoomfov", "22.5", CVAR_ARCHIVE },
	{ &cg_fov, "cg_fov", "80", CVAR_ARCHIVE },
	//{ &cg_fow, "cg_fow", "0", CVAR_ARCHIVE | CVAR_ROM | CVAR_NORESTART }, //RPG-X | Phenix | 05/02/2006
	{ &cg_viewsize, "cg_viewsize", "100", CVAR_ARCHIVE },
	{ &cg_stereoSeparation, "cg_stereoSeparation", "0.4", CVAR_ARCHIVE  },
	//{ &s_mhz, "s_mhz", "0", CVAR_ARCHIVE | CVAR_ROM | CVAR_NORESTART }, //RPG-X | Phenix | 05/02/2006
	{ &cg_shadows, "cg_shadows", "1", CVAR_ARCHIVE  },
	{ &cg_gibs, "cg_gibs", "0", CVAR_ARCHIVE  },	//no gibs in trek
	{ &cg_draw2D, "cg_draw2D", "1", CVAR_ARCHIVE  },
	{ &cg_drawStatus, "cg_drawStatus", "1", CVAR_ARCHIVE  },
	{ &cg_drawTimer, "cg_drawTimer", "0", CVAR_ARCHIVE  },
	{ &cg_drawFPS, "cg_drawFPS", "0", CVAR_ARCHIVE  },
	{ &cg_drawSnapshot, "cg_drawSnapshot", "0", CVAR_ARCHIVE  },
	{ &cg_draw3dIcons, "cg_draw3dIcons", "1", CVAR_ARCHIVE  },
	{ &cg_drawIcons, "cg_drawIcons", "1", CVAR_ARCHIVE  },
	{ &cg_drawAmmoWarning, "cg_drawAmmoWarning", "1", CVAR_ARCHIVE  },
	{ &cg_drawAttacker, "cg_drawAttacker", "0", CVAR_ARCHIVE  }, //RPG-X TiM
	{ &cg_drawCrosshair, "cg_drawCrosshair", "1", CVAR_ARCHIVE },
	{ &cg_drawCrosshairNames, "cg_drawCrosshairNames", "1", CVAR_ARCHIVE },
	{ &cg_drawRewards, "cg_drawRewards", "1", CVAR_ARCHIVE },
	//{ &cg_rewardsSize, "cg_rewardsSize", "0", CVAR_ARCHIVE | CVAR_ROM | CVAR_NORESTART }, //RPG-X | Phenix | 05/02/2006
	{ &cg_crosshairSize, "cg_crosshairSize", "24", CVAR_ARCHIVE },
	{ &cg_crosshairHealth, "cg_crosshairHealth", "1", CVAR_ARCHIVE },
	{ &cg_crosshairX, "cg_crosshairX", "0", CVAR_ARCHIVE },
	{ &cg_crosshairY, "cg_crosshairY", "0", CVAR_ARCHIVE },
	{ &cg_simpleItems, "cg_simpleItems", "0", CVAR_ARCHIVE },
	{ &cg_addMarks, "cg_marks", "1", CVAR_ARCHIVE },
	{ &cg_lagometer, "cg_lagometer", "0", CVAR_ARCHIVE },
	{ &cg_gun_x, "cg_gunX", "0", CVAR_CHEAT },
	{ &cg_gun_y, "cg_gunY", "0", CVAR_CHEAT },
	{ &cg_gun_z, "cg_gunZ", "0", CVAR_CHEAT },
	{ &cg_centertime, "cg_centertime", "3", CVAR_CHEAT },
	{ &cg_runpitch, "cg_runpitch", "0.002", CVAR_ARCHIVE},
	{ &cg_runroll, "cg_runroll", "0.005", CVAR_ARCHIVE },
	{ &cg_bobup , "cg_bobup", "0.005", CVAR_ARCHIVE },
	{ &cg_bobpitch, "cg_bobpitch", "0.002", CVAR_ARCHIVE },
	{ &cg_bobroll, "cg_bobroll", "0.002", CVAR_ARCHIVE },
	{ &cg_swingSpeed, "cg_swingSpeed", "0.2", CVAR_CHEAT }, //0.3 //TiM - slowed for better realism
	{ &cg_animSpeed, "cg_animspeed", "1", CVAR_CHEAT },
	{ &cg_debugAnim, "cg_debuganim", "0", CVAR_CHEAT },
	{ &cg_debugPosition, "cg_debugposition", "0", CVAR_CHEAT },
	{ &cg_debugEvents, "cg_debugevents", "0", CVAR_CHEAT },
	{ &cg_errorDecay, "cg_errordecay", "100", 0 },
	{ &cg_nopredict, "cg_nopredict", "0", 0 },
	{ &cg_noPlayerAnims, "cg_noplayeranims", "0", CVAR_CHEAT },
	{ &cg_showmiss, "cg_showmiss", "0", 0 },
	{ &cg_footsteps, "cg_footsteps", "1", CVAR_CHEAT },
	{ &cg_thirdPersonRange, "cg_thirdPersonRange", "80", CVAR_ARCHIVE },
	{ &cg_thirdPersonAngle, "cg_thirdPersonAngle", "0", CVAR_ARCHIVE },
	{ &cg_thirdPerson, "cg_thirdPerson", "0", CVAR_ARCHIVE },
	{ &cg_thirdPersonVertOffset, "cg_thirdPersonVertOffset", "16", CVAR_ARCHIVE }, //RPG-X: TiM
	{ &cg_thirdPersonHorzOffset, "cg_thirdPersonHorzOffset", "0", CVAR_ARCHIVE }, //RPG-X: TiM
	{ &cg_thirdPersonAlpha, "cg_thirdPersonAlpha", "1.0", CVAR_ARCHIVE },//RPG-X: TiM
	{ &cg_thirdPersonCameraDamp, "cg_thirdPersonCameraDamp", "0.3", CVAR_ARCHIVE },//RPG-X: TiM
	{ &cg_thirdPersonTargetDamp, "cg_thirdPersonTargetDamp", "0.5", CVAR_ARCHIVE },//RPG-X: TiM
	{ &cg_thirdPersonPitchOffset, "cg_thirdPersonPitchOffset", "0.0", CVAR_ARCHIVE},//RPG-X: TiM
	{ &cg_firstPersonBody, "cg_firstPersonBody", "0", CVAR_ARCHIVE},
	{ &cg_teamChatTime, "cg_teamChatTime", "3000", CVAR_ARCHIVE  },
	{ &cg_teamChatHeight, "cg_teamChatHeight", "0", CVAR_ARCHIVE  },
	{ &cg_forceModel, "cg_forceModel", "0", CVAR_ARCHIVE  },
	{ &cg_predictItems, "cg_predictItems", "1", CVAR_ARCHIVE },
	{ &cg_deferPlayers, "cg_deferPlayers", "1", CVAR_ARCHIVE },
	{ &cg_drawTeamOverlay, "cg_drawTeamOverlay", "0", CVAR_ARCHIVE },
	{ &cg_teamOverlayUserinfo, "teamoverlay", "0", CVAR_ROM | CVAR_USERINFO },
	{ &cg_stats, "cg_stats", "0", 0 },
	{ &cg_reportDamage, "cg_reportDamage", "0", 0},
	{ &rpg_ctribgrenade, "rpg_ctribgrenade", "0", CVAR_ARCHIVE},	//RPG-X: - RedTechie i think J2J added this to control invisible tripmines on clients end - TOFIX:THIS ALSO COULD BE HACKED!
	{ &pms_age, "age", "Unknown", CVAR_ARCHIVE | CVAR_USERINFO },
	{ &pms_height, "height", "1.0", CVAR_ARCHIVE | CVAR_USERINFO },
	{ &pms_weight, "weight", "1.0", CVAR_ARCHIVE | CVAR_USERINFO },
	{ &pms_race, "race", "Unknown", CVAR_ARCHIVE | CVAR_USERINFO },
	{ &emote_Offset, "modelOffset", "0", CVAR_ARCHIVE | CVAR_USERINFO },

	{ &cg_defaultChar, "cg_defaultChar", DEFAULT_CHAR, CVAR_ARCHIVE },

	// the following variables are created in other parts of the system,
	// but we also reference them here

	{ &cg_buildScript, "com_buildScript", "0", 0 },	// force loading of all possible data amd error on failures
	{ &cg_paused, "cl_paused", "0", CVAR_ROM },
	{ &cg_blood, "com_blood", "0", CVAR_ARCHIVE },	//no blood in trek
	//{ &cl_avgPacket, "cl_avgPacket", "0", CVAR_ARCHIVE | CVAR_ROM | CVAR_NORESTART }, //RPG-X | Phenix | 05/02/2006
	{ &cg_synchronousClients, "g_synchronousClients", "0", 0 },	// communicated by systeminfo
	{ &ui_playerClass, "ui_playerClass", "noclass", CVAR_ARCHIVE /*| CVAR_ROM | CVAR_USERINFO*/ },
	//{ &ui_playerclass, "ui_playerclass", "0", 0 },	// player class
	{ &ui_playerRank, "ui_playerRank", "crewman", CVAR_ARCHIVE /*| CVAR_ROM | CVAR_USERINFO*/ },

	{ &cg_disablekillmsgs, "cg_disablekillmsgs", "0", CVAR_ARCHIVE },
	{ &cg_drawradar, "cg_drawradar", "1", CVAR_ARCHIVE },

	{ &cg_dynamicCrosshair, "cg_dynamicCrosshair", "1", CVAR_ARCHIVE },	//RPG-X | Phenix | 09/06/2005
	{ &doomHead, "doomHead", "0", CVAR_ARCHIVE },	//RPG-X | Phenix | 09/06/2005
	{ &cg_dynamiclensflares, "cg_dynamicLensFlares", "1", CVAR_ARCHIVE }, //RPG-X | TiM | 29/6/2005

	{ &cg_dynamicCrosshairNames, "cg_dynamicCrosshairNames", "1", CVAR_ARCHIVE },

	{ &noAdminChat, "noAdminChat", "0", CVAR_ARCHIVE | CVAR_USERINFO },

	//RPG-X Memory optimization CVARs
	{ &cg_noTalkingHeads, "cg_noTalkingHeads", "0", CVAR_ARCHIVE },
	{ &cg_noDynamicRanks, "cg_noDynamicRanks", "0", CVAR_ARCHIVE },
	{ &cg_noFrowningHeads, "cg_noFrowningHeads", "1", CVAR_ARCHIVE }, //On by default since this isn't REALLY needed... :P
	{ &cg_noBlinkingHeads, "cg_noBlinkingHeads", "0", CVAR_ARCHIVE },

	{ &cg_thirdPersonZoomRate, "cg_thirdPersonZoomRate", "25", CVAR_ARCHIVE },
	{ &cg_showEntityNums, "cg_showEntityNums", "1", CVAR_ARCHIVE },
	
	//TiM - security cvars
	{ &sv_securityHash, "sv_securityHash", "4294967295", CVAR_ARCHIVE | CVAR_ROM | CVAR_NORESTART },
	{ &sv_securityCode, "sv_securityCode", "4294967295", CVAR_ARCHIVE | CVAR_USERINFO | CVAR_ROM | CVAR_NORESTART },	
	
	{ &cg_handleWidescreen, "cg_handleWidescreen", "1", CVAR_ARCHIVE },
	{ &ui_handleWidescreen, "ui_handleWidescreen", "1", CVAR_ARCHIVE },

	{ &cg_chatColor, "cg_chatColor", "", CVAR_ARCHIVE },

	//RPG-X | GSIO01 | 11/05/2009
	{ &rpg_forceFieldSet, "rpg_forceFieldSet", "1", CVAR_ARCHIVE | CVAR_LATCH },

	//{ &cg_chatBGColor, "cg_chatBGColor", "", CVAR_ARCHIVE }

	//{ &cg_defaultModel, "cg_defaultModel", DEFAULT_PLAYER, CVAR_ARCHIVE },

	//{ &cg_playerID, "cg_playerID", "0", CVAR_ARCHIVE | CVAR_ROM | CVAR_NORESTART }

	// grp cvars
	{ &grp_berp, "grp_berp", "0", CVAR_ARCHIVE | CVAR_LATCH }
};

static int	cvarTableSize = sizeof( cvarTable ) / sizeof( cvarTable[0] );

/*
=================
CG_RegisterCvars
=================
*/
void CG_RegisterCvars( void ) {
	int			i;
	cvarTable_t	*cv;
	char		var[MAX_TOKEN_CHARS];

	for ( i = 0, cv = cvarTable ; i < cvarTableSize ; i++, cv++ ) {
		trap_Cvar_Register( cv->vmCvar, cv->cvarName,
			cv->defaultString, cv->cvarFlags );
	}

	// see if we are also running the server on this machine
	trap_Cvar_VariableStringBuffer( "sv_running", var, sizeof( var ) );
	cgs.localServer = atoi( var );
}


/*
=================
CG_UpdateCvars
=================
*/
void CG_UpdateCvars( void ) {
	int			i;
	cvarTable_t	*cv;

	for ( i = 0, cv = cvarTable ; i < cvarTableSize ; i++, cv++ )
	{
		trap_Cvar_Update( cv->vmCvar );
	}

	// check for modications here

	// If team overlay is on, ask for updates from the server.  If its off,
	// let the server know so we don't receive it
	if ( drawTeamOverlayModificationCount != cg_drawTeamOverlay.modificationCount ) {
		drawTeamOverlayModificationCount = cg_drawTeamOverlay.modificationCount;

		if ( cg_drawTeamOverlay.integer > 0 ) {
			trap_Cvar_Set( "teamoverlay", "1" );
		} else {
			trap_Cvar_Set( "teamoverlay", "0" );
		}
	}
}


int CG_CrosshairPlayer( void ) {
	if ( cg.time > ( cg.crosshairClientTime + 1000 ) ) {
		return -1;
	}
	return cg.crosshairClientNum;
}


int CG_LastAttacker( void ) {
	if ( !cg.attackerTime ) {
		return -1;
	}
	return cg.snap->ps.persistant[PERS_ATTACKER];
}


void QDECL CG_Printf( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];
	char		*msgPtr;

	va_start (argptr, msg);
	vsprintf (text, msg, argptr);
	va_end (argptr);

	if ( cg_chatColor.integer > 0 && cg_chatColor.integer < 8 )
	{
		msgPtr = text;

		while ( msgPtr && *msgPtr != '\0' )
		{
			if ( *msgPtr == '^' )
			{
				msgPtr++;
				*msgPtr = 48 + cg_chatColor.integer;
			}

			msgPtr++;
		}
	}

	//CVAR background
	/*if ( cg_chatBGColor.string[0] )
	{
		char	*bgColor;
		char	*rimColor;
		int		rgb[3];
		int		i;

		Q_strlwr( cg_chatBGColor.string );

		bgColor = cg_chatBGColor.string;

		if ( *bgColor == '#' )
		{
			bgColor[7]='\0';
			bgColor++;

			for ( i=0; i < 3; i++ )
			{
				if ( *bgColor >= 'a' && *bgColor <='f' )
					rgb[i] = 16 * ( 10 + (int)(*bgColor - 'a'));
				else
					rgb[i] = 16 * atoi(bgColor[0]);

				bgColor++;

				if ( *bgColor >= 'a' && *bgColor <='f' )
					rgb[i] += ( 10 + (int)(*bgColor - 'a'));
				else
					rgb[i] += atoi(bgColor[0]);
			}


		}
	}*/

	trap_Print( text );
}

void QDECL CG_Error( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	vsprintf (text, msg, argptr);
	va_end (argptr);

	trap_Error( text );
}

#ifndef CGAME_HARD_LINKED
// this is only here so the functions in q_shared.c and bg_*.c can link (FIXME)

void QDECL Com_Error( int level, const char *error, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, error);
	vsprintf (text, error, argptr);
	va_end (argptr);

	CG_Error( "%s", text);
}

void QDECL Com_Printf( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	vsprintf (text, msg, argptr);
	va_end (argptr);

	CG_Printf ("%s", text);
}

#endif



/*
================
CG_Argv
================
*/
const char *CG_Argv( int arg ) {
	static char	buffer[MAX_STRING_CHARS];

	trap_Argv( arg, buffer, sizeof( buffer ) );

	return buffer;
}


//========================================================================

/*
=================
CG_RegisterItemSounds

The server says this item is used on this level
=================
*/
static void CG_RegisterItemSounds( int itemNum ) {
	gitem_t			*item;
	char			data[MAX_QPATH];
	char			*s, *start;
	int				len;

	item = &bg_itemlist[ itemNum ];

	if ( item->pickup_sound )
	{
		trap_S_RegisterSound( item->pickup_sound );
	}

	// parse the space seperated precache string for other media
	s = item->sounds;
	if (!s || !s[0])
		return;

	while (*s) {
		start = s;
		while (*s && *s != ' ') {
			s++;
		}

		len = s-start;
		if (len >= MAX_QPATH || len < 5) {
			CG_Error( "PrecacheItem: %s has bad precache string", 
				item->classname);
			return;
		}
		memcpy (data, start, len);
		data[len] = 0;
		if ( *s ) {
			s++;
		}

		if ( !strcmp(data+len-3, "wav" )) {
			trap_S_RegisterSound( data );
		}
	}
}


/*
=================
CG_RegisterSounds

called during a precache command
=================
*/
static void CG_RegisterSounds( void )
{
	int		i;
	char	items[MAX_ITEMS+1];
	char	name[MAX_QPATH];
	const char	*soundName;

	cg.loadLCARSStage = 1;	// Loading bar stage 1
	CG_LoadingString( "sounds" );

	//TiM
	/*if ( cgs.timelimit || cg_buildScript.integer ) {	// should we always load this?
		cgs.media.oneMinuteSound = trap_S_RegisterSound( "sound/voice/computer/misc/1_minute.wav" );
		cgs.media.fiveMinuteSound = trap_S_RegisterSound( "sound/voice/computer/misc/5_minute.wav" );
		cgs.media.suddenDeathSound = trap_S_RegisterSound( "sound/voice/computer/misc/sudden_death.wav" );
	}*/

	//TiM
	/*if ( cgs.fraglimit || cg_buildScript.integer ) {
		cgs.media.oneFragSound = trap_S_RegisterSound( "sound/voice/computer/misc/1_frag.wav" );
		cgs.media.twoFragSound = trap_S_RegisterSound( "sound/voice/computer/misc/2_frags.wav" );
		cgs.media.threeFragSound = trap_S_RegisterSound( "sound/voice/computer/misc/3_frags.wav" );
	}*/

//	if ( cgs.gametype == GT_TOURNAMENT || cg_buildScript.integer ) {
//  We always need this since a warmup can be enabled in any game mode
//TiM	/*cgs.media.count3Sound = trap_S_RegisterSound( "sound/voice/computer/misc/three.wav" );
		cgs.media.count2Sound = trap_S_RegisterSound( "sound/voice/computer/misc/two.wav" );
		cgs.media.count1Sound = trap_S_RegisterSound( "sound/voice/computer/misc/one.wav" );
		cgs.media.countFightSound = trap_S_RegisterSound( "sound/voice/computer/misc/fight.wav" );
		cgs.media.countPrepareSound = trap_S_RegisterSound( "sound/voice/computer/misc/prepare.wav" );
//	}
	//TiM
/*	if ( cgs.gametype >= GT_TEAM || cg_buildScript.integer ) {
		cgs.media.redLeadsSound = trap_S_RegisterSound( "sound/voice/computer/misc/redleads.wav" );
		cgs.media.blueLeadsSound = trap_S_RegisterSound( "sound/voice/computer/misc/blueleads.wav" );
		cgs.media.teamsTiedSound = trap_S_RegisterSound( "sound/voice/computer/misc/teamstied.wav" );
		cgs.media.hitTeamSound = trap_S_RegisterSound( "sound/feedback/hit_teammate.wav" );
	}*/

	//TiM
	/*if (cgs.gametype == GT_CTF || cg_buildScript.integer)
	{
		cgs.media.ctfStealSound = trap_S_RegisterSound("sound/voice/computer/misc/flagtk_blu.wav");
		cgs.media.ctfReturnSound = trap_S_RegisterSound("sound/voice/computer/misc/flagret_blu.wav");
		cgs.media.ctfScoreSound = trap_S_RegisterSound("sound/voice/computer/misc/flagcap_blu.wav");
		cgs.media.ctfYouStealVoiceSound = trap_S_RegisterSound("sound/voice/computer/misc/stolen.wav");
		cgs.media.ctfYouDroppedVoiceSound = trap_S_RegisterSound("sound/voice/computer/misc/dropped_e.wav");
		cgs.media.ctfYouReturnVoiceSound = trap_S_RegisterSound("sound/voice/computer/misc/returned.wav");
		cgs.media.ctfYouScoreVoiceSound = trap_S_RegisterSound("sound/voice/computer/misc/scored.wav");
		cgs.media.ctfTheyStealVoiceSound = trap_S_RegisterSound("sound/voice/computer/misc/stolen_e.wav");
		cgs.media.ctfTheyDroppedVoiceSound = trap_S_RegisterSound("sound/voice/computer/misc/dropped.wav");	// Note the flip, because YOU dropped THEIR flag
		cgs.media.ctfTheyReturnVoiceSound = trap_S_RegisterSound("sound/voice/computer/misc/returned_e.wav");
		cgs.media.ctfTheyScoreVoiceSound = trap_S_RegisterSound("sound/voice/computer/misc/scored_e.wav");
	}*/

	cgs.media.interfaceSnd1 = trap_S_RegisterSound( "sound/interface/button4.wav" );

	//cgs.media.selectSound = trap_S_RegisterSound( "sound/silence.wav" );//trap_S_RegisterSound( "sound/weapons/change.wav" );
	//cgs.media.wearOffSound = trap_S_RegisterSound( "sound/items/wearoff.wav" );
	cgs.media.useNothingSound = trap_S_RegisterSound( "sound/items/use_nothing.wav" );

	//TiM//cgs.media.holoOpenSound = trap_S_RegisterSound( "sound/movers/doors/holoopen.wav" );
	cgs.media.teleInSound = trap_S_RegisterSound( "sound/world/transin.wav" );
	//cgs.media.teleOutSound = trap_S_RegisterSound( "sound/world/transout.wav" );
	cgs.media.transportSound = trap_S_RegisterSound( "sound/world/transporter.wav" );
	cgs.media.respawnSound = trap_S_RegisterSound( "sound/items/respawn1.wav" );

	//cgs.media.noAmmoSound = trap_S_RegisterSound( "sound/weapons/noammo.wav" );

	cgs.media.talkSound = trap_S_RegisterSound( "sound/interface/communicator.wav" );
	
	//cgs.media.landSound = trap_S_RegisterSound( "sound/player/land1.wav");
	//RPG-X | GSIO01 | 20/05/2009:
	cgs.media.landSound[LANDSOUND_NORMAL] = trap_S_RegisterSound("sound/player/land1.wav");
	cgs.media.landSound[LANDSOUND_GRASS]  = trap_S_RegisterSound("sound/player/footsteps/grass_jump.wav");
	cgs.media.landSound[LANDSOUND_GRAVEL] = trap_S_RegisterSound("sound/player/footsteps/gravel_jump.wav");
	cgs.media.landSound[LANDSOUND_SNOW]	  = trap_S_RegisterSound("sound/player/footsteps/snow_jump.wav");
	cgs.media.landSound[LANDSOUND_WOOD]	  = trap_S_RegisterSound("sound/player/footsteps/wood_jump.wav");

	cgs.media.splatSound = trap_S_RegisterSound( "sound/weapons/bodyfall.wav");

	//cgs.media.hitSound = trap_S_RegisterSound( "sound/feedback/hit.wav" );
	//cgs.media.shieldHitSound = trap_S_RegisterSound( "sound/feedback/shieldHit.wav" );
	//cgs.media.shieldPierceSound = trap_S_RegisterSound( "sound/feedback/shieldPierce.wav" );

	//TiM
	/*cgs.media.rewardImpressiveSound		= trap_S_RegisterSound( "sound/voice/computer/misc/impressive.wav" );
	cgs.media.rewardExcellentSound		= trap_S_RegisterSound( "sound/voice/computer/misc/excellent.wav" );
	cgs.media.rewardDeniedSound			= trap_S_RegisterSound( "sound/voice/computer/misc/denied.wav" );
	cgs.media.rewardFirstStrikeSound	= trap_S_RegisterSound( "sound/voice/computer/misc/1ststrike.wav");
	cgs.media.rewardAceSound			= trap_S_RegisterSound( "sound/voice/computer/misc/ace.wav");
	cgs.media.rewardExpertSound			= trap_S_RegisterSound( "sound/voice/computer/misc/expert.wav");
	cgs.media.rewardMasterSound			= trap_S_RegisterSound( "sound/voice/computer/misc/master.wav");
	cgs.media.rewardChampionSound		= trap_S_RegisterSound( "sound/voice/computer/misc/champion.wav");*/

	//TiM
	/*cgs.media.takenLeadSound = trap_S_RegisterSound( "sound/voice/computer/misc/takenlead.wav");
	cgs.media.tiedLeadSound = trap_S_RegisterSound( "sound/voice/computer/misc/tiedlead.wav");
	cgs.media.lostLeadSound = trap_S_RegisterSound( "sound/voice/computer/misc/lostlead.wav");*/

	cgs.media.watrInSound = trap_S_RegisterSound( "sound/player/watr_in.wav");
	cgs.media.watrOutSound = trap_S_RegisterSound( "sound/player/watr_out.wav");
	cgs.media.watrUnSound = trap_S_RegisterSound( "sound/player/watr_un.wav");

	cgs.media.jumpPadSound = trap_S_RegisterSound ("sound/items/damage3.wav" );

	//cgs.media.poweruprespawnSound = trap_S_RegisterSound ("sound/items/poweruprespawn.wav");
	cgs.media.disintegrateSound = trap_S_RegisterSound( "sound/weapons/prifle/disint.wav" );
	cgs.media.disintegrate2Sound = trap_S_RegisterSound( "sound/weapons/prifle/disint2.wav" );
	cgs.media.playerExplodeSound = trap_S_RegisterSound( "sound/weapons/explosions/fireball.wav" );

	//TiM
	/*cgs.media.holoInitSound = trap_S_RegisterSound("sound/voice/computer/misc/proginit.wav");
	cgs.media.holoDoorSound = trap_S_RegisterSound("sound/movers/doors/holoopen.wav");
	cgs.media.holoFadeSound = trap_S_RegisterSound("sound/movers/holodeckdecloak.wav");*/

	cgs.media.phaserEmptySound = trap_S_RegisterSound("sound/weapons/phaser/phaserempty.wav");
	
	//RPG-X: RedTechie - Load sound for shake cmd
	cgs.media.ShakeSound = trap_S_RegisterSound("sound/shake.wav");

	cgs.media.tedTextSound = trap_S_RegisterSound( "sound/interface/tedtext.wav" );

	//RPG-X | Phenix | 13/02/2005
	for (i=0 ; i<N00bSoundCount ; i++) {
		Com_sprintf (name, sizeof(name), "sound/n00bs/insult%i.wav", i+1);
		cgs.media.N00bSound[i] = trap_S_RegisterSound (name);
	}

	//RPG-X | Phenix | 08/06/2005
	cgs.media.AdminMsgSound = trap_S_RegisterSound("sound/interface/button2.wav");

	for (i=0 ; i<4 ; i++) {
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/step%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_NORMAL][i] = trap_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/borg%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_BORG][i] = trap_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/reaver%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_REAVER][i] = trap_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/species%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_SPECIES][i] = trap_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/warbot%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_WARBOT][i] = trap_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/boot%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_BOOT][i] = trap_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/splash%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_SPLASH][i] = trap_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/clank%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_METAL][i] = trap_S_RegisterSound (name);

		//RPG-X | GSIO01 | 20.05.2009 | START MOD
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/wood%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_WOOD][i] = trap_S_RegisterSound(name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/grass%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_GRASS][i] = trap_S_RegisterSound(name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/gravel%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_GRAVEL][i] = trap_S_RegisterSound(name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/snow%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_SNOW][i] = trap_S_RegisterSound(name);
		//RPG-X | GSIO01 | 20.05.2009 | END MOD
	}

	cg.loadLCARSStage = 2;	// Loading bar stage 2
	CG_LoadingString( "item sounds" );

	// only register the items that the server says we need
	strcpy( items, CG_ConfigString( CS_ITEMS ) );

	for ( i = 1 ; i < bg_numItems ; i++ ) {
		if ( items[ i ] == '1' || cg_buildScript.integer ) {
			CG_RegisterItemSounds( i );
		}
	}

	for ( i = 1 ; i < MAX_SOUNDS ; i++ ) {
		soundName = CG_ConfigString( CS_SOUNDS+i );
		if ( !soundName[0] ) {
			break;
		}
		if ( soundName[0] == '*' ) {
			continue;	// custom sound
		}
		cgs.gameSounds[i] = trap_S_RegisterSound( soundName );
	}

	// FIXME: only needed with item
	//cgs.media.flightSound			= trap_S_RegisterSound( "sound/items/flight.wav" );
	cgs.media.medkitSound			= trap_S_RegisterSound ("sound/items/use_medkit.wav");
	//cgs.media.quadSound				= trap_S_RegisterSound("sound/items/damage3.wav");
	cgs.media.grenadeExplodeSound	= trap_S_RegisterSound(SOUND_DIR "glauncher/explode.wav");//detpack

	cgs.media.metalChunkSound	= trap_S_RegisterSound( "sound/weapons/explosions/metalexplode.wav" );
	cgs.media.glassChunkSound	= trap_S_RegisterSound( "sound/weapons/explosions/glassbreak1.wav" );
	cgs.media.woodChunkSound	= trap_S_RegisterSound( "sound/weapons/explosions/metalexplode.wav" );
	cgs.media.stoneChunkSound	= trap_S_RegisterSound( "sound/weapons/explosions/metalexplode.wav" );
	

//	cgs.media.sfx_rockexp = trap_S_RegisterSound ("sound/weapons/rocket/rocklx1a.wav");

	// trek sounds	

	//TiM : Q flash
	cgs.media.qFlash = trap_S_RegisterSound( "sound/world/q_flash.wav" );

	cgs.media.envSparkSound1 = trap_S_RegisterSound ("sound/ambience/spark1.wav");
	cgs.media.envSparkSound2 = trap_S_RegisterSound ("sound/ambience/spark2.wav");
	cgs.media.envSparkSound3 = trap_S_RegisterSound ("sound/ambience/spark3.wav");
	cgs.media.defaultPickupSound = trap_S_RegisterSound ("sound/items/n_health.wav");
	cgs.media.invulnoProtectSound = trap_S_RegisterSound("sound/items/protect3.wav");
	//cgs.media.regenSound = trap_S_RegisterSound("sound/items/regen.wav");
	cgs.media.waterDropSound1 = trap_S_RegisterSound("sound/ambience/waterdrop1.wav");
	cgs.media.waterDropSound2 = trap_S_RegisterSound("sound/ambience/waterdrop2.wav");
	cgs.media.waterDropSound3 = trap_S_RegisterSound("sound/ambience/waterdrop3.wav");

	// Zoom
	cgs.media.zoomStart		= trap_S_RegisterSound( "sound/interface/zoomstart.wav" );
	cgs.media.zoomLoop		= trap_S_RegisterSound( "sound/interface/zoomloop.wav" );
	cgs.media.zoomEnd		= trap_S_RegisterSound( "sound/interface/zoomend.wav" );
	cgs.media.zoomStart116	= trap_S_RegisterSound( "sound/weapons/tr116/powerup.wav" );
	cgs.media.zoomEnd116	= trap_S_RegisterSound( "sound/weapons/tr116/powerdown.wav" );
	cgs.media.tr116Chirp	= trap_S_RegisterSound( "sound/weapons/tr116/chirp.wav" );
	cgs.media.tr116Whir		= trap_S_RegisterSound( "sound/weapons/tr116/whir.wav" );

	cgs.media.surfaceExpSound[0]	= trap_S_RegisterSound("sound/weapons/explosions/explode8.wav");
	cgs.media.surfaceExpSound[1]	= trap_S_RegisterSound("sound/weapons/explosions/explode9.wav");
	cgs.media.surfaceExpSound[2]	= trap_S_RegisterSound("sound/weapons/explosions/explode14.wav");
	cgs.media.electricExpSound[0]	= trap_S_RegisterSound("sound/weapons/explosions/explode10.wav");
	cgs.media.electricExpSound[1]	= trap_S_RegisterSound("sound/weapons/explosions/explode11.wav");
	cgs.media.electricExpSound[2]	= trap_S_RegisterSound("sound/weapons/explosions/explode15.wav");
	cgs.media.bigSurfExpSound		= trap_S_RegisterSound("sound/weapons/explosions/explode12.wav");
}


//===================================================================================

/*static void PrecacheAwardsAssets()
{
	// kef -- precaching bot victory sounds (e.g. Desperado_wins.wav) in PlayerModel_BuildList()
	// TiM -- commenting out every asset RPG-X won't use.  Honestly, this'll speed up load time A LOT

	trap_R_RegisterShaderNoMip("menu/medals/medal_efficiency");			
	trap_R_RegisterShaderNoMip("menu/medals/medal_sharpshooter");		
	trap_R_RegisterShaderNoMip("menu/medals/medal_untouchable");		
	trap_R_RegisterShaderNoMip("menu/medals/medal_logistics");			
	trap_R_RegisterShaderNoMip("menu/medals/medal_tactician");			
	trap_R_RegisterShaderNoMip("menu/medals/medal_demolitionist");		
	trap_R_RegisterShaderNoMip("menu/medals/medal_ace");				
	trap_R_RegisterShaderNoMip("menu/medals/medal_teammvp");			
	trap_R_RegisterShaderNoMip("menu/medals/medal_section31");			

	trap_S_RegisterSound("sound/voice/computer/misc/effic.wav");			
	trap_S_RegisterSound("sound/voice/computer/misc/sharp.wav");			
	trap_S_RegisterSound("sound/voice/computer/misc/untouch.wav");		
	trap_S_RegisterSound("sound/voice/computer/misc/log.wav");			
	trap_S_RegisterSound("sound/voice/computer/misc/tact.wav");			
	trap_S_RegisterSound("sound/voice/computer/misc/demo.wav");			
	trap_S_RegisterSound("sound/voice/computer/misc/ace.wav");			
	trap_S_RegisterSound("sound/voice/computer/misc/sec31.wav");

	trap_R_RegisterShaderNoMip("menu/medals/medal_teammvp");		
	trap_R_RegisterShaderNoMip("menu/medals/medal_teamdefender");	
	trap_R_RegisterShaderNoMip("menu/medals/medal_teamwarrior");	
	trap_R_RegisterShaderNoMip("menu/medals/medal_teamcarrier");	
	trap_R_RegisterShaderNoMip("menu/medals/medal_teaminterceptor");	
	trap_R_RegisterShaderNoMip("menu/medals/medal_teambravery");		

	trap_S_RegisterSound("sound/voice/computer/misc/mvp.wav");			
	trap_S_RegisterSound("sound/voice/computer/misc/defender.wav");		
	trap_S_RegisterSound("sound/voice/computer/misc/warrior.wav");		
	trap_S_RegisterSound("sound/voice/computer/misc/carrier.wav");			
	trap_S_RegisterSound("sound/voice/computer/misc/intercept.wav");				
	trap_S_RegisterSound("sound/voice/computer/misc/bravery.wav");	

	trap_R_RegisterShaderNoMip("menu/medals/medal_ace");			
	trap_R_RegisterShaderNoMip("menu/medals/medal_expert");			
	trap_R_RegisterShaderNoMip("menu/medals/medal_master");			
	trap_R_RegisterShaderNoMip("menu/medals/medal_champion");				

	trap_S_RegisterSound("sound/voice/computer/misc/ace.wav");				
	trap_S_RegisterSound("sound/voice/computer/misc/expert.wav");			
	trap_S_RegisterSound("sound/voice/computer/misc/master.wav");			
	trap_S_RegisterSound("sound/voice/computer/misc/champion.wav");

	trap_S_RegisterSound("sound/voice/computer/misc/commendations.wav");
	trap_S_RegisterSound("sound/voice/computer/misc/progcomp.wav");
	trap_S_RegisterSound("sound/voice/computer/misc/2nd.wav");
	trap_S_RegisterSound("sound/voice/computer/misc/3rd.wav");
	trap_S_RegisterSound("sound/voice/computer/misc/notPlace.wav");
	trap_S_RegisterSound( "sound/voice/computer/misc/youwin.wav" );
	trap_S_RegisterSound( "sound/voice/computer/misc/blueteam_wins.wav" );
	trap_S_RegisterSound( "sound/voice/computer/misc/redteam_wins.wav" );
	trap_S_RegisterSound( "sound/voice/computer/misc/teamstied.wav" );
	trap_S_RegisterSound( "sound/voice/computer/misc/yourteam_wins.wav" );

	trap_R_RegisterShader("icons/icon_ready_on");
	trap_R_RegisterShader("icons/icon_ready_off");
}*/

/*
=================
CG_RegisterGraphics

This function may execute for a couple of minutes with a slow disk.
=================
*/
static void CG_RegisterGraphics( void ) {
	int			i;
	char		items[MAX_ITEMS+1];
	//char		temp_skin[100];
	static char		*sb_nums[11] = {
		"gfx/2d/numbers/zero",
		"gfx/2d/numbers/one",
		"gfx/2d/numbers/two",
		"gfx/2d/numbers/three",
		"gfx/2d/numbers/four",
		"gfx/2d/numbers/five",
		"gfx/2d/numbers/six",
		"gfx/2d/numbers/seven",
		"gfx/2d/numbers/eight",
		"gfx/2d/numbers/nine",
		"gfx/2d/numbers/minus",
	};

	static char		*sb_t_nums[11] = {
		"gfx/2d/numbers/t_zero",
		"gfx/2d/numbers/t_one",
		"gfx/2d/numbers/t_two",
		"gfx/2d/numbers/t_three",
		"gfx/2d/numbers/t_four",
		"gfx/2d/numbers/t_five",
		"gfx/2d/numbers/t_six",
		"gfx/2d/numbers/t_seven",
		"gfx/2d/numbers/t_eight",
		"gfx/2d/numbers/t_nine",
		"gfx/2d/numbers/t_minus",
	};


	// clear any references to old media
	memset( &cg.refdef, 0, sizeof( cg.refdef ) );
	trap_R_ClearScene();

	cg.loadLCARSStage = 3;	// Loading bar stage 3
	CG_LoadingString( cgs.mapname );

	trap_R_LoadWorldMap( cgs.mapname );

	// precache status bar pics
	cg.loadLCARSStage = 4;	// Loading bar stage 4
	CG_LoadingString( "game media" );

	for ( i=0 ; i<11 ; i++) {
		cgs.media.numberShaders[i] = trap_R_RegisterShaderNoMip( sb_nums[i] );
		cgs.media.smallnumberShaders[i] = trap_R_RegisterShaderNoMip( sb_t_nums[i] );
	}

	/*cgs.media.botSkillShaders[0] = trap_R_RegisterShaderNoMip( "menu/art/skill1.tga" );
	cgs.media.botSkillShaders[1] = trap_R_RegisterShaderNoMip( "menu/art/skill2.tga" );
	cgs.media.botSkillShaders[2] = trap_R_RegisterShaderNoMip( "menu/art/skill3.tga" );
	cgs.media.botSkillShaders[3] = trap_R_RegisterShaderNoMip( "menu/art/skill4.tga" );
	cgs.media.botSkillShaders[4] = trap_R_RegisterShaderNoMip( "menu/art/skill5.tga" );*/

	//cgs.media.pClassShaders[PC_NOCLASS] = trap_R_RegisterShaderNoMip( "menu/art/pc_noclass.tga" );//PC_NOCLASS,//default

	//cgs.media.pClassShaders[PC_SECURITY] = trap_R_RegisterShaderNoMip( "menu/art/pc_security.tga" );//PC_NOCLASS,//default
	//cgs.media.pClassShaders[PC_COMMAND] = trap_R_RegisterShaderNoMip( "menu/art/pc_command.tga" );//PC_NOCLASS,//default
	//cgs.media.pClassShaders[PC_ENGINEER] = trap_R_RegisterShaderNoMip( "menu/art/pc_engineer.tga" );//PC_NOCLASS,//default
	//cgs.media.pClassShaders[PC_ALIEN] = trap_R_RegisterShaderNoMip( "menu/art/pc_alien.tga" );//PC_NOCLASS,//default
	//cgs.media.pClassShaders[PC_SCIENCE] = trap_R_RegisterShaderNoMip( "menu/art/pc_science.tga" );//PC_NOCLASS,//default
	//cgs.media.pClassShaders[PC_MEDICAL] = trap_R_RegisterShaderNoMip( "menu/art/pc_medical.tga" );//PC_NOCLASS,//default
	//cgs.media.pClassShaders[PC_ALPHAOMEGA22] = trap_R_RegisterShaderNoMip( "menu/art/pc_security.tga" );//PC_NOCLASS,//default
	//cgs.media.pClassShaders[PC_ADMIN] = trap_R_RegisterShaderNoMip( "menu/art/pc_security.tga" );//PC_NOCLASS,//default

	//TiM
	/*if ( cgs.pModSpecialties || cg_buildScript.integer )
	{
		cgs.media.pClassShaders[PC_INFILTRATOR] = trap_R_RegisterShaderNoMip( "menu/art/pc_infiltrator.tga" );//PC_INFILTRATOR,//fast, low attack
		cgs.media.pClassShaders[PC_SNIPER] = trap_R_RegisterShaderNoMip( "menu/art/pc_sniper.tga" );//PC_SNIPER,//sneaky, snipe only
		cgs.media.pClassShaders[PC_HEAVY] = trap_R_RegisterShaderNoMip( "menu/art/pc_heavy.tga" );//PC_HEAVY,//slow, heavy attack
		cgs.media.pClassShaders[PC_DEMO] = trap_R_RegisterShaderNoMip( "menu/art/pc_demo.tga" );//PC_DEMO,//go boom
		cgs.media.pClassShaders[PC_MEDIC] = trap_R_RegisterShaderNoMip( "menu/art/pc_medic.tga" );//PC_MEDIC,//heal
		cgs.media.pClassShaders[PC_TECH] = trap_R_RegisterShaderNoMip( "menu/art/pc_tech.tga" );//PC_TECH,//operate
	}*/

	//TiM
	/*if ( cgs.pModActionHero || cg_buildScript.integer )
	{
		cgs.media.pClassShaders[PC_ACTIONHERO] = trap_R_RegisterShaderNoMip( "menu/art/pc_hero.tga" );//PC_ACTIONHERO,//has everything
		cgs.media.heroSpriteShader = trap_R_RegisterShader( "sprites/class_hero" );
	}*/

	//TiM
	/*if ( cgs.pModAssimilation || cg_buildScript.integer )
	{//borg beam
		cgs.media.whiteLaserShader		= trap_R_RegisterShader( "gfx/effects/whitelaser" );
		cgs.media.borgEyeFlareShader	= trap_R_RegisterShader( "gfx/misc/borgeyeflare" );
		cgs.media.pClassShaders[PC_BORG] = trap_R_RegisterShaderNoMip( "menu/art/pc_borg.tga" );//PC_BORG,//special weapons, slower, adapting shields
		cgs.media.borgIconShader = trap_R_RegisterShaderNoMip( "icons/icon_borg.tga" );
		cgs.media.borgQueenIconShader = trap_R_RegisterShaderNoMip( "icons/icon_borgqueen.tga" );
	}*/

	//TiM : For the ST style long beam effectszor
	cgs.media.transportShader		= trap_R_RegisterShader( "powerups/beamEffect" );
	//GSIO01: shaders for additional transport fx
	cgs.media.transportKlingon		= trap_R_RegisterShader( "gfx/effects/tKlingon" );
	cgs.media.transportRomulan		= trap_R_RegisterShader( "gfx/effects/tRom" );
	cgs.media.transportCardassian	= trap_R_RegisterShader( "gfx/effects/tCardi" );

	cgs.media.deferShader = trap_R_RegisterShaderNoMip( "gfx/2d/defer.tga" );
	//No more elimination - TiM
	//cgs.media.eliminatedShader = trap_R_RegisterShaderNoMip( "gfx/2d/eliminated.tga" );

	cgs.media.smokePuffRageProShader = trap_R_RegisterShader( "smokePuffRagePro" );
	cgs.media.lagometerShader = trap_R_RegisterShader("lagometer" );
	cgs.media.connectionShader = trap_R_RegisterShader( "disconnected" );

	cgs.media.waterBubbleShader = trap_R_RegisterShader( "waterBubble" );

	//cgs.media.selectShader = trap_R_RegisterShader( "gfx/2d/select" );

	/*for ( i = 0 ; i < NUM_CROSSHAIRS ; i++ ) {
		cgs.media.crosshairShader[i] = trap_R_RegisterShaderNoMip( va("gfx/2d/crosshair%c", 'a'+i) );
	}*/

	//LASER
	cgs.media.laserShader = trap_R_RegisterShader( "sprites/laser" );

	cgs.media.backTileShader = trap_R_RegisterShader( "gfx/2d/backtile" );
	//cgs.media.noammoShader = trap_R_RegisterShader( "icons/noammo" );

	// powerup shaders
	/*cgs.media.quadShader				= trap_R_RegisterShader("powerups/quad" );
	cgs.media.quadWeaponShader			= trap_R_RegisterShader("powerups/quadWeapon" );
	cgs.media.battleSuitShader			= trap_R_RegisterShader("powerups/battleSuit" );
	cgs.media.battleWeaponShader		= trap_R_RegisterShader("powerups/battleWeapon" );
	cgs.media.invisShader				= trap_R_RegisterShader("powerups/invisibility" );
	cgs.media.regenShader				= trap_R_RegisterShader("powerups/regen" );
	cgs.media.hastePuffShader			= trap_R_RegisterShader("hasteSmokePuff" );
	cgs.media.flightPuffShader			= trap_R_RegisterShader("flightSmokePuff" );
	cgs.media.borgFullBodyShieldShader	= trap_R_RegisterShader( "gfx/effects/borgfullbodyshield" );
	cgs.media.borgFlareShader			= trap_R_RegisterShader( "gfx/misc/borgflare" );*/
	cgs.media.disruptorShader			= trap_R_RegisterShader( "powerups/disrupt");
	cgs.media.explodeShellShader		= trap_R_RegisterShader( "powerups/explode");
	cgs.media.quantumDisruptorShader	= trap_R_RegisterShader( "powerups/quantum_disruptor_hm");
	cgs.media.borgFullBodyShieldShader	= trap_R_RegisterShader( "gfx/effects/borgfullbodyshield2" );

	//cgs.media.seekerModel = trap_R_RegisterModel("models/powerups/trek/flyer.md3" );
	//cgs.media.holoDoorModel = trap_R_RegisterModel("models/mapobjects/podium/hm_room.md3" );

	// Used in any explosion-oriented death.
	for (i = 0; i < NUM_CHUNKS; i++)
	{
		cgs.media.chunkModels[MT_METAL][i] = trap_R_RegisterModel( va( "models/chunks/generic/chunks_%i.md3", i+1 ) );
		cgs.media.chunkModels[MT_GLASS][i] = trap_R_RegisterModel( va( "models/chunks/glass/glchunks_%i.md3", i+1 ) );
		cgs.media.chunkModels[MT_WOOD][i] = trap_R_RegisterModel( va( "models/chunks/generic/chunks_%i.md3", i+1 ) );
		cgs.media.chunkModels[MT_STONE][i] = trap_R_RegisterModel( va( "models/chunks/generic/chunks_%i.md3", i+1 ) );
	}

	//TiM
	/*if ( cgs.gametype == GT_CTF || cg_buildScript.integer ) {
		cgs.media.redFlagModel = trap_R_RegisterModel( "models/flags/flag_red.md3" );//must match bg_misc item and botfiles/items.c
		cgs.media.blueFlagModel = trap_R_RegisterModel( "models/flags/flag_blue.md3" );//must match bg_misc item and botfiles/items.c
		cgs.media.redFlagShader[0] = trap_R_RegisterShaderNoMip( "icons/iconf_red1" );
		cgs.media.redFlagShader[1] = trap_R_RegisterShaderNoMip( "icons/iconf_red2" );
		cgs.media.redFlagShader[2] = trap_R_RegisterShaderNoMip( "icons/iconf_red3" );
		cgs.media.blueFlagShader[0] = trap_R_RegisterShaderNoMip( "icons/iconf_blu1" );
		cgs.media.blueFlagShader[1] = trap_R_RegisterShaderNoMip( "icons/iconf_blu2" );
		cgs.media.blueFlagShader[2] = trap_R_RegisterShaderNoMip( "icons/iconf_blu3" );

		// this determines the normal shaders / skins used by the ctf flags
		if (Q_stricmp("", CG_ConfigString( CS_RED_GROUP)))
		{
			// try loading the group based flag skin
			Com_sprintf(temp_skin, sizeof(temp_skin),"models/flags/%s_red", CG_ConfigString( CS_RED_GROUP));
			cgs.media.redFlagShader[3] = trap_R_RegisterShader3D( temp_skin );
			// did it load?
			if (!cgs.media.redFlagShader[3])
			{
				//no, go with default skin
				cgs.media.redFlagShader[3] = trap_R_RegisterShader3D( "models/flags/default_red" );
			}
		}
		else
		{
			cgs.media.redFlagShader[3] = trap_R_RegisterShader3D( "models/flags/default_red" );
		}

		if (Q_stricmp("", CG_ConfigString( CS_BLUE_GROUP)))
		{
			// try loading the group based flag skin
			Com_sprintf(temp_skin, sizeof(temp_skin),"models/flags/%s_blue", CG_ConfigString( CS_BLUE_GROUP));
			cgs.media.blueFlagShader[3] = trap_R_RegisterShader3D( temp_skin );
			// did it load?
			if (!cgs.media.blueFlagShader[3])
			{
				//no, go with default skin
				cgs.media.blueFlagShader[3] = trap_R_RegisterShader3D( "models/flags/default_blue" );
			}
		}
		else
		{
			cgs.media.blueFlagShader[3] = trap_R_RegisterShader3D( "models/flags/default_blue" );
		}

	}*/

	//TiM
	/*if ( cgs.gametype >= GT_TEAM || cg_buildScript.integer ) {
		cgs.media.teamRedShader = trap_R_RegisterShader( "sprites/team_red" );
		cgs.media.teamBlueShader = trap_R_RegisterShader( "sprites/team_blue" );
		cgs.media.redQuadShader = trap_R_RegisterShader("powerups/blueflag" );
		cgs.media.teamStatusBar = trap_R_RegisterShader( "gfx/2d/colorbar.tga" );
	}*/

	//cgs.media.chatShader = trap_R_RegisterShader( "sprites/chat" );

	//cgs.media.bloodExplosionShader = trap_R_RegisterShader( "bloodExplosion" );

	//cgs.media.ringFlashModel = trap_R_RegisterModel("models/weaphits/ring02.md3");
	cgs.media.teleportEffectModel = trap_R_RegisterModel( "models/misc/telep.md3" );
	cgs.media.teleportEffectShader = trap_R_RegisterShader( "playerTeleport" );

	//cgs.media.doorbox = trap_R_RegisterModel( "models/mapobjects/podium/hm_room.md3");

	//RPG-X TiM : the bolton assets
	cgs.media.phaserHolster	=	trap_R_RegisterModel( "models/boltOns/phaser_holster.md3");
	cgs.media.phaserHolsterInner = trap_R_RegisterModel( "models/boltOns/phaser_holster_inner.md3");

	cgs.media.tricorderHolster =	trap_R_RegisterModel( "models/boltOns/tricorder_holster.md3");
	cgs.media.tricorderHolsterInner = trap_R_RegisterModel( "models/boltOns/tricorder_holster_inner.md3");

	//TiM : TR116 Eyescope
	cgs.media.tr116EyeScope = trap_R_RegisterModel( "models/boltOns/tr116_scope.md3");
	//TiM : Flashlight
	cgs.media.simsModule = trap_R_RegisterModel( "models/boltOns/sims_beacon.md3" );
	//EVA FPS Helmet
	cgs.media.evaInterior = trap_R_RegisterModel( "models/boltOns/eva_interior.md3" );
	//Medical Scanner
	cgs.media.medicalScanner = trap_R_RegisterModel( "models/weapons2/tricorder/tricorder_scanner.md3" );
	cgs.media.hazardHelmet	= trap_R_RegisterModel( "models/boltOns/helmet.md3" );

	//RPG-X START | GSIO01 | 09/05/2009 | START
	switch(rpg_forceFieldSet.integer) {
		case 2:
			cgs.media.shieldActivateShaderBorg = trap_R_RegisterShader( "gfx/forcefielddarkgreen" );
			cgs.media.shieldActivateShaderYellow = trap_R_RegisterShader( "gfx/forcefielddarkyellow" );
			cgs.media.shieldActivateShaderRed = trap_R_RegisterShader( "gfx/forcefielddarkred" );
			cgs.media.shieldActivateShaderBlue = trap_R_RegisterShader( "gfx/forcefielddarkblue" );
			break;
		case 1:
		default:
			cgs.media.shieldActivateShaderBorg = trap_R_RegisterShader( "gfx/ff_green" );
			cgs.media.shieldActivateShaderYellow = trap_R_RegisterShader( "gfx/ff_yellow" );
			cgs.media.shieldActivateShaderRed = trap_R_RegisterShader( "gfx/ff_red" );
			cgs.media.shieldActivateShaderBlue = trap_R_RegisterShader( "gfx/ff_blue" );
			break;
	}
	//RPG-X END
	//cgs.media.shieldDamageShaderBlue = trap_R_RegisterShader( "gfx/misc/blue_dmgshield" );
	
	
	
	//cgs.media.shieldActivateShaderRed = trap_R_RegisterShader( "gfx/misc/red_portashield" );
	//cgs.media.shieldDamageShaderRed = trap_R_RegisterShader( "gfx/misc/red_dmgshield" );

	cgs.media.weaponPlaceholderShader	= trap_R_RegisterShader("powerups/placeholder" );
	cgs.media.rezOutShader				= trap_R_RegisterShader("powerups/rezout");
	cgs.media.electricBodyShader		= trap_R_RegisterShader("gfx/misc/electric");

	/*cgs.media.medalImpressive = trap_R_RegisterShaderNoMip( "medal_impressive" );
	cgs.media.medalExcellent = trap_R_RegisterShaderNoMip( "medal_excellent" );
	cgs.media.medalFirstStrike = trap_R_RegisterShaderNoMip( "medal_firststrike" );
	cgs.media.medalAce = trap_R_RegisterShaderNoMip( "medal_ace" );
	cgs.media.medalExpert = trap_R_RegisterShaderNoMip( "medal_expert" );
	cgs.media.medalMaster = trap_R_RegisterShaderNoMip( "medal_master" );
	cgs.media.medalChampion = trap_R_RegisterShaderNoMip( "medal_champion" );*/
	
	//RPG-X: RedTechie - Scoreboard Endcaps
	cgs.media.scoreboardtopleft = trap_R_RegisterShaderNoMip( "menu/common/rpgx_sb_topleft");
	cgs.media.scoreboardtopright = trap_R_RegisterShaderNoMip( "menu/common/rpgx_sb_topright");
	cgs.media.scoreboardbotleft = trap_R_RegisterShaderNoMip( "menu/common/rpgx_sb_bottomleft");
	cgs.media.scoreboardbotright = trap_R_RegisterShaderNoMip( "menu/common/rpgx_sb_bottomright");
	
	//RPG-X: RedTechie - Healthbar Curves
	cgs.media.healthendcap = trap_R_RegisterShaderNoMip("gfx/interface/rpgx_healthbar_endcap");
	cgs.media.healthbigcurve = trap_R_RegisterShaderNoMip("gfx/interface/rpgx_healthbar_leftcorner");
	//TiM: New Healthbar Graphics
	//cgs.media.healthSineWave = trap_R_RegisterShaderNoMip( "menu/healthbar/sinewave" );

	//RPG-X: RedTechie - Cloak Sprite
	//cgs.media.cloakspriteShader = trap_R_RegisterShader("sprites/cloak");

	cgs.media.scoreboardEndcap = trap_R_RegisterShaderNoMip( "menu/common/halfround_r_24");
	cgs.media.corner_12_24 = trap_R_RegisterShaderNoMip( "menu/common/corner_ll_24_12");
	cgs.media.corner_8_16_b = trap_R_RegisterShaderNoMip( "menu/common/corner_lr_8_16_b");

	cgs.media.weaponcap1 = trap_R_RegisterShaderNoMip("gfx/interface/cap4");
	cgs.media.weaponcap2 = trap_R_RegisterShaderNoMip("gfx/interface/cap5");

	cgs.media.weaponbox =  trap_R_RegisterShaderNoMip("gfx/interface/weapon_box");
	cgs.media.weaponbox2 =  trap_R_RegisterShaderNoMip("gfx/interface/weapon_box2");

	memset( cg_items, 0, sizeof( cg_items ) );
	memset( cg_weapons, 0, sizeof( cg_weapons ) );

	cg.loadLCARSStage = 5;	// Loading bar stage 5
	//don't need a 	CG_LoadingString because there will be one in the LoadingItem()

	// only register the items that the server says we need
	strcpy( items, CG_ConfigString( CS_ITEMS) );

	for ( i = 1 ; i < bg_numItems ; i++ ) {
		if ( items[ i ] == '1' || cg_buildScript.integer ) {
			CG_LoadingItem( i );
			CG_RegisterItemVisuals( i );
		}
	}

	// wall marks
	cgs.media.holeMarkShader = trap_R_RegisterShader( "gfx/damage/hole_lg_mrk" );
	cgs.media.energyMarkShader = trap_R_RegisterShader( "gfx/damage/plasma_mrk" );
	cgs.media.shadowMarkShader = trap_R_RegisterShader( "markShadow" );
	cgs.media.wakeMarkShader = trap_R_RegisterShader( "wake" );

	// Radar
	//TiM: OPTIMIZATION:
	//It's possible to use one set of textures, and use Alpha channels to vary
	//the color thru the code... wouldn't that be better?
	cgs.media.radarShader = trap_R_RegisterShader( "gfx/radar/radar" );
	/*cgs.media.rd_up = trap_R_RegisterShader( "gfx/radar/rd_up" );
	cgs.media.rd_down = trap_R_RegisterShader( "gfx/radar/rd_down" );
	cgs.media.rd_level = trap_R_RegisterShader( "gfx/radar/rd_level" );
	cgs.media.rd_red_up = trap_R_RegisterShader( "gfx/radar/rd_red_up" );
	cgs.media.rd_red_down = trap_R_RegisterShader( "gfx/radar/rd_red_down" );
	cgs.media.rd_red_level = trap_R_RegisterShader( "gfx/radar/rd_red_level" );
	cgs.media.rd_blue_up = trap_R_RegisterShader( "gfx/radar/rd_blue_up" );
	cgs.media.rd_blue_down = trap_R_RegisterShader( "gfx/radar/rd_blue_down" );
	cgs.media.rd_blue_level = trap_R_RegisterShader( "gfx/radar/rd_blue_level" );
	cgs.media.rd_white_up = trap_R_RegisterShader( "gfx/radar/rd_white_up" );
	cgs.media.rd_white_down = trap_R_RegisterShader( "gfx/radar/rd_white_down" );
	cgs.media.rd_white_level = trap_R_RegisterShader( "gfx/radar/rd_white_level" );
	cgs.media.rd_teal_up = trap_R_RegisterShader( "gfx/radar/rd_teal_up" );
	cgs.media.rd_teal_down = trap_R_RegisterShader( "gfx/radar/rd_teal_down" );
	cgs.media.rd_teal_level = trap_R_RegisterShader( "gfx/radar/rd_teal_level" );
	cgs.media.rd_black_up = trap_R_RegisterShader( "gfx/radar/rd_black_up" );
	cgs.media.rd_black_down = trap_R_RegisterShader( "gfx/radar/rd_black_down" );
	cgs.media.rd_black_level = trap_R_RegisterShader( "gfx/radar/rd_black_level" );
	cgs.media.rd_injured_up = trap_R_RegisterShader( "gfx/radar/injured_up" );
	cgs.media.rd_injured_down = trap_R_RegisterShader( "gfx/radar/injured_down" );*/
	cgs.media.rd_injured_level = trap_R_RegisterShader( "gfx/radar/injured_level" );

	cgs.media.radarMain = trap_R_RegisterShaderNoMip( "gfx/radar/radar_icon" );

	// register the inline models
	cgs.numInlineModels = trap_CM_NumInlineModels();
	for ( i = 1 ; i < cgs.numInlineModels ; i++ ) {
		char	name[10];
		vec3_t			mins, maxs;
		int				j;

		Com_sprintf( name, sizeof(name), "*%i", i );
		cgs.inlineDrawModel[i] = trap_R_RegisterModel( name );
		trap_R_ModelBounds( cgs.inlineDrawModel[i], mins, maxs );
		for ( j = 0 ; j < 3 ; j++ ) {
			cgs.inlineModelMidpoints[i][j] = mins[j] + 0.5 * ( maxs[j] - mins[j] );
		}
	}

	cg.loadLCARSStage = 6;	// Loading bar stage 6
	CG_LoadingString( "Game Models" );

	// register all the server specified models
	for (i=1 ; i<MAX_MODELS ; i++) {
		const char		*modelName;

		modelName = CG_ConfigString( CS_MODELS+i );
		if ( !modelName[0] ) {
			break;
		}
		cgs.gameModels[i] = trap_R_RegisterModel( modelName );
	}

	//TiM: Check to see if we have any valid locations stored locally.
	//If we do, we'll modify the scoreboard to display them
	for ( i = 1; i < MAX_LOCATIONS; i++ ) {
		const char		*strName;

		strName = CG_ConfigString ( CS_LOCATIONS + i );
		if ( strlen( strName ) > 0 ) {
			cgs.locations = qtrue;
			break;
		}
	}

	//TiM: Do the same for all tricorder string names
	/*or ( i = 1; i < MAX_TRIC_STRINGS; i++ ) {
		const char		*strName;

		strName = CG_ConfigString( CS_TRIC_STRINGS+i );
		if ( !strName[0] ) {
			break;
		}
		
		//Com_Printf( S_COLOR_RED "USABLE MESSAGE IN CG: %s\n", strName );

		//Com_sprintf( cgs.tricStrings[i], MAX_TOKEN_CHARS, "%s", strName );

		//cgs.tricStrings[i] = (char *)strName;
		//Com_Printf( S_COLOR_RED "%s\n", cgs.tricStrings[i] );
		Q_strncpyz( cgs.tricStrings[i], strName, MAX_TOKEN_CHARS );
	}*/

	cg.loadLCARSStage = 7;	// Loading bar stage 7
	CG_LoadingString( "Interface" );

	// Registering interface graphics
	for (i=0;i<IG_MAX;++i)
	{
		if (interface_graphics[i].file)
		{
			interface_graphics[i].graphic = trap_R_RegisterShaderNoMip(interface_graphics[i].file);
		}

		// Turn everything off at first
		if ((interface_graphics[i].type == SG_GRAPHIC) || (interface_graphics[i].type == SG_NUMBER))
		{
			interface_graphics[i].type = SG_OFF;
		}
	}

	interface_graphics[IG_GROW].type = SG_OFF;

	// Trek stuff
	cgs.media.sparkShader				= trap_R_RegisterShader( "gfx/misc/spark" );
	cgs.media.spark2Shader				= trap_R_RegisterShader( "gfx/misc/spark2" );
	cgs.media.sunnyFlareShader			= trap_R_RegisterShader( "gfx/misc/sunny_flare" );
	cgs.media.scavMarkShader			= trap_R_RegisterShader( "gfx/damage/burnmark4" );
	cgs.media.steamShader				= trap_R_RegisterShader( "gfx/misc/steam" );
	cgs.media.fireShader				= trap_R_RegisterShader( "gfx/misc/rpgflames" );
	cgs.media.smokeShader				= trap_R_RegisterShader( "gfx/misc/smoke" );
	cgs.media.explosionModel				= trap_R_RegisterModel ( "models/weaphits/explosion.md3" );
	//cgs.media.electricalExplosionFastShader = trap_R_RegisterShader( "electricalExplosionFast" );
	cgs.media.electricalExplosionSlowShader	= trap_R_RegisterShader( "electricalExplosionSlow" );
	cgs.media.surfaceExplosionShader		= trap_R_RegisterShader( "surfaceExplosion" );
	//cgs.media.purpleParticleShader		= trap_R_RegisterShader( "gfx/misc/purpleparticle" );
	cgs.media.blueParticleShader		= trap_R_RegisterShader( "gfx/misc/blueparticle" );
	cgs.media.ltblueParticleShader		= trap_R_RegisterShader( "gfx/misc/ltblueparticle" );
	cgs.media.yellowParticleShader		= trap_R_RegisterShader( "gfx/misc/yellowparticle" );
	cgs.media.orangeParticleShader		= trap_R_RegisterShader( "gfx/misc/orangeparticle" );
	cgs.media.dkorangeParticleShader	= trap_R_RegisterShader( "gfx/misc/dkorangeparticle" );
	cgs.media.redFlareShader			= trap_R_RegisterShader( "gfx/misc/red_flare" );
	cgs.media.redRingShader				= trap_R_RegisterShader( "gfx/misc/red_ring" );
	cgs.media.redRing2Shader			= trap_R_RegisterShader( "gfx/misc/red_ring2" );
	cgs.media.nukeModel					= trap_R_RegisterModel ( "models/weaphits/nuke.md3" );
	cgs.media.bigShockShader			= trap_R_RegisterShader( "gfx/misc/bigshock" );
	//cgs.media.IMODMarkShader			= trap_R_RegisterShader( "gfx/damage/burnmark2" );
	cgs.media.bolt2Shader				= trap_R_RegisterShader( "gfx/effects/electrica" );
	//cgs.media.holoOuchShader			= trap_R_RegisterShader( "powerups/holoOuch" );
	cgs.media.painBlobShader			= trap_R_RegisterShader( "gfx/misc/painblob" );
	cgs.media.painShieldBlobShader		= trap_R_RegisterShader( "gfx/misc/painshieldblob" );
	//cgs.media.shieldBlobShader			= trap_R_RegisterShader( "gfx/misc/shieldblob" );
	//cgs.media.halfShieldShader			= trap_R_RegisterShader( "halfShieldShell" );
	//cgs.media.holoDecoyShader			= trap_R_RegisterShader( "powerups/holodecoy" );
	cgs.media.trans1Shader				= trap_R_RegisterShader( "gfx/misc/trans1" );
	cgs.media.trans2Shader				= trap_R_RegisterShader( "gfx/misc/trans2" );
	//TiM
	cgs.media.transport1Shader			= trap_R_RegisterShader( "gfx/effects/transport1" );
	cgs.media.transport2Shader			= trap_R_RegisterShader( "gfx/effects/transport2" );
	cgs.media.fountainShader			= trap_R_RegisterShader( "garden/fountain" );
	cgs.media.rippleShader				= trap_R_RegisterShader( "garden/ripple" );

	cgs.media.probeBeam					= trap_R_RegisterShader( "gfx/effects/probe_beam");
	cgs.media.probeDecal				= trap_R_RegisterShader( "gfx/effects/beam_decal" );

	cgs.media.regenDecal				= trap_R_RegisterShader( "gfx/effects/regen_decal" );

	//cgs.media.blueParticleStreakShader	= trap_R_RegisterShader( "gfx/misc/blueparticle_anamorphic" );

	//RPG-X | GSIO01 | 11/05/2009:
	cgs.media.quantumGlow				= trap_R_RegisterShader( "gfx/fx/quantum_glow" );
	cgs.media.quantumRays				= trap_R_RegisterShader( "gfx/fx/quantum_rays" );
	cgs.media.photonGlow				= trap_R_RegisterShader( "gfx/fx/photon_glow" );
	cgs.media.photonRay					= trap_R_RegisterShader( "gfx/fx/photon_rays" );
	cgs.media.fireParticle				= trap_R_RegisterShader( "gfx/fx/fire_new_1" );


	for ( i = 0; i < 4; i++ ) {
		cgs.media.borgLightningShaders[i] = trap_R_RegisterShader( va( "gfx/misc/blightning%i", i+1 ) );
	}
	// detpack
	cgs.media.orangeTrailShader			= trap_R_RegisterShader( "gfx/misc/orangetrail" );
	cgs.media.compressionMarkShader		= trap_R_RegisterShader( "gfx/damage/burnmark1" );
	cgs.media.waterDropShader			= trap_R_RegisterShader( "gfx/misc/drop" );
	cgs.media.oilDropShader				= trap_R_RegisterShader( "gfx/misc/oildrop" );
	cgs.media.greenDropShader			= trap_R_RegisterShader( "gfx/misc/greendrop" );
	cgs.media.detpackPlacedIcon			= trap_R_RegisterShader( "icons/icon_detpack_use"); // placed icon

	// Zoom interface
	cgs.media.zoomMaskShader			= trap_R_RegisterShader( "gfx/misc/zoom_mask2" );
	cgs.media.zoomMask116Shader			= trap_R_RegisterShader( "gfx/misc/zoom_mask_tr116");
	cgs.media.zoomGlow116Shader			= trap_R_RegisterShader( "gfx/misc/zoom_tr116_flash" );
	/*cgs.media.zoomBarShader				= trap_R_RegisterShader( "gfx/2d/zoom_ctrl" );
	cgs.media.zoomArrowShader			= trap_R_RegisterShader( "gfx/2d/arrow" );
	cgs.media.ammoslider				= trap_R_RegisterShaderNoMip( "gfx/interface/ammobar" );
	cgs.media.zoomInsertShader			= trap_R_RegisterShaderNoMip( "gfx/misc/zoom_insert" );*/


	cgs.media.testDetpackShader3		= trap_R_RegisterShader( "gfx/misc/detpack3" );
	cgs.media.testDetpackRingShader1	= trap_R_RegisterShader( "gfx/misc/detpackring1" );
	cgs.media.testDetpackRingShader2	= trap_R_RegisterShader( "gfx/misc/detpackring2" );
	cgs.media.testDetpackRingShader3	= trap_R_RegisterShader( "gfx/misc/detpackring3" );
	cgs.media.testDetpackRingShader4	= trap_R_RegisterShader( "gfx/misc/detpackring4" );
	cgs.media.testDetpackRingShader5	= trap_R_RegisterShader( "gfx/misc/detpackring5" );
	cgs.media.testDetpackRingShader6	= trap_R_RegisterShader( "gfx/misc/detpackring6" );

	//RPG-X (J2J) Rank Images for Score Board
	//TiM: WHHHHHHHHHHYYYYYYYYYYYYYYYYYYYYYYYYYYYYY!!!!!!!!!!?!?!?!
	//Seriously... too many shader slots... T_T
	/*********************************************************/

	//Civillian / N/A
	/*cgs.media.ri_Civ = trap_R_RegisterShader("icons/rankinsignias/Civ-Blank");

	//Red (Command)
	cgs.media.ri_Crewman[0] = trap_R_RegisterShader("icons/rankinsignias/R-S1-Blank");
	cgs.media.ri_Cadet1[0] = trap_R_RegisterShader("icons/rankinsignias/R-AC1");
	cgs.media.ri_Cadet2[0] = trap_R_RegisterShader("icons/rankinsignias/R-AC2");
	cgs.media.ri_Cadet3[0] = trap_R_RegisterShader("icons/rankinsignias/R-AC3");
	cgs.media.ri_Cadet4[0] = trap_R_RegisterShader("icons/rankinsignias/R-AC4");
	cgs.media.ri_Ensign[0] = trap_R_RegisterShader("icons/rankinsignias/R-0");
	cgs.media.ri_Ltjg[0] = trap_R_RegisterShader("icons/rankinsignias/R-01");
	cgs.media.ri_Lt[0] = trap_R_RegisterShader("icons/rankinsignias/R-00");
	cgs.media.ri_Ltcmdr[0] = trap_R_RegisterShader("icons/rankinsignias/R-001");
	cgs.media.ri_Cmdr[0] = trap_R_RegisterShader("icons/rankinsignias/R-000");
	cgs.media.ri_Capt[0] = trap_R_RegisterShader("icons/rankinsignias/R-0000");
	cgs.media.ri_Cmmdr[0] = trap_R_RegisterShader("icons/rankinsignias/R-A0");
	cgs.media.ri_Admr2[0] = trap_R_RegisterShader("icons/rankinsignias/R-A00");
	cgs.media.ri_Admr3[0] = trap_R_RegisterShader("icons/rankinsignias/R-A000");
	cgs.media.ri_Admr4[0] = trap_R_RegisterShader("icons/rankinsignias/R-A0000");
	cgs.media.ri_Admr5[0] = trap_R_RegisterShader("icons/rankinsignias/R-A00000");

	//Gold (Engineering / Security)
	cgs.media.ri_Crewman[1] = trap_R_RegisterShader("icons/rankinsignias/Y-S1-Blank");
	cgs.media.ri_Cadet1[1] = trap_R_RegisterShader("icons/rankinsignias/Y-AC1");
	cgs.media.ri_Cadet2[1] = trap_R_RegisterShader("icons/rankinsignias/Y-AC2");
	cgs.media.ri_Cadet3[1] = trap_R_RegisterShader("icons/rankinsignias/Y-AC3");
	cgs.media.ri_Cadet4[1] = trap_R_RegisterShader("icons/rankinsignias/Y-AC4");
	cgs.media.ri_Ensign[1] = trap_R_RegisterShader("icons/rankinsignias/Y-0");
	cgs.media.ri_Ltjg[1] = trap_R_RegisterShader("icons/rankinsignias/Y-01");
	cgs.media.ri_Lt[1] = trap_R_RegisterShader("icons/rankinsignias/Y-00");
	cgs.media.ri_Ltcmdr[1] = trap_R_RegisterShader("icons/rankinsignias/Y-001");
	cgs.media.ri_Cmdr[1] = trap_R_RegisterShader("icons/rankinsignias/Y-000");
	cgs.media.ri_Capt[1] = trap_R_RegisterShader("icons/rankinsignias/Y-0000");
	cgs.media.ri_Cmmdr[1] = trap_R_RegisterShader("icons/rankinsignias/Y-A0");
	cgs.media.ri_Admr2[1] = trap_R_RegisterShader("icons/rankinsignias/Y-A00");
	cgs.media.ri_Admr3[1] = trap_R_RegisterShader("icons/rankinsignias/Y-A000");
	cgs.media.ri_Admr4[1] = trap_R_RegisterShader("icons/rankinsignias/Y-A0000");
	cgs.media.ri_Admr5[1] = trap_R_RegisterShader("icons/rankinsignias/Y-A00000");

	//Teal (Science / Medical)
	cgs.media.ri_Crewman[2] = trap_R_RegisterShader("icons/rankinsignias/T-S1-Blank");
	cgs.media.ri_Cadet1[2] = trap_R_RegisterShader("icons/rankinsignias/T-AC1");
	cgs.media.ri_Cadet2[2] = trap_R_RegisterShader("icons/rankinsignias/T-AC2");
	cgs.media.ri_Cadet3[2] = trap_R_RegisterShader("icons/rankinsignias/T-AC3");
	cgs.media.ri_Cadet4[2] = trap_R_RegisterShader("icons/rankinsignias/T-AC4");
	cgs.media.ri_Ensign[2] = trap_R_RegisterShader("icons/rankinsignias/T-0");
	cgs.media.ri_Ltjg[2] = trap_R_RegisterShader("icons/rankinsignias/T-01");
	cgs.media.ri_Lt[2] = trap_R_RegisterShader("icons/rankinsignias/T-00");
	cgs.media.ri_Ltcmdr[2] = trap_R_RegisterShader("icons/rankinsignias/T-001");
	cgs.media.ri_Cmdr[2] = trap_R_RegisterShader("icons/rankinsignias/T-000");
	cgs.media.ri_Capt[2] = trap_R_RegisterShader("icons/rankinsignias/T-0000");
	cgs.media.ri_Cmmdr[2] = trap_R_RegisterShader("icons/rankinsignias/T-A0");
	cgs.media.ri_Admr2[2] = trap_R_RegisterShader("icons/rankinsignias/T-A00");
	cgs.media.ri_Admr3[2] = trap_R_RegisterShader("icons/rankinsignias/T-A000");
	cgs.media.ri_Admr4[2] = trap_R_RegisterShader("icons/rankinsignias/T-A0000");
	cgs.media.ri_Admr5[2] = trap_R_RegisterShader("icons/rankinsignias/T-A00000");

	//Marine Class
	cgs.media.ri_Crewman[3] = trap_R_RegisterShader("icons/rankinsignias/G-S1-Blank");
	cgs.media.ri_Cadet1[3] = trap_R_RegisterShader("icons/rankinsignias/G-AC1");
	cgs.media.ri_Cadet2[3] = trap_R_RegisterShader("icons/rankinsignias/G-AC2");
	cgs.media.ri_Cadet3[3] = trap_R_RegisterShader("icons/rankinsignias/G-AC3");
	cgs.media.ri_Cadet4[3] = trap_R_RegisterShader("icons/rankinsignias/G-AC4");
	cgs.media.ri_Ensign[3] = trap_R_RegisterShader("icons/rankinsignias/G-2Lt");
	cgs.media.ri_Ltjg[3] = trap_R_RegisterShader("icons/rankinsignias/G-1Lt");
	cgs.media.ri_Lt[3] = trap_R_RegisterShader("icons/rankinsignias/G-MCapt");
	cgs.media.ri_Ltcmdr[3] = trap_R_RegisterShader("icons/rankinsignias/G-Maj");
	cgs.media.ri_Cmdr[3] = trap_R_RegisterShader("icons/rankinsignias/G-LtCol");
	cgs.media.ri_Capt[3] = trap_R_RegisterShader("icons/rankinsignias/G-Col");
	cgs.media.ri_Cmmdr[3] = trap_R_RegisterShader("icons/rankinsignias/G-GX");
	cgs.media.ri_Admr2[3] = trap_R_RegisterShader("icons/rankinsignias/G-GXX");
	cgs.media.ri_Admr3[3] = trap_R_RegisterShader("icons/rankinsignias/G-GXXX");
	cgs.media.ri_Admr4[3] = trap_R_RegisterShader("icons/rankinsignias/G-GXXXX");
	cgs.media.ri_Admr5[3] = trap_R_RegisterShader("icons/rankinsignias/G-GXXXXX");*/

	/***************************************************************************/

	//RPG-X: J2J - CrossHairs
/*	for(i = 0; i < 15; i++)
	{
		cgs.media.crosshair[i] = trap_R_RegisterShader(va("icons/CrossHairs/ch%i",i+1));
	}*/

	cgs.media.crosshairSheet = trap_R_RegisterShaderNoMip( "gfx/2d/rpgx_crosshairs" );

	/*
	01 Phaser
	02 PRifle
	03 Scav 
	04 alien
	05 imod
	06 tr116
	07 granede
	08 photon
	09 dermal
	10 hypo
	11 medkit
	12 engkit
	13 engtool
	14 padd
	15 tricorder
	*/

	/***************************************************************************/
	//PH34R T3H L3NZFL4R3!!!!

	//optimize shader usage if no one wants this :(
	if ( cg_dynamiclensflares.integer ) {
		cgs.media.flareCore = trap_R_RegisterShaderNoMip("gfx/effects/flares/flare_core");
		cgs.media.flareStreak = trap_R_RegisterShaderNoMip("gfx/effects/flares/flare_streak");
		cgs.media.flareHaze = trap_R_RegisterShaderNoMip("gfx/effects/flares/flare_haze");

		/*cgs.media.flareChroma = trap_R_RegisterShaderNoMip("gfx/effects/flares/flare_chromadisc");
		cgs.media.flareRadial = trap_R_RegisterShaderNoMip("gfx/effects/flares/flare_radial");
		cgs.media.flareStraight = trap_R_RegisterShaderNoMip("gfx/effects/flares/flare_straight");
		cgs.media.flareInverseRad = trap_R_RegisterShaderNoMip("gfx/effects/flares/flare_inverseradial");*/
	

		for ( i=0; i<10; i++ ) {
			lensReflec[i].graphic = trap_R_RegisterShaderNoMip( lensReflec[i].file );
		}
	}

	//TiM - for Prifle improved FX
	cgs.media.orangeStarShader = trap_R_RegisterShaderNoMip( "gfx/misc/orangestar" );

	cgs.media.qFlashSprite = trap_R_RegisterShaderNoMip( "gfx/effects/qflash" );

	//TiM: Save asset space
	/*if (cg_buildScript.integer)
	{
		PrecacheAwardsAssets();
	}*/
}

/*
===================
CG_RegisterClients

===================
*/
static void CG_RegisterClients( void ) {
	int		i;

	cg.loadLCARSStage = 8;	// Loading bar stage 8
	CG_LoadingString( "clients" );

	for (i=0 ; i<MAX_CLIENTS ; i++) {
		const char		*clientInfo;

		//TiM - Re-register decoys if need be
		if ( i < MAX_DECOYS ) {
			CG_NewDecoyInfo( i );
		}

		clientInfo = CG_ConfigString( CS_PLAYERS+i );
		if ( !clientInfo[0] ) {
			continue;
		}
		CG_LoadingClient( i );
		CG_NewClientInfo( i );

	}
	cg.loadLCARSStage = 9;	// Loading bar stage 9
}

//===========================================================================

/*
=================
CG_ConfigString
=================
*/
const char *CG_ConfigString( int index ) {
	if ( index < 0 || index >= MAX_CONFIGSTRINGS ) {
		CG_Error( "CG_ConfigString: bad index: %i", index );
	}
	return cgs.gameState.stringData + cgs.gameState.stringOffsets[ index ];
}

//==================================================================

/*
======================
CG_StartMusic

======================
*/
void CG_StartMusic( void ) {
	char	*s;
	char	parm1[MAX_QPATH], parm2[MAX_QPATH];

	// start the background music
	s = (char *)CG_ConfigString( CS_MUSIC );
	Q_strncpyz( parm1, COM_Parse( &s ), sizeof( parm1 ) );
	Q_strncpyz( parm2, COM_Parse( &s ), sizeof( parm2 ) );

	trap_S_StartBackgroundTrack( parm1, parm2 );
}

extern int altAmmoUsage[];
void CG_InitModRules( void )
{
	if ( cgs.pModDisintegration )
	{//don't use up ammo in disintegration mode
		altAmmoUsage[WP_COMPRESSION_RIFLE] = 0;
	}
	if ( cgs.pModSpecialties )
	{//tripwires use more ammo
		altAmmoUsage[WP_GRENADE_LAUNCHER] = 3;
	}
}

/*
=================
CG_Init

Called after every level change or subsystem restart
Will perform callbacks to make the loading info screen update.
=================
*/
void CG_Init( int serverMessageNum, int serverCommandSequence ) {
	const char	*s;
	int i;

	// clear everything
	memset( &cgs, 0, sizeof( cgs ) );
	memset( &cg, 0, sizeof( cg ) );
	memset( cg_entities, 0, sizeof(cg_entities) );
	memset( cg_weapons, 0, sizeof(cg_weapons) );
	memset( cg_items, 0, sizeof(cg_items) );

	//Flush Sound Effects
	memset( &cg_animsSndList, 0, sizeof( cg_animsSndList ) );

	init_tonextint(qfalse);

	cgs.processedSnapshotNum = serverMessageNum;
	cgs.serverCommandSequence = serverCommandSequence;

	CG_LoadIngameText();

	CG_LoadFonts();

	// Loading graphics
	cgs.media.loading1		= trap_R_RegisterShaderNoMip( "menu/loading/smpiece1.tga" );
	cgs.media.loading2		= trap_R_RegisterShaderNoMip( "menu/loading/smpiece2.tga" );
	cgs.media.loading3		= trap_R_RegisterShaderNoMip( "menu/loading/smpiece3.tga" );
	cgs.media.loading4		= trap_R_RegisterShaderNoMip( "menu/loading/smpiece4.tga" );
	cgs.media.loading5		= trap_R_RegisterShaderNoMip( "menu/loading/smpiece5.tga" );
	cgs.media.loading6		= trap_R_RegisterShaderNoMip( "menu/loading/smpiece6.tga" );
	cgs.media.loading7		= trap_R_RegisterShaderNoMip( "menu/loading/smpiece7.tga" );
	cgs.media.loading8		= trap_R_RegisterShaderNoMip( "menu/loading/smpiece8.tga" );
	cgs.media.loading9		= trap_R_RegisterShaderNoMip( "menu/loading/smpiece9.tga" );
	cgs.media.loadingcircle = trap_R_RegisterShaderNoMip( "menu/loading/arrowpiece.tga" );
	cgs.media.loadingquarter= trap_R_RegisterShaderNoMip( "menu/loading/quarter.tga" );
	cgs.media.loadingcorner	= trap_R_RegisterShaderNoMip( "menu/common/corner_lr_8_16.tga" );
	cgs.media.loadingtrim	= trap_R_RegisterShaderNoMip( "menu/loading/trimupper.tga" );
	cgs.media.circle		= trap_R_RegisterShaderNoMip( "menu/common/circle.tga" );
	cgs.media.circle2		= trap_R_RegisterShaderNoMip( "menu/objectives/circle_out.tga" );
	cgs.media.corner_12_18	= trap_R_RegisterShaderNoMip( "menu/common/corner_ll_12_18.tga" );
	cgs.media.halfroundr_22	= trap_R_RegisterShaderNoMip( "menu/common/halfroundr_22.tga" );

	cgs.media.corner_ul_20_30= trap_R_RegisterShaderNoMip( "menu/common/corner_ul_20_30.tga" );
	cgs.media.corner_ll_8_30= trap_R_RegisterShaderNoMip( "menu/common/corner_ll_8_30.tga" );

	cg.loadLCARSStage		= 0;
	cg.loadLCARScnt			= 0;
	// load a few needed things before we do any screen updates
	cgs.media.charsetShader		= trap_R_RegisterShaderNoMip( "gfx/2d/charsgrid_med" );
	cgs.media.whiteShader		= trap_R_RegisterShader( "white" );
	cgs.media.white2Shader		= trap_R_RegisterShader( "white2" );
	cgs.media.charsetPropTiny = trap_R_RegisterShaderNoMip("gfx/2d/chars_tiny");
	cgs.media.charsetProp		= trap_R_RegisterShaderNoMip("gfx/2d/chars_medium");
	cgs.media.charsetPropBig	= trap_R_RegisterShaderNoMip("gfx/2d/chars_big");
//	cgs.media.charsetPropGlow	= trap_R_RegisterShaderNoMip( "menu/art/font1_prop_glo.tga" );
	cgs.media.charsetPropB		= trap_R_RegisterShaderNoMip( "gfx/2d/chars_medium.tga" );

	CG_RegisterCvars();

	CG_InitConsoleCommands();

	BG_LoadItemNames();

	cg.weaponSelect = WP_NULL_HAND;

	cgs.redflag = cgs.blueflag = -1; // For compatibily, default to unset for
	// old servers

	// get the rendering configuration from the client system
	trap_GetGlconfig( &cgs.glconfig );
	cgs.screenXScale = cgs.glconfig.vidWidth / 640.0;
	cgs.screenYScale = cgs.glconfig.vidHeight / 480.0;

	//TiM - handle wide screens
	if ( cgs.glconfig.vidWidth  * 480 > cgs.glconfig.vidHeight * 640 ) 
	{
		cgs.widescreen.ratio	= 640.0f*cgs.screenYScale  * (1.0f/cgs.glconfig.vidWidth);
		cgs.widescreen.bias		= 0.5 * ( cgs.glconfig.vidWidth - ( cgs.glconfig.vidHeight * (640.0/480.0) ) );
	}
	else
	{
		cgs.widescreen.ratio	= 0;
		cgs.widescreen.bias		= 0;
	}

	// get the gamestate from the client system
	trap_GetGameState( &cgs.gameState );

	// check version
	s = CG_ConfigString( CS_GAME_VERSION );
	if ( strcmp( s, GAME_VERSION ) ) {
		CG_Error( "Client/Server game mismatch: %s/%s", GAME_VERSION, s );
	}

	s = CG_ConfigString( CS_LEVEL_START_TIME );
	cgs.levelStartTime = atoi( s );

	CG_ParseServerinfo();

	// load the new map
	CG_LoadingString( "collision map" );

	trap_CM_LoadMap( cgs.mapname );

	cg.loading = qtrue;		// force players to load instead of defer

	CG_LoadObjectivesForMap();

	CG_RegisterSounds();

	CG_RegisterGraphics();

	CG_RegisterClients();		// if low on memory, some clients will be deferred

	cg.loading = qfalse;	// future players will be deferred

	CG_InitLocalEntities();

	CG_InitMarkPolys();

	// remove the last loading update
	cg.infoScreenText[0] = 0;

	// Make sure we have update values (scores)
	CG_SetConfigValues();

	CG_StartMusic();

	CG_LoadingString( "" );

	//RPG-X | GSIO01 | 08/05/09:
	//Make sure all weapons are registered to prevent 
	//missing models when someone get killed and weapons are dropped
	for(i = 0; i < WP_NUM_WEAPONS; i++) {
		CG_RegisterWeapon(i);
	}

	// To get the interface timing started
	cg.interfaceStartupTime = 0;
	cg.interfaceStartupDone = 0;

	CG_InitModRules();

	if ( !CG_LoadCrosshairs() ) 
	{
		CG_Error( "Couldn't load crosshairs script" );
	}

	if ( !CG_LoadRanks() ) 
	{
		CG_Error( "Couldn't load rankset script: %s", cgs.rankSet );
	}

	if ( !CG_LoadClasses() )
	{
		CG_Error( "Couldn't load classset script: %s", cgs.classSet );
	}

	//where possible, load in the usable strings locally
	if ( cgs.scannablePanels )
		CG_LoadUsablesStrings();

	//TiM Finally, init class data received from Server
	//TiM2 - Separated this out so class data has to be locally accessed now
	//CG_ParseClassData();

	#ifdef XTRA
	CG_ShaderStateChanged();
	#endif

	if(grp_berp.integer)
		CG_Printf(S_COLOR_YELLOW "GSIO01 and Ubergames greet Brave Explorers.\n");
}

/*
=================
CG_Shutdown

Called before every level change or subsystem restart
=================
*/
void CG_Shutdown( void ) {
	// some mods may need to do cleanup work here,
	// like closing files or archiving session data
	//trap_Cvar_Set ("rpg_playIntro", "0");
}


#define MAXINGAMETEXT 5000
char ingameText[MAXINGAMETEXT];

/*
=================
CG_ParseIngameText
=================
*/
void CG_ParseIngameText(void)
{
	char	*token;
	char *buffer;
	int i;
	int len;

	COM_BeginParseSession();

	buffer = ingameText;
	i = 1;	// Zero is null string
	while ( buffer ) 
	{
		token = COM_ParseExt( &buffer, qtrue );

		len = strlen(token);
		if (len)
		{
			ingame_text[i] = (buffer - (len + 1));	// The +1 is to get rid of the " at the beginning of the sting.
			*(buffer - 1) = '\0';		//	Place an string end where is belongs.

			++i;
		}

		if (i> IGT_MAX)
		{
			Com_Printf( S_COLOR_RED "CG_ParseIngameText : too many values!\n");
			return;
		}
	}

	if (i != IGT_MAX)
	{
		Com_Printf( S_COLOR_RED "CG_ParseIngameText : not enough lines! Read %d of %d!\n",i,IGT_MAX);
		for(;i<IGT_MAX;i++) {
			ingame_text[i] = "?";
		}
	}

}

/*
CG_LanguageFilename - create a filename with an extension based on the value in g_language
*/
void CG_LanguageFilename(char *baseName,char *baseExtension,char *finalName)
{
	char	language[32];
	fileHandle_t	file;

	trap_Cvar_VariableStringBuffer("g_language", language, 32 );

	// If it's English then no extension
	if (language[0]=='\0' || Q_stricmp ("ENGLISH",language)==0)
	{
		Com_sprintf(finalName,MAX_QPATH,"%s.%s",baseName,baseExtension);
	}
	else
	{
		Com_sprintf(finalName,MAX_QPATH,"%s_%s.%s",baseName,language,baseExtension);

		//Attempt to load the file
		trap_FS_FOpenFile( finalName, &file, FS_READ );

		if ( file == 0 )	//	This extension doesn't exist, go English.
		{
			Com_sprintf(finalName,MAX_QPATH,"%s.%s",baseName,baseExtension);	//the caller will give the error if this isn't there
		}
		else
		{
			trap_FS_FCloseFile( file );
		}
	}
}

/*
=================
CG_LoadIngameText
=================
*/
void CG_LoadIngameText(void)
{
	int len;
	fileHandle_t	f;
	char fileName[MAX_QPATH];

	CG_LanguageFilename("ext_data/mp_ingametext","dat",fileName);

	len = trap_FS_FOpenFile( fileName, &f, FS_READ );

	if ( !f ) 
	{
		Com_Printf( S_COLOR_RED "CG_LoadIngameText : mp_ingametext.dat file not found!\n");
		return;
	}

	if (len > MAXINGAMETEXT)
	{
		Com_Printf( S_COLOR_RED "CG_LoadIngameText : mp_ingametext.dat file bigger than %d!\n",MAXINGAMETEXT);
		return;
	}

	// initialise the data area
	memset(ingameText, 0, sizeof(ingameText));	

	trap_FS_Read( ingameText, len, f );

	trap_FS_FCloseFile( f );


	CG_ParseIngameText();

}

/*
=================
CG_LoadObjectivesForMap
=================
*/
void CG_LoadObjectivesForMap(void)
{
	int		len, objnum = 0;
	char	*token;
	char	*buf;
	fileHandle_t	f;
	char	fileName[MAX_QPATH];
	char	fullFileName[MAX_QPATH];
	char	objtext[MAX_OBJ_TEXT_LENGTH];

	COM_StripExtension( cgs.mapname, fileName );
	CG_LanguageFilename( fileName, "efo", fullFileName);

	len = trap_FS_FOpenFile( fullFileName, &f, FS_READ );

	if ( len > MAX_OBJ_TEXT_LENGTH )
	{
		Com_Printf( S_COLOR_RED "CG_LoadObjectivesForMap : %s file bigger than %d!\n", fileName, MAX_OBJ_TEXT_LENGTH );
		return;
	}

	trap_FS_Read( objtext, len, f );

	trap_FS_FCloseFile( f );

	buf = objtext;
	//Now parse out each objective
	while ( 1 ) 
	{
		token = COM_ParseExt( &buf, qtrue );
		if ( !token[0] ) {
			break;
		}

		// Normal objective text
		if ( !Q_strncmp( token, "obj", 3 ) ) 
		{
			objnum = atoi( &token[3] );

			if ( objnum < 1 || objnum == MAX_OBJECTIVES ) {
				Com_Printf( "Invalid objective number (%d), valid range is 1 to %d\n", objnum, MAX_OBJECTIVES );
				break;
			}

			//Now read the objective text into the current objective
			token = COM_ParseExt( &buf, qfalse );
			Q_strncpyz( cgs.objectives[objnum-1].text, token, sizeof(cgs.objectives[objnum-1].text) );
		}

		else if ( !Q_strncmp( token, "abridged_obj", 12 ) ) 
		{
			objnum = atoi( &token[12] );

			if ( objnum < 1 || objnum == MAX_OBJECTIVES ) 
			{
				Com_Printf( "Invalid objective number (%d), valid range is 1 to %d\n", objnum, MAX_OBJECTIVES );
				break;
			}

			//Now read the objective text into the current objective
			token = COM_ParseExt( &buf, qfalse );
			Q_strncpyz( cgs.objectives[objnum-1].abridgedText, token, sizeof(cgs.objectives[objnum-1].abridgedText) );
		}
	}
}

qboolean CG_LoadClasses( void )
{
	fileHandle_t	f;
	int				file_len;
	char			buffer[32000];
	char			*token, *textPtr;
	char			filePath[MAX_QPATH];
	int				numClasses=0;
	int				i;

	Com_sprintf( filePath, sizeof( filePath ), "ext_data/classes/%s.classes", cgs.classSet );

	memset( &cgs.classData, 0, sizeof( cg_classData_t ) );

	file_len = trap_FS_FOpenFile( filePath, &f, FS_READ );

	if ( !file_len )
	{
		CG_Printf( S_COLOR_RED "Couldn't find class file: %s\n", filePath );
		return qfalse;
	}
	
	if ( file_len > sizeof( buffer ) )
	{
		CG_Printf( S_COLOR_RED "File %s was way too big to be loaded.\n", filePath );
		return qfalse;
	}

	trap_FS_Read( buffer, file_len, f );
	trap_FS_FCloseFile( f );

	buffer[file_len] = '\0';

	COM_BeginParseSession();

	textPtr = buffer;

	token = COM_Parse( &textPtr );

	if ( !token[0] ) {
		CG_Printf( S_COLOR_RED "ERROR: No data was found when going to parse the file!\n" );
		return qfalse;
	}

	if ( Q_stricmpn( token, "{", 1 ) ) {
		CG_Printf( S_COLOR_RED "ERROR: File did not start with a '{' symbol!\n" );
		return qfalse;
	}

	while( 1 )
	{
		if ( numClasses >= MAX_CLASSES )
			break;


		if ( !Q_strncmp( token, "{", 1 ) )
		{
			while ( 1 )
			{
				token = COM_Parse( &textPtr );
		
				if (!token[0]) {
					break;
				}

				if ( !Q_stricmpn( token, "formalName", 10 ) ) 
				{
					if ( COM_ParseString( &textPtr, &token ) ) 
					{
						CG_Printf( S_COLOR_RED "ERROR: Invalid class formal name in class index: %i.\n", numClasses );
						continue;
					}

					Q_strncpyz( cgs.classData[numClasses].formalName, token, sizeof( cgs.classData[numClasses].formalName ) );
					continue;
				}

				if ( !Q_stricmpn( token, "radarColor", 5 ) ) 
				{
					vec3_t	temp;

					if ( COM_ParseVec3( &textPtr, temp ) ) 
					{
						CG_Printf( S_COLOR_RED "ERROR: Invalid color values in class index: %i.\n", numClasses );
						continue;
					}

					for ( i = 0; i < 3; i++ ) 
					{
						cgs.classData[numClasses].radarColor[i] = (int)Com_Clamp( 0, 255, (int)temp[i] );
						//G_Printf( S_COLOR_RED "g_classData[numClasses].color[%i] = %i\n", i, g_classData[numClasses].color[i] );
					}

					continue;
				}

				if ( !Q_stricmpn( token, "iconColor", 9 ) ) 
				{
					if ( COM_ParseString( &textPtr, &token ) ) 
					{
						CG_Printf( S_COLOR_RED "ERROR: Invalid class icon color in class index: %i.\n", numClasses );
						continue;
					}

					//Eh... there are enum values for these,
					//but they're currently out of scope ;P
					if ( !Q_stricmp( token, "red" ) ) 
					{
						cgs.classData[numClasses].iconColor = 1; //CLR_RED
					}
					else if ( !Q_stricmp( token, "gold" ) ) 
					{
						cgs.classData[numClasses].iconColor = 2; //CLR_GOLD
					}
					else if ( !Q_stricmp( token, "teal" ) ) 
					{
						cgs.classData[numClasses].iconColor = 3; //CLR_TEAL
					}
					else if ( !Q_stricmp( token, "green" ) ) 
					{
						cgs.classData[numClasses].iconColor = 4; //CLR_GREEN
					}
					else 
					{
						cgs.classData[numClasses].iconColor = 0; //0
					}

					continue;
				}

				if ( !Q_stricmpn( token, "medical", 7 ) ) 
				{
					if ( COM_ParseInt( &textPtr, (int *)&cgs.classData[numClasses].isMedic ) ) 
					{
						CG_Printf( S_COLOR_RED "ERROR: Class medic check for class %i was invalid.\n", numClasses );
						continue;
					}

					continue;
				}

				if( !Q_stricmpn( token, "isBorg", 6 ) )
				{
					if( COM_ParseInt( &textPtr, (int *)&cgs.classData[numClasses].isBorg ) )
					{
						CG_Printf( S_COLOR_RED "ERROR: Class borg check for class %i was invalid.\n", numClasses );
						continue;
					}
					continue;
				}

				if ( !Q_stricmpn( token, "hasRanks", 8 ) ) 
				{
					if ( COM_ParseInt( &textPtr, (int *)&cgs.classData[numClasses].showRanks ) ) 
					{
						CG_Printf( S_COLOR_RED "ERROR: Class Ranks check for class %i was invalid.\n", numClasses );
						continue;
					}

					continue;
				}

				//I'm a n00b lol. I made a class called 'medical' and a parameter called 'medical'.
				//I have to double check both parms or else it confuses the parser
				//better check all of them. I'm still getting errors
				if ( !Q_stricmpn( token, "consoleName", 10 ) 
					|| !Q_stricmpn( token, "modelSkin", 9 ) 
					|| !Q_stricmpn( token, "message", 7 )
					|| !Q_stricmpn( token, "admin", 5 )
					|| !Q_stricmpn( token, "marine", 6 )
					|| !Q_stricmpn( token, "noShow", 6 )
					) 
				{
					SkipRestOfLine(&textPtr);
					continue;
				}

				//this one is a pain since it can potentially have multiple lines
				if ( !Q_stricmpn( token, "weapons", 7 ) )
				{
					SkipBracedSection( &textPtr );
					continue;
				}

				if ( !Q_stricmpn( token, "}", 1 ) ) 
				{
					numClasses++;
					break;
				}
			}
		}

		token = COM_Parse( &textPtr );
		
		if (!token[0]) 
		{
			break;
		}
	}

	if ( numClasses > 0 )
		return qtrue;
	else
	{
		CG_Printf( S_COLOR_RED "ERROR: No classes were found in the class file!\n" );
		return qfalse;
	}
}

qboolean CG_LoadUsablesStrings( void )
{
	char			fileRoute[MAX_QPATH];
	char			mapRoute[MAX_QPATH];
	char			buffer[20000];
	int				file_len;
	char			*textPtr, *token;
	fileHandle_t	f;
	int				i;
	int				strLen;

	//setup the file route
	Com_sprintf( mapRoute, sizeof( mapRoute ), "%s", cgs.mapname );

	strLen = strlen( mapRoute );

	//*sigh* remove the bsp bit
	if ( strLen > 4 && !Q_stricmp( mapRoute + strLen-4, ".bsp" ) )
		mapRoute[strLen-4] = '\0';

	//check for language
	CG_LanguageFilename( mapRoute, "usables", fileRoute );

	file_len = trap_FS_FOpenFile( fileRoute, &f, FS_READ );

	//It's assumed most maps won't have this feature, so just exit 'gracefully'
	if ( file_len<=1 )
	{
		//CG_Printf( S_COLOR_YELLOW "WARNING: No file named %s was found. If the server \n", fileRoute );
		trap_FS_FCloseFile( f );
		return qfalse;
	}

	//fill the buffer with the file data
	memset( &buffer, 0, sizeof( buffer ) );
	trap_FS_Read( buffer, file_len, f );
	buffer[file_len] = '0';

	trap_FS_FCloseFile( f );
	
	if ( !buffer[0] )
	{
		CG_Printf( S_COLOR_RED "ERROR: Attempted to load %s, but no data was inside!\n", fileRoute );
		return qfalse;
	}

	COM_BeginParseSession();
	textPtr = buffer;

	i = 0;	//used for the main arrays indices

	while( 1 )
	{
		token = COM_Parse( &textPtr );
		if ( !token[0] )
			break;

		if ( !Q_strncmp( token, "UsableDescriptions", 18 ) )
		{
			token = COM_Parse( &textPtr );
			if ( Q_strncmp( token, "{", 1 ) != 0 )
			{
				CG_Printf( S_COLOR_RED "ERROR: UsableDescriptions had no opening brace ( { )!\n", fileRoute );
				continue;
			}

			token = COM_Parse( &textPtr );

			//expected format is 'id' <space> 'string'
			while ( Q_strncmp( token, "}", 1 ) )
			{
				if ( !token[0] )
					break;

				if ( !Q_strncmp( token, "UsableEntities", 14 ) )
				{
					SkipBracedSection( &textPtr );
					continue;
				}
				else
				{
					//parse past the ID num
					token = COM_ParseExt( &textPtr, qfalse );

					//copy the line of text
					if( token[0] )
					{
						Q_strncpyz( cgs.scannableStrings[i], token, sizeof( cgs.scannableStrings[i] ) );
						i++;
					}

					token = COM_Parse( &textPtr );
				}
			}
		}
	}

	return qtrue;
}
