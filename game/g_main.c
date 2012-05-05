// Copyright (C) 1999-2000 Id Software, Inc.
//

#include "g_local.h"
#include "g_groups.h"

//extern qboolean BG_BorgTransporting( playerState_t *ps );
extern void BG_LoadItemNames(void);

extern qboolean BG_ParseRankNames ( char* fileName, rankNames_t rankNames[] );

qboolean G_LoadMapConfigurations( void );

//RPG-X: RedTechie
//int shaketimer; //Global shake timer varible //TiM: NOT NECESSARY
int RPGEntityCount; //Global entity count varible
//int lastTimedMessage; //The last timed message that was displayed //TiM: Not necessary here

level_locals_t	level;
extern char	races[256];	//this is evil!
extern qboolean levelExiting;

group_list_t	group_list[MAX_GROUP_MEMBERS];
int 			group_count;
int numKilled;

typedef struct {
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;
	int			modificationCount;  // for tracking changes
	qboolean	trackChange;	// track this variable, and announce if changed
} cvarTable_t;

gentity_t	g_entities[MAX_GENTITIES];
gclient_t	g_clients[MAX_CLIENTS];

rankNames_t g_rankNames[MAX_RANKS];

g_classData_t	g_classData[MAX_CLASSES];

vmCvar_t	g_gametype;
vmCvar_t	g_dmflags;
vmCvar_t	g_fraglimit;
vmCvar_t	g_timelimit;
vmCvar_t	g_timelimitWinningTeam;
vmCvar_t	g_capturelimit;
vmCvar_t	g_friendlyFire;
vmCvar_t	g_password;
vmCvar_t	g_needpass;
vmCvar_t	g_maxclients;
vmCvar_t	g_maxGameClients;
vmCvar_t	g_dedicated;
vmCvar_t	g_speed;
vmCvar_t	g_gravity;
vmCvar_t	g_cheats;
vmCvar_t	g_knockback;
vmCvar_t	g_dmgmult;
vmCvar_t	g_forcerespawn;
vmCvar_t	g_inactivity;
vmCvar_t	g_debugMove;
vmCvar_t	g_debugDamage;
vmCvar_t	g_debugAlloc;
vmCvar_t	g_weaponRespawn;
vmCvar_t	g_adaptRespawn;
vmCvar_t	g_motd;
vmCvar_t	g_synchronousClients;
vmCvar_t	g_warmup;
vmCvar_t	g_doWarmup;
vmCvar_t	g_restarted;
vmCvar_t	g_log;
vmCvar_t	g_logSync;
vmCvar_t	g_podiumDist;
vmCvar_t	g_podiumDrop;
vmCvar_t	g_allowVote;
vmCvar_t	g_teamAutoJoin;
vmCvar_t	g_teamForceBalance;
vmCvar_t	g_banIPs;
vmCvar_t	g_filterBan;
vmCvar_t	g_banIDs;	//TiM - Security ban system
vmCvar_t	g_debugForward;
vmCvar_t	g_debugRight;
vmCvar_t	g_debugUp;
vmCvar_t	g_language;
vmCvar_t	g_holoIntro;
//vmCvar_t	g_ghostRespawn;
vmCvar_t	g_intermissionTime;
vmCvar_t	g_team_group_red;
vmCvar_t	g_team_group_blue;
vmCvar_t	g_random_skin_limit;
vmCvar_t	g_classChangeDebounceTime;
//vmCvar_t	ui_playerclass;

//RPG-X: - RedTechie More CVAR INFO
vmCvar_t	rpg_allowvote;
vmCvar_t	rpg_chatsallowed;
vmCvar_t	rpg_allowsuicide;
vmCvar_t	rpg_selfdamage;
vmCvar_t	rpg_rpg;
vmCvar_t	rpg_kickspammers;				//!< Specifies whether player that spam get kicked automatically.
vmCvar_t	rpg_kicksuiciders;				//!< Specifies whether player that sucide get kicked automatically.
vmCvar_t	rpg_allowspmaps;				//!< Specifies whether singleplayer maps can be loaded
vmCvar_t	rpg_rangetricorder;				//!< Maximum range the Tricorder can scan.
vmCvar_t	rpg_rangehypo;					//!< Maximum range of the Hypospray.
vmCvar_t	rpg_norpgclasses;
vmCvar_t	rpg_forceclasscolor;			//!< Specifies whether class colors specified in the *.class file are enforced.
vmCvar_t	rpg_nosecurity;
vmCvar_t	rpg_nomarine;
vmCvar_t	rpg_nomedical;
vmCvar_t	rpg_noscience;
vmCvar_t	rpg_nocommand;
vmCvar_t	rpg_noengineer;
vmCvar_t	rpg_noalien;
vmCvar_t	rpg_nomaker;
vmCvar_t	rpg_non00b;
vmCvar_t	rpg_nocloak;					//!< Can be used to disable the admin cloaking device.
vmCvar_t	rpg_noflight;					//!< Can be used to disable the admin flight mode.
vmCvar_t	rpg_phaserdmg;					//!< Specifies whether the phaser damges players.
vmCvar_t	rpg_rifledmg;					//!< Specifies whether the phaser rifle damages players.
vmCvar_t	rpg_stasisdmg;					//!< Specifies whether the disruptor damages rifles.
vmCvar_t	rpg_imoddmg;					
vmCvar_t	rpg_noweapons;					//!< Can be used to disable all weapons.
//vmCvar_t	rpg_marinepass;
//vmCvar_t	rpg_securitypass;
//vmCvar_t	rpg_adminpass;
//vmCvar_t	rpg_medicalpass;
//vmCvar_t	rpg_sciencepass;
//vmCvar_t	rpg_commandpass;
//vmCvar_t	rpg_engineerpass;
//vmCvar_t	rpg_alienpass;
//vmCvar_t	rpg_n00bpass;
//vmCvar_t	rpg_alienflags;
//vmCvar_t	rpg_marineflags;
//vmCvar_t	rpg_securityflags;
//vmCvar_t	rpg_adminflags;
//vmCvar_t	rpg_medicalflags;
//vmCvar_t	rpg_scienceflags;
//vmCvar_t	rpg_commandflags;
//vmCvar_t	rpg_engineerflags;
vmCvar_t	rpg_welcomemessage;					//!< Welcome message displayed when a player joins the server.
//vmCvar_t	rpg_timedmessage;
vmCvar_t	rpg_timedmessagetime;				//!< Delay between timed mesagges
vmCvar_t	rpg_message1;						//!< Timed message
vmCvar_t	rpg_message2;						//!< Timed message
vmCvar_t	rpg_message3;						//!< Timed message
vmCvar_t	rpg_message4;						//!< Timed message
vmCvar_t	rpg_message5;						//!< Timed message
vmCvar_t	rpg_message6;						//!< Timed message
vmCvar_t	rpg_message7;						//!< Timed message
vmCvar_t	rpg_message8;						//!< Timed message
vmCvar_t	rpg_message9;						//!< Timed message
vmCvar_t	rpg_message10;						//!< Timed message
vmCvar_t	rpg_forcekillradius;				//!< Specifies whether the forcekillradius command is avaible.
vmCvar_t	rpg_forcekillradiuswaittime;		//!< forcekillradius delay
vmCvar_t	rpg_noclipspectating;				//!< Specifies whether spectators uses clipping.
vmCvar_t	rpg_chatarearange;					//!< Maximumrange for area chat.
vmCvar_t	rpg_forcefielddamage;				//!< Damage a player takes when touching an admin force field
vmCvar_t	rpg_invisibletripmines;				//!< Specifies whether invisible tripmines are enabled.
vmCvar_t	rpg_medicsrevive;					//!< Are medics allowed to revive other players
vmCvar_t	rpg_effectsgun;						//!< Can be used to enable/disable the effects gun
vmCvar_t	rpg_phaserdisintegrates;			//!< If enabled phasers disintegrate players instead ob incapacitating them.
//vmCvar_t	rpg_enabledranks;
//vmCvar_t	rpg_servershakeallclients;//RPG-X: RedTechie - Server only shake cmd used to shake clients view when set
//vmCvar_t	rpg_servershakeallclientsintensity;//RPG-X: RedTechie - Server only shake cmd used to shake clients view intensity
//! Kick player for n00bing after this ammount of kills
vmCvar_t	rpg_kickAfterXkills; //RPG-X | Phenix | 06/04/2005
vmCvar_t	rpg_rankSet;						//!< Rankset to use
vmCvar_t	rpg_passMessage;	 //RPG-X | TiM | 2/2/2006
//! If enabled the previous name of a player is displayed if the reconnects with a different one.
vmCvar_t	rpg_renamedPlayers;	 //RPG-X | TiM | For players that disconnect, reconnect with dif names to try and be sneaky...
//! If enabled only one player can have a name at the same time.
vmCvar_t	rpg_uniqueNames;	 //RPG-X | TiM | When active, only one player can have the same name on a server
//RPG-X | TiM | Cvars to make the rank system more controllable
vmCvar_t	rpg_startingRank;	 //!< The rank players will start as, regardless
vmCvar_t	rpg_maxRank;		 //!< Absolute rank players can set themselves to
vmCvar_t	rpg_changeRanks;	 //!< If players are allowed to change ranks themselves

//TiM - height paramters
vmCvar_t	rpg_maxHeight;
vmCvar_t	rpg_minHeight;
vmCvar_t	rpg_maxWeight;
vmCvar_t	rpg_minWeight;

//! Classet to use
vmCvar_t	rpg_classSet;		//TiM: current server class configuration

vmCvar_t	rpg_mapGiveFlags;

vmCvar_t	rpg_scannablePanels;	//!< Scan consoles and doors be scanned with the Tricorder

// Drop stuff
//! Enables weapon dropping
vmCvar_t    rpg_allowWeaponDrop; //RPG-X | Marcin | 03/12/2008
//! Do weapons stay in inventory when a player drops them
vmCvar_t    rpg_weaponsStay;     //RPG-X | Marcin | 04/12/2008
//! Does a player drop his weapons when he dies
vmCvar_t	rpg_dropOnDeath;	 //RPG-X | Marcin | 30/12/2008
//vmCvar_t	rpg_flushDroppedOnDisconnect; //RPG-X | GSIO01 | 08/05/2009

// Weapon speeds
//! Speed for compression riffle  projectiles
vmCvar_t    rpg_rifleSpeed;      //RPG-X | Marcin | 04/12/2008
//! Speed for disruptor projectiles
vmCvar_t    rpg_disruptorSpeed;  //RPG-X | Marcin | 04/12/2008
//! Speed for photon burst projectiles
vmCvar_t    rpg_photonSpeed;     //RPG-X | Marcin | 05/12/2008
//! Speed for altfire photon burst projectiles
vmCvar_t    rpg_altPhotonSpeed;  //RPG-X | Marcin | 06/12/2008

// Weapon delays
//! Fire delay for Compression Rifle
vmCvar_t    rpg_rifleDelay;      //RPG-X | Marcin | 06/12/2008
//! Fire delay for Disruptor
vmCvar_t    rpg_disruptorDelay;  //RPG-X | Marcin | 06/12/2008
//! Fire delay for photon burst primary fire
vmCvar_t    rpg_photonDelay;     //RPG-X | Marcin | 06/12/2008
//! Fire delay for photon burst secondary fire
vmCvar_t    rpg_altPhotonDelay;  //RPG-X | Marcin | 06/12/2008
//! Fire delay for TR116
vmCvar_t	rpg_TR116Delay;		 //RPG-X | Marcin | 30/12/2008
//! Fire delay for Tricorder alt fire
vmCvar_t	rpg_altTricorderDelay;	 //RPG-X | GSIO01 | 14/05/2009

// Weapon Damage
vmCvar_t	rpg_rifleDamage;
vmCvar_t	rpg_rifleAltDamage;
vmCvar_t	rpg_phaserDamage;
vmCvar_t	rpg_disruptorDamage;
vmCvar_t	rpg_grenadeDamage;
vmCvar_t	rpg_grenadeAltDamage;
vmCvar_t	rpg_tr116Damage;
vmCvar_t	rpg_photonDamage;
vmCvar_t	rpg_photonAltDamage;

// Motd
//! Specifies the message of the day file
vmCvar_t	rpg_motdFile;        //RPG-X | Marcin | 23/12/2008

// Privacy
//! If enabled admins can the private chat messages
vmCvar_t	rpg_respectPrivacy;  //RPG-X | Marcin | 24/12/2008

// Weaps
vmCvar_t	rpg_maxTricorders;	 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_maxPADDs;		 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_maxCups;		 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_maxPhasers;		 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_maxRifles;		 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_maxTR116s;		 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_maxAdminguns;	 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_maxPhotonbursts; //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_maxDisruptors;	 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_maxMedkits;		 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_maxHyposprays;	 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_maxRegenerators; //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_maxToolkits;	 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_maxHyperSpanners;//RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_minTricorders;	 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_minPADDs;		 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_minCups;		 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_minPhasers;		 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_minRifles;		 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_minTR116s;		 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_minAdminguns;	 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_minPhotonbursts; //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_minDisruptors;	 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_minMedkits;		 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_minHyposprays;	 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_minRegenerators; //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_minToolkits;	 //RPG-X | Marcin | 30/12/2008
vmCvar_t	rpg_minHyperSpanners;//RPG-X | Marcin | 30/12/2008

// respawn delay
//! Delay for respawn function
vmCvar_t	rpg_fraggerSpawnDelay; //RPG-X | Marcin | 03/01/2009

// borg adaption
//! If enabled borg get immune to weapons after some time
vmCvar_t	rpg_borgAdapt;				//RPG-X | GSIO01 | 08/05/2009
//! Borg shield plays sound if hit?
vmCvar_t	rpg_adaptUseSound;			//RPG-X | GSIO01 | 08/05/2009
//! Number of hits  before borg adapt to this weapon
vmCvar_t	rpg_adaptPhaserHits;		//RPG-X | GSIO01 | 08/05/2009
//! Number of hits  before borg adapt to this weapon
vmCvar_t	rpg_adaptPhotonHits;		//RPG-X | GSIO01 | 08/05/2009
//! Number of hits  before borg adapt to this weapon
vmCvar_t	rpg_adaptCrifleHits;		//RPG-X | GSIO01 | 08/05/2009
//! Number of hits  before borg adapt to this weapon
vmCvar_t	rpg_adaptDisruptorHits;		//RPG-X | GSIO01 | 08/05/2009
//! Number of hits  before borg adapt to this weapon
vmCvar_t	rpg_adaptTR116Hits;			//RPG-X | GSIO01 | 08/05/2009
//! Number of hits  before borg adapt to this weapon
vmCvar_t	rpg_adaptGrenadeLauncherHits; //RPG-X | GSIO01 | 08/05/2009
//! Are player allowed to remodulate their weapons
vmCvar_t	rpg_allowRemodulation;		//RPG-X | GSIO01 | 10/05/2009
//! Can borg move through admin force fields
vmCvar_t	rpg_borgMoveThroughFields;	//RPG-X | GSIO01 | 19/05/2009
//! Delay for modulation
vmCvar_t	rpg_RemodulationDelay;

// hypo melee
//! Can the hypospray be used as weapons
vmCvar_t	rpg_hypoMelee;

// repairing breakables
//! Can be used to modify repairspeed
vmCvar_t	rpg_repairModifier;	//RPG-X | GSIO01 | 09/05/2009

// force field colors
//! Current force field color
vmCvar_t	rpg_forceFieldColor; //RPG-X | GSIO01 | 09/05/2009

// modulation thingies //RPG-X | GSIO01 | 12/05/2009
vmCvar_t	rpg_forceFieldFreq;

// calc lif travel duration by dec distance
//! Calculathe the travel durration of the turbolift by the difference between the deck numbers?
vmCvar_t	rpg_calcLiftTravelDuration;
//! Can be used to modify the lift speed
vmCvar_t	rpg_liftDurationModifier;

// admin vote override
//! Admins can override votes
vmCvar_t	rpg_adminVoteOverride;

// server change
//! Enables/disables target_serverchange
vmCvar_t	rpg_serverchange;
vmCvar_t	rpg_server1;
vmCvar_t	rpg_server2;
vmCvar_t	rpg_server3;
vmCvar_t	rpg_server4;
vmCvar_t	rpg_server5;
vmCvar_t	rpg_server6;

// SP level change
//! Allow target_levelchange to change the current level?
vmCvar_t	rpg_allowSPLevelChange;

#ifdef XTRA
vmCvar_t	sql_dbName;		//!< Name of the SQL Database
vmCvar_t	sql_use;		//!< Use SQL? 1 = mysql, 2 = sqlite
vmCvar_t	sql_server;		//!< SQL server to connect to (only mysql)
vmCvar_t	sql_user;		//!< SQL user for sql_server (only mysql)
vmCvar_t	sql_password;	//!< SQL password for sql_server (only mysql)
vmCvar_t	sql_port;		//!< SQL port to use to connect to sql_server (only mysql)
vmCvar_t	sql_hash;		//!< Specifies whether passwords should be hashed and what hash to use (only mysql)
#endif

// developer tools
vmCvar_t	dev_showTriggers;

#ifdef G_LUA
// Print Lua debugging information into the game console?
vmCvar_t        g_debugLua;
vmCvar_t        lua_modules;
vmCvar_t        lua_allowedModules;
#endif

// XPERIMENTAL
//vmCvar_t	rpg_useLanguages;


