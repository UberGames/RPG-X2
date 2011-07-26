// entity lib for lua

#include "g_lua.h"
#include "g_spawn.h"

#ifdef G_LUA
// entity.GetTarget(entity ent)
// returns a target entity of ent
static int Entity_GetTarget(lua_State * L)
{
	lent_t     *lent;
	lent_t     *target;
	gentity_t      *t = NULL;

	target = lua_newuserdata(L, sizeof(lent_t));
	luaL_getmetatable(L, "game.entity");
	lua_setmetatable(L, -2);

	lent = Lua_GetEntity(L, 1);


	if(!lent->e)
	{
		Com_Printf("Entity_Target - invalid entity!\n");
		return 0;
	}
	if(!lent->e->target)
	{
		Com_Printf("Entity_Target - no target!\n");
		return 0;
	}

	t = G_PickTarget(lent->e->target);
	if(!t)
	{
		G_Printf("Entity_Target - Couldn't find target %s\n", lent->e->target);
		return 0;
	}

	target->e = t;

	return 1;
}

// entity.FindBModel(int bmodel)
// finds entity by brush model
static int Entity_FindBModel(lua_State *L) {
	lent_t		*lent;
	int			bmodel;

	lent = lua_newuserdata(L, sizeof(lent_t));

	luaL_getmetatable(L, "game.entity");
	lua_setmetatable(L, -2);

	bmodel = luaL_checkint(L, 1);

	lent->e = G_Find(NULL, FOFS(model), va("*%i", bmodel));
	if(lent->e)
		return 1;
	return 0;
}

// entity.FindNumber(int num)
// finds entity by number
static int Entity_FindNumber(lua_State * L)
{
	lent_t     *lent;
	int             entnum;
	gentity_t      *ent;

	lent = lua_newuserdata(L, sizeof(lent_t));

	luaL_getmetatable(L, "game.entity");
	lua_setmetatable(L, -2);

	entnum = luaL_checknumber(L, 1);

	lent->e = NULL;

	ent = &g_entities[entnum];
	if(ent)
	{
		if(ent->inuse)
		{
			lent->e = ent;
			return 1;
		}
	}
	return 0;
}

// entity.Find(string targetname)
// finds and returns an entity by it's targetname
static int Entity_Find(lua_State * L)
{
	char           *s;
	lent_t     *lent;
	int             i;
	gentity_t      *t;

	lent = lua_newuserdata(L, sizeof(lent_t));

	luaL_getmetatable(L, "game.entity");
	lua_setmetatable(L, -2);

	s = (char *)luaL_checkstring(L, 1);

	lent->e = NULL;

	for(i = 0; i < level.num_entities; i++)
	{
		t = &g_entities[i];

		if(!t || !t->inuse)
			continue;

		if(Q_stricmp(t->targetname, s) == 0)
		{
			LUA_DEBUG("Entity_Find - Found an entity.");
			lent->e = t;
			break;
		}

	}

	if(!lent->e)
	{
		Com_Printf("Entity_Find - entity '%s' not found!\n", s);
		return 0;
	}

	return 1;
}

// entity.Use(entity targetname)
// Uses an entity.
static int Entity_Use(lua_State * L)
{
	lent_t     *lent;

	lent = Lua_GetEntity(L, 1);

	if(!lent || !lent->e || !lent->e->use) return 0;

	if(lent->e->luaUse)
		LuaHook_G_EntityUse(lent->e->luaUse, lent->e-g_entities, lent->e-g_entities, lent->e-g_entities);
	lent->e->use(lent->e, NULL, lent->e);

	return 1;
}

// entity.Teleport(entity ent, entity target)
// Teleports a player to another entity
static int Entity_Teleport(lua_State * L)
{
	lent_t     *lent;
	lent_t     *target;


	lent = Lua_GetEntity(L, 1);
	target = Lua_GetEntity(L, 2);

	if(!lent || !lent->e)
	{
		Com_Printf("Entity_Teleport - invalid entity!\n");
		return 0;
	}
	if(!target || !target->e)
	{
		Com_Printf("Entity_Teleport - invalid target!\n");
		return 0;
	}

	if(lent->e->client)
		TeleportPlayer(lent->e, target->e->s.origin, target->e->s.angles, TP_NORMAL);
	return 1;
}


// entity.IsRocket(entity ent)
// Checks if an entity is a rocket
static int Entity_IsRocket(lua_State * L)
{
	lent_t     *lent;
	qboolean        rocket = qfalse;

	luaL_getmetatable(L, "game.entity");
	lua_setmetatable(L, -2);

	lent = Lua_GetEntity(L, 1);

	if(lent->e && !Q_stricmp(lent->e->classname, "rocket"))
		rocket = qtrue;

	lua_pushboolean(L, (int)rocket);

	return 1;
}

// entity.IsGrenade(entity ent)
// Checks if an enity is a grenade
static int Entity_IsGrenade(lua_State * L)
{
	lent_t     *lent;
	qboolean        grenade = qfalse;

	lent = Lua_GetEntity(L, 1);

	if(Q_stricmp(lent->e->classname, "grenade"))
		grenade = qtrue;

	lua_pushboolean(L, grenade);

	return 1;
}

// entity.Spawn(void)
// Spawn a new entity if possible
static int Entity_Spawn(lua_State * L)
{
	lent_t     *lent;

	lent = lua_newuserdata(L, sizeof(lent_t));

	luaL_getmetatable(L, "game.entity");
	lua_setmetatable(L, -2);

	lent->e = G_Spawn();

	return 1;
}

