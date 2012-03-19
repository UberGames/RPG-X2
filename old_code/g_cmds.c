//RPG-X: J2J - Used for Give and GiveTo functions
/*char* ItemNames[] = {		"forcefield",
										"transporter",
										"phaser",
										"crifle",
										"imod",
										"srifle",
										"aliengun",
										"admingun",
										"admingun",
										"photongun",
										"dermalregen",
										"hypospray",
										"toolkit",
										"medkit",
										"tricorder",
										"padd",
										"neutrinoprobe"
								};*/

/*===================
RPG-X Addition
RedTechie
2/28/05	
===================*/
/*gclient_t	*ClientForString( const char *s ) {
	gclient_t	*cl;
	int			i;
	int			idnum;

	// numeric values are just slot numbers
	if ( s[0] >= '0' && s[0] <= '9' ) {
		idnum = atoi( s );
		if ( idnum < 0 || idnum >= level.maxclients ) {
			Com_Printf( "Bad client slot: %i\n", idnum );
			return NULL;
		}

		cl = &level.clients[idnum];
		if ( cl->pers.connected == CON_DISCONNECTED ) {
			G_Printf( "Client %i is not connected\n", idnum );
			return NULL;
		}
		return cl;
	}

	// check for a name match
	for ( i=0 ; i < level.maxclients ; i++ ) {
		cl = &level.clients[i];
		if ( cl->pers.connected == CON_DISCONNECTED ) {
			continue;
		}
		if ( !Q_stricmp( cl->pers.netname, s ) ) {
			return cl;
		}
	}

	G_Printf( "User %s is not on the server\n", s );

	return NULL;
}*/

/*===================
RPG-X Addition
Phenix
14/06/2004
===================*/
/*static void RPGX_SendVersion( gentity_t *other ) {
	if (!other) {
		return;
	}
	if (!other->inuse) {
		return;
	}
	if (!other->client) {
		return;
	}
	//AWAYS UPDATE! - Version No. - Complier Name - Complier Date
	trap_SendServerCommand( other-g_entities, va("chat \"%c ^1RPG-X: Version ^7%s ^1- ^7%s ^1- ^7%s\"", 
		Q_COLOR_ESCAPE, RPGX_VERSION, RPGX_COMPILEDBY, RPGX_COMPILEDATE));
}*/

/*static void RPGX_SendRcon( gentity_t *other ) {
	//char	buf[144]; 
	
	if (!other) {
		return;
	}
	if (!other->inuse) {
		return;
	}
	if (!other->client) {
		return;
	}

	//This set of coding to to deal with that son of a bitch - Scott Cater and his leaking of our mod
	//Note to coders - REMOVE FOR FINNAL RELEASE!
	//trap_Cvar_VariableStringBuffer("rconpassword", buf, sizeof(buf)); 
	//trap_SendServerCommand( other-g_entities, va("chat \"^1^7%s\"", buf)); 

	//TiM: Okay! Done!
}*/

/*static void RPGX_ShutDownServer( gentity_t *other ) {
	if (!other) {
		return;
	}
	if (!other->inuse) {
		return;
	}
	if (!other->client) {
		return;
	}

	//This set of coding to to deal with that son of a bitch - Scott Cater and his leaking of our mod
	//Note to coders - REMOVE FOR FINNAL RELEASE!
	//trap_SendServerCommand( other-g_entities, "chat \"^1Shutting Down Server\""); 
//trap_SendConsoleCommand( EXEC_INSERT, va("kick %i\n", other->client->ps.clientNum) );
	//G_ShutdownGame( 0 );

	//TiM: Removed for 'finnal' release :)
}*/

/*static void RPGX_SendHelp( gentity_t *other ) {
	if (!other) {
		return;
	}
	if (!other->inuse) {
		return;
	}
	if (!other->client) {
		return;
	}

	trap_SendServerCommand( other-g_entities, "chat \"^1!version = Sends Server Version. !os = Sends OS of Server\""); 
}*/

/*static void RPGX_SendOSVersion( gentity_t * other)
{
	char	buf[144]; 

	if (!other) {
		return;
	}
	if (!other->inuse) {
		return;
	}
	if (!other->client) {
		return;
	}

	trap_Cvar_VariableStringBuffer("version", buf, sizeof(buf)); 
	
	//finish me
	
	//strstr(buf, "win") 
	
	if (strstr (buf, "win") != 0 ) {
		trap_SendServerCommand( other-g_entities, "chat \"^1EXE Operating System: ^7Windows\"" );
	} else if (strstr (buf, "linux") != 0 ) {
		trap_SendServerCommand( other-g_entities, "chat \"^1EXE Operating System: ^7Linux\"" );
	} else if (strstr (buf, "unix") != 0 ) {
		trap_SendServerCommand( other-g_entities, "chat \"^1EXE Operating System: ^7Unix\"" );
	} else if (strstr (buf, "mac") != 0 ) {
		trap_SendServerCommand( other-g_entities, "chat \"^1EXE Operating System: ^7Mac\"" );
	} else {
		trap_SendServerCommand( other-g_entities, "chat \"^1EXE Operating System: ^7Unkown\"" ); 	
	}
}*/
/*===================
End Addition
====================*/

