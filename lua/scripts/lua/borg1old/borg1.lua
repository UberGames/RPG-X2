function InitGame(levelTime, randomSeed, restart)
	local ent;
	local i = 17;
	local x = 113;
	local vec = vector.New();
	if restart == 1 then 
		game.ClientPrint(-1, "Please don't use map_restart, use map or devmap instead.");
		game.Print("Please don't use map_restart, use map or devmap instead.");
	end
	ent = entity.Find("borg_munro");
	entity.Remove(ent);
	ent = entity.Find("borg_alexa");
	entity.Remove(ent);
	ent = entity.Find("deathborg");
	entity.Remove(ent);
	ent = entity.Find("deathborg1");
	entity.Remove(ent);
	ent = entity.FindBModel(127);
	entity.Remove(ent);
	ent = entity.FindBModel(91);
	ent.SetKeyValue(ent, "spawnflags", "0");
	ent.SetKeyValue(ent, "angle", "-2");
	ent.SetKeyValue(ent, "count", "32");
	ent.SetKeyValue(ent, "wait", "5");
	entity.CallSpawn(ent);
	ent = entity.FindBModel(89);
	ent.SetKeyValue(ent, "spawnflags", "0");
	ent.SetKeyValue(ent, "angle", "-2");
	ent.SetKeyValue(ent, "count", "32");
	ent.SetKeyValue(ent, "wait", "5");
	entity.CallSpawn(ent);
	ent = entity.FindBModel(39);
	ent.SetKeyValue(ent, "classname", "func_usable");
	ent.SetKeyValue(ent, "spawnflags", "8");
	ent.SetKeyValue(ent, "target", "forcefield_1");
	ent.SetKeyValue(ent, "targetname", "fffu1");
	ent.SetKeyValue(ent, "wait", "2");
	entity.CallSpawn(ent);
	ent = entity.FindBModel(38);
	ent.SetKeyValue(ent, "spawnflags", "8");
	ent.SetKeyValue(ent, "luaEntity", "1");
	ent.SetKeyValue(ent, "dmg", "1");
	ent.SetKeyValue(ent, "target", "fffu1");
	ent.SetKeyValue(ent, "wait", "2");
	entity.CallSpawn(ent);
	ent = entity.FindBModel(88);
	ent.SetKeyValue(ent, "classname", "func_forcefield");
	ent.SetKeyValue(ent, "targetname", "forcefield_1");
	entity.CallSpawn(ent);
	ent = entity.FindBModel(13);
	ent.SetKeyValue(ent, "health", "20");
	entity.CallSpawn(ent);
	ent = entity.FindBModel(87);
	entity.Remove(ent);
	ent = entity.FindBModel(40);
	ent.SetKeyValue(ent, "health", "20");
	entity.CallSpawn(ent);
	ent = entity.FindBModel(109);
	ent.SetKeyValue(ent, "wait", "-1");
	ent.SetKeyValue(ent, "target", "null");
	ent.SetKeyValue(ent, "luaTrigger", "Grate_Fall");
	entity.CallSpawn(ent);
	ent = entity.FindBModel(41);
	ent.SetKeyValue(ent, "classname", "func_static");
	entity.CallSpawn(ent);
	ent = entity.FindBModel(25);
	entity.Remove(ent);
	ent = entity.FindBModel(148);
	ent.SetKeyValue(ent, "classname", "func_usable");
	ent.SetKeyValue(ent, "spawnflags", "8");
	ent.SetKeyValue(ent, "wait", "6");
	ent.SetKeyValue(ent, "target", "plat1");
	ent.SetKeyValue(ent, "dmg", "2");
	ent.SetKeyValue(ent, "luaEntity", "1");
	entity.CallSpawn(ent);
	ent = entity.FindBModel(147);
	ent.SetKeyValue(ent, "classname", "func_usable");
	ent.SetKeyValue(ent, "spawnflags", "8");
	ent.SetKeyValue(ent, "wait", "6");
	ent.SetKeyValue(ent, "target", "plat1");
	ent.SetKeyValue(ent, "dmg", "2");
	ent.SetKeyValue(ent, "luaEntity", "1");
	entity.CallSpawn(ent);
	entity.RemoveSpawns();
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 171, 1090, -103);
	entity.CallSpawn(ent);
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 143, -400, -8);
	entity.CallSpawn(ent);
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 172, -400, -8);
	entity.CallSpawn(ent);
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 202, -400, -8);
	entity.CallSpawn(ent);
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 232, -400, -8);
	entity.CallSpawn(ent);
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 143, -372, -8);
	entity.CallSpawn(ent);
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 172, -372, -8);
	entity.CallSpawn(ent);
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 202, -372, -8);
	entity.CallSpawn(ent);
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 232, -372, -8);
	entity.CallSpawn(ent);
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 143, -338, -8);
	entity.CallSpawn(ent);
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 172, -338, -8);
	entity.CallSpawn(ent);
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 202, -338, -8);
	entity.CallSpawn(ent);
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 232, -338, -8);
	entity.CallSpawn(ent);
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 143, -307, -8);
	entity.CallSpawn(ent);
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 172, -307, -8);
	entity.CallSpawn(ent);
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 202, -307, -8);
	entity.CallSpawn(ent);
	ent = entity.Spawn();
	ent.SetKeyValue(ent, "classname", "info_player_deathmatch");
	mover.SetPosition(ent.GetNumber(ent), 232, -307, -8);
	entity.CallSpawn(ent);
	ent = entity.FindBModel(125);
	ent.SetKeyValue(ent, "classname", "func_forcefield");
	ent.SetKeyValue(ent, "targetname", "imod_ff");
	entity.CallSpawn(ent);
	ent = entity.FindBModel(126);
	ent.SetKeyValue(ent, "classname", "func_forcefield");
	ent.SetKeyValue(ent, "targetname", "imod_ff");
	entity.CallSpawn(ent);
	ent = entity.FindBModel(152);
	entity.Remove(ent);
	ent = entity.FindBModel(146);
	ent.SetKeyValue(ent, "spawnflags", "8");
	ent.SetKeyValue(ent, "target", "imod_ff");
	ent.SetKeyValue(ent, "targetname", "imod_fu");
	ent.SetKeyValue(ent, "wait", "1");
	entity.CallSpawn(ent);
	ent = entity.FindBModel(17);
	ent.SetKeyValue(ent, "spawnflags", "8");
	ent.SetKeyValue(ent, "target", "imod_fu");
	ent.SetKeyValue(ent, "wait", "1");
	ent.SetKeyValue(ent, "dmg", "3");
	entity.CallSpawn(ent);
	