// entity.GetNumber(entity ent)
// returns the entities index number
static int Entity_GetNumber(lua_State * L)
{
	lent_t     *lent;

	lent = Lua_GetEntity(L, 1);
	lua_pushnumber(L, lent->e - g_entities);

	return 1;
}

// entity.IsClient(entity ent)
// Checks if an entity is a client
static int Entity_IsClient(lua_State * L)
{
	lent_t     *lent;

	lent = Lua_GetEntity(L, 1);
	
	if(!lent || !lent->e) {
		return 1;
	}

	lua_pushboolean(L, lent->e->client != NULL);

	return 1;
}

// entity.GetClientName(entity ent)
// Returns the display name of a client
static int Entity_GetClientName(lua_State * L)
{
	lent_t     *lent;

	lent = Lua_GetEntity(L, 1);

	if(!lent || !lent->e || !lent->e->classname) {
		return 1;
	}

	lua_pushstring(L, lent->e->client->pers.netname);

	return 1;
}

static int Entity_Print(lua_State * L)
{
	lent_t     *lent;
	int             i;
	char            buf[MAX_STRING_CHARS];
	int             n = lua_gettop(L);

	lent = Lua_GetEntity(L, 1);

	if(!lent|| !lent->e) return 0;

	if(!lent->e->client)
		return luaL_error(L, "\'Print\' must be used with a client entity");

	memset(buf, 0, sizeof(buf));

	lua_getglobal(L, "tostring");
	for(i = 2; i <= n; i++)
	{
		const char     *s;

		lua_pushvalue(L, -1);
		lua_pushvalue(L, i);
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);

		if(s == NULL)
			return luaL_error(L, "\'tostring\' must return a string to \'print\'");

		Q_strcat(buf, sizeof(buf), s);

		lua_pop(L, 1);
	}

	trap_SendServerCommand(lent->e - g_entities, va("print \"%s\n\"", buf));

	return 0;
}

static int Entity_CenterPrint(lua_State * L)
{
	lent_t     *lent;
	int             i;
	char            buf[MAX_STRING_CHARS];
	int             n = lua_gettop(L);

	lent = Lua_GetEntity(L, 1);
	
	if(!lent || !lent->e) return 0;

	if(!lent->e->client)
		return luaL_error(L, "\'CenterPrint\' must be used with a client entity");

	memset(buf, 0, sizeof(buf));

	lua_getglobal(L, "tostring");
	for(i = 2; i <= n; i++)
	{
		const char     *s;

		lua_pushvalue(L, -1);
		lua_pushvalue(L, i);
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);

		if(s == NULL)
			return luaL_error(L, "\'tostring\' must return a string to \'print\'");

		Q_strcat(buf, sizeof(buf), s);

		lua_pop(L, 1);
	}

	trap_SendServerCommand(lent->e - g_entities, va("cp \"" S_COLOR_WHITE "%s\n\"", buf));

	return 0;
}

extern qboolean G_ParseField(const char *key, const char *value, gentity_t *ent);
// entity.SetKeyValue(entity ent, string key, string value)
// Sets a key of an entity to a value
static int Entity_SetKeyValue(lua_State * L) {
	lent_t		*lent;
	char		*key, *value;
	
	lent = Lua_GetEntity(L, 1);
	key = (char *)luaL_checkstring(L, 2);
	value = (char *)luaL_checkstring(L, 3);

	if(!lent || !lent->e || !key[0] || !value[0]) {
		lua_pushboolean(L, qfalse);
		return 1;
	}

	lua_pushboolean(L, G_ParseField(key, value, lent->e));
	return 1;
}

// entity.GetClassname(entity ent)
// Returns the classname of an entity
static int Entity_GetClassName(lua_State * L)
{
	lent_t     *lent;

	lent = Lua_GetEntity(L, 1);
	lua_pushstring(L, lent->e->classname);

	return 1;
}

// entity.SetClassname(entity ent, string name)
// Sets the Classname of an entity to name
static int Entity_SetClassName(lua_State * L)
{
	lent_t     *lent;

	lent = Lua_GetEntity(L, 1);
	lent->e->classname = (char *)luaL_checkstring(L, 2);

	return 1;
}

// entity.GetTargetname(entity ent)
// Returns the targetname of an entity
static int Entity_GetTargetName(lua_State * L)
{
	lent_t     *lent;

	lent = Lua_GetEntity(L, 1);
	lua_pushstring(L, lent->e->targetname);

	return 1;
}

// entity.Rotate(entity ent, vector dir)
// Rotates an entity in the specified directions
static int Entity_Rotate(lua_State * L)
{
	lent_t     *lent;
	vec_t          *vec;

	lent = Lua_GetEntity(L, 1);
	vec = Lua_GetVector(L, 2);

	lent->e->s.apos.trType = TR_LINEAR;
	lent->e->s.apos.trDelta[0] = vec[0];
	lent->e->s.apos.trDelta[1] = vec[1];
	lent->e->s.apos.trDelta[2] = vec[2];

	return 1;
}

static int Entity_GC(lua_State * L)
{

	return 0;
}

// entity.ToString(entity)
// Prints an entity as string
static int Entity_ToString(lua_State * L)
{
	lent_t     *lent;
	gentity_t      *gent;
	char            buf[MAX_STRING_CHARS];

	lent = Lua_GetEntity(L, 1);
	gent = lent->e;
	Com_sprintf(buf, sizeof(buf), "entity: class=%s name=%s id=%i pointer=%p\n", gent->classname, gent->targetname, gent - g_entities,
				gent);
	lua_pushstring(L, buf);

	return 1;
}