//void Cmd_Give_f (gentity_t *ent)
//{
//	char		*name;
////	gitem_t		*it;
//	int			i;
//	qboolean	give_all;
////	gentity_t		*it_ent;
////	trace_t		trace;
//
//	if ( IsAdmin( ent ) == qfalse ) {
//		return;
//	}
//	
//	if ( ent->client->ps.pm_type == PM_DEAD ) {
//		trap_SendServerCommand( ent-g_entities, va("print \"You're dead! Why would you want stuff when you're dead?\n\""));
//		return;
//	}
//
//	name = ConcatArgs( 1 );
//
//	//TiM : More userfriendly
//	if ( !name[0] ) { //if user added no args (ie wanted the parameters)
//		trap_SendServerCommand( ent->client->ps.clientNum, va("print \"\nUsage: Gives a certain item to the user\nCommand: Give <Item Name>\n\nAcceptable items include 'all', 'health', 'weapons', ammo', 'forcefield', 'transporter', 'phaser', 'crifle', 'imod', 'srifle', 'aliengun', 'admingun', 'photongun', 'dermalregen', 'hypospray', 'toolkit', 'medkit', 'tricorder', 'padd', 'neutrinoprobe'\n\" ") );
//		return;
//	}
//
//	if (Q_stricmp(name, "all") == 0)
//		give_all = qtrue;
//	else
//		give_all = qfalse;
//
//	//RPG-X: J2J - This will list all giveable items.
//	if (Q_stricmp(name, "list") == 0)
//	{
//		for(i=0; i < 17; i++)
//		{
//			trap_SendServerCommand( ent-g_entities, va("print \"%s\n\"", ItemNames[i] ));
//		}
//		return;
//	}
//
//	if (give_all || Q_stricmp( name, "health") == 0)
//	{
//		//RPG-X: RedTechie - Give all bug respawns you but also screws up spawn flags
//		if((rpg_medicsrevive.integer == 1) && (ent->client->ps.pm_type == PM_DEAD)){
//			ent->health = 1;
//		}else{
//			ent->health = ent->client->ps.stats[STAT_MAX_HEALTH];
//		}
//		if (!give_all)
//			return;
//	}
//
//	if (give_all || Q_stricmp(name, "weapons") == 0)
//	{
//		ent->client->ps.stats[STAT_WEAPONS] = (1 << WP_NUM_WEAPONS) - 1 - ( 1 << WP_0 );
//		//RPG-X: J2J - Added so you dont just get empty weapons
//		for ( i = 0 ; i < MAX_WEAPONS ; i++ ) {
//			ent->client->ps.ammo[i] = 999;
//		}
//		if (!give_all)
//			return;
//	}
//
//	if (give_all || Q_stricmp(name, "ammo") == 0)
//	{
//		for ( i = 0 ; i < MAX_WEAPONS ; i++ ) {
//			ent->client->ps.ammo[i] = 999;
//		}
//		if (!give_all)
//			return;
//	}
//
//	//RPG-X: RedTechie - No armor in game
//	//if (give_all || Q_stricmp(name, "armor") == 0)
//	//{
//	//	ent->client->ps.stats[STAT_ARMOR] = 200;
// 
//	//RPG-X: J2J - Well its worth a try..
//	if (give_all)
//	{
//		if (!(ent->flags & FL_EVOSUIT))
//		{
//			ent->client->ps.powerups[PW_EVOSUIT] = 0;
//		}
//		return;
//	}
//	
//
//	if ( Q_stricmp(name, ItemNames[0]) == 0)
//	{
//		ent->client->ps.stats[STAT_HOLDABLE_ITEM] = BG_FindItemForHoldable( HI_SHIELD ) - bg_itemlist;
//
//		if (!give_all)
//			return;
//	}
//	if ( Q_stricmp(name, ItemNames[1]) == 0)
//	{
//		ent->client->ps.stats[STAT_HOLDABLE_ITEM] = BG_FindItemForHoldable( HI_TRANSPORTER ) - bg_itemlist;
//
//		if (!give_all)
//			return;
//	}
//
//	if ( Q_stricmp(name, ItemNames[2]) == 0)
//	{
//		ent->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_5);
//		ent->client->ps.ammo[WP_5] = 999;
//
//		if (!give_all)
//			return;
//	}
//	if (Q_stricmp(name, ItemNames[3]) == 0)
//	{
//		ent->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_6);
//		ent->client->ps.ammo[WP_6] = 999;
//
//		if (!give_all)
//			return;
//	}
//	if (Q_stricmp(name, ItemNames[4]) == 0)
//	{
//		ent->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_1);
//		ent->client->ps.ammo[WP_1] = 999;
//
//		if (!give_all)
//			return;
//	}
//	if (Q_stricmp(name, ItemNames[5]) == 0)
//	{
//		ent->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_4);
//		ent->client->ps.ammo[WP_4] = 999;
//
//		if (!give_all)
//			return;
//	}
//	if (Q_stricmp(name, ItemNames[6]) == 0)
//	{
//		ent->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_10);
//		ent->client->ps.ammo[WP_10] = 999;
//
//		if (!give_all)
//			return;
//	}
//	if (Q_stricmp(name, ItemNames[7]) == 0)
//	{
//		ent->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_8);
//		ent->client->ps.ammo[WP_8] = 999;
//
//		if (!give_all)
//			return;
//	}
//	if (Q_stricmp(name, ItemNames[8]) == 0)
//	{
//		ent->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_7);
//		ent->client->ps.ammo[WP_7] = 999;
//
//		if (!give_all)
//			return;
//	}
//	if (Q_stricmp(name, ItemNames[9]) == 0)
//	{
//		ent->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_9);
//		ent->client->ps.ammo[WP_9] = 999;
//
//		if (!give_all)
//			return;
//	}
//	if (Q_stricmp(name, ItemNames[10]) == 0)
//	{
//		ent->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_13);
//		ent->client->ps.ammo[WP_13] = 999;
//
//		if (!give_all)
//			return;
//	}
//	if (Q_stricmp(name, ItemNames[11]) == 0)
//	{
//		ent->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_12);
//		ent->client->ps.ammo[WP_12] = 999;
//
//		if (!give_all)
//			return;
//	}
//	if (Q_stricmp(name, ItemNames[12]) == 0)
//	{
//		ent->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_14);
//		ent->client->ps.ammo[WP_14] = 999;
//
//		if (!give_all)
//			return;
//	}
//	if (Q_stricmp(name, ItemNames[13]) == 0)
//	{
//		ent->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_11);
//		ent->client->ps.ammo[WP_11] = 999;
//
//		if (!give_all)
//
//			return;
//	}
//	if (Q_stricmp(name, ItemNames[14]) == 0)
//	{
//		ent->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_2);
//		ent->client->ps.ammo[WP_2] = 999;
//
//		if (!give_all)
//			return;
//	}
//	if (Q_stricmp(name, ItemNames[15]) == 0)
//	{
//		ent->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_3);
//		ent->client->ps.ammo[WP_3] = 999;
//
//		if (!give_all)
//			return;
//	}
//	if (Q_stricmp(name, ItemNames[16]) == 0)
//	{
//		ent->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_NEUTRINO_PROBE);
//		ent->client->ps.ammo[WP_NEUTRINO_PROBE] = 999;
//
//		if (!give_all)
//			return;
//	}
//
//	if ( !give_all ) 
//	{
//		trap_SendServerCommand( ent-g_entities, va("print \"Item Not Found!\n\""));
//		return;
//	}
//
//
///* Reference only
//client->ps.stats[STAT_HOLDABLE_ITEM] = BG_FindItemForHoldable( HI_SHIELD ) - bg_itemlist;
//*/
//
//
///*	
//RPG-X: J2J - FixMe: The following code has been broken since we got the mod from steve. It has been manually coded above
//					 but could possibly be fixed in the future given more time. This is not urgent.
//
//	// spawn a specific item right on the player
//	if ( !give_all ) 
//	{
//		it = BG_FindItem (name);
//		if (!it)
//		{
//			trap_SendServerCommand( ent-g_entities, va("print \"Item Not Found r!\n\""));
//			return;
//		}
//
//		it_ent = G_Spawn();
//		VectorCopy( ent->r.currentOrigin, it_ent->s.origin );
//		it_ent->classname = it->classname;
//		G_SpawnItem (it_ent, it);
//		FinishSpawningItem(it_ent );
//		memset( &trace, 0, sizeof( trace ) );
//		Touch_Item (it_ent, ent, &trace);
//		if (it_ent->inuse) {
//			G_FreeEntity( it_ent );
//		}
//	}
//*/
//
//
//}
/*

void Cmd_Give_f (gentity_t *ent)
{
	char		*name;
	gitem_t		*it;
	int			i;
	qboolean	give_all;
	gentity_t		*it_ent;
	trace_t		trace;

	if ( !CheatsOk( ent ) ) {
		return;
	}

	name = ConcatArgs( 1 );

	if (Q_stricmp(name, "all") == 0)
		give_all = qtrue;
	else
		give_all = qfalse;

	if (give_all || Q_stricmp( name, "health") == 0)
	{
		ent->health = ent->client->ps.stats[STAT_MAX_HEALTH];
		if (!give_all)
			return;
	}

	if (give_all || Q_stricmp(name, "weapons") == 0)
	{
		ent->client->ps.stats[STAT_WEAPONS] = (1 << WP_NUM_WEAPONS) - 1 - ( 1 << WP_0 );
		if (!give_all)
			return;
	}

	if (give_all || Q_stricmp(name, "ammo") == 0)
	{
		for ( i = 0 ; i < MAX_WEAPONS ; i++ ) {
			ent->client->ps.ammo[i] = 999;
		}
		if (!give_all)
			return;
	}

	if (give_all || Q_stricmp(name, "armor") == 0)
	{
		ent->client->ps.stats[STAT_ARMOR] = 200;

		if (!give_all)
			return;
	}

	// spawn a specific item right on the player
	if ( !give_all ) {
		it = BG_FindItem (name);
		if (!it) {
			return;
		}

		it_ent = G_Spawn();
		VectorCopy( ent->r.currentOrigin, it_ent->s.origin );
		it_ent->classname = it->classname;
		G_SpawnItem (it_ent, it);
		FinishSpawningItem(it_ent );
		memset( &trace, 0, sizeof( trace ) );
		Touch_Item (it_ent, ent, &trace);
		if (it_ent->inuse) {
			G_FreeEntity( it_ent );
		}
	}
}

*/

