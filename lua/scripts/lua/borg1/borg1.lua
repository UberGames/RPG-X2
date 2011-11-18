-- Startup
function InitGame(levelTime, randomSeed, restart)
	game.Print("Lua Level Init...");
	
	game.Print("-map_restart ...");
		game.Print("--workaround-setup ...");
			ent = entity.Spawn();
			ent:SetClassname("target_levelchange");
			ent:SatTargetname("map_restart");
			ent:SetTarget("borg1");
			entity.CallSpawn(ent);
		game.Print("--trigger-setup ...");
		if restart == 1 then
			game.ClientPrint(-1, "Please don't use map_restart, use map or devmap instead.");
			game.Print("Please don't use map_restart, use map or devmap instead.");
			ent = entity.Find("map_restart")
			entity.Use(ent)
		end
		
	game.Print("-Removing unused entities...");
		entity.Remove(entity.Find("borg_munro"));
		entity.Remove(entity.Find("borg_alexa"));
		entity.Remove(entity.Find("deathborg"));
		entity.Remove(entity.Find("deathborg1"));
		entity.Remove(entity.FindBModel(127));
		
	game.Print("-Updating and fixing Forcefields and Plasmafilters...");
		game.Print("--Forcefield Room 1");
			ent = entity.Spawn();
			ent:SetClassname("misc_model_breakable");
			ent:SetTarget("forcefield1");
			ent:SetHealth(1);  
			ent:SetSpawnflags(1); 
			ent:SetSplashDamage(75);  
			ent:SetSplashRadius(75);
			ent:SetKeyValue("material", 1);  
			ent:SetModel("models/mapobjects/borg/disnode.md3"); 
			mover.SetPosition(ent, -181, -160, 48);     
			mover.SetAngles(ent, 0, 0, 0); 			
			entity.CallSpawn(ent); 
			
			ent = entity.FindBModel(88);
			ent:SetClassname("func_forcefield");      
			entity.CallSpawn(ent); 
			
		game.Print("--Plasmafilter Room 2");
			ent = entity.Spawn(); 
			ent:SetClassname("misc_model_breakable");
			ent:SetTarget("grate3");
			ent:SetHealth(1);  
			ent:SetSpawnflags(9); 	
			ent:SetSplashDamage(250);  
			ent:SetSplashRadius(150);
			ent:SetKeyValue("material", 1);  
			ent:SetModel("models/mapobjects/borg/tank.md3");
			mover.SetPosition(ent, -707, -48, 32);      
			mover.SetAngles(ent, 0, 0, 0);			
			entity.CallSpawn(ent); 
			
			ent = entity.FindBModel(13);
			ent:SetClassname("func_breakable");  
			ent:SetSpawnflags(1);  
			ent:SetKeyValue("material", 1);      
			entity.CallSpawn(ent);  
			   
		game.Print("--Forcefield Room 2");
			ent = entity.Spawn();
			ent:SetClassname("misc_model_breakable");
			ent:SetTarget("forcefield2");
			ent:SetHealth(1);  
			ent:SetSpawnflags(1); 
			ent:SetSplashDamage(75);  
			ent:SetSplashRadius(75);
			ent:SetKeyValue("material", 1);  
			ent:SetModel("models/mapobjects/borg/disnode.md3");  
			mover.SetPosition(ent, -182, 672, 128);      
			mover.SetAngles(ent, 0, 0, 0);			
			entity.CallSpawn(ent); 
			
			ent = entity.FindBModel(87);
			ent:SetClassname("func_forcefield");      
			entity.CallSpawn(ent);  
			
		game.Print("--Plasmafilter Room 3 Corridor");
			ent = entity.Spawn();
			ent:SetClassname("misc_model_breakable");
			ent:SetTarget("grate");
			ent:SetHealth(1);  
			ent:SetSpawnflags(9); 	
			ent:SetSplashDamage(250);  
			ent:SetSplashRadius(150);
			ent:SetKeyValue("material", 1);  
			ent:SetModel("models/mapobjects/borg/tank.md3"); 
			mover.SetPosition(ent, 760, 86, 16);   
			mover.SetAngles(ent, 0, 90, 0); 			
			entity.CallSpawn(ent);
			 
			ent = entity.FindBModel(50);
			ent:SetClassname("func_breakable");  
			ent:SetSpawnflags(1);  
			ent:SetKeyValue("material", 1);        
			entity.CallSpawn(ent);
			   
		game.Print("--Plasmafilter Room 3 Ladder");
			ent = entity.Spawn();
			ent:SetClassname("misc_model_breakable");
			ent:SetTarget("grate1");
			ent:SetHealth(1);  
			ent:SetSpawnflags(9); 	
			ent:SetSplashDamage(250);  
			ent:SetSplashRadius(150);
			ent:SetKeyValue("material", 1);  
			ent:SetModel("models/mapobjects/borg/tank.md3"); 
			mover.SetPosition(ent, 453, 592, 96);      
			mover.SetAngles(ent, 0, 0, 0);
			
			entity.CallSpawn(ent); 
			ent = entity.FindBModel(40);
			ent:SetClassname("func_breakable");  
			ent:SetSpawnflags(1);  
			ent:SetKeyValue("material", 1);         
			entity.CallSpawn(ent); 
			   
	game.Print("-Final Room Firework...");
		game.Print("--Set up Plasmafilters");
			ent = entity.Spawn();
			ent:SetClassname("misc_model_breakable");
			ent:SetTarget("finalroom");
			ent:SetTargetname("blowup");
			ent:SetHealth(1);  
			ent:SetSpawnflags(9); 	
			ent:SetSplashDamage(250);  
			ent:SetSplashRadius(150);
			ent:SetKeyValue("material", 1);  
			ent:SetModel("models/mapobjects/borg/tank.md3"); 
			mover.SetPosition(ent, 479, 1727, -344);      
			mover.SetAngles(ent, 0, 180, 0);
			
			entity.CallSpawn(ent); 
			ent = entity.Spawn();
			ent:SetClassname("misc_model_breakable");
			ent:SetTarget("finalroom");
			ent:SetTargetname("blowup");
			ent:SetHealth(1);  
			ent:SetSpawnflags(9); 	
			ent:SetSplashDamage(250);  
			ent:SetSplashRadius(150);
			ent:SetKeyValue("material", 1);  
			ent:SetModel("models/mapobjects/borg/tank.md3");
			mover.SetPosition(ent, 513, 1761, -344);      
			mover.SetAngles(ent, 0, 90, 0);
			entity.CallSpawn(ent);
			 
			ent = entity.Spawn();
			ent:SetClassname("misc_model_breakable");
			ent:SetTarget("finalroom");
			ent:SetTargetname("blowup");
			ent:SetHealth(1);  
			ent:SetSpawnflags(9); 	
			ent:SetSplashDamage(250);  
			ent:SetSplashRadius(150);
			ent:SetKeyValue("material", 1);  
			ent:SetModel("models/mapobjects/borg/tank.md3");
			mover.SetPosition(ent, 545, 1727, -344);      
			mover.SetAngles(ent, 0, 0, 0);
			entity.CallSpawn(ent); 
			
			ent = entity.Spawn();
			ent:SetClassname("misc_model_breakable");
			ent:SetTarget("finalroom");
			ent:SetTargetname("blowup");
			ent:SetHealth(1);  
			ent:SetSpawnflags(9); 	
			ent:SetSplashDamage(250);  
			ent:SetSplashRadius(150);
			ent:SetKeyValue("material", 1);  
			ent:SetModel("models/mapobjects/borg/tank.md3"); 
			mover.SetPosition(ent, 513, 1695, -344);      
			mover.SetAngles(ent, 0, 270, 0);
			entity.CallSpawn(ent); 
			
		game.Print("--Convert forcefield");
			ent = entity.FindBModel(70);
			ent:SetClassname("func_forcefield"); 
			ent:SetTargetname("blowup");     
			entity.CallSpawn(ent);  
			
		game.Print("--Some Breakables");
			ent = entity.FindBModel(63);
			ent:SetClassname("func_breakable");  
			ent:SetSpawnflags(1);  
			ent:SetKeyValue("material", 1);         
			entity.CallSpawn(ent);
			  
			ent = entity.FindBModel(60);
			ent:SetClassname("func_breakable");  
			ent:SetSpawnflags(1);  
			ent:SetKeyValue("material", 1);   
			entity.CallSpawn(ent); 
			 
			ent = entity.FindBModel(157);  
			ent:SetKeyValue("material", "1"); 
			entity.CallSpawn(ent);
			  
		game.Print("--making it work");
			ent = entity.Spawn();
			ent:SetClassname("target_counter");
			ent:SetTarget("finalroomtrigger");
			ent:SetTargetname("finalroom");   
			ent:SetCount(1);   
			mover.SetPosition(ent, 0, 0, 0);     
			entity.CallSpawn(ent);
			  
			ent = entity.Spawn();
			ent:SetClassname("target_relay");
			ent:SetTarget("shutdown");
			ent:SetTargetname("finalroomtrigger");  
			ent:SetLuaUse("finalroomdrop");   
			mover.SetPosition(ent, 0, 0, 0);     
			entity.CallSpawn(ent);
			  
			ent = entity.Spawn();
			ent:SetClassname("target_relay");
			ent:SetTarget("blowup");
			ent:SetTargetname("finalroomtrigger");   
			mover.SetPosition(ent, 0, 0, 0);     
			entity.CallSpawn(ent);
			  
	game.Print("-Some Decorative Work...");
		ent = entity.Spawn();
		ent:SetClassname("misc_model_breakable");
		ent:SetSpawnflags(17);
		ent:SetModel("models/mapobjects/borg/plugin.md3"); 
		mover.SetPosition(ent, -819, 7, 64);     
		mover.SetAngles(ent, 0, 270, 0);
		entity.CallSpawn(ent); 
		  
		ent = entity.Spawn();
		ent:SetClassname("misc_model_breakable");
		ent:SetSpawnflags(17);
		ent:SetModel("models/mapobjects/borg/plugin2.md3");  
		mover.SetPosition(ent, 649, 32, 48);     
		mover.SetAngles(ent, 0, 0, 0);
		entity.CallSpawn(ent); 
		 
		ent = entity.Spawn();
		ent:SetClassname("misc_model_breakable");
		ent:SetSpawnflags(17);
		ent:SetModel("models/mapobjects/borg/plugin.md3"); 
		mover.SetPosition(ent, -245, 817, -84);     
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);  
		 
		ent = entity.Spawn();
		ent:SetClassname("misc_model_breakable");
		ent:SetSpawnflags(17);
		ent:SetModel("models/mapobjects/borg/plugin2.md3"); 
		mover.SetPosition(ent, -12, 817, -84);     
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent); 
		 
		ent = entity.Spawn();
		ent:SetClassname("misc_model_breakable");
		ent:SetSpawnflags(25); 
		ent:SetTargetname("blowup"); 
		ent:SetSplashDamage(75);  
		ent:SetSplashRadius(75);
		ent:SetKeyValue("material", 1);  
		ent:SetModel("models/mapobjects/borg/plugin.md3"); 
		mover.SetPosition(ent, 553, 1728, -256);     
		mover.SetAngles(ent, 0, 0, 0);
		entity.CallSpawn(ent);
		 
		ent = entity.Spawn();
		ent:SetClassname("misc_model_breakable");
		ent:SetSpawnflags(17);
		ent:SetModel("models/mapobjects/borg/plugin2.md3"); 
		mover.SetPosition(ent, 184, 1791, -80);     
		mover.SetAngles(ent, 0, 270, 0);
		entity.CallSpawn(ent);  
		
		ent = entity.Spawn();
		ent:SetClassname("misc_model_breakable");
		ent:SetSpawnflags(17);
		ent:SetModel("models/weapons2/imod/imod2_w.md3");
		mover.SetPosition(ent, -128, 1087, -98);     
		mover.SetAngles(ent, 0, 180, 0);
		entity.CallSpawn(ent);  
		
	game.Print("-Midway Drop prep...");
		ent = entity.Spawn();
		ent.SetupTrigger(ent, 24, 72, 104);
		ent:SetClassname("trigger_multiple");
		ent:SetTarget("midwaydroppercount");
		ent:SetWait(1);
		mover.SetPosition(ent, 216, 1088, 266); 
		entity.CallSpawn(ent); 
		
		ent = entity.Spawn();  
		ent:SetClassname("target_counter");
		ent:SetTargetname("midwaydroppercount"); 
		ent:SetTarget("midwaydropper");  
		ent:SetCount(1);   
		mover.SetPosition(ent, 0, 0, 0);     
		entity.CallSpawn(ent);
		
		ent = entity.FindBModel(41);   
		ent:SetClassname("func_static");
		ent:SetTargetname("midwaydropbrush");     
		entity.CallSpawn(ent); 
		 
		ent = entity.Spawn();
		ent:SetClassname("target_relay");
		ent:SetTargetname("midwaydropper"); 
		ent:SetTarget("xclip");  
		ent:SetLuaUse("midwaydrop");   
		mover.SetPosition(ent, 0, 0, 0);     
		entity.CallSpawn(ent);
		  
		ent = entity.Spawn();
		ent:SetClassname("target_delay");
		ent:SetTargetname("midwaydropper"); 
		ent:SetTarget("splatpanel");  
		ent:SetWait(1);   
		mover.SetPosition(ent, 0, 0, 0);     
		entity.CallSpawn(ent);
		
	game.Print("-I-Mod Room FFs and other stuff...");
		ent = entity.FindBModel(125);
		ent:SetClassname("func_forcefield");
		ent:SatTargetname("imod_ff");
		entity.CallSpawn(ent);
		
		ent = entity.FindBModel(126);
		ent:SetClassname("func_forcefield");
		ent:SatTargetname("imod_ff");
		entity.CallSpawn(ent);
		
		ent = entity.FindBModel(152);
		entity.Remove(ent);
		
		ent = entity.FindBModel(146);
		ent:SetSpawnflags(8);
		ent:SetTarget("imod_ff");
		ent:SetTargetname("imod_fu");
		ent:SetWait(1);
		ent:SetLuaUse("borgsound");
		entity.CallSpawn(ent);
		
		ent = entity.FindBModel(17);
		ent:SetSpawnflags(8);
		ent:SetTarget("imod_fu");
		ent:SetWait(1);
		ent:SetKeyValue("dmg", "3");
		entity.CallSpawn(ent);
		  
		ent = entity.FindBModel(24);
		ent:SetSpawnflags(8);
		ent:SetWait(1);    
		entity.CallSpawn(ent); 
		  
	game.Print("-The Last FF...");
		ent = entity.FindBModel(151);
		ent:SetClassname("func_usable");
		ent:SetSpawnflags(8");
		ent:SetWait(1");    
		ent:SetTarget("forcefield4");
		ent:SetLuaUse("borgsound");
		entity.CallSpawn(ent);
		
		ent = entity.FindBModel(52);
		ent:SetClassname("func_forcefield");
		entity.CallSpawn(ent);
		
	game.Print("-Transfer...");
		ent = entity.Spawn();
		ent.SetupTrigger(ent, 16, 152, 176);
		ent:SetClassname("trigger_multiple");
		ent:SetTarget("transfercount");
		ent:SetWait(1);  
		mover.SetPosition(ent, 919, 2177, -115);
		entity.CallSpawn(ent);
		
		ent = entity.Spawn();
		ent:SetClassname("target_counter");
		ent:SatTargetname("transfercount");
		ent:SetTarget("transfer");
		ent:SetCount(1");
		mover.SetPosition(ent, 0, 0, 0);
		entity.CallSpawn(ent);
		
		ent = entity.Spawn();
		ent:SetClassname("target_levelchange");
		ent:SatTargetname("transfer");
		ent:SetTarget("borg2");
		ent:SetWait(-1");
		mover.SetPosition(ent, 0, 0, 0);
		entity.CallSpawn(ent);
		
		ent = entity.Spawn();
		ent:SetClassname("target_relay");
		ent:SatTargetname("transfer");
		ent:SetTarget("forcefield4");
		mover.SetPosition(ent, 0, 0, 0);
		entity.CallSpawn(ent);
		
	game.Print("-Lift A fix...");
		ent = entity.FindBModel(147);
		ent:SetClassname("func_usable");
		ent:SetSpawnflags(8");
		ent:SetWait(1");    
		ent:SetTarget("plat1");
		ent:SetLuaUse("borgsound");
		entity.CallSpawn(ent);
		  
		ent = entity.FindBModel(148);
		ent:SetClassname("func_usable");
		ent:SetSpawnflags(8");
		ent:SetWait(1");    
		ent:SetTarget("plat1");
		ent:SetLuaUse("borgsound");
		entity.CallSpawn(ent); 
		
		ent = entity.FindBModel(91);
		ent:SetWait(-1");
		ent:SetKeyValue("angle", "-2");  
		ent:SetCount(24");        
		entity.CallSpawn(ent); 
		
	game.Print("-Lift B fix...");
		ent = entity.FindBModel(149);
		ent:SetClassname("func_usable");
		ent:SetSpawnflags(8");
		ent:SetWait(1");    
		ent:SetTarget("plat2");
		ent:SetLuaUse("borgsound");
		entity.CallSpawn(ent);
		  
		ent = entity.FindBModel(150);
		ent:SetClassname("func_usable");
		ent:SetSpawnflags(8");
		ent:SetWait(1");    
		ent:SetTarget("plat2");
		ent:SetLuaUse("borgsound");
		entity.CallSpawn(ent);
		    
		ent = entity.FindBModel(89);
		ent:SetWait(-1");
		ent:SetKeyValue("angle", "-2");   
		ent:SetCount(24");    
		entity.CallSpawn(ent); 
		
	game.Print("-Setting up spawnpoints...");
		entity.RemoveSpawns();
		
		ent = entity.Spawn();
		ent:SetClassname("info_player_deathmatch");
		mover.SetPosition(ent, 171, 1090, -103);  
		mover.SetAngles(ent, 0, 180, 0);
		entity.CallSpawn(ent);
		
		ent = entity.Spawn();
		ent:SetClassname("info_player_deathmatch");
		mover.SetPosition(ent, 143, -400, -8); 
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		
		ent = entity.Spawn();
		ent:SetClassname("info_player_deathmatch");
		mover.SetPosition(ent, 172, -400, -8); 
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		
		ent = entity.Spawn();
		ent:SetClassname("info_player_deathmatch");
		mover.SetPosition(ent, 202, -400, -8);  
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		
		ent = entity.Spawn();
		ent:SetClassname("info_player_deathmatch");
		mover.SetPosition(ent, 232, -400, -8);
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		
	game.Print("-Breakable-Mod...");
		game.Print("--func_breakable...");
			ent = entity.FindBModel(39);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			
			ent = entity.FindBModel(38);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent);
			
			ent = entity.FindBModel(123);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			
			ent = entity.FindBModel(124);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			
			ent = entity.FindBModel(138);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			
			ent = entity.FindBModel(22);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			
			ent = entity.FindBModel(82);
			ent:SetKeyValue("material", "1");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			
			ent = entity.FindBModel(28);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			
			ent = entity.FindBModel(27);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			
			ent = entity.FindBModel(26);
			ent:SetKeyValue("material", "2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			
			ent = entity.FindBModel(137);
			ent:SetKeyValue("team", "2")
			entity.CallSpawn(ent)
			
		game.Print("--misc_model_breakable...");
			game.Print("---checking for local or dedicated...");
				if not entity.FindNumber(334) == nil then
				ent = entity.FindNumber(334);
				test = ent.GetClassname(ent);
				if test == "misc_model_breakable" then
				
			game.Print("----dedicated...");
				ent = entity.FindNumber(199);
				ent:SetKeyValue("material", 1);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(200);
				ent:SetKeyValue("material", 1);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(308);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(309);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(310);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(311);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(312);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(313);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(314);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(315);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(316);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(317);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(318);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(319);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(320);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(321);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(322);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(323);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(324);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(325);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(326);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(327);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(328);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(329);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(330);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(331);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(332);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(333);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(334);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent); 
				
				ent = entity.FindNumber(400);
				ent:SetKeyValue("material", 1);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				else
				
			game.Print("----local...");
				ent = entity.FindNumber(199);
				ent:SetKeyValue("material", 1);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(200);
				ent:SetKeyValue("material", 1);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(306);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(307);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(308);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(309);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(310);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(311);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(312);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(313);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(314);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(315);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(316);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(317);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(318);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(319);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(320);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(321);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(322);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(323);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(324);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(325);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(326);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(327);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(328);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(329);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(330);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(331);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(332);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(392);
				ent:SetKeyValue("material", 1);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);;
				entity.CallSpawn(ent);
				end
				else
				
			game.Print("----local...");
				ent = entity.FindNumber(199);
				ent:SetKeyValue("material", 1);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(200);
				ent:SetKeyValue("material", 1);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(306);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(307);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(308);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(309);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(310);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(311);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(312);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(313);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(314);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(315);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(316);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(317);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(318);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(319);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(320);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(321);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(322);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(323);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(324);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(325);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(326);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(327);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(328);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(329);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(330);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(331);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(332);
				ent:SetKeyValue("material", 2);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);
				entity.CallSpawn(ent);
				
				ent = entity.FindNumber(392);
				ent:SetKeyValue("material", 1);
				ent:SetSplashDamage(75);
				ent:SetSplashRadius(75);;
				entity.CallSpawn(ent);
				end
game.Print("...Done"); 	
end     

-- Functions

function finalroomdrop(ent, other, activator) 
	ent = entity.FindBModel(72);
	mover.ToAngles(ent, 45, 90, 0, 20) 
	mover.ToPosition(ent, 400, 496, 1728, -912)  
	ent:SetLuaReached("midwaydropH");
	ent:SetLuaReachedAngular("midwaydropHA");   
	ent = entity.Spawn();
	ent:SetClassname("misc_model_breakable");
	ent:SetSpawnflags(17); 
	ent:SetModel("models/mapobjects/borg/plugin2_d1.md3"); 
	mover.SetPosition(ent, 553, 1728, -256);     
	mover.SetAngles(ent, 0, 0, 0);
	entity.CallSpawn(ent); 
end

function midwaydrop(ent, other, activator) 
	ent:SetLuaReached("luaReached", "midwaydrop2");
	mover.ToPosition(ent, 175, 96, 1088, 288)
end

function midwaydrop2(ent) 
	mover.Halt(ent);
	ent:SetLuaReached("midwaydropH");
	ent:SetLuaReachedAngular("midwaydropHA");
	mover.ToAngles(ent, 60, -90, 0, 0) 
	mover.ToPosition(ent, 175, 32, 1088, 90)
end

function midwaydropH(ent) 
	mover.Halt(ent);      
	ent:SetLuaReached("null");
end

function midwaydropHA(ent) 
	mover.HaltAngles(ent);      
	ent:SetLuaReachedAngular("null");
end
	
--Sounds

function borgsound(ent, other, activator)
    sound.PlaySound(ent, "sound/movers/switches/borglcarswitch.mp3", 0);
end