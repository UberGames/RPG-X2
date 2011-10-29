-- Startup
function InitGame(levelTime, randomSeed, restart)
	game.Print("Lua Level Init...");
	game.Print("-map_restart ...");
		game.Print("--workaround-setup ...");
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_levelchange");
			ent:SetKeyValue("targetname", "map_restart");
			ent:SetKeyValue("target", "dn6");
			entity.CallSpawn(ent);
		game.Print("--trigger-setup ...");
		if restart == 1 then
			game.ClientPrint(-1, "Please don't use map_restart, use map or devmap instead.");
			game.Print("Please don't use map_restart, use map or devmap instead.");
			ent = entity.Find("map_restart")
			entity.Use(ent)
		end
	game.Print("-Setting up Plasma Stream...");
		game.Print("--usable-fix...");
			ent = entity.FindBModel(143);
			ent:SetKeyValue("team", "2");
			entity.CallSpawn(ent); 
			ent = entity.FindBModel(9);
			ent:SetKeyValue("team", "2");
			entity.CallSpawn(ent); 
		game.Print("--plasma-replacement...");
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_steam");
			ent:SetKeyValue("targetname", "plasma_jets");
			ent:SetKeyValue("target", "t8");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 3576, 1360, -56);
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_steam");
			ent:SetKeyValue("targetname", "plasma_jets");
			ent:SetKeyValue("target", "t9");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 3496, 1360, -56);
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_steam");
			ent:SetKeyValue("targetname", "plasma_jets");
			ent:SetKeyValue("target", "t10");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 3536, 1360, -56); 
		game.Print("--relay...");
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_relay");
			ent:SetKeyValue("targetname", "plasma");
			ent:SetKeyValue("target", "plasma_jets");
			ent:SetKeyValue("luaUse", "clearplasma");
			entity.CallSpawn(ent); 
	game.Print("-plasma tube A..."); 
		game.Print("--usable snap...");
			ent = entity.FindBModel(48);
			ent:SetKeyValue("team", "2");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(49);
			ent:SetKeyValue("team", "2");
			entity.CallSpawn(ent);  
		game.Print("--visuals...");
			ent = entity.FindBModel(50);
			ent:SetKeyValue("material", "1");
			entity.CallSpawn(ent);  
			ent = entity.FindBModel(51);
			ent:SetKeyValue("targetname", "glass1a");
			ent:SetKeyValue("target", "glass1b");
			ent:SetKeyValue("material", "2");
			entity.CallSpawn(ent); 
			ent = entity.FindBModel(52);
			ent:SetKeyValue("targetname", "glass1b"); 
			ent:SetKeyValue("target", "glass1a");
			ent:SetKeyValue("material", "2");
			entity.CallSpawn(ent);
		game.Print("--background..."); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_relay");
			ent:SetKeyValue("target", "explodo1count");
			ent:SetKeyValue("targetname", "glass1a");
			entity.CallSpawn(ent); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_relay");
			ent:SetKeyValue("target", "explodo1count");
			ent:SetKeyValue("targetname", "glass1b");
			entity.CallSpawn(ent);
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_counter");
			ent:SetKeyValue("targetname", "explodo1count");
			ent:SetKeyValue("target", "explodo1");
			ent:SetKeyValue("count", "1");
			entity.CallSpawn(ent);
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_counter");
			ent:SetKeyValue("targetname", "explodo1count");
			ent:SetKeyValue("target", "explodo2");
			ent:SetKeyValue("count", "1");
			entity.CallSpawn(ent); 
		game.Print("--explosives...");  
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "info_notnull");
			ent:SetKeyValue("targetname", "fxtgta1");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1984, 1384, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_surface_explosion");
			ent:SetKeyValue("targetname", "explodo1");
			ent:SetKeyValue("target", "fxtgta1");
			ent:SetKeyValue("spawnflags", "2");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 2040, 1384, -128);  
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "info_notnull");
			ent:SetKeyValue("targetname", "fxtgta2");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1984, 1416, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_surface_explosion");
			ent:SetKeyValue("targetname", "explodo1");
			ent:SetKeyValue("target", "fxtgta2");
			ent:SetKeyValue("spawnflags", "2");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 2040, 1416, -128);   
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "info_notnull");
			ent:SetKeyValue("targetname", "fxtgta3");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1984, 1448, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_surface_explosion");
			ent:SetKeyValue("targetname", "explodo1");
			ent:SetKeyValue("target", "fxtgta3");
			ent:SetKeyValue("spawnflags", "2");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 2040, 1448, -128);  
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "info_notnull");
			ent:SetKeyValue("targetname", "fxtgta4");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1984, 1480, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_surface_explosion");
			ent:SetKeyValue("targetname", "explodo1");
			ent:SetKeyValue("target", "fxtgta4");
			ent:SetKeyValue("spawnflags", "2");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 2040, 1480, -128);  
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "info_notnull");
			ent:SetKeyValue("targetname", "fxtgta5");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1984, 1512, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_surface_explosion");
			ent:SetKeyValue("targetname", "explodo1");
			ent:SetKeyValue("target", "fxtgta5");
			ent:SetKeyValue("spawnflags", "2");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 2040, 1512, -128);  
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "info_notnull");
			ent:SetKeyValue("targetname", "fxtgta6");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1984, 1544, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_surface_explosion");
			ent:SetKeyValue("targetname", "explodo1");
			ent:SetKeyValue("target", "fxtgta6");
			ent:SetKeyValue("spawnflags", "2");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 2040, 1544, -128);  
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "info_notnull");
			ent:SetKeyValue("targetname", "fxtgta7");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1984, 1576, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_surface_explosion");
			ent:SetKeyValue("targetname", "explodo1");
			ent:SetKeyValue("target", "fxtgta7");
			ent:SetKeyValue("spawnflags", "2");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 2040, 1576, -128);  
	game.Print("-plasma tube B...");
		game.Print("--usable snap...");
			ent = entity.FindBModel(48);
			ent:SetKeyValue("team", "2");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(49);
			ent:SetKeyValue("team", "2");
			entity.CallSpawn(ent);
		game.Print("--visuals...");
			ent = entity.FindBModel(44);
			ent:SetKeyValue("material", "1");
			entity.CallSpawn(ent);  
			ent = entity.FindBModel(45);
			ent:SetKeyValue("targetname", "glass3a");
			ent:SetKeyValue("target", "glass3b");
			ent:SetKeyValue("material", "2");
			entity.CallSpawn(ent); 
			ent = entity.FindBModel(46);
			ent:SetKeyValue("targetname", "glass3b"); 
			ent:SetKeyValue("target", "glass3a");
			ent:SetKeyValue("material", "2");
			entity.CallSpawn(ent);
		game.Print("--background..."); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_relay");
			ent:SetKeyValue("target", "explodo3count");
			ent:SetKeyValue("targetname", "glass3a");
			entity.CallSpawn(ent); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_relay");
			ent:SetKeyValue("target", "explodo3count");
			ent:SetKeyValue("targetname", "glass3b");
			entity.CallSpawn(ent);
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_counter");
			ent:SetKeyValue("targetname", "explodo3count");
			ent:SetKeyValue("target", "explodo3");
			ent:SetKeyValue("count", "1");
			entity.CallSpawn(ent); 
			ent:SetKeyValue("classname", "target_counter");
			ent:SetKeyValue("targetname", "explodo3count");
			ent:SetKeyValue("target", "explodo4");
			ent:SetKeyValue("count", "1");
			entity.CallSpawn(ent); 
		game.Print("--explosives...");  
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "info_notnull");
			ent:SetKeyValue("targetname", "fxtgtb1");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1592, 1872, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_surface_explosion");
			ent:SetKeyValue("targetname", "explodo4");
			ent:SetKeyValue("target", "fxtgtb1");
			ent:SetKeyValue("spawnflags", "2");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1592, 1928, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "info_notnull");
			ent:SetKeyValue("targetname", "fxtgtb2");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1624, 1872, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_surface_explosion");
			ent:SetKeyValue("targetname", "explodo4");
			ent:SetKeyValue("target", "fxtgtb2");
			ent:SetKeyValue("spawnflags", "2");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1624, 1928, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "info_notnull");
			ent:SetKeyValue("targetname", "fxtgtb3");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1656, 1872, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_surface_explosion");
			ent:SetKeyValue("targetname", "explodo4");
			ent:SetKeyValue("target", "fxtgtb3");
			ent:SetKeyValue("spawnflags", "2");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1656, 1928, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "info_notnull");
			ent:SetKeyValue("targetname", "fxtgtb4");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1688, 1872, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_surface_explosion");
			ent:SetKeyValue("targetname", "explodo4");
			ent:SetKeyValue("target", "fxtgtb4");
			ent:SetKeyValue("spawnflags", "2");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1688, 1928, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "info_notnull");
			ent:SetKeyValue("targetname", "fxtgtb5");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1720, 1872, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_surface_explosion");
			ent:SetKeyValue("targetname", "explodo4");
			ent:SetKeyValue("target", "fxtgtb5");
			ent:SetKeyValue("spawnflags", "2");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1720, 1928, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "info_notnull");
			ent:SetKeyValue("targetname", "fxtgtb6");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1752, 1872, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_surface_explosion");
			ent:SetKeyValue("targetname", "explodo4");
			ent:SetKeyValue("target", "fxtgtb6");
			ent:SetKeyValue("spawnflags", "2");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1752, 1928, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "info_notnull");
			ent:SetKeyValue("targetname", "fxtgtb7");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1784, 1872, -128); 
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "fx_surface_explosion");
			ent:SetKeyValue("targetname", "explodo4");
			ent:SetKeyValue("target", "fxtgtb7");
			ent:SetKeyValue("spawnflags", "2");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 1784, 1928, -128); 
			
	--[[game.Print("-Setting up spawnpoints...");
		entity.RemoveSpawns();
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, -1512, 864, 8);
		mover.SetAngles(ent, 0, 270, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, -1544, 896, 8);
		mover.SetAngles(ent, 0, 270, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, -1544, 832, 8);
		mover.SetAngles(ent, 0, 270, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, -1480, 896, 8);
		mover.SetAngles(ent, 0, 270, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, -1480, 832, 8);
		mover.SetAngles(ent, 0, 270, 0);
		entity.CallSpawn(ent);]]