static cvarTable_t		gameCvarTable[] = {
	// don't override the cheat state set by the system
	{ &g_cheats, "sv_cheats", "", 0, 0, qfalse },

	// noset vars
	{ NULL, "gamename", GAMEVERSION , CVAR_SERVERINFO | CVAR_ROM, 0, qfalse  },
	{ NULL, "gamedate", __DATE__ , CVAR_ROM, 0, qfalse  },
	{ &g_restarted, "g_restarted", "0", CVAR_ROM, 0, qfalse  },
	{ NULL, "sv_mapname", "", CVAR_SERVERINFO | CVAR_ROM, 0, qfalse  },

	// latched vars
	{ &g_gametype, "g_gametype", "0", CVAR_SERVERINFO | CVAR_LATCH | CVAR_INIT | CVAR_ROM, 0, qfalse  },

	{ &g_maxclients, "sv_maxclients", "8", CVAR_SERVERINFO | CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  },
	{ &g_maxGameClients, "g_maxGameClients", "0", CVAR_SERVERINFO | CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  },

	// change anytime vars
	{ &g_dmflags, "dmflags", "0", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue  },
	{ &g_fraglimit, "fraglimit", "20", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_timelimit, "timelimit", "0", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_timelimitWinningTeam, "timelimitWinningTeam", "", CVAR_NORESTART, 0, qtrue },
	{ &g_capturelimit, "capturelimit", "8", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },

	{ &g_synchronousClients, "g_synchronousClients", "0", CVAR_SYSTEMINFO, 0, qfalse  },

	{ &g_friendlyFire, "g_friendlyFire", "0", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue },

	{ &g_teamAutoJoin, "g_teamAutoJoin", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &g_teamForceBalance, "g_teamForceBalance", "1", CVAR_ARCHIVE, 0, qfalse },

	{ &g_intermissionTime, "g_intermissionTime", "20", CVAR_ARCHIVE, 0, qtrue },
	{ &g_warmup, "g_warmup", "20", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse },
	{ &g_doWarmup, "g_doWarmup", "0", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse },
	{ &g_log, "g_log", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &g_logSync, "g_logSync", "0", CVAR_ARCHIVE, 0, qfalse  },

	{ &g_password, "g_password", "", CVAR_USERINFO, 0, qfalse  },

	{ &g_banIPs, "g_banIPs", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &g_filterBan, "g_filterBan", "1", CVAR_ARCHIVE, 0, qfalse  },

	{ &g_banIDs, "g_banIDs", "", CVAR_ARCHIVE, 0, qfalse },

	{ &g_needpass, "g_needpass", "0", CVAR_SERVERINFO | CVAR_ROM, 0, qfalse },

	{ &g_dedicated, "dedicated", "0", 0, 0, qfalse  },

	{ &g_speed, "g_speed", "250", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue  },				// Quake 3 default was 320.
	{ &g_gravity, "g_gravity", "800", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue  },
	{ &g_knockback, "g_knockback", "500", 0, 0, qtrue  },
	{ &g_dmgmult, "g_dmgmult", "1", 0, 0, qtrue  },
	{ &g_weaponRespawn, "g_weaponrespawn", "5", 0, 0, qtrue  },		// Quake 3 default (with 1 ammo weapons) was 5.
	{ &g_adaptRespawn, "g_adaptrespawn", "1", 0, 0, qtrue  },		// Make weapons respawn faster with a lot of players.
	{ &g_forcerespawn, "g_forcerespawn", "0", 0, 0, qtrue },		// Quake 3 default was 20.  This is more "user friendly".
	{ &g_inactivity, "g_inactivity", "0", 0, 0, qtrue },
	{ &g_debugMove, "g_debugMove", "0", 0, 0, qfalse },
	{ &g_debugDamage, "g_debugDamage", "0", 0, 0, qfalse },
	{ &g_debugAlloc, "g_debugAlloc", "0", 0, 0, qfalse },
	{ &g_motd, "g_motd", "", 0, 0, qfalse },

	{ &g_podiumDist, "g_podiumDist", "80", 0, 0, qfalse },
	{ &g_podiumDrop, "g_podiumDrop", "70", 0, 0, qfalse },

	{ &g_allowVote, "g_allowVote", "1", CVAR_SERVERINFO, 0, qfalse },

#if 0
	{ &g_debugForward, "g_debugForward", "0", 0, 0, qfalse },
	{ &g_debugRight, "g_debugRight", "0", 0, 0, qfalse },
	{ &g_debugUp, "g_debugUp", "0", 0, 0, qfalse },
#endif

	{ &g_language, "g_language", "", CVAR_ARCHIVE, 0, qfalse  },

	{ &g_holoIntro, "g_holoIntro", "1", CVAR_ARCHIVE, 0, qfalse},
//	{ &g_ghostRespawn, "g_ghostRespawn", "5", CVAR_ARCHIVE, 0, qfalse},		// How long the player is ghosted, in seconds.
	{ &g_team_group_red, "g_team_group_red", "", CVAR_LATCH, 0, qfalse  },		// Used to have CVAR_ARCHIVE	
	{ &g_team_group_blue, "g_team_group_blue", "", CVAR_LATCH, 0, qfalse  },		// Used to have CVAR_ARCHIVE
	{ &g_random_skin_limit, "g_random_skin_limit", "4", CVAR_ARCHIVE, 0, qfalse },
	{ &g_classChangeDebounceTime, "g_classChangeDebounceTime", "180", CVAR_ARCHIVE, 0, qfalse },
		
//	{ &ui_playerclass, "ui_playerclass", "NOCLASS", CVAR_ARCHIVE, 0, qfalse },
	
	//RPG-X: RedTechie - RPG-X CVARS....duh....just for the slow ones
	{ &rpg_allowvote, "rpg_allowVote", "1", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_chatsallowed, "rpg_chatsAllowed", "10", CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_allowsuicide, "rpg_allowSuicide", "1", CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_selfdamage, "rpg_selfDamage", "1", CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_rpg, "rpg_rpg", "1", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_kickspammers, "rpg_kickSpammers", "0", CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_kicksuiciders, "rpg_kickSuiciders", "0", CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_allowspmaps, "rpg_allowSPMaps", "0", CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_rangetricorder, "rpg_rangeTricorder", "128", CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_rangehypo, "rpg_rangeHypo", "32", CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_norpgclasses, "rpg_noRPGClasses", "0", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_forceclasscolor, "rpg_forceClassColor", "0", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_nosecurity, "rpg_noSecurity", "0", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_nomarine, "rpg_noMarine", "0", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_nomedical, "rpg_noMedical", "0", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_noscience, "rpg_noScience", "0", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_nocommand, "rpg_noCommand", "0", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_noengineer, "rpg_noEngineer", "0", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_noalien, "rpg_noAlien", "0", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_non00b, "rpg_non00b", "0", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_nomaker, "rpg_noMaker", "0", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_nocloak, "rpg_noCloak", "0", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_noflight, "rpg_noFlight", "0", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_phaserdmg, "rpg_phaserDmg", "1", CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_rifledmg, "rpg_rifleDmg", "1", CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_stasisdmg, "rpg_stasisDmg", "1", CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_imoddmg, "rpg_imodDmg", "1", CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_noweapons, "rpg_noWeapons", "0", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},
	/*{ &rpg_marinepass, "rpg_marinepass", "marine", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_securitypass, "rpg_securityPass", "security", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_adminpass, "rpg_adminPass", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_medicalpass, "rpg_medicalPass", "medical", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_sciencepass, "rpg_sciencePass", "science", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_commandpass, "rpg_commandPass", "command", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_engineerpass, "rpg_engineerPass", "engineer", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_alienpass, "rpg_alienpass", "alien", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_n00bpass, "rpg_n00bpass", "n00b", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_alienflags, "rpg_alienFlags", "1026", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  }, //16
	//{ &rpg_marineflags, "rpg_marineflags", "8184", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_marineflags, "rpg_marineFlags", "12398", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  }, //111
	{ &rpg_securityflags, "rpg_securityFlags", "110", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  }, //15
	{ &rpg_adminflags, "rpg_adminFlags", "65534", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  }, //24361
	{ &rpg_medicalflags, "rpg_medicalFlags", "14382", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  }, //231
	{ &rpg_scienceflags, "rpg_scienceFlags", "46", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  }, //7
	{ &rpg_commandflags, "rpg_commandFlags", "62", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  }, //6
	{ &rpg_engineerflags, "rpg_engineerFlags", "49198", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  }, //775*/
	{ &rpg_welcomemessage, "rpg_welcomeMessage", "Welcome to the RPG-X Mod", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  },
//	{ &rpg_timedmessage, "rpg_timedmessage", "Server is in: Character Development Mode", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_timedmessagetime, "rpg_timedMessageTime", "5", CVAR_ARCHIVE, 0, qfalse  }, //TiM : LATCH Not necessary here.
	{ &rpg_message1, "rpg_message1", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_message2, "rpg_message2", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_message3, "rpg_message3", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_message4, "rpg_message4", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_message5, "rpg_message5", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_message6, "rpg_message6", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_message7, "rpg_message7", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_message8, "rpg_message8", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_message9, "rpg_message9", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_message10, "rpg_message10", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &rpg_forcekillradius, "rpg_forceKillRadius", "0", CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue},
	{ &rpg_forcekillradiuswaittime, "rpg_forceKillRadiusWaitTime", "45000", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},
	//{ &rpg_noclipspectating, "rpg_noclipSpectating", "1", CVAR_ARCHIVE, 0, qtrue},		//Not latched (ie doesnt need server restart)
	{ &rpg_chatarearange, "rpg_chatAreaRange", "200", CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue},	//Not latched (ie doesnt need server restart)
	{ &rpg_forcefielddamage, "rpg_forcefieldDamage", "0", CVAR_ARCHIVE, 0, qfalse},
	{ &rpg_invisibletripmines, "rpg_invisibleTripmines", "1", CVAR_ARCHIVE | CVAR_SERVERINFO, 0, qfalse},
	{ &rpg_medicsrevive, "rpg_medicsRevive", "1", CVAR_ARCHIVE | CVAR_LATCH, 0, qfalse},
	{ &rpg_effectsgun, "rpg_effectsGun", "1", CVAR_ARCHIVE | CVAR_SERVERINFO, 0, qfalse},
	{ &rpg_phaserdisintegrates, "rpg_phaserDisintegrates", "1", CVAR_ARCHIVE, 0, qfalse},
	//{ &rpg_servershakeallclients, "rpg_serverShakeAllClients", "0", CVAR_ARCHIVE | CVAR_SERVERINFO, 0, qfalse}, //RPG-X: RedTechie - SHould not be used directly this is to shake allll clients views if set on
	//{ &rpg_servershakeallclientsintensity, "rpg_serverShakeAllClientsIntensity", "2", CVAR_ARCHIVE | CVAR_SERVERINFO, 0, qfalse}, //RPG-X: RedTechie - SHould not be used directly this is to shake allll clients views itensity
	//{ &rpg_enabledranks, "rpg_enabledRanks", "65535", CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse},//RPG-X: RedTechie - This use to be CVAR_ROM jason i have no idea why you set it to read only but im setting it back to normal to fix ranks
	{ &rpg_kickAfterXkills, "rpg_kickAfterNumkills", "2", CVAR_ARCHIVE, 0, qfalse }, //RPG-X | Phenix | 06/04/2005
	{ &rpg_rankSet, "rpg_rankSet", RANKSET_DEFAULT, CVAR_ARCHIVE | CVAR_SERVERINFO | CVAR_LATCH, 0, qfalse },

	{ &rpg_passMessage, "rpg_passMessage", "", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_renamedPlayers, "rpg_renamedPlayers", "1", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_uniqueNames, "rpg_uniqueNames", "1", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_startingRank, "rpg_startingRank", "", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_maxRank,	"rpg_maxRank", "", CVAR_ARCHIVE | CVAR_SERVERINFO, 0, qfalse },
	{ &rpg_changeRanks, "rpg_changeRanks", "1", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_classSet, "rpg_classSet", "rpgx_default", CVAR_ARCHIVE | CVAR_SERVERINFO | CVAR_LATCH, 0, qfalse },

	{ &rpg_maxHeight, "rpg_maxHeight", "1.15", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_minHeight, "rpg_minHeight", "0.90", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_maxWeight, "rpg_maxWeight", "1.10", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_minWeight, "rpg_minWeight", "0.90", CVAR_ARCHIVE, 0, qfalse },

	{ &rpg_mapGiveFlags, "rpg_mapGiveFlags", "0", CVAR_ARCHIVE | CVAR_LATCH, 0, qfalse },

	//TiM - maybe we can fix it later, but for now, disable it
	{ &rpg_scannablePanels, "rpg_scannablePanels", "1", CVAR_ARCHIVE | CVAR_LATCH | CVAR_SERVERINFO, 0, qfalse },
	//{ &rpg_scannableForceField, "rpg_scannableForceField", "1", CVAR_ARCHIVE, 0, qfalse }, //RPG-X | GSIO01 | 13/05/2009
    { &rpg_allowWeaponDrop, "rpg_allowWeaponDrop", "1", CVAR_ARCHIVE, 0, qfalse }, // RPG-X | Marcin | 03/12/2008
    { &rpg_weaponsStay, "rpg_weaponsStay", "0", CVAR_ARCHIVE, 0, qfalse },         // RPG-X | Marcin | 04/12/2008
    { &rpg_rifleSpeed, "rpg_rifleSpeed", "2700", 0, 0, qtrue },        // RPG-X | Marcin | 04/12/2008
    { &rpg_disruptorSpeed, "rpg_disruptorSpeed", "3000", 0, 0, qtrue },// RPG-X | Marcin | 04/12/2008
    { &rpg_photonSpeed, "rpg_photonSpeed", "1300", 0, 0, qtrue },      // RPG-X | Marcin | 05/12/2008
    { &rpg_altPhotonSpeed, "rpg_altPhotonSpeed", "650", 0, 0, qtrue }, // RPG-X | Marcin | 06/12/2008
    { &rpg_rifleDelay, "rpg_rifleDelay", "250", 0, 0, qtrue },         // RPG-X | Marcin | 06/12/2008
    { &rpg_disruptorDelay, "rpg_disruptorDelay", "700", 0, 0, qtrue }, // RPG-X | Marcin | 06/12/2008
    { &rpg_photonDelay, "rpg_photonDelay", "1200", 0, 0, qtrue },      // RPG-X | Marcin | 06/12/2008
    { &rpg_altPhotonDelay, "rpg_altPhotonDelay", "1600", 0, 0, qtrue },// RPG-X | Marcin | 06/12/2008
	{ &rpg_TR116Delay, "rpg_TR116Delay", "500", 0, 0, qtrue },		   // RPG-X | Marcin | 30/12/2008
	{ &rpg_motdFile, "rpg_motdFile", "RPG-X_Motd.txt", CVAR_ARCHIVE | CVAR_LATCH, 0, qfalse },		// RPG-X | Marcin | 23/12/2008
	{ &rpg_respectPrivacy, "rpg_respectPrivacy", "0", CVAR_ARCHIVE | CVAR_SERVERINFO , 0, qfalse },	// RPG-X | Marcin | 24/12/2008
	{ &rpg_maxTricorders, "rpg_maxTricorders", "1", CVAR_ARCHIVE, 0, qtrue },						// RPG-X | Marcin | 30/12/2008
	{ &rpg_maxPADDs, "rpg_maxPADDs", "10", CVAR_ARCHIVE, 0, qtrue },								// RPG-X | Marcin | 30/12/2008
	{ &rpg_maxCups, "rpg_maxCups", "2", CVAR_ARCHIVE, 0, qtrue },									// RPG-X | Marcin | 30/12/2008
	{ &rpg_maxPhasers, "rpg_maxPhasers", "1", CVAR_ARCHIVE, 0, qtrue },								// RPG-X | Marcin | 30/12/2008
	{ &rpg_maxRifles, "rpg_maxRifles", "1", CVAR_ARCHIVE, 0, qtrue },								// RPG-X | Marcin | 30/12/2008
	{ &rpg_maxTR116s, "rpg_maxTR116s", "1", CVAR_ARCHIVE, 0, qtrue },								// RPG-X | Marcin | 30/12/2008
	{ &rpg_maxAdminguns, "rpg_maxAdminguns", "1", CVAR_ARCHIVE, 0, qtrue },							// RPG-X | Marcin | 30/12/2008
	{ &rpg_maxPhotonbursts, "rpg_maxPhotonbursts", "1", CVAR_ARCHIVE, 0, qtrue },					// RPG-X | Marcin | 30/12/2008
	{ &rpg_maxDisruptors, "rpg_maxDisruptors", "1", CVAR_ARCHIVE, 0, qtrue },						// RPG-X | Marcin | 30/12/2008
	{ &rpg_maxMedkits, "rpg_maxMedkits", "1", CVAR_ARCHIVE, 0, qtrue },								// RPG-X | Marcin | 30/12/2008
	{ &rpg_maxHyposprays, "rpg_maxHyposprays", "2", CVAR_ARCHIVE, 0, qtrue },						// RPG-X | Marcin | 30/12/2008
	{ &rpg_maxRegenerators, "rpg_maxRegenerators", "1", CVAR_ARCHIVE, 0, qtrue },					// RPG-X | Marcin | 30/12/2008
	{ &rpg_maxToolkits, "rpg_maxToolkits", "1", CVAR_ARCHIVE, 0, qtrue },							// RPG-X | Marcin | 30/12/2008
	{ &rpg_maxHyperSpanners, "rpg_maxHyperSpanners", "1", CVAR_ARCHIVE, 0, qtrue },					// RPG-X | Marcin | 30/12/2008
	{ &rpg_minTricorders, "rpg_minTricorders", "1", CVAR_ARCHIVE, 0, qtrue },						// RPG-X | Marcin | 30/12/2008
	{ &rpg_minPADDs, "rpg_minPADDs", "5", CVAR_ARCHIVE, 0, qtrue },									// RPG-X | Marcin | 30/12/2008
	{ &rpg_minCups, "rpg_minCups", "1", CVAR_ARCHIVE, 0, qtrue },									// RPG-X | Marcin | 30/12/2008
	{ &rpg_minPhasers, "rpg_minPhasers", "1", CVAR_ARCHIVE, 0, qtrue },								// RPG-X | Marcin | 30/12/2008
	{ &rpg_minRifles, "rpg_minRifles", "1", CVAR_ARCHIVE, 0, qtrue },								// RPG-X | Marcin | 30/12/2008
	{ &rpg_minTR116s, "rpg_minTR116s", "1", CVAR_ARCHIVE, 0, qtrue },								// RPG-X | Marcin | 30/12/2008
	{ &rpg_minAdminguns, "rpg_minAdminguns", "1", CVAR_ARCHIVE, 0, qtrue },							// RPG-X | Marcin | 30/12/2008
	{ &rpg_minPhotonbursts, "rpg_minPhotonbursts", "1", CVAR_ARCHIVE, 0, qtrue },					// RPG-X | Marcin | 30/12/2008
	{ &rpg_minDisruptors, "rpg_minDisruptors", "1", CVAR_ARCHIVE, 0, qtrue },						// RPG-X | Marcin | 30/12/2008
	{ &rpg_minMedkits, "rpg_minMedkits", "1", CVAR_ARCHIVE, 0, qtrue },								// RPG-X | Marcin | 30/12/2008
	{ &rpg_minHyposprays, "rpg_minHyposprays", "1", CVAR_ARCHIVE, 0, qtrue },						// RPG-X | Marcin | 30/12/2008
	{ &rpg_minRegenerators, "rpg_minRegenerators", "1", CVAR_ARCHIVE, 0, qtrue },					// RPG-X | Marcin | 30/12/2008
	{ &rpg_minToolkits, "rpg_minToolkits", "1", CVAR_ARCHIVE, 0, qtrue },							// RPG-X | Marcin | 30/12/2008
	{ &rpg_minHyperSpanners, "rpg_minHyperSpanners", "1", CVAR_ARCHIVE, 0, qtrue },					// RPG-X | Marcin | 30/12/2008
    { &rpg_dropOnDeath, "rpg_dropItemsOnDeath", "1", CVAR_ARCHIVE, 0, qfalse },						// RPG-X | Marcin | 30/12/2008
	{ &rpg_fraggerSpawnDelay, "rpg_fraggerSpawnDelay", "100", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_borgAdapt, "rpg_borgAdapt", "0", CVAR_ARCHIVE, 0, qfalse },
	//{ &rpg_flushDroppedOnDisconnect, "rpg_flushDroppedOnDisconnect", "1", CVAR_ARCHIVE, 0, qfalse }
	{ &rpg_hypoMelee, "rpg_hypoMelee", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_adaptUseSound, "rpg_adaptUseSound", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_adaptCrifleHits, "rpg_adaptCrifleHits", "6", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_adaptDisruptorHits, "rpg_adaptDisruptorHits", "6", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_adaptPhaserHits, "rpg_adaptPhaserHits", "6", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_adaptPhotonHits, "rpg_adaptPhotonHits", "6", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_adaptTR116Hits, "rpg_adaptTR116Hits", "6", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_adaptGrenadeLauncherHits, "rpg_adaptGrenadeLauncherHits", "6", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_repairModifier, "rpg_repairModifier", "1", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_forceFieldColor, "rpg_forceFieldColor", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_allowRemodulation, "rpg_allowRemodulation", "1", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_forceFieldFreq, "rpg_forceFieldFreq", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_calcLiftTravelDuration, "rpg_calcLiftTravelDuration", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_liftDurationModifier, "rpg_liftDurationModifier" , "0.5", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_altTricorderDelay, "rpg_altTricorderDelay", "1000", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_borgMoveThroughFields, "rpg_borgMoveThroughFields", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_RemodulationDelay, "rpg_RemodulationDelay", "5000", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_adminVoteOverride, "rpg_adminVoteOverride", "1", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_serverchange, "rpg_serverchange", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_server1, "rpg_server1", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_server2, "rpg_server2", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_server3, "rpg_server3", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_server4, "rpg_server4", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_server5, "rpg_server5", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_server6, "rpg_server6", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_allowSPLevelChange, "rpg_allowSPLevelChange", "1", CVAR_ARCHIVE | CVAR_LATCH, 0, qfalse },
	//XPERIMENTAL
	//{ &rpg_useLanguages, "rpg_useLanguages", "1", CVAR_ARCHIVE, 0, qfalse }
	
	{ &dev_showTriggers, "dev_showTriggers", "0", CVAR_ARCHIVE, 0, qfalse }

#ifdef XTRA
	,
	{ &sql_dbName, "sql_dbName", "rpgx", CVAR_ARCHIVE, 0, qfalse },
	{ &sql_use, "sql_use", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &sql_password, "sql_password", "", CVAR_ARCHIVE, 0, qfalse },
	{ &sql_port, "sql_port", "3306", CVAR_ARCHIVE, 0, qfalse },
	{ &sql_server, "sql_server", "", CVAR_ARCHIVE, 0, qfalse },
	{ &sql_user, "sql_user", "rpgx", CVAR_ARCHIVE, 0, qfalse },
	{ &sql_hash, "sql_hash", "0", CVAR_ARCHIVE, 0, qfalse }
#endif

#ifdef G_LUA
	,
	{ &g_debugLua, "g_debugLua", "0", 0, 0, qfalse },
	{ &lua_allowedModules, "lua_allowedModules", "", 0, 0, qfalse },
	{ &lua_modules, "lua_modules", "", 0, 0, qfalse },
#endif

	{ &rpg_rifleDamage, "rpg_rifleDamage", "75", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_rifleAltDamage, "rpg_rifleAltDamage", "16", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_phaserDamage, "rpg_phaserDamage", "55", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_disruptorDamage, "rpg_disruptorDamage", "80", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_grenadeDamage, "rpg_grenadeDamage", "75", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_grenadeAltDamage, "rpg_grenadeAltDamage", "80", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_tr116Damage, "rpg_tr116Damage", "150", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_photonDamage, "rpg_photonDamage", "140", CVAR_ARCHIVE, 0, qfalse },
	{ &rpg_photonAltDamage, "rpg_photonAltDamage", "140", CVAR_ARCHIVE, 0, qfalse }
};

