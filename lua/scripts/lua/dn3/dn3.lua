-- Startup
function InitGame(levelTime, randomSeed, restart)
	game.Print("Lua Level Init...");
	game.Print("-map_restart ...");
		game.Print("--workaround-setup ...");
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_levelchange");
			ent:SetKeyValue("targetname", "map_restart");
			ent:SetKeyValue("target", "dn3");
			entity.CallSpawn(ent);
		game.Print("--trigger-setup ...");
		if restart == 1 then
			game.ClientPrint(-1, "Please don't use map_restart, use map or devmap instead.");
			game.Print("Please don't use map_restart, use map or devmap instead.");
			ent = entity.Find("map_restart")
			entity.Use(ent)
		end 
	game.Print("-Lift-A-Script...");
		game.Print("--set up usables and lift...");
			ent = entity.FindBModel(72);
			ent:SetKeyValue("classname", "func_usable");
			ent:SetKeyValue("target", "liftabool");
			ent:SetKeyValue("luaUse", "sound72");
			ent:SetKeyValue("spawnflags", "8");
			ent:SetKeyValue("wait", "3");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(73);
			ent:SetKeyValue("classname", "func_usable");
			ent:SetKeyValue("target", "liftabool");
			ent:SetKeyValue("luaUse", "sound73");
			ent:SetKeyValue("spawnflags", "8");
			ent:SetKeyValue("wait", "3");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(3);
			ent:SetKeyValue("classname", "func_static");
			ent:SetKeyValue("luaReached", "liftareachedsound");
			entity.CallSpawn(ent);
		game.Print("--set up overwrite-savety ...");
			game.Print("---set up boolean ...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_boolean");
				ent:SetKeyValue("targetname", "liftabool");
				ent:SetKeyValue("falsetarget", "liftastuff");
				ent:SetKeyValue("swapname", "liftaswap");
				ent:SetKeyValue("spawnflags", "4");
				entity.CallSpawn(ent);
			game.Print("---set up delay ...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_delay");
				ent:SetKeyValue("targetname", "liftastuff");
				ent:SetKeyValue("target", "liftaswap");
				ent:SetKeyValue("spawnflags", "1");
				ent:SetKeyValue("wait", "6");
				entity.CallSpawn(ent);
			game.Print("---set up relay ...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_relay");
				ent:SetKeyValue("targetname", "liftastuff");
				ent:SetKeyValue("target", "liftaswap");
				ent:SetKeyValue("spawnflags", "8");
				entity.CallSpawn(ent);
		game.Print("--set up lift-direction-control ...");
			game.Print("---set up delay ...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_relay");
				ent:SetKeyValue("targetname", "liftastuff");
				ent:SetKeyValue("target", "liftadir");
				ent:SetKeyValue("spawnflags", "8");
				entity.CallSpawn(ent);
			game.Print("---set up boolean ...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_boolean");
				ent:SetKeyValue("truetarget", "liftadownrelay");
				ent:SetKeyValue("falsetarget", "liftauprelay");
				ent:SetKeyValue("swapname", "liftadir");
				ent:SetKeyValue("spawnflags", "2");
				entity.CallSpawn(ent);
			game.Print("---set up relays...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_relay");
				ent:SetKeyValue("targetname", "liftauprelay");
				ent:SetKeyValue("luaUse", "liftaup");
				entity.CallSpawn(ent);
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_relay");
				ent:SetKeyValue("targetname", "liftadownrelay");
				ent:SetKeyValue("luaUse", "liftadown");
				entity.CallSpawn(ent); 
	game.Print("-Lift-B-Script...");
		game.Print("--set up usables and lift...");
			ent = entity.FindBModel(75);
			ent:SetKeyValue("classname", "func_usable");
			ent:SetKeyValue("target", "liftbbool");
			ent:SetKeyValue("luaUse", "sound75");
			ent:SetKeyValue("spawnflags", "8");
			ent:SetKeyValue("wait", "3");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(76);
			ent:SetKeyValue("classname", "func_usable");
			ent:SetKeyValue("target", "liftbbool");
			ent:SetKeyValue("luaUse", "sound76");
			ent:SetKeyValue("spawnflags", "8");
			ent:SetKeyValue("wait", "3");
			entity.CallSpawn(ent); 
			ent = entity.FindBModel(10);
			ent:SetKeyValue("luaReached", "liftbreachedsound");
			entity.CallSpawn(ent);
		game.Print("--set up overwrite-savety ...");
			game.Print("---set up boolean ...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_boolean");
				ent:SetKeyValue("targetname", "liftbbool");
				ent:SetKeyValue("falsetarget", "liftbstuff");
				ent:SetKeyValue("swapname", "liftbswap");
				ent:SetKeyValue("spawnflags", "4");
				entity.CallSpawn(ent);
			game.Print("---set up delay ...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_delay");
				ent:SetKeyValue("targetname", "liftbstuff");
				ent:SetKeyValue("target", "liftbswap");
				ent:SetKeyValue("spawnflags", "1");
				ent:SetKeyValue("wait", "6");
				entity.CallSpawn(ent);
			game.Print("---set up relay ...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_relay");
				ent:SetKeyValue("targetname", "liftbstuff");
				ent:SetKeyValue("target", "liftbswap");
				ent:SetKeyValue("spawnflags", "8");
				entity.CallSpawn(ent);
		game.Print("--set up lift-direction-control ...");
			game.Print("---set up delay ...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_delay");
				ent:SetKeyValue("targetname", "liftbstuff");
				ent:SetKeyValue("target", "liftbdir");
				ent:SetKeyValue("spawnflags", "1");
				ent:SetKeyValue("wait", "2");
				entity.CallSpawn(ent);
			game.Print("---set up boolean ...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_boolean");
				ent:SetKeyValue("falsetarget", "liftbdownrelay");
				ent:SetKeyValue("truetarget", "liftbuprelay");
				ent:SetKeyValue("swapname", "liftbdir");
				ent:SetKeyValue("spawnflags", "2");
				entity.CallSpawn(ent);
			game.Print("---set up relays...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_relay");
				ent:SetKeyValue("targetname", "liftbuprelay");
				ent:SetKeyValue("luaUse", "liftbup");
				entity.CallSpawn(ent);
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_relay");
				ent:SetKeyValue("targetname", "liftbdownrelay");
				ent:SetKeyValue("luaUse", "liftbdown");
				entity.CallSpawn(ent);  
	game.Print("-Lift-C-Script...");
		game.Print("--set up usables ..."); 
			ent = entity.FindBModel(77);
			ent:SetKeyValue("classname", "func_usable");
			ent:SetKeyValue("targetname", "liftcswitch");
			ent:SetKeyValue("target", "liftcstuff");
			ent:SetKeyValue("luaUse", "sound77");
			ent:SetKeyValue("spawnflags", "8");
			ent:SetKeyValue("wait", "3");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(78);
			ent:SetKeyValue("classname", "func_usable");
			ent:SetKeyValue("targetname", "liftcswitch");
			ent:SetKeyValue("target", "liftcdoor");
			ent:SetKeyValue("luaUse", "sound78");
			ent:SetKeyValue("spawnflags", "8");
			ent:SetKeyValue("wait", "3");
			entity.CallSpawn(ent);
		game.Print("--set up doors ...");
			ent = entity.FindBModel(51);
			ent:SetKeyValue("targetname", "liftcdoor");
			ent:SetKeyValue("angle", "270");
			ent:SetKeyValue("wait", "-1");
			ent:SetKeyValue("speed", "25");
			ent:SetKeyValue("count", "40");
			ent:SetKeyValue("spawnflags", "0");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(52);
			ent:SetKeyValue("targetname", "liftcdoor");
			ent:SetKeyValue("angle", "270");
			ent:SetKeyValue("wait", "-1");
			ent:SetKeyValue("speed", "25");
			ent:SetKeyValue("count", "32");
			ent:SetKeyValue("spawnflags", "0");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(53);
			ent:SetKeyValue("targetname", "liftcdoor");
			ent:SetKeyValue("angle", "90");
			ent:SetKeyValue("wait", "-1");
			ent:SetKeyValue("speed", "25");
			ent:SetKeyValue("count", "40");
			ent:SetKeyValue("spawnflags", "0");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(54);
			ent:SetKeyValue("targetname", "liftcdoor");
			ent:SetKeyValue("angle", "90");
			ent:SetKeyValue("wait", "-1");
			ent:SetKeyValue("speed", "25");
			ent:SetKeyValue("count", "32");
			ent:SetKeyValue("spawnflags", "0");
			entity.CallSpawn(ent);
		game.Print("--set up lift and transfer ...");
			ent = entity.FindBModel(15);
			ent:SetKeyValue("luaReached", "liftcreachedsound");
			entity.CallSpawn(ent);
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_levelchange");
			ent:SetKeyValue("targetname", "transfer");
			ent:SetKeyValue("target", "dn4");
			entity.CallSpawn(ent);
		game.Print("--set up delay ...");
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_delay");
			ent:SetKeyValue("targetname", "liftcstuff");
			ent:SetKeyValue("target", "liftcmove");
			ent:SetKeyValue("spawnflags", "1");
			ent:SetKeyValue("wait", "3");
			entity.CallSpawn(ent);
		game.Print("--set up relay for doors ...");
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_relay");
			ent:SetKeyValue("targetname", "liftcstuff");
			ent:SetKeyValue("target", "liftcdoor");
			ent:SetKeyValue("spawnflags", "8");
			entity.CallSpawn(ent);
		game.Print("--set up relay for move ...");
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_relay");
			ent:SetKeyValue("targetname", "liftcmove");
			ent:SetKeyValue("luaUse", "liftcup");
			entity.CallSpawn(ent);
	game.Print("-gasroom ..."); 
		game.Print("--setting up elevator ...");  
			ent = entity.FindBModel(13);
			ent:SetKeyValue("classname", "func_door");
			ent:SetKeyValue("targetname", "elevator");
			ent:SetKeyValue("angle", "-1");
			ent:SetKeyValue("wait", "5");
			ent:SetKeyValue("speed", "60");
			ent:SetKeyValue("count", "234");
			ent:SetKeyValue("spawnflags", "4");
			entity.CallSpawn(ent);
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_delay");
			ent:SetKeyValue("targetname", "uppity");
			ent:SetKeyValue("target", "elevator");
			ent:SetKeyValue("wait", "1");
			entity.CallSpawn(ent);
		game.Print("--setting up bridge ...");  
			ent = entity.FindBModel(27);
			ent:SetKeyValue("classname", "func_door_rotating");
			ent:SetKeyValue("targetname", "holdmeup");
			ent:SetKeyValue("distance", "90");
			ent:SetKeyValue("wait", "-1");
			ent:SetKeyValue("speed", "60");
			ent:SetKeyValue("spawnflags", "16");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(74);
			ent:SetKeyValue("classname", "func_usable");
			ent:SetKeyValue("target", "holdmeup");
			ent:SetKeyValue("luaUse", "sound74");
			ent:SetKeyValue("wait", "1"); 
			ent:SetKeyValue("spawnflags", "8");
			entity.CallSpawn(ent);  
	game.Print("-setting up start doors ...");
		ent = entity.FindBModel(40);
		ent:SetKeyValue("angle", "0");
		ent:SetKeyValue("wait", "-1");
		ent:SetKeyValue("speed", "25");
		ent:SetKeyValue("count", "40");
		ent:SetKeyValue("spawnflags", "4");
		entity.CallSpawn(ent);  
		entity.Use(ent); 
		ent = entity.FindBModel(41);
		ent:SetKeyValue("angle", "0");
		ent:SetKeyValue("wait", "-1");
		ent:SetKeyValue("speed", "25");
		ent:SetKeyValue("count", "32");
		ent:SetKeyValue("spawnflags", "4");
		entity.CallSpawn(ent);    
		entity.Use(ent); 
		ent = entity.FindBModel(42);
		ent:SetKeyValue("angle", "180");
		ent:SetKeyValue("wait", "-1");
		ent:SetKeyValue("speed", "25");
		ent:SetKeyValue("count", "40");
		ent:SetKeyValue("spawnflags", "4");
		entity.CallSpawn(ent);  
		entity.Use(ent); 
		ent = entity.FindBModel(43);
		ent:SetKeyValue("angle", "180");
		ent:SetKeyValue("wait", "-1");
		ent:SetKeyValue("speed", "25");
		ent:SetKeyValue("count", "32");
		ent:SetKeyValue("spawnflags", "4");
		entity.CallSpawn(ent);    
		entity.Use(ent);  
	game.Print("-Setting up spawnpoints..."); 
		entity.RemoveSpawns();
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, -24, -1224, 72); 
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, 8, -1256, 72); 
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, 8, -1192, 72); 
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, -56, -1256, 72); 
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
		ent = entity.Spawn();
		ent:SetKeyValue("classname", "info_player_deathmatch");
		mover.SetPosition(ent, -56, -1192, 72); 
		mover.SetAngles(ent, 0, 90, 0);
		entity.CallSpawn(ent);