game.Print("...Done");
end  

-- Plasma vent

function clearplasma(ent, other, activator)
    entity.Remove(entity.Find("t11"));
    entity.Remove(entity.Find("t11"));
end

function bridgecrash(ent, other, activator)
    entity.Use(entity.FindBModel(8));
    entity.Use(entity.FindBModel(23));
	mover.ToAngles(entity.FindBModel(24), 30, 21, 0, 0)
end

-- lift

function liftup(ent)
	ent = entity.FindBModel(5);
	mover.ToPosition(ent, 75, -2584, -352, 1024);
    sound.PlaySound(ent, "sound/movers/doors/largedoorstart.mp3", 0);
end

function liftdown(ent)
	ent = entity.FindBModel(5);
	mover.ToPosition(ent, 75, -2584, -352, 228);
    sound.PlaySound(ent, "sound/movers/doors/largedoorstart.mp3", 0);
end

function sound82(ent, other, activator)
    sound.PlaySound(entity.FindBModel(82), "sound/ambience/dreadnaught/dnlcarpattern1.wav", 0);
end

function sound84(ent, other, activator)
    sound.PlaySound(entity.FindBModel(84), "sound/ambience/dreadnaught/dnlcarpattern1.wav", 0);
end

function liftreachedsound(ent, other, activator)
    sound.PlaySound(entity.FindBModel(5), "sound/movers/doors/largedoorstop.mp3", 0);
end

-- transfer

function transferstart(ent)
	ent = entity.FindBModel(13);
	ent:SetKeyValue("classname", "func_static");
	mover.ToPosition(ent, 10, -768, -480, 1012);
    sound.PlaySound(ent, "sound/movers/doors/largedoorstart.mp3", 0);
end

function transfermove(ent)
	ent = entity.FindBModel(13);
	mover.Halt(ent);
    sound.PlaySound(entity.FindBModel(5), "sound/movers/doors/largedoorstop.mp3", 0);
	mover.ToPosition(ent, 125, -768, 1696, 1012);
    sound.PlaySound(ent, "sound/movers/doors/largedoorstart.mp3", 0);
	ent = entity.FindBModel(76);
	mover.ToPosition(ent, 125, -568, 1820, 1144);
    sound.PlaySound(ent, "sound/movers/doors/largedoorstart.mp3", 0);
end