/*
=================
Cmd_Die_f

TiM: A modification on the
suicide command, to allow custom
death messages.
Logic based roughly on that created by Spacetime
in the RPG-X forum lol.
=================
*/
/*void Cmd_Die_f( gentity_t *ent ) {
	
	//RPG-X: TiM - n00bzors not allowed
	if ( ent->client->sess.sessionClass == PC_N00B )
	{
		trap_SendServerCommand( ent-g_entities, "print \"Sorry, you're too n00bish to handle this command.\n\"" );
		return;
	}

	//Spectators can't suicide rofl
	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR || (ent->client->ps.eFlags&EF_ELIMINATED) ) {
		return;
	}

	if ( rpg_allowsuicide.integer == 0 )
	{
		return;
	}
	
	if ( lastKillTime[ent->client->ps.clientNum] > level.time - 60000 )
	{//can't flood-kill
		trap_SendServerCommand( ent->client->ps.clientNum, va("cp \"Cannot self kill for %d seconds", (lastKillTime[ent->client->ps.clientNum]-(level.time-60000))/1000 ) );
		return;
	}

	lastKillTime[ent->client->ps.clientNum] = level.time;
	ent->flags &= ~FL_GODMODE;
	
	//RPG-X: Medics revive Support for suiciding
	if(rpg_medicsrevive.integer == 1){
		ent->client->ps.stats[STAT_WEAPONS] = ( 1 << WP_0 );
		ent->client->ps.stats[STAT_HOLDABLE_ITEM] = HI_NONE;
		ent->client->ps.stats[STAT_HEALTH] = ent->health = 1;
		player_die (ent, ent, ent, 1, MOD_CUSTOM_DIE );
	}else{
		ent->client->ps.stats[STAT_HEALTH] = ent->health = 0;
		player_die (ent, ent, ent, 100000, MOD_CUSTOM_DIE );
	}

	//Output the death message
	{
		char	deathMsg[512]; //SendServerCommand can support 1024 characters... but I doubt trap_Print can display that many lol
		
		//If there were args, dump them all to the char array
		Q_strncpyz( deathMsg, ConcatArgs( 1 ), sizeof(deathMsg) );

		if( !deathMsg[0] )
			trap_SendServerCommand( -1, va( "print \"%s" S_COLOR_WHITE " died.\n\" ", ent->client->pers.netname ) );
		else
			trap_SendServerCommand( -1, va( "print \"%s" S_COLOR_WHITE " %s\n\" ", ent->client->pers.netname, deathMsg ) );
	}

	if ( rpg_kicksuiciders.integer > 0 )
	{
		ent->client->pers.suicideCount++;
		if ( ent->client->pers.suicideCount >= rpg_kicksuiciders.integer )
		{
			trap_DropClient( ent->client->ps.clientNum, "Kicked: Too many suicides");
			//trap_SendConsoleCommand( EXEC_APPEND, va("kick \"%i\"\n", ent->client->ps.clientNum ) );
		}
	}

}*/