game.Print("...Done");
end 

-- Functions lift A

function sound72(ent, other, activator)
    sound.PlaySound(entity.FindBModel(72), "sound/ambience/dreadnaught/dnlcarpattern1.wav", 0);
end

function sound73(ent, other, activator)
    sound.PlaySound(entity.FindBModel(73), "sound/ambience/dreadnaught/dnlcarpattern1.wav", 0); 
end   

function liftareachedsound(ent, other, activator)
    sound.PlaySound(entity.FindBModel(3), "sound/movers/doors/largedoorstop.mp3", 0);
end 

function liftaup(ent)
	ent = entity.FindBModel(3);
	mover.ToPosition(ent, 75, 160, 388, 48);
    sound.PlaySound(ent, "sound/movers/doors/largedoorstart.mp3", 0);
end

function liftadown(ent)
	ent = entity.FindBModel(3);
	mover.ToPosition(ent, 75, 320, 388, -272);
    sound.PlaySound(ent, "sound/movers/doors/largedoorstart.mp3", 0);
end

-- Functions lift b

function sound75(ent, other, activator)
    sound.PlaySound(entity.FindBModel(75), "sound/ambience/dreadnaught/dnlcarpattern1.wav", 0);
end

function sound76(ent, other, activator)
    sound.PlaySound(entity.FindBModel(76), "sound/ambience/dreadnaught/dnlcarpattern1.wav", 0); 