static int	gameCvarTableSize = sizeof( gameCvarTable ) / sizeof( gameCvarTable[0] );

void G_InitGame( int levelTime, int randomSeed, int restart );
void G_RunFrame( int levelTime );
void G_ShutdownGame( int restart );
void CheckExitRules( void );

//=============================
//** begin code

/*
================
vmMain

This is the only way control passes into the module.
This MUST be the very first function compiled into the .q3vm file
================
*/
//intptr_t
intptr_t vmMain( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6 ) {
	switch ( command ) {
	case GAME_INIT:
		G_InitGame( arg0, arg1, arg2 );
		return 0;
	case GAME_SHUTDOWN:
		G_ShutdownGame( arg0 );
		return 0;
	case GAME_CLIENT_CONNECT:
		return (size_t)ClientConnect( arg0, arg1, arg2 );
	case GAME_CLIENT_THINK:
		ClientThink( arg0 );
		return 0;
	case GAME_CLIENT_USERINFO_CHANGED:
		ClientUserinfoChanged( arg0 ); //TiM - this means a user just tried to change it
		return 0;
	case GAME_CLIENT_DISCONNECT:
		ClientDisconnect( arg0 );
		return 0;
	case GAME_CLIENT_BEGIN:
		ClientBegin( arg0, qtrue, qfalse, qtrue );
		return 0;
	case GAME_CLIENT_COMMAND:
		ClientCommand( arg0 );
		return 0;
	case GAME_RUN_FRAME:
		G_RunFrame( arg0 );
		return 0;
	case GAME_CONSOLE_COMMAND:
		//RPG-X : TiM - plagiarised Red's logic from SFEFMOD here lol
		return ConsoleCommand();
		//return ConsoleCommand();
	case BOTAI_START_FRAME:
		return BotAIStartFrame( arg0 );
	}

	return -1;
}

void QDECL G_PrintfClientAll(const char *fmt, ...) {
	va_list argptr;
	char	text[1024];

	va_start (argptr, fmt);
	vsprintf (text, fmt, argptr);
	va_end	 (argptr);

	trap_SendServerCommand(-1, va("print \"%s\n\"", text));
}

void QDECL G_PrintfClient(gentity_t *ent, const char *fmt, ...) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, fmt);
	vsprintf (text, fmt, argptr);
	va_end	 (argptr);

	#ifdef G_LUA
	LuaHook_G_ClientPrint(text, ent-g_entities);
	#endif
	
	trap_SendServerCommand(ent-g_entities, va("print \"%s\n\"", text));
}

void QDECL G_Printf( const char *fmt, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, fmt);
	vsprintf (text, fmt, argptr);
	va_end (argptr);

	#ifdef G_LUA
	// Lua API callbacks
	LuaHook_G_Print(text);
	#endif

	trap_Printf( text );
}

void QDECL G_Error( const char *fmt, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, fmt);
	vsprintf (text, fmt, argptr);
	va_end (argptr);

	#ifdef G_LUA
	G_LuaShutdown();
	#endif

	trap_Error( text );
}

stringID_table_t WeaponTable[] = {

	{ ENUM2STRING(WP_2) },
	{ ENUM2STRING(WP_3) },
	{ ENUM2STRING(WP_4) },				

	{ ENUM2STRING(WP_5) },				
	{ ENUM2STRING(WP_6) },	
	{ ENUM2STRING(WP_7) },

	{ ENUM2STRING(WP_8) },
	{ ENUM2STRING(WP_9) },		
	{ ENUM2STRING(WP_10) },			

	{ ENUM2STRING(WP_11) },				
	{ ENUM2STRING(WP_12) },		
	{ ENUM2STRING(WP_13) },		
	
	{ ENUM2STRING(WP_14) },				
	{ ENUM2STRING(WP_15) },
	{ NULL, -1 }
};

/**************************
G_LoadClassData

TiM: Loads a ".class" file
and parses the class data
for utilization on the server
and transfer to clients
**************************/
#ifdef Q3_VM
static qboolean G_LoadClassData( char* fileName )
{
	char			buffer[32000];
	char			*textPtr, *token;
	int				fileLen;
	fileHandle_t	f;
	qboolean		classValid=qfalse;
	int				classIndex=0;
	int				weapon;
	int				i;

	//Init the storage place
	memset( &g_classData, 0, sizeof ( g_classData ) );
	
	fileLen = trap_FS_FOpenFile( fileName, &f, FS_READ );

	if ( !f ) {
		G_Printf( S_COLOR_RED "ERROR: File %s not found.\n", fileName );
		return qfalse;
	}

	if ( fileLen >= sizeof( buffer ) ) {
		G_Printf( S_COLOR_RED "ERROR: File %s was way too big.\n", fileName );
		trap_FS_FCloseFile( f );
		return qfalse;
	}

	trap_FS_Read( buffer, fileLen, f );
	buffer[fileLen] = 0;
	trap_FS_FCloseFile( f );

	COM_BeginParseSession();

	textPtr = buffer;

	token = COM_Parse( &textPtr );

	if ( !token[0] ) {
		G_Printf( S_COLOR_RED "ERROR: No data was found when going to parse the file!\n" );
		return qfalse;
	}

	if ( Q_stricmpn( token, "{", 1 ) ) {
		G_Printf( S_COLOR_RED "ERROR: File did not start with a '{' symbol!\n" );
		return qfalse;
	}

	while ( 1 ) 
	{
		if ( classIndex >= MAX_CLASSES )
			break;

		if ( !Q_stricmpn( token, "{", 1 ) ) 
		{
			while ( 1 ) 
			{
				token = COM_Parse( &textPtr );
				if (!token[0]) {
					break;
				}				

				if ( !Q_stricmpn( token, "consoleName", 11 ) )
				{
					if ( COM_ParseString( &textPtr, &token ) ) 
					{
						G_Printf( S_COLOR_RED "ERROR: Invalid class console name in class index: %i.\n", classIndex );
						SkipBracedSection( &textPtr );
						continue;
					}

					Q_strncpyz( g_classData[classIndex].consoleName, token, sizeof( g_classData[classIndex].consoleName ) );
					classValid = qtrue;
					
					//G_Printf( S_COLOR_RED "%s\n", g_classData[classIndex].consoleName );

					continue;
				}

				if ( !Q_stricmpn( token, "formalName", 11 ) )
				{
					if ( COM_ParseString( &textPtr, &token ) ) 
					{
						G_Printf( S_COLOR_RED "ERROR: Invalid class formal name in class index: %i.\n", classIndex );
						SkipBracedSection( &textPtr );
						continue;
					}

					Q_strncpyz( g_classData[classIndex].formalName, token, sizeof( g_classData[classIndex].formalName ) );
					classValid = qtrue;
					
					//G_Printf( S_COLOR_RED "%s\n", g_classData[classIndex].consoleName );

					continue;
				}

				if ( !Q_stricmpn( token, "message", 7 ) ) 
				{
					if ( COM_ParseString( &textPtr, &token ) ) 
					{
						G_Printf( S_COLOR_RED "ERROR: Invalid class message in class index: %i.\n", classIndex );
						continue;
					}

					Q_strncpyz( g_classData[classIndex].message, token, sizeof( g_classData[classIndex].message ) );
					continue;					
				}

				if ( !Q_stricmpn( token, "modelSkin", 9 ) ) 
				{
					if ( COM_ParseString( &textPtr, &token ) ) 
					{
						G_Printf( S_COLOR_RED "ERROR: Invalid class skin color in class index: %i.\n", classIndex );
						continue;
					}

					Q_strncpyz( g_classData[classIndex].modelSkin, token, sizeof( g_classData[classIndex].modelSkin ) );
					continue;
				}

				if ( !Q_stricmpn( token, "weapons", 7) ) 
				{
					token = COM_Parse( &textPtr );

					if ( Q_stricmpn( token, "{", 1 ) ) 
					{
						G_Printf( S_COLOR_RED "No opening bracket found for weapons field in class: %i.\n", classIndex );
						SkipRestOfLine( &textPtr );
						continue;
					}

					//sub loop
					while ( 1 ) 
					{
						token = COM_Parse( &textPtr );

						if ( !token[0] )
							break;
						
						if ( !Q_stricmpn( token, "|", 1 ) )
							continue;

						if ( !Q_stricmpn( token, "}", 1 ) )
							break;

						if( !Q_stricmpn( token, "WP_", 3 ) ) 
						{
							weapon = GetIDForString( WeaponTable, token );

							if ( weapon >= 0 ) 
							{
								g_classData[classIndex].weaponsFlags |= ( 1 << weapon );
								continue;
							}
						}
					}

					continue;
				}

				if ( !Q_stricmpn( token, "admin", 5 ) ) 
				{
					if ( COM_ParseInt( &textPtr, &g_classData[classIndex].isAdmin ) ) 
					{
						G_Printf( S_COLOR_RED "ERROR: Class admin check for class %i was invalid.\n", classIndex );
						continue;
					}

					continue;
				}

				if ( !Q_stricmpn( token, "marine", 6 ) ) 
				{
					if ( COM_ParseInt( &textPtr, &g_classData[classIndex].isMarine ) ) 
					{
						G_Printf( S_COLOR_RED "ERROR: Class marine check for class %i was invalid.\n", classIndex );
						continue;
					}

					continue;
				}

				if ( !Q_stricmpn( token, "medical", 7 ) ) 
				{
					if ( COM_ParseInt( &textPtr, &g_classData[classIndex].isMedical ) ) 
					{
						G_Printf( S_COLOR_RED "ERROR: Class medic check for class %i was invalid.\n", classIndex );
						continue;
					}

					continue;
				}

				if( !Q_stricmpn( token, "isBorg", 6 ) )
				{
					if( COM_ParseInt( &textPtr, &g_classData[classIndex].isBorg ) )
					{
						G_Printf( S_COLOR_RED "ERROR: Class borg check for class %i was invalid.\n", classIndex );
						continue;
					}
					continue;
				}

				if ( !Q_stricmpn( token, "n00b", 4 ) ) 
				{
					if ( COM_ParseInt( &textPtr, &g_classData[classIndex].isn00b ) ) 
					{
						G_Printf( S_COLOR_RED "ERROR: Class n00b check for class %i was invalid.\n", classIndex );
						continue;
					}

					continue;
				}

				//skip the client-side specific entries since they interfere with the parsing
				if ( !Q_stricmpn( token, "radarColor", 10 ) 
					|| !Q_stricmpn( token, "iconColor", 9 ) 
					|| !Q_stricmpn( token, "hasRanks", 8 ) 
					|| !Q_stricmpn( token, "noShow", 6 )
					)
				{
					SkipRestOfLine( &textPtr );
					continue;
				}

				if ( !Q_stricmpn( token, "}", 1 ) ) 
				{
					break;
				}
			}

			
			if ( classValid ) 
			{
				classIndex++;
				classValid = qfalse;
			}
		}
		
		token = COM_Parse( &textPtr );	
		if (!token[0]) 
		{
			break;
		}
	}

	//build ourselves custom CVARs for each class
	for ( i=0; g_classData[i].consoleName[0] && i < MAX_CLASSES; i++ ) 
	{
		trap_Cvar_Register( NULL, va("rpg_%sPass", g_classData[i].consoleName ), g_classData[i].consoleName, CVAR_ARCHIVE );
		trap_Cvar_Register( NULL, va("rpg_%sFlags", g_classData[i].consoleName ), va("%i", g_classData[i].weaponsFlags), CVAR_ARCHIVE );
	}

	if ( classIndex > 0 ) 
	{
		return qtrue;
	}
	else 
	{
		G_Printf( S_COLOR_RED "ERROR: No valid classes were found.\n");
		return qfalse;
	}
}
#else
static qboolean G_LoadClassData( char* fileName )
{
	char			*buffer;
	char			*textPtr, *token;
	int				fileLen;
	fileHandle_t	f;
	qboolean		classValid=qfalse;
	int				classIndex=0;
	int				weapon;
	int				i;

	//Init the storage place
	memset( &g_classData, 0, sizeof ( g_classData ) );
	
	fileLen = trap_FS_FOpenFile( fileName, &f, FS_READ );

	if ( !f ) {
		G_Printf( S_COLOR_RED "ERROR: File %s not found.\n", fileName );
		return qfalse;
	}

	if ( fileLen >= 32000 ) {
		G_Printf( S_COLOR_RED "ERROR: File %s was way too big.\n", fileName );
		trap_FS_FCloseFile( f );
		return qfalse;
	}

	buffer = (char *)malloc(32000 * sizeof(char));

	if(!buffer) {
		G_Printf( S_COLOR_RED "ERROR: Was unable to allocate %i bytes.\n", 32000 * sizeof(char) );
		trap_FS_FCloseFile( f );
		return qfalse;
	}

	trap_FS_Read( buffer, fileLen, f );
	buffer[fileLen] = 0;
	trap_FS_FCloseFile( f );

	COM_BeginParseSession();

	textPtr = buffer;

	token = COM_Parse( &textPtr );

	if ( !token[0] ) {
		G_Printf( S_COLOR_RED "ERROR: No data was found when going to parse the file!\n" );
		free(buffer);
		return qfalse;
	}

	if ( Q_stricmpn( token, "{", 1 ) ) {
		G_Printf( S_COLOR_RED "ERROR: File did not start with a '{' symbol!\n" );
		free(buffer);
		return qfalse;
	}

	while ( 1 ) 
	{
		if ( classIndex >= MAX_CLASSES )
			break;

		if ( !Q_stricmpn( token, "{", 1 ) ) 
		{
			while ( 1 ) 
			{
				token = COM_Parse( &textPtr );
				if (!token[0]) {
					break;
				}				

				if ( !Q_stricmpn( token, "consoleName", 11 ) )
				{
					if ( COM_ParseString( &textPtr, &token ) ) 
					{
						G_Printf( S_COLOR_RED "ERROR: Invalid class console name in class index: %i.\n", classIndex );
						SkipBracedSection( &textPtr );
						continue;
					}

					Q_strncpyz( g_classData[classIndex].consoleName, token, sizeof( g_classData[classIndex].consoleName ) );
					classValid = qtrue;
					
					//G_Printf( S_COLOR_RED "%s\n", g_classData[classIndex].consoleName );

					continue;
				}

				if ( !Q_stricmpn( token, "formalName", 11 ) )
				{
					if ( COM_ParseString( &textPtr, &token ) ) 
					{
						G_Printf( S_COLOR_RED "ERROR: Invalid class formal name in class index: %i.\n", classIndex );
						SkipBracedSection( &textPtr );
						continue;
					}

					Q_strncpyz( g_classData[classIndex].formalName, token, sizeof( g_classData[classIndex].formalName ) );
					classValid = qtrue;
					
					//G_Printf( S_COLOR_RED "%s\n", g_classData[classIndex].consoleName );

					continue;
				}

				if ( !Q_stricmpn( token, "message", 7 ) ) 
				{
					if ( COM_ParseString( &textPtr, &token ) ) 
					{
						G_Printf( S_COLOR_RED "ERROR: Invalid class message in class index: %i.\n", classIndex );
						continue;
					}

					Q_strncpyz( g_classData[classIndex].message, token, sizeof( g_classData[classIndex].message ) );
					continue;					
				}

				if ( !Q_stricmpn( token, "modelSkin", 9 ) ) 
				{
					if ( COM_ParseString( &textPtr, &token ) ) 
					{
						G_Printf( S_COLOR_RED "ERROR: Invalid class skin color in class index: %i.\n", classIndex );
						continue;
					}

					Q_strncpyz( g_classData[classIndex].modelSkin, token, sizeof( g_classData[classIndex].modelSkin ) );
					continue;
				}

				if ( !Q_stricmpn( token, "weapons", 7) ) 
				{
					token = COM_Parse( &textPtr );

					if ( Q_stricmpn( token, "{", 1 ) ) 
					{
						G_Printf( S_COLOR_RED "No opening bracket found for weapons field in class: %i.\n", classIndex );
						SkipRestOfLine( &textPtr );
						continue;
					}

					//sub loop
					while ( 1 ) 
					{
						token = COM_Parse( &textPtr );

						if ( !token[0] )
							break;
						
						if ( !Q_stricmpn( token, "|", 1 ) )
							continue;

						if ( !Q_stricmpn( token, "}", 1 ) )
							break;

						if( !Q_stricmpn( token, "WP_", 3 ) ) 
						{
							weapon = GetIDForString( WeaponTable, token );

							if ( weapon >= 0 ) 
							{
								g_classData[classIndex].weaponsFlags |= ( 1 << weapon );
								continue;
							}
						}
					}

					continue;
				}

				if ( !Q_stricmpn( token, "admin", 5 ) ) 
				{
					if ( COM_ParseInt( &textPtr, &g_classData[classIndex].isAdmin ) ) 
					{
						G_Printf( S_COLOR_RED "ERROR: Class admin check for class %i was invalid.\n", classIndex );
						continue;
					}

					continue;
				}

				if ( !Q_stricmpn( token, "marine", 6 ) ) 
				{
					if ( COM_ParseInt( &textPtr, &g_classData[classIndex].isMarine ) ) 
					{
						G_Printf( S_COLOR_RED "ERROR: Class marine check for class %i was invalid.\n", classIndex );
						continue;
					}

					continue;
				}

				if ( !Q_stricmpn( token, "medical", 7 ) ) 
				{
					if ( COM_ParseInt( &textPtr, &g_classData[classIndex].isMedical ) ) 
					{
						G_Printf( S_COLOR_RED "ERROR: Class medic check for class %i was invalid.\n", classIndex );
						continue;
					}

					continue;
				}

				if( !Q_stricmpn( token, "isBorg", 6 ) )
				{
					if( COM_ParseInt( &textPtr, &g_classData[classIndex].isBorg ) )
					{
						G_Printf( S_COLOR_RED "ERROR: Class borg check for class %i was invalid.\n", classIndex );
						continue;
					}
					continue;
				}

				if ( !Q_stricmpn( token, "n00b", 4 ) ) 
				{
					if ( COM_ParseInt( &textPtr, &g_classData[classIndex].isn00b ) ) 
					{
						G_Printf( S_COLOR_RED "ERROR: Class n00b check for class %i was invalid.\n", classIndex );
						continue;
					}

					continue;
				}

				//skip the client-side specific entries since they interfere with the parsing
				if ( !Q_stricmpn( token, "radarColor", 10 ) 
					|| !Q_stricmpn( token, "iconColor", 9 ) 
					|| !Q_stricmpn( token, "hasRanks", 8 ) 
					|| !Q_stricmpn( token, "noShow", 6 )
					)
				{
					SkipRestOfLine( &textPtr );
					continue;
				}

				if ( !Q_stricmpn( token, "}", 1 ) ) 
				{
					break;
				}
			}

			
			if ( classValid ) 
			{
				classIndex++;
				classValid = qfalse;
			}
		}
		
		token = COM_Parse( &textPtr );	
		if (!token[0]) 
		{
			break;
		}
	}

	free(buffer);

	//build ourselves custom CVARs for each class
	for ( i=0; g_classData[i].consoleName[0] && i < MAX_CLASSES; i++ ) 
	{
		trap_Cvar_Register( NULL, va("rpg_%sPass", g_classData[i].consoleName ), g_classData[i].consoleName, CVAR_ARCHIVE );
		trap_Cvar_Register( NULL, va("rpg_%sFlags", g_classData[i].consoleName ), va("%i", g_classData[i].weaponsFlags), CVAR_ARCHIVE );
	}

	if ( classIndex > 0 ) 
	{
		return qtrue;
	}
	else 
	{
		G_Printf( S_COLOR_RED "ERROR: No valid classes were found.\n");
		return qfalse;
	}
}
#endif