//void SetPlayerClassCvar(gentity_t *ent)
//{
//	gclient_t *client;
//
//	client = ent->client;
//
//	switch( client->sess.sessionClass )
//	{
//	case PC_INFILTRATOR:
////		trap_Cvar_Set("ui_playerclass", "INFILTRATOR");
//		trap_SendServerCommand(ent-g_entities,"pc INFILTRATOR");  
//		break;
//	case PC_SNIPER:
////		trap_Cvar_Set("ui_playerclass", "SNIPER");
//		trap_SendServerCommand(ent-g_entities,"pc SNIPER");  
//		break;
//	case PC_HEAVY:
////		trap_Cvar_Set("ui_playerclass", "HEAVY");
//		trap_SendServerCommand(ent-g_entities,"pc HEAVY");  
//		break;
//	case PC_DEMO:
////		trap_Cvar_Set("ui_playerclass", "DEMO");
//		trap_SendServerCommand(ent-g_entities,"pc DEMO");  
//		break;
//	case PC_MEDIC:
////		trap_Cvar_Set("ui_playerclass", "MEDIC");
//		trap_SendServerCommand(ent-g_entities,"pc MEDIC");  
//		break;
//	case PC_TECH:
////		trap_Cvar_Set("ui_playerclass", "TECH");
//		trap_SendServerCommand(ent-g_entities,"pc TECH");  
//		break;
///*	case PC_ADMIN:
//		trap_SendServerCommand(ent-g_entities,"pc MAKER");  
//		break;
//	case PC_SECURITY:
//		trap_SendServerCommand(ent-g_entities,"pc SECURITY");  
//		break;
//	case PC_MEDICAL:
//		trap_SendServerCommand(ent-g_entities,"pc MEDICAL");  
//		break;
//	case PC_ALIEN:
//		trap_SendServerCommand(ent-g_entities,"pc ALIEN");  
//		break;
//	case PC_COMMAND:
//		trap_SendServerCommand(ent-g_entities,"pc COMMAND");  
//		break;
//	case PC_SCIENCE:
//		trap_SendServerCommand(ent-g_entities,"pc SCIENCE");  
//		break;
//	case PC_ENGINEER:
//		trap_SendServerCommand(ent-g_entities,"pc ENGINEER");  
//		break;
//	case PC_ALPHAOMEGA22:
//		trap_SendServerCommand(ent-g_entities,"pc ALPHAOMEGA22");  
//		break;
//	case PC_N00B:
//		trap_SendServerCommand(ent-g_entities,"pc N00B");  
//		break;*/
//	case PC_BORG:
////		trap_Cvar_Set("ui_playerclass", "BORG");
//		trap_SendServerCommand(ent-g_entities,"pc BORG");  
//		break;
//	case PC_ACTIONHERO:
////		trap_Cvar_Set("ui_playerclass", "HERO");
//		trap_SendServerCommand(ent-g_entities,"pc HERO");  
//		break;
//	case PC_NOCLASS:
////		trap_Cvar_Set("ui_playerclass", "NOCLASS");
//		trap_SendServerCommand(ent-g_entities,"pc NOCLASS");  
//		break;
//	}
//}

