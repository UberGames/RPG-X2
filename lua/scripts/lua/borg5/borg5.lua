-- Startup
function InitGame(levelTime, randomSeed, restart)
	game.Print("Lua Level Init...");
	game.Print("-map_restart ...");
		game.Print("--workaround-setup ...");
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_levelchange");
			ent:SetKeyValue("targetname", "map_restart");
			ent:SetKeyValue("target", "borg5");
			entity.CallSpawn(ent);
		game.Print("--trigger-setup ...");
		if restart == 1 then
			game.ClientPrint(-1, "Please don't use map_restart, use map or devmap instead.");
			game.Print("Please don't use map_restart, use map or devmap instead.");
			ent = entity.Find("map_restart")
			entity.Use(ent)
		end
	game.Print("-Updating and fixing Forcefields and Plasmafilters...");
		game.Print("--Forcefield Corridor 2");
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "misc_model_breakable");
			ent:SetKeyValue("target", "disfield");
			ent:SetKeyValue("health", "1");  
			ent:SetKeyValue("spawnflags", "1"); 
			ent:SetKeyValue("splashDamage", "75");  
			ent:SetKeyValue("splashRadius", "75");
			ent:SetKeyValue("material", "1");  
			ent:SetKeyValue("model", "models/mapobjects/borg/disnode.md3"); 
			mover.SetPosition(ent, -184, 210, 190);     
			mover.SetAngles(ent, 90, 90, 90);
			entity.CallSpawn(ent); 
			ent = entity.FindBModel(78);
			ent:SetKeyValue("classname", "func_forcefield");      
			entity.CallSpawn(ent); 
		game.Print("--Plasmafilter Mid Level");
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "misc_model_breakable");
			ent:SetKeyValue("target", "grate3");
			ent:SetKeyValue("health", "1");     
			ent:SetKeyValue("spawnflags", "9");  
			ent:SetKeyValue("splashDamage", "250");  
			ent:SetKeyValue("splashRadius", "150");
			ent:SetKeyValue("material", "1");    
			ent:SetKeyValue("model", "models/mapobjects/borg/tank.md3"); 
			mover.SetPosition(ent, 44, -74, 178);      
			mover.SetAngles(ent, 0, 180, 0);
			entity.CallSpawn(ent); 
			ent = entity.FindBModel(1);
			ent:SetKeyValue("classname", "func_breakable");  
			ent:SetKeyValue("spawnflags", "1");  
			ent:SetKeyValue("material", "1");      
			entity.CallSpawn(ent);    
		game.Print("--Forcefield end");;
			ent = entity.FindBModel(11);
			ent:SetKeyValue("spawnflags", "8");
			ent:SetKeyValue("wait", "1");    
			ent:SetKeyValue("target", "exitfield");
			ent:SetKeyValue("luaUse", "sound11");
			entity.CallSpawn(ent);  
			ent = entity.FindBModel(29);
			ent:SetKeyValue("classname", "func_forcefield");      
			entity.CallSpawn(ent); 
	game.Print("-Some Decorative Work...");
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin.md3"); 
		mover.SetPosition(ent, -667, -830, 455);     
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);   
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin2.md3"); 
		mover.SetPosition(ent, 169, -255, 455);     
		mover.SetAngles(ent, 0, 0, 0);
		entity.CallSpawn(ent); 
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin.md3"); 
		mover.SetPosition(ent, 441, -113, 455);     
		mover.SetAngles(ent, 0, 270, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin.md3"); 
		mover.SetPosition(ent, 485, -592, 455);     
		mover.SetAngles(ent, 0, 180, 0);
		entity.CallSpawn(ent)
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin.md3"); 
		mover.SetPosition(ent, 193, -202, 295);     
		mover.SetAngles(ent, 0, 315, 0);
		entity.CallSpawn(ent);  
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin2.md3"); 
		mover.SetPosition(ent, -671, 462, 213);     
		mover.SetAngles(ent, 0, 0, 0);
		entity.CallSpawn(ent); 
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin.md3"); 
		mover.SetPosition(ent, -548, 439, 244);     
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent); 
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "misc_model_breakable");
		ent:SetKeyValue("spawnflags", "17");
		ent:SetKeyValue("model", "models/mapobjects/borg/plugin2.md3"); 
		mover.SetPosition(ent, 135, 1053, 7);     
		mover.SetAngles(ent, 0, 180, 0);
		entity.CallSpawn(ent); 
	game.Print("-Transfer...");
		ent = entity.Spawn();
		ent.SetupTrigger(ent, 128, 16, 136);
		ent:SetKeyValue("classname", "trigger_multiple");
		ent:SetKeyValue("target", "transfercount");
		ent:SetKeyValue("wait", 1);
		entity.CallSpawn(ent);
		mover.SetPosition(ent, -192, 1364, 36);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "target_counter");
		ent:SetKeyValue("targetname", "transfercount");
		ent:SetKeyValue("target", "transfer");
		ent:SetKeyValue("count", "1");
		mover.SetPosition(ent, 0, 0, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "target_levelchange");
		ent:SetKeyValue("targetname", "transfer");
		ent:SetKeyValue("target", "borg6");
		ent:SetKeyValue("wait", "-1");
		mover.SetPosition(ent, 0, 0, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "target_relay");
		ent:SetKeyValue("targetname", "transfer");
		ent:SetKeyValue("target", "exitfield");
		mover.SetPosition(ent, 0, 0, 0);
		entity.CallSpawn(ent); 
	game.Print("-Lift A fix...");
		ent = entity.FindBModel(71);
		ent:SetKeyValue("classname", "func_usable");
		ent:SetKeyValue("spawnflags", "8");
		ent:SetKeyValue("wait", "1");    
		ent:SetKeyValue("target", "plat1");
		ent:SetKeyValue("luaUse", "sound71");
		entity.CallSpawn(ent);  
		ent = entity.FindBModel(72);
		ent:SetKeyValue("classname", "func_usable");
		ent:SetKeyValue("spawnflags", "8");
		ent:SetKeyValue("wait", "1");    
		ent:SetKeyValue("target", "plat1"); 
		ent:SetKeyValue("luaUse", "sound72");
		entity.CallSpawn(ent); 
		ent = entity.FindBModel(7); 
		ent:SetKeyValue("classname", "func_door");
		ent:SetKeyValue("targetname", "plat1");
		ent:SetKeyValue("wait", "-1");
		ent:SetKeyValue("angle", "-2");  
		ent:SetKeyValue("count", "24");        
		entity.CallSpawn(ent); 
	game.Print("-Lift B fix...");
		ent = entity.FindBModel(73);
		ent:SetKeyValue("classname", "func_usable");
		ent:SetKeyValue("spawnflags", "8");
		ent:SetKeyValue("wait", "1");    
		ent:SetKeyValue("target", "plat2");
		ent:SetKeyValue("luaUse", "sound73");
		entity.CallSpawn(ent);  
		ent = entity.FindBModel(74);
		ent:SetKeyValue("classname", "func_usable");
		ent:SetKeyValue("spawnflags", "8");
		ent:SetKeyValue("wait", "1");    
		ent:SetKeyValue("target", "plat2");
		ent:SetKeyValue("luaUse", "sound74");
		entity.CallSpawn(ent);    
		ent = entity.FindBModel(6);
		ent:SetKeyValue("classname", "func_door");
		ent:SetKeyValue("targetname", "plat2");
		ent:SetKeyValue("wait", "-1");
		ent:SetKeyValue("angle", "-2");   
		ent:SetKeyValue("count", "14");    
		entity.CallSpawn(ent);
	game.Print("-Setting up spawnpoints and surrounding...");  
		ent = entity.FindBModel(9);
		ent:SetKeyValue("classname", "func_static"); 
		mover.SetPosition(ent, -896, -968, 412);   
		entity.CallSpawn(ent);    
		entity.RemoveSpawns();
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, -800, -928, 444);  
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, -832, -896, 444); 
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, -832, -960, 444); 
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, -768, -896, 444);  
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, -768, -960, 444);
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
	game.Print("-Breakable-Mod..."); 
		game.Print("--func_breakable...");
			ent = entity.FindBModel(52);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(51);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(55);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(54);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(65);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(66);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(15);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(16);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(61);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(60);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			ent = entity.FindBModel(58);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(57);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
		game.Print("--misc_model_breakable...");
			game.Print("---checking for local or dedicated..."); 
				if not entity.FindNumber(242) == nil then
				ent = entity.FindNumber(242);
				test = ent.GetClassname(ent);
				if test == "misc_model_breakable" then
			game.Print("----dedicated...");
				ent = entity.FindNumber(191);
				ent:SetKeyValue("material", "1");
				ent:SetKeyValue("health", "1");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(193);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(194);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(195);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(196);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(197);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(199);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(200);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(201);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(205);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(206);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(207);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(232);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(208);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(209);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(210);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(211);
				ent:SetKeyValue("material", "1");
				ent:SetKeyValue("health", "1");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(212);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(242);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				else
			game.Print("----local...");
				ent = entity.FindNumber(186);
				ent:SetKeyValue("material", "1");
				ent:SetKeyValue("health", "1");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(188);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(189);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(190);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(191);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(192);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(194);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(195);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(196);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(200);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(201);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(202);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(203);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(204);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(205);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(206);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(207);
				ent:SetKeyValue("material", "1");
				ent:SetKeyValue("health", "1");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(237);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(242);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent)
				end
				else
			game.Print("----local..."); 
				ent = entity.FindNumber(186);
				ent:SetKeyValue("material", "1");
				ent:SetKeyValue("health", "1");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(188);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(189);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(190);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(191);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(192);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(194);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(195);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(196);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(200);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(201);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(202);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(203);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(204);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(205);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(206);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(207);
				ent:SetKeyValue("material", "1"); 
				ent:SetKeyValue("health", "1");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(237);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent);
				ent = entity.FindNumber(242);
				ent:SetKeyValue("material", "2");
				ent:SetKeyValue("splashDamage", "75");
				ent:SetKeyValue("splashRadius", "75");
				entity.CallSpawn(ent)
				end
game.Print("...Done"); 	
end   

--Sounds

function sound11(ent, other, activator)
    sound.PlaySound(entity.FindBModel(11), "sound/movers/switches/borglcarswitch.mp3", 0);
end   

function sound71(ent, other, activator)
    sound.PlaySound(entity.FindBModel(71), "sound/movers/switches/borglcarswitch.mp3", 0);
end 

function sound72(ent, other, activator)
    sound.PlaySound(entity.FindBModel(72), "sound/movers/switches/borglcarswitch.mp3", 0);
end

function sound73(ent, other, activator)
    sound.PlaySound(entity.FindBModel(73), "sound/movers/switches/borglcarswitch.mp3", 0);
end

function sound74(ent, other, activator)
    sound.PlaySound(entity.FindBModel(74), "sound/movers/switches/borglcarswitch.mp3", 0);
end   