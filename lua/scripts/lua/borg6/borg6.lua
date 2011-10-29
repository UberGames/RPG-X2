-- Startup
function InitGame(levelTime, randomSeed, restart)
	game.Print("Lua Level Init...");
	game.Print("-map_restart ...");
		game.Print("--workaround-setup ...");
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_levelchange");
			ent:SetKeyValue("targetname", "map_restart");
			ent:SetKeyValue("target", "borg6");
			entity.CallSpawn(ent);
		game.Print("--trigger-setup ...");
		if restart == 1 then
			game.ClientPrint(-1, "Please don't use map_restart, use map or devmap instead.");
			game.Print("Please don't use map_restart, use map or devmap instead.");
			ent = entity.Find("map_restart")
			entity.Use(ent)
		end
	game.Print("-Some Decorative Work...");
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin.md3"); 
		mover.SetPosition(ent, 3483, 1857, 31);     
		mover.SetAngles(ent, 0, 180, 0);
		entity.CallSpawn(ent);   
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin2.md3"); 
		mover.SetPosition(ent, 2792, 1712, 20);     
		mover.SetAngles(ent, 0, 270, 0);
		entity.CallSpawn(ent);  
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin.md3"); 
		mover.SetPosition(ent, 2679, 1417, 20);     
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin2.md3"); 
		mover.SetPosition(ent, 2147, 737, 14);     
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent); 
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin.md3"); 
		mover.SetPosition(ent, 590, 191, 15);     
		mover.SetAngles(ent, 0, 135, 0);
		entity.CallSpawn(ent);  
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin2.md3"); 
		mover.SetPosition(ent, 925, 1260, 12);     
		mover.SetAngles(ent, 0, 180, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin.md3"); 
		mover.SetPosition(ent, 231, 1693, 16);     
		mover.SetAngles(ent, 0, 0, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin2.md3"); 
		mover.SetPosition(ent, 525, 115, 259);     
		mover.SetAngles(ent, 0, 270, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin.md3"); 
		mover.SetPosition(ent, 219, 133, 259);     
		mover.SetAngles(ent, 0, 270, 0);
		entity.CallSpawn(ent);
	game.Print("-Lift fix...");
		ent = entity.FindBModel(95);
		ent:SetKeyValue("classname", "func_usable");
		ent:SetKeyValue("spawnflags", "8");
		ent:SetKeyValue("wait", "1");    
		ent:SetKeyValue("target", "elevator"); 
		ent:SetKeyValue("luaUse", "sound95");
		entity.CallSpawn(ent);  
		ent = entity.FindBModel(15);
		ent:SetKeyValue("classname", "func_usable");
		ent:SetKeyValue("spawnflags", "8");
		ent:SetKeyValue("wait", "1");    
		ent:SetKeyValue("target", "elevator");
		ent:SetKeyValue("luaUse", "sound15");
		entity.CallSpawn(ent); 
		ent = entity.FindBModel(41); 
		ent:SetKeyValue("classname", "func_door");
		ent:SetKeyValue("wait", "-1");
		ent:SetKeyValue("angle", "-1");  
		ent:SetKeyValue("count", "-226");  
		ent:SetKeyValue("speed", "75");        
		entity.CallSpawn(ent);  
	game.Print("-the final door...");
		game.Print("--set up door and trigger ...");
			ent = entity.Spawn();
			ent.SetupTrigger(ent, 160, 100, 160);
			ent:SetKeyValue("classname", "trigger_multiple");
			ent:SetKeyValue("target", "enddoortrigger");
			ent:SetKeyValue("wait", "1");
			entity.CallSpawn(ent);
			mover.SetPosition(ent, 360, 224, 272);
			ent = entity.FindBModel(4);
			ent:SetKeyValue("targetname", "lastdoor");
			ent:SetKeyValue("wait", "-1");
			ent:SetKeyValue("angle", "-1");
			ent:SetKeyValue("count", "8");
			ent:SetKeyValue("speed", "75");
			ent:SetKeyValue("spawnflags", "0");
		game.Print("--set up delays ...");
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_delay");
			ent:SetKeyValue("targetname", "jeffriesstuff");
			ent:SetKeyValue("target", "jeffriesswap");
			ent:SetKeyValue("spawnflags", "1");
			ent:SetKeyValue("wait", "11");
			entity.CallSpawn(ent);
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_delay");
			ent:SetKeyValue("targetname", "jeffriesstuff");
			ent:SetKeyValue("target", "lastdoor");
			ent:SetKeyValue("spawnflags", "1");
			ent:SetKeyValue("wait", "10");
			entity.CallSpawn(ent);
		game.Print("--set up boolean ...");
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_boolean");
			ent:SetKeyValue("falsetarget", "jeffriesstuff");
			ent:SetKeyValue("swapname", "jeffriesswap");
			ent:SetKeyValue("targetname", "enddoortrigger");
			entity.CallSpawn(ent);
		game.Print("--set up relays...");
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_relay");
			ent:SetKeyValue("targetname", "jeffriesstuff");
			ent:SetKeyValue("target", "lastdoor");
			entity.CallSpawn(ent);
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_relay");
			ent:SetKeyValue("targetname", "jeffriesstuff");
			ent:SetKeyValue("target", "jeffriesswap");
			entity.CallSpawn(ent);
	game.Print("-Setting up spawnpoints...");
		entity.RemoveSpawns();
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, 3648, 1872, -8);  
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, 3616, 1904, -8); 
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, 3616, -1840, -8); 
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, 3680, 1904, -8);  
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, 3680, 1840, -8);
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
	game.Print("-Jeffries-access...");
		ent = entity.FindBModel(9);  
		entity.Remove(ent);
		ent = entity.FindBModel(10);  
		entity.Remove(ent);
		ent = entity.FindBModel(11); 
		entity.Remove(ent);  
	game.Print("-Breakable-Mod...");
		game.Print("--func_breakable...");
			ent = entity.FindBModel(59);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(58);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(86);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(85);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(77);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(76);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(68);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(67);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(34);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(33);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(23);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(19);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(26);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(22);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(25);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(21);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(24);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(20);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(62);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(61);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(89);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(88);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(83);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(82);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(71);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(70);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(92);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(91);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(31);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(30);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(80);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(79);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(74);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(73);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(65);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(64);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(28);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(27);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
		game.Print("--misc_model_breakable...");
			game.Print("---checking for local or dedicated..."); 
				if not entity.FindNumber(514) == nil then
				ent = entity.FindNumber(514);
				test = ent.GetClassname(ent);
				if test == "misc_model_breakable" then
			game.Print("----dedicated...");
				ent = entity.FindNumber(229);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(310);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(431);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(432);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(493);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(513);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(514);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				else
			game.Print("----local...");
				ent = entity.FindNumber(225);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(287);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(408);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(409);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(463);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(483);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(484);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				end
				else
			game.Print("----local...");
				ent = entity.FindNumber(225);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(287);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(408);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(409);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(463);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(483);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(484);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				end
game.Print("...Done"); 	
end   

--Sounds 

function sound15(ent, other, activator)
    sound.PlaySound(entity.FindBModel(15), "sound/movers/switches/borglcarswitch.mp3", 0);
end 

function sound95(ent, other, activator)
    sound.PlaySound(entity.FindBModel(95), "sound/movers/switches/borglcarswitch.mp3", 0);
end       