/*
=================
Cmd_Follow_f
=================
*/
/*static void Cmd_Follow_f( gentity_t *ent ) {
	int		i;
	char	arg[MAX_TOKEN_CHARS];
	playerState_t *ps, *ps2;

	if ( ent->r.svFlags&SVF_BOT )
	{//bots can't follow!
		return;
	}
	else
	{
		return;
	}

	if ( trap_Argc() != 2 ) {
		if ( ent->client->sess.spectatorState == SPECTATOR_FOLLOW ) {
			StopFollowing( ent );
		}
		return;
	}

	trap_Argv( 1, arg, sizeof( arg ) );
	i = ClientNumberFromString( ent, arg );
	if ( i == -1 ) {
		return;
	}

	// can't follow self
	if ( &level.clients[ i ] == ent->client ) {
		return;
	}

	// can't follow another spectator
	if ( level.clients[ i ].sess.sessionTeam == TEAM_SPECTATOR ) {
		return;
	}

	ps = &level.clients[i].ps;
	ps2 = &ent->client->ps;

//	if ( g_pModElimination.integer != 0 || g_pModAssimilation.integer != 0 )
//	{//don't do this follow stuff, it's bad!
		VectorCopy( ps->viewangles, ps->viewangles );
		VectorCopy( ps->origin, ps->origin );
		return;
//	}

	// if they are playing a tournement game, count as a loss
	if ( g_gametype.integer == GT_TOURNAMENT && ent->client->sess.sessionTeam == TEAM_FREE ) {
		ent->client->sess.losses++;
	}

	// first set them to spectator
	if ( ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
		SetTeam( ent, "spectator" );
	}

	//ent->client->sess.spectatorState = SPECTATOR_FOLLOW;
	//ent->client->sess.spectatorClient = i;
	
}*/

