-- Startup
function InitGame(levelTime, randomSeed, restart)
	game.Print("Lua Level Init...");
	game.Print("-map_restart ...");
		game.Print("--workaround-setup ...");
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_levelchange");
			ent:SetKeyValue("targetname", "map_restart");
			ent:SetKeyValue("target", "dn5");
			entity.CallSpawn(ent);
		game.Print("--trigger-setup ...");
		if restart == 1 then
			game.ClientPrint(-1, "Please don't use map_restart, use map or devmap instead.");
			game.Print("Please don't use map_restart, use map or devmap instead.");
			ent = entity.Find("map_restart")
			entity.Use(ent)
		end
	game.Print("-Setting up bridge-room...");
		game.Print("--remove stuff...");
			entity.Remove(entity.Find("killkill"));
			entity.Use(entity.Find("nostick"));
		game.Print("--set up brushmodels...");
			ent = entity.FindBModel(8);
			ent:SetKeyValue("classname", "func_breakable");
			ent:SetKeyValue("material", "1");
			ent:SetKeyValue("luaReached", "bridgecrash");
			ent:SetKeyValue("target", "pusher2");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(23);
			ent:SetKeyValue("classname", "func_breakable");
			ent:SetKeyValue("material", "1");
			ent:SetKeyValue("splashDamage", "75");
			ent:SetKeyValue("splashRadius", "75");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(15);
			ent:SetKeyValue("classname", "func_usable");
			ent:SetKeyValue("luaUse", "carcrash");
			ent:SetKeyValue("target", "pusher1");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(24);
			ent:SetKeyValue("classname", "func_static");
			entity.CallSpawn(ent);
	game.Print("-Lift-Script...");
		game.Print("--set up usables ...");
			ent = entity.FindBModel(82);
			ent:SetKeyValue("targetname", "liftswitch");
			ent:SetKeyValue("target", "liftbool");
			ent:SetKeyValue("luaUse", "sound82");
			ent:SetKeyValue("spawnflags", "8");
			ent:SetKeyValue("wait", "3");
			entity.CallSpawn(ent);
			ent = entity.FindBModel(84);
			ent:SetKeyValue("classname", "func_usable");
			ent:SetKeyValue("targetname", "liftswitch");
			ent:SetKeyValue("target", "liftbool");
			ent:SetKeyValue("luaUse", "sound84");
			ent:SetKeyValue("spawnflags", "8");
			ent:SetKeyValue("wait", "3");
			entity.CallSpawn(ent);
		game.Print("--set up lift ...");
			ent = entity.FindBModel(5);
			ent:SetKeyValue("classname", "func_static");
			ent:SetKeyValue("luaReached", "liftreachedsound");
			entity.CallSpawn(ent);
		game.Print("--set up overwrite-savety ...");
			game.Print("---set up boolean ...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_boolean");
				ent:SetKeyValue("targetname", "liftbool");
				ent:SetKeyValue("falsetarget", "liftstuff");
				ent:SetKeyValue("swapname", "liftswap");
				ent:SetKeyValue("spawnflags", "4");
				entity.CallSpawn(ent);
			game.Print("---set up delay ...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_delay");
				ent:SetKeyValue("targetname", "liftstuff");
				ent:SetKeyValue("target", "liftswap");
				ent:SetKeyValue("spawnflags", "1");
				ent:SetKeyValue("wait", "13");
				entity.CallSpawn(ent);
			game.Print("---set up relay ...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_relay");
				ent:SetKeyValue("targetname", "liftstuff");
				ent:SetKeyValue("target", "liftswap");
				ent:SetKeyValue("spawnflags", "8");
				entity.CallSpawn(ent);
		game.Print("--set up lift-direction-control ...");
			game.Print("---set up delay ...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_relay");
				ent:SetKeyValue("targetname", "liftstuff");
				ent:SetKeyValue("target", "liftdir");
				ent:SetKeyValue("spawnflags", "8");
				entity.CallSpawn(ent);
			game.Print("---set up boolean ...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_boolean");
				ent:SetKeyValue("falsetarget", "liftdownrelay");
				ent:SetKeyValue("truetarget", "liftuprelay");
				ent:SetKeyValue("swapname", "liftdir");
				ent:SetKeyValue("spawnflags", "2");
				entity.CallSpawn(ent);
			game.Print("---set up relays...");
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_relay");
				ent:SetKeyValue("targetname", "liftuprelay");
				ent:SetKeyValue("luaUse", "liftup");
				entity.CallSpawn(ent);
				ent = entity.Spawn();
				ent:SetKeyValue("classname", "target_relay");
				ent:SetKeyValue("targetname", "liftdownrelay");
				ent:SetKeyValue("luaUse", "liftdown");
				entity.CallSpawn(ent);
	game.Print("-Setting up transporter...");
			ent = entity.FindBModel(13);
			ent:SetKeyValue("classname", "func_usable");
			ent:SetKeyValue("target", "transfer");
			ent:SetKeyValue("luaUse", "transferstart");
			ent:SetKeyValue("luaReached", "transfermove");
			ent:SetKeyValue("spawnflags", "8");
			ent:SetKeyValue("wait", "3");
			entity.CallSpawn(ent);
			ent = entity.Spawn();
			ent:SetKeyValue("classname", "target_levelchange");
			ent:SetKeyValue("targetname", "transfer");
			ent:SetKeyValue("target", "dn6");
			ent:SetKeyValue("wait", "15");
			entity.CallSpawn(ent);
	game.Print("-Setting up spawnpoints...");
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
		entity.CallSpawn(ent);
game.Print("...Done");
end  

-- bridge-room

function carcrash(ent, other, activator)
    sound.PlaySound(entity.FindBModel(15), "sound/ambience/dreadnaught/dnlcarpattern1.wav", 0);
	mover.ToPosition(entity.FindBModel(8), 25, -256, -1484, 64)
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