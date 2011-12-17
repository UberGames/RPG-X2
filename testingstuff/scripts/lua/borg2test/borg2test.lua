-- Startup
function InitGame(levelTime, randomSeed, restart)
		game.Print("--Lift D...");
			game.Print("---Segment reclassification ...");
				ent = entity.FindBModel(52)
				ent:SetClassname("func_static");
				--ent:SetTargetname("dual_lift_upper");
				ent:SetLuaReached("liftdreachedsound");
				entity.CallSpawn(ent);
				
				ent = entity.FindBModel(54)
				ent:SetClassname("func_static");
				--ent:SetTargetname("dual_lift_lower");
				ent:SetLuaReached("liftdreachedsound");
				entity.CallSpawn(ent); 
				
			game.Print("---Metascripts ...");
				game.Print("---down1 ...");
					ent = entity.Spawn();
					ent:SetClassname("target_relay");
					ent:SetTargetname("down1");
					ent:SetLuaUse("liftddown1");
					entity.CallSpawn(ent);
					
					ent = entity.Spawn();
					ent:SetClassname("target_delay");
					ent:SetTargetname("down1");
					ent:SetTarget("down2");
					ent:SetWait(5);
					entity.CallSpawn(ent);
					
				game.Print("---down2 ...");
					ent = entity.Spawn();
					ent:SetClassname("target_relay");
					ent:SetTargetname("down2");
					ent:SetLuaUse("liftddown2");
					entity.CallSpawn(ent);
					
					ent = entity.Spawn();
					ent:SetClassname("target_delay");
					ent:SetTargetname("down2");
					ent:SetTarget("up2");
					ent:SetWait(5);
					entity.CallSpawn(ent);
					
				game.Print("---up2 ...");
					ent = entity.Spawn();
					ent:SetClassname("target_relay");
					ent:SetTargetname("up2");
					ent:SetLuaUse("liftdup2");
					entity.CallSpawn(ent);
					
					ent = entity.Spawn();
					ent:SetClassname("target_delay");
					ent:SetTargetname("up2");
					ent:SetTarget("up1");
					ent:SetWait(5);
					entity.CallSpawn(ent);
					
				game.Print("---up1 ...");
					ent = entity.Spawn();
					ent:SetClassname("target_relay");
					ent:SetTargetname("up1");
					ent:SetLuaUse("liftdup1");
					entity.CallSpawn(ent);
					
					ent = entity.Spawn();
					ent:SetClassname("target_delay");
					ent:SetTargetname("up1");
					ent:SetTarget("down1");
					ent:SetWait(5);
					entity.CallSpawn(ent);
					
			game.Print("---init ...");
				ent = entity.Spawn();
				ent:SetClassname("target_relay");
				ent:SetTarget("down1");
				entity.CallSpawn(ent);
				entity.Use(ent)
				
			game.Print("---fx-startspawn ...");
				--ent = entity.Spawn();
				--ent:SetClassname("fx_borg_bolt");
				--ent:SetMessage("dual_lift_upper");
				--ent:SetTarget("dual_lift_lower");
				--entity.CallSpawn(ent);
end
-- Functions Lift D

function liftddown1(ent, other, activator)
	ent = entity.FindBModel(52);
	mover.ToPosition(ent, 12, 768, -1984, 224);
    sound.PlaySound(ent, "sound/movers/doors/largedoorstart.mp3", 0);
end

function liftddown2(ent, other, activator)
	ent = entity.FindBModel(52); 
	mover.ToPosition(ent, 30, 768, -1984, 104);
    sound.PlaySound(ent, "sound/movers/doors/largedoorstart.mp3", 0);
	ent = entity.FindBModel(54);
	mover.ToPosition(ent, 30, 768, -1984, 104);
    sound.PlaySound(ent, "sound/movers/doors/largedoorstart.mp3", 0);
end

function liftdup2(ent, other, activator)
	ent = entity.FindBModel(52);
	mover.ToPosition(ent, 30, 768, -1984, 224);
    sound.PlaySound(ent, "sound/movers/doors/largedoorstart.mp3", 0);
	ent = entity.FindBModel(54);
	mover.ToPosition(ent, 30, 768, -1984, 224);
    sound.PlaySound(ent, "sound/movers/doors/largedoorstart.mp3", 0);
end

function liftdup1(ent, other, activator)
	ent = entity.FindBModel(52);
	mover.ToPosition(ent, 12, 768, -1984, 272);
    sound.PlaySound(ent, "sound/movers/doors/largedoorstart.mp3", 0);
end

function liftdreachedsound(ent, other, activator)
    sound.PlaySound(entity.FindBModel(52), "sound/movers/doors/largedoorstop.mp3", 0);
end