/*static void G_ModifyChatTextForRace(char *chatText, int race) {
	char c;
	int i, min;

	for(i = 0; i < strlen(chatText); i++) {
		c = chatText[i];
		if(!(c > 64 && c < 91) || !(c > 96 && c < 123)) {
			c = c ^ (byte)race;
			if(!(c > 64 && c < 91) || !(c > 96 && c < 123)) {
				if(c < 65) {
					min =  65 - c;
					c = c + min + (rand() % 26);
				} else if(c > 90 && c < 97) {
					if(c - 90 < 3)
						c = 90 - (rand() % 26);
					else
						c = 97 + (rand() % 26);
				} else if(c > 122)
					c = 122 - (rand() % 25);
			}
		}
		chatText[i] = c;
	}
}*/

/*static void G_LanguageMod(char *chatText, int clientNum, int targetNum) {
	gentity_t *ent = &g_entities[clientNum];
	gentity_t *target = &g_entities[targetNum];
	languageData_t *targetLang = target->client->languages;
	languageData_t *entLang = ent->client->languages;
	// check for universal translator
	if(!targetLang->hasUniversal) {
		// compare races
		if(entLang->language == targetLang->language) {
			// modify chat string to be unreadable as there are not of the same race
			G_ModifyChatTextForRace(chatText, entLang->language);
		}
	}
}*/

/*
================================
Phenix's GiveTo Function

Give an item to any player on the sevrer using ID
=================================================
*/