void BG_LanguageFilename(char *baseName,char *baseExtension,char *finalName);
void SP_target_location (gentity_t *ent);

holoData_t holoData;

#ifdef Q3_VM
void G_LoadHolodeckFile(void) {
	char			fileRoute[MAX_QPATH];
	char			mapRoute[MAX_QPATH];
	char			info[MAX_INFO_STRING];
	fileHandle_t	f;
	char			buffer[20000];
	int				file_len;
	char			*txtPtr, *token;
	int				numProgs = 0;
	int				i;

	//get the map name out of the server data
	trap_GetServerinfo( info, sizeof( info ) );

	//setup the file route
	Com_sprintf( mapRoute, sizeof( mapRoute ), "maps/%s", Info_ValueForKey( info, "mapname" ) );

	BG_LanguageFilename( mapRoute, "holodeck", fileRoute );

	file_len = trap_FS_FOpenFile( fileRoute, &f, FS_READ );

	if ( !file_len )
		return;

	memset( buffer, 0, sizeof(buffer) );

	trap_FS_Read( buffer, file_len, f );
	if ( !buffer[0] )
	{
		G_Printf( S_COLOR_RED "ERROR: Couldn't read in file: %s!\n", fileRoute );
		trap_FS_FCloseFile( f );
		return;
	}

	buffer[file_len] = '\0';
	trap_FS_FCloseFile( f );

	COM_BeginParseSession();
	txtPtr = buffer;

	while(1) {
		token = COM_Parse(&txtPtr);
		if(!token[0]) break;

		if(!Q_stricmpn(token, "HolodeckData", 12)) {
			token = COM_Parse(&txtPtr);
			if(Q_stricmpn(token, "{", 1)) {
				G_Printf( S_COLOR_RED "ERROR: HolodeckData had no opening brace ( { )!\n");
				continue;
			}
			while(Q_stricmpn(token, "}", 1)) {
				token = COM_Parse(&txtPtr);
				if(!token[0]) break;

				if(!Q_stricmpn(token, "Program", 7)) {
					token = COM_Parse(&txtPtr);
					if(Q_stricmpn(token, "[", 1)) {
						G_Printf( S_COLOR_RED "ERROR: Program had no opening brace ( [ )!\n");
						continue;
					}

					// expected format:
					// <string> - target notnull
					// <string> - name
					// <string> - desc1
					// <string> - desc2
					// <string> - image
					// <string> - iTrigger
					// <string> - dTrigger
					while(Q_stricmpn(token, "]", 1)) {
						if(!token[0]) break;

						if(numProgs >= 5) return;

						// targetname of info_notnull
						token = COM_Parse(&txtPtr);
						Q_strncpyz(holoData.target[numProgs], token, sizeof(holoData.target[numProgs]));

						// parse name
						token = COM_Parse(&txtPtr);
						Q_strncpyz(holoData.name[numProgs], token, sizeof(holoData.name[numProgs]));

						// parse desc1
						token = COM_Parse(&txtPtr);
						Q_strncpyz(holoData.desc1[numProgs], token, sizeof(holoData.desc1[numProgs]));

						// parse desc2
						token = COM_Parse(&txtPtr);
						Q_strncpyz(holoData.desc2[numProgs], token, sizeof(holoData.desc2[numProgs]));

						// parse image
						token = COM_Parse(&txtPtr);
						Q_strncpyz(holoData.image[numProgs], token, sizeof(holoData.image[numProgs]));

						// parse iTrigger
						token = COM_Parse(&txtPtr);
						Q_strncpyz(holoData.iTrigger[numProgs+1], token, sizeof(holoData.iTrigger[numProgs+1]));

						// parse dTrigger
						token = COM_Parse(&txtPtr);
						Q_strncpyz(holoData.dTrigger[numProgs+1], token, sizeof(holoData.dTrigger[numProgs+1]));

						holoData.active = -1;

						numProgs++;

						token = COM_Parse(&txtPtr);
					}
				}
			}
		}
	}

	for(i = 0; i < MAX_GENTITIES; i++)
		if(!strcmp("target_holodeck", &g_entities[i]->classname)) {
			strcpy(holoData.iTrigger[0], &g_entities[i]->target);
			strcpy(holoData.dTrigger[0], &g_entities[i]->redsound);
			break;
		}

	holoData.numProgs = numProgs;
}
#else
void G_LoadHolodeckFile(void) {
	char			fileRoute[MAX_QPATH];
	char			mapRoute[MAX_QPATH];
	char			*info;
	fileHandle_t	f;
	char			*buffer;
	int				file_len;
	char			*txtPtr, *token;
	int				numProgs = 0;
	//int				i;

	info = (char *)malloc(MAX_INFO_STRING * sizeof(char));
	if(!info) {
		G_Printf(S_COLOR_RED "ERROR: Was unable to allocate %i byte.\n", MAX_INFO_STRING * sizeof(char) );
		return;
	}

	//get the map name out of the server data
	trap_GetServerinfo( info, MAX_INFO_STRING * sizeof(char) );

	//setup the file route
	Com_sprintf( mapRoute, sizeof( mapRoute ), "maps/%s", Info_ValueForKey( info, "mapname" ) );

	BG_LanguageFilename( mapRoute, "holodeck", fileRoute );

	file_len = trap_FS_FOpenFile( fileRoute, &f, FS_READ );

	free(info);

	if ( !file_len ) return;

	buffer = (char *)malloc(32000 * sizeof(char));
	if(!buffer) {
		G_Printf(S_COLOR_RED "ERROR: Was unable to allocate %i bytes.\n", 32000 * sizeof(char) );
		trap_FS_FCloseFile(f);
		return;
	}

	trap_FS_Read( buffer, file_len, f );
	if ( !buffer[0] )
	{
		G_Printf( S_COLOR_RED "ERROR: Couldn't read in file: %s!\n", fileRoute );
		trap_FS_FCloseFile( f );
		free(buffer);
		return;
	}

	buffer[file_len] = '\0';
	trap_FS_FCloseFile( f );

	memset(&holoData, 0, sizeof(holoData));

	COM_BeginParseSession();
	txtPtr = buffer;

	while(1) {
		token = COM_Parse(&txtPtr);
		if(!token[0]) break;

		if(!Q_stricmpn(token, "HolodeckData", 12)) {
			token = COM_Parse(&txtPtr);
			if(Q_stricmpn(token, "{", 1)) {
				G_Printf( S_COLOR_RED "ERROR: HolodeckData had no opening brace ( { )!\n");
				continue;
			}
			while(Q_stricmpn(token, "}", 1)) {
				token = COM_Parse(&txtPtr);
				if(!token[0]) break;

				if(!Q_stricmpn(token, "Program", 7)) {
					token = COM_Parse(&txtPtr);
					if(Q_stricmpn(token, "[", 1)) {
						G_Printf( S_COLOR_RED "ERROR: Program had no opening brace ( [ )!\n");
						continue;
					}

					// expected format:
					// <string> - target notnull
					// <string> - name
					// <string> - desc1
					// <string> - desc2
					// <string> - image
					while(Q_stricmpn(token, "]", 1)) {
						if(!token[0]) break;

						if(numProgs >= 5) {
							free(buffer);
							return;
						}

						// targetname of info_notnull
						token = COM_Parse(&txtPtr);
						Q_strncpyz(holoData.target[numProgs], token, sizeof(holoData.target[numProgs]));

						// parse name
						token = COM_Parse(&txtPtr);
						Q_strncpyz(holoData.name[numProgs], token, sizeof(holoData.name[numProgs]));

						// parse desc1
						token = COM_Parse(&txtPtr);
						Q_strncpyz(holoData.desc1[numProgs], token, sizeof(holoData.desc1[numProgs]));

						// parse desc2
						token = COM_Parse(&txtPtr);
						Q_strncpyz(holoData.desc2[numProgs], token, sizeof(holoData.desc2[numProgs]));

						// parse image
						token = COM_Parse(&txtPtr);
						Q_strncpyz(holoData.image[numProgs], token, sizeof(holoData.image[numProgs]));

						holoData.active = -1;

						numProgs++;

						token = COM_Parse(&txtPtr);
					}
				}
			}
		}
	}

	holoData.numProgs = numProgs;

	free(buffer);
}
#endif

srvChangeData_t srvChangeData;

#ifdef Q3_VM
static void G_LoadServerChangeFile(void) {
	char			fileRoute[MAX_QPATH];
	//char			mapRoute[MAX_QPATH];
	char			infoString[MAX_INFO_STRING];
	fileHandle_t	f;
	char			buffer[20000];
	int				file_len;
	char			*txtPtr, *token;
	char			*temp;
	int				cnt = 0;
	int				i = 0;

	BG_LanguageFilename("serverchange", "cfg", fileRoute);

	file_len = trap_FS_FOpenFile(fileRoute, &f, FS_READ);

	if(!file_len) 
		return;

	memset(buffer, 0, sizeof(buffer));
	memset(infoString, 0, sizeof(infoString));

	trap_FS_Read(buffer, file_len, f);
	if ( !buffer[0] )
	{
		G_Printf( S_COLOR_RED "ERROR: Couldn't read in file: %s!\n", fileRoute );
		trap_FS_FCloseFile( f );
		return;
	}

	buffer[file_len] = '\0';
	trap_FS_FCloseFile(f);

	COM_BeginParseSession();
	txtPtr = buffer;

	while(1) {
		token = COM_Parse(&txtPtr);
		if(!token[0]) break;

		if(!Q_stricmp(token, "ServerChangeConfig")) {
			token = COM_Parse( &txtPtr );
			if ( Q_strncmp( token, "{", 1 ) != 0 )
			{
				G_Printf( S_COLOR_RED "ERROR: ServerChangeConfig had no opening brace ( { )!\n" );
				continue;
			}
			
			while(Q_strncmp(token, "}", 1)) {
				token = COM_Parse(&txtPtr);
				if(!token[0]) break;

				if(!Q_stricmp(token, "Server")) {
					token = COM_Parse(&txtPtr);
					if ( Q_strncmp( token, "[", 1 ) != 0 )
					{
						G_Printf( S_COLOR_RED "ERROR: Server had no opening brace ( [ )!\n" );
						continue;
					}

					token = COM_Parse(&txtPtr);
					while(Q_strncmp(token, "]", 1)) {
						if(!token[0]) break;

						if(cnt > 12) break;

						temp = G_NewString(token);

						/*if(!infoString[0])
							Com_sprintf(infoString, sizeof(infoString), "i%i\\%s\\", cnt, temp);
						else {
							if(cnt % 2 == 0)
								Com_sprintf(infoString, sizeof(infoString), "%si%i\\%s\\", infoString, i, temp);
							else
								Com_sprintf(infoString, sizeof(infoString), "%sd%i\\%s\\", infoString, i, temp);
						}*/

						if(cnt % 2 == 0)
							Q_strncpyz(srvChangeData.ip[i], token, sizeof(srvChangeData.ip[i]));
						else
							Q_strncpyz(srvChangeData.name[i], token, sizeof(srvChangeData.name[i]));

						cnt++;
						if(cnt % 2 == 0)
							i++;

						token = COM_Parse(&txtPtr);
					}
				}
			}
		}
	}

	//trap_SetConfigstring(CS_SERVERCHANGE, infoString);
}
#else
static void G_LoadServerChangeFile(void) {
	char			fileRoute[MAX_QPATH];
	//char			mapRoute[MAX_QPATH];
	fileHandle_t	f;
	char			*buffer;
	int				file_len;
	char			*txtPtr, *token;
	char			*temp;
	int				cnt = 0;
	int				i = 0;

	BG_LanguageFilename("serverchange", "cfg", fileRoute);

	file_len = trap_FS_FOpenFile(fileRoute, &f, FS_READ);

	if(!file_len) 
		return;

	buffer = (char *)malloc(32000 * sizeof(char));
	if(!buffer) {
		G_Printf(S_COLOR_RED "ERROR: Was unable to allocate %i bytes.\n", 32000 * sizeof(char) );
		trap_FS_FCloseFile(f);
		return;
	}

	trap_FS_Read(buffer, file_len, f);
	if ( !buffer[0] )
	{
		G_Printf( S_COLOR_RED "ERROR: Couldn't read in file: %s!\n", fileRoute );
		trap_FS_FCloseFile( f );
		free(buffer);
		return;
	}

	buffer[file_len] = '\0';
	trap_FS_FCloseFile(f);

	memset(&srvChangeData, 0, sizeof(srvChangeData));

	COM_BeginParseSession();
	txtPtr = buffer;

	while(1) {
		token = COM_Parse(&txtPtr);
		if(!token[0]) break;

		if(!Q_stricmp(token, "ServerChangeConfig")) {
			token = COM_Parse( &txtPtr );
			if ( Q_strncmp( token, "{", 1 ) != 0 )
			{
				G_Printf( S_COLOR_RED "ERROR: ServerChangeConfig had no opening brace ( { )!\n" );
				continue;
			}
			
			while(Q_strncmp(token, "}", 1)) {
				token = COM_Parse(&txtPtr);
				if(!token[0]) break;

				if(!Q_stricmp(token, "Server")) {
					token = COM_Parse(&txtPtr);
					if ( Q_strncmp( token, "[", 1 ) != 0 )
					{
						G_Printf( S_COLOR_RED "ERROR: Server had no opening brace ( [ )!\n" );
						continue;
					}

					token = COM_Parse(&txtPtr);
					while(Q_strncmp(token, "]", 1)) {
						if(!token[0]) break;

						if(cnt > 12) break;

						temp = G_NewString(token);

						/*if(!infoString[0])
							Com_sprintf(infoString, sizeof(infoString), "i%i\\%s\\", cnt, temp);
						else {
							if(cnt % 2 == 0)
								Com_sprintf(infoString, sizeof(infoString), "%si%i\\%s\\", infoString, i, temp);
							else
								Com_sprintf(infoString, sizeof(infoString), "%sd%i\\%s\\", infoString, i, temp);
						}*/

						if(cnt % 2 == 0)
							Q_strncpyz(srvChangeData.ip[i], token, sizeof(srvChangeData.ip[i]));
						else
							Q_strncpyz(srvChangeData.name[i], token, sizeof(srvChangeData.name[i]));

						cnt++;
						if(cnt % 2 == 0)
							i++;

						token = COM_Parse(&txtPtr);
					}
				}
			}
		}
	}

	free(buffer);
	//trap_SetConfigstring(CS_SERVERCHANGE, infoString);
}
#endif