extern qboolean G_CallSpawn(gentity_t *ent);

static void ent_delay(gentity_t *ent) {
	ent->think = 0;
	ent->nextthink = -1;
	G_CallSpawn(ent);
}

// entity.DelayedCallSpawn(entity ent, int ms)
// Calls the entities spawn function delayed
static int Entity_DelayedCallSpawn(lua_State *L) {
	lent_t *lent;
	int		delay;

	lent = Lua_GetEntity(L, 1);

	if(!lent || !lent->e)
		return 1;

	delay = luaL_checkint(L, 2);

	if(!delay)
		delay = FRAMETIME;

	lent->e->nextthink = delay;
	lent->e->think = ent_delay;

	return 1;
}

// entity.CallSpawn(entity ent)
// Calls the entities spawn function
static int Entity_CallSpawn(lua_State *L) {
	lent_t *lent;
	qboolean r = qfalse;
	gentity_t *e = NULL;

	LUA_DEBUG("Entity_CallSpawn - start");

	lent = Lua_GetEntity(L, 1);

	if(lent)
		e = lent->e;

	if(e) {
		LUA_DEBUG("Entity_CallSpawn - G_CallSpawn");
		trap_UnlinkEntity(e);
		r = G_CallSpawn(e);
		lua_pushboolean(L, r);
		return 1;
	} else {
		LUA_DEBUG("Entity_CallSpawn - NULL entity");
	}
	
	lua_pushboolean(L, 0);
	return 1;
}

extern field_t fields[];
static int Entity_RemoveUnnamedSpawns(lua_State *L) {
	gentity_t *ent;
	int cnt = 0, i;
	
	for(i = 0; i < MAX_GENTITIES; i++) {
		if(!&g_entities[i]) continue;
		ent = &g_entities[i];
		if(!ent->classname) continue;
		if(!Q_stricmp(ent->classname, "info_player_deathmatch"))
			if(!ent->targetname) {
				G_FreeEntity(ent);
				cnt++;
			}
	}

	lua_pushnumber(L, cnt);
	return 1;
}

static int Entity_RemoveSpawns(lua_State *L) {
	gentity_t *ent;
	int cnt = 0, i;
	
	for(i = 0; i < MAX_GENTITIES; i++) {
		if(!&g_entities[i]) continue;
		ent = &g_entities[i];
		if(!ent->classname) continue;
		if(!Q_stricmp(ent->classname, "info_player_deathmatch")) {
			G_FreeEntity(ent);
			cnt++;
		}
	}

	lua_pushnumber(L, cnt);
	return 1;
}

static int Entity_RemoveType(lua_State *L) {
	int i, cnt = 0;
	char *classname;

	classname = (char *)luaL_checkstring(L , 1);
	if(!classname) {
		lua_pushinteger(L, -1);
		return 1;
	}

	for(i = 0; i < MAX_GENTITIES; i++) {
		if(!strcmp(g_entities[i].classname, classname)) {
			G_FreeEntity(&g_entities[i]);
			cnt++;
		}
	}

	lua_pushinteger(L, cnt);
	return 1;
}

// entity.Remove(entity ent)
// Removes an entity if it is not protected
static int Entity_Remove(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);

	if(!lent || !lent->e) {
		lua_pushboolean(L, 0);
		return 1;
	}

	if(lent->e->neverFree || lent->e->client) {
		lua_pushboolean(L, 0);
		return 1;
	}

	G_FreeEntity(lent->e);
	lua_pushboolean(L, 1);

	return 1;
}

// entity.SetupTrigger(entity ent, int sizex, int sizey, int sizez)
// entity.SetupTrigger(entity ent, vector size)
// Sets up some basic trigger things
static int Entity_SetupTrigger(lua_State *L) {
	lent_t *lent;
	gentity_t *e;
	vec_t  *vptr;
	vec3_t size;

	lent = Lua_GetEntity(L, 1);

	if(!lent || !lent->e) {
		return 1;
	}

	e = lent->e;

	if(Lua_IsVector(L, 2)) {
		vptr = Lua_GetVector(L, 2); 
		VectorCopy(vptr, size);
	} else {
		size[0] = luaL_checkint(L, 2);
		size[1] = luaL_checkint(L, 3);
		size[2] = luaL_checkint(L, 4);
	}

	VectorCopy(size, e->r.mins);
	VectorCopy(size, e->r.maxs);
	VectorScale(e->r.mins, -.5, e->r.mins);
	VectorScale(e->r.maxs, .5, e->r.maxs);
	VectorCopy(e->r.mins, e->r.absmin);
	VectorCopy(e->r.maxs, e->r.absmax);

	e->tmpEntity = qtrue;

	return 1;
}

// entity.GetOrigin(entity ent)
// Returns the origin of an entity as vector
static int Entity_GetOrigin(lua_State *L) {
	lent_t *lent;
	vec3_t	origin;

	lent = Lua_GetEntity(L, 1);

	if(!lent || !lent->e) {
		lua_pushnil(L);
		return 1;
	}

	if(lent->e->r.svFlags & SVF_USE_CURRENT_ORIGIN)
		VectorCopy(lent->e->r.currentOrigin, origin);
	else
		VectorCopy(lent->e->s.origin, origin);

	Lua_PushVector(L, origin);

	return 1;
}