end   

function liftbreachedsound(ent, other, activator)
    sound.PlaySound(entity.FindBModel(10), "sound/movers/doors/largedoorstop.mp3", 0);
end 

function liftbup(ent)
	ent = entity.FindBModel(10);
	mover.ToPosition(ent, 75, 1360, 768, 148);
    sound.PlaySound(ent, "sound/movers/doors/largedoorstart.mp3", 0);
end

function liftbdown(ent)
	ent = entity.FindBModel(10);
	mover.ToPosition(ent, 75, 1360, 768, -300);
    sound.PlaySound(ent, "sound/movers/doors/largedoorstart.mp3", 0);  
end 

-- Functions lift C 

function sound77(ent, other, activator)
    sound.PlaySound(entity.FindBModel(77), "sound/ambience/dreadnaught/dnlcarpattern1.wav", 0);
end

function sound78(ent, other, activator)
    sound.PlaySound(entity.FindBModel(78), "sound/ambience/dreadnaught/dnlcarpattern1.wav", 0); 
end   

function liftcreachedsound(ent, other, activator)
    sound.PlaySound(entity.FindBModel(15), "sound/movers/doors/largedoorstop.mp3", 0);
end                                    

function liftcup(ent)
	ent = entity.FindBModel(15);
	mover.ToPosition(ent, 75, 704, 768, 1000);
    sound.PlaySound(ent, "sound/movers/doors/largedoorstart.mp3", 0);
	entity.Use(entity.Find("transfer"));
end  

-- Else

function sound74(ent, other, activator)
    sound.PlaySound(entity.FindBModel(77), "sound/ambience/dreadnaught/dnlcarpattern1.wav", 0);
end