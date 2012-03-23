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

void INeedAHero( void )
{
	//G_RandomActionHero( actionHeroClientNum );
	if ( actionHeroClientNum >= 0 && actionHeroClientNum < level.maxclients )
	{// get and distribute relevent paramters
		ClientUserinfoChanged( actionHeroClientNum );
		ClientSpawn( &g_entities[actionHeroClientNum], 0, qfalse );//RPG-X: RedTechie - Modifyed
	}//else ERROR!!!
}

/**
*	Check wheter to replace the current action hero
*/
void G_CheckReplaceActionHero( int clientNum )
{
	if ( clientNum == actionHeroClientNum )
	{
		//G_RandomActionHero( clientNum );
		if ( actionHeroClientNum >= 0 && actionHeroClientNum < level.maxclients )
		{
			// get and distribute relevent paramters
			ClientUserinfoChanged( actionHeroClientNum );
			ClientSpawn( &g_entities[actionHeroClientNum], 0, qfalse );//RPG-X: RedTechie - Modifyed
		}//else ERROR!!!
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