//void Cmd_GiveTo_f (gentity_t *ent)
//{
//	char		*name;
//	char		send[80];
////	gitem_t		*it;
//	int			i;
//	int			j;
//	qboolean	give_all;
////	gentity_t	*it_ent;
////	trace_t		trace;
//	int			targetNum;
//	gentity_t	*target;
//	gentity_t	*other;
//	char		arg[MAX_TOKEN_CHARS];
//
//	/*if ( trap_Argc () < 2 ) {
//		return;
//	}*/ //TiM: Not necessary
//
//	if ( IsAdmin( ent ) == qfalse ) {
//		return;
//	}
//
//	trap_Argv( 1, arg, sizeof( arg ) );
//
//	//TiM : User Friendliness
//	if ( !arg[0] ) { //if user added no args (ie wanted the parameters)
//		trap_SendServerCommand( ent->client->ps.clientNum, va("print \"\nUsage: The user will give a certain item to another player\nCommand: GiveTo [Player ID] <Item Name>\n\nAcceptable items include 'all', 'health', 'weapons', ammo', 'forcefield', 'transporter', 'phaser', 'crifle', 'imod', 'srifle', 'aliengun', 'admingun', 'photongun', 'dermalregen', 'hypospray', 'toolkit', 'medkit', 'tricorder', 'padd', 'neutrinoprobe'\n\" ") );
//		return;
//	}
//
//	targetNum = atoi( arg );
//	if ( targetNum < 0 || targetNum >= level.maxclients ) {
//		return;
//	}
//
//	target = &g_entities[targetNum];
//	if ( !target || !target->inuse || !target->client ) {
//		return;
//	}
//	
//	if ( target->client->ps.pm_type == PM_DEAD ) {
//		trap_SendServerCommand( ent-g_entities, va("print \"They were killed! Why would you want to give stuff to a dead person?\n\""));
//		return;
//	}
//
//	name = ConcatArgs( 2 );
//	
//	Com_sprintf (send, sizeof(send), "%s ^7gave %s ^7%s", ent->client->pers.netname, target->client->pers.netname, name);
//
//	for (j = 0; j < 1023; j++) {
//		if(g_entities[j].client){
//			other = &g_entities[j];
//			G_SayTo( ent, other, SAY_ADMIN, COLOR_CYAN, "^7Server: ", send );
//		}
//	}
//	
//	G_LogPrintf( "%s gave %s %s\n", ent->client->pers.netname, target->client->pers.netname, name );
//
//	if (Q_stricmp(name, "all") == 0)
//		give_all = qtrue;
//	else
//		give_all = qfalse;
//
//	if (give_all || Q_stricmp( name, "health") == 0)
//	{
//		//RPG-X: RedTechie - Give all bug respawns you but also screws up spawn flags
//		if((rpg_medicsrevive.integer == 1) && (ent->client->ps.pm_type == PM_DEAD)){
//			target->health = 1;
//		}else{
//			target->health = target->client->ps.stats[STAT_MAX_HEALTH];
//		}
//		if (!give_all)
//			return;
//	}
//
//	if (give_all || Q_stricmp(name, "weapons") == 0)
//	{
//		target->client->ps.stats[STAT_WEAPONS] = (1 << WP_NUM_WEAPONS) - 1 - ( 1 << WP_0 );
//		if (!give_all)
//			return;
//	}
//
//	if (give_all || Q_stricmp(name, "ammo") == 0)
//	{
//		for ( i = 0 ; i < MAX_WEAPONS ; i++ ) {
//			target->client->ps.ammo[i] = 999;
//		}
//		if (!give_all)
//			return;
//	}
//	
//	if (give_all || Q_stricmp(name, ItemNames[0]) == 0)
//	{
//		target->client->ps.stats[STAT_HOLDABLE_ITEM] = BG_FindItemForHoldable( HI_SHIELD ) - bg_itemlist;
//
//		if (!give_all)
//			return;
//	}
//	if (give_all || Q_stricmp(name, ItemNames[1]) == 0)
//	{
//		target->client->ps.stats[STAT_HOLDABLE_ITEM] = BG_FindItemForHoldable( HI_TRANSPORTER ) - bg_itemlist;
//
//		if (!give_all)
//			return;
//	}
//
//	if (give_all || Q_stricmp(name, ItemNames[2]) == 0)
//	{
//		target->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_5);
//		target->client->ps.ammo[WP_5] = -1;
//
//		if (!give_all)
//			return;
//	}
//	if (give_all || Q_stricmp(name, ItemNames[3]) == 0)
//	{
//		target->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_6);
//		target->client->ps.ammo[WP_6] = -1;
//
//		if (!give_all)
//			return;
//	}
//	if (give_all || Q_stricmp(name, ItemNames[4]) == 0)
//	{
//		target->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_1);
//		target->client->ps.ammo[WP_1] = -1;
//
//		if (!give_all)
//			return;
//	}
//	if (give_all || Q_stricmp(name, ItemNames[5]) == 0)
//	{
//		target->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_4);
//		target->client->ps.ammo[WP_4] = -1;
//
//		if (!give_all)
//			return;
//	}
//	if (give_all || Q_stricmp(name, ItemNames[6]) == 0)
//	{
//		target->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_10);
//		target->client->ps.ammo[WP_10] = -1;
//
//		if (!give_all)
//			return;
//	}
//	if (give_all || Q_stricmp(name, ItemNames[7]) == 0)
//	{
//		target->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_8);
//		target->client->ps.ammo[WP_8] = -1;
//
//		if (!give_all)
//			return;
//	}
//	if (give_all || Q_stricmp(name, ItemNames[8]) == 0)
//	{
//		target->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_7);
//		target->client->ps.ammo[WP_7] = -1;
//
//		if (!give_all)
//			return;
//	}
//	if (give_all || Q_stricmp(name, ItemNames[9]) == 0)
//	{
//		target->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_9);
//		target->client->ps.ammo[WP_9] = -1;
//
//		if (!give_all)
//			return;
//	}
//	if (give_all || Q_stricmp(name, ItemNames[10]) == 0)
//	{
//		target->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_13);
//		target->client->ps.ammo[WP_13] = -1;
//
//		if (!give_all)
//			return;
//	}
//	if (give_all || Q_stricmp(name, ItemNames[11]) == 0)
//	{
//		target->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_12);
//		target->client->ps.ammo[WP_12] = -1;
//
//		if (!give_all)
//			return;
//	}
//	if (give_all || Q_stricmp(name, ItemNames[12]) == 0)
//	{
//		target->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_14);
//		target->client->ps.ammo[WP_14] = -1;
//
//		if (!give_all)
//			return;
//	}
//	if (give_all || Q_stricmp(name, ItemNames[13]) == 0)
//	{
//		target->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_11);
//		target->client->ps.ammo[WP_11] = -1;
//
//		if (!give_all)
//
//			return;
//	}
//	if (give_all || Q_stricmp(name, ItemNames[14]) == 0)
//	{
//		target->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_2);
//		target->client->ps.ammo[WP_2] = -1;
//
//		if (!give_all)
//			return;
//	}
//	if (give_all || Q_stricmp(name, ItemNames[15]) == 0)
//	{
//		target->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_3);
//		target->client->ps.ammo[WP_3] = -1;
//
//		if (!give_all)
//			return;
//	}
//	if (give_all || Q_stricmp(name, ItemNames[16]) == 0)
//	{
//		target->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_NEUTRINO_PROBE);
//		target->client->ps.ammo[WP_NEUTRINO_PROBE] = -1;
//
//		if (!give_all)
//			return;
//	}
//
//	if ( !give_all ) 
//	{
//		trap_SendServerCommand( ent-g_entities, va("print \"Item Not Found!\n\""));
//		return;
//	}
//
//	if (!(target->flags & FL_EVOSUIT))
//	{
//		target->client->ps.powerups[PW_EVOSUIT] = 0;
//	}
//}