static int Entity_Lock(lua_State *L) {
	lent_t *lent;
	gentity_t *ent;

	lent = Lua_GetEntity(L, 1);

	if(!lent || !lent->e) return 1;

	ent = lent->e;

	if(!strncmp(ent->classname, "func_door", 9) ||
		!strncmp(ent->classname, "func_door_rotating", 18) ||
		!strncmp(ent->classname, "target_teleporter", 17) ||
		!strncmp(ent->classname, "target_turbolift", 16) ||
		!strncmp(ent->classname, "func_usable", 11) ||
		!strncmp(ent->classname, "target_serverchange", 19) ||
		!strncmp(ent->classname, "trigger_teleport", 16) ||
		!strncmp(ent->classname, "ui_transporter", 14) ||
		!strncmp(ent->classname, "ui_holodeck", 11)
		) {
		if(ent->flags & FL_LOCKED) return 1;
		ent->flags ^= FL_LOCKED;
	}
	return 1;
}

static int Entity_Unlock(lua_State *L) {
	lent_t *lent;
	gentity_t *ent;

	lent = Lua_GetEntity(L, 1);

	if(!lent || !lent->e) return 1;

	ent = lent->e;

	if(!strncmp(ent->classname, "func_door", 9) ||
		!strncmp(ent->classname, "func_door_rotating", 18) ||
		!strncmp(ent->classname, "target_teleporter", 17) ||
		!strncmp(ent->classname, "target_turbolift", 16) ||
		!strncmp(ent->classname, "func_usable", 11) ||
		!strncmp(ent->classname, "target_serverchange", 19) ||
		!strncmp(ent->classname, "trigger_teleport", 16) ||
		!strncmp(ent->classname, "ui_transporter", 14) ||
		!strncmp(ent->classname, "ui_holodeck", 11)
		) {
		if(ent->flags & FL_LOCKED) 
			ent->flags ^= FL_LOCKED;
	}
	return 1;
}

static int Entity_IsLocked(lua_State *L) {
	lent_t *lent;
	gentity_t *ent;

	lent = Lua_GetEntity(L, 1);

	if(!lent || lent->e) return 1;

	ent = lent->e;

	lua_pushboolean(L, (int)(ent->flags & FL_LOCKED));
	return 1;
}

static int Entity_GetParm(lua_State *L) {
	lent_t *lent;
	gentity_t *ent;
	int parm;
	char *res = NULL;

	lent = Lua_GetEntity(L, 1);
	
	if(!lent || !lent->e) {
		lua_pushnil(L);
		return 1;
	}

	ent = lent->e;

	parm = luaL_checkint(L, 2);

	if(!parm || parm < 0 || parm > 4) {
		lua_pushnil(L);
		return 1;
	}

	switch(parm) {
		case 1:
			res = ent->luaParm1;
			break;
		case 2:
			res = ent->luaParm2;
			break;
		case 3:
			res = ent->luaParm3;
			break;
		case 4:
			res = ent->luaParm4;
			break;
	}

	if(!res) {
		lua_pushnil(L);
	} else {
		lua_pushstring(L, res);
	}
	return 1;
}

static int Entity_SetParm(lua_State *L) {
	lent_t *lent;
	gentity_t *ent;
	int parm;
	char *parms;
	char *s = NULL;

	lent = Lua_GetEntity(L, 1);

	if(!lent || !lent->e) return 1;

	ent = lent->e;

	parm = luaL_checkint(L, 2);

	if(!parm || parm < 0 || parm > 4) return 1;

	parms = (char *)luaL_checkstring(L, 3);

	if(!parms) return 1;

	switch(parm) {
		case 1:
			s = ent->luaParm1;
			break;
		case 2:
			s = ent->luaParm2;
			break;
		case 3:
			s = ent->luaParm3;
			break;
		case 4:
			s = ent->luaParm4;
			break;
	}

	if(s) {
		// check if the new string fits into the existing one
		if(strlen(s) > (strlen(parms) + 1)) {
			// it fits so copy it
			strncpy(s, parms, sizeof(s));
		} else {
			// it does not fit in so alloc a new string
			s = G_NewString(parms);
		}
	} else 
		s = G_NewString(parms);
	return 1;
}

static int Entity_GetActivator(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushnil(L);
		return 1;
	}
	Lua_PushEntity(L, lent->e->activator);

	return 1;
}

static int Entity_SetActivator(lua_State *L) {
	lent_t *lent;
	lent_t *activator;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	activator = Lua_GetEntity(L, 2);
	if(!activator || activator->e)
		lent->e->activator = NULL;
	else
		lent->e->activator = activator->e;

	return 1;
}

static int Entity_GetAngle(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushnumber(L, 0);
		return 1;
	}
	lua_pushnumber(L, lent->e->angle);

	return 1;
}

static int Entity_SetAngle(lua_State *L) {
	lent_t *lent;
	float angle;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	angle = (float)luaL_checknumber(L, 2);
	lent->e->angle = angle;

	return 1;
}

static int Entity_GetApos1(lua_State *L) {
	lent_t *lent;
	vec3_t null = { 0, 0, 0 };

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		Lua_PushVector(L, null);
		return 1;
	}
	Lua_PushVector(L, lent->e->apos1);

	return 1;
}

static int Entity_GetApos2(lua_State *L) {
	lent_t *lent;
	vec3_t null = { 0, 0, 0 };

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		Lua_PushVector(L, null);
		return 1;
	}
	Lua_PushVector(L, lent->e->apos2);

	return 1;
}