mapChangeData_t mapChangeData;

#ifdef Q3_VM
static void G_LoadMapChangeFile(void) {
	char			fileRoute[MAX_QPATH];
	//char			mapRoute[MAX_QPATH];
	char			infoString[MAX_INFO_STRING];
	fileHandle_t	f;
	char			buffer[20000];
	int				file_len;
	char			*txtPtr, *token;
	char			*temp;
	int				cnt = 0;
	int				i = 0;

	BG_LanguageFilename("mapchange", "cfg", fileRoute);

	file_len = trap_FS_FOpenFile(fileRoute, &f, FS_READ);

	if(!file_len) 
		return;

	memset(buffer, 0, sizeof(buffer));
	memset(infoString, 0, sizeof(infoString));

	trap_FS_Read(buffer, file_len, f);
	if ( !buffer[0] )
	{
		G_Printf( S_COLOR_RED "ERROR: Couldn't read in file: %s!\n", fileRoute );
		trap_FS_FCloseFile( f );
		return;
	}

	buffer[file_len] = '\0';
	trap_FS_FCloseFile(f);

	COM_BeginParseSession();
	txtPtr = buffer;

	while(1) {
		token = COM_Parse(&txtPtr);
		if(!token[0]) break;

		if(!Q_stricmp(token, "MapChangeConfig")) {
			token = COM_Parse( &txtPtr );
			if ( Q_strncmp( token, "{", 1 ) != 0 )
			{
				G_Printf( S_COLOR_RED "ERROR: MapChangeConfig had no opening brace ( { )!\n" );
				continue;
			}
			
			while(Q_strncmp(token, "}", 1)) {
				token = COM_Parse(&txtPtr);
				if(!token[0]) break;

				if(!Q_stricmp(token, "Map")) {
					token = COM_Parse(&txtPtr);
					if ( Q_strncmp( token, "[", 1 ) != 0 )
					{
						G_Printf( S_COLOR_RED "ERROR: Server had no opening brace ( [ )!\n" );
						continue;
					}

					token = COM_Parse(&txtPtr);
					while(Q_strncmp(token, "]", 1)) {
						if(!token[0]) break;

						if(cnt > 12) break;

						temp = G_NewString(token);

						/*if(!infoString[0])
							Com_sprintf(infoString, sizeof(infoString), "i%i\\%s\\", cnt, temp);
						else {
							if(cnt % 2 == 0)
								Com_sprintf(infoString, sizeof(infoString), "%si%i\\%s\\", infoString, i, temp);
							else
								Com_sprintf(infoString, sizeof(infoString), "%sd%i\\%s\\", infoString, i, temp);
						}*/

						if(cnt % 2 == 0)
							Q_strncpyz(mapChangeData.name[i], token, sizeof(mapChangeData.name[i]));
						else
							Q_strncpyz(mapChangeData.bspname[i], token, sizeof(mapChangeData.bspname[i]));

						cnt++;
						if(cnt % 2 == 0)
							i++;

						token = COM_Parse(&txtPtr);
					}
				}
			}
		}
	}

	//trap_SetConfigstring(CS_SERVERCHANGE, infoString);
}
#else
static void G_LoadMapChangeFile(void) {
	char			fileRoute[MAX_QPATH];
	//char			mapRoute[MAX_QPATH];
	fileHandle_t	f;
	char			*buffer;
	int				file_len;
	char			*txtPtr, *token;
	char			*temp;
	int				cnt = 0;
	int				i = 0;

	BG_LanguageFilename("mapchange", "cfg", fileRoute);

	file_len = trap_FS_FOpenFile(fileRoute, &f, FS_READ);

	if(!file_len) 
		return;

	buffer = (char *)malloc(32000 * sizeof(char));
	if(!buffer) {
		G_Printf(S_COLOR_RED "ERROR: Was unable to allocate %i bytes.\n", 32000 * sizeof(char) );
		trap_FS_FCloseFile(f);
		return;
	}

	trap_FS_Read(buffer, file_len, f);
	if ( !buffer[0] )
	{
		G_Printf( S_COLOR_RED "ERROR: Couldn't read in file: %s!\n", fileRoute );
		trap_FS_FCloseFile( f );
		free(buffer);
		return;
	}

	buffer[file_len] = '\0';
	trap_FS_FCloseFile(f);

	memset(&srvChangeData, 0, sizeof(srvChangeData));

	COM_BeginParseSession();
	txtPtr = buffer;

	while(1) {
		token = COM_Parse(&txtPtr);
		if(!token[0]) break;

		if(!Q_stricmp(token, "MapChangeConfig")) {
			token = COM_Parse( &txtPtr );
			if ( Q_strncmp( token, "{", 1 ) != 0 )
			{
				G_Printf( S_COLOR_RED "ERROR: MapChangeConfig had no opening brace ( { )!\n" );
				continue;
			}
			
			while(Q_strncmp(token, "}", 1)) {
				token = COM_Parse(&txtPtr);
				if(!token[0]) break;

				if(!Q_stricmp(token, "Map")) {
					token = COM_Parse(&txtPtr);
					if ( Q_strncmp( token, "[", 1 ) != 0 )
					{
						G_Printf( S_COLOR_RED "ERROR: Server had no opening brace ( [ )!\n" );
						continue;
					}

					token = COM_Parse(&txtPtr);
					while(Q_strncmp(token, "]", 1)) {
						if(!token[0]) break;

						if(cnt > 12) break;

						temp = G_NewString(token);

						/*if(!infoString[0])
							Com_sprintf(infoString, sizeof(infoString), "i%i\\%s\\", cnt, temp);
						else {
							if(cnt % 2 == 0)
								Com_sprintf(infoString, sizeof(infoString), "%si%i\\%s\\", infoString, i, temp);
							else
								Com_sprintf(infoString, sizeof(infoString), "%sd%i\\%s\\", infoString, i, temp);
						}*/

						if(cnt % 2 == 0)
							Q_strncpyz(mapChangeData.name[i], token, sizeof(mapChangeData.name[i]));
						else
							Q_strncpyz(mapChangeData.bspname[i], token, sizeof(mapChangeData.bspname[i]));

						cnt++;
						if(cnt % 2 == 0)
							i++;

						token = COM_Parse(&txtPtr);
					}
				}
			}
		}
	}

	free(buffer);
	//trap_SetConfigstring(CS_SERVERCHANGE, infoString);
}
#endif

#ifdef Q3_VM
static void G_LoadLocationsFile( void )
{
	char			fileRoute[MAX_QPATH];
	char			mapRoute[MAX_QPATH];
	char			serverInfo[MAX_TOKEN_CHARS];
	fileHandle_t	f;
	char			buffer[20000];
	int				file_len;
	char			*textPtr, *token;
	vec3_t			origin, angles;
	gentity_t		*ent;
	char			*desc;
	int				rest;

	//get the map name out of the server data
	trap_GetServerinfo( serverInfo, sizeof( serverInfo ) );

	//setup the file route
	Com_sprintf( mapRoute, sizeof( mapRoute ), "maps/%s", Info_ValueForKey( serverInfo, "mapname" ) );

	BG_LanguageFilename( mapRoute, "locations", fileRoute );

	file_len = trap_FS_FOpenFile( fileRoute, &f, FS_READ );

	if ( !file_len )
		return;

	memset( buffer, 0, sizeof(buffer) );

	trap_FS_Read( buffer, file_len, f );
	if ( !buffer[0] )
	{
		G_Printf( S_COLOR_RED "ERROR: Couldn't read in file: %s!\n", fileRoute );
		trap_FS_FCloseFile( f );
		return;
	}

	buffer[file_len] = '\0';
	trap_FS_FCloseFile( f );
	
	G_Printf( "Locations file %s located. Proceeding to load scan data.\n", fileRoute ); //GSIO01: why did this say "Usables file ..."? lol

	COM_BeginParseSession();
	textPtr = buffer;

	while( 1 )
	{
		token = COM_Parse( &textPtr );
		if ( !token[0] )
			break;

		if(!Q_strncmp( token, "LocationsList2", 19 )) { // new style locations list with restricted locations
			token = COM_Parse( &textPtr );
			if ( Q_strncmp( token, "{", 1 ) != 0 )
			{
				G_Printf( S_COLOR_RED "ERROR: LocationsList2 had no opening brace ( { )!\n", fileRoute );
				continue;
			}

			//expected format is "<origin> <angle> <int> <string>"
			while ( Q_strncmp( token, "}", 1 ) )
			{
				if ( !token[0] )
					break;

				//Parse origin
				if ( COM_ParseVec3( &textPtr, origin ) )
				{
					G_Printf( S_COLOR_RED "Invalid origin entry in %s!\n", fileRoute );
					return;
				}

				//Parse angles
				if ( COM_ParseVec3( &textPtr, angles ) )
				{
					G_Printf( S_COLOR_RED "Invalid origin entry in %s!\n", fileRoute );
					return;
				}

				//Pars restriction value
				if( COM_ParseInt( &textPtr, &rest ) )
				{
					G_Printf( S_COLOR_RED "Invalid restriction entry in %s!\n", fileRoute );
					return;
				}

				//Parse Location string
				token = COM_ParseExt( &textPtr, qfalse );
				if ( !token[0] )
				{
					G_Printf( S_COLOR_RED "Invalid string desc entry in %s!\n", fileRoute );
					return;
				}

				desc = token;

				//create a new entity
				ent = G_Spawn();
				if ( !ent )
				{
					G_Printf( S_COLOR_RED "Couldn't create entity in %s!\n", fileRoute );
					return;
				}

				ent->classname = "target_location";

				//copy position data
				VectorCopy( origin, ent->s.origin );
				VectorCopy( angles, ent->s.angles );

				//copy string
				ent->message = G_NewString( desc );

				//copy desc into target as well
				ent->target = ent->targetname = G_NewString( desc );

				// copy restriction value
				ent->sound1to2 = rest;

				//G_Printf( S_COLOR_RED "Added string %s to entity %i.\n", ent->message, (int)(ent-g_entities) );

				//initiate it as a location ent
				SP_target_location( ent );

				//reset the ent
				ent = NULL;

				//--
				token = COM_Parse( &textPtr );
			}
		} else if ( !Q_strncmp( token, "LocationsList", 18 ) ) // old stly locations file
		{
			token = COM_Parse( &textPtr );
			if ( Q_strncmp( token, "{", 1 ) != 0 )
			{
				G_Printf( S_COLOR_RED "ERROR: LocationsList had no opening brace ( { )!\n", fileRoute );
				continue;
			}

			//expected format is "<origin> <angle> <string>"
			while ( Q_strncmp( token, "}", 1 ) )
			{
				if ( !token[0] )
					break;

				//Parse origin
				if ( COM_ParseVec3( &textPtr, origin ) )
				{
					G_Printf( S_COLOR_RED "Invalid origin entry in %s!\n", fileRoute );
					return;
				}

				//Parse angles
				if ( COM_ParseVec3( &textPtr, angles ) )
				{
					G_Printf( S_COLOR_RED "Invalid origin entry in %s!\n", fileRoute );
					return;
				}

				//Parse Location string
				token = COM_ParseExt( &textPtr, qfalse );
				if ( !token[0] )
				{
					G_Printf( S_COLOR_RED "Invalid string desc entry in %s!\n", fileRoute );
					return;
				}

				desc = token;

				//create a new entity
				ent = G_Spawn();
				if ( !ent )
				{
					G_Printf( S_COLOR_RED "Couldn't create entity in %s!\n", fileRoute );
					return;
				}

				ent->classname = "target_location";

				//copy position data
				VectorCopy( origin, ent->s.origin );
				VectorCopy( angles, ent->s.angles );

				//copy string
				ent->message = G_NewString( desc );

				//copy desc into target as well
				ent->target = ent->targetname = G_NewString( desc );

				//G_Printf( S_COLOR_RED "Added string %s to entity %i.\n", ent->message, (int)(ent-g_entities) );

				//initiate it as a location ent
				SP_target_location( ent );

				//reset the ent
				ent = NULL;

				//--
				token = COM_Parse( &textPtr );
			}
		}
	} 
}
#else
static void G_LoadLocationsFile( void )
{
	char			fileRoute[MAX_QPATH];
	char			mapRoute[MAX_QPATH];
	char			*serverInfo;
	fileHandle_t	f;
	char			*buffer;
	int				file_len;
	char			*textPtr, *token;
	vec3_t			origin, angles;
	gentity_t		*ent;
	char			*desc;
	int				rest;

	serverInfo = (char *)malloc(MAX_INFO_STRING * sizeof(char));
	if(!serverInfo) {
		G_Printf(S_COLOR_RED "ERROR: Was unable to allocate %i bytes.\n", MAX_INFO_STRING * sizeof(char));
		return;
	}

	//get the map name out of the server data
	trap_GetServerinfo( serverInfo, MAX_INFO_STRING * sizeof(char) );

	//setup the file route
	Com_sprintf( mapRoute, sizeof( mapRoute ), "maps/%s", Info_ValueForKey( serverInfo, "mapname" ) );

	BG_LanguageFilename( mapRoute, "locations", fileRoute );

	file_len = trap_FS_FOpenFile( fileRoute, &f, FS_READ );

	free(serverInfo);

	if ( !file_len )
		return;

	buffer = (char *)malloc(32000 * sizeof(char));
	if(!buffer) {
		G_Printf(S_COLOR_RED "ERROR: Was unable to allocate %i bytes.\n", 32000 * sizeof(char));
		trap_FS_FCloseFile(f);
		return;
	}

	trap_FS_Read( buffer, file_len, f );
	if ( !buffer[0] )
	{
		G_Printf( S_COLOR_RED "ERROR: Couldn't read in file: %s!\n", fileRoute );
		trap_FS_FCloseFile( f );
		free(buffer);
		return;
	}

	buffer[file_len] = '\0';
	trap_FS_FCloseFile( f );
	
	G_Printf( "Locations file %s located. Proceeding to load scan data.\n", fileRoute ); //GSIO01: why did this say "Usables file ..."? lol

	COM_BeginParseSession();
	textPtr = buffer;

	while( 1 )
	{
		token = COM_Parse( &textPtr );
		if ( !token[0] )
			break;

		if(!Q_strncmp( token, "LocationsList2", 19 )) { // new style locations list with restricted locations
			token = COM_Parse( &textPtr );
			if ( Q_strncmp( token, "{", 1 ) != 0 )
			{
				G_Printf( S_COLOR_RED "ERROR: LocationsList2 had no opening brace ( { )!\n", fileRoute );
				continue;
			}

			//expected format is "<origin> <angle> <int> <string>"
			while ( Q_strncmp( token, "}", 1 ) )
			{
				if ( !token[0] )
					break;

				//Parse origin
				if ( COM_ParseVec3( &textPtr, origin ) )
				{
					G_Printf( S_COLOR_RED "Invalid origin entry in %s!\n", fileRoute );
					free(buffer);
					return;
				}

				//Parse angles
				if ( COM_ParseVec3( &textPtr, angles ) )
				{
					G_Printf( S_COLOR_RED "Invalid origin entry in %s!\n", fileRoute );
					free(buffer);
					return;
				}

				//Pars restriction value
				if( COM_ParseInt( &textPtr, &rest ) )
				{
					G_Printf( S_COLOR_RED "Invalid restriction entry in %s!\n", fileRoute );
					free(buffer);
					return;
				}

				//Parse Location string
				token = COM_ParseExt( &textPtr, qfalse );
				if ( !token[0] )
				{
					G_Printf( S_COLOR_RED "Invalid string desc entry in %s!\n", fileRoute );
					free(buffer);
					return;
				}

				desc = token;

				//create a new entity
				ent = G_Spawn();
				if ( !ent )
				{
					G_Printf( S_COLOR_RED "Couldn't create entity in %s!\n", fileRoute );
					free(buffer);
					return;
				}

				ent->classname = "target_location";

				//copy position data
				VectorCopy( origin, ent->s.origin );
				VectorCopy( angles, ent->s.angles );

				//copy string
				ent->message = G_NewString( desc );

				//copy desc into target as well
				ent->target = ent->targetname = G_NewString( desc );

				// copy restriction value
				ent->sound1to2 = rest;

				//G_Printf( S_COLOR_RED "Added string %s to entity %i.\n", ent->message, (int)(ent-g_entities) );

				//initiate it as a location ent
				SP_target_location( ent );

				//reset the ent
				ent = NULL;

				//--
				token = COM_Parse( &textPtr );
			}
		} else if ( !Q_strncmp( token, "LocationsList", 18 ) ) // old stly locations file
		{
			token = COM_Parse( &textPtr );
			if ( Q_strncmp( token, "{", 1 ) != 0 )
			{
				G_Printf( S_COLOR_RED "ERROR: LocationsList had no opening brace ( { )!\n", fileRoute );
				continue;
			}

			//expected format is "<origin> <angle> <string>"
			while ( Q_strncmp( token, "}", 1 ) )
			{
				if ( !token[0] )
					break;

				//Parse origin
				if ( COM_ParseVec3( &textPtr, origin ) )
				{
					G_Printf( S_COLOR_RED "Invalid origin entry in %s!\n", fileRoute );
					free(buffer);
					return;
				}

				//Parse angles
				if ( COM_ParseVec3( &textPtr, angles ) )
				{
					G_Printf( S_COLOR_RED "Invalid origin entry in %s!\n", fileRoute );
					free(buffer);
					return;
				}

				//Parse Location string
				token = COM_ParseExt( &textPtr, qfalse );
				if ( !token[0] )
				{
					G_Printf( S_COLOR_RED "Invalid string desc entry in %s!\n", fileRoute );
					free(buffer);
					return;
				}

				desc = token;

				//create a new entity
				ent = G_Spawn();
				if ( !ent )
				{
					G_Printf( S_COLOR_RED "Couldn't create entity in %s!\n", fileRoute );
					free(buffer);
					return;
				}

				ent->classname = "target_location";

				//copy position data
				VectorCopy( origin, ent->s.origin );
				VectorCopy( angles, ent->s.angles );

				//copy string
				ent->message = G_NewString( desc );

				//copy desc into target as well
				ent->target = ent->targetname = G_NewString( desc );

				//G_Printf( S_COLOR_RED "Added string %s to entity %i.\n", ent->message, (int)(ent-g_entities) );

				//initiate it as a location ent
				SP_target_location( ent );

				//reset the ent
				ent = NULL;

				//--
				token = COM_Parse( &textPtr );
			}
		}
	} 

	free(buffer);
}
#endif