end

function Grate_Fall(ent, other)
	local grate = entity.FindBModel(41);
	local target = entity.Find("splat4");
	grate.SetKeyValue(grate, "luaReached", "Grate_Fall_Reached");
	mover.ToPosition(grate.GetNumber(grate), 300, target.GetOrigin(target));
	mover.ToAngles(grate.GetNumber(grate), 100, -90, 0, 0);
end

function Grate_Fall_Reached(ent)
	local target = entity.Find("splat1");
	local fs = entity.FindNumber(ent);
	mover.Halt(ent);
	fs.SetKeyValue(fs, "luaReached", "Grate_Fall_Reached2");
	mover.ToPosition(ent, 300, target.GetOrigin(target));
end

function Grate_Fall_Reached2(ent)
	local target = entity.Find("splat2");
	local fs = entity.FindNumber(ent);
	mover.Halt(ent);
	fs.SetKeyValue(fs, "luaReached", "Grate_Fall_Reached3");
	mover.ToPosition(ent, 300, target.GetOrigin(target));
end

function Grate_Fall_Reached3(ent)
	local fs = entity.FindNumber(ent);
	local cor = vector.Construct(32, 1088, 90);
	mover.Halt(ent);
	fs.SetKeyValue(fs, "luaReached", "Grate_Fall_Reached4");
	mover.ToPosition(ent, 300, cor);
end

function Grate_Fall_Reached4(ent)
	local breakable = entity.FindBModel(82);
	entity.Use(breakable);
	mover.Halt(ent);
end