static int Entity_SetApos1(lua_State *L) {
	lent_t *lent;
	vec_t *vec;
	
	lent = Lua_GetEntity(L, 1);
	if(!lent || lent->e)
		return 1;
	vec = Lua_GetVector(L, 2);
	if(!vec)
		return 1;
	VectorCopy(vec, lent->e->apos1);

	return 1;
}

static int Entity_SetApos2(lua_State *L) {
	lent_t *lent;
	vec_t *vec;
	
	lent = Lua_GetEntity(L, 1);
	if(!lent || lent->e)
		return 1;
	vec = Lua_GetVector(L, 2);
	if(!vec)
		return 1;
	VectorCopy(vec, lent->e->apos2);

	return 1;
}

static int Entity_GetBluename(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	if(!lent->e->bluename[0])
		lua_pushstring(L, "");
	else
		lua_pushstring(L, lent->e->bluename);

	return 1;
}

static int Entity_SetBluename(lua_State *L) {
	lent_t	*lent;
	char	*s;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	if(lua_isstring(L, 2)) {
		s = (char *)luaL_checkstring(L, 2);
		lent->e->bluename = G_NewString(s);
	} else 
		lent->e->bluename = NULL;

	return 1;
}

static int Entity_GetBluesound(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushstring(L, "");
		return 1;
	}
	if(!lent->e->bluesound[0])
		lua_pushstring(L, "");
	else
		lua_pushstring(L, lent->e->bluesound);

	return 1;
}

static int Entity_SetBluesound(lua_State *L) {
	lent_t	*lent;
	char	*s;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	if(lua_isstring(L, 2)) {
		s = (char *)luaL_checkstring(L, 2);
		lent->e->bluesound = G_NewString(s);
	} else 
		lent->e->bluesound = NULL;

	return 1;
}

static int Entity_GetBooleanstate(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || lent->e) {
		lua_pushboolean(L, 0);
		return 1;
	}
	lua_pushboolean(L, (int)lent->e->booleanstate);

	return 1;
}

static int Entity_SetBooleanstate(lua_State *L) {
	lent_t *lent;
	qboolean b;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	b = (qboolean)lua_toboolean(L, 2);

	lent->e->booleanstate = b;

	return 1;
}

static int Entity_GetClipmask(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushinteger(L, 0);
		return 1;
	}
	lua_pushinteger(L, lent->e->clipmask);

	return 1;
}

static int Entity_SetClipmask(lua_State *L) {
	lent_t *lent;
	int mask;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	mask = (int)luaL_checknumber(L, 2);

	lent->e->clipmask = mask;

	return 1;
}

static int Entity_GetCount(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushinteger(L, 0);
		return 1;
	}
	lua_pushinteger(L, lent->e->count);

	return 1;
}

static int Entity_SetCount(lua_State *L) {
	lent_t *lent;
	int		count;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	count = (int)luaL_checknumber(L, 2);

	lent->e->count = count;

	return 1;
}

static int Entity_GetDamage(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushinteger(L, 0);
		return 1;
	}
	lua_pushinteger(L, lent->e->damage);

	return 1;
}

static int Entity_SetDamage(lua_State *L) {
	lent_t *lent;
	int		damage;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	damage = (int)luaL_checknumber(L, 2);

	lent->e->damage = damage;

	return 1;
}

static int Entity_GetDistance(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushnumber(L, 0);
		return 1;
	}
	lua_pushnumber(L, lent->e->distance);

	return 1;
}

static int Entity_SetDistance(lua_State *L) {
	lent_t *lent;
	float	distance;

	lent = Lua_GetEntity(L, 1);
	if(!lent || ! lent->e)
		return 1;
	distance = (float)luaL_checknumber(L, 2);

	lent->e->distance = distance;

	return 1;
}

static int Entity_GetEnemy(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushnil(L);
		return 1;
	}
	Lua_PushEntity(L, lent->e->enemy);

	return 1;
}

static int Entity_SetEnemy(lua_State *L) {
	lent_t *lent;
	lent_t *enemy;

	lent = Lua_GetEntity(L, 1);
	if(!lent || lent->e)
		return 1;
	if(lua_isnil(L, 2)) {
		lent->e->enemy = NULL;
	} else {
		enemy = Lua_GetEntity(L, 2);
		if(!enemy || !enemy->e) {
			lent->e->enemy = NULL;
		} else {
			lent->e->enemy = enemy->e;
		}
	}

	return 1;
}

static int Entity_GetEventTime(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushinteger(L, 0);
		return 1;
	}
	lua_pushinteger(L, lent->e->eventTime);

	return 1;
}

static int Entity_SetEventTime(lua_State *L) {
	lent_t *lent;
	int		eTime;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) 
		return 1;
	eTime = (int)luaL_checknumber(L, 2);

	lent->e->eventTime = eTime;

	return 1;
}

static int Entity_GetFalsename(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushstring(L, "");
	}
	if(!lent->e->falsename[0])
		lua_pushstring(L, "");
	else
		lua_pushstring(L, lent->e->falsename);

	return 1;
}

static int Entity_SetFalsename(lua_State *L) {
	lent_t	*lent;
	char	*s;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	s = (char *)luaL_checkstring(L, 2);
	if(!s[0])
		lent->e->falsename = NULL;
	else
		lent->e->falsename = G_NewString(s);

	return 1;
}

static int Entity_SetTargetName(lua_State *L) {
	lent_t	*lent;
	char	*s;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) 
		return 1;
	s = (char *)luaL_checkstring(L, 2);
	if(!s[0])
		lent->e->targetname = NULL;
	else 
		lent->e->targetname = G_NewString(s);

	return 1;
}