/*void G_initGroupsList(void)
{
	char	filename[MAX_QPATH];
	char	dirlist[4096];
	int		i;
	char*	dirptr;
	char*	race_list;
	int		numdirs;
	int		dirlen;

	memset(group_list, 0, sizeof(group_list));
	group_count = 0;

	// search through each and every skin we can find
	//BOOKMARK
	numdirs = trap_FS_GetFileList("models/players_rpgx", "/", dirlist, sizeof(dirlist) );
	dirptr  = dirlist;
	for (i=0; i<numdirs; i++,dirptr+=dirlen+1)
	{
		dirlen = strlen(dirptr);
		
		if (dirlen && dirptr[dirlen-1]=='/')
		{
			dirptr[dirlen-1]='\0';
		}

		if (!strcmp(dirptr,".") || !strcmp(dirptr,".."))
		{
			continue;
		}

		if (group_count == MAX_GROUP_MEMBERS)
		{
			G_Printf("Number of possible models larger than group array - limiting to first %d models\n", MAX_GROUP_MEMBERS);
			break;
		}
		// work out racename to 
		Com_sprintf(filename, sizeof(filename), "models/players_rpgx/%s/groups.cfg", dirptr);
		race_list = BG_RegisterRace(filename);

		Q_strncpyz( group_list[group_count].name, dirptr , sizeof(group_list[0].name) );
		Q_strncpyz( group_list[group_count++].text, race_list , sizeof(group_list[0].text) );
	}
}*/



#define MAX_GROUP_FILE_SIZE	5000
char *G_searchGroupList(const char *name)
{
	char	*text_p = NULL, *slash = NULL;
	char	text[MAX_GROUP_FILE_SIZE];
	int		i;
	char	mod_name[200];

	memset (races, 0, sizeof(races));
	memset (text, 0, sizeof(text));

	// check to see if there is a '/' in the name
	Q_strncpyz(mod_name, name, sizeof(mod_name));
	slash = strstr( mod_name, "/" );
	if ( slash != NULL )
	{//drop the slash and everything after it for the purpose of finding the model name in th group
		*slash = 0;
	}

	// find the name in the group list
	for (i=0; i<group_count; i++)
	{
		if (!Q_stricmp(mod_name, group_list[i].name))
		{
			text_p = group_list[i].text;
			break;
		}
	}

	// did we find this group in the list?
	if (i == group_count)
	{
		Com_sprintf(races, sizeof(races), "unknown");
	}
	else
	{
		Com_sprintf(races, sizeof(races), text_p);
	}
	return races;

}


/*
================
G_FindTeams

Chain together all entities with a matching team field.
Entity teams are used for item groups and multi-entity mover groups.

All but the first will have the FL_TEAMSLAVE flag set and teammaster field set
All but the last will have the teamchain field set to the next one
================
*/
void G_FindTeams( void ) {
	gentity_t	*e, *e2;
	int		i, j;
	int		c, c2;

	c = 0;
	c2 = 0;
	for ( i=1, e=g_entities+i ; i < level.num_entities ; i++,e++ ){
		if (!e->inuse)
			continue;
		if (!e->team)
			continue;
		if (e->flags & FL_TEAMSLAVE)
			continue;
		if ( e->classname && Q_stricmp( "func_door", e->classname ) != 0 )
		{//not a door
			if ( Q_stricmp( "1", e->team ) == 0 || Q_stricmp( "2", e->team ) == 0 )
			{//is trying to tell us it belongs to the TEAM_RED or TEAM_BLUE
				continue;
			}
		}
		e->teammaster = e;
		c++;
		c2++;
		for (j=i+1, e2=e+1 ; j < level.num_entities ; j++,e2++)
		{
			if (!e2->inuse)
				continue;
			if (!e2->team)
				continue;
			if (e2->flags & FL_TEAMSLAVE)
				continue;
			if (!strcmp(e->team, e2->team))
			{
				c2++;
				e2->teamchain = e->teamchain;
				e->teamchain = e2;
				e2->teammaster = e;
				e2->flags |= FL_TEAMSLAVE;

				// make sure that targets only point at the master
				if ( e2->targetname ) {
					e->targetname = e2->targetname;
					e2->targetname = NULL;
				}
			}
		}
	}

	G_Printf ("%i teams with %i entities\n", c, c2);
}

/*
=================
G_RegisterCvars
=================
*/
void G_RegisterCvars( void ) {
	int			i;
	cvarTable_t	*cv;

	for ( i = 0, cv = gameCvarTable ; i < gameCvarTableSize ; i++, cv++ )
	{

		if ( rpg_rpg.integer != 0 )
		{
			trap_Cvar_Set( "g_gametype", "0" );//RPG-X: RedTechie - Make sure we keep gametype at 0
		}

		trap_Cvar_Register( cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags );
		if ( cv->vmCvar )
		{
			cv->modificationCount = cv->vmCvar->modificationCount;
		}
	}

	// check some things

	if ( g_gametype.integer < 0 || g_gametype.integer >= GT_MAX_GAME_TYPE ) {
		G_Printf( "g_gametype %i is out of range, defaulting to 0\n", g_gametype.integer );
		trap_Cvar_Set( "g_gametype", "0" );
	}

	level.warmupModificationCount = g_warmup.modificationCount;
}

/*
=================
G_UpdateCvars
=================
*/
void G_UpdateCvars( void )
{
	int			i;
	cvarTable_t	*cv;

	for ( i = 0, cv = gameCvarTable ; i < gameCvarTableSize ; i++, cv++ )
	{
		if ( cv->vmCvar )
		{
			trap_Cvar_Update( cv->vmCvar );

			if ( cv->modificationCount != cv->vmCvar->modificationCount )
			{
				cv->modificationCount = cv->vmCvar->modificationCount;

				if ( cv->trackChange )
				{
					if ( !levelExiting )
					{//no need to do this during level changes
						//RPG-X: J2J - Don't show gravity changed messages any more (for the gravity target ents)
						if(Q_stricmp(cv->cvarName,"g_gravity") != 0)
						{
							trap_SendServerCommand( -1, va("print \"Server: %s changed to %s\n\"", 
								cv->cvarName, cv->vmCvar->string ) );
						}
					}
				}
			}
		}
	}
}

extern int altAmmoUsage[];
extern team_t	borgTeam;
extern team_t	initialBorgTeam;
void G_InitModRules( void )
{
	numKilled = 0;
}

extern qboolean G_CallSpawn(gentity_t *ent);
extern qboolean G_ParseField(const char *key, const char *value, gentity_t *ent);
/*
============
G_LoadSpawnFile
If there is a spawn file it will parse it and spawn/change things.

Expected file Layout:
SpawnFile 
	{
	Spawn
	{
		Entity
		{
			classname	<string> //needs to be there
			<key>		<value>  //unlimited number of keys and values
		}
	}
	Convert
	{
		Entity
		{
			"Bmodel"	"*<int>"
			<key>		<value>
		}
		Entity
		{
			"targetname"	<string>
			<key>			<value>
		}
		Entity
		{
			"target"		<string>
			<key>			<value>
		}
	}
	Remove
	{
		Entity
		{
			"Bmodel"	"*<int>"
		}
		Entity
		{
			"targetname"	<string>
		}
		Entity
		{
			"target"	<string>
		}
	}
}
============
*/
void G_LoadSpawnFile( void ) {
/*	char			fileRoute[MAX_QPATH];
	char			mapRoute[MAX_QPATH];
	char			serverInfo[MAX_TOKEN_CHARS];
	fileHandle_t	f;
	char			buffer[29400];
	int				file_len;
	char			*textPtr, *token;
	gentity_t		*newEnt;
	char			tempKey[MAX_TOKEN_CHARS], tempValue[MAX_TOKEN_CHARS];
	//vec3_t			tempVec;
	//int				tempInt;
	//float			tempFloat;

	//get the map name out of the server data
	trap_GetServerinfo( serverInfo, sizeof( serverInfo ) );

	//setup the file route
	Com_sprintf( mapRoute, sizeof( mapRoute ), "maps/%s.%s", Info_ValueForKey( serverInfo, "mapname" ), "spawn" );

	file_len = trap_FS_FOpenFile( fileRoute, &f, FS_READ );

	if ( !file_len )
		return;

	memset( buffer, 0, sizeof(buffer) );

	trap_FS_Read( buffer, file_len, f );
	buffer[file_len] = '0';
	trap_FS_FCloseFile(f);
	if(!buffer[0]) {
		G_Printf( S_COLOR_RED "ERROR: Attempted to load %s, but no data was inside!\n", fileRoute );
		return;
	}

	G_Printf( "Spawn file %s located. Proceeding to load scan data.\n", fileRoute );
	
	COM_BeginParseSession();
	textPtr = buffer;
	while(1) {
		token = COM_Parse(&textPtr);
		if(!token[0])
			break;

		while(1) {
			token = COM_Parse(&textPtr);

			if(!token[0]) return;

			if(!Q_stricmpn(token, "SpawnFile", 9)) {
				token = COM_Parse(&textPtr);
				if ( Q_strncmp( token, "{", 1 ) != 0 )
				{
					G_Printf( S_COLOR_RED "ERROR: SpawnFile in %s had no opening brace ( { )!\n", fileRoute );
					continue;
				}

				token = COM_Parse(&textPtr);
				while(Q_stricmpn(token, "}", 1)) {
					token = COM_Parse(&textPtr);
					if(!Q_stricmpn(token, "Spawn", 5)) {
						token = COM_Parse(&textPtr);
						if( Q_strncmp(token, "{", 1)) {
							G_Printf(S_COLOR_RED "ERROR: Spawn block in %s had no opening brace ( { )!\n", fileRoute);
							continue;
						}

						token = COM_Parse(&textPtr);
						while(Q_strncmp(token, "}", 1)) {
							token = COM_Parse(&textPtr);
							if(!Q_stricmpn(token, "Entity", 6)) {
								if(Q_strncmp(token, "{", 1)) {
									G_Printf(S_COLOR_RED, "ERROR: Entity in %s had no opening brace ( { )!\n", fileRoute);
									continue;
								}

								token = COM_Parse(&textPtr);
								if(!Q_strncmp(token, "classname", 9)) {
									G_Printf(S_COLOR_RED, "ERROR: First key in Entity in Spawn block in %s was not \"classname\"!\n", fileRoute);
									continue;
								}

								newEnt = G_Spawn();
								if(!newEnt) continue;

								newEnt->tmpEntity = qtrue;
								
								token = COM_Parse(&textPtr);
								newEnt->classname = G_NewString(token);

								// check if this entity is valid in Spawn block ...
								if(!Q_strncmp(newEnt->classname, "func_", 5) // func entities are never valid as they need brushmodels
									|| !Q_strncmp(newEnt->classname, "misc_", 5) // misc entities are all invalid for now
									) {
										G_FreeEntity(newEnt);
										G_Printf(S_COLOR_RED "ERROR: Entities of type %s are invalid in Spawn Block!\n", token);
										continue;
								}
								
								while(Q_strncmp(token, "}", 1)) {
									token = COM_Parse(&textPtr);
									Com_sprintf(tempKey, sizeof(tempKey), "%s", token);
									token = COM_Parse(&textPtr);
									Com_sprintf(tempValue, sizeof(tempValue), "%s", token);
									if(!G_ParseField(tempKey, tempValue, newEnt)) {
										//GSIO01: oh well not part of fields ehh?
										//then let's see what entities this is and check if you are valid for it
									}
								}
							}
						}
					} else if(!Q_strncmp(token, "Convert", 7)) { 
						//all entities in here are converted as long as the wanted conversion is valid
						//criterias might be: 
						//				do both have a brushmodel? 
						//				do both have a origin brush? 
						//				and so on
						//because we simply can't convert a info_notnull to a func_door xD
						token = COM_Parse(&textPtr);
						if( Q_strncmp(token, "{", 1)) {
							G_Printf(S_COLOR_RED "ERROR: Convert block in %s had no opening brace ( { )!\n", fileRoute);
							continue;
						}

						//There are 3 ways to identify an entity 100 percent accurate:
						//		by it's targetname (only if it is the only one with it)
						//		by it's target (only if it is the only one targeting it)
						//		by it's brushmodel (always 100% accurate but only works on brushentities)
						//Entity IDs can't be used as loading the map locally or on a server
						//might result in diffrent entity IDs.


					} else if(!Q_strncmp(token, "Remove", 6)) {
						token = COM_Parse(&textPtr);
						if( Q_strncmp(token, "{", 1)) {
							G_Printf(S_COLOR_RED "ERROR: Remove block in %s had no opening brace ( { )!\n", fileRoute);
							continue;
						}
					}
						

				}
			}
		}
	}*/
}

static void Dev_ShowTriggers(gentity_t *ent) {
	int i;
	gentity_t *tar;

	ent->nextthink = level.time + 9500;

	for(i = 0; i < MAX_GENTITIES; i++) {
		if((tar = &g_entities[i]) == NULL) continue;
		if(!Q_stricmpn(tar->classname, "trigger_", 8)) {
			if(!Q_stricmp(tar->classname, "trigger_always")) continue;
			if(tar->r.svFlags & SVF_NOCLIENT)
				tar->r.svFlags ^= SVF_NOCLIENT;
			trap_LinkEntity(ent);
			if(!Q_stricmpn(tar->classname, "trigger_push", 13))
				G_AddEvent(tar, EV_TRIGGER_SHOW, 1);
			else
				G_AddEvent(tar, EV_TRIGGER_SHOW, 0);
		}
	}
}


/*
============
G_InitGame
============
*/
extern int lastKillTime[];
extern int LastFKRadius[];					//(RPG-X J2J) added so array can be initialised to 0 first.
extern RPGX_SiteTOSiteData TransDat[];		//(RPG-X J2J) added for tricorder transporter
extern RPGX_DragData DragDat[];
void G_InitGame( int levelTime, int randomSeed, int restart ) {
	int					i;//,j;
	//vec3_t				Zero = {0.0,0.0,0.0};			//RPG-X J2J //TiM - use vec3_origin instead
	gentity_t*			SpawnPnt;						//RPG-X J2J
	char				fileName[MAX_QPATH];
	float				messageTime;

	G_Printf ("------- Game Initialization -------\n");
	G_Printf ("gamename: %s\n", GAMEVERSION);
	G_Printf ("gamedate: %s\n", __DATE__);

	init_tonextint(qtrue);
	srand( randomSeed );

	numKilled = 0;
	level.numObjectives = 0;//no objectives by default

	G_RegisterCvars();

	G_ProcessIPBans();
	G_ProcessIDBans();

	G_InitMemory();

	//G_initGroupsList();

	BG_LoadItemNames();

	// set some level globals
	memset( &level, 0, sizeof( level ) );

	//Class loader
	memset( fileName, 0, sizeof( fileName ) );
	Com_sprintf( fileName, sizeof( fileName ), "ext_data/classes/%s.classes", rpg_classSet.string );	
	if ( !G_LoadClassData( fileName ) ) 
	{
		G_Printf( S_COLOR_RED "ERROR: Could not load class set %s. Reverting to default.\n", fileName );
		trap_Cvar_Set( "rpg_classSet", CLASS_DEFAULT );

		if ( !G_LoadClassData( va( "ext_data/classes/%s.classes", CLASS_DEFAULT ) ) )
		{
			G_Error( "Could not load default class set: %s", CLASS_DEFAULT );
		}
	}

	//Rank Loader
	memset( fileName, 0, sizeof( fileName ) );
	Com_sprintf( fileName, sizeof( fileName ), "ext_data/ranksets/%s.ranks", rpg_rankSet.string );
	if ( !BG_ParseRankNames( fileName, g_rankNames ) ) 
	{
		G_Printf( S_COLOR_RED "ERROR: Could not load rankset %s. Reverting to default.\n", fileName );
		trap_Cvar_Set( "rpg_rankSet", RANKSET_DEFAULT );

		if ( !BG_ParseRankNames( va( "ext_data/ranksets/%s.ranks", RANKSET_DEFAULT ), g_rankNames ) ) 
		{
			G_Error( "Could not load default rankset: %s", RANKSET_DEFAULT );
		}
	}

	level.time = levelTime;
	level.startTime = levelTime;
	level.restarted = restart;

	//level.message = levelTime - (int)(rpg_timedmessagetime.value * 60000) + 30000;
	if ( rpg_timedmessagetime.value < 0.2 ) {
		messageTime = 0.2;
	} else {
		messageTime = rpg_timedmessagetime.value;
	}

	level.message = levelTime + (messageTime * 60000);

	level.snd_fry = G_SoundIndex("sound/player/fry.wav");	// FIXME standing in lava / slime

	if ( g_gametype.integer != GT_SINGLE_PLAYER && g_log.string[0] ) {
		if ( g_logSync.integer ) {
			trap_FS_FOpenFile( g_log.string, &level.logFile, FS_APPEND_SYNC );
		} else {
			trap_FS_FOpenFile( g_log.string, &level.logFile, FS_APPEND );
		}
		if ( !level.logFile ) {
			G_Printf( "WARNING: Couldn't open logfile: %s\n", g_log.string );
		} else {
			char	serverinfo[MAX_INFO_STRING];

			trap_GetServerinfo( serverinfo, sizeof( serverinfo ) );

			G_LogPrintf("------------------------------------------------------------\n" );
			G_LogPrintf("InitGame: %s\n", serverinfo );
		}
	} else {
		G_Printf( "Not logging to disk.\n" );
	}

	#ifdef G_LUA
	G_LuaInit();
	#endif

	G_LogWeaponInit();

	G_InitWorldSession();

	// initialize all entities for this game
	memset( g_entities, 0, MAX_GENTITIES * sizeof(g_entities[0]) );
	level.gentities = g_entities;

	// initialize all clients for this game
	level.maxclients = g_maxclients.integer;
	memset( g_clients, 0, MAX_CLIENTS * sizeof(g_clients[0]) );
	level.clients = g_clients;

	// set client fields on player ents
	for ( i=0 ; i<level.maxclients ; i++ ) {
		g_entities[i].client = level.clients + i;
	}
	
	// always leave room for the max number of clients,
	// even if they aren't all used, so numbers inside that
	// range are NEVER anything but clients
	level.num_entities = MAX_CLIENTS;

	// let the server system know where the entites are
	trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ), 
		&level.clients[0].ps, sizeof( level.clients[0] ) );

	// reserve some spots for dead player bodies
	InitBodyQue();

	ClearRegisteredItems();

	// parse the map usables file
	G_SetupUsablesStrings();

	// parse the key/value pairs and spawn gentities
	G_SpawnEntitiesFromString();

	//TiM: load a locations file, whereever possible
	G_LoadLocationsFile();

	//GSIO: load server change file if avaible
	G_LoadServerChangeFile();

	//GSIO: load map change file if avaible
	G_LoadMapChangeFile();

	//GSIO: load holodeck file if avaible
	G_LoadHolodeckFile();

	// general initialization
	G_FindTeams();

	// make sure we have flags for CTF, etc
	G_CheckTeamItems();

	SaveRegisteredItems();

	G_Printf ("-----------------------------------\n");

	if( g_gametype.integer == GT_SINGLE_PLAYER || trap_Cvar_VariableIntegerValue( "com_buildScript" ) ) {
		G_ModelIndex( SP_PODIUM_MODEL );
		G_SoundIndex( "sound/player/gurp1.wav" );
		G_SoundIndex( "sound/player/gurp2.wav" );
	}
	if (g_gametype.integer >= GT_TEAM || trap_Cvar_VariableIntegerValue( "com_buildScript" ) )
	{
		G_ModelIndex( TEAM_PODIUM_MODEL );
	}

	if ( trap_Cvar_VariableIntegerValue( "bot_enable" ) ) {
		BotAISetup( restart );
		BotAILoadMap( restart );
		G_InitBots( restart );
	}

	G_InitModRules();
	
	levelExiting = qfalse;