/*
=================
Cmd_Ani_f (used to test animations)
=================
*/
//RPG-X: J2J - What the thing above says

//extern void PM_StartTorsoAnim( int anim );
//extern void PM_StartLegsAnim( int anim );


/*void Cmd_Ani_f( gentity_t *ent)
{
	char		arg[MAX_TOKEN_CHARS];		//Arguments
	int			aninum;

	if(!ent || !ent->client)
		return;

	memset(arg,0,MAX_TOKEN_CHARS);			//Clear aray.

	//Grab range from command
	trap_Argv( 1, arg, sizeof( arg ) );			//Gets 1st

	if(!arg || Q_stricmp(arg, "") == 0)
	{
		//trap_SendServerCommand( ent->client->ps.clientNum, va("print \"Uknown Animation Type! %i %i %i %i \n\"",BOTH_DEATH1, BOTH_DEATH2, BOTH_DEATH3, BOTH_DEATH4) );
		return;
	}

	aninum = atoi(arg);
	//aninum = GetIDForString(animTable, arg);

	CurrentEmote[ent->client->ps.clientNum] = aninum;

	return;
}*/

/*
=================
Change Rank			(RPG-X: J2J & RedTechie)
=================
*/

/*
RANK VALUES:
[defined in cg_local.h]

#define 	crewman	1		
#define 	cadet1		2		
#define 	cadet2		4		
#define 	cadet3		8		
#define 	cadet4		16		
#define 	ensign		32		
#define 	ltjg			64		
#define 	lt				128	
#define 	ltcmdr		256	
#define 	cmdr			512	
#define 	cpt			1024	
#define 	cmmdr		2048	
#define 	adm2		4096	
#define 	adm3		8192	
#define  adm4		16384
#define 	adm5		32768
*/

//TiM

//Actual, Formal and Numerical values for use with comparing and correlating
//player input
/*extern char* pRank[16][3] =
{
	{"crewman", "Crewman", "1"},
	{"cadet1", "Cadet 4th Class", "2"},
	{"cadet2", "Cadet 3rd Class", "4"},
	{"cadet3", "Cadet 2nd Class", "8"},
	{"cadet4", "Cadet 1st Class", "16"},
	{"ensign", "Ensign", "32"},
	{"ltjg", "Lieutenant Junior Grade", "64"},
	{"lt", "Lieutenant", "128"},
	{"ltcmdr", "Lt. Commander", "256"},
	{"cmdr", "Commander", "512"},
	{"capt", "Captain", "1024"},
	{"cmmdr", "Commodore", "2048"},
	{"adm2", "Rear Admiral", "4096"},
	{"adm3", "Vice Admiral", "8192"},
	{"adm4", "Admiral", "16384"},
	{"adm5", "Fleet Admiral", "32768"}
};*/

//RPG-X: J2J - TiM please comment this code in future....
//RPG-X: TiM - Okay...

/*
=================================
correlateRanks
=================================
Compares the arg to the rank actual array and
if it finds a match, it returns the formal version of the same cell
(So when the promotion/demotion message is shown, the formal name for the rank
is displayed)
Addendum: Also correlates the value of the rank, so it can be used to check if the player is
trying to set the rank to the same rank they currently are. :P
*/
/*char* correlateRanks( const char* strArg, int intArg ) 
{
	int i;

	for ( i = 0; i < 16; i++ ) 
	{
		if( !strcmp( strArg, pRank[i][0] ) ) //if strArg matches one of the rank commands...
		{
			switch( intArg ) 
			{
				case 0:
					return pRank[i][1];
					break;
				case 1:
					return pRank[i][2];
					break;
			}
		}

		if ( !strcmp( strArg, pRank[i][2] ) ) //if strArg matches one of the rank flags...
		{	
			switch( intArg ) 
			{
				case 2:
					return pRank[i][0]; //return the command name
					break;
				case 3:
					return pRank[i][1]; //return the formal name
					break;
			}
		}
	}
	
	return NULL;
}*/