static int Entity_GetFalsetarget(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushstring(L, "");
		return 1;
	}
	if(!lent->e->falsetarget[0]) {
		lua_pushstring(L, "");
	} else {
		lua_pushstring(L, lent->e->falsetarget);
	}

	return 1;
}

static int Entity_SetFalsetarget(lua_State *L) {
	lent_t	*lent;
	char	*s;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	s = (char *)luaL_checkstring(L, 2);
	if(!s[0])
		lent->e->falsetarget = NULL;
	else
		lent->e->falsetarget = G_NewString(s);

	return 1;
}

static int Entity_GetFlags(lua_State *L) {
	lent_t *lent;
	
	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushinteger(L, 0);
		return 1;
	}
	lua_pushinteger(L, lent->e->flags);

	return 1;
}

static int Entity_SetFlags(lua_State *L) {
	lent_t *lent;
	int		flags;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) 
		return 1;
	flags = (int)luaL_checknumber(L, 2);
	
	lent->e->flags = flags;

	return 1;
}

static int Entity_GetFreeAfterEvent(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushboolean(L, 0);
		return 1;
	}
	lua_pushboolean(L, (int)lent->e->freeAfterEvent);

	return 1;
}

static int Entity_SetFreeAfterEvent(lua_State *L) {
	lent_t *lent;
	qboolean b;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	b = (qboolean)lua_toboolean(L, 2);
	
	lent->e->freeAfterEvent = b;

	return 1;
}

static int Entity_GetFreetime(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushinteger(L, 0);
		return 1;
	}
	lua_pushinteger(L, lent->e->freetime);

	return 1;
}

static int Entity_GetGreensound(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushstring(L, "");
		return 1;
	}
	if(!lent->e->greensound[0]) {
		lua_pushstring(L, "");
	} else {
		lua_pushstring(L, lent->e->greensound);
	}

	return 1;
}

static int Entity_SetGreensound(lua_State *L) {
	lent_t	*lent;
	char	*s;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) 
		return 1;
	s = (char *)luaL_checkstring(L, 2);
	if(!s[0])
		lent->e->greensound = NULL;
	else
		lent->e->greensound = G_NewString(s);

	return 1;
}

static int Entity_GetHealth(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushinteger(L, 0);
		return 1;
	}
	lua_pushinteger(L, lent->e->health);

	return 1;
}

static int Entity_SetHealth(lua_State *L) {
	lent_t *lent;
	int		health;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	health = (int)luaL_checknumber(L, 2);

	lent->e->health = health;

	return 1;
}

static int Entity_GetInUse(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushboolean(L, 0);
		return 1;
	}
	lua_pushboolean(L, (int)lent->e->inuse);

	return 1;
}

static int Entity_SetInUse(lua_State *L) {
	lent_t *lent;
	qboolean b;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) 
		return 1;
	b = (qboolean)lua_toboolean(L, 2);
	lent->e->inuse = b;

	return 1;
}

static int Entity_GetLastEnemy(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushnil(L);
		return 1;
	}
	if(!lent->e->lastEnemy)
		lua_pushnil(L);
	else
		Lua_PushEntity(L, lent->e->lastEnemy);

	return 1;
}

static int Entity_SetLastEnemy(lua_State *L) {
	lent_t *lent;
	lent_t *lastEnemy;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	lastEnemy = Lua_GetEntity(L, 2);
	if(!lastEnemy || !lastEnemy->e) 
		return 1;

	lent->e->lastEnemy = lastEnemy->e;

	return 1;
}

static int Entity_GetLuaDie(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushstring(L, "");
		return 1;
	}
	if(!lent->e->luaDie[0]) {
		lua_pushstring(L, "");
	} else {
		lua_pushstring(L, lent->e->luaDie);
	}

	return 1;
}

static int Entity_SetLuaDie(lua_State *L) {
	lent_t	*lent;
	char	*s;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	s = (char *)luaL_checkstring(L, 2);
	if(!s[0])
		lent->e->luaDie = NULL;
	else
		lent->e->luaDie = G_NewString(s);

	return 1;
}

static int Entity_GetLuaEntity(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushboolean(L, 0);
		return 1;
	}
	lua_pushboolean(L, (int)lent->e->luaEntity);

	return 1;
}

static int Entity_SetLuaEntity(lua_State *L) {
	lent_t	*lent;
	qboolean b;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) 
		return 1;
	b = (qboolean)lua_toboolean(L, 2);

	lent->e->luaEntity = b;

	return 1;
}

static int Entity_GetLuaFree(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushstring(L, "");
		return 1;
	}
	if(!lent->e->luaDie[0]) {
		lua_pushstring(L, "");
	} else {
		lua_pushstring(L, lent->e->luaFree);
	}

	return 1;
}

static int Entity_SetLuaFree(lua_State *L) {
	lent_t	*lent;
	char	*s;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	s = (char *)luaL_checkstring(L, 2);
	if(!s[0])
		lent->e->luaFree = NULL;
	else
		lent->e->luaFree = G_NewString(s);

	return 1;
}

static int Entity_GetLuaHurt(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushstring(L, "");
		return 1;
	}
	if(!lent->e->luaDie[0]) {
		lua_pushstring(L, "");
	} else {
		lua_pushstring(L, lent->e->luaHurt);
	}

	return 1;
}