/*RPG-X J2J************************************************************************************/
	G_Printf ("Initializing RPG-X Globals...");
	SpawnPnt = SelectRandomSpawnPoint();			//Grab a random spawn point.

	//TiM : Reset teh recon system on game init.
	//Leave this out for now to make this data persistant.
	//Altho, make sure to init the arrays before actually using lol
	
	//memset( &g_reconData, 0, sizeof( g_reconData ) );
	//g_reconNum = 0;

	//WARNING - used hard coded number to shut up compiler, 1023 is MAX_ENTITIES (which apperently cant be increased without substansial exe recoding)
	//TiM : NB Ents 0 -> 128 are clients only. cyclicng thru all ents here is not needed.
	for(i = 0; i < MAX_CLIENTS; i++)
	{
		//g_entities[i].last_tr116_fire = 0;
		
		//RPG-X: Redtechie - Make sure score cant be chnaged unless other wise told to
		if(g_entities[i].client){
			g_entities[i].client->UpdateScore = qfalse;
			g_entities[i].client->AdminFailed = 0; // RPG-X | Phenix | 21/11/2004 | Init Admin Failed Integer
			g_entities[i].client->LoggedAsAdmin = qfalse;
			g_entities[i].client->LoggedAsDeveloper = qfalse;
		}
	}

	for ( i = 0; i < MAX_CLIENTS; i++ )
	{
		lastKillTime[i] = level.time-30000;

		//FKR
		LastFKRadius[i] = level.time - rpg_forcekillradiuswaittime.integer;

		//Transporter
		memset( &TransDat[i], 0, sizeof( TransDat[i] ) );
		TransDat[i].LastClick = level.time-5000;				//Make last click current time less 5 seconds.
		//TransDat[i].Used = qfalse;							//And set both site to site and tricorder to unused.
		//TransDat[i].pUsed = qfalse;
		//And Zero the vectors
		//Huh... why couldn't we use the memset func 
		//here for most of those =0 statements?
		/*VectorCopy(vec3_origin, TransDat[i].TransCoord);
		VectorCopy(vec3_origin, TransDat[i].TransCoordRot);
		VectorCopy(vec3_origin, TransDat[i].pTransCoord);
		VectorCopy(vec3_origin, TransDat[i].pTransCoordRot);*/
		//TransDat[i].beamed = qfalse;
		//TransDat[i].beamer = -1;
		//TransDat[i].beamTime = 0;

		//Drag Data
		DragDat[i].AdminId = -1;
		//DragDat[i].distance = 0.0f;
	}

	//Couldn't think of anywhere else for this, so here it goes.
	//TiM - WTF?  You can't cache multimedia assets like that in the game module. O_o
	//The server couldn't care less about playing sounds.  No one would hear them >.<
	//n00bsnd = trap_S_RegisterSound("sound\n00b.mp3");

	//RPG-X: RedTechie - Set Shake cvars back to normal just to stop hacking
	//trap_SendConsoleCommand( EXEC_APPEND, "set rpg_servershakeallclients 0\n" );
	//trap_SendConsoleCommand( EXEC_APPEND, "set rpg_servershakeallclientsintensity 2\n" );

	// check for remap file, load and apply if one if found
	//G_LoadShaderRemaps();

	//Check spawn file - not in this release yet
	//G_ParseSpawnFile();

	G_Printf( "%i entities loaded.\n", level.num_entities );

	G_Printf ("COMPLETED\n");

	#ifdef G_LUA
	LuaHook_G_InitGame(levelTime, randomSeed, restart);
	#endif

	if(dev_showTriggers.integer && !restart) {
		gentity_t *t;
		t = G_Spawn();
		if(t) {
			t->think = Dev_ShowTriggers;
			t->nextthink = level.time + 1000;
		}
	}
/*************************************************************************************************/

	G_Printf("                       ,.                      \n");	G_Printf("          ..:,        :Xt.       ,:.            \n");
    G_Printf("         ,=+t:       .IRX=       :++=.         \n");	G_Printf("        .=iVt:.      :RYYI.      .itt+         \n");    
	G_Printf("       .:tXI=;.      tRtiV;       ,IYY:.       \n");	G_Printf("      .+;ii=;.      ,XVi+Vt.       :tIi+      \n");    
	G_Printf("     .;ti;;:.       +RI++IY,        ,+tt=.     \n");	G_Printf("    ,++YY;.        ,XXi+++X=         ;IYI=.    \n");    
	G_Printf("    ;ttY+;.    .,=iVRI++++YX+;.       ;VYt;    \n");	G_Printf("   .;ii+=,   .;IXRRXVi++++iVRXVi:.    ,=iii.   \n");    
	G_Printf("  .==;ti,  .;YRRVVXYii+++++IVIVRXt,   ,+=tI=   \n");	G_Printf("  .iitY=, .tRRVXXVRV+++ii++YRXVIYXV;   :tYti,  \n");   
	G_Printf("  .+iii=,,IBVVXYiiXViiiiiiitVtIXViVR=  ,+t+I:  \n");	G_Printf("   =+=I:.tBVXVt=;tRIiiiiiiiiXi:=YXiIX; :+=It;  \n");    
	G_Printf(" .;;tYt:;RVVV+=:,YRiiiiiiiiiYI,.:IXiVY..+IYi=  \n");	G_Printf(" .ti=t+;tRIXi;, :XViiiiiiiiiIV:  ,YViX=.:titt. \n");
	G_Printf("  iY++I;YVYY=:  +BIiiiiiiiiiiX=   +XiVi;i++Vi, \n");    G_Printf(" ,+YYYI:VYYY;. .YRiiiiiiiiiiiVt.  ;RIYt:IIVVi: \n");	
	G_Printf(" ,+tYXi;YVIX;  ;RVtiiiiIXXtiiVI,  iRIVt,=XVit: \n");    G_Printf(" .+iiti++XiXI. iBIiiiiYXIIXtiIV: :XXIV++;i+iI;.\n");
	G_Printf("  ;Ii=ii:VYtRi,VRtiiiVVi=;IXitX=;VBYXI=i+;iV+;.\n");    G_Printf("  ;tYtVt;;XYIRXBVttiVV+;:.:VYiXVRBVXY+;+IYVt+, \n");	
	G_Printf("  =iiItii,=XVIRRIttXV+=:..,tRtVBXVRI+=i:iIit+. \n");    G_Printf("  :t==++I:.=YXYIIiYBXYIttIVRBYtVXXI+;;t+;;+Y=, \n");
	G_Printf("   +I=;+Y= .:IRItYIVXRRRBBRXXVIRY+=;.:i=;iVi;. \n");    G_Printf("   .+IYVV+:  +BYXXVXXXXXXXXXVRVVi;:.:;tVYY+=:  \n");	
	G_Printf("    .+ttii+ .IBXY++ittIIIti++tXXi, .++=tI+;:   \n");    G_Printf("     ;YYtIY;;VBI+;:,::;;;;;:,:IBt,::tItYV=.    \n");
	G_Printf("      =IYYI++ti+;,   .......  :Xt;i=iYYI+;.    \n");    G_Printf("      .:+i++ii;;.             .=i=+i=t+;;:.    \n");
	G_Printf("        ,tYIVI==:,..       ..,;=+iYIVt:..      \n");    G_Printf("         ,itt+iIYYti;.   ,;itYIIt:iIi=;.       \n");
	G_Printf("          .:;;:+tIIVIi:.;iYYIii+=:,;;:.        \n");    G_Printf("            .  ,:=itIXi.tXYit=;::,  .          \n");	
	G_Printf("                 .+tti=,,iIt+;.                \n");    G_Printf("                  .:;;:. ,;;;:.                \n");	
	
}


/*
=================
G_ShutdownGame
=================
*/
void G_ShutdownGame( int restart ) {
	G_Printf ("==== ShutdownGame ====\n");

	#ifdef G_LUA
	LuaHook_G_Shutdown(restart);
	G_LuaShutdown();
	#endif

#if 0	// kef -- Pat sez this is causing some trouble these days
	G_LogWeaponOutput();
#endif
	if ( level.logFile ) {
		G_LogPrintf("ShutdownGame:\n" );
		G_LogPrintf("------------------------------------------------------------\n" );
		trap_FS_FCloseFile( level.logFile );
	}

	// write all the client session data so we can get it back
	G_WriteSessionData();

	if ( trap_Cvar_VariableIntegerValue( "bot_enable" ) ) {
		BotAIShutdown( restart );
	}
}



//===================================================================

#ifndef GAME_HARD_LINKED
// this is only here so the functions in q_shared.c and bg_*.c can link

void QDECL Com_Error ( int errlevel, const char *error, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, error);
	vsprintf (text, error, argptr);
	va_end (argptr);

	G_Error( "%s", text);
}

void QDECL Com_Printf( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	vsprintf (text, msg, argptr);
	va_end (argptr);

	G_Printf ("%s", text);
}

#endif

/*
========================================================================

PLAYER COUNTING / SCORE SORTING

========================================================================
*/

/*
=============
AddTournamentPlayer

If there are less than two tournament players, put a
spectator in the game and restart
=============
*/
void AddTournamentPlayer( void ) {
	int			i;
	gclient_t	*client;
	gclient_t	*nextInLine;
	clientSession_t *sess;

	if ( level.numPlayingClients >= 2 ) {
		return;
	}

	// never change during intermission
	if ( level.intermissiontime ) {
		return;
	}

	nextInLine = NULL;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		client = &level.clients[i];
		if ( client->pers.connected != CON_CONNECTED ) {
			continue;
		}
		sess = &client->sess;
		if ( sess->sessionTeam != TEAM_SPECTATOR ) {
			continue;
		}
		// never select the dedicated follow or scoreboard clients
		if ( sess->spectatorState == SPECTATOR_SCOREBOARD || 
			sess->spectatorClient < 0  ) {
			continue;
		}

		if ( !nextInLine || sess->spectatorTime < nextInLine->sess.spectatorTime ) {
			nextInLine = client;
		}
	}

	if ( !nextInLine ) {
		return;
	}

	level.warmupTime = -1;

	// set them to free-for-all team
	SetTeam( &g_entities[ nextInLine - level.clients ], "f" );
}


/*
=======================
RemoveTournamentLoser

Make the loser a spectator at the back of the line
=======================
*/
void RemoveTournamentLoser( void ) {
	int			clientNum;

	if ( level.numPlayingClients != 2 ) {
		return;
	}

	clientNum = level.sortedClients[1];

	if ( level.clients[ clientNum ].pers.connected != CON_CONNECTED ) {
		return;
	}

	// make them a spectator
	SetTeam( &g_entities[ clientNum ], "s" );
}


/*
=======================
AdjustTournamentScores

=======================
*/
void AdjustTournamentScores( void ) {
	int			clientNum;

	clientNum = level.sortedClients[0];
	if ( level.clients[ clientNum ].pers.connected == CON_CONNECTED ) {
		level.clients[ clientNum ].sess.wins++;
		ClientUserinfoChanged( clientNum );
	}

	clientNum = level.sortedClients[1];
	if ( level.clients[ clientNum ].pers.connected == CON_CONNECTED ) {
		level.clients[ clientNum ].sess.losses++;
		ClientUserinfoChanged( clientNum );
	}

}



/*
=============
SortRanks

=============
*/
int QDECL SortRanks( const void *a, const void *b ) {
	gclient_t	*ca, *cb;

	ca = &level.clients[*(int *)a];
	cb = &level.clients[*(int *)b];

	// sort special clients last
	if ( ca->sess.spectatorState == SPECTATOR_SCOREBOARD || ca->sess.spectatorClient < 0 ) {
		return 1;
	}
	if ( cb->sess.spectatorState == SPECTATOR_SCOREBOARD || cb->sess.spectatorClient < 0  ) {
		return -1;
	}

	// then connecting clients
	if ( ca->pers.connected == CON_CONNECTING ) {
		return 1;
	}
	if ( cb->pers.connected == CON_CONNECTING ) {
		return -1;
	}


	// then spectators
	if ( ca->sess.sessionTeam == TEAM_SPECTATOR && cb->sess.sessionTeam == TEAM_SPECTATOR ) {
		if ( ca->sess.spectatorTime < cb->sess.spectatorTime ) {
			return -1;
		}
		if ( ca->sess.spectatorTime > cb->sess.spectatorTime ) {
			return 1;
		}
		return 0;
	}
	if ( ca->sess.sessionTeam == TEAM_SPECTATOR ) {
		return 1;
	}
	if ( cb->sess.sessionTeam == TEAM_SPECTATOR ) {
		return -1;
	}

	// then sort by score & number of times killed
	if ( ca->ps.persistant[PERS_SCORE]
		> cb->ps.persistant[PERS_SCORE] ) {
		return -1;
	}
	if ((ca->ps.persistant[PERS_SCORE] == cb->ps.persistant[PERS_SCORE]) &&
		(ca->ps.persistant[PERS_KILLED] < cb->ps.persistant[PERS_KILLED])   )
	{	return -1;}

	if ( ca->ps.persistant[PERS_SCORE]
		< cb->ps.persistant[PERS_SCORE] ) {
		return 1;
	}
	if ((ca->ps.persistant[PERS_SCORE] == cb->ps.persistant[PERS_SCORE]) &&
		(ca->ps.persistant[PERS_KILLED] > cb->ps.persistant[PERS_KILLED])   )
	{	return 1;}

	return 0;
}

/*
============
CalculateRanks

Recalculates the score ranks of all players
This will be called on every client connect, begin, disconnect, death,
and team change.

FIXME: for elimination, the last man standing must be ranked first
============
*/
void CalculateRanks( qboolean fromExit ) {
	int		i;
	int		rank;
	int		score;
	int		newScore;
	gclient_t	*cl;

	level.follow1 = -1;
	level.follow2 = -1;
	level.numConnectedClients = 0;
	level.numNonSpectatorClients = 0;
	level.numPlayingClients = 0;
	level.numVotingClients = 0;		// don't count bots
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].pers.connected != CON_DISCONNECTED ) {
			level.sortedClients[level.numConnectedClients] = i;
			level.numConnectedClients++;

			if ( level.clients[i].sess.sessionTeam != TEAM_SPECTATOR ) {
				level.numNonSpectatorClients++;
			
				// decide if this should be auto-followed
				if ( level.clients[i].pers.connected == CON_CONNECTED ) {
					level.numPlayingClients++;
					if ( !(g_entities[i].r.svFlags & SVF_BOT) ) {
						level.numVotingClients++;
					}
					if ( level.follow1 == -1 ) {
						level.follow1 = i;
					} else if ( level.follow2 == -1 ) {
						level.follow2 = i;
					}
				}
			}
		}
	}

	qsort( level.sortedClients, level.numConnectedClients, 
		sizeof(level.sortedClients[0]), SortRanks );

	// set the rank value for all clients that are connected and not spectators
	if ( g_gametype.integer >= GT_TEAM ) {
		// in team games, rank is just the order of the teams, 0=red, 1=blue, 2=tied
		for ( i = 0;  i < level.numConnectedClients; i++ ) {
			cl = &level.clients[ level.sortedClients[i] ];
			if ( level.teamScores[TEAM_RED] == level.teamScores[TEAM_BLUE] ) {
				cl->ps.persistant[PERS_RANK] = 2;
			} else if ( level.teamScores[TEAM_RED] > level.teamScores[TEAM_BLUE] ) {
				cl->ps.persistant[PERS_RANK] = 0;
			} else {
				cl->ps.persistant[PERS_RANK] = 1;
			}
		}
	} else {	
		rank = -1;
		score = 0;
		for ( i = 0;  i < level.numPlayingClients; i++ ) {
			cl = &level.clients[ level.sortedClients[i] ];
			newScore = cl->ps.persistant[PERS_SCORE];
			if ( i == 0 || newScore != score ) {
				rank = i;
				// assume we aren't tied until the next client is checked
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank;
			} else {
				// we are tied with the previous client
				level.clients[ level.sortedClients[i-1] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
			}
			score = newScore;
			if ( g_gametype.integer == GT_SINGLE_PLAYER && level.numPlayingClients == 1 ) {
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
			}
		}
	}

	// set the CS_SCORES1/2 configstrings, which will be visible to everyone
	if ( g_gametype.integer >= GT_TEAM ) {
		trap_SetConfigstring( CS_SCORES1, va("%i", level.teamScores[TEAM_RED] ) );
		trap_SetConfigstring( CS_SCORES2, va("%i", level.teamScores[TEAM_BLUE] ) );
	} else {
		if ( level.numConnectedClients == 0 ) {
			trap_SetConfigstring( CS_SCORES1, va("%i", SCORE_NOT_PRESENT) );
			trap_SetConfigstring( CS_SCORES2, va("%i", SCORE_NOT_PRESENT) );
		} else if ( level.numConnectedClients == 1 ) {
			trap_SetConfigstring( CS_SCORES1, va("%i", level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE] ) );
			trap_SetConfigstring( CS_SCORES2, va("%i", SCORE_NOT_PRESENT) );
		} else {
			trap_SetConfigstring( CS_SCORES1, va("%i", level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE] ) );
			trap_SetConfigstring( CS_SCORES2, va("%i", level.clients[ level.sortedClients[1] ].ps.persistant[PERS_SCORE] ) );
		}
	}

	// see if it is time to end the level
	if ( !fromExit )
	{//not coming this from the CheckExitRules func
		CheckExitRules();
	}

	// if we are at the intermission, send the new info to everyone
	if ( level.intermissiontime ) {
		SendScoreboardMessageToAllClients();
	}
}


/*
========================================================================

MAP CHANGING

========================================================================
*/

/*
========================
SendScoreboardMessageToAllClients

Do this at BeginIntermission time and whenever ranks are recalculated
due to enters/exits/forced team changes
========================
*/
void SendScoreboardMessageToAllClients( void ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[ i ].pers.connected == CON_CONNECTED ) {
			DeathmatchScoreboardMessage( g_entities + i );
		}
	}
}