static int Entity_SetLuaHurt(lua_State *L) {
	lent_t	*lent;
	char	*s;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	s = (char *)luaL_checkstring(L, 2);
	if(!s[0])
		lent->e->luaHurt = NULL;
	else
		lent->e->luaHurt = G_NewString(s);

	return 1;
}

static int Entity_GetLuaReached(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushstring(L, "");
		return 1;
	}
	if(!lent->e->luaDie[0]) {
		lua_pushstring(L, "");
	} else {
		lua_pushstring(L, lent->e->luaReached);
	}

	return 1;
}

static int Entity_SetLuaReached(lua_State *L) {
	lent_t	*lent;
	char	*s;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	s = (char *)luaL_checkstring(L, 2);
	if(!s[0])
		lent->e->luaReached = NULL;
	else
		lent->e->luaReached = G_NewString(s);

	return 1;
}

static int Entity_GetLuaReachedAngular(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushstring(L, "");
		return 1;
	}
	if(!lent->e->luaDie[0]) {
		lua_pushstring(L, "");
	} else {
		lua_pushstring(L, lent->e->luaReachedAngular);
	}

	return 1;
}

static int Entity_SetLuaReachedAngular(lua_State *L) {
	lent_t	*lent;
	char	*s;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	s = (char *)luaL_checkstring(L, 2);
	if(!s[0])
		lent->e->luaReachedAngular = NULL;
	else
		lent->e->luaReachedAngular = G_NewString(s);

	return 1;
}

static int Entity_GetLuaSpawn(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushstring(L, "");
		return 1;
	}
	if(!lent->e->luaDie[0]) {
		lua_pushstring(L, "");
	} else {
		lua_pushstring(L, lent->e->luaSpawn);
	}

	return 1;
}

static int Entity_SetLuaSpawn(lua_State *L) {
	lent_t	*lent;
	char	*s;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	s = (char *)luaL_checkstring(L, 2);
	if(!s[0])
		lent->e->luaSpawn = NULL;
	else
		lent->e->luaSpawn = G_NewString(s);

	return 1;
}

static int Entity_GetLuaThink(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushstring(L, "");
		return 1;
	}
	if(!lent->e->luaDie[0]) {
		lua_pushstring(L, "");
	} else {
		lua_pushstring(L, lent->e->luaThink);
	}

	return 1;
}

static int Entity_SetLuaThink(lua_State *L) {
	lent_t	*lent;
	char	*s;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	s = (char *)luaL_checkstring(L, 2);
	if(!s[0])
		lent->e->luaThink = NULL;
	else
		lent->e->luaThink = G_NewString(s);

	return 1;
}

static int Entity_GetLuaTouch(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushstring(L, "");
		return 1;
	}
	if(!lent->e->luaDie[0]) {
		lua_pushstring(L, "");
	} else {
		lua_pushstring(L, lent->e->luaTouch);
	}

	return 1;
}

static int Entity_SetLuaTouch(lua_State *L) {
	lent_t	*lent;
	char	*s;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	s = (char *)luaL_checkstring(L, 2);
	if(!s[0])
		lent->e->luaTouch = NULL;
	else
		lent->e->luaTouch = G_NewString(s);

	return 1;
}

static int Entity_GetLuaTrigger(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushstring(L, "");
		return 1;
	}
	if(!lent->e->luaDie[0]) {
		lua_pushstring(L, "");
	} else {
		lua_pushstring(L, lent->e->luaTrigger);
	}

	return 1;
}

static int Entity_SetLuaTrigger(lua_State *L) {
	lent_t	*lent;
	char	*s;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	s = (char *)luaL_checkstring(L, 2);
	if(!s[0])
		lent->e->luaTrigger = NULL;
	else
		lent->e->luaTrigger = G_NewString(s);

	return 1;
}

static int Entity_GetLuaUse(lua_State *L) {
	lent_t *lent;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e) {
		lua_pushstring(L, "");
		return 1;
	}
	if(!lent->e->luaDie[0]) {
		lua_pushstring(L, "");
	} else {
		lua_pushstring(L, lent->e->luaUse);
	}

	return 1;
}

static int Entity_SetLuaUse(lua_State *L) {
	lent_t	*lent;
	char	*s;

	lent = Lua_GetEntity(L, 1);
	if(!lent || !lent->e)
		return 1;
	s = (char *)luaL_checkstring(L, 2);
	if(!s[0])
		lent->e->luaUse = NULL;
	else
		lent->e->luaUse = G_NewString(s);

	return 1;
}

static const luaL_Reg Entity_ctor[] = {
	{"Spawn", Entity_Spawn},
	{"Find", Entity_Find},
	{"FindNumber", Entity_FindNumber},
	{"FindBModel", Entity_FindBModel},
	{"GetTarget", Entity_GetTarget},
	{"CallSpawn", Entity_CallSpawn},
	{"DelayedCallSpawn", Entity_DelayedCallSpawn },
	{"Remove", Entity_Remove},
	{"RemoveUnnamedSpawns", Entity_RemoveUnnamedSpawns},
	{"RemoveSpawns", Entity_RemoveSpawns},
	{"RemoveType", Entity_RemoveType},
	{"Use", Entity_Use},
	{NULL, NULL}
};

/*void dummy(gentity_t *ent) {
	ent->luaUse;
}*/