/*
========================
MoveClientToIntermission

When the intermission starts, this will be called for all players.
If a new client connects, this will be called after the spawn function.
========================
*/
void MoveClientToIntermission( gentity_t *ent ) {
	entityState_t *es = &ent->s;
	playerState_t *ps = &ent->client->ps;

	// take out of follow mode if needed
	if ( ent->client->sess.spectatorState == SPECTATOR_FOLLOW ) {
		StopFollowing( ent );
	}


	// move to the spot
	VectorCopy( level.intermission_origin, es->origin );
	VectorCopy( level.intermission_origin, ps->origin );
	VectorCopy (level.intermission_angle, ps->viewangles);
	ps->pm_type = PM_INTERMISSION;
	ps->eFlags ^= EF_TELEPORT_BIT;

	// clean up powerup info
	memset( ps->powerups, 0, sizeof(ps->powerups) );

	ps->eFlags = 0;
	es->eFlags = 0;
	es->eType = ET_GENERAL;
	es->modelindex = 0;
	es->loopSound = 0;
	es->event = 0;
	ent->r.contents = 0;
}

/*
==================
FindIntermissionPoint

This is also used for spectator spawns
==================
*/
void FindIntermissionPoint( void ) {
	gentity_t	*ent, *target;
	vec3_t		dir;

	// find the intermission spot
	ent = G_Find (NULL, FOFS(classname), "info_player_intermission");
	if ( !ent ) {	// the map creator forgot to put in an intermission point...
		SelectSpawnPoint ( vec3_origin, level.intermission_origin, level.intermission_angle );
	} else {
		VectorCopy (ent->s.origin, level.intermission_origin);
		VectorCopy (ent->s.angles, level.intermission_angle);
		// if it has a target, look towards it
		if ( ent->target ) {
			target = G_PickTarget( ent->target );
			if ( target ) {
				VectorSubtract( target->s.origin, level.intermission_origin, dir );
				vectoangles( dir, level.intermission_angle );
			}
		}
	}

}

/*
==================
ClearFiringFlags
==================
*/
void ClearFiringFlags()
{
	int i = 0;
	gentity_t	*ent = NULL;

	for (i=0 ; i< level.maxclients ; i++) 
	{
		ent = g_entities + i;
		if (!ent->inuse)
			continue;
		// clear the firing flag
		if (ent->client)
		{
			ent->client->ps.eFlags &= ~EF_FIRING;
		}
	}
}

/*
==================
BeginIntermission
==================
*/
void BeginIntermission( void ) {
	int			i;
	gentity_t	*client;
	qboolean	doingLevelshot;

	if (level.intermissiontime == -1)
		doingLevelshot = qtrue;
	else
		doingLevelshot = qfalse;

	if ( level.intermissiontime && level.intermissiontime != -1 ) {
		return;		// already active
	}

	// if in tournament mode, change the wins / losses
	if ( g_gametype.integer == GT_TOURNAMENT ) {
		AdjustTournamentScores();
	}

	level.intermissiontime = level.time;
	FindIntermissionPoint();

	// kef -- make sure none of the players are still firing (cuz we don't want weapons fx going off while
	//they're on the podium)
	ClearFiringFlags();


	// cdr - Want to generate victory pads for all game types  - except level shots (gametype 10)
	UpdateTournamentInfo();
	if (!doingLevelshot)
		SpawnModelsOnVictoryPads();


	// move all clients to the intermission point
	for (i=0 ; i< level.maxclients ; i++) {
		client = g_entities + i;
		if (!client->inuse)
			continue;
		// respawn if dead
		//if ( BG_BorgTransporting( &client->client->ps ) )
		//{//in borg teleport fly around mode, turn it off
		//	client->client->ps.stats[STAT_USEABLE_PLACED] = 0;
		//	client->client->ps.stats[STAT_HOLDABLE_ITEM] = 0;
		//	client->client->ps.eFlags &= ~EF_NODRAW;
		//	client->s.eFlags &= ~EF_NODRAW;
		//	client->flags &= ~FL_NOTARGET;
		//}
		if (client->health <= 0) {
			respawn(client);
		}
		MoveClientToIntermission( client );

	}

	// send the current scoring to all clients
	SendScoreboardMessageToAllClients();
}


void G_ClearObjectives( void )
{
	gentity_t *tent;

	tent = G_TempEntity( vec3_origin, EV_OBJECTIVE_COMPLETE );
	//Be sure to send the event to everyone
	tent->r.svFlags |= SVF_BROADCAST;
	tent->s.eventParm = 0;//tells it to clear all
}
/*
=============
ExitLevel

When the intermission has been exited, the server is either killed
or moved to a new level based on the "nextmap" cvar 

=============
*/
qboolean levelExiting = qfalse;
void ExitLevel (void) {
	int		i;

	levelExiting = qtrue;

	//bot interbreeding
	BotInterbreedEndMatch();

	G_ClearObjectives();
	// if we are running a tournement map, kick the loser to spectator status,
	// which will automatically grab the next spectator and restart
	if ( g_gametype.integer == GT_TOURNAMENT ) {
		if ( !level.restarted ) {
			RemoveTournamentLoser();
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			level.changemap = NULL;
			level.intermissiontime = 0;
		}
		return;	
	}

	trap_SendConsoleCommand( EXEC_APPEND, "vstr nextmap\n" );
	level.changemap = NULL;
	level.intermissiontime = 0;

	// we need to do this here before chaning to CON_CONNECTING
	G_WriteSessionData();

	// change all client states to connecting, so the early players into the
	// next level will know the others aren't done reconnecting
	for (i=0 ; i< g_maxclients.integer ; i++) {
		if ( level.clients[i].pers.connected == CON_CONNECTED ) {
			level.clients[i].pers.connected = CON_CONNECTING;
		}
	}

}


/*
=================
CheckIntermissionExit

The level will stay at the intermission for a minimum of 5 seconds
If all players wish to continue, the level will then exit.
If one or more players have not acknowledged the continue, the game will
wait 20 seconds before going on.
=================
*/
void CheckIntermissionExit( void ) {
	int			ready, notReady;
	int			i;
	gclient_t	*cl;
	int			readyMask;


	if ( levelExiting )
	{//already on our way out, skip the check
		return;
	}

	if ( level.time < level.intermissiontime + 5000 )
	{
			// bring up the scoreboard after 5 seconds
	}


	// Single player exit does not happen until menu event
	if ( g_gametype.integer == GT_SINGLE_PLAYER ) {
		return;
	}

	// see which players are ready
	ready = 0;
	notReady = 0;
	readyMask = 0;
	for (i=0 ; i< g_maxclients.integer ; i++) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
		if ( g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT ) {
			continue;
		}

		if ( cl->readyToExit ) {
			ready++;
			if ( i < 16 ) {
				readyMask |= 1 << i;
			}
		} else {
			notReady++;
		}
	}

	// copy the readyMask to each player's stats so
	// it can be displayed on the scoreboard
	for (i=0 ; i< g_maxclients.integer ; i++) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
		cl->ps.stats[STAT_CLIENTS_READY] = readyMask;
	}

	// never exit in less than five seconds
	if ( level.time < level.intermissiontime + 5000 ) {
		return;
	}

	// if nobody wants to go, clear timer
	if ( !ready ) {
		level.readyToExit = qfalse;
		return;
	}

	// if everyone wants to go, go now
	if ( !notReady ) {
		ExitLevel();
		return;
	}

	// the first person to ready starts the ten second timeout
	if ( !level.readyToExit ) {
		level.readyToExit = qtrue;
		level.exitTime = level.time;
	}

	// if we have waited g_intermissionTime seconds since at least one player
	// wanted to exit, go ahead
	if ( level.time < level.exitTime + (1000 * g_intermissionTime.integer) ) {
		return;
	}

	ExitLevel();
}

/*
=============
ScoreIsTied
=============
*/
qboolean ScoreIsTied( void ) {
	int		a, b;

	if ( level.numPlayingClients < 2 ) {
		return qfalse;
	}
	
	if ( g_gametype.integer >= GT_TEAM ) {
		return level.teamScores[TEAM_RED] == level.teamScores[TEAM_BLUE];
	}

	a = level.clients[level.sortedClients[0]].ps.persistant[PERS_SCORE];
	b = level.clients[level.sortedClients[1]].ps.persistant[PERS_SCORE];

	return a == b;
}

/*
=================
CheckExitRules

There will be a delay between the time the exit is qualified for
and the time everyone is moved to the intermission spot, so you
can see the last frag.
=================
*/
void CheckExitRules( void ) {
	//RPG-X: RedTechie - No exit in RPG Your TRAPED! MHAHAHA
	return;
}



/*
========================================================================

FUNCTIONS CALLED EVERY FRAME

========================================================================
*/


/*
=============
CheckTournement

Once a frame, check for changes in tournement player state
=============
*/
void CheckTournement( void ) {
	if ( level.numPlayingClients == 0 ) {
		return;
	}

	if ( g_gametype.integer == GT_TOURNAMENT ) {

		// pull in a spectator if needed
		//RPG-X: RedTechie - pulling on people isnt nice
		/*if ( level.numPlayingClients < 2 ) {
			AddTournamentPlayer();
		}*/

		// if we don't have two players, go back to "waiting for players"
		//RPG-X: RedTechie - No warmup!
		/*if ( level.numPlayingClients != 2 ) {
			if ( level.warmupTime != -1 ) {
				level.warmupTime = -1;
				trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
				G_LogPrintf( "Warmup:\n" );
			}
			return;
		}*/

		if ( level.warmupTime == 0 || level.warmupTime != 0) {//RPG-X: RedTechie - No warmup Fail safe
			return;
		}

		// if the warmup is changed at the console, restart it
		//RPG-X: RedTechie - No warmup!
		/*if ( g_warmup.modificationCount != level.warmupModificationCount ) {
			level.warmupModificationCount = g_warmup.modificationCount;
			level.warmupTime = -1;
		}*/

		// if all players have arrived, start the countdown
		//RPG-X: RedTechie - No warmup!
		/*if ( level.warmupTime < 0 )
		{
			if ( level.numPlayingClients == 2 )
			{
				if ( g_warmup.integer > 1 )
				{
					// fudge by -1 to account for extra delays
					level.warmupTime = level.time + ( g_warmup.integer - 1 ) * 1000;
					trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
				}
				else
				{
					level.warmupTime = 0;
				}
			}
			return;
		}*/
		
		//RPG-X: RedTechie - No warmup!
		// if the warmup time has counted down, restart
		/*if ( level.time > level.warmupTime ) {
			level.warmupTime += 10000;
			trap_Cvar_Set( "g_restarted", "1" );
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			return;
		}*/
	} else if ( g_gametype.integer != GT_SINGLE_PLAYER /*&& g_doWarmup.integer*/ ) { //RPG-X: RedTechie - No warmup!
		int		counts[TEAM_NUM_TEAMS];
		//qboolean	notEnough = qfalse;

		if ( g_gametype.integer > GT_TEAM ) {
			counts[TEAM_BLUE] = TeamCount( -1, TEAM_BLUE );
			counts[TEAM_RED] = TeamCount( -1, TEAM_RED );
		
		//RPG-X: RedTechie - Enough players always
		/*	if (counts[TEAM_RED] < 1 || counts[TEAM_BLUE] < 1) {
				notEnough = qtrue;
			}
		} else if ( level.numPlayingClients < 2 ) {
			notEnough = qtrue;
		}*/
		
		//RPG-X: RedTechie - No warmup!
		/*if ( notEnough ) {
			if ( level.warmupTime != -1 ) {
				level.warmupTime = -1;
				trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
				G_LogPrintf( "Warmup:\n" );
			}
			return; // still waiting for team members
		}*/
		
		if ( level.warmupTime == 0) {
			return;
		}

		// if the warmup is changed at the console, restart it
		//RPG-X: RedTechie - No warmup!
		/*if ( g_warmup.modificationCount != level.warmupModificationCount ) {
			level.warmupModificationCount = g_warmup.modificationCount;
			level.warmupTime = -1;
		}*/

		// if all players have arrived, start the countdown
		//RPG-X: RedTechie - No warmup!
		/*if ( level.warmupTime < 0 ) {
			// fudge by -1 to account for extra delays
			level.warmupTime = level.time + ( g_warmup.integer - 1 ) * 1000;
			trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
			return;
		}*/

		// if the warmup time has counted down, restart
		//RPG-X: RedTechie - No warmup!
		/*if ( level.time > level.warmupTime ) {
			level.warmupTime += 10000;
			trap_Cvar_Set( "g_restarted", "1" );
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			return;*/
		}
	}
}


/*
==================
CheckVote
==================
*/
void CheckVote( void ) {
	if ( !level.voteTime ) {
		return;
	}
	if ( level.time - level.voteTime >= VOTE_TIME ) {
		trap_SendServerCommand( -1, "print \"Vote failed.\n\"" );
	} else {
		if ( level.voteYes > level.numVotingClients/2 ) {
			// execute the command, then remove the vote
			trap_SendServerCommand( -1, "print \"Vote passed.\n\"" );
			trap_SendConsoleCommand( EXEC_APPEND, va("%s\n", level.voteString ) );
		} else if ( level.voteNo >= level.numVotingClients/2 ) {
			// same behavior as a timeout
			trap_SendServerCommand( -1, "print \"Vote failed.\n\"" );
		} else {
			// still waiting for a majority
			return;
		}
	}
	level.voteTime = 0;
	trap_SetConfigstring( CS_VOTE_TIME, "" );

}


/*
==================
CheckCvars
==================
*/
void CheckCvars( void ) {
	static int lastMod = -1;

	if ( g_password.modificationCount != lastMod ) {
		lastMod = g_password.modificationCount;
		if ( *g_password.string && Q_stricmp( g_password.string, "none" ) ) {
			trap_Cvar_Set( "g_needpass", "1" );
		} else {
			trap_Cvar_Set( "g_needpass", "0" );
		}
	}
}

/*
=============
G_RunThink

Runs thinking code for this frame if necessary
=============
*/
void G_RunThink (gentity_t *ent) {
	float	thinktime;

	thinktime = ent->nextthink;
	if (thinktime <= 0) {
		return;
	}
	if (thinktime > level.time) {
		return;
	}
	
	ent->nextthink = 0;
	if (!ent->think) {
		G_Error ( "NULL ent->think");
	}

	#ifdef G_LUA
	if(ent->luaThink && !ent->client)
	{
		LuaHook_G_EntityThink(ent->luaThink, ent->s.number);
	}
	#endif

	ent->think (ent);
}

/*
================
G_RunFrame

Advances the non-player objects in the world
================
*/

extern void SetClass( gentity_t *ent, char *s, char *teamName, qboolean SaveToCvar );
void DragCheck( void );								//RPG-X: J2J - Added to rid warning.
void CheckHealthInfoMessage( void );
extern qboolean levelExiting;
void G_RunFrame( int levelTime ) {
	int			i;
	gentity_t	*ent;
	gclient_t	*client;
	playerState_t *ps;
	entityState_t *es;
	int			msec;
int start, end;

	// if we are waiting for the level to restart, do nothing
	if ( level.restarted ) {
		return;
	}

	level.framenum++;
	level.previousTime = level.time;
	level.time = levelTime;
	msec = level.time - level.previousTime;

	// get any cvar changes
	G_UpdateCvars();

	//
	// go through all allocated objects
	//
start = trap_Milliseconds();
	ent = &g_entities[0];
	for (i=0 ; i<level.num_entities ; i++, ent++) {
		if ( !ent->inuse ) {
			continue;
		}

		es = &ent->s;
		ps = &ent->client->ps;

		// clear events that are too old
		if ( level.time - ent->eventTime > EVENT_VALID_MSEC ) {
			if ( es->event ) {
				es->event = 0;	// &= EV_EVENT_BITS;
				if ( ent->client ) {
					ps->externalEvent = 0;
					ps->events[0] = 0;
					ps->events[1] = 0;
					ps->events[2] = 0;
					ps->events[3] = 0;
				}
			}
			if ( ent->freeAfterEvent ) {
				// tempEntities or dropped items completely go away after their event
				G_FreeEntity( ent );
				continue;
			} else if ( ent->unlinkAfterEvent ) {
				// items that will respawn will hide themselves after their pickup event
				ent->unlinkAfterEvent = qfalse;
				trap_UnlinkEntity( ent );
			}
		}

		// temporary entities don't think
		if ( ent->freeAfterEvent ) {
			continue;
		}

		if ( !ent->r.linked && ent->neverFree ) {
			continue;
		}

		if ( !ent->client )
		{
			if ( es->eFlags & EF_ANIM_ONCE )
			{//this must be capped render-side
				es->frame++;
			}
		}

		if ( (es->eType == ET_MISSILE) || (es->eType == ET_ALT_MISSILE) ) {
			G_RunMissile( ent );
			continue;
		}

		if ( es->eType == ET_ITEM || ent->physicsObject ) {
			G_RunItem( ent );
			continue;
		}

		if ( es->eType == ET_MOVER || es->eType == ET_MOVER_STR ) { //RPG-X | GSIO01 | 13/05/2009
			G_RunMover( ent );
			continue;
		}

		if ( i < MAX_CLIENTS ) {
			G_RunClient( ent );
			continue;
		}

		G_RunThink( ent );
	}
	end = trap_Milliseconds();

	start = trap_Milliseconds();
	// perform final fixups on the players
	ent = &g_entities[0];
	for (i=0 ; i < level.maxclients ; i++, ent++ ) {
		if ( ent->inuse ) {
			ClientEndFrame( ent );
		}
	}
	end = trap_Milliseconds();

	// see if it is time to do a tournement restart
	CheckTournement();

	// see if it is time to end the level
	CheckExitRules();

	// update to team status?
	CheckTeamStatus();

	// update to health status
	//CheckHealthInfoMessage();//done from inside CheckTeamStatus now

	// cancel vote if timed out
	CheckVote();

	// for tracking changes
	CheckCvars();

	//RPG-X: RedTechie - Count down our shake camera timer
	//TiM: NOT NECESSARY
	/*if(rpg_servershakeallclients.integer == 1){
		if(shaketimer < level.time){
			trap_SendConsoleCommand( EXEC_APPEND, "set rpg_servershakeallclients 0\n" );
		}
	}*/

	//RPG-X: J2J - This will check for drag movements that need to be calculated.
	DragCheck();

	for ( i = 0; i < MAX_CLIENTS; i++ )
	{
		ent = &g_entities[i];

		if(!ent || !ent->client)
			continue;

		client = ent->client;

		if (!(client->pers.cmd.buttons & BUTTON_USE)) {
			client->pressedUse = qfalse;
		}

		if (g_classData[client->sess.sessionClass].isn00b/*ent->client->sess.sessionClass == PC_N00B*/)
		{
			if ((client->n00bTime != -1) && (client->n00bTime <= level.time)&&client->origClass[0])
			{
				SetClass( ent, client->origClass, NULL, qtrue );
			}
		}
	}

	//RPG-X: Marcin: To clear pressedUse. - 30/12/2008

	#ifdef G_LUA
	LuaHook_G_RunFrame(levelTime);
	#endif

}