static const luaL_Reg Entity_meta[] = {
	{"__gc", Entity_GC},
	{"__tostring", Entity_ToString},
	{"GetNumber", Entity_GetNumber},
	{"IsClient", Entity_IsClient},
	{"GetClientname", Entity_GetClientName},
	{"Print", Entity_Print},
	{"CenterPrint", Entity_CenterPrint},
	{"GetClassname", Entity_GetClassName},
	{"SetClassname", Entity_SetClassName},
	{"GetTargetname", Entity_GetTargetName},
	{"SetTargetname", Entity_SetTargetName},
	{"Rotate", Entity_Rotate},

	{"IsRocket", Entity_IsRocket},
	{"IsGrenade", Entity_IsGrenade},
	{"Teleport", Entity_Teleport},
	{"SetKeyValue", Entity_SetKeyValue},
	{"GetOrigin", Entity_GetOrigin},
	{"SetupTrigger", Entity_SetupTrigger},

	{"Lock", Entity_Lock},
	{"Unlock", Entity_Unlock},
	{"IsLocked", Entity_IsLocked},

	{"GetParm", Entity_GetParm},
	{"SetParm", Entity_SetParm},

	{"GetActivator", Entity_GetActivator},
	{"SetActivator", Entity_SetActivator},

	{"GetAngle", Entity_GetAngle},
	{"SetAngle", Entity_SetAngle},

	{"GetApos1", Entity_GetApos1},
	{"GetApos2", Entity_GetApos2},

	{"SetApos1", Entity_SetApos1},
	{"SetApos2", Entity_SetApos2},

	{"GetBluename", Entity_GetBluename},
	{"SetBluename", Entity_SetBluename},

	{"GetBluesound", Entity_GetBluesound},
	{"SetBluesound", Entity_SetBluesound},

	{"GetBooleanstate", Entity_GetBooleanstate},
	{"SetBooleanstate", Entity_SetBooleanstate},

	{"GetClipmask", Entity_GetClipmask},
	{"SetClipmask", Entity_SetClipmask},

	{"GetCount", Entity_GetCount},
	{"SetCount", Entity_SetCount},

	{"GetDamage", Entity_GetDamage},
	{"SetDamage", Entity_SetDamage},

	{"GetDistance", Entity_GetDistance},
	{"SetDistance", Entity_SetDistance},

	{"GetEnemy", Entity_GetEnemy},
	{"SetEnemy", Entity_SetEnemy},

	{"GetEventTime", Entity_GetEventTime},
	{"SetEventTime", Entity_SetEventTime},

	{"GetFalsename", Entity_GetFalsename},
	{"SetFalsename", Entity_SetFalsename},

	{"GetFalsetarget", Entity_GetFalsetarget},
	{"SetFalsetarget", Entity_SetFalsetarget},

	{"GetFlags", Entity_GetFlags},
	{"SetFlags", Entity_SetFlags},

	{"GetFreeAfterEvent", Entity_GetFreeAfterEvent},
	{"SetFreeAfterEvent", Entity_SetFreeAfterEvent},

	{"GetFreetime", Entity_GetFreetime},

	{"GetGreensound", Entity_GetGreensound},
	{"SetGreensound", Entity_SetGreensound},

	{"GetHealth", Entity_GetHealth},
	{"SetHealth", Entity_SetHealth},

	{"GetInUse", Entity_GetInUse},
	{"SetInUse", Entity_SetInUse},

	{"GetLastEnemy", Entity_GetLastEnemy},
	{"SetLastEnemy", Entity_SetLastEnemy},

	{"GetLuaDie", Entity_GetLuaDie},
	{"SetLuaDie", Entity_SetLuaDie},

	{"GetLuaEntity", Entity_GetLuaEntity},
	{"SetLuaEntity", Entity_SetLuaEntity},

	{"GetLuaFree", Entity_GetLuaFree},
	{"SetLuaFree", Entity_SetLuaFree},

	{"GetLuaHurt", Entity_GetLuaHurt},
	{"SetLuaHurt", Entity_SetLuaHurt},

	{"GetLuaReached", Entity_GetLuaReached},
	{"SetLuaReached", Entity_SetLuaReached},

	{"GetLuaReachedAngular", Entity_GetLuaReachedAngular},
	{"SetLuaReachedAngular", Entity_SetLuaReachedAngular},

	{"GetLuaSpawn", Entity_GetLuaSpawn},
	{"SetLuaSpawn", Entity_SetLuaSpawn},

	{"GetLuaThink", Entity_GetLuaThink},
	{"SetLuaThink", Entity_SetLuaThink},

	{"GetLuaTouch", Entity_GetLuaTouch},
	{"SetLuaTouch", Entity_SetLuaTouch},

	{"GetLuaTrigger", Entity_GetLuaTrigger},
	{"SetLuaTrigger", Entity_SetLuaTrigger},

	{"GetLuaUse", Entity_GetLuaUse},
	{"SetLuaUse", Entity_SetLuaUse},

	{NULL, NULL}
};

int Luaopen_Entity(lua_State * L)
{
	luaL_newmetatable(L, "game.entity");

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_register(L, NULL, Entity_meta);
	luaL_register(L, "entity", Entity_ctor);

	return 1;
}

void Lua_PushEntity(lua_State * L, gentity_t * ent)
{
	lent_t     *lent;

	lent = (lent_t *)lua_newuserdata(L, sizeof(lent_t));

	luaL_getmetatable(L, "game.entity");
	lua_setmetatable(L, -2);

	lent->e = ent;
}

lent_t *Lua_GetEntity(lua_State * L, int argNum)
{
	void           *ud;

	ud = luaL_checkudata(L, argNum, "game.entity");
	luaL_argcheck(L, ud != NULL, argNum, "\'entity\' expected");
	return (lent_t *) ud;
}
#endif
